// Module:		CIS023_S2020_HW16.cpp
// Author:		Miguel Antonio Logarta
// Date:		April 18, 2020
// Purpose:		Insert a linked list into a project.
//				Load, expand, and traverse the linked list without regard to the number of nodes.
//

#include "framework.h"
#include "CIS023_S2020_HW16.h"

#define MAX_LOADSTRING 100
//#define MAX_BALL 100							// size of array
#define WM_TIMER_CREATE 2001					// timer to create ball objects
#define WM_TIMER_MOVE 2002						// timer to send move and point message to ball objects

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

//BallClass* Ball[MAX_BALL];						// array of pointers to BallClass
int iCount = 0;									// how many objects have created (for screen display)

// Linked list pointers
BallClass* firstNode, * lastNode, * newNode, * currentNode;

												// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CIS023S2020HW16, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CIS023S2020HW16));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIS023S2020HW16));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CIS023S2020HW16);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// reduce flicker by double-buffering 

		// get window client rectangle sizes
		RECT rClient;
		int cx, cy;
		GetClientRect(hWnd, &rClient);
		cx = rClient.right - rClient.left;
		cy = rClient.bottom - rClient.top;

		// create memory device
		HDC memdc = CreateCompatibleDC(hdc);			// create memory copy of dc
		unsigned bpp = GetDeviceCaps(hdc, BITSPIXEL);	// discover color depth
		HBITMAP hBmp = CreateBitmap(cx, cy, 1, bpp, NULL); // create bitmap for background
		HBITMAP hTmpBmp = (HBITMAP)SelectObject(memdc, (HGDIOBJ)hBmp); // fill memory dc

		// draw GDI background
		FillRect(memdc, &rClient, (HBRUSH)GetStockObject(WHITE_BRUSH));

		// run through array and paint each ball
		/*for (int i = 0; i < MAX_BALL; i++)
			if (Ball[i])
				Ball[i]->Draw(memdc);*/

		// Run through the array and paint each ball
		currentNode = firstNode;					// Start at the top of the list
		while (currentNode != nullptr)				// Loop until end of linked list
		{
			currentNode->Draw(memdc);				// Paint the ball
			currentNode = currentNode->GetNext();	// Set the current node to the next node
		}

		// display count
		TCHAR szCount[TCHAR_SIZE];
		InttoTCHAR(iCount, szCount);
		TextOut(memdc, 0, 0, szCount, lstrlen(szCount));

		//update the main DC
		BitBlt(hdc, 0, 0, cx, cy, memdc, 0, 0, SRCCOPY); // blt memory dc to window dc

		// clean up
		SelectObject(memdc, (HGDIOBJ)hTmpBmp);			// return to original dc
		DeleteDC(memdc);								// delete temp dc
		DeleteObject(hBmp);								// delete object

		// end
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_TIMER:
	{
		//if (wParam == WM_TIMER_CREATE)					// create timer
		//{
		//	for (int i = 0; i < MAX_BALL; i++)			// look for an empty object
		//		if (!Ball[i])							//	found an empty object
		//		{

		//			Ball[i] = new BallClass(hWnd);		// create new object

		//			iCount++;							// count the number of nodes created

		//			break;								// look no further
		//		}

		//}
		//else if (wParam == WM_TIMER_MOVE)				// move timer
		//{
		//	for (int i = 0; i < MAX_BALL; i++)			// look for objects
		//	{
		//		if (Ball[i])							// if Ball exists
		//		{
		//			Ball[i]->Move(hWnd);

		//			InvalidateRect(hWnd,				// force WM_PAINT message
		//				NULL,							// entire window
		//				FALSE);							// false for non-flicker with hmemdc
		//		}

		//	}
		//}

		if (wParam == WM_TIMER_CREATE)					// Create timer
		{
			// Create a linked list that will store the balls
			newNode = new BallClass(hWnd);				// Create a new node

			if (firstNode == nullptr)					// If this is the first node / new list
				firstNode = newNode;					// Then set this node as the first node
			else                                        // The list has already been started
				lastNode->SetNext(newNode);				// Then set the last node to be this node

			iCount++;									// Increment count to show that we have created one more ball
			lastNode = newNode;							// This new node becomes the last node
		}
		else if (wParam == WM_TIMER_MOVE)				// Move timer
		{
			currentNode = firstNode;					// Start at the top of the list
			while (currentNode != nullptr)				// Go through entire list
			{
				currentNode->Move(hWnd);				// Move the ball
				InvalidateRect(hWnd,					// Force WM_PAINT message
					NULL,								// Entire window
					FALSE);								// False for non-flicker with hmemdc
				currentNode = currentNode->GetNext();	// Set the current node to the next node
			}
		}
	}
	break;
	case WM_CREATE:
	{
		//// run through array and set every pointer to null
		//for (int i = 0; i < MAX_BALL; i++)
		//	Ball[i] = nullptr;

		// ball timers
		SetTimer(hWnd, WM_TIMER_CREATE, 100, NULL);
		SetTimer(hWnd, WM_TIMER_MOVE, 10, NULL);
	}
	break;
	case WM_DESTROY:
	{
		// run through array and delete all objects
		/*for (int i = 0; i < MAX_BALL; i++)
			if (Ball[i])
				delete Ball[i];*/

		// Delete the linked list
		currentNode = firstNode;					// Set it to the first node since the currentNode is currently nullptr		
		if (currentNode)							// If the list exists
		{
			lastNode->SetNext(nullptr);				// Set last next to prevent overflow

			BallClass* tempNode = nullptr;			// Temporary pointer
			currentNode = firstNode;				// Start at the top of the list

			while (currentNode != nullptr)			// Loop until the entire list is deleted
			{
				tempNode = currentNode->GetNext();	// Save pointer to next node
				delete currentNode;					// Delete the current node
				currentNode = tempNode;				// Move on to the next node
			}
		}
		PostQuitMessage(0);
	}
		break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
