#include "stdafx.h"
#include "WinFilter.h"
#include "FilterConfig.h"
#include <Shlwapi.h>

CWinFilter::CWinFilter()
{

}

CWinFilter::~CWinFilter()
{

}

BOOL IsTargetClassName(HWND hWnd)
{
	LPCTSTR lpszClassName = CFilterConfig::GetInstance().GetClassName();
	if(lstrlen(lpszClassName) == 0) return TRUE;

	TCHAR szClassName[128] = {0};
	GetClassName(hWnd, szClassName, 128);
	if(StrStrI(szClassName, lpszClassName) != NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL IsTargetText(HWND hWnd)
{
	LPCTSTR lpszText = CFilterConfig::GetInstance().GetText();
	if(lstrlen(lpszText) == 0) return TRUE;

	TCHAR szText[128] = {0};
	GetWindowText(hWnd, szText, 128);
	if(StrStrI(szText, lpszText) != NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL IsTargetProcessID(HWND hWnd)
{
	DWORD dwTargetProcessID = CFilterConfig::GetInstance().GetDwordProcessID();
	if(dwTargetProcessID == 0) return TRUE;

	DWORD dwProcessID(0);
	GetWindowThreadProcessId(hWnd, &dwProcessID);
	return dwProcessID == dwTargetProcessID;
}

BOOL IsTargetVisibility(HWND hWnd)
{
	BOOL bTargetVisible = CFilterConfig::GetInstance().GetVisible();
	if(!bTargetVisible) return TRUE;

	BOOL bVisible = (GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE);
	return bVisible;
}

BOOL IsTargetLayered(HWND hWnd)
{
	BOOL bTargetLayered = CFilterConfig::GetInstance().GetLayered();
	if(!bTargetLayered) return TRUE;

	BOOL bLayered = (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYERED);
	return bLayered;
}

BOOL IsTargetTopMost(HWND hWnd)
{
	BOOL bTargetTopMost = CFilterConfig::GetInstance().GetTopMost();
	if(!bTargetTopMost) return TRUE;

	BOOL bTopMost = (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST);
	return bTopMost;
}

BOOL CWinFilter::IsTargetWindow(HWND hWnd) const
{	
	return IsTargetVisibility(hWnd)
		&& IsTargetTopMost(hWnd)
		&& IsTargetLayered(hWnd)
		&& IsTargetProcessID(hWnd)
		&& IsTargetClassName(hWnd)
		&& IsTargetText(hWnd);
}

