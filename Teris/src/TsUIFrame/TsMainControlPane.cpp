#include "TsMainControlPane.h"
#include "TsWinFrame.h"
#include "TsDrawBase.h"
#include "TsResMgr.h"
#include "TsUIInstanceApi.h"
#include "TsGameMainWnd.h"


#define CONTROL_WIN_GAP 2 

CTsSubControlContainer::CTsSubControlContainer()
{
	m_pFrame  = new CTsControlPaneFrame(NULL);
	_ASSERTE(m_pFrame != NULL);

	m_hWndView = NULL;
}

CTsSubControlContainer::~CTsSubControlContainer()
{
	if(m_pFrame != NULL)
	{
		delete m_pFrame;
		m_pFrame = NULL;
	}

	m_hWndView = NULL;
}

LRESULT CTsSubControlContainer::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	if(m_pFrame == NULL) return -1;

	return S_OK;
}

LRESULT CTsSubControlContainer::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	return S_OK;
}

void CTsSubControlContainer::SetView(HWND hWnd)
{
	_ASSERTE(hWnd != NULL && ::IsWindow(hWnd));
	m_hWndView = hWnd;

	if(IsWindow())
	{
		_AdjustView();
	}

}
void CTsSubControlContainer::SetTitleIcon(HBITMAP bmp)
{
	if(m_pFrame != NULL)
	{
		m_pFrame->SetIcon(bmp);
	}
	else
	{
		_ASSERTE(FALSE);
	}
}
void CTsSubControlContainer::SetTitleText(LPCTSTR lpszTitle)
{
	if(m_pFrame != NULL)
	{
		m_pFrame->SetTitleText(lpszTitle);
	}
	else
	{
		_ASSERTE(FALSE);
	}
}
LRESULT CTsSubControlContainer::OnSize(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	if(wParam != SIZE_MINIMIZED)
	{
		_AdjustView();
	}

	return S_OK;
}
void CTsSubControlContainer::_AdjustView()
{
	if(m_pFrame != NULL)
	{
		CRect rtClient;
		GetClientRect(&rtClient);
		m_pFrame->SetFrameRect(rtClient);

		CRect rtContent = m_pFrame->GetContentRect();
		if(m_hWndView != NULL && ::IsWindow(m_hWndView))
		{
			::MoveWindow(m_hWndView, rtContent.left, rtContent.top, rtContent.Width(), rtContent.Height(), TRUE);
		}
	}
}
LRESULT CTsSubControlContainer::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	CPaintDC paintDC(m_hWnd);
	CHYMemDC memDC(paintDC);
	CDCHandle dc(memDC);
	
	CRect rtClient;
	GetClientRect(&rtClient);
	dc.FillSolidRect(&rtClient, GetSysColor(COLOR_WINDOW));

	if(m_pFrame != NULL)
	{
		m_pFrame->DrawFrame(dc);
	}

	return S_OK;
}

LRESULT CTsSubControlContainer::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&)
{
	return TRUE;
}


////////////////////////////  CTsControlTypeDlg  ////////////////////////
LRESULT CTsControlTypeDlg::OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	_CheckTypeMode();

	return S_OK;
}
LRESULT CTsControlTypeDlg::OnSize(UINT, WPARAM wParam, LPARAM, BOOL& )
{
	if(wParam != SIZE_MINIMIZED)
	{
		_AdjustSize();
	}
	return S_OK;
}
void CTsControlTypeDlg::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == TS_WPARAM_GAME_MODE_CHANGED)
	{
		_CheckTypeMode();
	}
}
LRESULT CTsControlTypeDlg::OnCommand(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CTsGameMainWnd* pMainWnd = TsGetGameMainWnd();
	if(pMainWnd != NULL)
	{
		return pMainWnd->SendMessage(WM_COMMAND, wParam, lParam);
	}

	return S_OK;
}
void CTsControlTypeDlg::_CheckTypeMode()
{
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);

	CButton btnCheck;
	btnCheck =  GetDlgItem(IDC_PLAY_SINGLE);
	btnCheck.SetCheck(FALSE);
	btnCheck =  GetDlgItem(IDC_PLAY_TWO);
	btnCheck.SetCheck(FALSE);
	btnCheck =  GetDlgItem(IDC_PLAY_COMPUTER);
	btnCheck.SetCheck(FALSE);

	btnCheck.m_hWnd = NULL;
	switch(pInfo->GetPlayMode())
	{
	case SINGLE_PLAYER_MODE:
		btnCheck = GetDlgItem(IDC_PLAY_SINGLE);
		break;

	case TWO_PLAYER_MODE:
		btnCheck = GetDlgItem(IDC_PLAY_TWO);
		break;

	case COMPUTER_PLAYER_MODE:
		btnCheck = GetDlgItem(IDC_PLAY_COMPUTER);
		break;

	default:
		break;
	}

	if(btnCheck.m_hWnd != NULL && btnCheck.IsWindow())
	{
		btnCheck.SetCheck(TRUE);
	}
}
void CTsControlTypeDlg::_AdjustSize()
{
	CWindow win1 = GetDlgItem(IDC_PLAY_SINGLE);
	CWindow win2 = GetDlgItem(IDC_PLAY_COMPUTER);
	CWindow win3 = GetDlgItem(IDC_PLAY_TWO);
	
	const int nGap = 10;
	INT nStartX(0), nStartY(nGap);
	CRect rtClient;
	GetClientRect(&rtClient);

	CRect rtSub;
	win1.GetClientRect(&rtSub);
	if(rtSub.Width() < rtClient.Width())
	{
		nStartX = ( rtClient.Width() - rtSub.Width() ) / 2;
	}
	win1.SetWindowPos(NULL, nStartX, nStartY, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
	nStartY += rtSub.Height();
	nStartY += nGap;

	win2.SetWindowPos(NULL, nStartX, nStartY, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
	nStartY += rtSub.Height();
	nStartY += nGap;

	win3.SetWindowPos(NULL, nStartX, nStartY, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
}

CSize CTsControlTypeDlg::GetIdealSize()
{
	return CSize(100, 120);
}
//////////////////////////  CTsControlOptionDlg  ////////////////////
LRESULT CTsControlOptionDlg::OnSize(UINT, WPARAM wParam, LPARAM, BOOL& )
{
	if(wParam != SIZE_MINIMIZED)
	{
		_AdjustSize();
	}
	return S_OK;
}

void CTsControlOptionDlg::_AdjustSize()
{
	CWindow win1 = GetDlgItem(IDC_STATIC_LEVEL);
	CWindow win4 = GetDlgItem(IDC_LEVEL);

	CWindow win5 = GetDlgItem(IDC_STATIC_START_LINE);
	CWindow win2 = GetDlgItem(IDC_START_LINE);

	CWindow win3 = GetDlgItem(IDC_RECORD);
	
	const int nGapHeight = 13;
	const int nGapWidth = 10;
	INT nStartX(0), nStartY(nGapHeight);
	CRect rtClient;
	GetClientRect(&rtClient);

	CRect rtSub1;
	win1.GetClientRect(&rtSub1);
	CRect rtSub2;
	win4.GetClientRect(&rtSub2);
	if(rtSub1.Width() + rtSub2.Width() + nGapWidth < rtClient.Width())
	{
		nStartX = (rtClient.Width() - rtSub1.Width() - rtSub2.Width() - nGapWidth) / 2;
	}


	win1.SetWindowPos(NULL, nStartX, nStartY, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
	win4.SetWindowPos(NULL, nStartX + rtSub1.Width() + nGapWidth, nStartY, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
	nStartY += rtSub1.Height();
	nStartY += nGapHeight;

	win5.SetWindowPos(NULL, nStartX, nStartY, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
	win2.SetWindowPos(NULL, nStartX + rtSub1.Width() + nGapWidth, nStartY, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
	
	nStartY += rtSub1.Height() + 5;
	nStartY += nGapHeight;

	win3.SetWindowPos(NULL, nStartX, nStartY, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
}

CSize CTsControlOptionDlg::GetIdealSize()
{
	return CSize(100, 130);
}
LRESULT CTsControlOptionDlg::OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
{
	_LoadData();
	_CheckGameOption(0, TRUE);

	return S_OK;
}
LRESULT CTsControlOptionDlg::OnLevelChanged(WORD, WORD, HWND, BOOL&)
{
	CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
	_ASSERTE(pBaseInfo != NULL);

	CComboBox box = GetDlgItem(IDC_LEVEL);
	INT nIndex = box.GetCurSel();
	pBaseInfo->SetHardLevel((TS_HARD_LEVEL)nIndex);

	return S_OK;
}
LRESULT CTsControlOptionDlg::OnStartLineChanged(WORD, WORD, HWND, BOOL&)
{
	CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
	_ASSERTE(pBaseInfo != NULL);

	CComboBox box = GetDlgItem(IDC_START_LINE);
	INT nIndex = box.GetCurSel();
	pBaseInfo->SetStartLineCount((TS_START_LINE)nIndex);

	return S_OK;
}
LRESULT CTsControlOptionDlg::OnRecordOption(WORD, WORD, HWND, BOOL&)
{
	CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
	_ASSERTE(pBaseInfo != NULL);
	
	CButton btn = GetDlgItem(IDC_RECORD);
	BOOL bCheck = btn.GetCheck();

	pBaseInfo->SetRecordOption(bCheck);

	return S_OK;
}
void CTsControlOptionDlg::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == TS_WPARAM_HARD_LEVEL_CHANGED
		|| wParam == TS_WPARAM_RECORD_OPTION_CHANGED
		|| wParam == TS_WPARAM_START_LINE_CHANGED)
	{
		_CheckGameOption(wParam);
	}
}
void CTsControlOptionDlg::_CheckGameOption(WPARAM wType, BOOL bAll)
{
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);

	if(!bAll)
	{
		if(wType == TS_WPARAM_HARD_LEVEL_CHANGED)
		{
			TS_HARD_LEVEL level = pInfo->GetHardLevel();
			CComboBox box = GetDlgItem(IDC_LEVEL);
			box.SetCurSel((INT)level);
		}
		else if(wType == TS_WPARAM_RECORD_OPTION_CHANGED)
		{
			CButton btn = GetDlgItem(IDC_RECORD);
			btn.SetCheck(pInfo->GetRecordOption());
		}
		else if(wType == TS_WPARAM_START_LINE_CHANGED)
		{
			TS_START_LINE line = pInfo->GetStartLineCount();
			CComboBox box = GetDlgItem(IDC_START_LINE);
			box.SetCurSel(INT(line));
		}
	}
	else
	{
		TS_HARD_LEVEL level = pInfo->GetHardLevel();
		CComboBox box = GetDlgItem(IDC_LEVEL);
		box.SetCurSel((INT)level);
		CButton btn = GetDlgItem(IDC_RECORD);
		btn.SetCheck(pInfo->GetRecordOption());
		TS_START_LINE line = pInfo->GetStartLineCount();
		box = GetDlgItem(IDC_START_LINE);
		box.SetCurSel(INT(line));
	}
}
void CTsControlOptionDlg::_LoadData()
{
	CTsResMgr* pResMgr = TsGetResMgr();
	_ASSERTE(pResMgr != NULL);
	
	CComboBox box = GetDlgItem(IDC_LEVEL);
	const int nLen = 32;
	TCHAR szBuffer[nLen] = {0};
	pResMgr->LoadString(IDS_LEVEL_EASY, szBuffer, nLen);
	box.AddString(szBuffer);

	pResMgr->LoadString(IDS_LEVEL_MIDDLE, szBuffer, nLen);
	box.AddString(szBuffer);
	pResMgr->LoadString(IDS_LEVEL_HARD, szBuffer, nLen);
	box.AddString(szBuffer);	
	pResMgr->LoadString(IDS_LEVEL_CRAZY, szBuffer, nLen);
	box.AddString(szBuffer);
	
	box = GetDlgItem(IDC_START_LINE);
	for(int i=LINE_0; i <LINE_COUNT; ++i)
	{
		_ltot(i, szBuffer, 10);
		box.AddString(szBuffer);
	}
}

///////////////////////// CTsStartControlDlg  //////////////////////
CSize CTsStartControlDlg::GetIdealSize()
{
	return CSize(100, 120);
}
void CTsStartControlDlg::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == TS_WPARAM_GAME_STATUS_CHANGED)
	{
		_CheckGameStatus();
	}
}
void CTsStartControlDlg::_CheckGameStatus()
{
	const int nBufferLen = 64;
	TCHAR szBuffer1[nBufferLen] = {0};
	TCHAR szBuffer2[nBufferLen] = {0};

	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);
	
	CTsResMgr* pResMgr = TsGetResMgr();
	_ASSERTE(pResMgr != NULL);
	
	CButton btn1 = GetDlgItem(IDC_START_BUTTON);
	CButton btn2 = GetDlgItem(IDC_PAUSE_BUTTON);
	TS_GAME_STATUS eStatus = pInfo->GetGameStatus();
	if(eStatus == IDLE_STATUS)
	{
		pResMgr->LoadString(IDS_START, szBuffer1, nBufferLen);
		pResMgr->LoadString(IDS_PAUSE, szBuffer2, nBufferLen);

		btn1.SetWindowText(szBuffer1);
		btn2.SetWindowText(szBuffer2);

		btn1.EnableWindow(TRUE);
		btn2.EnableWindow(FALSE);
		
	}
	else if(eStatus == RUNNING_STATUS)
	{
		pResMgr->LoadString(IDS_END, szBuffer1, nBufferLen);
		pResMgr->LoadString(IDS_PAUSE, szBuffer2, nBufferLen);

		btn1.SetWindowText(szBuffer1);
		btn2.SetWindowText(szBuffer2);

		btn1.EnableWindow(TRUE);
		btn2.EnableWindow(TRUE);		
	}
	else if(eStatus == PAUSE_STATUS)
	{
		pResMgr->LoadString(IDS_END, szBuffer1, nBufferLen);
		pResMgr->LoadString(IDS_RESUME, szBuffer2, nBufferLen);

		btn1.SetWindowText(szBuffer1);
		btn2.SetWindowText(szBuffer2);

		btn1.EnableWindow(TRUE);
		btn2.EnableWindow(TRUE);
	}
	else
	{
		_ASSERTE(FALSE);
	}
}
LRESULT CTsStartControlDlg::OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
{
	_CheckGameStatus();

	return S_OK;
}
LRESULT CTsStartControlDlg::OnCommand(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CTsGameMainWnd* pMainWnd = TsGetGameMainWnd();
	_ASSERTE(pMainWnd != NULL);

	pMainWnd->SendMessage(nMsgType, wParam, lParam);

	return S_OK;
}
LRESULT CTsStartControlDlg::OnSize(UINT, WPARAM wParam, LPARAM, BOOL& )
{
	if(wParam != SIZE_MINIMIZED)
	{
		_AdjustSize();
	}

	return S_OK;
}
void CTsStartControlDlg::_AdjustSize()
{
	CWindow winStart = GetDlgItem(IDC_START_BUTTON);
	CWindow winPause = GetDlgItem(IDC_PAUSE_BUTTON);
	
	const int nGap = 20;
	INT nStartX(0), nStartY(nGap);
	CRect rtClient;
	GetClientRect(&rtClient);

	CRect rtSub;
	winStart.GetClientRect(&rtSub);
	if(rtClient.Width() > rtSub.Width())
	{
		nStartX = (rtClient.Width() - rtSub.Width()) / 2;
	}
	winStart.MoveWindow(nStartX, nStartY, rtSub.Width(), rtSub.Height(), TRUE);
	nStartY += rtSub.Height();
	nStartY += nGap;

	winPause.MoveWindow(nStartX, nStartY, rtSub.Width(), rtSub.Height(), TRUE);
}
///////////////////////// CTsUserListCtrl  //////////////////////

CSize CTsUserListCtrl::GetMinSize()
{
	return CSize(100, 80);
}
void CTsUserListCtrl::_LoadUserData()
{
	DeleteAllItems();
	
	CTsUserListData* pUserList = TsGetUserList();
	_ASSERTE(pUserList != NULL);

	if(pUserList == NULL) return;

	int nCount = pUserList->GetUserCount();
	for(int i=0; i<nCount; ++i)
	{
		CTsUserItem* pUser = pUserList->GetUserByIndex(i);
		_ASSERTE(pUser != NULL);

		AddItem(i, 0, _T(""), pUser->m_eUserType);
		AddItem(i, 1, pUser->m_szUserName);
		AddItem(i, 2, m_arType[pUser->m_eUserType]);
	}
}
LRESULT CTsUserListCtrl::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled  = FALSE;
	
	_LoadResource();
	_CreateColumnHeader();
	_LoadUserData();

	return S_OK;
}
void CTsUserListCtrl::_CreateColumnHeader()
{
	LVCOLUMN lvc = { 0 };
	lvc.mask =  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ;
	lvc.fmt = LVCFMT_LEFT;

	int nIndex(0);
	lvc.pszText = (LPTSTR)_T("");
	lvc.cx = 20;
	InsertColumn(nIndex++, &lvc);

	DWORD dwExStyle = GetExtendedListViewStyle();
	dwExStyle |=  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES;
	SetExtendedListViewStyle(dwExStyle);

	CTsResMgr* pResMgr = TsGetResMgr();
	_ASSERTE(pResMgr != NULL);
	const int nLen = 32;
	TCHAR szBuffer[nLen] = {0};
	pResMgr->LoadString(IDS_USER_COLUMN_NAME, szBuffer, nLen);

	lvc.pszText = szBuffer;
	lvc.cx = 100;
	InsertColumn(nIndex++,&lvc);

	pResMgr->LoadString(IDS_USER_COLUMN_TYPE, szBuffer, nLen);
	lvc.pszText = szBuffer;
	lvc.cx = 100;
	InsertColumn(nIndex++,&lvc);

	SetImageList(m_imgIcon, LVSIL_SMALL);
}

void CTsUserListCtrl::_LoadResource()
{
	CTsResMgr* pResMgr = TsGetResMgr(); 
	_ASSERTE(pResMgr != NULL);

	if(m_imgIcon.IsNull())
	{
		m_imgIcon = pResMgr	->LoadImageList(IDB_USER_ICON, 4);
	}

	const int nLen = sizeof(m_arType[0]) / sizeof(TCHAR);
	pResMgr->LoadString(IDS_USER_TYPE_COMPUTER, m_arType[0], nLen);
	pResMgr->LoadString(IDS_USER_TYPE_LOCAL_CONTROL, m_arType[1], nLen);
	pResMgr->LoadString(IDS_USER_TYPE_NET_CONTROL, m_arType[2], nLen);
	pResMgr->LoadString(IDS_USER_TYPE_NET_OBSERVER, m_arType[3], nLen);
}
LRESULT CTsUserListCtrl::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	if(!m_imgIcon.IsNull())
	{
		m_imgIcon.Destroy();
	}

	return S_OK;
}

void CTsUserListCtrl::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == TS_WPARAM_USER_CHANGED)
	{
		_LoadUserData();
	}
}

////////////////////////  CTsSingleControlPane  ///////////////////
CTsSingleControlPane::CTsSingleControlPane()
{
	m_szType[0] = 0;
	m_szOption[0] = 0;
	m_szUserlist[0] = 0;
}
BOOL CTsSingleControlPane::_CreateSubWindow()
{
	CTsResMgr* pResMgr = TsGetResMgr();
	_ASSERTE(pResMgr != NULL);
	if(pResMgr == NULL) return FALSE;

	//type container
	m_typeContainer.Create(m_hWnd, CRect(0, 0, 0, 0));
	m_typeDlg.Create(m_typeContainer);
	m_typeDlg.ShowWindow(SW_SHOW);

	m_typeContainer.SetView(m_typeDlg);
	if(m_typeIcon.IsNull())
	{
		m_typeIcon = pResMgr->LoadBitmap(IDB_GAME_TYPE);
		_ASSERTE(!m_typeIcon.IsNull());
	}
	pResMgr->LoadString(IDS_SINGLE_TYPE, m_szType, sizeof(m_szType) / sizeof(TCHAR));
	m_typeContainer.SetTitleIcon(m_typeIcon);
	m_typeContainer.SetTitleText(m_szType);

	//option container
	m_optionContainer.Create(m_hWnd, CRect(0, 0, 0, 0));
	m_optionDlg.Create(m_optionContainer);
	m_optionDlg.ShowWindow(SW_SHOW);

	
	m_optionContainer.SetView(m_optionDlg);
	if(m_optionIcon.IsNull())
	{
		m_optionIcon = pResMgr->LoadBitmap(IDB_GAME_TYPE);
	}
	pResMgr->LoadString(IDS_GAME_OPTION, m_szOption, sizeof(m_szOption) / sizeof(TCHAR));
	m_optionContainer.SetTitleIcon(m_optionIcon);
	m_optionContainer.SetTitleText(m_szOption);

	//user list
	m_userlistContainer.Create(m_hWnd, CRect(0, 0, 0, 0));
	m_userlist.Create(m_userlistContainer, CRect(0, 0, 0, 0));
	m_userlistContainer.SetView(m_userlist);
	if(m_userlistIcon.IsNull())
	{
		m_userlistIcon = pResMgr->LoadBitmap(IDB_GAME_TYPE);
		_ASSERTE(!m_userlistIcon.IsNull());
	}
	pResMgr->LoadString(IDS_USER_LIST, m_szUserlist, sizeof(m_szUserlist) / sizeof(TCHAR));
	m_userlistContainer.SetTitleIcon(m_userlistIcon);
	m_userlistContainer.SetTitleText(m_szUserlist);

	//Start control
	m_startControlDlg.Create(m_hWnd);
	m_startControlDlg.ShowWindow(SW_SHOW);

	return TRUE;

}
void CTsSingleControlPane::_DestroySubWindow()
{
	m_typeDlg.DestroyWindow();
	m_typeContainer.DestroyWindow();

	m_optionDlg.DestroyWindow();
	m_optionContainer.DestroyWindow();

	m_userlistContainer.DestroyWindow();

	m_startControlDlg.DestroyWindow();
}
void CTsSingleControlPane::_AdjustSize()
{
	CRect rtClient;
	GetClientRect(&rtClient);
	
	INT nStartY(0);
	CSize szType = m_typeDlg.GetIdealSize();
	m_typeContainer.MoveWindow(0, nStartY, rtClient.Width(), szType.cy , TRUE);
	nStartY += szType.cy;
	nStartY += CONTROL_WIN_GAP;

	szType = m_optionDlg.GetIdealSize();
	CSize szUserList = m_userlist.GetMinSize();
	CSize szStartControl = m_startControlDlg.GetIdealSize();
	INT nMinHeight = szUserList.cy + CONTROL_WIN_GAP * 2 + szStartControl.cy + szType.cy;
	INT nLeftHeight = rtClient.Height() - nStartY;
	if(nMinHeight < nLeftHeight)
	{
		szUserList.cy += (nLeftHeight - nMinHeight);
	}

	m_userlistContainer.MoveWindow(0, nStartY, rtClient.Width(), szUserList.cy, TRUE);
	nStartY += szUserList.cy;
	nStartY += CONTROL_WIN_GAP;	

	m_optionContainer.MoveWindow(0, nStartY, rtClient.Width(), szType.cy , TRUE);
	nStartY += szType.cy;
	nStartY += CONTROL_WIN_GAP;


	m_startControlDlg.MoveWindow(0, nStartY, rtClient.Width(), szStartControl.cy, TRUE);

}
LRESULT CTsSingleControlPane::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	if(!_CreateSubWindow())
	{
		return -1;
	}

	return S_OK;
}
LRESULT CTsSingleControlPane::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	_DestroySubWindow();

	return S_OK;
}

LRESULT CTsSingleControlPane::OnSize(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	if(wParam != SIZE_MINIMIZED)
	{
		_AdjustSize();	
	}

	return S_OK;
}

LRESULT CTsSingleControlPane::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	CPaintDC dc(m_hWnd);
	CRect rtClient;
	GetClientRect(&rtClient);

	dc.FillSolidRect(rtClient, RGB(78, 153, 77));

	return S_OK;
}
LRESULT CTsSingleControlPane::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&)
{
	return S_OK;
}
///////////////////////////////// CTsMainControlPane /////////////////////
BOOL CTsMainControlPane::PreTranslateMessage(MSG* pMsg)
{
	BOOL bRet(FALSE);
	
	return bRet;
}
LRESULT CTsMainControlPane::OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CPaintDC dc(m_hWnd);
	
	CRect rtClient;
	GetClientRect(&rtClient);
	
	dc.FillSolidRect(&rtClient, RGB(204, 233, 128));

	LPCTSTR lpszText = _T("Control Pane");
	dc.DrawText(lpszText, lstrlen(lpszText), rtClient, DT_CENTER | DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE);

	return S_OK;
}

LRESULT CTsMainControlPane::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return TRUE;
}
LRESULT CTsMainControlPane::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	m_singleControlPane.Create(m_hWnd, CRect(0, 0, 0, 0));

	return S_OK;
}

LRESULT CTsMainControlPane::OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CRect rtClient;
	GetClientRect(&rtClient);

	m_singleControlPane.SetWindowPos(NULL, 0, 0, rtClient.Width(), rtClient.Height(), SWP_NOACTIVATE);

	return S_OK;
}

LRESULT CTsMainControlPane::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return S_OK;
}