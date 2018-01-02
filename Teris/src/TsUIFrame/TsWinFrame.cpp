#include "TsWinFrame.h"
#include "TsDrawBase.h"

//////////////////  ITsWinFrame ////////////////////////////
ITsWinFrame::ITsWinFrame(ITsWinFrame * pBaseFrame /* = NULL */)
:m_rtFrame(0, 0, 0, 0)
{
	m_pBaseFrame = pBaseFrame;
}
void ITsWinFrame::SetFrameRect(CRect& rtFrame)
{
	m_rtFrame = rtFrame;
	if(m_pBaseFrame != NULL)
	{
		m_pBaseFrame->SetFrameRect(GetContentRect());
	}
}

/////////////////////////// CTsColorFrame //////////////// 
CTsColorFrame::CTsColorFrame(COLORREF clrFrame, int nWidth /* = 1 */, int nHeight /* = 1*/, ITsWinFrame* pBase /* = NULL */)
:ITsWinFrame(pBase)
{
	m_clrFrame = clrFrame;
	m_nFrameHight = nHeight;
	m_nFrameWidth = nWidth;

	SetFrameRect(m_rtFrame);
}

CRect CTsColorFrame::GetContentRect()
{
	CRect rtRet;
	rtRet.SetRectEmpty();

	if(!m_rtFrame.IsRectEmpty())
	{
		rtRet = m_rtFrame;
		rtRet.InflateRect(-m_nFrameWidth, -m_nFrameHight, -m_nFrameWidth, -m_nFrameHight);
		rtRet.NormalizeRect();
		if(rtRet.left < 0 || rtRet.top < 0)
		{
			rtRet.SetRectEmpty();
		}
	}

	return rtRet;
}

void CTsColorFrame::DrawFrame(CDCHandle dc)
{
	CRect rtDraw(m_rtFrame);
	if(!m_rtFrame.IsRectEmpty())
	{
		if(m_rtFrame.Width() <= (m_nFrameWidth * 2) || m_rtFrame.Height() <= (m_nFrameHight * 2))
		{
			dc.FillSolidRect(&m_rtFrame, m_clrFrame);
		}
		else
		{
			rtDraw  = m_rtFrame;
			rtDraw.right = rtDraw.left + m_nFrameWidth;
			dc.FillSolidRect(&rtDraw, m_clrFrame);

			rtDraw  = m_rtFrame;
			rtDraw.bottom = rtDraw.top + m_nFrameHight;
			dc.FillSolidRect(&rtDraw, m_clrFrame);

			rtDraw  = m_rtFrame;
			rtDraw.left = rtDraw.right  - m_nFrameWidth;
			dc.FillSolidRect(&rtDraw, m_clrFrame);

			rtDraw  = m_rtFrame;
			rtDraw.top = rtDraw.bottom - m_nFrameHight;
			dc.FillSolidRect(&rtDraw, m_clrFrame);
		}
	}
	else
	{
		_ASSERTE(FALSE);
	}

	if(m_pBaseFrame != NULL)
	{
		m_pBaseFrame->DrawFrame(dc);
	}
}

CRect CTsColorFrame::GetIdealRect(CRect& rtContent)
{
	CRect rtRet(rtContent);
	if(m_pBaseFrame != NULL)
	{
		rtRet = m_pBaseFrame->GetIdealRect(rtContent);
	}
	rtRet.InflateRect(m_nFrameWidth, m_nFrameHight, m_nFrameWidth, m_nFrameHight);

	return rtRet;
}

/////////////////////////  CTsControlPaneFrame  /////////////////////
#define CONTROL_FRAME_TOP_HEIGHT 25
#define CONTROL_FRAME_WIDTH 1
#define CONTROL_FRAME_BOTTOM_HEIGHT 1
#define CONTROL_FRAME_COLOR_START RGB(144, 175, 225)
#define CONTROL_FRAME_COLOR_END RGB(255, 255, 255)  
#define CONTROL_FRAME_COLOR_FRAME RGB(0, 0, 255)

CTsControlPaneFrame::CTsControlPaneFrame(ITsWinFrame* pBase)
:ITsWinFrame(pBase)
{
	m_clrText = RGB(0, 0, 0);
	m_szTitle[0] = 0;
}

CRect CTsControlPaneFrame::GetContentRect()
{
	CRect rtRet;
	rtRet.SetRectEmpty();

	if(!m_rtFrame.IsRectEmpty())
	{
		rtRet = m_rtFrame;
		rtRet.InflateRect(-CONTROL_FRAME_WIDTH, -CONTROL_FRAME_TOP_HEIGHT, 
			-CONTROL_FRAME_WIDTH, -CONTROL_FRAME_BOTTOM_HEIGHT);

		rtRet.NormalizeRect();
		if(rtRet.left < 0 || rtRet.top < 0)
		{
			rtRet.SetRectEmpty();
		}
	}

	return rtRet;
}
void CTsControlPaneFrame::_DrawFrame(CDCHandle dc)
{
	CRect rtDraw(m_rtFrame);
// 	_ASSERTE(m_rtFrame.Width() >= CONTROL_FRAME_WIDTH * 2
// 		&& m_rtFrame.Height() >= CONTROL_FRAME_BOTTOM_HEIGHT + CONTROL_FRAME_TOP_HEIGHT);

	//draw frame

	rtDraw.right = rtDraw.left + CONTROL_FRAME_WIDTH;
	dc.FillSolidRect(&rtDraw, CONTROL_FRAME_COLOR_FRAME);

	if(m_rtFrame.Width() >= CONTROL_FRAME_WIDTH)
	{
		rtDraw = m_rtFrame;
		rtDraw.left = rtDraw.right - CONTROL_FRAME_WIDTH;
		dc.FillSolidRect(&rtDraw, CONTROL_FRAME_COLOR_FRAME);
	}

	if(m_rtFrame.Height() >= CONTROL_FRAME_BOTTOM_HEIGHT)
	{
		rtDraw = m_rtFrame;
		rtDraw.top = rtDraw.bottom - CONTROL_FRAME_BOTTOM_HEIGHT;
		dc.FillSolidRect(&rtDraw, CONTROL_FRAME_COLOR_FRAME);
	}

	//draw title bar
	rtDraw = m_rtFrame;
	rtDraw.InflateRect(-CONTROL_FRAME_WIDTH, - CONTROL_FRAME_WIDTH, 
		-CONTROL_FRAME_WIDTH, -CONTROL_FRAME_BOTTOM_HEIGHT);

	rtDraw.bottom = rtDraw.top + CONTROL_FRAME_TOP_HEIGHT;
	FillGradientColor(dc, rtDraw, CONTROL_FRAME_COLOR_START, CONTROL_FRAME_COLOR_END, TRUE);

}
CFontHandle CTsControlPaneFrame::_GetFont()
{
	if(m_fntText.IsNull())
	{
		return GetTsDefultFont(TRUE);
	}
	else
	{
		return m_fntText; 
	}
}
void CTsControlPaneFrame::_DrawTitleBar(CDCHandle dc)
{
	INT nStartX(5);
	INT nStartY(0);
	if(!m_bmpIcon.IsNull())
	{
		CSize szBmp = GetBitmapSize(m_bmpIcon);
		if(szBmp.cy < CONTROL_FRAME_TOP_HEIGHT)
		{
			nStartY = (CONTROL_FRAME_TOP_HEIGHT - szBmp.cy) / 2;
		}
		CDC memDC = CreateCompatibleDC(dc);
		HBITMAP hOldBmp = memDC.SelectBitmap(m_bmpIcon);
		dc.TransparentBlt(nStartX, nStartY, szBmp.cx, szBmp.cy, memDC, 0, 0, szBmp.cx, szBmp.cy, RGB(255, 0, 255));
		memDC.SelectBitmap(hOldBmp);

		nStartX += szBmp.cx;
		nStartX += 5;
	}

	INT nTextLen = lstrlen(m_szTitle);
	INT nRtLen = m_rtFrame.right - nStartX ;
	if(nTextLen > 0 && nRtLen > 0)
	{
		HFONT hOldFont = dc.SelectFont(_GetFont());
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(m_clrText);
		CRect rtText(nStartX, m_rtFrame.top, m_rtFrame.right,  m_rtFrame.top + CONTROL_FRAME_TOP_HEIGHT);
		dc.DrawText(m_szTitle, nTextLen, &rtText, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE);
		dc.SelectFont(hOldFont);
	}

}
void CTsControlPaneFrame::DrawFrame(CDCHandle dc)
{
	if(!m_rtFrame.IsRectEmpty())
	{
		_DrawFrame(dc);
		_DrawTitleBar(dc);
	}

	if(m_pBaseFrame != NULL)
	{
		m_pBaseFrame->DrawFrame(dc);
	}	
}

CRect CTsControlPaneFrame::GetIdealRect(CRect& rtContent)
{
	CRect rtRet(rtContent);
	if(m_pBaseFrame != NULL)
	{
		rtRet = m_pBaseFrame->GetIdealRect(rtContent);
	}
	rtRet.InflateRect(CONTROL_FRAME_WIDTH, CONTROL_FRAME_TOP_HEIGHT,
		CONTROL_FRAME_WIDTH, CONTROL_FRAME_BOTTOM_HEIGHT);

	return rtRet;
}

void CTsControlPaneFrame::SetTitleText(LPCTSTR lpszText)
{
	_ASSERTE(lpszText != NULL);
	lstrcpyn(m_szTitle, lpszText, sizeof(m_szTitle) / sizeof(TCHAR));
}

void CTsControlPaneFrame::SetIcon(HBITMAP bmp)
{
	_ASSERTE(bmp != NULL);
	m_bmpIcon = bmp;
}
void CTsControlPaneFrame::SetFont(HFONT hFont)
{
	_ASSERTE(hFont != NULL && GetObjectType(hFont) == OBJ_FONT);
	m_fntText = hFont;
}
void CTsControlPaneFrame::SetTitleTextColor(COLORREF clr)
{
	m_clrText = clr;
}