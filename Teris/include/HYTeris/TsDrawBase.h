#pragma once 

#include "TsStdAfx.h"

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


#ifndef WM_MOUSEENTER
   #define WM_MOUSEENTER WM_USER + 253
#endif // WM_MOUSEENTER

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
   END_MSG_MAP()

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

HFONT CreateTsFont(BOOL bBold, int nHeight, BOOL lfItalic = FALSE, BOOL lfUnderline = FALSE, LPCTSTR lpszName = _T("Tahoma"));
void DrawBorder(CDCHandle hDC, RECT& rt, COLORREF clrLeftTop, COLORREF clrRightBottom, int nFrameWidth);
HRGN BitmapToRgn(HBITMAP hBmp, COLORREF cTransparentColor = RGB(255, 0, 255), COLORREF cTolerance = 0);
CSize GetBitmapSize(HBITMAP hbmp);
HFONT GetTsDefultFont(BOOL bBold = FALSE);
VOID FillGradientColor(CDCHandle& dc, CRect& rect, COLORREF clrBegin, COLORREF clrEnd, BOOL bHor = TRUE);
