#include "stdafx.h"
#include "WinMonitorManager.h"
#include "WinEvent.h"
#include "WinOutput.h"
#include "WinDebug.h"

using namespace std;

CWinMonitorManager::CWinMonitorManager()
:m_bMonitorCreateDestroy(FALSE), m_bMonitorFocus(FALSE), m_bMonitorShowHide(FALSE)
{

}

CWinMonitorManager::~CWinMonitorManager()
{

}

BOOL CWinMonitorManager::InstallMonitor()
{
	if(!CWinEvent::GetInstance().InstallEventHook(this))
	{
		return FALSE;
	}

	return TRUE;
}

VOID CWinMonitorManager::UninstallMonitor()
{
	CWinEvent::GetInstance().UninstallEventHook();
}

BOOL CWinMonitorManager::StartMonitorFocus()
{
	m_bMonitorFocus = TRUE;

	return TRUE;
}

BOOL CWinMonitorManager::StartMonitorCreateDestroy()
{
	m_bMonitorCreateDestroy = TRUE;

	return TRUE;
}

BOOL CWinMonitorManager::StartMonitorShowHide()
{
	m_bMonitorShowHide = TRUE;
	
	return TRUE;
}

VOID CWinMonitorManager::StopMonitor()
{
	m_bMonitorCreateDestroy = FALSE;
	m_bMonitorFocus = FALSE;
	m_bMonitorShowHide = FALSE;
}

BOOL IsPopupWindow(HWND hWnd)
{
	if(!IsWindow(hWnd)) return FALSE;
	return !(GetWindowLong(hWnd,  GWL_STYLE) & WS_CHILD);
}

VOID DebugWinMessage(LPCTSTR lpszHeader, HWND hWnd, BOOL bForPopup = TRUE)
{
	CWinOutput::GetInstance().OutputMessage(lpszHeader, FALSE);

	//if(!bForPopup || IsPopupWindow(hWnd))
	{
		CWinDebug debug(hWnd);
		std::wstring strWin = debug.GetDebugText();
		CWinOutput::GetInstance().OutputMessage(strWin.c_str(), TRUE);

		CWinOutput::GetInstance().OutputMessage(_T("\r\n"), FALSE);
	}
}

 VOID CWinMonitorManager::OnForegroundChanged(HWND hWnd)
 {
	if(!m_bMonitorFocus) return;

	DebugWinMessage(_T("Foreground window changed to: \r\n"), hWnd);
 }

 VOID CWinMonitorManager::OnFocusChanged(HWND hWnd)
 {
	 if(!m_bMonitorFocus) return;

	 DebugWinMessage(_T("Focus window changed to: \r\n"), hWnd);
 }

 VOID CWinMonitorManager::OnWinShow(HWND hWnd)
 {
	 if(!m_bMonitorShowHide) return;

	 DebugWinMessage(_T("Show window : \r\n"), hWnd);	
 }

 VOID CWinMonitorManager::OnWinHide(HWND hWnd)
 {
	 if(!m_bMonitorShowHide) return;

	 DebugWinMessage(_T("Hide window : \r\n"), hWnd);	
 }

 VOID CWinMonitorManager::OnWinCreate(HWND hWnd)
 {
	 if(!m_bMonitorCreateDestroy) return;

	 DebugWinMessage(_T("Created window : \r\n"), hWnd);	
 }

 VOID CWinMonitorManager::OnWinDestroy(HWND hWnd)
 {
	 if(!m_bMonitorCreateDestroy) return;

	 DebugWinMessage(_T("Destroy window : \r\n"), hWnd);	
 }