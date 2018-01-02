// ScreenCapture.cpp : main source file for ScreenCapture.exe
//

#include "HYUtilBase.h"
#include "SCCanvasApp.h"
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>

CAppModule _Module;

BOOL RunCanvasApp(BOOL bEnter, LPCTSTR lpszCmdLine = NULL)
{
	BOOL bRet(FALSE);
	CSCCanvasApp* pAppInstance = CSCCanvasApp::GetInstance();
	if(pAppInstance == NULL) return FALSE;

	if(bEnter)
	{
		if(pAppInstance->Initialize(lpszCmdLine))
		{
			if(pAppInstance->InitializeUI())
			{
				bRet = TRUE;
			}
		}
	}
	else
	{	
		pAppInstance->UnInitializeUI();
		pAppInstance->Uninitialize();
		bRet = TRUE;
		
		pAppInstance->DeleteInstance();
	}



	return bRet;
}


int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	int nRet(-1);

	if(RunCanvasApp(TRUE, lpstrCmdLine))
	{
		nRet = theLoop.Run();
	}

	RunCanvasApp(FALSE);


	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
