// functions related to keyboard keys


void KeyMoveLeft(int hasFrame[][100], RECT r[], int i, int id, int step = 1) {
	if(hasFrame[id][i] == 1){
		r[i].left -= step;
		r[i].right -= step;
	}

}

void KeyMoveRight(int hasFrame[][100], RECT r[], int i, int id, int step = 1) {
	if(hasFrame[id][i] == 1){
		r[i].left += step;
		r[i].right += step;
	}

}

void KeyMoveUp(int hasFrame[][100], RECT r[], int i, int id, int step = 1) {
	if(hasFrame[id][i] == 1){
		r[i].top -= step;
		r[i].bottom -= step;
	}

}

void KeyMoveDown(int hasFrame[][100], RECT r[], int i, int id, int step = 1) {
	if(hasFrame[id][i] == 1){
		r[i].top += step;
		r[i].bottom += step;
	}

}