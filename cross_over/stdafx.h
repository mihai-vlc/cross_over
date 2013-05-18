/*
*
* Cross Over C++ game
* Author: Mihai Ionut Vilcu
* March-April 2013
*
*/
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <direct.h>
#include <vector>
#include <algorithm> 
#include <fstream>
#include <sstream>
#include <CommCtrl.h>
#include "resource.h"


#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment (lib, "Comctl32.lib")


#define WINDOW_WIDTH 840
#define WINDOW_HEIGHT 480

#define UPDATE_RATE 50

#define GAME_TITLE "Cross Over"
#define OBIECTIV "\n The onjective of the game is to get to the green gate."
#define LEVEL_TXT "\n\n\n        LEVEL %d"
#define DEATHS "\n\n\nDEATHS %d       "
#define WIN_TXT "CONGRATULATIONS !\n\n Press ok to get to the next level !"
#define WIN_TXT_TITLE "Congratulation"
#define LOSE_TXT "Try again !"
#define LOSE_TXT_TITLE "C'est la vie..."
#define FINISH_GAME "CONGRATULATIONS !\n\nThe game is over !\n\nAuthor: Mihai Ionut Vilcu\n\nMarch-April 2013"
#define BTN_CREATE_LEVEL "Create Level"
#define BTN_EXIT "Exit"
#define ARE_YOU_SURE "Are you sure ?"
#define CONFIRM "Confirm"
#define DRAG_N_DROP "\nDrag and drop the objects in the play zone and press Save when you are done !"
#define BACK_TO_GAME "Back to the game"
#define UPDOWN "Up-Down"
#define LEFTRIGHT "Left-Right"
#define CIRCULAR "Circular"
#define BOUNCE "Bounce"
#define FOLLOWER "Follower"
#define TIP_SPEED "The onject speed"
#define TIP_MOVE "The type of movement"
#define TIP_RADIUS "The radius of the circular motion"
#define TIP_WALL "Transform the object into a wall"
#define WALL "Wall"
#define BTN_SAVE "Save"
#define BTN_RESTART "Reset"
#define MSG_ERROR "Error"
#define MSG_NO_OBSTACLE "No obstacle was selected !"
#define MSG_OVERWRITE "This file already exists ! Replace ?"
#define MSG_ERROR_WRITE "There was an error while writting the file !"
#define BTN_OPEN "Open"
#define BTN_TEST "Test"
#define BTN_BACK "Back"
#define TEST_MODE "! LEVEL TESTING !"
#define CM_DEL "Delete"
#define CM_CUT "Cut"
#define CM_COPY "Copy"
#define CM_PASTE "Paste"
#define BTN_SAVE_PROGRESS "Save Progress"
#define MSG_SUCCESS "Success"
#define MSG_PROGRESS_SAVED "The progress was saved !"


// some obstacleInfo constants
#define CO_MOVE 0
#define CO_TYPE 1
#define CO_STEP 2
#define CO_RADIUS 3

#define CO_WTYPE 4 // wall type
#define CO_XPOS 5
#define CO_YPOS 6
#define CO_WIDTH 7
#define CO_HEIGHT 8

// buttons
#define ID_BTN_CREATE 111
#define ID_BTN_EXIT 110
#define ID_BTN_WALL 112
#define ID_BTN_SAVE 113
#define ID_BTN_RESTART 114
#define ID_BTN_BACK 115
#define ID_BTN_OPEN 116
#define ID_BTN_TEST 117
#define ID_BTN_SAVE_PROGRESS 118


// combo boxes
#define ID_COMBO_MOVE 30

// edit boxes
#define ID_EDIT_SPEED 40
#define ID_EDIT_RADIUS 41


// context menu
#define ID_CM_DEL 60
#define ID_CM_CUT 61
#define ID_CM_COPY 62
#define ID_CM_PASTE 63


TCHAR DEFAULT_DIR_PATH[MAX_PATH] = {0}; 


typedef struct cpb{
	int w;
	int h;
	int type;
	int obsType;
	int obsId;
} CLIP;

// we include the functions
#include "cursor.h"
#include "colision.h"
#include "functions.h"
#include "buttons.h"
#include "movement.h"
#include "keys.h"
#include "files.h"

#include "game_play.h"
#include "maker.h"

