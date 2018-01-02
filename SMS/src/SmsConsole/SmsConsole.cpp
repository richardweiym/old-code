// SmsConsole.cpp : main source file for SmsConsole.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>

#include "resource.h"

#include "aboutdlg.h"
#include "maindlg.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();

	return nRet;
}


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	//check is server already running
	//if it is running active it and return
	BOOL bFind(FALSE);
	HANDLE hMutex = CreateMutex(NULL, TRUE, _T("SMS_SERVER_CONSOLE_RICHARD(612)"));
	if(hMutex != NULL && (GetLastError() == ERROR_ALREADY_EXISTS))
	{
		TCHAR szTitle[256] = {0};
		::LoadString(_Module.GetResourceInstance(), IDS_MAIN_DLG_TITLE, szTitle, 256);
		
		int nCount = 0;

		while(nCount < 3)
		{
			HWND hMaiDlg = ::FindWindow(NULL, szTitle);
			if(hMaiDlg == NULL)
			{
				lstrcat(szTitle, _T("(ÊÔÓÃ°æ)"));
				hMaiDlg = ::FindWindow(NULL, szTitle);
			}
			if(hMaiDlg != NULL)
			{
				PostMessage(hMaiDlg, WM_COMMAND, ID_SHOW_MAIN, NULL);
				bFind = TRUE;
				break;
			}
			else
			{
				Sleep(500);
				nCount++;
			}
		}
	}
	
	if(bFind && hMutex != NULL)
	{
		CloseHandle(hMutex);
		hMutex = NULL;
		return 0;
	}


	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();
	
	if(hMutex != NULL)
	{
		CloseHandle(hMutex);
		hMutex = NULL;
	}

	return nRet;
}
