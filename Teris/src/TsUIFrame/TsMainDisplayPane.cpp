#include "TsMainDisplayPane.h"
#include "TsDrawBase.h"
#include "TsDataKernel.h"
#include "TerisRes.h"
#include "TsResMgr.h"
#include "TsGameContext.h"
#include "TsDisplayContainer.h"
#include "TsWinFrame.h"
#include "TsUIInstanceApi.h"

#define ELE_NEXT_GAP_LEFT 8
#define ELE_NEXT_GAP_TOP  4
#define ELE_NEXT_WIDTH 20
#define ELE_NEXT_FRAME 2 

#define LEFT_SCORE_COLOR	RGB(255, 0, 0)
#define RIGHT_SCORE_COLOR	RGB(0, 0, 255)
#define CAPTION_SCORE_COLOR RGB(0, 0, 0)
#define SCORE_BACK_COLOR	RGB(255, 233, 122)
#define SCORE_TEXT_HEIGHT   20

#define SINGLE_SCORE_WIDTH 100
#define MULTI_SCORE_WIDTH 140

#define DISPLAY_LEFT_MARGIN 20
#define DISPLAY_RIGHT_MARGIN 20
#define DISPLAY_TOP_MARGIN 20
#define DISPLAY_BOTTOM_MARGIN 20
#define DISPLAY_SCORE_MARGIN 10
#define DISPLAY_NEXT_MARGIN 10

CTsNextElementContainer::CTsNextElementContainer()
{
	m_pElement = NULL;
}
LRESULT CTsNextElementContainer::OnSetNextElement(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	int nIndex = wParam;
	CTsDataFactory* data = TsGetDataFactory();
	_ASSERTE(data != NULL);

	if(nIndex == -1)
	{
		m_pElement = NULL;
	}

//	_ASSERTE(nIndex >= 0  && nIndex < data->GetElementCount());
	if(nIndex >= 0 && nIndex < data->GetElementCount())
	{
		m_pElement = data->GetElementByIndex(nIndex);
		_ASSERTE(m_pElement != NULL);
	}
	
	InvalidateRect(NULL, TRUE);

	return S_OK;
}
CSize CTsNextElementContainer::GetIdealSize()
{
	CSize szRet(0, 0);
	szRet.cx = ELE_NEXT_GAP_LEFT * 2 + ELE_NEXT_WIDTH * 4;
	szRet.cy = ELE_NEXT_GAP_TOP * 2 + ELE_NEXT_WIDTH * 2;
	return szRet;
}

LRESULT CTsNextElementContainer::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return TRUE;
}
void CTsNextElementContainer::_DrawBkgnd(CDCHandle dc)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	dc.FillSolidRect(&rtClient, RGB(0, 0, 0));
}
void CTsNextElementContainer::_DrawElement(CDCHandle dc)
{
	if(m_pElement == NULL) return;

	COLORREF clrLeftTop = m_pElement->m_clrLeftTopFrame;
	COLORREF clrFace = m_pElement->m_clrFace;
	COLORREF clrBottomRight = m_pElement->m_clrRightBottomFrame	;

	CTsBinData* pBinData = &(m_pElement->m_arData)[1];

	for(int i=1; i<TS_ELEMENT_WIDHT - 1; ++i)
	{
		for(int j=0; j<TS_ELEMENT_WIDHT; ++j)
		{
			if(pBinData->m_arBin[i][j] != 0)
			{
				CRect rtDraw;
				rtDraw.left = ELE_NEXT_GAP_LEFT + j * ELE_NEXT_WIDTH;
				rtDraw.top = ELE_NEXT_GAP_TOP + (i - 1) * ELE_NEXT_WIDTH;
				rtDraw.right = rtDraw.left + ELE_NEXT_WIDTH;
				rtDraw.bottom = rtDraw.top + ELE_NEXT_WIDTH;
				
				const int nGap = CTsField::GetGapWidth();
				rtDraw.InflateRect(-nGap, -nGap, -nGap, -nGap);

				dc.FillSolidRect(&rtDraw, clrFace);
				DrawBorder(dc, rtDraw, clrLeftTop, clrBottomRight, ELE_NEXT_FRAME);
			}
		}
	}
}
LRESULT CTsNextElementContainer::OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CPaintDC dcPaint(m_hWnd);
	CHYMemDC dc(dcPaint);
	
	_DrawBkgnd(dc.m_hDC);
	_DrawElement(dc.m_hDC);

	return S_OK;
}

///////////////////////////  CTsSingleScoreContainer  ///////////////


void CTsSingleScoreContainer::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	_ASSERTE(wParam == TS_WPARAM_PROCESS_CHANGED);
	if(wParam == TS_WPARAM_PROCESS_CHANGED)
	{
		InvalidateRect(NULL, TRUE);
	}
}
CFontHandle CTsSingleScoreContainer::_GetTextFont()
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
LRESULT CTsSingleScoreContainer::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CTsResMgr* resMgr = TsGetResMgr();
	_ASSERTE(resMgr != NULL);

	m_szEleCount[0] = 0;
	m_szRemoveCount[0] = 0;
	m_szScore[0] = 0;

	resMgr->LoadString(IDS_ELEMENT_COUNT, m_szEleCount, sizeof(m_szEleCount) / sizeof(TCHAR));
	resMgr->LoadString(IDS_SCORE, m_szScore, sizeof(m_szScore) / sizeof(TCHAR));
	resMgr->LoadString(IDS_REMOVE_LINE, m_szRemoveCount, sizeof(m_szRemoveCount) / sizeof(TCHAR));

	return S_OK;
}
LRESULT CTsSingleScoreContainer::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return TRUE;
}
void CTsSingleScoreContainer::_DrawBkgnd(CDCHandle dc)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	dc.FillSolidRect(&rtClient, SCORE_BACK_COLOR);
}

CSize CTsSingleScoreContainer::GetIdealSize()
{
	CRect rt1 = _GetTextRect(ELE_COUNT_CAPTION);
	CRect rt2 = _GetTextRect(REMOVE_LINE_VALUE);

	return CSize(SINGLE_SCORE_WIDTH, rt2.bottom - rt1.top);
}
CRect CTsSingleScoreContainer::_GetTextRect(int nIndex)
{
	_ASSERTE(nIndex >= (int)ELE_COUNT_CAPTION && nIndex <= (int)REMOVE_LINE_VALUE);
	CRect rtClient(0, 0, 0, 0);
	if(IsWindow())
	{
		GetClientRect(&rtClient);
	}

	int nHeight = SCORE_TEXT_HEIGHT;

	return CRect(0, nIndex * SCORE_TEXT_HEIGHT, rtClient.right, nIndex*SCORE_TEXT_HEIGHT + nHeight);
}
void CTsSingleScoreContainer::_DrawCaption(CDCHandle dc)
{
	COLORREF clrCaption(CAPTION_SCORE_COLOR);

	CFontHandle hOldFont = dc.SelectFont(_GetTextFont());
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(clrCaption);
	CRect rtDraw = _GetTextRect(ELE_COUNT_CAPTION);
	dc.DrawText(m_szEleCount, lstrlen(m_szEleCount), &rtDraw, DT_CENTER | DT_END_ELLIPSIS | DT_VCENTER);
	rtDraw = _GetTextRect(SCORE_CAPTION);
	dc.DrawText(m_szScore, lstrlen(m_szScore), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	rtDraw = _GetTextRect(REMOVE_LINE_CAPTION);
	dc.DrawText(m_szRemoveCount, lstrlen(m_szRemoveCount), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	dc.SelectFont(hOldFont);
}

void CTsSingleScoreContainer::_DrawValue(CDCHandle dc)
{
	CTsGameContext* pContext = TsGetGameContext();	
	_ASSERTE(pContext != NULL);

	COLORREF clrText(LEFT_SCORE_COLOR);

	TS_USE_WIN_TYPE useWin = pContext->GetUseWinType();
	if(useWin == USE_RIGHT_WIN)
	{
		clrText = RIGHT_SCORE_COLOR;
	}

	CTsPlayProcessInfo* pPlayInfo = TsGetGameProcessInfo();
	_ASSERTE(pPlayInfo != NULL);
	CTsPlayProcessInfo& tp = *pPlayInfo;
	INT nEleCount(tp.GetLeftElementCount());
	INT nScoreCount(tp.GetLeftScore());
	INT nRemoveLine(tp.GetLeftRemoveLineCount());
	if(useWin == USE_RIGHT_WIN)
	{
		nEleCount = tp.GetRightElementCount();
		nScoreCount = tp.GetRightScore();
		nRemoveLine = tp.GetRightRemoveLineCount();
	}
	CFontHandle hOldFont = dc.SelectFont(_GetTextFont());
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(clrText);
	TCHAR szBuffer[64] = {0};
	_ltot(nEleCount, szBuffer, 10);
	CRect rtDraw = _GetTextRect(ELE_COUNT_VALUE);
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	_ltot(nScoreCount, szBuffer, 10);
	rtDraw = _GetTextRect(SCORE_VALUE);
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	_ltot(nRemoveLine, szBuffer, 10);
	rtDraw = _GetTextRect(REMOVE_LINE_VALUE);
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	dc.SelectFont(hOldFont);
}
void CTsSingleScoreContainer::_DrawScore(CDCHandle dc)
{
	_DrawCaption(dc);
	_DrawValue(dc);	
}
LRESULT CTsSingleScoreContainer::OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CPaintDC paintDC(m_hWnd);
	CHYMemDC memDC(paintDC);
	CDCHandle dc(memDC);

	_DrawBkgnd(dc);
	_DrawScore(dc);

	return S_OK;
}
/////////////////////////////// CTsMutiScoreContainer ////////
void CTsMutiScoreContainer::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	_ASSERTE(wParam == TS_WPARAM_PROCESS_CHANGED);
	if(wParam == TS_WPARAM_PROCESS_CHANGED)
	{
		InvalidateRect(NULL, TRUE);
	}
}
CFontHandle CTsMutiScoreContainer::_GetTextFont()
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
void CTsMutiScoreContainer::_DrawScore(CDCHandle dc)
{
	_DrawCaption(dc);
	_DrawValue(dc);
}
void CTsMutiScoreContainer::_DrawValue(CDCHandle dc)
{
	CTsPlayProcessInfo* pPlayInfo = TsGetGameProcessInfo();
	_ASSERTE(pPlayInfo != NULL);
	CTsPlayProcessInfo& tp = *pPlayInfo;

	CFontHandle hOldFont = dc.SelectFont(_GetTextFont());
	dc.SetBkMode(TRANSPARENT);

	TCHAR szBuffer[64] = {0};

	//draw left score
	dc.SetTextColor(LEFT_SCORE_COLOR);
	CRect rtDraw = _GetTextRect(PLAYER_NAME, 1);
	dc.DrawText(tp.GetLeftPlayerName(), lstrlen(tp.GetLeftPlayerName()), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	
	rtDraw = _GetTextRect(ELE_COUNT_VALUE, 1);
	_ltot(tp.GetLeftElementCount(), szBuffer, 10);
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	rtDraw = _GetTextRect(SCORE_VALUE, 1);
	_ltot(tp.GetLeftScore(), szBuffer, 10);
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	rtDraw = _GetTextRect(REMOVE_LINE_VALUE, 1);
	_ltot(tp.GetLeftRemoveLineCount(), szBuffer, 10);
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	rtDraw = _GetTextRect(KILL_LINE_VALUE, 1);
	//_ltot(tp.GetLeftKillLineCount(), szBuffer, 10);
	_sntprintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), _T("%u ->"), tp.GetLeftKillLineCount());
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	rtDraw = _GetTextRect(WIN_COUNT_VALUE, 1);
	_ltot(tp.GetLeftWinCount(), szBuffer, 10);
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	//draw right score
	dc.SetTextColor(RIGHT_SCORE_COLOR);
	rtDraw = _GetTextRect(PLAYER_NAME, 2);
	dc.DrawText(tp.GetRightPlayerName(), lstrlen(tp.GetRightPlayerName()), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	
	rtDraw = _GetTextRect(ELE_COUNT_VALUE, 2);
	_ltot(tp.GetRightElementCount(), szBuffer, 10);
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	rtDraw = _GetTextRect(SCORE_VALUE, 2);
	_ltot(tp.GetRightScore(), szBuffer, 10);
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	rtDraw = _GetTextRect(REMOVE_LINE_VALUE, 2);
	_ltot(tp.GetRightRemoveLineCount(), szBuffer, 10);
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	rtDraw = _GetTextRect(KILL_LINE_VALUE, 2);
//	_ltot(tp.GetRightKillLineCount(), szBuffer, 10);
	_sntprintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), _T("%<- %u"), tp.GetRightKillLineCount());
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	rtDraw = _GetTextRect(WIN_COUNT_VALUE, 2);
	_ltot(tp.GetRightWinCount(), szBuffer, 10);
	dc.DrawText(szBuffer, lstrlen(szBuffer), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	dc.SelectFont(hOldFont);
}

void CTsMutiScoreContainer::_DrawCaption(CDCHandle dc)
{
	COLORREF clrCaption(CAPTION_SCORE_COLOR);

	CFontHandle hOldFont = dc.SelectFont(_GetTextFont());
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(clrCaption);
	CRect rtDraw = _GetTextRect(PLAYER_CAPTION, 0);
	dc.DrawText(m_szPlayer, lstrlen(m_szPlayer), &rtDraw, DT_CENTER | DT_END_ELLIPSIS | DT_VCENTER);
	rtDraw = _GetTextRect(ELE_COUNT_CAPTION, 0);
	dc.DrawText(m_szEleCount, lstrlen(m_szEleCount), &rtDraw, DT_CENTER | DT_END_ELLIPSIS | DT_VCENTER);
	rtDraw = _GetTextRect(SCORE_CAPTION, 0);
	dc.DrawText(m_szScore, lstrlen(m_szScore), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	rtDraw = _GetTextRect(REMOVE_LINE_CAPTION, 0);
	dc.DrawText(m_szRemoveCount, lstrlen(m_szRemoveCount), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	rtDraw = _GetTextRect(WIN_COUNT_CAPTION, 0);
	dc.DrawText(m_szWinCount, lstrlen(m_szWinCount), &rtDraw, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
	dc.SelectFont(hOldFont);
}

CRect CTsMutiScoreContainer::_GetTextRect(int nIndex, INT nType)
{
	_ASSERTE(nIndex >= (int)PLAYER_CAPTION && nIndex <= (int)WIN_COUNT_VALUE);
	CRect rtClient(0, 0, 0, 0);
	if(IsWindow())
	{
		GetClientRect(&rtClient);
	}

	int nHeight = SCORE_TEXT_HEIGHT;
	
	if(nType == 0)
	{
		return CRect(0, nIndex * SCORE_TEXT_HEIGHT, rtClient.right, nIndex*SCORE_TEXT_HEIGHT + nHeight);
	}
	else if(nType == 1)
	{
		return CRect(0, nIndex * SCORE_TEXT_HEIGHT, rtClient.right / 2, nIndex*SCORE_TEXT_HEIGHT + nHeight);
	}
	else if(nType == 2)
	{
		return CRect(rtClient.right / 2, nIndex * SCORE_TEXT_HEIGHT, rtClient.right, nIndex*SCORE_TEXT_HEIGHT + nHeight);
	}
	else
	{
		_ASSERTE(FALSE);
		return CRect(0, 0, 0, 0);
	}
}
LRESULT CTsMutiScoreContainer::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CTsResMgr* resMgr = TsGetResMgr();
	_ASSERTE(resMgr != NULL);

	m_szEleCount[0] = 0;
	m_szRemoveCount[0] = 0;
	m_szScore[0] = 0;
	m_szWinCount[0] = 0;
	m_szPlayer[0] = 0;

	resMgr->LoadString(IDS_ELEMENT_COUNT, m_szEleCount, sizeof(m_szEleCount) / sizeof(TCHAR));
	resMgr->LoadString(IDS_SCORE, m_szScore, sizeof(m_szScore) / sizeof(TCHAR));
	resMgr->LoadString(IDS_REMOVE_LINE, m_szRemoveCount, sizeof(m_szRemoveCount) / sizeof(TCHAR));
	resMgr->LoadString(IDS_PLAYER_NAME, m_szPlayer, sizeof(m_szPlayer) / sizeof(TCHAR));
	resMgr->LoadString(IDS_WIN_COUNT, m_szWinCount, sizeof(m_szWinCount) / sizeof(TCHAR));

	return S_OK;
}
LRESULT CTsMutiScoreContainer::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return TRUE;
}
void CTsMutiScoreContainer::_DrawBkgnd(CDCHandle dc)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	dc.FillSolidRect(&rtClient, SCORE_BACK_COLOR);
}

CSize CTsMutiScoreContainer::GetIdealSize()
{
	CRect rt1 = _GetTextRect(PLAYER_CAPTION, 0);
	CRect rt2 = _GetTextRect(WIN_COUNT_VALUE, 0);

	return CSize(MULTI_SCORE_WIDTH, rt2.bottom - rt1.top);
}

LRESULT CTsMutiScoreContainer::OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CPaintDC paintDC(m_hWnd);
	CHYMemDC memDC(paintDC);
	CDCHandle dc(memDC);

	_DrawBkgnd(dc);
	_DrawScore(dc);

	return S_OK;
}


/////////////////////////////  CTsMainDisplayPane  /////////////////////
CTsMainDisplayPane::CTsMainDisplayPane()
{
	m_pLeftNextContainer = m_pRightNextContainer = NULL;
	m_pLeftDisplayContainer = m_pRightDisplayContainer = NULL;
	m_pSingleScoreContainer = NULL;
	m_pMultiScoreContainer = NULL;

	m_pLeftDisplayFrame = m_pRightDisplayFrame = NULL;
	m_pLeftNextFrame = m_pRightNextFrame = NULL;
	m_pSingleScoreFrame = m_pMultiScoreFrame = NULL;
}
CTsMainDisplayPane::~CTsMainDisplayPane()
{
	_DeleteObj();
}
void CTsMainDisplayPane::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == TS_WPARAM_DISPLAY_MODE_CHANGED)
	{
		_CheckDisplayStatus();
	}
	if(wParam == TS_WPARAM_GAME_MODE_CHANGED)
	{
		_CheckGameMode();
		_ClearDisplayData();
	}
}
void CTsMainDisplayPane::_ClearDisplayData()
{
	m_pLeftDisplayContainer->ClearDisplay();
	m_pRightDisplayContainer->ClearDisplay();

	m_pLeftNextContainer->SendMessage(WM_TS_SET_NEXT_ELEMENT, -1, 0);
	m_pRightNextContainer->SendMessage(WM_TS_SET_NEXT_ELEMENT, -1, 0);
}
LRESULT CTsMainDisplayPane::OnUpdateDisplay(UINT, WPARAM, LPARAM, BOOL&)
{
	_CheckDisplayStatus();

	return S_OK;
}
void CTsMainDisplayPane::_CheckGameMode()
{
	CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
	_ASSERTE(pBaseInfo != NULL);

	TS_DISPLAY_MODE eDisplayMode(DISPLAY_NONE);
	switch(pBaseInfo->GetPlayMode())
	{
	case NONE_MODE:
	case NET_SELECT_MODE:
	case RECORD_SELECT_MODE:
		eDisplayMode = DISPLAY_NONE;
		break;
	
	case SINGLE_PLAYER_MODE:
		eDisplayMode = DISPLAY_SINGLE;
		break;
	
	case TWO_PLAYER_MODE:
	case COMPUTER_PLAYER_MODE:
	case NET_PLAY_MODE:
		eDisplayMode = DISPLAY_MULTI;
		break;
	
	default:
		break;
	}

	pBaseInfo->SetDisplayMode(eDisplayMode);

}
void CTsMainDisplayPane::_CheckDisplayStatus()
{
	CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
	_ASSERTE(pBaseInfo != NULL);
	CTsGameBaseInfo& ti = *pBaseInfo;
	
	TS_DISPLAY_MODE eMode = ti.GetDisplayMode();
	switch(eMode)
	{
	case DISPLAY_NONE:
		_SetNoneStatus();
		break;

	case DISPLAY_SINGLE:
		_SetSingleStatus();
		_AdjustSingle();
		break;

	case DISPLAY_MULTI:
		_SetMultiStatus();
		_AdjustMulti();
		break;
	
	default:
		_ASSERTE(FALSE);
		break;
	}
	
	InvalidateRect(NULL, TRUE);
}
void CTsMainDisplayPane::_AdjustSingleFun(BOOL bLeft)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	BOOL bXMin(FALSE), bYMin(FALSE);

	CSize szScore = m_pSingleScoreContainer->GetIdealSize();
	CRect rtEmpty(0, 0, 0, 0);
	CRect rtRet = m_pSingleScoreFrame->GetIdealRect(rtEmpty);
	CSize szScoreFrame(rtRet.Width(), rtRet.Height());

	CTsNextElementContainer* pNextContainer = m_pRightNextContainer;
	CTsDisplayContainer* pDisplayContainer = m_pRightDisplayContainer;
	ITsWinFrame* pNextFrame = m_pRightNextFrame;
	ITsWinFrame* pDisplayFrame = m_pRightDisplayFrame;
	
	if(bLeft)
	{
		pNextContainer = m_pLeftNextContainer;
		pDisplayContainer = m_pLeftDisplayContainer;
		pNextFrame = m_pLeftNextFrame;
		pDisplayFrame = m_pLeftDisplayFrame;
	}

	CSize szNext = pNextContainer->GetIdealSize();
	rtRet = pNextFrame->GetIdealRect(rtEmpty);
	CSize szNextFrame(rtRet.Width(), rtRet.Height());

	rtRet = pDisplayFrame->GetIdealRect(rtEmpty);
	CSize szDisplayFrame(rtRet.Width(), rtRet.Height());

	INT nWidthLeft = rtClient.Width() - DISPLAY_LEFT_MARGIN - DISPLAY_RIGHT_MARGIN - DISPLAY_SCORE_MARGIN
		- szScoreFrame.cx - szDisplayFrame.cx - szScore.cx;

	INT nHeightLeft = rtClient.Height() - DISPLAY_TOP_MARGIN - DISPLAY_BOTTOM_MARGIN - DISPLAY_NEXT_MARGIN
		- szNext.cy - szNextFrame.cy - szDisplayFrame.cy;

	CSize szDisplayMin = CTsDisplayContainer::GetMinSize();
	if(nWidthLeft <= szDisplayMin.cx)
	{
		bXMin = TRUE;
	}

	if(nHeightLeft <= szDisplayMin.cy)
	{
		bYMin = TRUE;
	}

	CSize szDisplay = szDisplayMin;
	if(!bXMin && !bYMin)
	{
		szDisplay.cx = nWidthLeft;
		szDisplay.cy = nHeightLeft;
		BOOL bRetCheck = CTsDisplayContainer::GetCorrectAutoSize(szDisplay);
		_ASSERTE(bRetCheck);
	}
	
	CRect rtNextFrame(0, 0, 0, 0), rtDisplayFrame(0, 0, 0, 0), rtScoreFrame(0, 0, 0, 0);
	INT nGapX(0);
	if(!bXMin)
	{
		nGapX = (nWidthLeft - szDisplay.cx /*- szDisplayFrame.cx*/) / 2;
	}
	rtDisplayFrame.left = DISPLAY_LEFT_MARGIN + nGapX;
	rtDisplayFrame.right = rtDisplayFrame.left + szDisplay.cx + szDisplayFrame.cx;

	rtScoreFrame.left = rtDisplayFrame.right + DISPLAY_SCORE_MARGIN;
	rtScoreFrame.right = rtScoreFrame.left + szScore.cx + szScoreFrame.cx;

	rtNextFrame.left = rtDisplayFrame.left + (rtDisplayFrame.Width() - szNext.cx - szNextFrame.cx) / 2;
	rtNextFrame.right = rtNextFrame.left + szNext.cx + szNextFrame.cx;
	

	int nGapY(0);
	if(!bYMin)
	{
		nGapY = (nHeightLeft - szDisplay.cy - szDisplayFrame.cy) / 2;
	}

	rtNextFrame.top = DISPLAY_TOP_MARGIN + nGapY;
	rtNextFrame.bottom = rtNextFrame.top + szNext.cy + szNextFrame.cy;

	rtDisplayFrame.top = rtNextFrame.bottom + DISPLAY_NEXT_MARGIN;
	rtDisplayFrame.bottom = rtDisplayFrame.top + szDisplay.cy + szDisplayFrame.cy;

	rtScoreFrame.bottom = rtDisplayFrame.bottom;
	rtScoreFrame.top = rtScoreFrame.bottom - szScore.cy - szScoreFrame.cy;


	pNextFrame->SetFrameRect(rtNextFrame);
	CRect rtContent = pNextFrame->GetContentRect();
	pNextContainer->SetWindowPos(NULL, rtContent.left, rtContent.top, rtContent.Width(), rtContent.Height(), SWP_NOACTIVATE);

	pDisplayFrame->SetFrameRect(rtDisplayFrame);
	rtContent = pDisplayFrame->GetContentRect();
	pDisplayContainer->SetWindowPos(NULL, rtContent.left, rtContent.top, rtContent.Width(), rtContent.Height(), SWP_NOACTIVATE);
	
	m_pSingleScoreFrame->SetFrameRect(rtScoreFrame);
	rtContent = m_pSingleScoreFrame->GetContentRect();
	m_pSingleScoreContainer->SetWindowPos(NULL, rtContent.left, rtContent.top, rtContent.Width(), rtContent.Height(), SWP_NOACTIVATE);
}
void CTsMainDisplayPane::_AdjustSingle()
{
	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);
	TS_USE_WIN_TYPE eType = pContext->GetUseWinType();
	if(eType == USE_LEFT_WIN)
	{
		_AdjustSingleFun(TRUE);
	}
	else
	{
		_AdjustSingleFun(FALSE);
	}
}

void CTsMainDisplayPane::_AdjustMulti()
{
	CRect rtClient;
	GetClientRect(&rtClient);
	BOOL bXMin(FALSE), bYMin(FALSE);

	CSize szScore = m_pMultiScoreContainer->GetIdealSize();
	CRect rtEmpty(0, 0, 0, 0);
	CRect rtRet = m_pMultiScoreFrame->GetIdealRect(rtEmpty);
	CSize szScoreFrame(rtRet.Width(), rtRet.Height());



	CSize szLeftNext = m_pLeftNextContainer->GetIdealSize();
	rtRet = m_pLeftNextFrame->GetIdealRect(rtEmpty);
	CSize szLeftNextFrame(rtRet.Width(), rtRet.Height());

	CSize szRightNext = m_pRightNextContainer->GetIdealSize();
	rtRet = m_pRightNextFrame->GetIdealRect(rtEmpty);
	CSize szRightNextFrame(rtRet.Width(), rtRet.Height());

	rtRet = m_pLeftDisplayFrame->GetIdealRect(rtEmpty);
	CSize szLeftDisplayFrame(rtRet.Width(), rtRet.Height());

	rtRet = m_pRightDisplayFrame->GetIdealRect(rtEmpty);
	CSize szRightDisplayFrame(rtRet.Width(), rtRet.Height());

	INT nWidthLeft = (rtClient.Width() - DISPLAY_LEFT_MARGIN - DISPLAY_RIGHT_MARGIN - DISPLAY_SCORE_MARGIN * 2
		- szScoreFrame.cx - szLeftDisplayFrame.cx - szScore.cx - szRightDisplayFrame.cx) / 2;

	INT nHeightLeft = rtClient.Height() - DISPLAY_TOP_MARGIN - DISPLAY_BOTTOM_MARGIN - DISPLAY_NEXT_MARGIN
		- szLeftNext.cy - szLeftNextFrame.cy - szLeftDisplayFrame.cy;

	CSize szDisplayMin = CTsDisplayContainer::GetMinSize();
	if(nWidthLeft <= szDisplayMin.cx)
	{
		bXMin = TRUE;
	}

	if(nHeightLeft <= szDisplayMin.cy)
	{
		bYMin = TRUE;
	}

	CSize szDisplay = szDisplayMin;
	if(!bXMin && !bYMin)
	{
		szDisplay.cx = nWidthLeft;
		szDisplay.cy = nHeightLeft;
		BOOL bRetCheck = CTsDisplayContainer::GetCorrectAutoSize(szDisplay);
		_ASSERTE(bRetCheck);
	}
	
	CRect rtLeftNextFrame(0, 0, 0, 0), rtLeftDisplayFrame(0, 0, 0, 0), rtScoreFrame(0, 0, 0, 0);
	CRect rtRightNextFrame(0, 0, 0, 0), rtRightDisplayFrame(0, 0, 0, 0);
	INT nGapX(0);
	if(!bXMin)
	{
		nGapX = nWidthLeft - szDisplay.cx;
	}
	rtLeftDisplayFrame.left = DISPLAY_LEFT_MARGIN + nGapX;
	rtLeftDisplayFrame.right = rtLeftDisplayFrame.left + szDisplay.cx + szLeftDisplayFrame.cx;

	rtScoreFrame.left = rtLeftDisplayFrame.right + DISPLAY_SCORE_MARGIN;
	rtScoreFrame.right = rtScoreFrame.left + szScore.cx + szScoreFrame.cx;
	
	rtRightDisplayFrame.left = rtScoreFrame.right + DISPLAY_SCORE_MARGIN;
	rtRightDisplayFrame.right = rtRightDisplayFrame.left + szDisplay.cx + szRightDisplayFrame.cx;

	rtLeftNextFrame.left = rtLeftDisplayFrame.left + (rtLeftDisplayFrame.Width() - szLeftNext.cx - szLeftNextFrame.cx) / 2;
	rtLeftNextFrame.right = rtLeftNextFrame.left + szLeftNext.cx + szLeftNextFrame.cx;

	rtRightNextFrame.left = rtRightDisplayFrame.left + (rtRightDisplayFrame.Width() - szRightNext.cx - szRightNextFrame.cx) / 2;
	rtRightNextFrame.right = rtRightNextFrame.left + szRightNext.cx + szRightNextFrame.cx;
	

	int nGapY(0);
	if(!bYMin)
	{
		nGapY = (nHeightLeft - szDisplay.cy - szLeftDisplayFrame.cy) / 2;
	}

	rtLeftNextFrame.top = DISPLAY_TOP_MARGIN + nGapY;
	rtLeftNextFrame.bottom = rtLeftNextFrame.top + szLeftNext.cy + szLeftNextFrame.cy;

	rtLeftDisplayFrame.top = rtLeftNextFrame.bottom + DISPLAY_NEXT_MARGIN;
	rtLeftDisplayFrame.bottom = rtLeftDisplayFrame.top + szDisplay.cy + szLeftDisplayFrame.cy;

	rtRightNextFrame.top = DISPLAY_TOP_MARGIN + nGapY;
	rtRightNextFrame.bottom = rtRightNextFrame.top + szRightNext.cy + szRightNextFrame.cy;

	rtRightDisplayFrame.top = rtRightNextFrame.bottom + DISPLAY_NEXT_MARGIN;
	rtRightDisplayFrame.bottom = rtRightDisplayFrame.top + szDisplay.cy + szRightDisplayFrame.cy;


	rtScoreFrame.top = rtLeftDisplayFrame.top + (rtLeftDisplayFrame.Height() - szScoreFrame.cy - szScore.cy) / 2;
	rtScoreFrame.bottom = rtScoreFrame.top + szScore.cy + szScoreFrame.cy;


	m_pLeftNextFrame->SetFrameRect(rtLeftNextFrame);
	CRect rtContent = m_pLeftNextFrame->GetContentRect();
	m_pLeftNextContainer->SetWindowPos(NULL, rtContent.left, rtContent.top, rtContent.Width(), rtContent.Height(), SWP_NOACTIVATE);

	m_pLeftDisplayFrame->SetFrameRect(rtLeftDisplayFrame);
	rtContent = m_pLeftDisplayFrame->GetContentRect();
	m_pLeftDisplayContainer->SetWindowPos(NULL, rtContent.left, rtContent.top, rtContent.Width(), rtContent.Height(), SWP_NOACTIVATE);
	
	
	m_pRightNextFrame->SetFrameRect(rtRightNextFrame);
	rtContent = m_pRightNextFrame->GetContentRect();
	m_pRightNextContainer->SetWindowPos(NULL, rtContent.left, rtContent.top, rtContent.Width(), rtContent.Height(), SWP_NOACTIVATE);

	m_pRightDisplayFrame->SetFrameRect(rtRightDisplayFrame);
	rtContent = m_pRightDisplayFrame->GetContentRect();
	m_pRightDisplayContainer->SetWindowPos(NULL, rtContent.left, rtContent.top, rtContent.Width(), rtContent.Height(), SWP_NOACTIVATE);
	

	m_pMultiScoreFrame->SetFrameRect(rtScoreFrame);
	rtContent = m_pMultiScoreFrame->GetContentRect();
	m_pMultiScoreContainer->SetWindowPos(NULL, rtContent.left, rtContent.top, rtContent.Width(), rtContent.Height(), SWP_NOACTIVATE);

}
void CTsMainDisplayPane::_SetSingleStatus()
{
	_SetNoneStatus();

	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);
	TS_USE_WIN_TYPE eType = pContext->GetUseWinType();
	if(eType == USE_LEFT_WIN)
	{
		m_pLeftDisplayContainer->ShowWindow(SW_SHOW);
		m_pLeftNextContainer->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pRightDisplayContainer->ShowWindow(SW_SHOW);
		m_pRightNextContainer->ShowWindow(SW_SHOW);
	}

	m_pSingleScoreContainer->ShowWindow(SW_SHOW);
}
void CTsMainDisplayPane::_SetMultiStatus()
{
	m_pLeftDisplayContainer->ShowWindow(SW_SHOW);
	m_pLeftNextContainer->ShowWindow(SW_SHOW);
	m_pRightDisplayContainer->ShowWindow(SW_SHOW);
	m_pRightNextContainer->ShowWindow(SW_SHOW);
	m_pSingleScoreContainer->ShowWindow(SW_HIDE);
	m_pMultiScoreContainer->ShowWindow(SW_SHOW);
}
void CTsMainDisplayPane::_SetNoneStatus()
{
	m_pLeftDisplayContainer->ShowWindow(SW_HIDE);
	m_pLeftNextContainer->ShowWindow(SW_HIDE);
	m_pRightDisplayContainer->ShowWindow(SW_HIDE);
	m_pRightNextContainer->ShowWindow(SW_HIDE);
	m_pSingleScoreContainer->ShowWindow(SW_HIDE);
	m_pMultiScoreContainer->ShowWindow(SW_HIDE);
}
BOOL CTsMainDisplayPane::_NewObj()
{
	//new window
	if(m_pLeftDisplayContainer == NULL)
	{
		m_pLeftDisplayContainer = new CTsDisplayContainer;
	}
	if(m_pRightDisplayContainer == NULL)
	{
		m_pRightDisplayContainer = new CTsDisplayContainer;
	}
	if(m_pLeftNextContainer == NULL)
	{
		m_pLeftNextContainer = new CTsNextElementContainer;
	}
	if(m_pRightNextContainer == NULL)
	{
		m_pRightNextContainer = new CTsNextElementContainer;
	}
	if(m_pSingleScoreContainer == NULL)
	{
		m_pSingleScoreContainer = new CTsSingleScoreContainer;
	}
	if(m_pMultiScoreContainer == NULL)
	{
		m_pMultiScoreContainer = new CTsMutiScoreContainer;
	}

	//new Frame
	if(m_pLeftDisplayFrame == NULL)
	{
		m_pLeftDisplayFrame = new CTsColorFrame(RGB(255, 0, 0), 1, 1, NULL);
	}
	if(m_pRightDisplayFrame == NULL)
	{
		m_pRightDisplayFrame = new CTsColorFrame(RGB(255, 0, 0), 1, 1, NULL);
	}
	if(m_pLeftNextFrame == NULL)
	{
		m_pLeftNextFrame = new CTsColorFrame(RGB(255, 0, 0), 1, 1, NULL);
	}
	if(m_pRightNextFrame == NULL)
	{
		m_pRightNextFrame = new CTsColorFrame(RGB(255, 0, 0), 1, 1, NULL);
	}
	if(m_pSingleScoreFrame == NULL)
	{
		m_pSingleScoreFrame = new CTsColorFrame(RGB(255, 0, 0), 1, 1, NULL);
	}
	if(m_pMultiScoreFrame == NULL)
	{
		m_pMultiScoreFrame = new CTsColorFrame(RGB(255, 0, 0), 1, 1, NULL);
	}

	if(m_pLeftNextContainer != NULL
		&& m_pRightNextContainer != NULL
		&& m_pLeftDisplayContainer != NULL
		&& m_pRightDisplayContainer != NULL
		&& m_pSingleScoreContainer != NULL
		&& m_pMultiScoreContainer != NULL
		&& m_pLeftDisplayFrame != NULL
		&& m_pRightDisplayFrame != NULL
		&& m_pLeftNextFrame != NULL
		&& m_pRightNextFrame != NULL
		&& m_pSingleScoreFrame != NULL
		&& m_pMultiScoreFrame != NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CTsMainDisplayPane::_DeleteObj()
{
	//delete window
	if(m_pLeftDisplayContainer != NULL)
	{
		delete m_pLeftDisplayContainer;
		m_pLeftDisplayContainer = NULL;
	}
	if(m_pRightDisplayContainer != NULL)
	{
		delete	m_pRightDisplayContainer;
		m_pRightDisplayContainer = NULL;
	}
	if(m_pLeftNextContainer != NULL)
	{
		delete	m_pLeftNextContainer;
		m_pLeftNextContainer = NULL;
	}
	if(m_pRightNextContainer != NULL)
	{
		delete 	m_pRightNextContainer;
		m_pRightNextContainer = NULL;
	}
	if(m_pSingleScoreContainer != NULL)
	{
		delete	m_pSingleScoreContainer;
		 m_pSingleScoreContainer = NULL;
	}
	if(m_pMultiScoreContainer != NULL)
	{
		delete	m_pMultiScoreContainer;
		m_pMultiScoreContainer = NULL;
	}

	//delete frame
	if(m_pLeftDisplayFrame != NULL)
	{
		delete m_pLeftDisplayFrame;
		m_pLeftDisplayFrame = NULL;
	}
	if(m_pRightDisplayFrame != NULL)
	{
		delete m_pRightDisplayFrame;
		m_pRightDisplayFrame = NULL;
	}
	if(m_pLeftNextFrame != NULL)
	{
		delete m_pLeftNextFrame;
		m_pLeftNextFrame = NULL;
	}
	if(m_pRightNextFrame != NULL)
	{
		delete m_pRightNextFrame;
		m_pRightNextFrame = NULL;
	}
	if(m_pSingleScoreFrame != NULL)
	{
		delete m_pSingleScoreFrame;
		m_pSingleScoreFrame = NULL;
	}
	if(m_pMultiScoreFrame != NULL)
	{
		delete m_pMultiScoreFrame;
		m_pMultiScoreFrame = NULL;
	}
}
BOOL CTsMainDisplayPane::_CreateObj()
{
	CRect rtWin(0, 0, 0, 0);

	m_pLeftDisplayContainer->Create(m_hWnd, rtWin);
	m_pRightDisplayContainer->Create(m_hWnd, rtWin);
	m_pLeftNextContainer->Create(m_hWnd, rtWin);
	m_pRightNextContainer->Create(m_hWnd, rtWin);
	m_pSingleScoreContainer->Create(m_hWnd, rtWin);
	m_pMultiScoreContainer->Create(m_hWnd, rtWin);

	if(m_pLeftNextContainer->IsWindow()
		&& m_pRightNextContainer->IsWindow()
		&& m_pLeftDisplayContainer->IsWindow()
		&& m_pRightDisplayContainer->IsWindow()
		&& m_pSingleScoreContainer->IsWindow()
		&& m_pMultiScoreContainer->IsWindow())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void CTsMainDisplayPane::_DrawNoneBkgnd(CDCHandle dc)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	dc.FillSolidRect(&rtClient, RGB(0, 0, 0));
}
void CTsMainDisplayPane::_DrawSingleBkgnd(CDCHandle dc)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	dc.FillSolidRect(&rtClient, RGB(100, 200, 80));	
}
void CTsMainDisplayPane::_DrawMultiBkgnd(CDCHandle dc)
{
	_DrawSingleBkgnd(dc);
}
void CTsMainDisplayPane::_DrawSingleFrame(CDCHandle dc)
{
	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);
	TS_USE_WIN_TYPE eType = pContext->GetUseWinType();
	if(eType == USE_LEFT_WIN)
	{
		m_pLeftNextFrame->DrawFrame(dc);
		m_pLeftDisplayFrame->DrawFrame(dc);
	}
	else
	{
		m_pRightNextFrame->DrawFrame(dc);
		m_pRightDisplayFrame->DrawFrame(dc);
	}
	m_pSingleScoreFrame->DrawFrame(dc);
}
void CTsMainDisplayPane::_DrawMultiFrame(CDCHandle dc)
{
	m_pLeftNextFrame->DrawFrame(dc);
	m_pLeftDisplayFrame->DrawFrame(dc);
	m_pRightNextFrame->DrawFrame(dc);
	m_pRightDisplayFrame->DrawFrame(dc);
	m_pMultiScoreFrame->DrawFrame(dc);
}
LRESULT CTsMainDisplayPane::OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CPaintDC paintDC(m_hWnd);
	CHYMemDC memDC(paintDC);
	CDCHandle dc(memDC);

	CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
	_ASSERTE(pBaseInfo != NULL);
	CTsGameBaseInfo& ti = *pBaseInfo;
	TS_DISPLAY_MODE eMode = ti.GetDisplayMode();
	switch(eMode)
	{
	case DISPLAY_NONE:
		_DrawNoneBkgnd(dc);
		break;

	case DISPLAY_SINGLE:
		_DrawSingleBkgnd(dc);
		_DrawSingleFrame(dc);
		break;

	case DISPLAY_MULTI:
		_DrawMultiBkgnd(dc);
		_DrawMultiFrame(dc);
		break;
	
	default:
		_ASSERTE(FALSE);
		break;
	}

	return S_OK;
}

LRESULT CTsMainDisplayPane::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return TRUE;
}

LRESULT CTsMainDisplayPane::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	BOOL bRet = _NewObj();
	_ASSERTE(bRet);
	
	if(bRet)
	{
		bRet = 	_CreateObj();
		_ASSERTE(bRet);
	}

	if(bRet)
	{
		_CheckDisplayStatus();
		return S_OK;
	}
	else
	{
		return -1;
	}
}
LRESULT CTsMainDisplayPane::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;

	return S_OK;
}
LRESULT CTsMainDisplayPane::OnSize(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	if(wParam != SIZE_MINIMIZED)
	{
		CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
		_ASSERTE(pBaseInfo != NULL);
		CTsGameBaseInfo& ti = *pBaseInfo;
		
		TS_DISPLAY_MODE eMode = ti.GetDisplayMode();
		switch(eMode)
		{
		case DISPLAY_NONE:
			break;

		case DISPLAY_SINGLE:
			_AdjustSingle();
			break;

		case DISPLAY_MULTI:
			_AdjustMulti();
			break;
		
		default:
			_ASSERTE(FALSE);
			break;
		}
	}

	return S_OK;
}