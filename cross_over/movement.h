// functions related to updating the objects coordinates


// moves up and down the object
int moveUpAndDown(RECT &r, RECT limit, int step, int id, RECT obstacles[], double obstaclesInfo[][100], int reset = 0) {
	static double info[100];

	if(reset){
		fill_n(info, 100, 0);
		return 0;
	}
	// 0 means he is going down
	if(info[id] == 0) {
		if(r.bottom + step >= limit.bottom) {
			r.top += limit.bottom - r.bottom;
			r.bottom = limit.bottom;
			info[id] = 1;
		} else {
			r.top += step;
			r.bottom += step;
		}
	} else {
		if(r.top - step <= limit.top) {
			r.bottom -= r.top - limit.top;
			r.top = limit.top;
			info[id] = 0;
		} else {
			r.top -= step;
			r.bottom -= step;
		}
	}

	// check if it collides with any wall
	for(int i = 0; i < 100; i++)
		if(obstaclesInfo[i][CO_TYPE] == 3 && do_rectangles_intersect(r, obstacles[i])) {
			if(!info[id]) {
				r.top = obstacles[i].top - (r.bottom - r.top);
				r.bottom = obstacles[i].top;			
			} else {
				r.bottom = obstacles[i].bottom + (r.bottom - r.top);
				r.top = obstacles[i].bottom;
			}

			info[id] = !info[id];
			break;
		}

	return 0;
}

// moves up and down the object
int moveLeftAndRight(RECT &r, RECT limit, int step, int id, RECT obstacles[], double obstaclesInfo[][100], int reset = 0) {
	static double info[100];
	if(reset){
		fill_n(info, 100, 0);
		return 0;
	}
	// 0 means he is going right
	if(info[id] == 0) {
		if(r.right + step >= limit.right) {
			r.left += limit.right - r.right;
			r.right = limit.right;
			info[id] = 1;
		} else {
			r.left += step;
			r.right += step;
		}
	} else {
		if(r.left - step <= limit.left) {
			r.right -= r.left - limit.left;
			r.left = limit.left;
			info[id] = 0;
		} else {
			r.left -= step;
			r.right -= step;	
		}
	}
	// check if it collides with any wall
	for(int i = 0; i < 100; i++)
		if(obstaclesInfo[i][CO_TYPE] == 3 && do_rectangles_intersect(r, obstacles[i])) {
			if(!info[id]) {
				r.left = obstacles[i].left - (r.right - r.left);
				r.right = obstacles[i].left;			
			} else {
				r.right = obstacles[i].right + (r.right - r.left);
				r.left = obstacles[i].right;
			}

			info[id] = !info[id];
			break;
		}	
	return 0;
}



// moves the object in a circle
int moveInCircle(RECT &r, RECT limit, double step, double radius, int id, RECT obstacles[], double obstaclesInfo[][100], int reset = 0) {
	static double info[100][10] = {};
	// we handle the reset
	if(reset == 1){
		for(int i = 0; i < 100; i++)
			fill_n(info[i], 10, 0);
		return 0;
	}

	step = step/10;

	if(!info[id][0]) {
		info[id][0] = r.left;
		info[id][1] = r.right;
		info[id][2] = r.top;
		info[id][3] = r.bottom;
		info[id][4] = 0; // angle
		info[id][5] = 1; // check if we change direction

	}

	info[id][4] = fmod(info[id][4],360);

	RECT prev = r;
	
	r.left = (long)(info[id][0] + radius * cos(info[id][4]));
	r.right = (long)(info[id][1] + radius * cos(info[id][4]));
	r.top = (long)(info[id][2] + radius * sin(info[id][4]));
	r.bottom = (long)(info[id][3] + radius * sin(info[id][4]));
	
	if(r.left < limit.left) {
		r.right += limit.left - r.left;
		r.left = limit.left;
		info[id][5] = !info[id][5];
	}

	if(r.top < limit.top) {
		r.bottom += limit.top - r.top;
		r.top = limit.top;
		info[id][5] = !info[id][5];
	}
	if(r.right > limit.right) {
		r.left -= r.right - limit.right;
		r.right = limit.right;
		info[id][5] = !info[id][5];
	}
	if(r.bottom > limit.bottom) {
		r.top -= r.bottom - limit.bottom;
		r.bottom = limit.bottom;
		info[id][5] = !info[id][5];
	}
	int h = r.bottom - r.top, w = r.right - r.left;
	// check if it collides with any wall
	for(int i = 0; i < 100; i++)
		if(obstaclesInfo[i][CO_TYPE] == 3 && do_rectangles_intersect(r, obstacles[i])) {
			info[id][5] = !info[id][5];
			// RL
			if( prev.left >= obstacles[i].right ) {
			
				r.right = obstacles[i].right + w; 
				r.left = obstacles[i].right;
			} else
			// BT
			if( prev.top >= obstacles[i].bottom ) {
				
				r.bottom = obstacles[i].bottom + h; 
				r.top = obstacles[i].bottom;
			} else
			// LR
			if( prev.right <= obstacles[i].left ) {
				
				r.left = obstacles[i].left - w; 
				r.right = obstacles[i].left;
			} else
			// TB
			if( prev.bottom <= obstacles[i].top ) {
				
				r.top = obstacles[i].top - h; 
				r.bottom = obstacles[i].top;
			}
			//break;
		}

	if(info[id][5])
		info[id][4] += step;
	else
		info[id][4] -= step;
	return 0;
}



// bouces the object on the walls of the container
int moveBounceOnWalls(RECT &r, RECT limit, int step, int id, RECT obstacles[], double obstaclesInfo[][100], int reset = 0) {
	
	static int info[100][5];
	if(!info[id][0]) {
		info[id][0] = 1; // x
		info[id][1] = 1; // y
		//info[id][2] = 0;
	}
	if(reset) {
		for(int i = 100; i >= 0; i--)
			fill_n(info[i], 5, 1);
		return 0;
	}

	RECT prev = r;

	r.left += step * info[id][0];
	r.top += step * info[id][1];
	r.right += step * info[id][0];
	r.bottom += step * info[id][1];

	int h = r.bottom - r.top, w = r.right - r.left;
	// we handle the container
	// we touch the top
	if(r.top < limit.top) {
		r.bottom = limit.top + h;
		r.top = limit.top;
		info[id][1] = 1;
	}
	// we touch the bottom
	if(r.bottom > limit.bottom) {
		r.top = limit.bottom - h;
		r.bottom = limit.bottom;
		info[id][1] = -1;

	}
	// we touch the left
	if(r.left < limit.left) {
		r.right = limit.left + w;
		r.left = limit.left;
		info[id][0] = 1;
	}
	// we touch the right
	if(r.right > limit.right) {
		r.left = limit.right - w;
		r.right = limit.right;
		info[id][0] = -1;
	}

	// check if it collides with any wall
	for(int i = 0; i < 100; i++)
		if(obstaclesInfo[i][CO_TYPE] == 3 && do_rectangles_intersect(r, obstacles[i])) {
			// RL
			if( prev.left >= obstacles[i].right ) {
				info[id][0] = 1;
				r.right = obstacles[i].right + w; 
				r.left = obstacles[i].right;
			} else
			// BT
			if( prev.top >= obstacles[i].bottom ) {
				info[id][1] = 1;
				r.bottom = obstacles[i].bottom + h; 
				r.top = obstacles[i].bottom;
			} else
			// LR
			if( prev.right <= obstacles[i].left ) {
				info[id][0] = -1;
				r.left = obstacles[i].left - w; 
				r.right = obstacles[i].left;
			} else
			// TB
			if( prev.bottom <= obstacles[i].top ) {
				info[id][1] = -1;
				r.top = obstacles[i].top - h; 
				r.bottom = obstacles[i].top;
			}
			//break;
		}

	return 1;
}

// moves the object to follow the player
void moveFollower(RECT &r, RECT destination, RECT limit, int step) {
	double dx = destination.right - r.right;
	double dy = destination.bottom - r.bottom;

	double angle = atan2(dy,dx);

	r.left += (long)(step * cos(angle));
	r.right += (long)(step * cos(angle));
	r.top += (long)(step * sin(angle));
	r.bottom += (long)(step * sin(angle));
	// if he tryes to move outsite the container we stop it
	if(r.top < limit.top) {
		r.bottom += limit.top - r.top;
		r.top = limit.top;
	}	

	if(r.left < limit.left) {
		r.right += limit.left - r.left;
		r.left = limit.left;
	}
	if(r.right > limit.right) {
		r.left -= r.right - limit.right;
		r.right = limit.right;
	}
	if(r.bottom > limit.bottom) {
		r.top -= r.bottom - limit.bottom;
		r.bottom = limit.bottom;
	}
}



// handeles the movement of the player
void OBJMove(RECT &r1, RECT Container, int move, int &ok) {
 		if(GetAsyncKeyState(VK_LEFT) && !outOfContainer(r1,Container, 0, move)) {
			
			r1.left -= move;
			r1.right -= move;
			ok = 1;
		}
				

		if(GetAsyncKeyState(VK_RIGHT) && !outOfContainer(r1,Container, 1, move)) {
			r1.left += move;
			r1.right += move;
			ok = 2;
		}
				
		if(GetAsyncKeyState(VK_UP) && !outOfContainer(r1,Container, 2, move)) {
			r1.top -= move;
			r1.bottom -= move;
			ok = 3;
		}
				

		if(GetAsyncKeyState(VK_DOWN) && !outOfContainer(r1,Container, 3, move)) {
			r1.top += move;
			r1.bottom += move;
			ok = 4;
		}

}


// change the move type for the selected items
void changeMovement(int hasFrame[][100], int i, int selection, double obstaclesInfo[][100][100], HWND BTN[], int id) {

	char tmp_txt[100];
	if(hasFrame[id][i] == 1) {
		obstaclesInfo[id][i][CO_MOVE] = selection + 1;

		if(obstaclesInfo[id][i][CO_MOVE] == 2){
			EnableWindow(BTN[2], TRUE);
			sprintf_s(tmp_txt, "%d", (int)obstaclesInfo[0][i][CO_RADIUS]);
			SendMessage(BTN[2], WM_SETTEXT, 0, (LPARAM)tmp_txt);
		} else {
			SendMessage(BTN[2], WM_SETTEXT, 0, (LPARAM)"");
			EnableWindow(BTN[2], FALSE);
		}
	}
}

// change the speed type for the selected items
void changeSpeed(HWND BTN[], int sz, double obstaclesInfo[][100][100], int hasFrame[][100], int i, int id) {
	if(hasFrame[id][i] == 1) {
		char tmp_txt[100];
		SendMessage (BTN[1], WM_GETTEXT, sz+1, (LPARAM)&tmp_txt);

		sz = strtol(tmp_txt, NULL, 10);
		if(sz != 0)
			obstaclesInfo[id][i][CO_STEP] = sz;
	}
}

// change the radius type for the selected items
void changeRadius(HWND BTN[], int sz, double obstaclesInfo[][100][100], int hasFrame[][100], int i, int id) {
	if(hasFrame[id][i] == 1) {
		char tmp_txt[100];
		SendMessage (BTN[2], WM_GETTEXT, sz+1, (LPARAM)&tmp_txt);

		sz = strtol(tmp_txt, NULL, 10);
		if(sz != 0)
			obstaclesInfo[id][i][CO_RADIUS] = sz;
	}
}
