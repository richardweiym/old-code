// WindowMonitor.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WindowMonitor.h"
#include "WinEnumManager.h"
#include "WinOutput.h"
#include "FilterDialog.h"
#include "WinMonitorManager.h"

#define MAX_LOADSTRING 100

#define ENUM_AFTER_5S_TIMER_ID		10
#define ENUM_AFTER_10S_TIMER_ID		11

#define MONITOR_FOCUS_5S 12
#define MONITOR_FOCUS_10S 13

#define MONITOR_SHOW_5S 14
#define MONITOR_SHOW_10S 15

#define MONITOR_CREATE_5S 16
#define MONITOR_CREATE_10S 17

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HINSTANCE g_hInstance = NULL;
HWND g_hWndEdit = NULL;
CWinEnumManager g_winEnumMgr;
CFilterDialog g_filterDialog;
CWinMonitorManager g_winMonitorMgr;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
HWND CreateEdit(HWND hWndParent);
BOOL PreTranslateMessage(MSG* msg);



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	g_hInstance = hInstance;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINDOWMONITOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWMONITOR));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if(PreTranslateMessage(&msg)) continue;

		if(g_filterDialog.IsWindow())
		{
			if(g_filterDialog.IsDialogMessage(&msg)) continue;
		}

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
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWMONITOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINDOWMONITOR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


HWND CreateEdit(HWND hWndParent)
{
	RECT rcClient = {0};
	GetClientRect(hWndParent, &rcClient);
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_LEFT | ES_MULTILINE | ES_NOHIDESEL;
	DWORD dwExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_CLIENTEDGE;

	HWND hWndEdit = CreateWindow(_T("Edit"), NULL, dwStyle, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top
		, hWndParent, NULL, hInst, NULL);

	CWinOutput::GetInstance().Init(hWndEdit);
	
	return hWndEdit;
}

BOOL PreTranslateMessage(MSG* msg)
{
	if(::GetKeyState(VK_CONTROL) < 0
		&& ::GetKeyState('A') < 0)
	{
		CWinOutput::GetInstance().SelectAll();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID StartEnumWindow()
{
	g_winEnumMgr.Refresh();
}

VOID StartMonitorFocus()
{
	g_winMonitorMgr.StartMonitorFocus();
}

VOID StartMonitorCreateDestroy()
{
	g_winMonitorMgr.StartMonitorCreateDestroy();
}

VOID StartMonitorShowHide()
{
	g_winMonitorMgr.StartMonitorShowHide();
}

VOID StopMonitor(HWND hWnd)
{
	g_winMonitorMgr.StopMonitor();

	KillTimer(hWnd, MONITOR_FOCUS_5S);
	KillTimer(hWnd, MONITOR_FOCUS_10S);
	KillTimer(hWnd, MONITOR_SHOW_5S);
	KillTimer(hWnd, MONITOR_SHOW_10S);
	KillTimer(hWnd, MONITOR_CREATE_5S);
	KillTimer(hWnd, MONITOR_CREATE_10S);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		g_hWndEdit = CreateEdit(hWnd);
		g_winMonitorMgr.InstallMonitor();
		break;

	case WM_SIZE:
		{
			RECT rcClient = {0};
			GetClientRect(hWnd, &rcClient);
			MoveWindow(g_hWndEdit, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, TRUE);
		}
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		case ID_STARTENUM_NOW:
			StartEnumWindow();
			break;

		case ID_STARTENUM_AFTER5S:
			SetTimer(hWnd, ENUM_AFTER_5S_TIMER_ID, 5*1000, NULL);
			break;

		case ID_STARTENUM_AFTER10S:
			SetTimer(hWnd, ENUM_AFTER_10S_TIMER_ID, 10*1000, NULL);
			break;

		case ID_ENUM_CONFIG:
			g_filterDialog.ShowDialog(hWnd);
			break;

		case ID_FOCUSMONITOR_NOW:
			StartMonitorFocus();
			break;

		case ID_FOCUSMONITOR_AFTER5S:
			SetTimer(hWnd, MONITOR_FOCUS_5S, 5*1000, NULL);
			break;

		case ID_FOCUSMONITOR_AFTER10S:
			SetTimer(hWnd, MONITOR_FOCUS_10S, 10*1000, NULL);
			break;
		
		case ID_CREATE_NOW:
			StartMonitorCreateDestroy();
			break;

		case ID_CREATE_AFTER5S:
			SetTimer(hWnd, MONITOR_CREATE_5S, 5*1000, NULL);
			break;

		case ID_CREATE_AFTER10S:
			SetTimer(hWnd, MONITOR_CREATE_10S, 10*1000, NULL);
			break;

		case ID_STARTSHOW_NOW:
			StartMonitorShowHide();
			break;

		case ID_STARTSHOW_AFTER5S:
			SetTimer(hWnd, MONITOR_SHOW_5S, 5*1000, NULL);
			break;

		case ID_STARTSHOW_AFTER10S:
			SetTimer(hWnd, MONITOR_SHOW_10S, 10*1000, NULL);
			break;

		case ID_FILE_STOPMONITOR:
		{
			StopMonitor(hWnd);
			break;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	
	case WM_TIMER:
		if(wParam == ENUM_AFTER_5S_TIMER_ID)
		{
			KillTimer(hWnd, ENUM_AFTER_5S_TIMER_ID);
			StartEnumWindow();
		}
		else if(wParam == ENUM_AFTER_10S_TIMER_ID)
		{
			KillTimer(hWnd, ENUM_AFTER_10S_TIMER_ID);
			StartEnumWindow();
		}
		else if(wParam == MONITOR_FOCUS_5S)
		{
			KillTimer(hWnd, MONITOR_FOCUS_5S);
			StartMonitorFocus();
		}
		else if(wParam == MONITOR_FOCUS_10S)
		{
			KillTimer(hWnd, MONITOR_FOCUS_10S);
			StartMonitorFocus();
		}
		else if(wParam == MONITOR_CREATE_5S)
		{
			KillTimer(hWnd, MONITOR_CREATE_5S);
			StartMonitorCreateDestroy();
		}
		else if(wParam == MONITOR_CREATE_10S)
		{
			KillTimer(hWnd, MONITOR_CREATE_10S);
			StartMonitorCreateDestroy();
		}
		else if(wParam == MONITOR_SHOW_5S)
		{
			KillTimer(hWnd, MONITOR_SHOW_5S);
			StartMonitorShowHide();
		}
		else if(wParam == MONITOR_SHOW_10S)
		{
			KillTimer(hWnd, MONITOR_SHOW_10S);
			StartMonitorShowHide();
		}

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;

	case WM_ERASEBKGND:
		return TRUE;

	case WM_DESTROY:
		{
			if(g_filterDialog.IsWindow()) g_filterDialog.DestroyDialog();
			g_winMonitorMgr.UninstallMonitor();
		}
		PostQuitMessage(0);
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
