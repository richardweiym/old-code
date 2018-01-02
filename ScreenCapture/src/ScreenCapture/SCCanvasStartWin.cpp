#include "SCCanvasStartWin.h"
#include "SCCanvasSkin.h"
#include "SCIDDef.h"
#include "SCCanvasWindow.h"
#include "SCCanvasData.h"
#include "SCRes.h"

#define SC_START_HOT_KEY_ID  100 

CSCCanvasStartWin::CSCCanvasStartWin()
{
	SetBkgndMapType(HY_BKGND_SINGLE_COLOR);
	SetBkMapSingleColor(SC_MASK_COLOR);
}
CSCCanvasStartWin::~CSCCanvasStartWin()
{
	FreeResource();
}

LRESULT CSCCanvasStartWin::OnCommand(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	WORD nCmdID = LOWORD(wParam);

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(nMsg, wParam, lParam);

	return S_OK;
}

LRESULT CSCCanvasStartWin::OnNCCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;

	LoadResource();
	SetAeroBMP(m_bmpAero.m_hBitmap);

	return S_OK;
}

LRESULT CSCCanvasStartWin::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	SetupHotKey();
	CreateControl();
	LayoutControl();

	//SendMessage(WM_COMMAND, MAKEWPARAM(SC_START_CAPTURE_BTN_ID, 0), 0);

	return S_OK;
}

LRESULT CSCCanvasStartWin::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	UnInstallHotKey();
	DestroyControl();
	FreeResource();

	return S_OK;
}
LRESULT CSCCanvasStartWin::OnSize(UINT, WPARAM, LPARAM, BOOL&)
{
	LayoutControl();

	return S_OK;
}

LRESULT CSCCanvasStartWin::OnHotKey(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if(wParam == SC_START_HOT_KEY_ID)
	{
		SendMessage(WM_COMMAND, MAKEWPARAM(SC_START_CAPTURE_BTN_ID, 0), 0);
	}

	return S_OK;
}

LRESULT CSCCanvasStartWin::OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL&)
{	
	CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	::ClientToScreen(m_hWnd, &pt);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pt.x, pt.y));

	return S_OK;
}
BOOL CSCCanvasStartWin::SetupHotKey()
{
	return ::RegisterHotKey(m_hWnd, SC_START_HOT_KEY_ID, NULL, VK_F8);
}

VOID CSCCanvasStartWin::UnInstallHotKey()
{
	::UnregisterHotKey(m_hWnd, SC_START_HOT_KEY_ID);	
}

VOID CSCCanvasStartWin::LoadResource()
{
	FreeResource();

	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();

	m_imgStartBtn = pSkin->GetStartBtnImg();
	m_bmpStartBtnBK = pSkin->GetStartBtnBK();
	m_strStartBtnText = pSkin->GetStartBtnText();

	m_imgCloseBtn = pSkin->GetCloseBtnImg();
	m_bmpCloseBtnBK = pSkin->GetCloseBtnBK();
	m_strCloseBtnTip = pSkin->GetCloseBtnTip();

	m_bmpAero = pSkin->GetCoverShadow();
	m_fntBtn = CHYUICommonFunctionWrapper::CreateFont(TRUE, -12, FALSE, FALSE, pSkin->GetDefaultFontName());
}

VOID CSCCanvasStartWin::FreeResource()
{
	if(!m_imgStartBtn.IsNull())
	{
		m_imgStartBtn.Destroy();
	}

	if(!m_bmpStartBtnBK.IsNull())
	{
		m_bmpStartBtnBK.DeleteObject();
	}

	if(!m_imgCloseBtn.IsNull())
	{
		m_imgCloseBtn.Destroy();
	}

	if(!m_bmpCloseBtnBK.IsNull())
	{
		m_bmpCloseBtnBK.DeleteObject();
	}

	if(!m_bmpAero.IsNull())
	{
		m_bmpAero.DeleteObject();
	}

	if(!m_fntBtn.IsNull())
	{
		m_fntBtn.DeleteObject();
	}
}

VOID CSCCanvasStartWin::CreateControl()
{
	_ASSERTE(!m_StartBtn.IsWindow());
	m_StartBtn.SetTextFont((HFONT)m_fntBtn);
//	m_StartBtn.SetTextColor(RGB(255, 255, 255));
	m_StartBtn.SetBkgndBitmap(m_bmpStartBtnBK);
	m_StartBtn.SetBtnImagelist(m_imgStartBtn);
	m_StartBtn.SetBtnStyle(TS_BTN_RGN_STYLE | TS_BTN_CENTER_TEXT_STYLE);
	m_StartBtn.Create(m_hWnd, CRect(0, 0, 0, 0), m_strStartBtnText, 0, 0, SC_START_CAPTURE_BTN_ID);

	_ASSERTE(!m_CloseBtn.IsWindow());
	m_CloseBtn.SetBkgndBitmap(m_bmpCloseBtnBK);
	m_CloseBtn.SetBtnImagelist(m_imgCloseBtn);
	m_CloseBtn.SetBtnStyle(TS_BTN_RGN_STYLE | TS_BTN_CENTER_TEXT_STYLE);
	m_CloseBtn.SetTooltipText(m_strCloseBtnTip);
	m_CloseBtn.Create(m_hWnd, CRect(0, 0, 0, 0), NULL, 0, 0, SC_CLOSE_CAPTURE_BTN_ID);
}

VOID CSCCanvasStartWin::DestroyControl()
{
	if(m_StartBtn.IsWindow())
	{
		m_StartBtn.DestroyWindow();
	}

	if(m_CloseBtn.IsWindow())
	{
		m_CloseBtn.DestroyWindow();
	}
}

VOID CSCCanvasStartWin::LayoutControl()
{
	CRect rtClient;
	GetClientRect(&rtClient);
	
	if(rtClient.Width() > 0 && rtClient.Height() > 0)
	{
		CRgnHandle rgnWin = CreateRoundRectRgn(rtClient.left, rtClient.top, rtClient.right, rtClient.bottom, 20, 20);
		SetWindowRgn(rgnWin);
	}

	m_StartBtn.CenterWindow();
	
	const int right_offset = 5;
	const int top_offset = 5;
	CRect rtBtn;
	m_CloseBtn.GetWindowRect(&rtBtn);
	int nX = rtClient.Width() - rtBtn.Width() - right_offset;
	int nY = top_offset;
	m_CloseBtn.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}



CSCHelpTipWindow::CSCHelpTipWindow()
:m_rtSelect(0, 0, 0, 0), m_clrCursor(RGB(0, 0, 0))
{
	SetAeroColor(RGB(0, 0, 0));	
	SetBkgndMapType(HY_BKGND_SINGLE_COLOR);
	SetBkMapSingleColor(RGB(0, 0, 0));
}

VOID CSCHelpTipWindow::UpdateRender()
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	CPoint ptCursor(0, 0);
	GetCursorPos(&ptCursor);
	CRect rtSelect = pData->GetSelectRect();

	CPoint ptTemp(ptCursor);
	HWND hWndCanvas = GetCanvasWindow()->m_hWnd;
	CClientDC dc(hWndCanvas);
	::ScreenToClient(hWndCanvas, &ptTemp);
	COLORREF clrCurrent = dc.GetPixel(ptTemp.x, ptTemp.y);

	m_ptCursor = ptCursor;
	m_clrCursor	 = clrCurrent;
	m_rtSelect = rtSelect;

	CPoint ptDest(0, 0);
	GetIdealWinPos(ptDest);	
	SetWindowPos(NULL, ptDest.x, ptDest.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	InvalidateRect(NULL, TRUE);
	UpdateWindow();
}

LRESULT CSCHelpTipWindow::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	LoadResource();

	return S_OK;
}

LRESULT CSCHelpTipWindow::OnPaint(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	CDCHandle dc((HDC)wParam);
	
	DrawText(dc);
	DrawZoomBmp(dc);

	return S_OK;
}
VOID CSCHelpTipWindow::GetTextRect(CRect& rtText)
{
	const int c_text_len = 190;
	GetClientRect(&rtText);
	rtText.right = c_text_len;
}

VOID CSCHelpTipWindow::GetZoomRect(CRect& rtZoom)
{
	const int c_zoom_len = 	80;

	CRect rtClient;
	GetClientRect(&rtClient);

	rtZoom.right = rtClient.right - 10;
	rtZoom.top = 10;
	rtZoom.bottom = rtZoom.top + c_zoom_len;
	rtZoom.left = rtZoom.right - c_zoom_len;
}

VOID CSCHelpTipWindow::DrawText(CDCHandle dc)
{
	TCHAR szTip[256] = {0};
	_sntprintf(szTip, sizeof(szTip) / sizeof(TCHAR), m_strHelpTip, 
		m_rtSelect.Width(), m_rtSelect.Height(), GetRValue(m_clrCursor),
		GetGValue(m_clrCursor), GetBValue(m_clrCursor));

	CRect rtText;
	GetTextRect(rtText);

	CFontHandle fntOld = dc.SelectFont(CHYUICommonFunctionWrapper::GetDefaultFont());
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255, 255, 255));
	dc.DrawText(szTip, lstrlen(szTip), &rtText, DT_LEFT);
	dc.SelectFont(fntOld);	
}

VOID CSCHelpTipWindow::DrawZoomBmp(CDCHandle dc)
{
	CRect rtZoom;
	GetZoomRect(rtZoom);
	
	dc.FillSolidRect(&rtZoom, RGB(1, 1, 1)); //should not use RGB(0, 0, 0), it is mask color

	INT nSrcWidth = rtZoom.Width() / SC_HELP_ZOOM_RATE;

	CRect rtSrc(m_ptCursor, m_ptCursor);
	rtSrc.InflateRect(nSrcWidth / 2, nSrcWidth / 2);
//	for multi-monitor support
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(!pCanvas.IsNull() && pCanvas->IsWindow())
	{
		CRect rtWindow;
		pCanvas->GetWindowRect(&rtWindow);
		rtSrc.OffsetRect(-rtWindow.left, -rtWindow.top);
	}

	CDCHandle dcSrc = GetCanvasData()->GetCopyDataDC();
	if(!dcSrc.IsNull())
	{
		dc.TransparentBlt(rtZoom.left, rtZoom.top, rtZoom.Width(), rtZoom.Height(),
			dcSrc, rtSrc.left, rtSrc.top, rtSrc.Width(), rtSrc.Height(), RGB(0, 0, 0));
	}
}

VOID CSCHelpTipWindow::GetIdealWinPos(CPoint& pt)
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	CSize sizeScreen = pData->GetScreenSize();

	CRect rtWin;
	GetClientRect(&rtWin);
	const int c_nOffset = 10;

	pt = m_rtSelect.TopLeft();
	pt.Offset(c_nOffset, c_nOffset);
	if(!m_rtSelect.IsRectEmpty())
	{
		if(m_rtSelect.top - c_nOffset > rtWin.Height()) //top
		{
			pt.y = m_rtSelect.top - c_nOffset - rtWin.Height();
			pt.x = m_rtSelect.left;
		}
		else if(sizeScreen.cy - c_nOffset - m_rtSelect.bottom > rtWin.Height()) //bottom
		{
			pt.y = m_rtSelect.bottom + c_nOffset;
			pt.x = m_rtSelect.left;
		}
		else if(m_rtSelect.left - c_nOffset > rtWin.Width()) //left
		{
			pt.y = m_rtSelect.top;
			pt.x = m_rtSelect.left - c_nOffset - rtWin.Width();
		}
		else if(sizeScreen.cx - c_nOffset - m_rtSelect.right > rtWin.Width()) //right
		{
			pt.y = m_rtSelect.top;
			pt.x = m_rtSelect.right + c_nOffset;
		}
		else //in select rect
		{
			CPoint ptCursor;
			GetCursorPos(&ptCursor);
			CRect rtTemp(pt.x, pt.y, pt.x + rtWin.Width(), pt.y + rtWin.Height());

			//	for multi-monitor support
			CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
			if(!pCanvas.IsNull() && pCanvas->IsWindow())
			{
				CRect rtWindow;
				pCanvas->GetWindowRect(&rtWindow);
				rtTemp.OffsetRect(rtWindow.left, rtWindow.top);
			}

			if(rtTemp.PtInRect(ptCursor)) //move to right-top in select rect
			{
				pt.x = m_rtSelect.right - c_nOffset - rtWin.Width();
				pt.y = m_rtSelect.top + c_nOffset;
			}
		}

		if(pt.x + rtWin.Width() > sizeScreen.cx) //right frame is out of screen
		{
			pt.x = sizeScreen.cx - c_nOffset - rtWin.Width();
		}
		else if(pt.x <= 0)
		{
			pt.x = c_nOffset;
		}

	//	for multi-monitor support
		CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
		if(!pCanvas.IsNull() && pCanvas->IsWindow())
		{
			CRect rtWindow;
			pCanvas->GetWindowRect(&rtWindow);
			pt.x += rtWindow.left;
			pt.y +=	rtWindow.top;
		}
	}	
}

VOID CSCHelpTipWindow::LoadResource()
{
		m_strHelpTip =  GetCanvasSkin()->GetResString(IDS_HELP_TIP_FORMAT);
}
void CSCHelpTipWindow::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == UI_WPARAM_LAN_CHANGED)
	{
		LoadResource();

		if(IsWindowVisible())
		{
			UpdateRender();
		}
	}

}




CSCInfoBubbleTip::CSCInfoBubbleTip()
{
}

CSCInfoBubbleTip::~CSCInfoBubbleTip()
{
	FreeResource();
}

LRESULT CSCInfoBubbleTip::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	LoadResource();
	ApplyResource();

	return S_OK;
}

LRESULT CSCInfoBubbleTip::OnPaint(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	CDCHandle dc((HDC)wParam);
	
	CRect rtClient;
	GetClientRect(&rtClient);

	CFontHandle fntOld = dc.SelectFont(m_fntText);

	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(m_strText, m_strText.GetLength(), &rtClient, 
		DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER | DT_VCENTER);

	dc.SelectFont(fntOld);

	return S_OK;
}

VOID CSCInfoBubbleTip::LoadResource()
{
	FreeResource();

	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	m_bmpBK = pSkin->GetInforBubbleBk();
	
	CRefPtr<CSCCanvasData> pConvasData = GetCanvasData();
	CRect rtSelect = pConvasData->GetSelectRect();
	if(rtSelect.IsRectEmpty())
	{
		m_strText = pSkin->GetResString(IDS_IMAGE_COPY_TIP2);
	}
	else
	{
		m_strText = pSkin->GetResString(IDS_IMAGE_COPY_TIP);
	}

	m_fntText = CHYUICommonFunctionWrapper::CreateFont(TRUE, -12, FALSE, FALSE, pSkin->GetDefaultFontName());
}

VOID CSCInfoBubbleTip::FreeResource()
{
	if(!m_bmpBK.IsNull())
	{
		m_bmpBK.DeleteObject();
	}

	if(!m_fntText.IsNull())
	{
		m_fntText.DeleteObject();
	}
}

VOID CSCInfoBubbleTip::ApplyResource()
{
	SetBkMapBmp((HBITMAP)m_bmpBK);

	InvalidateRect(NULL, TRUE);
}

void CSCInfoBubbleTip::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == UI_WPARAM_SKIN_CHANGED
		|| wParam == UI_WPARAM_LAN_CHANGED)
	{
		LoadResource();
		ApplyResource();
	}

}