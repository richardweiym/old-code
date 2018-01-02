// SmsServer.cpp : main source file for SmsServer.exe
//

#include "stdafx.h"
#include "SmsServiceAgentWnd.h"

CAppModule _Module;

int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);
	int nRet;


  	if(lpstrCmdLine!=NULL)
  	{
 		HWND hController = (HWND)(_ttol(lpstrCmdLine));
  		if((hController!=NULL) && IsWindow(hController))
 		{
 			CSmsServiceAgentWnd wndServiceAgent(hController);
			if(wndServiceAgent.Create(NULL,CWindow::rcDefault,NULL,WS_POPUP))
			{
				nRet = theLoop.Run();
			}		
  		}
 	}

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);

	ATLASSERT(SUCCEEDED(hRes));

	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
