#pragma once 

#include "HYUtilBase.h"
#include <vector>

#ifndef  WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif

#ifndef WS_EX_TRANSPARENT
#define WS_EX_TRANSPARENT 0x00000020L
#endif

class CSCWinFilter
{
public:
	static BOOL IsFilterWindow(HWND hWnd)
	{
		_ASSERTE(hWnd != NULL);
		DWORD dwProcessID = GetCurrentProcessId();
		if(hWnd != NULL && IsWindow(hWnd))
		{
			DWORD dwWinProcessId(0);
			GetWindowThreadProcessId(hWnd, &dwWinProcessId);
			if(dwProcessID == dwWinProcessId) 
			{
				return TRUE;
			}
		}
		
		return FALSE;
	}
	
	static DWORD GetIncludeStyle()
	{
		return WS_VISIBLE;
	}
	
	static DWORD GetExcludeStyleEx()
	{
		return  WS_EX_TRANSPARENT;
	}
	
	static BOOL IsTargetPopupWindow()
	{
		return FALSE;
	}
};

class CSCWinInfo
{
public:
	HWND m_hWnd;	
	CRect m_rtWin;	//window rect
					
	INT m_nLevel;	// 1 - pop up window  ;  2...N - child window
};

//pop up win 1 (level 1)........ first Z order
//		child11 (level 2)
//		child12 (level 2)
//				chilld121 (level 3)
//				chilld122 (level 3)
//				...
//		child3 (level 2)
//pop up win2
//		child21 (level 2)
//		child21 (level 2)
// ....
// ....
//pop up winN ................... last Z order


template<typename CWinFilterTraits = CSCWinFilter>
class CSCWinSpy:  public CHYSingleton<CSCWinSpy<CWinFilterTraits> >
{
public:
	BOOL SnapshotAllWinRect()
	{
		ClearData();

		// cache current window Z order when call this function
		EnumWindows(EnumWindowsSnapshotProc, 1); 
		
		return TRUE;
	}
	
	//get from current Z order of desktop
	HWND GetHWNDByPoint(CPoint pt)
	{
		m_hWndTarget = NULL;
		
		EnumWindows(EnumWindowsRealTimeProc, MAKELPARAM(pt.x, pt.y));
		
		return m_hWndTarget;
	}
	
	CRect GetWinRectByPoint(CPoint ptHit, BOOL bGetInRealTime = FALSE)
	{
		CRect rtRect(0, 0, 0, 0);
		if(bGetInRealTime) //get from current Z order
		{
			HWND hWndTarget = GetHWNDByPoint(ptHit);
			if(hWndTarget != NULL )
			{
				GetWindowRect(hWndTarget, &rtRect);
			}
		}
		else //get from snapshot cache
		{
			GetRectByPointFromSnapshot(ptHit, rtRect);
		}
		
		return rtRect;
	}
	
protected:
	static BOOL CALLBACK EnumWindowsRealTimeProc(HWND hwnd, LPARAM lParam)
	{
		if(!PtInWinRect(hwnd, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))) return TRUE;
		
		if(ShouldWinBeFiltered(hwnd))  return TRUE;
		
		m_hWndTarget = hwnd;
		
		if(CWinFilterTraits::IsTargetPopupWindow()) return FALSE; //this is the target window, exit search
		
		EnumChildWindows(hwnd, EnumChildRealTimeProc, lParam);
		
		return FALSE;
	}
	
	static BOOL CALLBACK EnumChildRealTimeProc(HWND hwnd, LPARAM lParam)
	{
		if(!PtInWinRect(hwnd, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))) return TRUE;
		
		if(ShouldWinBeFiltered(hwnd)) return TRUE;
		
		m_hWndTarget = hwnd;
		EnumChildWindows(hwnd, EnumChildRealTimeProc, lParam);
		
		return FALSE;
	}
	
protected:
	static BOOL CALLBACK EnumWindowsSnapshotProc(HWND hwnd, LPARAM lParam)
	{
		INT nLevel = lParam;
		if(ShouldWinBeFiltered(hwnd))  return TRUE;
		
		SaveSnapshotWindow(hwnd, nLevel);
		
		if(!CWinFilterTraits::IsTargetPopupWindow())
		{
			++nLevel;
			EnumChildWindows(hwnd, EnumChildSnapshotProc, nLevel);
		}
		
		return TRUE;
	}
	
	static BOOL CALLBACK EnumChildSnapshotProc(HWND hwnd, LPARAM lParam)
	{
		INT nLevel = lParam;
		
		if(ShouldWinBeFiltered(hwnd)) return TRUE;
		
		SaveSnapshotWindow(hwnd, nLevel);
		
		++nLevel;
		EnumChildWindows(hwnd, EnumChildSnapshotProc, nLevel);
		
		return TRUE;
	}
	
protected:
	static BOOL PtInWinRect(HWND hWnd, CPoint pt)
	{
		CRect rtWin(0, 0, 0, 0);
		GetWindowRect(hWnd, &rtWin);
		return PtInRect(&rtWin, pt);
	}
	
	static BOOL ShouldWinBeFiltered(HWND hWnd)
	{
		if(CWinFilterTraits::IsFilterWindow(hWnd)) return TRUE;
		
		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		DWORD dwStyleMust = CWinFilterTraits::GetIncludeStyle();
		if((dwStyle & dwStyleMust) != dwStyleMust) return TRUE;
		
		DWORD dwStyleEx = GetWindowLong(hWnd, GWL_EXSTYLE);
		DWORD dwStyleMustNot = CWinFilterTraits::GetExcludeStyleEx();
		if((dwStyleMustNot & dwStyleEx) != 0) return TRUE;
		
		return FALSE;
	}
	
	//find the first window that level is biggest
	static BOOL  GetRectByPointFromSnapshot(CPoint ptHit, CRect& rtRet)
	{
		int nCount = m_arSnapshot.size();
		_ASSERTE(nCount > 0);
		CSCWinInfo* pInfo = NULL;
		CSCWinInfo* pTarget = NULL; 
		
		for(int i=0; i<nCount; ++i)
		{
			pInfo = m_arSnapshot[i];
			_ASSERTE(pInfo != NULL);
			
			//target window is found 
			//and level is not increasing, 
			//that is checking its sibling or parent window, exit search
			if(pTarget != NULL
				&& pInfo->m_nLevel <= pTarget->m_nLevel)
			{
				break;
			}
			
			if(PtInRect(&pInfo->m_rtWin, ptHit))
			{
				if(pTarget == NULL)
				{
					pTarget = pInfo;
				}
				else
				{
					if( pInfo->m_nLevel > pTarget->m_nLevel)
					{
						pTarget = pInfo;
					}
				}
			}
		}
		
		if(pTarget != NULL)
		{
#ifdef _DEBUG
			if(pTarget != NULL)
			{
				HWND hWnd = pTarget->m_hWnd;
				TCHAR szText[128] = {0};
				_sntprintf(szText, 127, _T("GetRectByPointFromSnapshot: pt(%d, %d), hWnd(%x)"),
					ptHit.x, ptHit.y, (UINT)(pInfo->m_hWnd));
				OutputDebugString(szText);
			}
#endif

			rtRet.CopyRect(&pTarget->m_rtWin);
			return TRUE;
		}
		
		return FALSE;
	}
	
	static VOID SaveSnapshotWindow(HWND hWnd, INT nLevel)
	{
		_ASSERTE(hWnd != NULL && IsWindow(hWnd));
		CRect rtWin(0, 0, 0, 0);
		GetWindowRect(hWnd, &rtWin);
		if(rtWin.IsRectEmpty()) return;
		
		CSCWinInfo* pInfo = new CSCWinInfo;
		if(pInfo == NULL) return;
		
		pInfo->m_hWnd = hWnd;
		pInfo->m_nLevel = nLevel;
		pInfo->m_rtWin = rtWin;
		
		m_arSnapshot.push_back(pInfo);
	}
	
	static VOID ClearData()
	{
		int nCount = m_arSnapshot.size();
		for(int i=0; i<nCount; ++i)
		{
			delete m_arSnapshot[i];
		}
		
		m_arSnapshot.clear();
	}
	
protected:
	friend class CHYSingleton<CSCWinSpy>;

	CSCWinSpy() { NULL; }
	~CSCWinSpy() {	ClearData(); }
	
	static HWND m_hWndTarget;
	static std::vector<CSCWinInfo*> m_arSnapshot;
};

template<typename T> HWND CSCWinSpy<T>::m_hWndTarget = NULL;
template<typename T> std::vector<CSCWinInfo*> CSCWinSpy<T>::m_arSnapshot;

