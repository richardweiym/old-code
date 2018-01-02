#include "HYCustomMenu.h"


HHOOK CHYCustomMenuMgr::s_hkMenu = NULL;
IHYMenuDrawer* CHYCustomMenuMgr::s_pMenuDrawer = NULL;
std::map<HWND, DWORD> CHYCustomMenuMgr::s_mapMenuProc;


VOID CHYCustomMenuMgr::SetMenuDrawer(IHYMenuDrawer* pMenuDrawer)
{
	s_pMenuDrawer = pMenuDrawer;
}

IHYMenuDrawer* CHYCustomMenuMgr::GetMenuDrawer()
{
	return s_pMenuDrawer;
}

BOOL CHYCustomMenuMgr::InitializeHook( )
{
	if(s_hkMenu != NULL)
	{
		UninitializeHook();
	}
	
	s_hkMenu = SetWindowsHookEx(WH_CBT, CBTProc, GetModuleHandle(NULL), GetCurrentThreadId());
	
	return s_hkMenu != NULL;
}

VOID CHYCustomMenuMgr::UninitializeHook()
{
	if(s_hkMenu != NULL)
	{
		RestoreAllMenuTrack();
		UnhookWindowsHookEx(s_hkMenu);
		s_hkMenu = NULL;
	}
	
	s_mapMenuProc.clear();
}


LRESULT CALLBACK CHYCustomMenuMgr::CBTProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HCBT_CREATEWND)
	{
		HWND hWnd = (HWND)wParam;
		if(IsTargetMenuWindow(hWnd))
		{
			AddMenuTrack(hWnd);
		}
	}
	else if(nCode == HCBT_DESTROYWND)
	{
		HWND hWnd = (HWND)wParam;
		if(IsTargetMenuWindow(hWnd))
		{
			RemoveMenuTrack(hWnd);
		}
	}
	
	return CallNextHookEx(s_hkMenu, nCode, wParam, lParam);
}

VOID CHYCustomMenuMgr::DrawBorder(CDCHandle dc, HWND hWnd)
{
	if(s_pMenuDrawer == NULL) return;

	COLORREF clrFrame = s_pMenuDrawer->GetMenuFrameColor();

	CRect rtWindow;
	::GetWindowRect(hWnd, &rtWindow);

	CRect rtFrame(0, 0, rtWindow.Width(), rtWindow.Height());

	CHYUICommonFunctionWrapper::DrawFrame(dc, rtFrame, clrFrame, 1);
}

LRESULT CALLBACK CHYCustomMenuMgr::WindowProc(HWND hWnd, UINT nMsgType, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled(FALSE);
	LRESULT lRet(0);
	
	switch(nMsgType)
	{
	case WM_NCCALCSIZE:
		{
			lRet = CallOldMenuProc(hWnd, nMsgType, wParam, lParam);
			
			LPNCCALCSIZE_PARAMS pNCSize = (LPNCCALCSIZE_PARAMS)lParam;
			RECT& rtNC = pNCSize->rgrc[0];
// 			rtNC.right -= 15;
// 			rtNC.bottom -= 15;
			
			bHandled = TRUE;
		}
		break;
		
	case WM_WINDOWPOSCHANGING:
		{
			lRet = CallOldMenuProc(hWnd, nMsgType, wParam, lParam);
			
			WINDOWPOS* pWinPos = (WINDOWPOS*)lParam;
// 			pWinPos->cx += 15;
// 			pWinPos->cy += 15;
			
			bHandled = TRUE;
		}
		break;
		
	case WM_NCPAINT:
		{
			lRet = CallOldMenuProc(hWnd, nMsgType, wParam, lParam);
			CWindowDC dc(hWnd);
			DrawBorder(dc.m_hDC, hWnd);
			bHandled = TRUE;
		}
		break;


	case WM_PRINT:
		{
			lRet = CallOldMenuProc(hWnd, nMsgType, wParam, lParam);
			
			DrawBorder((CDCHandle)(HDC)wParam, hWnd);
		
			bHandled = TRUE;			
		}
		break;
		
	default:
		break;	
	}
	
	if(!bHandled)
	{
		return CallOldMenuProc(hWnd, nMsgType, wParam, lParam);
	}
	
	return lRet;
}

LRESULT CHYCustomMenuMgr::CallOldMenuProc(HWND hWnd, UINT nMsgType, WPARAM wParam, LPARAM lParam )
{
	CConItrType itr = s_mapMenuProc.find(hWnd);
	
	if(itr == s_mapMenuProc.end())
	{
		_ASSERTE(FALSE);
		return S_OK;
	}
	DWORD dwOldProc = itr->second;
	
	return CallWindowProc((WNDPROC)dwOldProc, hWnd, nMsgType, wParam, lParam);
}

BOOL CHYCustomMenuMgr::IsTargetMenuWindow(HWND hWnd)
{
	_ASSERTE(hWnd != NULL);
	TCHAR szClassName[32] = {0};
	GetClassName(hWnd, szClassName, sizeof(szClassName) / sizeof(TCHAR) - 1);
	if(lstrcmp(szClassName, _T("#32768")) == 0) //menu class name
	{
		return TRUE;
	}
	
	return FALSE;
}

BOOL CHYCustomMenuMgr::AddMenuTrack(HWND hWnd)
{
	if(s_mapMenuProc.find(hWnd) != s_mapMenuProc.end())
	{
		_ASSERTE(FALSE);
		return TRUE;
	} 
	
	DWORD dwOldProc = GetWindowLong(hWnd, GWL_WNDPROC);
	if(SetWindowLong(hWnd, GWL_WNDPROC, (DWORD)WindowProc))
	{
		s_mapMenuProc.insert(std::make_pair(hWnd, dwOldProc));
		return TRUE;
	} 
	
	_ASSERTE(FALSE);
	return FALSE;
}

BOOL CHYCustomMenuMgr::RemoveMenuTrack(HWND hWnd)
{
	CConItrType itr = s_mapMenuProc.find(hWnd);
	if(itr != s_mapMenuProc.end())
	{
		DWORD dwOldProc = itr->second;
		SetWindowLong(hWnd, GWL_WNDPROC, dwOldProc);
		s_mapMenuProc.erase(hWnd);
		return TRUE;
	}
	
	_ASSERTE(FALSE);
	return FALSE;
}

VOID CHYCustomMenuMgr::RestoreAllMenuTrack()
{
	while(s_mapMenuProc.size() > 0)
	{
		RemoveMenuTrack(s_mapMenuProc.begin()->first);
	}
}

