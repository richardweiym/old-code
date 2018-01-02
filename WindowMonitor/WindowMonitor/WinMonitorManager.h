#ifndef __WIN_MONITOR_MANAGER_H__
#define __WIN_MONITOR_MANAGER_H__

#include "WinEvent.h"

class CWinMonitorManager: public IWinEventSink
{
public:
	CWinMonitorManager();
	~CWinMonitorManager();

	BOOL InstallMonitor();
	VOID UninstallMonitor();

	BOOL StartMonitorFocus();
	BOOL StartMonitorCreateDestroy();
	BOOL StartMonitorShowHide();

	VOID StopMonitor();

public:
	virtual VOID OnForegroundChanged(HWND hWnd);
	virtual VOID OnFocusChanged(HWND hWnd);
	virtual VOID OnWinShow(HWND hWnd);
	virtual VOID OnWinHide(HWND hWnd);
	virtual VOID OnWinCreate(HWND hWnd);
	virtual VOID OnWinDestroy(HWND hWnd);

private:
	CWinMonitorManager(const CWinMonitorManager&);
	CWinMonitorManager& operator = (const CWinMonitorManager&);

private:
	BOOL m_bMonitorFocus;
	BOOL m_bMonitorShowHide;
	BOOL m_bMonitorCreateDestroy;
};

#endif //__WIN_MONITOR_MANAGER_H__