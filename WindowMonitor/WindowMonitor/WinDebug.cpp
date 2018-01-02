#include "stdafx.h"
#include "WinDebug.h"
#include <sstream>

using namespace std;

BOOL GetProcessFileNameByWnd(HWND hWnd, LPTSTR lpszBuffer, INT nSize)
{
	_ASSERTE(lpszBuffer != NULL && nSize > 0);
	_ASSERTE(::IsWindow(hWnd));
	if(!::IsWindow(hWnd)) return FALSE;

	DWORD dwProcessId(0);
	::GetWindowThreadProcessId(hWnd, &dwProcessId);
	_ASSERTE(dwProcessId != NULL);
	if(dwProcessId == NULL) return FALSE;

	BOOL bRet(FALSE);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION , FALSE, dwProcessId);
	if(hProcess != NULL)
	{	
		typedef DWORD (__stdcall* pGetProcessImageFileNameProc)(HANDLE hProcess, LPTSTR lpImageFileName, DWORD nSize);

		HMODULE hModPs = ::LoadLibrary(_T("PSAPI.DLL"));
		if(hModPs != NULL)
		{
			pGetProcessImageFileNameProc FunGetProcessImageFileName = NULL;
			FunGetProcessImageFileName = (pGetProcessImageFileNameProc)GetProcAddress(hModPs, "GetProcessImageFileNameW");
		
			if(FunGetProcessImageFileName != NULL)
			{
				bRet = (FunGetProcessImageFileName(hProcess, lpszBuffer, nSize) > 0);
			}

			FreeLibrary(hModPs);	
		}

		CloseHandle(hProcess);
	}

	return bRet;
}

wstring GetExeFileName(HWND hWnd)
{
	TCHAR szFileName[MAX_PATH] = {0};
	wstring strRet;
	
	if(GetProcessFileNameByWnd(hWnd, szFileName, MAX_PATH))
	{
		LPTSTR lpszExe = _tcsrchr(szFileName, _T('\\'));
		if(lpszExe != NULL)
		{
			strRet = ++lpszExe;
		}
	}

	return strRet;
}

wstring GetClassStyleName(DWORD dwClassStyle)
{
	wostringstream os;

#define STYLE_NAME(x) if(dwClassStyle&x) os << L#x << L" ";

	STYLE_NAME( CS_BYTEALIGNCLIENT );
	STYLE_NAME( CS_BYTEALIGNWINDOW );
	STYLE_NAME( CS_CLASSDC );
	STYLE_NAME( CS_DBLCLKS );
	STYLE_NAME( CS_DROPSHADOW );
	STYLE_NAME( CS_GLOBALCLASS );
	STYLE_NAME( CS_HREDRAW );
	STYLE_NAME( CS_NOCLOSE );
	STYLE_NAME( CS_OWNDC );
	STYLE_NAME( CS_PARENTDC );
	STYLE_NAME( CS_SAVEBITS );
	STYLE_NAME( CS_VREDRAW );
	STYLE_NAME( CS_IME );

	return os.str().c_str();
}

wstring GetStyleName(DWORD dwStyle)
{
	wostringstream os;

#define STYLE_NAME(x) if(dwStyle&x) os << L#x << L" ";

	STYLE_NAME(WS_POPUP)
	STYLE_NAME(WS_OVERLAPPED);
	STYLE_NAME(WS_CHILD);
	STYLE_NAME(WS_MINIMIZE);
	STYLE_NAME(WS_VISIBLE);
	STYLE_NAME(WS_DISABLED);
	STYLE_NAME(WS_CLIPSIBLINGS);
	STYLE_NAME(WS_CLIPCHILDREN);
	STYLE_NAME(WS_MAXIMIZE);
	STYLE_NAME(WS_CAPTION);
	STYLE_NAME(WS_BORDER);
	STYLE_NAME(WS_DLGFRAME);
	STYLE_NAME(WS_VSCROLL);
	STYLE_NAME(WS_HSCROLL);
	STYLE_NAME(WS_SYSMENU);
	STYLE_NAME(WS_THICKFRAME);
	STYLE_NAME(WS_GROUP);
	STYLE_NAME(WS_TABSTOP);
	STYLE_NAME(WS_MINIMIZEBOX);
	STYLE_NAME(WS_MAXIMIZEBOX);

	return os.str().c_str();
}

wstring GetExStyleName(DWORD dwExStyle)
{
	wostringstream os;

#define STYLE_NAME(x) if(dwExStyle&x) os << L#x << L" ";

	STYLE_NAME( WS_EX_DLGMODALFRAME );
	STYLE_NAME( WS_EX_NOPARENTNOTIFY );
	STYLE_NAME( WS_EX_TOPMOST );
	STYLE_NAME( WS_EX_ACCEPTFILES );
	STYLE_NAME( WS_EX_TRANSPARENT );
	STYLE_NAME( WS_EX_MDICHILD );
	STYLE_NAME( WS_EX_TOOLWINDOW );
	STYLE_NAME( WS_EX_WINDOWEDGE );
	STYLE_NAME( WS_EX_CLIENTEDGE );
	STYLE_NAME( WS_EX_CONTEXTHELP );
	STYLE_NAME( WS_EX_RIGHT );
	STYLE_NAME( WS_EX_LEFT );
	STYLE_NAME( WS_EX_RTLREADING );
	STYLE_NAME( WS_EX_LTRREADING );
	STYLE_NAME( WS_EX_LEFTSCROLLBAR );
	STYLE_NAME( WS_EX_RIGHTSCROLLBAR );
	STYLE_NAME( WS_EX_CONTROLPARENT );
	STYLE_NAME( WS_EX_STATICEDGE );
	STYLE_NAME( WS_EX_APPWINDOW );
	STYLE_NAME( WS_EX_LAYERED );
	STYLE_NAME( WS_EX_NOINHERITLAYOUT );
	STYLE_NAME( WS_EX_LAYOUTRTL );
	STYLE_NAME( WS_EX_COMPOSITED );
	STYLE_NAME( WS_EX_NOACTIVATE );

	return os.str().c_str();
}


CWinDebug::CWinDebug(HWND hWnd)
:m_hWnd(hWnd)
{

}

CWinDebug::~CWinDebug()
{

}

UINT GenerateTestPoints(POINT * pTestPoints,UINT uCount,RECT * pWndRect)
{
	if ( !pTestPoints || !pWndRect || uCount < 1 || IsRectEmpty(pWndRect))
		return -1;

	UINT uIndex = 0;
	POINT pt = {0};

	//////////////////////////////////////////////////////////////////////////
	//Center Point
	if ( uIndex >= uCount )
		return uIndex;
	pt.x = pWndRect->left + ( pWndRect->right - pWndRect->left )/2;
	pt.y = pWndRect->top + ( pWndRect->bottom - pWndRect->top )/2;
	pTestPoints[uIndex] = pt;
	++uIndex;

	//////////////////////////////////////////////////////////////////////////
	//Top-Left quarter
	if ( uIndex >= uCount )
		return uIndex;
	pt.x = pWndRect->left + ( pWndRect->right - pWndRect->left )/4;
	pt.y = pWndRect->top + ( pWndRect->bottom - pWndRect->top )/4;
	pTestPoints[uIndex] = pt;
	++uIndex;

	//////////////////////////////////////////////////////////////////////////
	//Top-Right quarter
	if ( uIndex >= uCount )
		return uIndex;
	pt.x = pWndRect->right - ( pWndRect->right - pWndRect->left )/4;
	pt.y = pWndRect->top + ( pWndRect->bottom - pWndRect->top )/4;
	pTestPoints[uIndex] = pt;
	++uIndex;

	//////////////////////////////////////////////////////////////////////////
	//Left-Bottom quarter
	if ( uIndex >= uCount )
		return uIndex;
	pt.x = pWndRect->left + ( pWndRect->right - pWndRect->left )/4;
	pt.y = pWndRect->bottom - ( pWndRect->bottom - pWndRect->top )/4;
	pTestPoints[uIndex] = pt;
	++uIndex;

	//////////////////////////////////////////////////////////////////////////
	//Right-Bottom quarter
	if ( uIndex >= uCount )
		return uIndex;
	pt.x = pWndRect->right - ( pWndRect->right - pWndRect->left )/4;
	pt.y = pWndRect->bottom - ( pWndRect->bottom - pWndRect->top )/4;
	pTestPoints[uIndex] = pt;
	++uIndex;

	//left-top
	if(uIndex >= uCount) return uIndex;
	pt.x = pWndRect->left +1;
	pt.y = pWndRect->top + 1;
	pTestPoints[uIndex] = pt;
	++uIndex;

	//left-bottom
	if(uIndex >= uCount) return uIndex;
	if(pWndRect->right - pWndRect->left > 500
		&& pWndRect->bottom - pWndRect->top > 500)
	{
		//left-top, for full screen metro window covered by task manager window issue
		pt.x = pWndRect->left + 25;
		pt.y = pWndRect->top + 25;
	}
	else
	{
		pt.x = pWndRect->left +1;
		pt.y = pWndRect->bottom - 1;
	}
	pTestPoints[uIndex] = pt;
	++uIndex;

	//right-top
	if(uIndex >= uCount) return uIndex;
	if(pWndRect->right - pWndRect->left > 500
		&& pWndRect->bottom - pWndRect->top > 500)
	{
		//right-bottom
		pt.x = pWndRect->right - 25;
		pt.y = pWndRect->bottom - 25;
	}
	else
	{
		pt.x = pWndRect->right - 1;
		pt.y = pWndRect->top + 1;
	}
	pTestPoints[uIndex] = pt;
	++uIndex;

	//right-bottom
	if(uIndex >= uCount) return uIndex;
	pt.x = pWndRect->right - 1;
	pt.y = pWndRect->bottom - 1;
	pTestPoints[uIndex] = pt;
	++uIndex;

	return uIndex;
}

BOOL TestMetroWindowVisibility(HWND hWnd)
{
	if(!::IsWindow(hWnd)) return FALSE;

	RECT rc = {0};
	GetWindowRect(hWnd,&rc);
	if(IsRectEmpty(&rc)) return FALSE;

	if(rc.left<=-32000) //Minimized window
		return FALSE;

	//the test point should be in the window region
	HRGN hWndRgn = CreateRectRgnIndirect(&rc);
	HRGN hWindowRgn = CreateRectRgn(0, 0, 0, 0);
	int nError = GetWindowRgn(hWnd, hWindowRgn);
	if( nError != ERROR)
	{
		OffsetRgn(hWndRgn, -rc.left, -rc.top);
		CombineRgn(hWndRgn, hWindowRgn, hWndRgn, RGN_AND);		
		OffsetRgn(hWndRgn, rc.left, rc.top);
	}

	GetRgnBox(hWndRgn, &rc);

	DeleteObject(hWindowRgn);
	DeleteObject(hWndRgn);

	if(IsRectEmpty(&rc)) return FALSE;

	const int MAX_TEST_COUNT=10;
	POINT ptTests[MAX_TEST_COUNT] = {0};

	UINT uGenerateCount = GenerateTestPoints(ptTests,MAX_TEST_COUNT,&rc);
	if ( uGenerateCount == -1 || uGenerateCount > MAX_TEST_COUNT )
		return FALSE;

	HWND hWndFromPoint = NULL;
	for (UINT uIndex = 0;uIndex < uGenerateCount;uIndex++)
	{
		hWndFromPoint = ChildWindowFromPointEx(GetDesktopWindow(), ptTests[uIndex], CWP_SKIPINVISIBLE);
		if(hWndFromPoint == hWnd) return TRUE;
	}

	return FALSE;
}


std::wstring CWinDebug::GetDebugText() const
{
	if(m_hWnd == NULL || !IsWindow(m_hWnd))
	{
		TCHAR szTempMsg[128] = {0};
		_sntprintf(szTempMsg, 128, _T("hWnd[0x%x] \t Not a valid window now \r\n"), m_hWnd);
		return szTempMsg;
	}

	TCHAR szClassName[128] = {0};
	GetClassName(m_hWnd, szClassName, 128);

	TCHAR szText[128] = {0};
	GetWindowText(m_hWnd, szText, 128);

	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	wstring strStyle = GetStyleName(dwStyle);

	DWORD dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	wstring strExStyle = GetExStyleName(dwExStyle);

	wstring strExeName = GetExeFileName(m_hWnd);

	RECT rcWin = {0};
	GetWindowRect(m_hWnd, &rcWin);

	DWORD dwClassStyle = GetClassLong(m_hWnd, GCL_STYLE);
	wstring strClassStyle = GetClassStyleName(dwClassStyle);

	BOOL bTestVisibility = TestMetroWindowVisibility(m_hWnd);

	BOOL bVisible = (dwStyle & WS_VISIBLE);
	BOOL bTopMost = (dwExStyle & WS_EX_TOPMOST);
	BOOL bLayered = (dwExStyle & WS_EX_LAYERED);
	TCHAR szFlag[32] = {0};
	_sntprintf(szFlag, 32, _T("%s %s %s"), bVisible ? _T("V"): _T(""),
		bLayered ? _T("L"): _T(""), bTopMost ? _T("T"): _T(""));

	DWORD dwProcessID(0);
	DWORD dwThreadID = GetWindowThreadProcessId(m_hWnd, &dwProcessID);

	TCHAR szMessage[1024] = {0};
	_sntprintf(szMessage, 1024, _T("hWnd[0x%x] \t %s \t Process[%d:%d][%s]\t ClassName[%s]\t Pos[%d,%d]\t Size[%d, %d] \t Text[%s]\t Style[%s]\t ExStyle[%s] \tClassStyle[%s]  \tTestVisibility[%d]\r\n"),
		m_hWnd, szFlag, dwProcessID, dwThreadID, strExeName.c_str(), szClassName, rcWin.left, rcWin.top, rcWin.right - rcWin.left
		, rcWin.bottom - rcWin.top,  szText, strStyle.c_str(), strExStyle.c_str(), strClassStyle.c_str(), bTestVisibility);

	return szMessage;
}

