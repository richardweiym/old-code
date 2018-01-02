#include "TsButton.h"


CTsButton::CTsButton()
{
	m_bChecked = BST_UNCHECKED;
	m_dwBtnStyle = TS_BTN_AUTO_FIT_STYLE | TS_BTN_CENTER_TEXT_STYLE;
}

DWORD CTsButton::GetBtnStyle() 
{ 
	return m_dwBtnStyle; 
}

DWORD CTsButton::SetBtnStyle(DWORD dwStyle)
{
	if((dwStyle&TS_BTN_AUTO_FIT_STYLE) && (dwStyle&TS_BTN_RGN_STYLE))
	{
		_ASSERTE(!_T("should not use the two style together"));
	}

	DWORD dwRet(m_dwBtnStyle);
	m_dwBtnStyle = dwStyle;
	
	if(IsWindow())
	{
		Invalidate();
	}

	return dwRet;
}

void CTsButton::SetBkgndBitmap(HBITMAP bmp)
{
	_ASSERTE(GetObjectType(bmp) == OBJ_BITMAP);
	m_bmpBkgnd = bmp;
	
	if(IsWindow())
	{
		Invalidate();
	}
}

CBitmapHandle CTsButton::GetBkgndBitmap()
{
	return m_bmpBkgnd;
}

void CTsButton::SetBtnImagelist(HIMAGELIST img)
{
	m_imgBtn = img;

	if(IsWindow())
	{
		Invalidate();
	}
}

CImageList CTsButton::GetBtnImagelist()
{
	return m_imgBtn;
}

LRESULT CTsButton::OnSetCheck(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	if(m_bChecked != (INT)wParam)
	{
		m_bChecked = wParam;
		if(IsWindow())
		{
			Invalidate();
		}
	}

	return S_OK;
}

LRESULT CTsButton::OnGetCheck(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return m_bChecked;
}

void CTsButton::SetTooltipText(LPCTSTR strText)
{
	if(strText == NULL)
	{
		if(m_tip.IsWindow())
			m_tip.Activate(FALSE);
		m_strTooltip = _T("");
		return;
	}	

	m_strTooltip = strText;

	if(m_tip.IsWindow() && m_strTooltip != _T(""))
	{
		m_tip.Activate(TRUE);
		m_tip.AddTool(m_hWnd, (LPCTSTR)m_strTooltip);
	}
}

LRESULT CTsButton::OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MSG msg = { m_hWnd, uMsg, wParam, lParam };
	if(m_tip.IsWindow())
		m_tip.RelayEvent(&msg);
	bHandled = FALSE;
	return 1;
}

LRESULT CTsButton::OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  return 1;   // no background needed
}

BOOL CTsButton::SubclassWindow(HWND hWnd)
{
  ATLASSERT(m_hWnd==NULL);
  ATLASSERT(::IsWindow(hWnd));
#ifdef _DEBUG
  // Check class
  TCHAR szBuffer[16];
  if( ::GetClassName(hWnd, szBuffer, sizeof(szBuffer)/sizeof(TCHAR)) ) {
     ATLASSERT(::lstrcmpi(szBuffer, CButton::GetWndClassName())==0);
  }
#endif
  BOOL bRet = CWindowImpl<CTsButton, CButton, CTsButtonTraits>::SubclassWindow(hWnd);
  if( bRet ) _Init();
  return bRet;		
}

HWND CTsButton::UnsubclassWindow(BOOL bForce )
{
	_Release();
	return CWindowImpl<CTsButton, CButton, CTsButtonTraits>::UnsubclassWindow(bForce);
}

HFONT CTsButton::GetTextFont()
{
	HFONT hRet(GetTsDefultFont(FALSE));
	if(!m_hFont.IsNull())
	{
		hRet = m_hFont;
	}

	return hRet;
}

LRESULT CTsButton::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
  LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
  _Init();
  return lRes;
}

LRESULT CTsButton::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
   LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
   _Release();
   return lRes;
}

LRESULT CTsButton::OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	_AdjustSize();

	return S_OK;
}

LRESULT CTsButton::OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT) lParam;
	ATLASSERT(lpDIS->CtlType==ODT_BUTTON);
	CHYMemDC dcMem(lpDIS->hDC);
	CDCHandle dc(dcMem.m_hDC);
	CRect rc = lpDIS->rcItem;
	bool bSelected = (lpDIS->itemState & ODS_SELECTED)!=0;
	bool bDisabled = (lpDIS->itemState & (ODS_DISABLED|ODS_GRAYED))!=0;
	bool bHover = m_fMouseOver;
	bool bChecked = (m_bChecked != BST_UNCHECKED);

	_DrawBkgnd(dc, &rc);	

	int nIndex(_index_normal);
	if(bChecked)	nIndex = _index_checked;
	if(bHover)		nIndex = _index_hover;
	if(bSelected)	nIndex = _index_pressed;
	if(bDisabled)	nIndex = _index_disable;

	if(nIndex >= m_imgBtn.GetImageCount())
	{
		_ASSERTE(FALSE);
		nIndex = _index_normal;
	}

	CRect rtBmp = _GetBitmapRect();
	if(!rtBmp.IsRectEmpty() && !m_imgBtn.IsNull())
	{
		m_imgBtn.Draw(dc, nIndex, rtBmp.left, rtBmp.top, ILD_TRANSPARENT);
	}
	else
	{
		_ASSERTE(FALSE);
	}

	DWORD dwBtnStyle = GetBtnStyle();
	if(dwBtnStyle&TS_BTN_CENTER_TEXT_STYLE)
	{
		CRect rtText = _GetTextRect();
		if(!rtText.IsRectEmpty())
		{	
			HFONT hOldFont = dc.SelectFont(GetTextFont());
			TCHAR szText[128] = {0};
			GetWindowText(szText, sizeof(szText) / sizeof(TCHAR) - 1);
			dc.SetBkMode(TRANSPARENT);
			dc.DrawText(szText, lstrlen(szText), &rtText, DT_CENTER | DT_END_ELLIPSIS | DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);
			dc.SelectFont(hOldFont);
		}
	}

  return S_OK;
 }

void CTsButton::_DrawBkgnd(CDCHandle dc, CRect* lpRect)
{
	_ASSERTE(IsWindow());
	_ASSERTE(!m_bmpBkgnd.IsNull());
	if(m_bmpBkgnd.IsNull()) return;
	
	CDC memDC;
	memDC.CreateCompatibleDC(dc);
	HBITMAP hOldBmp = memDC.SelectBitmap(m_bmpBkgnd);
	CSize szBmp = GetBitmapSize(m_bmpBkgnd);

	if(GetBtnStyle()&TS_BTN_RGN_STYLE)
	{
		::TransparentBlt(dc, lpRect->left, lpRect->top, lpRect->Width(), lpRect->Height(), \
			memDC, 0, 0, szBmp.cx, szBmp.cy, RGB(255, 0, 255));	
	}
	else if(GetBtnStyle()&TS_BTN_AUTO_FIT_STYLE)
	{
		::BitBlt(dc, lpRect->left, lpRect->top, lpRect->Width(), lpRect->Height(), \
			memDC, 0, 0, SRCCOPY);	
	}
	else
	{
		::StretchBlt(dc, lpRect->left, lpRect->top, lpRect->Width(), lpRect->Height(), \
			memDC, 0, 0, szBmp.cx, szBmp.cy, SRCCOPY);
	}	

	memDC.SelectBitmap(hOldBmp);
}

void CTsButton::_AdjustSize()
{
	_ASSERTE(IsWindow());
	_ASSERTE(!m_bmpBkgnd.IsNull());

	if(!m_bmpBkgnd.IsNull())
	{
		if((GetBtnStyle()&TS_BTN_AUTO_FIT_STYLE)
			||(GetBtnStyle()&TS_BTN_RGN_STYLE) )
		{
			CSize szBmp = GetBitmapSize(m_bmpBkgnd);
			CRect rtWin(0, 0, 0, 0);
			GetWindowRect(&rtWin);

			_ASSERTE(!(GetBtnStyle()&WS_POPUP));

			HWND hParent = GetParent();
			::ScreenToClient(hParent, (LPPOINT)&rtWin);
			MoveWindow(rtWin.left, rtWin.top, szBmp.cx, szBmp.cy, TRUE);
		}

		if(GetBtnStyle()&TS_BTN_RGN_STYLE)
		{
			CRgnHandle rgn = BitmapToRgn(m_bmpBkgnd);
			SetWindowRgn(rgn);
		}	
	}

	Invalidate();
}

void CTsButton::_Init()
{
	ModifyStyle(0, BS_OWNERDRAW);
	
	_ASSERTE(!m_tip.IsWindow());
	if(!m_tip.IsWindow())
	{
		m_tip.Create(m_hWnd);
	}
	_ASSERTE(m_tip.IsWindow());
	if(m_tip.IsWindow() && m_strTooltip != _T(""))
	{
		m_tip.Activate(TRUE);
		m_tip.AddTool(m_hWnd, (LPCTSTR)m_strTooltip);
	}

	_ASSERTE(IsWindow());
	if(IsWindow())
	{
		_AdjustSize();
	}
}

void CTsButton::_Release()
{
	if(m_tip.IsWindow())
	{
		m_tip.DelTool(m_hWnd);
		m_tip.DestroyWindow();
	}
}

CRect CTsButton::_GetTextRect()
{
	CRect rtRet(0, 0, 0, 0);

	CSize szText = _GetTextSize();
	if(szText.cx == 0 || szText.cy == 0)
	{
		return rtRet;
	}

	CRect rtClient(0, 0, 0, 0);
	GetClientRect(&rtClient);

	DWORD dwBtnStyle = GetBtnStyle();
	if(dwBtnStyle&TS_BTN_CENTER_TEXT_STYLE)
	{
		if(rtClient.Width() > szText.cx)
		{
			rtRet.left = (rtClient.Width() - szText.cx) / 2;
			rtRet.right = rtRet.left + szText.cx;
		}
		else
		{
			rtRet.left = 0;
			rtRet.right = rtClient.Width();
		}

		if(rtClient.Height() > szText.cy)
		{
			rtRet.top = (rtClient.Height() - szText.cy) / 2;
			rtRet.bottom = rtRet.top + szText.cy;
		}
		else
		{
			rtRet.top = 0;
			rtRet.bottom = rtRet.top + rtClient.Height();
		}
	}

	return rtRet;
}

CRect CTsButton::_GetBitmapRect()
{
	CRect rtRet(0, 0, 0, 0);
	
	CSize szBmp = _GetBitmapSize();
	if(szBmp.cx == 0 || szBmp.cy == 0)
	{
		return rtRet;
	}

	CRect rtClient(0, 0, 0, 0);
	GetClientRect(&rtClient);

	DWORD dwBtnStyle = GetBtnStyle();
	if(dwBtnStyle&TS_BTN_RGN_STYLE)
	{
		if(rtClient.Width() > szBmp.cx)
		{
			rtRet.left = (rtClient.Width() - szBmp.cx) / 2;
			rtRet.right = rtRet.left + szBmp.cx;
		}
		else
		{
			rtRet.left = 0;
			rtRet.right = rtClient.Width();
		}

		if(rtClient.Height() > szBmp.cy)
		{
			rtRet.top = (rtClient.Height() - szBmp.cy) / 2;
			rtRet.bottom = rtRet.top + szBmp.cy;
		}
		else
		{
			rtRet.top = 0;
			rtRet.bottom = rtRet.top + rtClient.Height();
		}
	}
	else
	{
		rtRet = rtClient;
	}

	return rtRet;
}

CSize CTsButton::_GetTextSize()
{
	CSize szRet(0, 0);

	TCHAR szBuffer[128] = {0};
	GetWindowText(szBuffer, sizeof(szBuffer) / sizeof(TCHAR) - 1);
	if(szBuffer[0] != _T('\0'))
	{
		CClientDC dc(m_hWnd);
		HFONT hOldFnt = dc.SelectFont(GetTextFont());
		CRect rtText(0, 0, 0, 0);
		dc.DrawText(szBuffer, lstrlen(szBuffer), &rtText, DT_LEFT | DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);
		szRet.cx = rtText.Width();
		szRet.cy = rtText.Height();
		dc.SelectFont(hOldFnt);
	}
	
	return szRet;
}

CSize CTsButton::_GetBitmapSize()
{
	CSize szRet(0, 0);
	if(!m_imgBtn.IsNull())
	{
		m_imgBtn.GetIconSize(szRet);
	}

	return szRet;
}