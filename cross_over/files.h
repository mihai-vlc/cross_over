// functions related to file handling

// generates the text with object properties used in the saveLevel function
string GenerateObstacleText(RECT r, double obstaclesInfo[], int offset[]){
	string result = "";

	if(obstaclesInfo[CO_TYPE] == 1)
		result.append("Rectangle(");
	else if(obstaclesInfo[CO_TYPE] == 2)
		result.append("Ellipse(");
	else if(obstaclesInfo[CO_TYPE] == 3)
		result.append("Wall(");

	char tmp[100];
	sprintf_s(tmp, "%d,%d,%d,%d,", r.left - offset[0], r.top - offset[2], r.right - r.left, r.bottom - r.top);
	result.append(tmp);

	if(obstaclesInfo[CO_TYPE] != 3) {
		if(obstaclesInfo[CO_MOVE] == 2) {
			sprintf_s(tmp, "%d|%d,%d", (int)obstaclesInfo[CO_MOVE], (int)obstaclesInfo[CO_RADIUS], (int)obstaclesInfo[CO_STEP]);
		}else {
			sprintf_s(tmp, "%d,%d", (int)obstaclesInfo[CO_MOVE], (int)obstaclesInfo[CO_STEP]);		
		}

	}else {
		// this may need to be updated if future obstacles are added
		sprintf_s(tmp, "%d", (int)obstaclesInfo[CO_WTYPE]);	
	}
	result.append(tmp);
	result.append(")\r\n");
	return result;
}



// open the popup and select a txt file
BOOL OpenLevel(HWND hwnd, RECT DefE, RECT DefR, double obstaclesInfo[][100][100], RECT r[], RECT e[], int offset[], int LastSelObj[]) {

	// when we open a level in level creator
	// Fill the OPENFILENAME structure
	TCHAR szFilters[] = TEXT("Scribble Files (*.txt)\0*.txt\0\0");
	TCHAR szFilePathName[_MAX_PATH] = TEXT("");
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = szFilters;
	ofn.lpstrFile = szFilePathName;  // This will be the file name
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = TEXT("Open File");
	ofn.Flags = OFN_FILEMUSTEXIST;

	// Bring up the dialog, and choose the file
	GetOpenFileName(&ofn);


	// we read the level file
	ifstream hFile(ofn.lpstrFile);

	if(!hFile) 
		return FALSE;

	// we first clean the current level

	for( int k = 99; k >= 0; k-- ) {
		r[k] = DefR;
		e[k] = DefE;
		fill_n(obstaclesInfo[0][k], 10, 0);
		fill_n(obstaclesInfo[1][k], 10, 0);

	}


	string line;
	int i = 99, j = 0;
	vector <string> objName, objInfo, objInfo2;
	double extra = 0;

	// read the lines
	while(getline(hFile, line) && i >= 0){
		j = 0;
		
		objName = explode(line, '(');

		if(trim(objName[0]) == "Rectangle") 
			obstaclesInfo[0][i][CO_TYPE] = 1;
		else if(trim(objName[0]) == "Ellipse"){ 
			obstaclesInfo[1][i][CO_TYPE] = 2;
			j = 1;
		}else if(trim(objName[0]) == "Wall"){ 
			objInfo = explode(objName[1], ',');
			if(strtodbl(trim(objInfo[4])) == 2) // determine if it's a rectangle or an ellipse
				j = 1;

			obstaclesInfo[j][i][CO_TYPE] = 3;

		} else
			continue;

		LastSelObj[0] = j;
		LastSelObj[1] = i;

		objInfo = explode(objName[1], ',');
		if(j == 0) {
			r[i].left = (long)(offset[0] + strtodbl(trim(objInfo[0])));
			r[i].top = (long)(offset[2] + strtodbl(trim(objInfo[1])));
			r[i].right = (long)(r[i].left + strtodbl(trim(objInfo[2])));
			r[i].bottom = (long)(r[i].top + strtodbl(trim(objInfo[3])));
		} else {
			e[i].left = (long)(offset[0] + strtodbl(trim(objInfo[0])));
			e[i].top = (long)(offset[2] + strtodbl(trim(objInfo[1])));
			e[i].right = (long)(e[i].left + strtodbl(trim(objInfo[2])));
			e[i].bottom = (long)(e[i].top + strtodbl(trim(objInfo[3])));		
		}		


		if(objInfo[4].find_first_of("|") != string::npos) { // if it's a circle get the radius
			objInfo2 = explode(objInfo[4], '|');
			obstaclesInfo[j][i][CO_MOVE] = strtodbl(trim(objInfo2[0]));
			extra = obstaclesInfo[j][i][CO_RADIUS] = strtodbl(trim(objInfo2[1]));
		} else{
			obstaclesInfo[j][i][CO_MOVE] = strtodbl(trim(objInfo[4]));	
		}
		

		// if it's a wall we have no movement therefor the step is not required
		if(obstaclesInfo[j][i][CO_TYPE] == 3) {
			
			obstaclesInfo[j][i][CO_STEP] = 0;
			i--;
			continue;
		}

		obstaclesInfo[j][i][CO_STEP] = strtodbl(trim(objInfo[5], " \t)")); // the step			

		i--;

	}

	return TRUE;
}


// saves a tmp file for testing it is deleted when the app is closed
// while the app is running the file is located in the CO_LEVELS folder
BOOL SaveLevelTmp(HWND hwnd, RECT Container, double obstaclesInfo[][100][100], RECT r[], RECT e[], int offset[]) {
	
	
	
	// we make sure we are in the right folder
	SetCurrentDirectory(DEFAULT_DIR_PATH);


	int hasObstacles = 0;
	int TextLength = 1;
	string pszText;

	for(int i = 99; i >= 0; i--) {
			
		if(is_in_container(r[i], Container)) {
			hasObstacles++;
			pszText.append(GenerateObstacleText(r[i], obstaclesInfo[0][i], offset));
		}
		if(is_in_container(e[i], Container)) {
			hasObstacles++;
			pszText.append(GenerateObstacleText(e[i], obstaclesInfo[1][i], offset));
		}

	}
	if(hasObstacles == 0) {
		MessageBox(hwnd, TEXT(MSG_NO_OBSTACLE), TEXT(MSG_ERROR), MB_OK);
		return FALSE;
	}


	char szFileName[100] = "CO_LEVELS/tmp.txt";

        HANDLE hFile;
		hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                                           FILE_ATTRIBUTE_NORMAL, NULL);


	// Write contents of text output box to this file
	// Double check that file was created
	if(hFile != INVALID_HANDLE_VALUE) {
		
		TextLength = pszText.size();

		// Do nothing if container is empty
		if(TextLength > 0) {
		
			DWORD dwWritten;
			if(!WriteFile(hFile, pszText.c_str(), TextLength, &dwWritten, NULL))
				MessageBox(hwnd,TEXT(MSG_ERROR),TEXT(MSG_ERROR),MB_OK);
				
		}
		
		CloseHandle(hFile);

		return TRUE;
	} else {
		MessageBox(hwnd,TEXT(MSG_ERROR_WRITE),TEXT(MSG_ERROR),MB_OK);
		return FALSE;
	}

   return FALSE;


}


// clean the tmp files
void CleanFiles(int onlyProgress = 0) {
	
	// we make sure we are in the right folder
	SetCurrentDirectory(DEFAULT_DIR_PATH);

	if(onlyProgress)
		DeleteFile("info.dat");
	else
		DeleteFile("CO_LEVELS/tmp.txt");

}

// save the progress of the gameplay
BOOL SaveProgress(HWND hwnd, int Level, int Deaths) {
	// we make sure we are in the right folder
	SetCurrentDirectory(DEFAULT_DIR_PATH);

	string pszText;
	int TextLength = 1;
	char tmp[100];

	sprintf_s(tmp, "%d|%d", Level, Deaths);
	
	pszText.append(tmp);

	char szFileName[100] = "info.dat";

        HANDLE hFile;
		hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                                           FILE_ATTRIBUTE_NORMAL, NULL);


	// Write contents of text output box to this file
	// Double check that file was created
	if(hFile != INVALID_HANDLE_VALUE) {
		
		TextLength = pszText.size();

		// Do nothing if container is empty
		if(TextLength > 0) {
		
			DWORD dwWritten;
			if(!WriteFile(hFile, pszText.c_str(), TextLength, &dwWritten, NULL))
				MessageBox(hwnd,TEXT(MSG_ERROR),TEXT(MSG_ERROR),MB_OK);
				
		}
		
		CloseHandle(hFile);
		MessageBox(hwnd,TEXT(MSG_PROGRESS_SAVED),TEXT(MSG_SUCCESS),MB_OK);
		return TRUE;
	} else {
		MessageBox(hwnd,TEXT(MSG_ERROR_WRITE),TEXT(MSG_ERROR),MB_OK);
		return FALSE;
	}

   return FALSE;

}

// grab the saved progress
void GetSavedProgress( int &Level, int &Deaths) {

	// we make sure we are in the right folder
	SetCurrentDirectory(DEFAULT_DIR_PATH);	
	// we read the level file
	ifstream hFile("info.dat");
	
	if(!hFile)
		return;

	vector <string> txt;
	string line;

	// read the lines
	while(getline(hFile, line)){
		if(line.find_first_of("|") != string::npos) {
			txt = explode(line, '|');
			Level = (int)strtodbl(txt[0]);
			Deaths = (int)strtodbl(txt[1]);
			break;
		}

	}
}


// check if dir exists
bool dirExists(const std::string& dirName_in)
{
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!

  return false;    // this is not a directory!
}

// checks if a file exists
BOOL fileExists(LPCTSTR szPath)
{
  DWORD dwAttrib = GetFileAttributes(szPath);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

// open the save popup and save the file
BOOL SaveLevel(HWND hwnd, RECT Container, double obstaclesInfo[][100][100], RECT r[], RECT e[], int offset[]) {
	
	// we make sure we are in the right folder
	SetCurrentDirectory(DEFAULT_DIR_PATH);

	int hasObstacles = 0;
	int TextLength = 1;
	string pszText;

	for(int i = 99; i >= 0; i--) {
			
		if(is_in_container(r[i], Container)) {
			hasObstacles++;
			pszText.append(GenerateObstacleText(r[i], obstaclesInfo[0][i], offset));
		}
		if(is_in_container(e[i], Container)) {
			hasObstacles++;
			pszText.append(GenerateObstacleText(e[i], obstaclesInfo[1][i], offset));
		}

	}
	if(hasObstacles == 0) {
		MessageBox(hwnd, TEXT(MSG_NO_OBSTACLE), TEXT(MSG_ERROR), MB_OK);
		return FALSE;
	}

  // popup dialog to get filename to save to
   OPENFILENAME ofn;
   char szFileName[100] = "level1.txt";

   ZeroMemory(&ofn, sizeof(ofn));

   ofn.lStructSize = sizeof(ofn);
   ofn.hwndOwner = hwnd;
   ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
   ofn.lpstrFile = szFileName;
   ofn.nMaxFile = 100;
   ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
   ofn.lpstrDefExt = "txt";

   if (GetSaveFileName(&ofn)) {
	    // We've got a file name.  See if it exists
            HANDLE hFile;
	    hFile = CreateFile(ofn.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	    // First handle the case where the file already exists
	    if (hFile == INVALID_HANDLE_VALUE) {
		// Ask permission to overwrite
		if (MessageBox(hwnd, TEXT(MSG_OVERWRITE), TEXT(CONFIRM),
                               MB_OKCANCEL) == IDOK)
			hFile = CreateFile(ofn.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                                           FILE_ATTRIBUTE_NORMAL, NULL);

		// If permission denied, get out of here
		else {
			CloseHandle(hFile);
			return FALSE;
		}
	}

	// If we're still here, file should have been created

	// Write contents of text output box to this file
	// Double check that file was created
	if(hFile != INVALID_HANDLE_VALUE) {
		
		TextLength = pszText.size();

		// Do nothing if container is empty
		if(TextLength > 0) {
		
			DWORD dwWritten;
			if(!WriteFile(hFile, pszText.c_str(), TextLength, &dwWritten, NULL))
				MessageBox(hwnd,TEXT(MSG_ERROR),TEXT(MSG_ERROR),MB_OK);
				
		}
		
		CloseHandle(hFile);

		return TRUE;
	} else {
		MessageBox(hwnd,TEXT(MSG_ERROR_WRITE),TEXT(MSG_ERROR),MB_OK);
		return FALSE;
	}

   } // Else no file selected
   return FALSE;
}


// generate obstacles

void generate_obstacles(RECT obstacle[], double obstaclesInfo[100][100], int Level, int offset[], RECT DefPos, int Test = 0) {
	// we first clear the obstacles
	RECT a = {};
	for(int i = 0; i < 100; i++) {
		obstacle[i] = a;
		fill_n(obstaclesInfo[i], 5, 0);
	}
	// we also clear the static arrays in the move functions
	moveInCircle(obstacle[0], obstacle[0], 1, 1, 1, obstacle, obstaclesInfo, 1); // we use some dump values who are not used anyway
	
	moveUpAndDown(obstacle[0], obstacle[0], 1, 1, obstacle, obstaclesInfo, 1);
	moveLeftAndRight(obstacle[0], obstacle[0], 1, 1, obstacle, obstaclesInfo, 1);
	moveBounceOnWalls(obstacle[0], obstacle[0], 1, 1, obstacle, obstaclesInfo, 1);



	// we make sure we are in the right folder
	SetCurrentDirectory(DEFAULT_DIR_PATH);

	if(!dirExists("CO_LEVELS") || !fileExists("CO_LEVELS/level1.txt")) {
		// if we have no leves or no level1
		_mkdir("CO_LEVELS");
		HANDLE hFile; 

		DWORD wmWritten; 
		
		char strData[] = "Rectangle (200,100,20,40,1,10)\r\nEllipse(150,150,50,50,2|30,5)\r\n";
		
		hFile = CreateFile("CO_LEVELS/level1.txt",GENERIC_READ|GENERIC_WRITE, 
			FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); 
		
		WriteFile(hFile,strData,(DWORD)(sizeof(strData)),&wmWritten,NULL); 
		
		CloseHandle(hFile);
		
	}

	char filename[100];
	sprintf_s(filename, "CO_LEVELS/level%d.txt", Level);

	if(Test == 1)
		sprintf_s(filename, "CO_LEVELS/tmp.txt");
	
	// we read the level file
	ifstream hFile(filename);
	
	if(!hFile)
		return;


	string line;
	int i = 0;
	vector <string> objName, objInfo, objInfo2;
	double extra = 0;

	// read the lines
	while(getline(hFile, line)){
		
		
		objName = explode(line, '(');

		if(trim(objName[0]) == "Rectangle") 
			obstaclesInfo[i][CO_TYPE] = 1;
		else if(trim(objName[0]) == "Ellipse") 
			obstaclesInfo[i][CO_TYPE] = 2;
		else if(trim(objName[0]) == "Wall") 
			obstaclesInfo[i][CO_TYPE] = 3;
		else
			continue;

		
		objInfo = explode(objName[1], ',');
		obstacle[i].left = (long)(offset[0] + strtodbl(trim(objInfo[0])));
		obstacle[i].top = (long)(offset[2] + strtodbl(trim(objInfo[1])));
		obstacle[i].right = (long)(obstacle[i].left + strtodbl(trim(objInfo[2])));
		obstacle[i].bottom = (long)(obstacle[i].top + strtodbl(trim(objInfo[3])));
				


		if(objInfo[4].find_first_of("|") != string::npos) { // if it's a circle get the radius
			objInfo2 = explode(objInfo[4], '|');
			obstaclesInfo[i][CO_MOVE] = strtodbl(trim(objInfo2[0]));
			extra = obstaclesInfo[i][CO_RADIUS] = strtodbl(trim(objInfo2[1]));
		} else{
			obstaclesInfo[i][CO_MOVE] = strtodbl(trim(objInfo[4]));	
		}
		

		// check if the rectangles ovelap when drown we don't want that
		while(do_rectangles_intersect(DefPos, obstacle[i])) {
			obstacle[i].left += (long)(45 + extra);
			obstacle[i].right += (long)(45 + extra);

		}

		// if it's a wall we have no movement therefor the step is not required
		if(obstaclesInfo[i][CO_TYPE] == 3) {
			obstaclesInfo[i][CO_STEP] = 0;
			i++;
			continue;
		}

		obstaclesInfo[i][CO_STEP] = strtodbl(trim(objInfo[5], " \t)")); // the step			

		i++;

	}


}
