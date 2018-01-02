#pragma  once 

#include "HYUtilBase.h"

#define CHAIN_MSG_MAP_MEMBER_PTR(m) \
	if(m != NULL)  \
		if(m->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID)) \
			return TRUE;

#define CHAIN_MSG_MAP_MEMBER_REF_PTR(m) \
	if(!m.IsNull())	\
		if(m->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID)) \
			return TRUE;

class CHYMemDC: public CDC
{
public:
	CHYMemDC(HDC hDC, LPRECT pRect = NULL);
	~CHYMemDC();
	
private:
	CDCHandle m_hDCDest;
	CBitmap m_bmpDest;
	CBitmapHandle m_hBmpOld;
	CRect m_rtDraw;
};


// To use it, derive from it and chain it in the message map.
// Make sure to set bHandled to FALSE when handling WM_MOUSEMOVE or
// the WM_MOUSELEAVE message!
template< class T >
class CMouseHover
{
public:   
   bool m_fMouseOver;          // Internal mouse-over state
   bool m_fMouseForceUpdate;   // Update window immediately on event

   CMouseHover() : 
      m_fMouseOver(false),
      m_fMouseForceUpdate(true)
   {
   }

   void SetForceMouseOverUpdate(bool bForce = false)
   {
      m_fMouseForceUpdate = bForce;
   }

   BEGIN_MSG_MAP(CMouseHover)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
      MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
	  MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
   END_MSG_MAP()

   LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
   {
	   bHandled = FALSE;
	   m_fMouseOver = FALSE;	

	   return S_OK;
   }

   LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
      T* pT = static_cast<T*>(this);
      if( !m_fMouseOver )   {
         m_fMouseOver = true;
         pT->SendMessage(WM_MOUSEENTER, wParam, lParam);         
         if( m_fMouseForceUpdate ) {
            pT->Invalidate();
            pT->UpdateWindow();
         }
         _StartTrackMouseLeave(pT->m_hWnd);
      }
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      T* pT = static_cast<T*>(this);
      if( m_fMouseOver ) {
         m_fMouseOver = false;         
         if( m_fMouseForceUpdate ) {
            pT->Invalidate();
            pT->UpdateWindow();
         }
      }
      bHandled = FALSE;
      return 0;
   }
   BOOL _StartTrackMouseLeave(HWND hWnd) const
   {
      ATLASSERT(::IsWindow(hWnd));
      TRACKMOUSEEVENT tme = { 0 };
      tme.cbSize = sizeof(tme);
      tme.dwFlags = TME_LEAVE;
      tme.hwndTrack = hWnd;
      return _TrackMouseEvent(&tme);
   }
   BOOL _CancelTrackMouseLeave(HWND hWnd) const
   {
      TRACKMOUSEEVENT tme = { 0 };
      tme.cbSize = sizeof(tme);
      tme.dwFlags = TME_LEAVE | TME_CANCEL;
      tme.hwndTrack = hWnd;
      return _TrackMouseEvent(&tme);
   }
};


#ifndef LWA_COLORKEY
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002

#define ULW_COLORKEY            0x00000001
#define ULW_ALPHA               0x00000002
#define ULW_OPAQUE              0x00000004
#endif //LWA_COLORKEY

class CHYUICommonFunctionWrapper
{
public:
	static CSize GetBitmapSize(HBITMAP hBmp);
	static HFONT CreateFont(BOOL bBold, int nHeight, BOOL lfItalic  = FALSE , BOOL lfUnderline  = FALSE , LPCTSTR lpszName  = _T("Tahoma") );
	static HFONT GetDefaultFont(BOOL bBold = FALSE);
	static HRGN BitmapToRgn(HBITMAP hBmp, COLORREF cTransparentColor = RGB(255, 0, 255), COLORREF cTolerance = 0);
	static VOID GradientFill(CDCHandle dc, LPRECT pRect, COLORREF clrStart, COLORREF clrEnd, BOOL bVert = FALSE);
	static VOID DrawBmp(CDCHandle dc, CRect* rtDraw, CBitmapHandle bmp, BOOL bStretch = FALSE, BOOL bTransparent = FALSE);
	static VOID DrawBmpInCenter(CDCHandle dc, CRect* pRtDraw, CBitmapHandle bmp, BOOL bTransparent);
	static VOID FillBmp(CDCHandle dc, CRect* rtDraw, CBitmapHandle bmp);
	static CBitmapHandle GetSubBmp(CBitmapHandle bmpTarget, CRect rtTarget);
	static CBitmapHandle GetMiddleStretchBmp(CBitmapHandle bmpSrc, int nLen, BOOL bVert);
	static CBitmapHandle GetMiddleStretchBmp(CBitmapHandle bmpSrc, CSize sizeTarget);
	static VOID MiddleStretchDrawBmp(CDCHandle dc, CRect* rtDraw, CBitmapHandle bmp);
	static VOID DrawFrame(CDCHandle dc, CRect& rtDraw, COLORREF clrFrame, INT nFrameWidth);
	static BOOL SaveBMP2File(HBITMAP hBitmap, LPCTSTR lpszFileName );
	
	static BOOL SetLayeredWindowAttributes( HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags );
	static BOOL UpdateLayeredWindow( HWND hwnd, HDC hdcDst, POINT *pptDst, SIZE *psize,HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);
};

class CHYCommonFunctionWrapper
{
public:
	 static BOOL GetCurrentFilePath(LPTSTR pBuffer, int nBufferLen, LPCTSTR lpszName);
};


//back ground fill color type
#define HY_BKGND_SINGLE_COLOR			1
#define HY_BKGND_GRADIENT_COLOR_HOR		2
#define HY_BKGND_GRADIENT_COLOR_VER		3
#define HY_BKGND_FULL_FILL_BMP			4
#define HY_BKGND_FULL_STRETCH_BMP		5
#define HY_BKGND_MIDDLE_STRETCH_BMP		6
#define HY_BKGND_NO_STRETCH_BMP			7

template<typename T>
class CHYBkgndDrawMap
{
public:
	CHYBkgndDrawMap()
	{
		m_nBkMapMode = HY_BKGND_SINGLE_COLOR;
		m_clrMapBegin = m_clrMapEnd = m_clrMapFilled = GetSysColor(COLOR_WINDOW);
	}

	BEGIN_MSG_MAP(CHYBkgndDrawMap)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	VOID SetBkgndMapType(UINT nType) 
	{
		if(nType != m_nBkMapMode)
		{
			m_nBkMapMode = nType;

			T* pThis = static_cast<T*>(this);
			if(pThis->IsWindow())
			{
				pThis->InvalidateRect(NULL, TRUE);
			}
		}
	}
	
	VOID SetBkMapSingleColor(COLORREF clr)
	{
		if(m_clrMapFilled != clr)
		{
			m_clrMapFilled = clr;

			T* pThis = static_cast<T*>(this);
			if(pThis->IsWindow())
			{
				pThis->InvalidateRect(NULL, TRUE);
			}
		}
	}
	VOID SetBkMapGradientColor(COLORREF clrBegin, COLORREF clrEnd)
	{
		if(m_clrMapBegin != clrBegin
			&& m_clrMapEnd != clrEnd)
		{
			m_clrMapBegin = clrBegin;
			m_clrMapEnd = clrEnd;

			T* pThis = static_cast<T*>(this);
			if(pThis->IsWindow())
			{
				pThis->InvalidateRect(NULL, TRUE);
			}
		}
	}
	VOID SetBkMapBmp(CBitmapHandle bmp)
	{
		m_bmpMapBkgnd = bmp;
		T* pThis = static_cast<T*>(this);
		if(pThis->IsWindow())
		{
			pThis->InvalidateRect(NULL, TRUE);
		}
	}

protected:
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&)
	{
		return TRUE;
	}

	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&)
	{
		T* pThis = static_cast<T*>(this);
		CPaintDC dc(pThis->m_hWnd);
		CHYMemDC memDC(dc);

		CRect rtClient;
		pThis->GetClientRect(&rtClient);

		switch(m_nBkMapMode)
		{
		case HY_BKGND_SINGLE_COLOR:
			memDC.FillSolidRect(&rtClient, m_clrMapFilled);
			break;
		
		case HY_BKGND_GRADIENT_COLOR_HOR:
			CHYUICommonFunctionWrapper::GradientFill(memDC.m_hDC, &rtClient, m_clrMapBegin, m_clrMapEnd, FALSE);
			break;
		
		case HY_BKGND_GRADIENT_COLOR_VER:
			CHYUICommonFunctionWrapper::GradientFill(memDC.m_hDC, &rtClient, m_clrMapBegin, m_clrMapEnd, TRUE);
			break;

		case HY_BKGND_FULL_FILL_BMP:
			{
 				_ASSERTE(!m_bmpMapBkgnd.IsNull());
				CHYUICommonFunctionWrapper::FillBmp(memDC.m_hDC, &rtClient, m_bmpMapBkgnd);
			}
			break;
		
		case HY_BKGND_FULL_STRETCH_BMP:
			{
				CHYUICommonFunctionWrapper::DrawBmp(memDC.m_hDC, &rtClient, m_bmpMapBkgnd, TRUE);	
			}
			break;

		case HY_BKGND_MIDDLE_STRETCH_BMP:
			{
				CHYUICommonFunctionWrapper::MiddleStretchDrawBmp(memDC.m_hDC, &rtClient, m_bmpMapBkgnd);		
			}
			break;

		case HY_BKGND_NO_STRETCH_BMP:
			{
				CHYUICommonFunctionWrapper::DrawBmp(memDC.m_hDC, &rtClient, m_bmpMapBkgnd, FALSE, TRUE);
			}
			break;

		default:
			_ASSERTE(FALSE);
			break;

		}
		
		
		pThis->SendMessage(WM_HY_PAINT, (WPARAM)(memDC.m_hDC), NULL);

		return S_OK;
	}

protected:
	DWORD m_nBkMapMode;
	COLORREF m_clrMapFilled;
	COLORREF m_clrMapBegin;
	COLORREF m_clrMapEnd;
	CBitmapHandle m_bmpMapBkgnd;
};


template<typename T>
class CHYBmpWindowMap: public CHYBkgndDrawMap<T>
{
	BEGIN_MSG_MAP(CHYBmpWindowMap)
		CHAIN_MSG_MAP(CHYBkgndDrawMap<T>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_MOVE, OnMove)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()

	CHYBmpWindowMap()
	{
		SetBkgndMapType(HY_BKGND_NO_STRETCH_BMP);
	}

protected:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		T* pThis = static_cast<T*>(this);
		_ASSERTE(pThis->GetStyle() & WS_POPUP);
		bHandled = FALSE;
		_AdjustLayout();
		return S_OK;
	}
	LRESULT OnMove(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
	//	_AdjustLayout();
		return S_OK;
	}
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
		_AdjustLayout();
		return S_OK;
	}

protected:
	VOID _AdjustLayout()
	{
		T* pThis = static_cast<T*>(this);
		if(!pThis->IsWindow()) return;

		if(m_bmpMapBkgnd.IsNull())	return;
		

		CSize sizeWin = CHYUICommonFunctionWrapper::GetBitmapSize(m_bmpMapBkgnd);
		pThis->SetWindowPos(NULL, 0, 0, sizeWin.cx, sizeWin.cy, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);

		HRGN hRgn = CHYUICommonFunctionWrapper::BitmapToRgn((HBITMAP)m_bmpMapBkgnd);
		if(hRgn != NULL)
		{
			pThis->SetWindowRgn(hRgn);
		}
		else
		{
			_ASSERTE(FALSE);
		}
	}

protected:
	DWORD m_dwBMPWinStyle;
	CBitmap m_bmpWinStretched;
};



#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#endif //WS_EX_LAYERED

#ifndef WS_EX_NOACTIVATE
#define WS_EX_NOACTIVATE        0x08000000L
#endif //WS_EX_NOACTIVATE

// class for part of window transparent feature
//
typedef CWinTraits<WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW> CAeroWinTraits;
template<typename T, COLORREF clrMask = RGB(255, 0, 255), BYTE alpha = 50, BOOL bUserRender = FALSE>
class CHYAeroWindowMap: public CMessageMap
{
protected:
	class CAeroWindow: public CWindowImpl<CAeroWindow, CWindow, CAeroWinTraits>
	{
	public:			
		BEGIN_MSG_MAP(CAeroWindow)
			MESSAGE_HANDLER(WM_CREATE, OnCreate)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
			MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMsg)
			MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		END_MSG_MAP()
		
		CAeroWindow() { m_clrAero = RGB(0, 0, 0); m_hWndOver = NULL; }

		COLORREF m_clrAero;
		CBitmapHandle m_bmpAero;
		HWND m_hWndOver;

	protected:
		LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
		{
			_ASSERTE(GetStyle() & WS_POPUP);
			_ASSERTE(GetExStyle() & WS_EX_LAYERED);
			
			return S_OK;
		}
		LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&)
		{
			return TRUE;
		}
		LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&)
		{
			CPaintDC dc(m_hWnd);
			CRect rtClient;
			GetClientRect(&rtClient);

			if(!m_bmpAero.IsNull())
			{
				CHYUICommonFunctionWrapper::DrawBmp(dc.m_hDC, &rtClient, m_bmpAero, TRUE, TRUE);
			}
			else
			{
				dc.FillSolidRect(&rtClient, m_clrAero);
			}

			return S_OK;
		}
		LRESULT OnMouseMsg(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			if(m_hWndOver != NULL)
			{
				bHandled = TRUE;
				return ::SendMessage(m_hWndOver, nMsg, wParam, lParam);
			}

			bHandled = FALSE;
			return S_OK;
		}
		LRESULT OnSetCursor(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			if(m_hWndOver != NULL)
			{
				bHandled = TRUE;
				return ::SendMessage(m_hWndOver, nMsg, wParam, lParam);
			}
			
			bHandled = FALSE;
			return S_OK;
		}
	};


public:
	BEGIN_MSG_MAP(CHYAeroWindowMap)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_MOVE, OnMove)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
	END_MSG_MAP()
	
	VOID SetAeroColor(COLORREF clr) { m_WinAero.m_clrAero = clr; }
	VOID SetAeroBMP(CBitmapHandle bmp) { m_WinAero.m_bmpAero = bmp; }


protected:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pThis = static_cast<T*>(this);
		_ASSERTE(pThis->GetStyle() & WS_POPUP);
		m_hWndParent = pThis->GetWindow(GW_OWNER);
		
		DWORD dwExStyle = pThis->GetExStyle();
		BOOL bTopMost = (dwExStyle & WS_EX_TOPMOST);
		BOOL bTransparent = (dwExStyle & WS_EX_TRANSPARENT);

		m_WinAero.m_hWndOver = pThis->m_hWnd;

		CRect rtWin;
		pThis->GetWindowRect(&rtWin);
		m_WinAero.Create(m_hWndParent, rtWin);
		_ASSERTE(m_WinAero.IsWindow());
		if(bTopMost)
		{
			m_WinAero.SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE);
		}
		if(bTransparent)
		{
			m_WinAero.SetWindowLong(GWL_EXSTYLE, (m_WinAero.GetExStyle() | WS_EX_TRANSPARENT));
		}
		CHYUICommonFunctionWrapper::SetLayeredWindowAttributes(m_WinAero, 0, alpha,  LWA_ALPHA);

		::SetWindowLong(pThis->m_hWnd, GWL_HWNDPARENT, (LONG)m_WinAero.m_hWnd);
		pThis->ModifyStyleEx(0, WS_EX_LAYERED);

		if(!bUserRender)
		{
			CHYUICommonFunctionWrapper::SetLayeredWindowAttributes(pThis->m_hWnd, clrMask, 0,  LWA_COLORKEY);
		}

		return S_OK;
	}

	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pThis = static_cast<T*>(this);
		::SetWindowLong(pThis->m_hWnd, GWL_HWNDPARENT, (LONG)m_hWndParent);

		if(m_WinAero.IsWindow())
		{
			m_WinAero.DestroyWindow();
		}

		m_WinAero.m_hWndOver = NULL;

		return S_OK;
	}
	
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
		LayOut();

		return S_OK;		
	}

	LRESULT OnMove(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
		LayOut(FALSE);

		return S_OK;
	}

	LRESULT OnShowWindow(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;

		INT nCmd = SW_SHOW;
		if(wParam == FALSE) nCmd = SW_HIDE;
		if(m_WinAero.IsWindow())
		{
		   m_WinAero.ShowWindow(nCmd);
		}
				
		return S_OK;
	}

	LRESULT OnWindowPosChanged(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		
		LayOut(FALSE);
		
		return S_OK;
	}
	


	VOID LayOut(BOOL bUpdateRgn = TRUE)
	{
		CRect rtWin;
		T* pThis = static_cast<T*>(this);

		pThis->GetWindowRect(&rtWin);
		if(m_WinAero.IsWindow())
		{
			m_WinAero.SetWindowPos(NULL, rtWin.left, rtWin.top, rtWin.Width(), rtWin.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		
			if(bUpdateRgn)
			{
				CRgn hRgn = CreateRectRgn(0,0,0,0);
				int regionType = pThis->GetWindowRgn( hRgn);
				if (regionType != ERROR) 
				{ 
					m_WinAero.SetWindowRgn(hRgn.Detach());
				}
			}
		}
	}

protected:
	CAeroWindow m_WinAero;
	HWND m_hWndParent;
};



