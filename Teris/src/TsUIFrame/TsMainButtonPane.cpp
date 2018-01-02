#pragma  once 

#include "TsMainButtonPane.h"
#include "TsResMgr.h"
#include "TerisRes.h"
#include "TsDefine.h"
#include "TsUIInstanceApi.h"
#include "TsGameMainWnd.h"

VOID CTsMainButtonPane::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == TS_WPARAM_GAME_MODE_CHANGED)
	{
		_CheckButtonStatus();
	}
	else if(wParam == TS_WPARAM_GAME_STATUS_CHANGED)
	{
	//	_CheckGameStatus();
	}
}

LRESULT CTsMainButtonPane::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	_LoadResouce();
	_CreateButtons();
	_CheckButtonStatus();
	_CheckGameStatus();
	return S_OK;
}

LRESULT CTsMainButtonPane::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	if(!m_imgBtn.IsNull())
	{
		m_imgBtn.Destroy();
	}
	if(!m_bmpBtnBk.IsNull())
	{
		m_bmpBtnBk.DeleteObject();
	}

	return S_OK;
}

LRESULT CTsMainButtonPane::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return TRUE;
}

LRESULT CTsMainButtonPane::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	CPaintDC dc(m_hWnd);
	CRect rtClient(0, 0, 0, 0);
	GetClientRect(&rtClient);
	dc.FillSolidRect(&rtClient, GetSysColor(COLOR_WINDOW));

	return S_OK;
}

LRESULT CTsMainButtonPane::OnSize(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	if(wParam != SIZE_MINIMIZED)
	{
		_AdjustSize();
	}

	return S_OK;
}
LRESULT CTsMainButtonPane::OnCommand(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CTsGameMainWnd* pMainWnd = TsGetGameMainWnd();
	_ASSERTE(pMainWnd != NULL && pMainWnd->IsWindow());
	pMainWnd->SendMessage(nMsgType, wParam, lParam);

	return S_OK;
}
void CTsMainButtonPane::_AdjustSize()
{
	_ASSERTE(m_btnSingle.IsWindow() 
		&& m_btnNet.IsWindow() 
		&& m_btnRecordPlay.IsWindow() 
		&& m_btnSetup.IsWindow());

	CRect rtSingle(0, 0, 0, 0), rtNet(0, 0, 0, 0), rtRecPlay(0, 0, 0, 0), rtSetup(0, 0,0, 0);
	m_btnSingle.GetClientRect(&rtSingle);
	m_btnNet.GetClientRect(&rtNet);
	m_btnRecordPlay.GetClientRect(&rtRecPlay);
	m_btnSetup.GetClientRect(&rtSetup);
	
	CRect rtClient;
	GetClientRect(&rtClient);

	const int Left_margin = 20;
	const int Right_margin = 20;
	const int Top_margin = 10;
	int nStartX(Left_margin);
	int nTotalX = rtSingle.Width() + rtNet.Width() + rtRecPlay.Width() + rtSetup.Width();
	int nGap = (rtClient.Width() - nTotalX - Left_margin  - Right_margin) / 3;
	if(nGap <= 0)
	{
		nGap = (rtClient.Width() - nTotalX) / 3;
		nStartX = 0;
		if(nGap <= 0)
		{
			nGap = 0;
		}
	}
	
	::SetWindowPos(m_btnSingle, NULL, nStartX, Top_margin, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE);
	nStartX += (nGap + rtSingle.Width());
	::SetWindowPos(m_btnNet, NULL, nStartX, Top_margin, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE);
	nStartX += (nGap + rtNet.Width());
	::SetWindowPos(m_btnRecordPlay, NULL, nStartX, Top_margin, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE);
	nStartX += (nGap + rtRecPlay.Width());
	::SetWindowPos(m_btnSetup, NULL, nStartX, Top_margin, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE);
}
void CTsMainButtonPane::_LoadResouce()
{
	CTsResMgr* resMgr = TsGetResMgr();
	_ASSERTE(resMgr != NULL);
	if(m_imgBtn.IsNull())
		m_imgBtn = resMgr->LoadImageList(IDB_MAIN_BTN_IMG, 5);
	if(m_bmpBtnBk.IsNull())
		m_bmpBtnBk = resMgr->LoadBitmap(IDB_MAIN_BTN_BK);

}

void CTsMainButtonPane::_CreateButtons()
{
	const int bufferLen = 63;
	TCHAR szBuffer[bufferLen + 1] = {0};

	CTsResMgr* resMgr = TsGetResMgr();
	_ASSERTE(resMgr != NULL);
	m_btnSingle.SetBkgndBitmap(m_bmpBtnBk);
	m_btnSingle.SetBtnImagelist(m_imgBtn);
	m_btnSingle.SetBtnStyle(TS_BTN_RGN_STYLE| TS_BTN_CENTER_TEXT_STYLE);
	resMgr->LoadString(IDS_SINGLE_BTN_TEXT, szBuffer, bufferLen);
	m_btnSingle.Create(m_hWnd, CRect(0, 0, 0, 0), szBuffer, 0, 0, TS_SINGLE_BTN_ID);

	m_btnNet.SetBkgndBitmap(m_bmpBtnBk);
	m_btnNet.SetBtnImagelist(m_imgBtn);
	m_btnNet.SetBtnStyle(TS_BTN_RGN_STYLE | TS_BTN_CENTER_TEXT_STYLE);
	resMgr->LoadString(IDS_NET_BTN_TEXT, szBuffer, bufferLen);
	m_btnNet.Create(m_hWnd, CRect(0, 0, 0, 0), szBuffer, 0, 0, TS_NET_BTN_ID);

	m_btnRecordPlay.SetBkgndBitmap(m_bmpBtnBk);
	m_btnRecordPlay.SetBtnImagelist(m_imgBtn);
	m_btnRecordPlay.SetBtnStyle(TS_BTN_RGN_STYLE | TS_BTN_CENTER_TEXT_STYLE);
	resMgr->LoadString(IDS_RECORD_PLAY_BTN_TEXT, szBuffer, bufferLen);
	m_btnRecordPlay.Create(m_hWnd, CRect(0, 0, 0, 0), szBuffer, 0, 0, TS_REC_PLAY_BTN_ID);

	m_btnSetup.SetBkgndBitmap(m_bmpBtnBk);
	m_btnSetup.SetBtnImagelist(m_imgBtn);
	m_btnSetup.SetBtnStyle(TS_BTN_RGN_STYLE | TS_BTN_CENTER_TEXT_STYLE);
	resMgr->LoadString(IDS_SETUP_BTN_TEXT, szBuffer, bufferLen);
	m_btnSetup.Create(m_hWnd, CRect(0, 0, 0, 0), szBuffer, 0, 0, TS_SETUP_BTN_ID);
}

void CTsMainButtonPane::_CheckGameStatus()
{
	CTsGameBaseInfo* pGameInfo = TsGetGameBaseInfo();
	_ASSERTE(pGameInfo != NULL);

	BOOL bEnabeSetup(TRUE);
	if(pGameInfo->GetGameStatus() != IDLE_STATUS)
	{
		bEnabeSetup = FALSE;
	}
	CButton btn = GetDlgItem(TS_SETUP_BTN_ID);
	_ASSERTE(btn.IsWindow());
	btn.EnableWindow(bEnabeSetup);
}

void CTsMainButtonPane::_CheckButtonStatus()
{
	_ASSERTE(m_btnSingle.IsWindow() 
		&& m_btnNet.IsWindow() 
		&& m_btnRecordPlay.IsWindow() 
		&& m_btnSetup.IsWindow());

	m_btnSingle.SetCheck(FALSE);
	m_btnNet.SetCheck(FALSE);
	m_btnRecordPlay.SetCheck(FALSE);
	m_btnSetup.SetCheck(FALSE);

	CTsGameBaseInfo* pGameInfo = TsGetGameBaseInfo();
	_ASSERTE(pGameInfo != NULL);
	switch(pGameInfo->GetPlayMode())
	{
	case SINGLE_PLAYER_MODE:
	case TWO_PLAYER_MODE:
	case COMPUTER_PLAYER_MODE:
		m_btnSingle.SetCheck(TRUE);
		break;
	
	case NET_SELECT_MODE:
	case NET_PLAY_MODE:
		m_btnNet.SetCheck(TRUE);
		break;
	
	case RECORD_SELECT_MODE:
	case RECORD_PLAY_MODE:
		m_btnRecordPlay.SetCheck(TRUE);
		break;

	case NONE_MODE:
		break;
	}
}