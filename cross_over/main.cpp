/*
*
* Cross Over C++ game
* Author: Mihai Ionut Vilcu
* March-April 2013
*
*/

#include "stdafx.h"


LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

LPINITCOMMONCONTROLSEX lpInitCtrls;

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow )
{
	
	InitCommonControlsEx(lpInitCtrls);
    UNREFERENCED_PARAMETER( prevInstance );
    UNREFERENCED_PARAMETER( cmdLine );

    WNDCLASSEX wndClass = { 0 };
    wndClass.cbSize = sizeof( WNDCLASSEX ) ;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hInstance = hInstance;
    wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
    wndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1));
    wndClass.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = "ClassName";
	wndClass.hIconSm = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16,16, 0);
	

    if( !RegisterClassEx( &wndClass ) )
        return -1;

    RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    HWND hwnd = CreateWindowA( "ClassName", GAME_TITLE, WS_CLIPCHILDREN|WS_OVERLAPPEDWINDOW&~WS_THICKFRAME&~WS_MAXIMIZEBOX,
                                CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
                                NULL, NULL, hInstance, NULL );

    if( !hwnd )
        return -1;





    ShowWindow( hwnd, cmdShow );


    MSG msg = { 0 };

    while( msg.message != WM_QUIT )
    {
		Sleep(2);
        if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }

    }
    //cleanup and shutdown

    return static_cast<int>( msg.wParam );
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static int OPTION = 0;
	static int TestLevel = 0;
	
	if(OPTION == 1) {
		LevelMaker(hwnd, message, wParam, lParam, OPTION, TestLevel);
	} else
		GamePlay(hwnd, message, wParam, lParam, OPTION, TestLevel);
	
	return DefWindowProc (hwnd, message, wParam, lParam);

}


