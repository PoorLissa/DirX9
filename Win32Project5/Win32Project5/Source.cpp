// define the screen resolution
#define SCREEN_WIDTH  1600
#define SCREEN_HEIGHT  900

#define fullScreen
#undef  fullScreen

// include the basic windows header file
#include <windows.h>
#include <string>
#include <windowsx.h>
#include "__d3d.h"

// mouse coordinates
int xPos = 0, yPos = 0, xPos0, yPos0, zPos = 0;


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    // the handle for the window, filled by a function
    HWND hWnd;
    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";

    // register the window class
    RegisterClassEx(&wc);

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(NULL,
                          L"WindowClass1",					// name of the window class
                          L"Our First Windowed Program",	// title of the window
#if defined fullScreen
                          WS_OVERLAPPEDWINDOW,				// window style
#else
						  WS_EX_TOPMOST | WS_POPUP,			// fullscreen values
#endif
                          10,								// x-position of the window
                          10,								// y-position of the window
#if defined fullScreen
                          SCREEN_WIDTH,						// width of the window
                          SCREEN_HEIGHT,					// height of the window
#else
						  800,
						  500,
#endif
                          NULL,								// we have no parent window, NULL
                          NULL,								// we aren't using menus, NULL
                          hInstance,						// application handle
                          NULL);							// used with multiple windows, NULL

    // display the window on the screen
    ShowWindow(hWnd, nCmdShow);

#if defined fullScreen
	initD3D(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);
#else
	initD3D(hWnd, 800, 500);
#endif

    // enter the main loop:
    // this struct holds Windows event messages
    MSG msg;
	unsigned long Counter = 0;

    // Enter the infinite message loop
    while(TRUE)
    {
        // Check to see if any messages are waiting in the queue
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function
            DispatchMessage(&msg);
        }

        // If the message is WM_QUIT, exit the while loop
        if(msg.message == WM_QUIT)
            break;

        // Run game code here
		Counter++;
		render_frame3();
        // ...
        // ...
    }

	TCHAR buffer[65];
   _itow_s(Counter, buffer, 10);

	//MessageBox(NULL, buffer, L"huyets", 0);

	cleanD3D();

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
        // this message is read when the window is closed
        case WM_DESTROY: {
                // close the application entirely
                PostQuitMessage(0);
                return 0;
		} break;

		// onKeyDown
		case WM_KEYDOWN: {

            switch( LOWORD(wParam) )
			{
				case VK_ESCAPE:
	                PostQuitMessage(0);
		            return 0;
                break;
			}

		} break;

		// onMouseButtonDown
		case WM_RBUTTONDOWN: {

			// запоминаем координаты, где нажали кнопку мыши
			xPos0 = GET_X_LPARAM(lParam);
			yPos0 = GET_Y_LPARAM(lParam);

		} break;

		// onMouseButtonUp
		case WM_MBUTTONUP: {
		} break;

		// onMouseMove
		case WM_MOUSEMOVE: {

			if (LOWORD(wParam) == MK_RBUTTON) {

				int x = GET_X_LPARAM(lParam);
				xPos += xPos0 - x;
				xPos0 = x;

				int y = GET_Y_LPARAM(lParam);
				yPos += y - yPos0;
				yPos0 = y;
			}

		} break;

		case WM_MOUSEWHEEL: {
		
			zPos -= GET_WHEEL_DELTA_WPARAM(wParam);
		} break;
    }

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}