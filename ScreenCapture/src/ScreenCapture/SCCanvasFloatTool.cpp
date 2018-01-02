#include "SCCanvasFloatTool.h"
#include "SCCanvasSkin.h"
#include "SCIDDef.h"
#include "SCCanvasContext.h"
#include "SCCanvasWindow.h"
#include "SCRes.h"

#define PALETTE_COL_COUNT 8
#define PALETTE_BIG_WIDTH 30
#define PALETTE_SMALL_WIDTH 14
#define PALETTE_BIG_GAP 4
#define PALETTE_SMALL_GAP (PALETTE_BIG_WIDTH - PALETTE_SMALL_WIDTH * 2)
#define PALETTE_FRAME_COLOR RGB(0, 0, 0)

COLORREF CColorPickerControl::s_Palette[COLOR_PALETTE_COUNT] =  
{	
	RGB(0, 0, 0), RGB(128, 128, 128), RGB(128, 0, 0), RGB(128, 128, 0), 
	RGB(0, 128, 0), RGB(0, 0, 128), RGB(128, 0, 128), RGB(0, 128, 128), 

	RGB(255, 255, 255), RGB(192, 192, 192), RGB(255, 0, 0), RGB(255, 255, 0), 
	RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 0, 255), RGB(0, 255, 255), 
};

VOID GetTextLogFontData(LOGFONT& lfDefFont)
{
	ZeroMemory(&lfDefFont, sizeof(lfDefFont));
	
	int nCount;
	lfDefFont.lfHeight = -(GetFontSizeTable(nCount)[1]);
	lfDefFont.lfWidth = 0;
	lfDefFont.lfEscapement = 0;
	lfDefFont.lfOrientation = 0;
	lfDefFont.lfWeight = FW_NORMAL;

	LANGID gId=GetSystemDefaultLangID();
	
	if(gId==0x0411)
		lfDefFont.lfCharSet = SHIFTJIS_CHARSET;
	else if(gId==0x0404 || gId==0x0c04 || gId==0x1004)
		lfDefFont.lfCharSet = CHINESEBIG5_CHARSET;
	else if(gId==0x0804 || gId==0x0c04 || gId==0x1004)
		lfDefFont.lfCharSet = GB2312_CHARSET;
	else if(gId==0x0412 || gId==0x0812)
		lfDefFont.lfCharSet = ANSI_CHARSET;
	else
		lfDefFont.lfCharSet =  ANSI_CHARSET;
	lfDefFont.lfItalic = FALSE;
	lfDefFont.lfUnderline = FALSE;
	lfDefFont.lfStrikeOut = FALSE;
	lfDefFont.lfPitchAndFamily = DEFAULT_PITCH;
	lfDefFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfDefFont.lfClipPrecision  = CLIP_DEFAULT_PRECIS; 
	lfDefFont.lfQuality = DEFAULT_QUALITY; 
	
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	CString strFontName = pSkin->GetDefaultFontName();

	lstrcpyn( lfDefFont.lfFaceName, strFontName, sizeof(lfDefFont.lfFaceName)/sizeof(TCHAR) - 1);
}

CColorPickerControl::CColorPickerControl()
{
	m_clrCurrent =  RGB(255, 0, 0);
	SetBkgndMapType(HY_BKGND_SINGLE_COLOR);
}

VOID CColorPickerControl::OnSkinChanged()
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	SetBkMapSingleColor(pSkin->GetMainToolColor());
	InvalidateRect(NULL, TRUE);
}

VOID CColorPickerControl::SetCurrentColor(COLORREF clr)
{
	if(m_clrCurrent != clr)
	{
		m_clrCurrent = clr;
		if(IsWindow())
		{
			InvalidateRect(NULL, TRUE);
			OnColorChanged(clr);
		}
	}
}
CSize CColorPickerControl::GetIdealSize() const
{
	CSize szRet(0, 0);
	szRet.cx = PALETTE_BIG_WIDTH + PALETTE_BIG_GAP + PALETTE_COL_COUNT * PALETTE_SMALL_WIDTH
		+ (PALETTE_COL_COUNT - 1) * PALETTE_SMALL_GAP;
	szRet.cy = PALETTE_BIG_WIDTH;

	return szRet;
}

LRESULT CColorPickerControl::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	OnSkinChanged();
	AdjustLayout();
	return S_OK;
}

LRESULT CColorPickerControl::OnPaint(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	CDCHandle dc((HDC)wParam);

	DrawSelect(dc);
	DrawPalette(dc);

	return S_OK;
}

VOID CColorPickerControl::AdjustLayout()	
{
	CSize sizeControl = GetIdealSize();
	SetWindowPos(NULL, 0, 0, sizeControl.cx, sizeControl.cy, SWP_NOACTIVATE | SWP_NOMOVE);
}

LRESULT CColorPickerControl::OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	AdjustLayout();

	return S_OK;
}
LRESULT CColorPickerControl::OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
	CPoint ptHit(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	INT nHit = HitTest(ptHit) ;
	if(nHit >= 0 )
	{
		COLORREF clrNew = s_Palette[nHit];
		if(clrNew != m_clrCurrent)
		{
			m_clrCurrent = s_Palette[nHit];
			InvalidateRect(NULL, TRUE);
			OnColorChanged(m_clrCurrent);
		}
	}
	
	return S_OK;
}

VOID CColorPickerControl::GetSelectRect(CRect& rtRet)
{
	rtRet.SetRect(0, 0, PALETTE_BIG_WIDTH, PALETTE_BIG_WIDTH);	
}

VOID CColorPickerControl::DrawSelect(CDCHandle dc)
{
	CRect rtDraw;
	GetSelectRect(rtDraw);
	
	CBrushHandle brushOld = dc.SelectBrush((HBRUSH)GetStockObject(WHITE_BRUSH));
	CPen penNew = CreatePen(PS_SOLID, 1, PALETTE_FRAME_COLOR);
	CPenHandle penOld = dc.SelectPen(penNew);
	dc.Rectangle(&rtDraw);
	rtDraw.InflateRect(-2, -2, -2, -2);
	dc.FillSolidRect(rtDraw, m_clrCurrent);
	dc.SelectPen(penOld);
	dc.SelectBrush(brushOld);
}

VOID CColorPickerControl::DrawPalette(CDCHandle dc)
{
	CRect rtDraw;
	for(int i=0; i<COLOR_PALETTE_COUNT; ++i)
	{
		GetPaletteRectByIndex(i, rtDraw);
		
		CBrush newBrush = CreateSolidBrush(s_Palette[i]);
		CPen penNew = CreatePen(PS_SOLID, 1, PALETTE_FRAME_COLOR);
		CPenHandle penOld = dc.SelectPen(penNew);
		CBrushHandle brushOld = dc.SelectBrush(newBrush);
		dc.Rectangle(&rtDraw);
		dc.SelectPen(penOld);
		dc.SelectBrush(brushOld);		
	}
}
INT CColorPickerControl::HitTest(CPoint& pt)
{
	CRect rtDraw;
	for(int i=0; i<COLOR_PALETTE_COUNT; ++i)
	{
		GetPaletteRectByIndex(i, rtDraw);
			
		if(PtInRect(&rtDraw, pt))
		{
			return i;
		}
	}

	return -1;
}
VOID CColorPickerControl::GetPaletteRectByIndex(INT nIndex, CRect& rtRet)
{
	int nLineIndex = nIndex / PALETTE_COL_COUNT;
	int nColIndex = nIndex % PALETTE_COL_COUNT;

	rtRet.left = PALETTE_BIG_WIDTH + PALETTE_BIG_GAP + (PALETTE_SMALL_GAP + PALETTE_SMALL_WIDTH) * nColIndex;
	rtRet.top = nLineIndex * (PALETTE_SMALL_WIDTH + PALETTE_SMALL_GAP);
	
	rtRet.right = rtRet.left + PALETTE_SMALL_WIDTH;
	rtRet.bottom = rtRet.top + PALETTE_SMALL_WIDTH;
}

VOID CColorPickerControl::OnColorChanged(COLORREF clr)
{
	HWND hWndParent = GetParent();
	::SendMessage(hWndParent, WM_COMMAND, MAKEWPARAM(SC_TOOL_COLOR_PICKER_ID, 0), 0);
}



#define FONT_PICK_CONTROL_WIDTH			60
#define FONT_PICK_CONTROL_HEIGHT		22
#define FONT_PICK_ARROW_WIDTH		FONT_PICK_CONTROL_HEIGHT
#define FONT_PICK_TEXT_WIDHT		(FONT_PICK_CONTROL_WIDTH - FONT_PICK_ARROW_WIDTH)

CFontSizePickerControl::CFontSizePickerControl()
{
	INT nCount(0);
	m_nCurrentValue = 20;
	m_clrFrameColor = RGB(84, 165, 213);
	m_clrSelect = RGB(124, 197, 234);
	m_clrArrow = RGB(25, 85, 95);

	m_clrText = RGB(0, 0, 0);
	m_bMenuTracking = FALSE;
}
VOID CFontSizePickerControl::OnSkinChanged()
{
	IHYMenuDrawer* pMenuDrawer = GetCanvasSkin()->GetCurrentMenuDrawer();
	_ASSERTE(pMenuDrawer != NULL);
	
	m_clrFrameColor = pMenuDrawer->GetMenuFrameColor();
	m_clrSelect = pMenuDrawer->GetMenuSelectBKColor();

	AdjustLayout();
	InvalidateRect(NULL, TRUE);
}
CSize CFontSizePickerControl::GetIdealSize() const
{
	return CSize(FONT_PICK_CONTROL_WIDTH, FONT_PICK_CONTROL_HEIGHT);
}

VOID CFontSizePickerControl::SetCurrentValue(INT nValue)
{
	if(m_nCurrentValue != nValue)
	{
		m_nCurrentValue = nValue;

		if(IsWindow())
		{
			InvalidateRect(NULL, TRUE);
		}
	}
}

CFontHandle CFontSizePickerControl::GetTextFont()
{
	return CHYUICommonFunctionWrapper::GetDefaultFont(FALSE);
}

VOID CFontSizePickerControl::GetArrowRect(CRect& rtArrow)
{
	CRect rtFrame;
	GetFrameRect(rtFrame);
	rtFrame.left = rtFrame.right - FONT_PICK_ARROW_WIDTH;
	if(rtFrame.left < 0) rtFrame.left = 0;

	rtArrow = rtFrame;
}
VOID CFontSizePickerControl::GetFrameRect(CRect& rtFrame)
{
	GetClientRect(&rtFrame);
}

VOID CFontSizePickerControl::GetTextRect(CRect& rtText)
{
	CRect rtFrame;
	GetFrameRect(rtFrame);

	CRect rtArrow;
	GetArrowRect(rtArrow);

	rtText = CRect(rtFrame.left, rtFrame.top, rtFrame.Width() - rtArrow.Width(), rtFrame.bottom);
}

VOID CFontSizePickerControl::AdjustLayout()	
{
	CSize sizeControl = GetIdealSize();
	SetWindowPos(NULL, 0, 0, sizeControl.cx, sizeControl.cy, SWP_NOACTIVATE | SWP_NOMOVE);
}

HMENU CFontSizePickerControl::CreateTrackMenu()
{
	INT nCount(0);
	int* pFontSizeTable = GetFontSizeTable(nCount);

	TCHAR szMenu[32] = {0};
	HMENU hMenu = CreatePopupMenu();
	for(int i=0; i < nCount; ++i)
	{
		_ltot(pFontSizeTable[i], szMenu, 10);
		AppendMenu(hMenu, MF_STRING, pFontSizeTable[i], szMenu);
	}

	return hMenu;
}

VOID CFontSizePickerControl::GetTrackPos( CPoint& pt, BOOL& bBottomAlign)
{
	CRect rtWin;
	GetWindowRect(&rtWin);
	
	bBottomAlign = TRUE;

	int nY = rtWin.bottom;
	if(bBottomAlign)
	{
		nY	= rtWin.top;
	}
	pt.y = nY;
	pt.x = rtWin.left;
}

VOID CFontSizePickerControl::DrawFrame(CDCHandle dc)
{
	CRect rtFrame;
	GetFrameRect(rtFrame);
	CRect rtArrow;
	GetArrowRect(rtArrow);

	CPen penNew = CreatePen(PS_SOLID, 1, m_clrFrameColor);
	CPenHandle penOld = dc.SelectPen(penNew);
	CBrushHandle brushOld = dc.SelectBrush((HBRUSH)GetStockObject(WHITE_BRUSH));
	dc.Rectangle(&rtFrame);

	if(IsInHotStatus())
		dc.Rectangle(&rtArrow);

	dc.SelectBrush(brushOld);
	dc.SelectPen(penOld);
}

VOID CFontSizePickerControl::DrawArrow(CDCHandle dc)
{
	CRect rtArrow;
	GetArrowRect(rtArrow);

	if(IsInHotStatus())
	{
		rtArrow.InflateRect(-2, -2, -2, -2);
		dc.FillSolidRect(&rtArrow, m_clrSelect);
	}

	INT nMiddleX = (rtArrow.left + rtArrow.right ) / 2;
	INT nMiddleY = (rtArrow.top + rtArrow.bottom) / 2;

	const int nArrowSize = 3;
	POINT ptArray[] = 
	{
		nMiddleX - nArrowSize, nMiddleY - nArrowSize,
		nMiddleX + nArrowSize, nMiddleY - nArrowSize,
		nMiddleX , nMiddleY + nArrowSize
	};

	CPen newPen = CreatePen(PS_SOLID, 1, m_clrArrow);
	CBrush newBrush = CreateSolidBrush(m_clrArrow);

	CPenHandle penOld = dc.SelectPen(newPen);
	CBrushHandle brushOld = dc.SelectBrush(newBrush);
	dc.Polygon(ptArray, sizeof(ptArray) / sizeof(ptArray[0]));
	dc.SelectBrush(brushOld);
	dc.SelectPen(penOld);
}

VOID CFontSizePickerControl::DrawText(CDCHandle dc)
{
	CRect rtText;
	GetTextRect(rtText);
	TCHAR szText[32] = {0};
	_ltot(m_nCurrentValue, szText, 10);

	CFontHandle fntOld = dc.SelectFont(GetTextFont());
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_clrText);
	dc.DrawText(szText, lstrlen(szText), &rtText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	dc.SelectFont(fntOld);
}
LRESULT CFontSizePickerControl::OnSize(UINT, WPARAM, LPARAM, BOOL&)
{
	AdjustLayout();
	return S_OK;
}
LRESULT CFontSizePickerControl::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	OnSkinChanged();
	AdjustLayout();
	return S_OK;
}
LRESULT CFontSizePickerControl::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	CPaintDC dc(m_hWnd);
	CHYMemDC dcMem(dc);
	
	DrawFrame(dcMem.m_hDC);
	DrawText(dcMem.m_hDC);
	DrawArrow(dcMem.m_hDC);

	return S_OK;
}

LRESULT CFontSizePickerControl::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&)
{
	return TRUE;
}

LRESULT CFontSizePickerControl::OnLButtonDown(UINT, WPARAM, LPARAM, BOOL&)
{
	m_bMenuTracking = TRUE;
	
	CPoint ptTrack;
	BOOL bBottomAlign;
	GetTrackPos(ptTrack, bBottomAlign);

	UINT nFlag(TPM_TOPALIGN);
	if(bBottomAlign)
	{
		nFlag = TPM_BOTTOMALIGN;
	}

	CMenu menu = CreateTrackMenu();
	InitMenuCache(menu);
	int nCmd = TrackPopupMenuEx(menu, TPM_LEFTALIGN | nFlag | TPM_NONOTIFY | TPM_RETURNCMD, ptTrack.x, ptTrack.y, m_hWnd, NULL);
	ClearMenuCache();
	if(nCmd != 0 && nCmd != m_nCurrentValue)
	{
		SetCurrentValue(nCmd);

		HWND hWndParent = GetParent();
		::SendMessage(hWndParent, WM_COMMAND, MAKEWPARAM(SC_TOOL_FONT_SIZE_ID, 0), 0);
	}
	
	m_bMenuTracking = FALSE;
	
	InvalidateRect(NULL, TRUE);

	return S_OK;
}




CPenPickerTool::CPenPickerTool()
{
	m_penTool.SetBkgndMapType(HY_BKGND_SINGLE_COLOR);
	m_clrMainTool = GetSysColor(COLOR_WINDOW);
//	m_clrFrame = RGB(0, 0, 0);
	m_hWndNotify = NULL;
}

CPenPickerTool::~CPenPickerTool()
{
	ReleaseResource();
}

VOID CPenPickerTool::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == UI_WPARAM_SKIN_CHANGED
		|| wParam == UI_WPARAM_LAN_CHANGED)
	{
		m_colorPicker.OnSkinChanged();
		m_fntSizePicker.OnSkinChanged();

		LoadResource();
		ApplyResource();
		AdjustLayout();
	}

}
VOID CPenPickerTool::SetPaletteColor(COLORREF clr)
{
	m_colorPicker.SetCurrentColor(clr);
}
VOID CPenPickerTool::SetFontSize(INT nSize)
{
#ifdef _DEBUG
	INT nCount(0);
	int* pFontSizeTable = GetFontSizeTable(nCount);

	BOOL bFound(FALSE);
	for(int i=0; i < nCount; ++i)
	{
		if(pFontSizeTable[i] == nSize)
		{
			bFound = TRUE;
			break;
		}
	}

	if(!bFound)
	{
		_ASSERTE(FALSE);
	}
#endif

	m_fntSizePicker.SetCurrentValue(nSize);
}
VOID CPenPickerTool::ShowFontTools(BOOL bShow, BOOL bUpdate)
{
	m_penTool.ShowItemByID(SC_TOOL_FONT_BOLD_ID, bShow, FALSE);
	m_penTool.ShowItemByID(SC_TOOL_FONT_SIZE_ID, bShow, bUpdate);
}

VOID CPenPickerTool::ShowPenWidthTools(BOOL bShow, BOOL Update)
{
	m_penTool.ShowItemByID(SC_TOOL_SMALL_PEN_ID, bShow, FALSE);
	m_penTool.ShowItemByID(SC_TOOL_MIDDLE_PEN_ID, bShow, FALSE);
	m_penTool.ShowItemByID(SC_TOOL_BIG_PEN_ID, bShow, Update);
}
BOOL CPenPickerTool::GetFontBold()
{
	CTsToolItem* pItem = m_penTool.GetItemByID(SC_TOOL_FONT_BOLD_ID);
	_ASSERTE(pItem != NULL);
	if(pItem != NULL )
	{
		return pItem->GetCheck();
	}

	return FALSE;
}

VOID CPenPickerTool::SetFontBold(BOOL bBold)
{
	CTsToolItem* pItem = m_penTool.GetItemByID(SC_TOOL_FONT_BOLD_ID);
	_ASSERTE(pItem != NULL);
	if(pItem != NULL )
	{
		pItem->SetCheck(bBold);
	}
}
DWORD CPenPickerTool::GetPenType()
{
	DWORD dwRet(NONE_PEN);
	
	INT nPenCount(0);
	PEN_TYPE_ID_PAIR* pPenTable = GetPenTypeIDTable(nPenCount);

	int nCount  = m_penTool.GetItemCount();
	for(int i=0; i<nCount; ++i)
	{
		CTsToolItem* pItem = m_penTool.GetItemByIndex(i);
		if(pItem->IsButton() && pItem->GetCheck())
		{
			for(int k=0; k<nPenCount; ++k)
			{
				PEN_TYPE_ID_PAIR* pData = pPenTable + k;
				if(pData->dwBtnID == pItem->GetID())
				{
					return pData->dwType;
				}
			}
		}
	}

	return dwRet;
}
VOID CPenPickerTool::SetPenType(DWORD dwType)
{
	m_penTool.ResetAllItem();

	INT nPenCount(0);
	PEN_TYPE_ID_PAIR* pPenTable = GetPenTypeIDTable(nPenCount);
	
	DWORD dwBtn(0);
	
	for(int i=0; i<nPenCount; ++i)
	{
		if(pPenTable[i].dwType == dwType)
		{
			dwBtn = pPenTable[i].dwBtnID;
		}
	}
	
	
	if(dwBtn != 0)
	{
		m_penTool.SetBtnChecked(dwBtn, TRUE);
	}
	else
	{
		_ASSERTE(FALSE);
	}
}

LRESULT CPenPickerTool::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	LoadResource();
	CreateTools();
	ApplyResource();

	AdjustLayout();

	return S_OK;
}

LRESULT CPenPickerTool::OnPaint(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	CDCHandle dc((HDC)wParam);
//	DrawFrame(dc);

	return S_OK;
}
// VOID CPenPickerTool::DrawFrame(CDCHandle dc)
// {
// 	CRect rtClient;
// 	GetClientRect(&rtClient);
// 	CHYUICommonFunctionWrapper::DrawFrame(dc, rtClient, m_clrFrame, 1);
// }

LRESULT CPenPickerTool::OnSize(UINT, WPARAM, LPARAM, BOOL&)
{
	AdjustLayout();

	return S_OK;
}

LRESULT CPenPickerTool::OnCommand(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL&)
{
	WORD nCmdID = LOWORD(wParam);

	if(nCmdID == SC_TOOL_SMALL_PEN_ID
		|| nCmdID == SC_TOOL_MIDDLE_PEN_ID
		|| nCmdID == SC_TOOL_BIG_PEN_ID)
	{
		m_penTool.ResetItemByIDRange(SC_TOOL_SMALL_PEN_ID, SC_TOOL_BIG_PEN_ID);
		m_penTool.SetBtnChecked(nCmdID, TRUE);
	}
	else if(nCmdID == SC_TOOL_FONT_BOLD_ID)
	{

		CTsToolItem* pItem = m_penTool.GetItemByID(nCmdID);
		_ASSERTE(pItem != NULL);

		BOOL bCheck = pItem->GetCheck();
		m_penTool.SetBtnChecked(nCmdID, !bCheck);
	}
	

	if(m_hWndNotify != NULL && ::IsWindow(m_hWndNotify))
	{
		return 	::SendMessage(m_hWndNotify, nMsgType, wParam, lParam);
	}
	
	return S_OK;
}


VOID CPenPickerTool::CreateTools()
{
	if(!m_penTool.IsWindow())	
	{
		m_penTool.AddButton(SC_TOOL_FONT_BOLD_ID, m_bmpBtnBK.m_hBitmap, m_imgFontBold);
		m_penTool.AddSeperator(m_bmpSeperator.m_hBitmap);
		
		m_penTool.AddButton(SC_TOOL_SMALL_PEN_ID, m_bmpBtnBK.m_hBitmap, m_imgSmallPen);
		m_penTool.AddButton(SC_TOOL_MIDDLE_PEN_ID, m_bmpBtnBK.m_hBitmap, m_imgMiddlePen);
		m_penTool.AddButton(SC_TOOL_BIG_PEN_ID, m_bmpBtnBK.m_hBitmap, m_imgBigPen);


		if(!m_fntSizePicker.IsWindow())
		{
			m_fntSizePicker.Create(m_hWnd, CRect(0, 0, 0, 0));
		}
		m_penTool.AddControl(SC_TOOL_FONT_SIZE_ID, m_fntSizePicker.m_hWnd);

		m_penTool.AddSeperator(m_bmpSeperator.m_hBitmap);
		m_penTool.Create(m_hWnd, CRect(0, 0, 0, 0));
	}

	if(!m_colorPicker.IsWindow())
	{
		m_colorPicker.Create(m_penTool, CRect(0, 0, 0, 0));
	}
	m_penTool.AddControl(SC_TOOL_COLOR_PICKER_ID, m_colorPicker.m_hWnd);
}

VOID CPenPickerTool::AdjustLayout()
{
	if(!m_penTool.IsWindow() 
		|| !m_colorPicker.IsWindow()
		|| !m_fntSizePicker.IsWindow())
	{
		return;
	}

	CRect rtWin;
	GetWindowRect(&rtWin);
	CSize sizeTool = m_penTool.GetIdealSize();
// 	int nWidth = sizeTool.cx + 8;
// 	int nHeight = sizeTool.cy + 8;

	int nX = (rtWin.Width() - sizeTool.cx ) / 2;
	int nY = (rtWin.Height() - sizeTool.cy) / 2;
	
	::SetWindowPos(m_penTool, NULL, nX, nY, sizeTool.cx, sizeTool.cy, SWP_NOACTIVATE | SWP_NOZORDER);
	//::SetWindowPos(m_hWnd, NULL, 0, 0, nWidth, nHeight, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

VOID CPenPickerTool::LoadResource()
{
	ReleaseResource();	
	
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();

	m_bmpSeperator = pSkin->GetToolSeperatorBK();
	m_clrMainTool = pSkin->GetMainToolColor();
//	m_clrFrame =  pSkin->GetFloatToolFrameColor();
	
	m_bmpBtnBK = pSkin->GetPenWidthBtnBK();
	m_imgSmallPen = pSkin->GetSmallPenBtnImg();
	m_imgMiddlePen = pSkin->GetMiddlePenBtnImg();
	m_imgBigPen = pSkin->GetBigPenBtnImg();
	m_imgFontBold = pSkin->GetFontBoldBtnImg();

	m_bmpBK = pSkin->GetSubFloatToolBK();
}	

VOID CPenPickerTool::ReleaseResource()
{
	if(!m_bmpBK.IsNull())
	{
		m_bmpBK.DeleteObject();
	}
	if(!m_bmpSeperator.IsNull())
	{
		m_bmpSeperator.DeleteObject();
	}
	if(!m_bmpBtnBK.IsNull())
	{
		m_bmpBtnBK.DeleteObject();
	}
	if(!m_imgSmallPen.IsNull())
	{
		m_imgSmallPen.Destroy();
	}
	if(!m_imgMiddlePen.IsNull())
	{
		m_imgMiddlePen.Destroy();
	}
	if(!m_imgBigPen.IsNull())
	{
		m_imgBigPen.Destroy();
	}
	if(!m_imgFontBold.IsNull())
	{
		m_imgFontBold.Destroy();
	}
}

VOID CPenPickerTool::ApplyResource()
{
	m_penTool.SetBkMapSingleColor(m_clrMainTool);

	int nCount = m_penTool.GetItemCount();
	for(int i=0; i<nCount; ++i)
	{
		CTsToolItem* pItem = m_penTool.GetItemByIndex(i);
		_ASSERTE(pItem != NULL);
		if(pItem->IsSeperator())
		{
			pItem->SetBkgndBitmap(m_bmpSeperator);
		}
		else if(pItem->IsButton())
		{
			CBitmapHandle bmpBk = m_bmpBtnBK;
			CImageList imgBtn = m_imgSmallPen;
			switch(pItem->GetID())
			{
			case SC_TOOL_SMALL_PEN_ID:
				imgBtn = m_imgSmallPen;
				break;
			case SC_TOOL_MIDDLE_PEN_ID:
				imgBtn = m_imgMiddlePen;
				break;
			case SC_TOOL_BIG_PEN_ID:
				imgBtn = m_imgBigPen;
				break;
			case SC_TOOL_FONT_BOLD_ID:
				imgBtn = m_imgFontBold;
				break;

			default:
				_ASSERTE(FALSE);
				break;
			}
			pItem->SetBkgndBitmap(bmpBk);
			pItem->SetBtnImagelist(imgBtn);
		}
		else if(pItem->IsControl())
		{
			NULL;
		}
	}
	SetBkMapBmp((HBITMAP)m_bmpBK);
	PostMessage(WM_SIZE, 0, 0);
}

LRESULT CSCDragButton::OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
	CWindow winParent = GetParent();
	while(winParent.GetStyle() & WS_CHILD)
	{
		winParent = winParent.GetParent();
	}
	
	CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	::ClientToScreen(m_hWnd, &pt);
	winParent.PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pt.x, pt.y));

	return S_OK;
}

LRESULT CSCDragButton::OnSetCursor(UINT, WPARAM, LPARAM, BOOL& )
{
	SetCursor(LoadCursor(NULL, IDC_SIZEALL));

	return TRUE;
}


CSCCanvasFloatTool::CSCCanvasFloatTool()
{
	SetBkgndMapType(HY_BKGND_MIDDLE_STRETCH_BMP);
	m_mainToolBar.SetBkgndMapType(HY_BKGND_SINGLE_COLOR);
	m_clrMainTool = GetSysColor(COLOR_WINDOW);
//	m_clrFrame = RGB(0, 0, 0);
}
CSCCanvasFloatTool::~CSCCanvasFloatTool()
{
	ReleaseResource();
}
VOID CSCCanvasFloatTool::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == UI_WPARAM_SKIN_CHANGED 
		|| wParam == UI_WPARAM_LAN_CHANGED)
	{
		LoadResource();
		ApplyResource();
		AdjustLayout();
		CheckCurrentTool();
		
		if(wParam == UI_WPARAM_LAN_CHANGED)
		{
			ResetFontToolData(); //default font name changed
		}
	}
	else if(wParam == UI_WPARAM_TOOL_CHANGED)
	{
		CheckCurrentTool();
	}

}

LRESULT CSCCanvasFloatTool::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	CreateFontToolData();

	LoadResource();
	CreateMainTools();
	ApplyResource();

	AdjustLayout();
	
	CreateSubTools();

	return S_OK;
}
VOID CSCCanvasFloatTool::CreateSubTools()
{
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();

	if(!m_penPicker.IsWindow())
	{
		m_penPicker.SetNotifyWnd(m_hWnd);
		m_penPicker.Create(pCanvas->m_hWnd, CRect(0, 0, 0, 0));
	}
}
VOID CSCCanvasFloatTool::DestroySubTools()
{
	if(m_penPicker.IsWindow())
	{
		m_penPicker.SetNotifyWnd(NULL);
		m_penPicker.DestroyWindow();
	}
}

LRESULT CSCCanvasFloatTool::OnSize(UINT, WPARAM, LPARAM, BOOL&)
{
	AdjustLayout();
	
	return S_OK;
}

LRESULT CSCCanvasFloatTool::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	DestroySubTools();

	DeleteFontToolData();

	return S_OK;
}

VOID CSCCanvasFloatTool::CreateMainTools()
{
	//create toolbar
	if(!m_mainToolBar.IsWindow())
	{
		m_mainToolBar.AddButton(SC_TOOL_SELECT_ID, m_bmpBtnBK.m_hBitmap, m_imgBtnSelect, m_strTipBtnSelect);
		m_mainToolBar.AddButton(SC_TOOL_WIN_SELECT_ID, m_bmpBtnBK.m_hBitmap, m_imgBtnWinSelect, m_strTipBtnWinSelect);
		m_mainToolBar.AddSeperator(m_bmpSeperator.m_hBitmap);

		m_mainToolBar.AddButton(SC_TOOL_RECT_ID, m_bmpBtnBK.m_hBitmap, m_imgBtnRect, m_strTipBtnRect);
		m_mainToolBar.AddButton(SC_TOOL_ELLIPSE_ID, m_bmpBtnBK.m_hBitmap, m_imgBtnEllipse, m_strTipBtnEllipse);
		m_mainToolBar.AddSeperator(m_bmpSeperator.m_hBitmap);

		m_mainToolBar.AddButton(SC_TOOL_LINE_ID, m_bmpBtnBK.m_hBitmap, m_imgBtnLine, m_strTipBtnLine);
		m_mainToolBar.AddButton(SC_TOOL_ARROW_ID, m_bmpBtnBK.m_hBitmap, m_imgBtnArrow, m_strTipBtnArrow);
		m_mainToolBar.AddSeperator(m_bmpSeperator.m_hBitmap);

		m_mainToolBar.AddButton(SC_TOOL_PEN_ID, m_bmpBtnBK.m_hBitmap, m_imgBtnPen, m_strTipBtnPen);
		m_mainToolBar.AddButton(SC_TOOL_TEXT_ID, m_bmpBtnBK.m_hBitmap, m_imgBtnText, m_strTipBtnText);
		m_mainToolBar.AddSeperator(m_bmpSeperator.m_hBitmap);

		m_mainToolBar.AddButton(SC_TOOL_ERASER_ID, m_bmpBtnBK.m_hBitmap, m_imgBtnEraser, m_strTipBtnEraser);

		m_mainToolBar.Create(m_hWnd, CRect(0, 0, 0, 0));
	}


	//create drag button
	if(!m_btnDrag.IsWindow())
	{
		m_btnDrag.SetBtnStyle(TS_BTN_RGN_STYLE);
		m_btnDrag.SetBtnImagelist(m_imgBtnDrag);
		m_btnDrag.SetBkgndBitmap(m_bmpBtnDrag);
		m_btnDrag.Create(m_hWnd, CRect(0, 0, 0, 0));
	}

	//create menu button
	if(!m_btnMenu.IsWindow())
	{
		m_btnMenu.SetBtnStyle(TS_BTN_RGN_STYLE);
		m_btnMenu.SetBtnImagelist(m_imgBtnMenu);
		m_btnMenu.SetBkgndBitmap(m_bmpBtnMenu);
		m_btnMenu.SetTooltipText(m_strTipBtnMenu);
		m_btnMenu.Create(m_hWnd, CRect(0, 0, 0, 0), NULL, NULL, NULL, SC_TOOL_EXTEND_MENU_ID);
	}
}

VOID CSCCanvasFloatTool::ApplyResource()
{
	SetBkMapBmp((HBITMAP)m_bmpWinBK);
	m_mainToolBar.SetBkMapSingleColor(m_clrMainTool);
	
	m_btnDrag.SetBtnImagelist(m_imgBtnDrag);
	m_btnDrag.SetBkgndBitmap(m_bmpBtnDrag);

	m_btnMenu.SetBtnImagelist(m_imgBtnMenu);
	m_btnMenu.SetBkgndBitmap(m_bmpBtnMenu);
	m_btnMenu.SetTooltipText(m_strTipBtnMenu);

	int nCount = m_mainToolBar.GetItemCount();
	for(int i=0; i<nCount; ++i)
	{
		CTsToolItem* pItem = m_mainToolBar.GetItemByIndex(i);
		_ASSERTE(pItem != NULL);
		if(pItem->IsSeperator())
		{
			pItem->SetBkgndBitmap(m_bmpSeperator.m_hBitmap);
		}
		else if(pItem->IsButton())
		{
			CBitmapHandle hBmpBk = m_bmpBtnBK.m_hBitmap;
			CImageList imgBtn = NULL;
			LPCTSTR lpszTip = NULL;
			switch(pItem->GetID())
			{
			case SC_TOOL_SELECT_ID:
				imgBtn = m_imgBtnSelect;
				lpszTip = m_strTipBtnSelect;
				break;

			case SC_TOOL_WIN_SELECT_ID:
				imgBtn = m_imgBtnWinSelect;
				lpszTip = m_strTipBtnWinSelect;
				break;

			case SC_TOOL_RECT_ID:
				imgBtn = m_imgBtnRect;
				lpszTip = m_strTipBtnRect;
				break;


			case SC_TOOL_ELLIPSE_ID:
				imgBtn = m_imgBtnEllipse;
				lpszTip = m_strTipBtnEllipse;
				break;

			case SC_TOOL_LINE_ID:
				imgBtn = m_imgBtnLine;
				lpszTip = m_strTipBtnLine;
				break;

			case SC_TOOL_ARROW_ID:
				imgBtn = m_imgBtnArrow;
				lpszTip = m_strTipBtnArrow;
				break;

			case SC_TOOL_PEN_ID:
				imgBtn = m_imgBtnPen;
				lpszTip = m_strTipBtnPen;
				break;	

			case SC_TOOL_TEXT_ID:
				imgBtn = m_imgBtnText;
				lpszTip = m_strTipBtnText;
				break;	

			case SC_TOOL_ERASER_ID:
				imgBtn = m_imgBtnEraser;
				lpszTip = m_strTipBtnEraser;
				break;	
				
			default:
				_ASSERTE(FALSE);
				break;
			}

			pItem->SetBkgndBitmap(hBmpBk);
			pItem->SetBtnImagelist(imgBtn);
			pItem->SetTooltipText(lpszTip);
		}
		else if(pItem->IsControl())
		{
			NULL;
		}
	}
}
VOID CSCCanvasFloatTool::AdjustLayout()
{
	if(!m_mainToolBar.IsWindow()
		|| !m_btnDrag.IsWindow()
		|| !m_btnMenu.IsWindow())
	{
		return;
	}

	CSize sizeToolBar = m_mainToolBar.GetIdealSize();
	
	CRect rtDrag;
	CSize sizeDrag = m_btnDrag.GetIdealSize();
	rtDrag.SetRect(0, 0,  sizeDrag.cx, sizeDrag.cy);
	CRect rtMenu;
	CSize sizeMenuBtn = m_btnMenu.GetIdealSize();
	rtMenu.SetRect(0, 0, sizeMenuBtn.cx, sizeMenuBtn.cy);
	
	int nWidth = rtDrag.Width() + rtMenu.Width() + sizeToolBar.cx;
	int nHeight = max(max(rtDrag.Height(), rtMenu.Height()), sizeToolBar.cy);


	int nScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	CRect rtWin;
	GetWindowRect(&rtWin);
	INT nPosX = rtWin.left;
	INT nPosY = rtWin.top;
	if(rtWin.left + nWidth > nScreenWidth)
	{
		nPosX = nScreenWidth - nWidth - 5;
	}

	SetWindowPos(NULL, nPosX, nPosY, nWidth, nHeight, SWP_NOACTIVATE /*| SWP_NOMOVE*/);

	m_btnDrag.SetWindowPos(NULL, 0, 0, 0, 0,  SWP_NOACTIVATE);

	int nY = ( nHeight - sizeToolBar.cy ) / 2;
	if(nY < 0) nY	= 0;
	m_mainToolBar.SetWindowPos(NULL, rtDrag.right, nY, sizeToolBar.cx, sizeToolBar.cy, SWP_NOACTIVATE);
	
	m_btnMenu.SetWindowPos(NULL, rtDrag.right + sizeToolBar.cx, 0, sizeMenuBtn.cx, sizeMenuBtn.cy, SWP_NOACTIVATE);
	
	CRgn rgnDrag = CreateRectRgn(0, 0, 0, 0), rgnMenu = CreateRectRgn(0, 0, 0, 0);
	m_btnDrag.GetWindowRgn(rgnDrag);
	m_btnMenu.GetWindowRgn(rgnMenu);
	CRgn rgnToolBar = CreateRectRgn(rtDrag.right, 0, rtDrag.right + sizeToolBar.cx, nHeight);
	rgnMenu.OffsetRgn(rtDrag.right + sizeToolBar.cx, 0);

	CRgn rgnTemp = CreateRectRgn(0, 0, 0, 0);
	rgnTemp.CombineRgn(rgnDrag, rgnToolBar, RGN_OR);
	
	CRgnHandle rgnWindow = CreateRectRgn(0, 0, 0, 0);
	rgnWindow.CombineRgn(rgnTemp, rgnMenu, RGN_OR);
	SetWindowRgn(rgnWindow, TRUE);
}

VOID CSCCanvasFloatTool::ReleaseResource()
{
	if(!m_bmpWinBK.IsNull())
	{
		m_bmpWinBK.DeleteObject();
	}
	if(!m_bmpBtnBK.IsNull())
	{
		m_bmpBtnBK.DeleteObject();
	}

	if(!m_bmpSeperator.IsNull())
	{
		m_bmpSeperator.DeleteObject();
	}
	
	if(!m_imgBtnSelect.IsNull())
	{
		m_imgBtnSelect.Destroy();
	}

	if(!m_bmpBtnDrag.IsNull())
	{
		m_bmpBtnDrag.DeleteObject();
	}

	if(!m_bmpBtnMenu.IsNull())
	{
		m_bmpBtnMenu.DeleteObject();
	}

	if(!m_imgBtnDrag.IsNull())
	{
		m_imgBtnDrag.Destroy();
	}

	if(!m_imgBtnMenu.IsNull())
	{
		m_imgBtnMenu.Destroy();
	}

	if(!m_imgBtnEllipse.IsNull())
	{
		m_imgBtnEllipse.Destroy();
	}

	if(!m_imgBtnRect.IsNull())
	{
		m_imgBtnRect.Destroy();
	}

	if(!m_imgBtnLine.IsNull())
	{
		m_imgBtnLine.Destroy();
	}
	if(!m_imgBtnPen.IsNull())
	{
		m_imgBtnPen.Destroy();
	}
	if(!m_imgBtnText.IsNull())
	{
		m_imgBtnText.Destroy();
	}
	if(!m_imgBtnEraser.IsNull())
	{
		m_imgBtnEraser.Destroy();
	}
	if(!m_imgBtnArrow.IsNull())
	{
		m_imgBtnArrow.Destroy();
	}
	if(!m_imgBtnWinSelect.IsNull())
	{
		m_imgBtnWinSelect.Destroy();
	}
}
VOID CSCCanvasFloatTool::LoadResource()
{
	ReleaseResource();

	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();

	m_bmpWinBK = pSkin->GetFloatToolBK();
	m_bmpBtnBK = pSkin->GetToolBtnBK();
	m_bmpSeperator = pSkin->GetToolSeperatorBK();
	m_clrMainTool = pSkin->GetMainToolColor();
//	m_clrFrame =  pSkin->GetFloatToolFrameColor();

	m_imgBtnSelect = pSkin->GetToolSelectBtnImg();
	m_imgBtnRect = pSkin->GetToolRectBtnImg();
	m_imgBtnEllipse = pSkin->GetToolEllipseBtnImg();
	m_imgBtnLine = pSkin->GetToolLineBtnImg();
	m_imgBtnPen = pSkin->GetToolPenBtnImg();
	m_imgBtnText = pSkin->GetToolTextBtnImg();
	m_imgBtnEraser = pSkin->GetToolEraserBtnImg();
	m_imgBtnArrow = pSkin->GetToolArrowBtnImg();
	m_imgBtnWinSelect = pSkin->GetToolWinSelectBtnImg();

	m_strTipBtnMenu = pSkin->GetToolMenuTip();
	m_strTipBtnSelect = pSkin->GetToolSelectTip();
	m_strTipBtnRect = pSkin->GetToolRectTip();
	m_strTipBtnEllipse = pSkin->GetToolEllipseTip();
	m_strTipBtnLine = pSkin->GetToolLineTip();
	m_strTipBtnPen = pSkin->GetToolPenTip();
	m_strTipBtnText = pSkin->GetToolTextTip();
	m_strTipBtnEraser = pSkin->GetToolEraserTip();
	m_strTipBtnArrow = pSkin->GetToolArrowTip();
	m_strTipBtnWinSelect = pSkin->GetToolWinSelectTip();

	m_bmpBtnDrag = pSkin->GetFloatDragBK();
	m_imgBtnDrag = pSkin->GetFloatDragImg();
	m_bmpBtnMenu = pSkin->GetFloatMenuBK();
	m_imgBtnMenu = pSkin->GetFloatMenuImg();
}


// VOID CSCCanvasFloatTool::DrawFrame(CDCHandle dc)
// {
// 	CRect rtClient;
// 	GetClientRect(&rtClient);
// 	CHYUICommonFunctionWrapper::DrawFrame(dc, rtClient, m_clrFrame, 1);
// }


VOID CSCCanvasFloatTool::CheckCurrentTool()
{
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	_ASSERTE(!pContext.IsNull());

	DWORD dwToolType = pContext->GetToolType();
	DWORD dwBtnID(0);

	int nCount(0);
	TOOL_ID_PAIR* arTools = GetToolBtnIDTable(nCount);
	for(int i=0; i<nCount; ++i)
	{
		if(arTools[i].dwToolType == dwToolType)
		{
			dwBtnID = arTools[i].dwBtnID;
			break;
		}
	}

	m_mainToolBar.ResetAllItem();
	if(dwBtnID != 0)
	{
		m_mainToolBar.SetBtnChecked(dwBtnID, TRUE);
		ShowSubFloatTool(dwBtnID);
	}
}
VOID CSCCanvasFloatTool::GetSubFloatToolPos(CPoint& pt)
{
	CRect rtWin;
	GetWindowRect(&rtWin);
	
	pt.x = rtWin.left;
	pt.y = rtWin.bottom + 5;

// 	CRect rtDragBtn(0, 0, 0, 0);
// 	m_btnDrag.GetWindowRect(&rtDragBtn);
// 	pt.x = +=  rtDragBtn.Width();
}
BOOL CSCCanvasFloatTool::CreateFontToolData()
{
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA ta;
	if(!pContext->GetToolData(TOOL_TYPE_TEXT, ta))
	{
		_ASSERTE(FALSE);
		return FALSE;
	}
	
	_ASSERTE(ta.m_dwUserData == NULL);
	if(!ta.m_dwUserData == NULL) return TRUE;

	LPLOGFONT pLogFnt = new LOGFONT;
	if(pLogFnt == NULL) return FALSE;

	ta.m_dwUserData = (DWORD)pLogFnt;
	GetTextLogFontData(*pLogFnt);

	pContext->SetToolData(TOOL_TYPE_TEXT, ta);

	return TRUE;
}

VOID CSCCanvasFloatTool::ResetFontToolData()
{
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA ta;
	if(!pContext->GetToolData(TOOL_TYPE_TEXT, ta))
	{
		return;
	}
	
	if(ta.m_dwUserData != NULL)
	{
		LPLOGFONT pFnt = (LPLOGFONT)ta.m_dwUserData;
		lstrcpyn(pFnt->lfFaceName, GetCanvasSkin()->GetDefaultFontName(), sizeof(pFnt->lfFaceName) / sizeof(TCHAR) - 1);

		::SendMessage(m_hWnd, WM_COMMAND, MAKEWPARAM(SC_TOOL_FONT_NAME_ID, 0), 0);
	}
}

VOID CSCCanvasFloatTool::DeleteFontToolData()
{
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA ta;
	if(!pContext->GetToolData(TOOL_TYPE_TEXT, ta))
	{
		_ASSERTE(FALSE);
		return;
	}
	
	if(ta.m_dwUserData != NULL)
	{
		delete (LPLOGFONT)ta.m_dwUserData;
		ta.m_dwUserData = NULL;
	}
}

VOID CSCCanvasFloatTool::OutputFontToolData()
{
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pToolData = pContext->GetCurrentToolData();
	_ASSERTE(pToolData != NULL);

	if(pToolData->m_dwUserData == 0 )
	{
		_ASSERTE(FALSE);
		return;
	}

	pToolData = pContext->GetCurrentToolData();
	_ASSERTE(pToolData->m_dwUserData != NULL);

	LPLOGFONT pFont = (LPLOGFONT)(pToolData->m_dwUserData);
	
	m_penPicker.SetFontBold(pFont->lfWeight == FW_BOLD);
	m_penPicker.SetFontSize(-pFont->lfHeight);
	m_penPicker.SetPaletteColor(pToolData->m_clrTool);
}
VOID CSCCanvasFloatTool::OutputPenToolData()
{
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pToolData = pContext->GetCurrentToolData();
	_ASSERTE(pToolData != NULL);

	DWORD dwPenType(NONE_PEN);
	INT nPenCount(0);
	PEN_TYPE_ID_PAIR* pPenTable = GetPenTypeIDTable(nPenCount);
	for(int i=0; i<nPenCount; ++i)
	{
		if(pPenTable[i].dwWidth == pToolData->m_nWidth)
		{
			dwPenType = pPenTable[i].dwType;
			break;
		}
	}

	m_penPicker.SetPenType(dwPenType);
	m_penPicker.SetPaletteColor(pToolData->m_clrTool);
}
VOID CSCCanvasFloatTool::InputFontToolData()
{
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();

	CANVAS_TOOL_DATA* pToolData = pContext->GetCurrentToolData();
	_ASSERTE(pToolData != NULL);

	if(pToolData->m_dwUserData == 0 )
	{
		_ASSERTE(FALSE);
		return;
	}

	pToolData = pContext->GetCurrentToolData();
	_ASSERTE(pToolData->m_dwUserData != NULL);

	CANVAS_TOOL_DATA data(*pToolData);
	LPLOGFONT pFont = LPLOGFONT(data.m_dwUserData);

	data.m_clrTool = m_penPicker.GetPaletteColor();
	pFont->lfHeight =  -m_penPicker.GetFontSize();
	pFont->lfWeight = (m_penPicker.GetFontBold() ? FW_BOLD : FW_NORMAL);
	pContext->SetCurrentToolData(data);
}

VOID CSCCanvasFloatTool::InputPenToolData()
{
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pToolData = pContext->GetCurrentToolData();
	_ASSERTE(pToolData != NULL);
	
	CANVAS_TOOL_DATA data(*pToolData);
	
	data.m_clrTool = m_penPicker.GetPaletteColor();
	DWORD dwPenType = m_penPicker.GetPenType();

	INT nPenCount(0);
	PEN_TYPE_ID_PAIR* pPenTable = GetPenTypeIDTable(nPenCount);

	DWORD dwWidth(PEN_SMALL_WIDTH);
	for(int i=0; i<nPenCount; ++i)
	{
		if(pPenTable[i].dwType == dwPenType)
		{
			dwWidth = pPenTable[i].dwWidth;
			break;
		}
	}
	data.m_nWidth = dwWidth;
	
	pContext->SetCurrentToolData(data);
}
VOID CSCCanvasFloatTool::ShowFontSelectTool()
{
	m_penPicker.ShowPenWidthTools(FALSE);
	m_penPicker.ShowFontTools(TRUE);

	m_penPicker.SendMessage(WM_SIZE, 0, 0);

	CPoint ptPos;
	GetSubFloatToolPos(ptPos);
	m_penPicker.SetWindowPos(m_hWnd, ptPos.x, ptPos.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	
	OutputFontToolData();
}
VOID CSCCanvasFloatTool::ShowPenSelectTool()
{
	m_penPicker.ShowPenWidthTools(TRUE);
	m_penPicker.ShowFontTools(FALSE);

	m_penPicker.SendMessage(WM_SIZE, 0, 0);

	CPoint ptPos;
	GetSubFloatToolPos(ptPos);
	m_penPicker.SetWindowPos(m_hWnd, ptPos.x, ptPos.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	
	OutputPenToolData();
}

VOID CSCCanvasFloatTool::ShowSubFloatTool(DWORD dwBtnID)
{
	m_penPicker.ShowWindow(SW_HIDE);

	switch(dwBtnID)
	{
	case SC_TOOL_RECT_ID:
	case SC_TOOL_ELLIPSE_ID:
	case SC_TOOL_LINE_ID:
	case SC_TOOL_ARROW_ID:
	case SC_TOOL_PEN_ID:
		ShowPenSelectTool();	
		break;

	case SC_TOOL_TEXT_ID:
		ShowFontSelectTool();

	default:
		break;
	}
}
LRESULT CSCCanvasFloatTool::OnMove(UINT, WPARAM, LPARAM, BOOL& )
{
	CPoint ptPos;
	GetSubFloatToolPos(ptPos);
	
	if(m_penPicker.IsWindow() && m_penPicker.IsWindowVisible())
	{
		m_penPicker.SetWindowPos(NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);		
	}

	return S_OK;
}
LRESULT CSCCanvasFloatTool::OnCommand(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	
	WORD nCmdID = LOWORD(wParam);
	
	if(	nCmdID == SC_TOOL_SMALL_PEN_ID 
		|| nCmdID == SC_TOOL_MIDDLE_PEN_ID	
		|| nCmdID == SC_TOOL_BIG_PEN_ID)
	{
		InputPenToolData();
		bHandled = TRUE;
	}
	else if(nCmdID == SC_TOOL_FONT_BOLD_ID
		|| nCmdID == SC_TOOL_FONT_SIZE_ID)
	{
		InputFontToolData();
		bHandled = TRUE;
	}
	else if(nCmdID == SC_TOOL_COLOR_PICKER_ID)
	{
		bHandled = TRUE;
		if(GetCanvasContext()->GetToolType() == TOOL_TYPE_TEXT)
		{
			InputFontToolData();
		}
		else
		{
			InputPenToolData();
		}
	}
	else if(nCmdID == SC_TOOL_EXTEND_MENU_ID)
	{
		bHandled = TRUE;
		
		TrackToolExtendMenu();
	}
	

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(nMsgType, wParam, lParam);

	return S_OK;
}


VOID CSCCanvasFloatTool::TrackToolExtendMenu()
{
	CPoint ptTrack;
	BOOL bTopAlign;
	GetMenuTrackPos(ptTrack, bTopAlign);

	UINT nFlag(TPM_BOTTOMALIGN);
	if(bTopAlign)
	{
		nFlag = TPM_TOPALIGN;
	}
	nFlag |= TPM_LEFTALIGN;

	TrackCanvasMenu(ptTrack, nFlag);
}

VOID CSCCanvasFloatTool::GetMenuTrackPos(CPoint& pt, BOOL& bTopAlign)
{
	CRect rtWin;
	m_btnMenu.GetWindowRect(&rtWin);
	
	bTopAlign = FALSE;

	int nY = rtWin.bottom;
	if(!bTopAlign)
	{
		nY	 = rtWin.top;
	}
	pt.y = nY;
	pt.x = rtWin.left;	
}
VOID CSCCanvasFloatTool::AppendLanMenu(HMENU hMenu)
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();

	HMENU hMenuLan = CreatePopupMenu();
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuLan, pSkin->GetResString(IDS_MENU_LAN));

	int nCount = pSkin->GetLanCount();
	for(int i=0; i<nCount; ++i)
	{
		CString strName = pSkin->GetLanNameByIndex(i);
		AppendMenu(hMenuLan, MF_STRING, SC_LAN_TYPE_ID_BASE + i + 1, strName);
	}

	DWORD dwType = pSkin->GetLanType();
	CheckMenuItem(hMenuLan, SC_LAN_TYPE_ID_BASE + dwType, MF_CHECKED | MF_BYCOMMAND);
}
VOID CSCCanvasFloatTool::AppendSkinMenu(HMENU hMenu)
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();

	HMENU hMenuSkin = CreatePopupMenu();
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuSkin, pSkin->GetResString(IDS_MENU_SKIN));

	int nCount = pSkin->GetSkinCount();
	for(int i=0; i<nCount; ++i)
	{
		CString strName = pSkin->GetSkinNameByIndex(i);
		AppendMenu(hMenuSkin, MF_STRING, SC_SKIN_TYPE_ID_BASE + i + 1, strName);
	}

	DWORD dwType = pSkin->GetSkinType();
	CheckMenuItem(hMenuSkin, SC_SKIN_TYPE_ID_BASE + dwType, MF_CHECKED | MF_BYCOMMAND);
}

VOID CSCCanvasFloatTool::AppendToolMenu(HMENU hMenu)
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();

	HMENU hMenuTool = CreatePopupMenu();
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuTool, pSkin->GetResString(IDS_MENU_TOOL));

	AppendMenu(hMenuTool, MF_STRING, SC_TOOL_SELECT_ID, pSkin->GetResString(IDS_MENU_SELECT));
	AppendMenu(hMenuTool, MF_STRING, SC_TOOL_WIN_SELECT_ID, pSkin->GetResString(IDS_MENU_WIN_SELECT));
	AppendMenu(hMenuTool, MF_SEPARATOR, NULL, NULL);

	AppendMenu(hMenuTool, MF_STRING, SC_TOOL_RECT_ID, pSkin->GetResString(IDS_MENU_RECT));
	AppendMenu(hMenuTool, MF_STRING, SC_TOOL_ELLIPSE_ID, pSkin->GetResString(IDS_MENU_ELLIPSE));
	AppendMenu(hMenuTool, MF_STRING, SC_TOOL_LINE_ID, pSkin->GetResString(IDS_MENU_LINE));
	AppendMenu(hMenuTool, MF_STRING, SC_TOOL_ARROW_ID, pSkin->GetResString(IDS_MENU_ARROW));
	AppendMenu(hMenuTool, MF_STRING, SC_TOOL_PEN_ID, pSkin->GetResString(IDS_MENU_PEN));
	AppendMenu(hMenuTool, MF_STRING, SC_TOOL_TEXT_ID, pSkin->GetResString(IDS_MENU_TEXT));
	AppendMenu(hMenuTool, MF_SEPARATOR, NULL, NULL);

	AppendMenu(hMenuTool, MF_STRING, SC_TOOL_ERASER_ID, pSkin->GetResString(IDS_MENU_ERASER));

	DWORD dwToolType = pContext->GetToolType();
	DWORD dwBtnID(0);

	int nCount(0);
	TOOL_ID_PAIR* arTools = GetToolBtnIDTable(nCount);
	for(int i=0; i<nCount; ++i)
	{
		if(arTools[i].dwToolType == dwToolType)
		{
			dwBtnID = arTools[i].dwBtnID;
			break;
		}
	}
	CheckMenuItem(hMenuTool, dwBtnID, MF_BYCOMMAND | MF_CHECKED);

}

VOID CSCCanvasFloatTool::AppendOperationMenu(HMENU hMenu)
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();

	// undo and redo command
	UINT nFlag(MF_STRING);
	if(!pCanvas->CanUndo()) nFlag |= MF_GRAYED;
	AppendMenu(hMenu, nFlag, SC_TOOL_UNDO_CMD_ID, pSkin->GetResString(IDS_MENU_UNDO));
	
	nFlag = MF_STRING;
	if(!pCanvas->CanRedo()) nFlag |= MF_GRAYED;
	AppendMenu(hMenu, nFlag, SC_TOOL_REDO_CMD_ID, pSkin->GetResString(IDS_MENU_REDO));

	AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);

// 	//show help tip
// 	nFlag = MF_STRING;
// 	BOOL bShowHelp = pContext->GetShowHelpFlag();
// 	if(bShowHelp) nFlag |= MF_CHECKED;
// 	AppendMenu(hMenu, nFlag, SC_SHOW_HELP_TIP_CMD_ID, pSkin->GetResString(IDS_MENU_HELP_TIP));
// 	
// 	//Show Cursor
// 	nFlag = MF_STRING;
// 	BOOL bShowCursor = pContext->GetShowCursorFlag();
// 	if(bShowCursor) nFlag |= MF_CHECKED;
// 	AppendMenu(hMenu, nFlag, SC_SHOW_CURSOR_CMD_ID, pSkin->GetResString(IDS_MENU_SHOW_CURSOR));
// 
// 	
// 	//Transparent Annotation
// 	nFlag = MF_STRING;
// 	BOOL bTransparentMark = pContext->GetTransparentMark();
// 	if(bTransparentMark) nFlag |= MF_CHECKED;
// 	AppendMenu(hMenu, nFlag, SC_TRANSPARENT_MARK_ID, pSkin->GetResString(IDS_TRANSPARENT_ANNOTATION));
// 
// 	AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
// 
// 	//double click to exit
// 	nFlag = MF_STRING;
// 	BOOL bDoubleClickExit = pContext->GetDoubleClickExitFlag();
// 	if(bDoubleClickExit) nFlag |= MF_CHECKED;
// 	AppendMenu(hMenu, nFlag, SC_DOUBLE_CLICK_TO_EXIT, pSkin->GetResString(IDS_MENU_DOUBLE_CLICK_EXIT));

	AppendMenu(hMenu, MF_STRING, SC_SETTING_ID, pSkin->GetResString(IDS_SETTING));

	AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
	
	AppendMenu(hMenu, MF_STRING, SC_ABOUT_ID, pSkin->GetResString(IDS_ABOUT));
	AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);

	//save
	AppendMenu(hMenu, MF_STRING, SC_SAVE_CMD_ID, pSkin->GetResString(IDS_MENU_SAVE));
	AppendMenu(hMenu, MF_STRING, SC_SAVE_EDIT_CMD_ID, pSkin->GetResString(IDS_MENU_SAVE_EDIT));

	AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);

	//copy and exit
	AppendMenu(hMenu, MF_STRING, SC_COPY_EXIT_CMD_ID, pSkin->GetResString(IDS_COPY_EXIT));

	//Exit
	AppendMenu(hMenu, MF_STRING, SC_EXIT_CMD_ID, pSkin->GetResString(IDS_MENU_EXIT));
}

HMENU CSCCanvasFloatTool::CreateMainMenu()
{
	

	HMENU hMenu = CreatePopupMenu();
	
	AppendToolMenu(hMenu);
	AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
	AppendSkinMenu(hMenu);
	AppendLanMenu(hMenu);
	AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
	AppendOperationMenu(hMenu);
	
	return hMenu;	
}

VOID CSCCanvasFloatTool::TrackCanvasMenu(CPoint& ptTrack, int nFlag)
{
	if(nFlag == 0) nFlag = 	TPM_LEFTALIGN | TPM_TOPALIGN;
	
	CMenu menu = CreateMainMenu();

	InitMenuCache(menu);
	TrackPopupMenuEx(menu,  nFlag , ptTrack.x, ptTrack.y, *this, NULL);
	ClearMenuCache();
}

HBITMAP CSCCanvasFloatTool::GetMenuIconByID(INT nID)
{
	return NULL;
}