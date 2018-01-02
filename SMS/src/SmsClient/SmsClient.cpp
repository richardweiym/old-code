// SmsClient.cpp : main source file for SmsClient.exe
//

#include "stdafx.h"
#include "ServiceManager.h"
#include "resource.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CServiceManager manager;
	manager.StartService();

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}
BOOL CheckIsRunning()
{
	HWND hWnd = FindWindow(_T("SmsClientMainFrame_Richard_612"), NULL);
	if(hWnd != NULL)
	{
		::PostMessage(hWnd, WM_COMMAND, ID_SYSTEM_SHOW, NULL);
		return TRUE;
	}
	return FALSE;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	if(CheckIsRunning())
	{
		return 0;
	}

	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES
		| ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES
		| ICC_PROGRESS_CLASS | ICC_DATE_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
