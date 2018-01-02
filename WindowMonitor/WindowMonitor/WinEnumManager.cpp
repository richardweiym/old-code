#include "stdafx.h"
#include "WinEnumManager.h"
#include "WinDebug.h"
#include "WinOutput.h"
#include <string>

using namespace std;

CWinEnumManager::CWinEnumManager()
{

}

CWinEnumManager::~CWinEnumManager()
{

}


VOID CWinEnumManager::Refresh()
{
	//CWinOutput::GetInstance().Clear();

	m_winEnum.SetEnumSink(this);
	m_winEnum.StartEnum();
}


VOID CWinEnumManager::OnEnumStart()
{

}

VOID CWinEnumManager::OnEnumEnd()
{

}

VOID CWinEnumManager::OnEnumWindow(HWND hWnd)
{
	BOOL bMetroSplit = FALSE;
	TCHAR szClassName[128] = {0};
	GetClassName(hWnd, szClassName, sizeof(szClassName) / sizeof(szClassName[0]));
	if(lstrcmpi(szClassName, TEXT("ApplicationManager_ImmersiveShellWindow")) == 0 //win8.1
		|| lstrcmpi(szClassName, TEXT("ImmersiveBackgroundWindow")) == 0) //win8
	{
		bMetroSplit = TRUE;
	}

	if(m_winFilter.IsTargetWindow(hWnd))
	{
		CWinDebug debug(hWnd);
		wstring strMessage = debug.GetDebugText();

		CWinOutput::GetInstance().OutputMessage(debug.GetDebugText().c_str());
	}

	if(bMetroSplit)
	{
		CWinOutput::GetInstance().OutputMessage(_T("================= Metro Splitter ================= \r\n"), FALSE);
	}
}