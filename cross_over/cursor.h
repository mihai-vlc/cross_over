// cursor/mouse related functions


// it will change the cursor if the mouse is near the corners of the object
void my_cursor(int p) {
	switch(p) {
	case 1 : case 3 :
		SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		break;
	case 2 : case 4 :
		SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		break;
	case 5 : case 7 :
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
		break;
	case 6 : case 8 :
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		break;
	}

}


// resizes the object 
void resize_obj(RECT &r, POINT mouse, POINT mouse2, int obj[]) {
	int limit = 20;
	
	if((obj[4] == 1) && (obj[3] == 1)) {
		// top left
		r.left += mouse.x - mouse2.x; 
		r.top += mouse.y - mouse2.y; 

		// we make sure that we have a minimum width and height
		if(r.bottom - r.top < limit)
			r.top = r.bottom - limit;
		if(r.right - r.left < limit)
			r.left = r.right - limit;

	} else if((obj[4] == 2) && (obj[3] == 2)) {
		// top right
		r.top += mouse.y - mouse2.y;
		r.right += mouse.x - mouse2.x; 

		if(r.bottom - r.top < limit)
			r.top = r.bottom - limit;
		if(r.right - r.left < limit)
			r.right = r.left + limit;

	} else if((obj[4] == 3) && (obj[3] == 3)) {
		// bottom right
		r.bottom += mouse.y - mouse2.y; 
		r.right += mouse.x - mouse2.x; 
	
		if(r.bottom - r.top < limit)
			r.bottom = r.top + limit;
		if(r.right - r.left < limit)
			r.right = r.left + limit;

	} else if((obj[4] == 4) && (obj[3] == 4)) {
		// bottom left
		r.bottom += mouse.y - mouse2.y; 
		r.left += mouse.x - mouse2.x;

		if(r.bottom - r.top < limit)
			r.bottom = r.top + limit;
		if(r.right - r.left < limit)
			r.left = r.right - limit;

	} else if((obj[4] == 5) && (obj[3] == 5)) {
		// top border
		r.top += mouse.y - mouse2.y; 

		if(r.bottom - r.top < limit)
			r.top = r.bottom - limit;


	}  else if((obj[4] == 6) && (obj[3] == 6)) {
		// right border
		r.right += mouse.x - mouse2.x; 

		if(r.right - r.left < limit)
			r.right = r.left + limit;


	}  else if((obj[4] == 7) && (obj[3] == 7)) {
		// bottom border
		r.bottom += mouse.y - mouse2.y; 

		if(r.bottom - r.top < limit)
			r.bottom = r.top + limit;


	} else if((obj[4] == 8) && (obj[3] == 8)) {
		// bottom border
		r.left += mouse.x - mouse2.x; 

		if(r.right - r.left < limit)
			r.left = r.right - limit;


	} else if(obj[4] == -1) { // we also move it here if we don't select any of the corners
		r.left += mouse.x - mouse2.x; 
		r.right += mouse.x - mouse2.x; 
		r.top += mouse.y - mouse2.y; 
		r.bottom += mouse.y - mouse2.y; 
	} 



}


