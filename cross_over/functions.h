/*
*
* Cross Over C++ game
* Author: Mihai Ionut Vilcu
* March-April 2013
*
*/
using namespace std;

// general use functions


// generates a random numer between the limits
int GetRandomNumber(int nLow, int nHigh)
{	
	 srand( (unsigned)time( NULL ) );
    return (rand() % (nHigh - nLow + 1)) + nLow;
}

// splits the string an array based on the second argument as a delimitator
vector <string> explode(const string& str, const char& ch) {
    string next;
    vector<string> result;

    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
         result.push_back(next);
    return result;
}

// removes the extra chars from a string
string trim(const string& str, const string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

// returns the double value from a 
double strtodbl(string String) { //can be modified to make better, but this should work for simple stuff
	return ::atof(String.c_str());
}


// makes a simple ellipse
void My_Ellipse(HDC hdc, RECT obstacle) {
	Ellipse(hdc, obstacle.left, obstacle.top, obstacle.right, obstacle.bottom);

}

// generates the gate where the player is supposed to go
void generateGate(int offset, RECT &Gate, RECT Container, int GateInfo[]) {
		
		Gate.top = GetRandomNumber(offset,Container.bottom);
		Gate.left = Container.right - GateInfo[0];
		Gate.right = Container.right;
		if(Gate.top + GateInfo[1] > Container.bottom) {
			Gate.top -= Gate.top + GateInfo[1] - Container.bottom;
			Gate.bottom = Container.bottom; 
		} else {
			Gate.bottom = Gate.top + GateInfo[1];
		}

}



// creates a tooltip for a window
void CreateToolTipForRect(HWND hwndParent, LPSTR strText)
{
    // Create a tooltip.
    HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, 
                                 WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, 
                                 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                                 hwndParent, NULL, NULL,NULL);

    SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0, 
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    // Set up "tool" information. In this case, the "tool" is the entire parent window.
    
    TOOLINFO ti = { 0 };
    ti.cbSize   = sizeof(TOOLINFO);
    ti.uFlags   = TTF_SUBCLASS;
    ti.hwnd     = hwndParent;
    ti.hinst    = NULL;
    ti.lpszText = strText;
    
    GetClientRect (hwndParent, &ti.rect);

    // Associate the tooltip with the "tool" window.
    SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);	
} 



// disable the wall property for the selected item
void DisableWall(HWND BTN[], double obstaclesInfo[][100][100], int hasFrame[][100], int i, int id) {
	if(hasFrame[id][i] == 1) {	
		char tmp_txt[100];
		obstaclesInfo[id][i][CO_TYPE] = id+1;
		
		EnableWindow(BTN[0], TRUE);
		EnableWindow(BTN[1], TRUE);
						
		// update the options menu
		SendMessage(BTN[0], CB_SETCURSEL, (WPARAM)obstaclesInfo[id][i][CO_MOVE] - 1, 0);
		// step/speed
		sprintf_s(tmp_txt, "%d", (int)obstaclesInfo[id][i][CO_STEP]);
		SendMessage(BTN[1], WM_SETTEXT, 0, (LPARAM)tmp_txt);

		// radius
		if(obstaclesInfo[id][i][CO_MOVE] == 2) {
			EnableWindow(BTN[2], TRUE);
			sprintf_s(tmp_txt, "%d", (int)obstaclesInfo[id][i][CO_RADIUS]);
			SendMessage(BTN[2], WM_SETTEXT, 0, (LPARAM)tmp_txt);
		}
	}
}



// generate some basic colors
void GenerateColors(HBRUSH color[]) {
	// colors
	color[0] = CreateSolidBrush (RGB (240, 240, 240)); // grey-ish container

	color[1] = CreateSolidBrush (RGB (140, 0, 140)); // player purple

	color[2] = CreateSolidBrush(RGB(0,222,52)); // green

	color[3] = CreateSolidBrush(RGB(0,0,255)); // blue
		
	color[4] = CreateSolidBrush(RGB(255,0,0)); // red

	color[5] = CreateSolidBrush(RGB(0,0,0)); // black

	color[6] = CreateSolidBrush(RGB(0,255,0)); //Fuchsia

}

// paints the frame on the selected item
void paintFrame(HDC hdc, RECT r, HBRUSH color) {
	r.left -= 1;
	r.top -= 1;
	r.right += 1;
	r.bottom += 1;

	FrameRect(hdc, &r, color);
}

