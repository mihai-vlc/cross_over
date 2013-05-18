/*
*
* Cross Over C++ game
* Author: Mihai Ionut Vilcu
* March-April 2013
*
*/


// todo: miscare din sageti al obstacolului
int LevelMaker(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, int &OPTION, int &TestLevel) {



	HDC hdc;
	static HDC hdcBackBuffer;
	static HBITMAP hBitmap;
	PAINTSTRUCT ps;
	static RECT client = {}, r[100] = {}, e[100] = {};
	static int width, height, obj[10] = {};
	// we set all the values to -1
	if(!obj[9])
		fill_n(obj, 10, -1);

	static HBRUSH color[50] = {};
	static RECT Container = {}, DefR = {}, DefE = {}, Selector = {}, orgSelector = {};
	static int hasFrame[5][100] = {};
	static POINT mouse, mouse2;
	static double obstaclesInfo[2][100][100] = {}; // obstacles info

	static CLIP Clipboard = {};

	static HWND BTN[10];

	static int LastSelObj[10] = {};

	static int offset[4] = {};
	
	offset[0] = 30; // left
	offset[1] = 35; // right
	offset[2] = 80; // top
	offset[3] = 35; // bottom


	int sz;
	char tmp_txt[100] = {};
	
	static int isMultiSelect = 0;

	switch(message) {
	

	case WM_CREATE : {

		GetClientRect (hwnd, &client);	

		width = client.right;
		height = client.bottom;
		
		GenerateColors(color);




		// position of the container
		Container.left = client.left + offset[0];
		Container.right = client.right - offset[1];
		Container.top = client.top + offset[2];
		Container.bottom = client.bottom - offset[3];

		// default position for the objects

		// rectangle
		DefR.left = 50;
		DefR.top = 40;
		DefR.right = 80;
		DefR.bottom = 70;
		
		// ellipse
		DefE.left = 100;
		DefE.top = 40;
		DefE.right = 130;
		DefE.bottom = 70;

		// combo box
		BTN[0] = 	CreateWindow(TEXT("combobox"),  TEXT(" "), WS_VISIBLE | CBS_DROPDOWNLIST | WS_CHILD, 
			client.right - offset[1] - 150, client.top + offset[2] - 27, 
			150, 25, hwnd, (HMENU)ID_COMBO_MOVE, NULL, NULL);
		

		
		// we add the items in the combo box
		 SendMessage(BTN[0], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(UPDOWN));
		 SendMessage(BTN[0], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(CIRCULAR));
		 SendMessage(BTN[0], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(BOUNCE));
		 SendMessage(BTN[0], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(FOLLOWER));
		 SendMessage(BTN[0], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(LEFTRIGHT));

		BTN[1] = 	CreateWindow(TEXT("EDIT"),  TEXT(""),  WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, 
			client.right - offset[1] - 200, client.top + offset[2] - 27, 
			45, 25, hwnd, (HMENU)ID_EDIT_SPEED, NULL, NULL);

		BTN[2] = 	CreateWindow(TEXT("EDIT"),  TEXT(""),  WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | WS_BORDER, 
			client.right - offset[1] - 250, client.top + offset[2] - 27, 
			45, 25, hwnd, (HMENU)ID_EDIT_RADIUS, NULL, NULL);
		
		BTN[3] = 	CreateWindow(TEXT("button"),  TEXT(WALL),  WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
			client.right - offset[1] - 350, client.top + offset[2] - 27, 
			95, 25, hwnd, (HMENU)ID_BTN_WALL, NULL, NULL);

		BTN[4] = CreateWindow(TEXT("button"),  TEXT(BTN_SAVE), WS_VISIBLE | WS_CHILD, 
			client.right - offset[1] - 170, client.bottom - offset[3], 
			80, 25, hwnd, (HMENU)ID_BTN_SAVE, NULL, NULL);

		BTN[5] = 	CreateWindow(TEXT("button"),  TEXT(BACK_TO_GAME), WS_VISIBLE | WS_CHILD, 
			client.left + offset[0], client.bottom - offset[3], 
			150, 25, hwnd, (HMENU)ID_BTN_BACK, NULL, NULL);
		
		BTN[6] = CreateWindow(TEXT("button"),  TEXT(BTN_OPEN), WS_VISIBLE | WS_CHILD, 
			client.right - offset[1] - 260, client.bottom - offset[3], 
			80, 25, hwnd, (HMENU)ID_BTN_OPEN, NULL, NULL);

		BTN[7] = CreateWindow(TEXT("button"),  TEXT(BTN_TEST), WS_VISIBLE | WS_CHILD, 
			client.right - offset[1] - 350, client.bottom - offset[3], 
			80, 25, hwnd, (HMENU)ID_BTN_TEST, NULL, NULL);
		
		BTN[8] = 	CreateWindow(TEXT("button"),  TEXT(BTN_EXIT), WS_VISIBLE | WS_CHILD, 
					client.right - offset[1] - 80, client.bottom - offset[3], 
					80, 25, hwnd, (HMENU)ID_BTN_EXIT, NULL, NULL);

		CreateToolTipForRect(BTN[0], TEXT(TIP_MOVE));
		CreateToolTipForRect(BTN[1], TEXT(TIP_SPEED));
		CreateToolTipForRect(BTN[2], TEXT(TIP_RADIUS));
		CreateToolTipForRect(BTN[3], TEXT(TIP_WALL));

		// by default it is disabled
		EnableWindow(BTN[0], FALSE);
		EnableWindow(BTN[1], FALSE);
		EnableWindow(BTN[2], FALSE);
		EnableWindow(BTN[3], FALSE);
		// Create the back buffer
		hdcBackBuffer = CreateCompatibleDC(NULL);

		// Get the device context
		hdc = GetDC(hwnd);

		// Create a bitmap
		hBitmap = CreateCompatibleBitmap(hdc, width, height);

		// Select the bitmap
		SelectObject(hdcBackBuffer, hBitmap);
		
	
		ReleaseDC(hwnd, hdc);					 
					 
	}


	case WM_PAINT : 
	{
		
		hdc = BeginPaint( hwnd, &ps );
		BitBlt(hdcBackBuffer,0,0,width,height, NULL, NULL, NULL, WHITENESS);

		// add the content here

		DrawText(hdcBackBuffer, GAME_TITLE, -1, &client, DT_CENTER);
		DrawText(hdcBackBuffer, DRAG_N_DROP, -1, &client, DT_CENTER);


		// draw the recatngle
		//SelectObject(hdcBackBuffer, CreateSolidBrush(RGB(0,0,0)));
		FillRect(hdcBackBuffer, &Container, color[0]);
		FrameRect(hdcBackBuffer, &Container, color[5]);


		for(int i = 0; i <= 99; i++) {
			if(!r[i].bottom) {
				r[i] = DefR;
			}
			if(obstaclesInfo[0][i][CO_TYPE] == 3)
				FillRect(hdcBackBuffer, &r[i], color[5]);
			else
				FillRect(hdcBackBuffer, &r[i], color[3]);

			if(hasFrame[0][i] == 1) {
				paintFrame(hdcBackBuffer, r[i], color[4]);
			}

			if(!e[i].bottom) {

				e[i] = DefE;
			}
			if(obstaclesInfo[1][i][CO_TYPE] == 3)		
				SelectObject(hdcBackBuffer, color[5]);
			else
				SelectObject(hdcBackBuffer, color[4]);
			
			My_Ellipse(hdcBackBuffer, e[i]);
			if(hasFrame[1][i] == 1) {
				paintFrame(hdcBackBuffer, e[i], color[3]);
			}

		}


		// selected object info
		int k = LastSelObj[0], m = LastSelObj[1];
		if(hasFrame[k][m] == 1) {
			int x = client.right - offset[1] - 540;
			int y = client.top + offset[2] - 23;
			// we update the coordinates
			if(k == 0 && r[m].left >= offset[0] && r[m].top >= offset[2]){
				
				sprintf_s(tmp_txt, "X: %d Y: %d W: %d H: %d", r[m].left - offset[0],
				r[m].top - offset[2], r[m].right - r[m].left, r[m].bottom - r[m].top);
				TextOut(hdcBackBuffer, x, y, tmp_txt, strlen(tmp_txt));

				if(isMultiSelect) { // if it's multiselect we add a special frame
					paintFrame(hdcBackBuffer, r[m], color[6]);
				}

			} else if(k == 1 && e[m].left >= offset[0] && e[m].top >= offset[2]){
			
				sprintf_s(tmp_txt, "X: %d Y: %d W: %d H: %d", e[m].left - offset[0],
				e[m].top - offset[2], e[m].right - e[m].left, e[m].bottom - e[m].top);
				TextOut(hdcBackBuffer, x, y, tmp_txt, strlen(tmp_txt));
				
				if(isMultiSelect) { // if it's multiselect we add a special frame
					paintFrame(hdcBackBuffer, e[m], color[6]);
				}
			}


		}

		if(Selector.right != 0) {

			FrameRect(hdcBackBuffer, &Selector, color[5]);
		}


		// End of the content here

		// Display the back buffer
		BitBlt(hdc, 0, 0, width, height, hdcBackBuffer, 0, 0, SRCCOPY);
		EndPaint(hwnd,&ps);
		ReleaseDC(hwnd, hdc);
		ReleaseDC(hwnd, hdcBackBuffer);
	}

	case WM_KEYDOWN: {


		RECT* crtObs;
		int k = LastSelObj[0], m = LastSelObj[1], selected = 0;
		if(hasFrame[k][m] == 1) {
			selected = 1;
			if(k == 0)
				crtObs = &r[m];
			else
				crtObs = &e[m];
		}
		int nStep = GetAsyncKeyState(VK_SHIFT) ? 10 : 1;
		
		switch(LOWORD(wParam)) {
			case VK_DELETE : {				

				DelOutsiders(Container, r, e, DefR, DefE, hasFrame, obstaclesInfo, 1);
				DisableAll(hwnd, BTN);

				isMultiSelect = 0;
				InvalidateRect(hwnd, NULL, false);
				UpdateWindow(hwnd);

				break;		 
			}
			case VK_ESCAPE: {
				for(int i = 99; i >= 0; i--) {
					hasFrame[0][i] = 0;
					hasFrame[1][i] = 0;
				}
					
				isMultiSelect = 0;
				DisableAll(hwnd, BTN);
					
				InvalidateRect(hwnd, NULL, false);
				UpdateWindow(hwnd);
				break;				   
			}
			case VK_LEFT: {

				for( int i = 99; i >= 0; i--) {
					KeyMoveLeft(hasFrame, r, i, 0, nStep);
					KeyMoveLeft(hasFrame, e, i, 1, nStep);					
				} 
				InvalidateRect(hwnd, NULL, false);
				UpdateWindow(hwnd);
		
				break;
			}
			case VK_RIGHT: {

				for( int i = 99; i >= 0; i--) {
					KeyMoveRight(hasFrame, r, i, 0, nStep);
					KeyMoveRight(hasFrame, e, i, 1, nStep);					
				} 
				InvalidateRect(hwnd, NULL, false);
				UpdateWindow(hwnd);
		
				break;
			}
			case VK_UP: {

				for( int i = 99; i >= 0; i--) {
					KeyMoveUp(hasFrame, r, i, 0, nStep);
					KeyMoveUp(hasFrame, e, i, 1, nStep);					
				} 
				InvalidateRect(hwnd, NULL, false);
				UpdateWindow(hwnd);
			
				break;
			}
			case VK_DOWN: {
				for( int i = 99; i >= 0; i--) {
					KeyMoveDown(hasFrame, r, i, 0, nStep);
					KeyMoveDown(hasFrame, e, i, 1, nStep);				
				} 
				InvalidateRect(hwnd, NULL, false);
				UpdateWindow(hwnd);			
				break;
			}
			case 0x43 : { // c 
				if(hasFrame[k][m] == 1 && GetAsyncKeyState(VK_CONTROL))
					SendMessage(hwnd, WM_COMMAND, ID_CM_COPY, lParam);
				break;
			}
			case 0x53 : { // s 
				if(GetAsyncKeyState(VK_CONTROL))
					SendMessage(hwnd, WM_COMMAND, ID_BTN_SAVE, lParam);
				break;
			}

			case 0x58 : { // x 
				if(hasFrame[k][m] == 1 && GetAsyncKeyState(VK_CONTROL))
					SendMessage(hwnd, WM_COMMAND, ID_CM_CUT, lParam);
				break;
			}
			case 0x56 : { // v 
				if(GetAsyncKeyState(VK_CONTROL) && (Clipboard.w != 0))
					SendMessage(hwnd, WM_COMMAND, ID_CM_PASTE, lParam);
				break;
			}
			case 0x41 : { // a , select all
				if(GetAsyncKeyState(VK_CONTROL))
					for(int i = 99; i >= 0; i--) {
						if(is_in_container(r[i],Container))
							hasFrame[0][i] = 1;
						if(is_in_container(e[i],Container))
							hasFrame[1][i] = 1;						

					}
				isMultiSelect = 1;
				InvalidateRect(hwnd, NULL, false);
				UpdateWindow(hwnd);
				break;
			}
		}

		break;
	}
		
	case WM_COMMAND : { // here we can handle the buttons
		int k = LastSelObj[0], m = LastSelObj[1];
		switch(LOWORD(wParam)) {
			// context menu delete
			case ID_CM_DEL : {

				SendMessage(hwnd, WM_KEYDOWN, VK_DELETE, lParam);

				break;
			}
			case ID_CM_CUT : {
				RECT* crtObs;
				if(k == 0)
					crtObs = &r[m];
				else 
					crtObs = &e[m];

				Clipboard.type = 0;
				Clipboard.obsType = k;
				Clipboard.h = crtObs->bottom - crtObs->top;
				Clipboard.w = crtObs->right - crtObs->left;
				Clipboard.obsId = m;
				
				crtObs->bottom = 0;
				crtObs->top = 0;
				
				hasFrame[k][m] = 0;
				DisableAll(hwnd, BTN);

				InvalidateRect(hwnd, NULL, false);
				UpdateWindow(hwnd);
				break;
			}
			case ID_CM_COPY : {
				RECT* crtObs;
				if(k == 0)
					crtObs = &r[m];
				else 
					crtObs = &e[m];

				Clipboard.type = 1;
				Clipboard.obsType = k;
				Clipboard.h = crtObs->bottom - crtObs->top;
				Clipboard.w = crtObs->right - crtObs->left;
				Clipboard.obsId = m;
				

				
				break;
			}

			case ID_CM_PASTE :{
				// the mouse pos in saved in `mouse` when we right click
				
				if(!is_point_in_rectangle(Container, mouse.x, mouse.y)) {
					// we make sure we paste the obj on the screen
					mouse.x = (Container.left + Container.right)/2;
					mouse.y = (Container.top + Container.bottom)/2;
				
				}
				RECT* crtObs;

				if(Clipboard.type == 0) { // cut
					if(Clipboard.obsType == 0)
						crtObs = &r[Clipboard.obsId];
					else
						crtObs = &e[Clipboard.obsId];

					if(crtObs->left != 0) {
						// it was already pasted we need a new obstacle
						int i;
						for(i = 99; i >= 0; i--)
							if(Clipboard.obsType == 0 && !is_in_container(r[i], Container)) {
								crtObs = &r[i];
								break;
							} else if(Clipboard.obsType == 1 && !is_in_container(e[i], Container)){
								crtObs = &e[i];
								break;							
							}

						for(int j = 0; j < 100; j++)
							obstaclesInfo[Clipboard.obsType][i][j] = obstaclesInfo[Clipboard.obsType][Clipboard.obsId][j];
						
					}

					crtObs->left = mouse.x;
					crtObs->top = mouse.y;
					crtObs->right = mouse.x + Clipboard.w;
					crtObs->bottom = mouse.y + Clipboard.h;

				} else { // copy
					int i, ok = 0;
					for(i = 99; i >= 0; i--)
						if(Clipboard.obsType == 0 && !is_in_container(r[i], Container)) {
							crtObs = &r[i];
							ok = 1;
							break;
						} else if(Clipboard.obsType == 1 && !is_in_container(e[i], Container)){
							crtObs = &e[i];
							ok = 1;
							break;							
						}
					if(!ok) // if we used all the objects don't do anything
						break;

					for(int j = 0; j < 100; j++)
					obstaclesInfo[Clipboard.obsType][i][j] = obstaclesInfo[Clipboard.obsType][Clipboard.obsId][j];					
					


					crtObs->left = mouse.x;
					crtObs->top = mouse.y;
					crtObs->right = mouse.x + Clipboard.w;
					crtObs->bottom = mouse.y + Clipboard.h;

				}

				InvalidateRect(hwnd, NULL, false);
				UpdateWindow(hwnd);				
				break;
			}
			case ID_BTN_EXIT : {
				if(MessageBox(hwnd, TEXT(ARE_YOU_SURE), TEXT(CONFIRM), MB_OKCANCEL) == IDOK){
					CleanFiles();
					PostQuitMessage (0);  // we exit
				
				}
				break;		   
			} 
			case ID_BTN_TEST : {

				if(!SaveLevelTmp(hwnd, Container, obstaclesInfo, r, e, offset))
					break;

				TestLevel = 1;  // we start the test mode
				OPTION = 0; // back to gameplay

				// clean the buttons
				DestroyButtons(BTN);

				fill_n(hasFrame[0], 100, 0);
				fill_n(hasFrame[1], 100, 0);

				SendMessage(hwnd, WM_CREATE, wParam, lParam); // set it to create the new content 
				SetFocus(hwnd);
				InvalidateRect(hwnd, NULL, false);
				UpdateWindow(hwnd);
				break;		   
			} 
			case ID_BTN_SAVE : {
				SaveLevel(hwnd, Container, obstaclesInfo, r, e, offset);
				break;		   
			} 
			case ID_BTN_OPEN : {
				OpenLevel(hwnd, DefE, DefR, obstaclesInfo, r, e, offset, LastSelObj);
				// we need to set the focus back to the main window.
				SetFocus(hwnd);
				// repaint
				InvalidateRect(hwnd,NULL,false);
				UpdateWindow(hwnd);
				break;		   
			} 
			case ID_BTN_BACK : {
				if(MessageBox(hwnd, TEXT(ARE_YOU_SURE), TEXT(CONFIRM), MB_OKCANCEL) != IDOK)
					break;


				OPTION = 0;
				TestLevel = 0;

				DestroyButtons(BTN);


				fill_n(hasFrame[0], 100, 0);
				fill_n(hasFrame[1], 100, 0);
				//SetTimer(hwnd,1,UPDATE_RATE, NULL); 		// start the action
				SendMessage(hwnd, WM_CREATE, wParam, lParam); // set it to create the new content 
				InvalidateRect(hwnd, NULL, false);
				UpdateWindow(hwnd);
				break;
			}
			case ID_COMBO_MOVE  : {
				switch(HIWORD(wParam)) {
					case CBN_SELCHANGE : {
						int selection = SendMessage (BTN[0],CB_GETCURSEL,NULL,NULL);
						for(int i = 99; i >= 0; i--) {
							changeMovement(hasFrame, i, selection, obstaclesInfo, BTN, 0);
							changeMovement(hasFrame, i, selection, obstaclesInfo, BTN, 1);
						}
						break;
					}
				}  // hiword 
				break;
			} // move

			case ID_EDIT_SPEED : {
				// we get the value of the textbox for the speed
				if(HIWORD(wParam) == EN_CHANGE) {
					sz = SendMessage (BTN[1], WM_GETTEXTLENGTH, 0, 0);
					for(int i = 99; i >= 0; i--) {
						changeSpeed(BTN, sz, obstaclesInfo, hasFrame, i, 0);
						changeSpeed(BTN, sz, obstaclesInfo, hasFrame, i, 1);
					}
					break;
				}
			}
			case ID_EDIT_RADIUS : { // we get the value for the radius
				if(HIWORD(wParam) == EN_CHANGE) {
					sz = SendMessage (BTN[2], WM_GETTEXTLENGTH, 0, 0);
					for(int i = 99; i >= 0; i--) {
						changeRadius(BTN, sz, obstaclesInfo, hasFrame, i, 0);
						changeRadius(BTN, sz, obstaclesInfo, hasFrame, i, 1);
					}

					break;
				}
			}
			case ID_BTN_WALL : {
				if(HIWORD(wParam) == BN_CLICKED) {
					if(SendMessage(BTN[3], BM_GETCHECK, 0, 0) == BST_CHECKED) { 
						EnableWindow(BTN[0], FALSE);
						EnableWindow(BTN[1], FALSE);
						EnableWindow(BTN[2], FALSE);
						SendMessage(BTN[0], CB_SETCURSEL, -1, 0);
						SendMessage(BTN[1], WM_SETTEXT, 0, (LPARAM)"");
						SendMessage(BTN[2], WM_SETTEXT, 0, (LPARAM)"");	

						for(int i = 99; i >= 0; i--) {
							if(hasFrame[0][i] == 1)
								obstaclesInfo[0][i][CO_TYPE] = 3;

							if(hasFrame[1][i] == 1)	
								obstaclesInfo[1][i][CO_TYPE] = 3;

						}
					
					} else {
						for(int i = 99; i >= 0; i--) {
							DisableWall(BTN, obstaclesInfo, hasFrame, i, 0);
							DisableWall(BTN, obstaclesInfo, hasFrame, i, 1);
						}

					}
					SetFocus(hwnd);
				}
				
				InvalidateRect(hwnd,NULL,false);
				UpdateWindow(hwnd);
				break;
			}
		}

		return 0;
    }
	// context menu 
	case WM_RBUTTONDOWN : {

		mouse.x = LOWORD(lParam); 
		mouse.y = HIWORD(lParam);
		RECT* crtObs;
		int selected = 0;

		for(int i = 99; i >= 0; i--) {
			if(is_point_in_rectangle(r[i], mouse.x, mouse.y)){
				selected = 1;
				crtObs = &r[i];
				hasFrame[0][i] = 1;
				LastSelObj[0] = 0; // type
				LastSelObj[1] = i;
				break;
			}
			if(is_point_in_rectangle(e[i], mouse.x, mouse.y)){
				selected = 1;
				crtObs = &e[i];
				hasFrame[1][i] = 1;
				LastSelObj[0] = 1; // type
				LastSelObj[1] = i;
				break;
			}
			
		}
		// check the frames
		if(!isMultiSelect)
			OnlyOneFrame(hasFrame, r, e, mouse);

		InvalidateRect(hwnd,NULL,false);
		UpdateWindow(hwnd);

		if(selected || is_point_in_rectangle(Container, mouse.x, mouse.y)) {


			HMENU hPopupMenu = CreatePopupMenu();

			InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_CM_DEL, TEXT(CM_DEL));
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_CM_CUT, TEXT(CM_CUT));
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_CM_COPY, TEXT(CM_COPY));
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_CM_PASTE, TEXT(CM_PASTE));
			
			// check if we have something in our `clipboard`
			if(Clipboard.w == 0) 
				EnableMenuItem(hPopupMenu, ID_CM_PASTE, MF_GRAYED);

			
			if(!selected) {
				EnableMenuItem(hPopupMenu, ID_CM_CUT, MF_GRAYED);
				EnableMenuItem(hPopupMenu, ID_CM_COPY, MF_GRAYED);
				EnableMenuItem(hPopupMenu, ID_CM_DEL, MF_GRAYED);
			}

			if(isMultiSelect){
				EnableMenuItem(hPopupMenu, ID_CM_CUT, MF_GRAYED);
				EnableMenuItem(hPopupMenu, ID_CM_COPY, MF_GRAYED);
			}

			SetForegroundWindow(hwnd);

			POINT p;
			GetCursorPos(&p);// we use this for context menu popups
		
			TrackPopupMenu(hPopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, p.x, p.y, 0, hwnd, NULL);
						  
		}

		break;
	}

	case WM_LBUTTONDOWN: { // if we click the left mouse button
		int anyFrame = 0;

		mouse.x = LOWORD(lParam); 
		mouse.y = HIWORD(lParam);

		if(isMultiSelect && is_point_in_rectangle(Container, mouse.x, mouse.y)) {
			int inR = 0;

			for(int i = 99; i >= 0; i--) // we make sure that the click was made inside one of the selected RECT
				if(hasFrame[0][i] && is_point_in_rectangle(r[i], mouse.x, mouse.y)) {
					isMultiSelect = 2;
					
					prepare4Resize(r[i], i, 0, mouse, obj);
					
					//break; // from the for loop
				} else if(hasFrame[1][i] && is_point_in_rectangle(e[i], mouse.x, mouse.y)) {
					isMultiSelect = 2;

					prepare4Resize(e[i], i, 0, mouse, obj);
					
					// break; // from the for loop				
				} else if(is_point_in_rectangle(r[i], mouse.x, mouse.y)) {
					inR = 1;
					if(GetAsyncKeyState(VK_CONTROL)&0x8000) {
						LastSelObj[0] = 0;
						LastSelObj[1] = i;
						hasFrame[0][i] = 1;
						isMultiSelect = 2;
					}

				} else if(is_point_in_rectangle(e[i], mouse.x, mouse.y)) {
					inR = 1;
					if(GetAsyncKeyState(VK_CONTROL)&0x8000) {
						LastSelObj[0] = 1;
						LastSelObj[1] = i;
						hasFrame[1][i] = 1;
						isMultiSelect = 2;
					}				
				}
			
			if(isMultiSelect != 2) { // if we click outside we remove all the frames
				SendMessage(hwnd, WM_KEYDOWN, VK_ESCAPE, lParam);			
				if(!inR) {
					Selector.left = Selector.right = mouse.x;
					Selector.top = Selector.bottom = mouse.y;
					orgSelector = Selector;			
				}
			} else
				break;
	
		} else if(!is_point_in_rectangle(Container, mouse.x, mouse.y)){ // remove all the frames
			SendMessage(hwnd, WM_KEYDOWN, VK_ESCAPE, lParam);
		}

		CheckSelectedObstacle(hwnd, r, hasFrame, LastSelObj, anyFrame, mouse, obstaclesInfo, BTN, obj);
		CheckSelectedObstacle(hwnd, e, hasFrame, LastSelObj, anyFrame, mouse, obstaclesInfo, BTN, obj);



		// we make sure that only the current object has the frame of selection if we 
		if(!(GetAsyncKeyState(VK_CONTROL)&0x8000)){
			OnlyOneFrame(hasFrame, r, e, mouse);
			isMultiSelect = 0;
		}else
			isMultiSelect = 1;
		// if we click in a rectangle we enable the select options else we disable them
		if(!anyFrame) {
			DisableAll(hwnd, BTN);
			
			Selector.left = mouse.x;
			Selector.top = mouse.y;
			Selector.right = mouse.x;
			Selector.bottom = mouse.y;

			orgSelector = Selector;
		
		} else {
			EnableWindow(BTN[3], TRUE);
			if(obstaclesInfo[LastSelObj[0]][LastSelObj[1]][CO_TYPE] != 3) {
				EnableWindow(BTN[0], TRUE);
				EnableWindow(BTN[1], TRUE);

				// we also handle the radius textbox on the ES_CHNAGE
				if(obstaclesInfo[LastSelObj[0]][LastSelObj[1]][CO_MOVE] != 2) {
					EnableWindow(BTN[2], FALSE);
					SendMessage(BTN[2], WM_SETTEXT, 0, (LPARAM)"");
				} else {
					EnableWindow(BTN[2], TRUE);
				}
			} else {
				SendMessage(BTN[0], CB_SETCURSEL, -1, 0);
				SendMessage(BTN[1], WM_SETTEXT, 0, (LPARAM)"");
				SendMessage(BTN[2], WM_SETTEXT, 0, (LPARAM)"");				
			}
		}
		InvalidateRect(hwnd,NULL,false);
		UpdateWindow(hwnd);
		break;
    }

	case WM_MOUSEMOVE : {
		mouse2 = mouse;
		mouse.x = LOWORD(lParam); 
		mouse.y = HIWORD(lParam);	

		if(isMultiSelect == 2 || isMultiSelect == 4) {
			isMultiSelect = 4; // we are still in resize/move
			for(int i = 99; i >= 0; i--) {	
				
				if(hasFrame[0][i] == 1)
					resize_obj(r[i], mouse, mouse2, obj);
					

				if(hasFrame[1][i] == 1) 
					resize_obj(e[i], mouse, mouse2, obj);
					
				
			}


			InvalidateRect(hwnd,NULL,false);
			UpdateWindow(hwnd);
			break;
		}
		// we change the cursor if it's in the resize position
		for(int i=99; i>=0; i--){
			if(valid_select(r[i], mouse, obj[3]) || valid_select(e[i], mouse, obj[3])) {
				my_cursor(obj[3]);
				break;
			}
		}

		if(obj[0] != -1) {
			
			if(obj[1] == 0) {
				resize_obj(r[obj[0]], mouse, mouse2, obj);
			} else {
				resize_obj(e[obj[0]], mouse, mouse2, obj);
			}

			InvalidateRect(hwnd,NULL,false);
			UpdateWindow(hwnd);
		} else if(Selector.right != 0) { // the selection rectangle


			// we update the selection rectangle

			if(mouse.y > orgSelector.top && mouse.x < orgSelector.left) {

				Selector.left = mouse.x;
				Selector.bottom = mouse.y;

			} else 	if(mouse.y < orgSelector.top && mouse.x > orgSelector.left) {
			
				Selector.right = mouse.x;
				Selector.top = mouse.y;				
			
			} else if(mouse.x > orgSelector.left) {

				Selector.right = mouse.x;
				Selector.bottom = mouse.y;

			} else if(mouse.x < orgSelector.left){

			
				Selector.left = mouse.x;
				Selector.top = mouse.y;

			}

			InvalidateRect(hwnd,NULL,false);
			UpdateWindow(hwnd);
		}


		break;				
	}
    
	case WM_LBUTTONUP : {

		if(Selector.right != 0) {

			// check for selected items
			for(int i = 99; i >= 0; i--) {
				if(do_rectangles_intersect(Selector, r[i])) {
					hasFrame[0][i] = 1;
					// in the selected group we make sure that we have a `LastSelObj`
					LastSelObj[0] = 0;
					LastSelObj[1] = i;
					
					isMultiSelect = 1;
				} 
				if(do_rectangles_intersect(Selector, e[i])) {
					hasFrame[1][i] = 1;
					
					LastSelObj[0] = 1;
					LastSelObj[1] = i;

					isMultiSelect = 1;
				} 
			}

			Selector.left = 0;
			Selector.right = 0;

			if(isMultiSelect == 1) {
				EnableWindow(BTN[0], TRUE);
				EnableWindow(BTN[1], TRUE);
				EnableWindow(BTN[2], TRUE);
				EnableWindow(BTN[3], TRUE);


				SendMessage(BTN[0], CB_SETCURSEL, -1, 0);
				SendMessage(BTN[1], WM_SETTEXT, 0, (LPARAM)"");
				SendMessage(BTN[2], WM_SETTEXT, 0, (LPARAM)"");								
			}

		}

		if(isMultiSelect == 2) {
			
			// if we click on an object without dragging it while multiselect we consider it as a normal click
			int anyFrame = 0;
			isMultiSelect = 0;
			SendMessage(hwnd, WM_LBUTTONDOWN, wParam, lParam);
			SendMessage(hwnd, WM_LBUTTONUP, wParam, lParam);

		}
		if(isMultiSelect == 4) // if we just moved them we keep them selected
			isMultiSelect = 1;

		// if the object is out of the container move it to it's default position
		DelOutsiders(Container, r, e, DefR, DefE, hasFrame, obstaclesInfo);


		// we reset the vars for single select
		obj[0] = -1;
		obj[3] = -1;
		obj[4] = -1;
		


		InvalidateRect(hwnd,NULL,false);
		UpdateWindow(hwnd);
		break;
	}

	case WM_DESTROY :
		CleanFiles();
		PostQuitMessage (0);       
		return 0;		
					  
	
	}

	return 0;

}