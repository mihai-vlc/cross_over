// function related to colision of files


// checks if the next move is outside of the container
bool outOfContainer(RECT &r1, RECT Container, int direction, int move) {
	// left
	if(direction == 0 && (r1.left - move < Container.left)){
		r1.right = Container.left + (r1.right - r1.left);
		r1.left = Container.left;
		return true;
	}
	// right
	if(direction == 1 && (r1.right + move > Container.right)){
		r1.left = Container.right - (r1.right - r1.left);
		r1.right = Container.right;
		return true;
	}

	// up
	if(direction == 2 && (r1.top - move < Container.top)) {
		r1.bottom = Container.top + (r1.bottom - r1.top);
		r1.top = Container.top;
		return true;
	
	}

	// down
	if(direction == 3 && (r1.bottom + move > Container.bottom)) {
		r1.top = Container.bottom - (r1.bottom - r1.top);
		r1.bottom = Container.bottom;
		return true;
	}
	return false;

}

// return 1 if the point is in the provided rectangle
int is_point_in_rectangle(RECT r, int x, int y) {
    if ((r.left   <= x && r.right >= x) &&
        (r.top <= y && r.bottom   >= y))
        return 1;
    return 0;
}

// Returns 1 if the rectangles overlap, 0 otherwise
int do_rectangles_intersect(RECT a, RECT b) {
	if (a.left >= b.right || a.right <= b.left ||
    a.top >= b.bottom || a.bottom <= b.top) 
		return 0;
    return 1;
}


// makes sure that the player doesn't pass the wall
void checkWall(RECT &r, RECT obstacle, int d, RECT p) {

	if(do_rectangles_intersect(r, obstacle)){
		// fix for diagonal movement(it took way more longer then it should to figure it out :( )
		if((d == 3 || d == 2) && (p.right <= obstacle.left))
			d = 1;

		if((d == 3 || d == 2) && (p.left >= obstacle.right))
			d = 0;		

		// right to left
		if(d == 0 && r.left < obstacle.right) {
			r.right += obstacle.right - r.left;
			r.left = obstacle.right;
		} 
		// left to right
		if(d == 1 && r.right > obstacle.left) {
			r.left -= r.right - obstacle.left;
			r.right = obstacle.left;
		} 
		// down to up
		if(d == 2 && r.top < obstacle.bottom) {
			r.bottom += obstacle.bottom - r.top;
			r.top = obstacle.bottom;
		} 
		// up to down
		if(d == 3 && r.bottom > obstacle.top) {
			r.top -= r.bottom - obstacle.top;
			r.bottom = obstacle.top;
		}
	}

}

// check if object is in the container
int is_in_container(RECT  r, RECT Container) {
	
	if(r.left >= Container.left && r.top >= Container.top && r.bottom <= Container.bottom && r.right <= Container.right)
		return 1;
	
	return 0;
}


// returns 1 if the mose is in a hotspot for resizing and 0 if not
int valid_select(RECT r, POINT m, int &p) {
	RECT a = r;
	// we check if it's in top left corner
	a.right = r.left + 8;
	a.bottom = r.top + 8;

	if(is_point_in_rectangle(a, m.x, m.y)) {
		p = 1;
		return 1;
	}

	// we check if it's in the top right
	a = r;

	a.left = r.right - 8;
	a.bottom = r.top + 8;

	if(is_point_in_rectangle(a, m.x, m.y)) {
		p = 2;
		return 1;
	}

	// we check if it's in the bottom right
	a = r;

	a.left = r.right - 8;
	a.top = r.bottom - 8;
	
	if(is_point_in_rectangle(a, m.x, m.y)) {
		p = 3;
		return 1;
	}

	// we check if it's in the bottom left
	a = r;

	a.top = r.bottom - 8;
	a.right = r.left + 8;

	if(is_point_in_rectangle(a, m.x, m.y)) {
		p = 4;
		return 1;
	}

	// we check if it's in the top border
	a = r;

	a.bottom = r.top + 8;

	if(is_point_in_rectangle(a, m.x, m.y)) {
		p = 5;
		return 1;
	}

	// we check if it's in the right border
	a = r;

	a.left = r.right - 8;

	if(is_point_in_rectangle(a, m.x, m.y)) {
		p = 6;
		return 1;
	}
	
	// we check if it's in the bottom border
	a = r;

	a.top = r.bottom - 8;

	if(is_point_in_rectangle(a, m.x, m.y)) {
		p = 7;
		return 1;
	}
	// we check if it's in the left border
	a = r;

	a.right = r.left + 8;

	if(is_point_in_rectangle(a, m.x, m.y)) {
		p = 8;
		return 1;
	}
	return 0;
}


// makes sure that we only have one frame to indicate the selected obstacle
void OnlyOneFrame(int hasFrame[][100], RECT r[], RECT e[], POINT mouse) {

	for(int i=99; i>=0; i--){
		if((hasFrame[0][i] == 1) && !is_point_in_rectangle(r[i], mouse.x, mouse.y)) {
			hasFrame[0][i] = 0;
		}

		if((hasFrame[1][i] == 1) && !is_point_in_rectangle(e[i], mouse.x, mouse.y)) {
			hasFrame[1][i] = 0;
		}
	}
}


// moves the objects outsite of the conainer to their original position 
void DelOutsiders(RECT Container, RECT r[], RECT e[], RECT DefR, RECT DefE, int hasFrame[2][100], double obstaclesInfo[][100][100], int ALL = 0) {
	for( int i = 99; i >= 0; i--) {
		if((ALL && hasFrame[0][i]) || !is_in_container(r[i], Container)){
			hasFrame[0][i] = 0;
			obstaclesInfo[0][i][CO_TYPE] = 1;
			r[i] = DefR;
		}
		if((ALL && hasFrame[1][i]) || !is_in_container(e[i], Container)){
			hasFrame[1][i] = 0;
			obstaclesInfo[1][i][CO_TYPE] = 2;
			e[i] = DefE;
		}
	}
}



// game over
int game_over(HWND hwnd, RECT &r1, RECT &obstacol, RECT DefPos) {
	if(r1.right > obstacol.left && r1.left < obstacol.right && do_rectangles_intersect(r1, obstacol)){
		
		KillTimer(hwnd,1); // stop the action

		MessageBox(hwnd, TEXT(LOSE_TXT), TEXT(LOSE_TXT_TITLE), MB_OK);
		
		SetTimer(hwnd,1,UPDATE_RATE, NULL); 		// start the action
	
		// we place the player to the start position
		r1 = DefPos;
		// if they happen to collide on the start position move the obstacle out of the screen 
		// till it's position is set back to default
		while(do_rectangles_intersect(DefPos, obstacol)) {
			obstacol.left -= 4500;
			obstacol.right -= 4500;

		}

		InvalidateRect(hwnd,NULL,false);
		UpdateWindow(hwnd);
		return 1;
	}
	return 0;
}

// prepares some vars needed in the resize/move action
// in order to avoid the problems at resizing regarding hot spots we only check for them when we click
// the left button of the mouse and mantain that selection till we let go of the mouse
// so we store the hotspot of resizing in obj var
void prepare4Resize(RECT r,int i, int id, POINT mouse, int obj[]) {
	obj[0] = i; // if it's not -1 it means that the mouse button is clicked

	obj[1] = id; // type of the object rectangle or ellipse

	if(valid_select(r, mouse, obj[3])){ // obj[3] holds the place where it was selected

		my_cursor(obj[3]);
		obj[4] = obj[3];		// it was selected for resize			
	}
}


// checks and updates some info on an obstacle. it's used when the left mouse button is clicked
void CheckSelectedObstacle(HWND hwnd, RECT r[],int hasFrame[][100], int LastSelObj[], int &anyFrame, POINT mouse, double obstaclesInfo[][100][100], HWND BTN[], int obj[]) {
	static int k = 0;
	char tmp[10];

	k %= 2; // we keep k in the 0 and 1 values

	for(int i = 99; i >= 0; i--) {
		if(is_point_in_rectangle(r[i], mouse.x, mouse.y)) {
			// we bring the focus on the main window
			SetFocus(hwnd);
			
			anyFrame++;

			hasFrame[k][i] = 1;
			LastSelObj[0] = k; // type
			LastSelObj[1] = i;

			if(!obstaclesInfo[k][i][CO_MOVE]) {// first time when we click the item we add some info about it
				obstaclesInfo[k][i][CO_TYPE] = k+1;					
				obstaclesInfo[k][i][CO_MOVE] = 1;
				obstaclesInfo[k][i][CO_STEP] = 10;
				obstaclesInfo[k][i][CO_RADIUS] = 50;
				obstaclesInfo[k][i][CO_WTYPE] = k+1;
			}
			// type of movement
			SendMessage(BTN[0], CB_SETCURSEL, (WPARAM)obstaclesInfo[k][i][CO_MOVE] - 1, 0);
			// speed/step
			sprintf_s(tmp, "%d", (int)obstaclesInfo[k][i][CO_STEP]);
			SendMessage(BTN[1], WM_SETTEXT, 0, (LPARAM)tmp);

			// radius
			if(obstaclesInfo[k][i][CO_MOVE] == 2) {
				sprintf_s(tmp, "%d", (int)obstaclesInfo[k][i][CO_RADIUS]);
				SendMessage(BTN[2], WM_SETTEXT, 0, (LPARAM)tmp);
			}
			// checkbox
			if(obstaclesInfo[k][i][CO_TYPE] == 3) {
				CheckDlgButton(hwnd, ID_BTN_WALL, BST_CHECKED);
			} else {
				CheckDlgButton(hwnd, ID_BTN_WALL, BST_UNCHECKED);
			}


			prepare4Resize(r[i], i, k, mouse, obj);

			break;
		}
	}
	k++;
}

