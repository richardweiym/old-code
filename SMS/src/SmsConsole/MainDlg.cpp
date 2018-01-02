#include "StdAfx.h"
#include "maindlg.h"
#include "../SmsServer/smsmessage.h"
#include "../SmsServer/SmsAccountManager.h"
#include "SetupDlg.h"
#include "../SmsServer/SmsDeviceExport.h"
#include "../SmsServer/SmsCommunicateBase.h"
#include "DeviceDlg.h"

HINSTANCE CIEButton::m_hResModule = NULL;

CMainDlg::CMainDlg()
{
	m_hResModule = NULL;
	m_hServiceAgent = NULL;

// 	m_hRunningIcon = NULL;
// 	m_hStartingIcon = NULL;
// 	m_hStoppedIcon = NULL;
//	m_pAccountManager = new CSmsAccountManager();
	m_pSettingDlg = new CSetupDlg(this);
	ResetValues();
	m_bExit = FALSE;
	m_pConsoleTray = new CConsoleTray(this);
}
CMainDlg::~CMainDlg()
{
// 	if(m_pAccountManager != NULL)
// 	{
// 		delete m_pAccountManager;
// 		m_pAccountManager = NULL;
// 	}
	if(m_pSettingDlg != NULL)
	{
		delete m_pSettingDlg;
		m_pSettingDlg = NULL;
	}
	if(m_pConsoleTray != NULL)
	{
		delete m_pConsoleTray;
		m_pConsoleTray = NULL;
	}
}
VOID CMainDlg::ResetValues()
{
	m_hServiceAgent = NULL;
	m_nDispatchedCount = 0;
	m_nSendedCount = 0;
	m_nConCount = 0;
	m_nSignalCount = 0;
	m_eCurrentStatus = UI_STATUS_STOPPED;
	m_szErrorMsg[0] = 0;
}
LRESULT CMainDlg::OnTimer(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_hServiceAgent != NULL)
	{
		if(!(::IsWindow(m_hServiceAgent)))
		{
			ResetValues();
			SetUIStatus(UI_STATUS_STOPPED);
			KillTimer(1);
		}
	}
	else
	{
		KillTimer(1);
	}

	return 0;
}
LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();
	m_pConsoleTray->AddTray();
	m_hResModule =(HINSTANCE) GetModuleHandle(NULL);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);


	
	HICON hIcon = (HICON)::LoadImage(m_hResModule, MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(m_hResModule, MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);
	
	KillAgentExist();

	//load
// 	m_hStoppedIcon = ::LoadIcon(m_hResModule, MAKEINTRESOURCE(IDI_STOPPED));
// 	m_hStartingIcon = ::LoadIcon(m_hResModule, MAKEINTRESOURCE(IDI_STARTING));
// 	m_hRunningIcon = ::LoadIcon(m_hResModule, MAKEINTRESOURCE(IDI_RUNNING));

	//subclass  button
	CIEButton::SetResModule(m_hResModule);
	m_btnStart.SetBitmap(IDB_START);
	m_btnStop.SetBitmap(IDB_STOP);
	m_btnSetting.SetBitmap(IDB_SETTING);
 	m_btnStart.SubclassWindow(GetDlgItem(IDC_START));
	m_btnStop.SubclassWindow(GetDlgItem(IDC_STOP));
	m_btnSetting.SubclassWindow(GetDlgItem(IDC_SETTING));
	
	//create statusbar
	INT arStatus[] = {ID_PANE_STATUS, ID_PANE_COUNT};
	m_statusbar.Create(m_hWnd, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	m_statusbar.SetPanes(arStatus,2);
 	SetUIStatus(UI_STATUS_STOPPED);
	

//	m_pAccountManager ->LoadAccounts();
	ResetCaption();

	return TRUE;
}
VOID CMainDlg::ResetCaption()
{
	TCHAR szTitle[256] = {0};
	::LoadString(m_hResModule, IDS_MAIN_DLG_TITLE, szTitle, 256);
	SetWindowText(szTitle);

	if(!CheckIsRegistered())
	{
		lstrcat(szTitle, _T("(试用版)"));	
	}

	SetWindowText(szTitle);
}
VOID CMainDlg::PaintCurrentInformation(CPaintDC& dc)
{
	CRect rect;
	TCHAR szBuf[MAX_PATH] = {0};
	TCHAR szTemp[MAX_PATH] = {0};
	static INT xChar(0), yChar(0);
	
	//获取字体大小
	if((xChar == 0) || (yChar == 0))
	{
		TEXTMETRIC tm;
		GetTextMetrics(dc, &tm);
		xChar = tm.tmAveCharWidth ;
		xChar = (tm.tmPitchAndFamily & 1 ? 3 : 2) * xChar / 2 ;
		yChar = tm.tmHeight + tm.tmExternalLeading ;

	}

	::GetWindowRect(GetDlgItem(IDC_STATICCONTAINER),&rect);
	ScreenToClient(&rect);
	dc.FillRect(&rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	dc.SetBkMode(TRANSPARENT);
	HPEN hNewPen = CreatePen(PS_SOLID,1,RGB(0,255,0));
	HBRUSH hOldBrush = dc.SelectBrush((HBRUSH)GetStockObject(BLACK_BRUSH));
	HPEN hOldPen = dc.SelectPen(hNewPen);
	dc.SetTextColor(RGB(0,255,0));

	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(lf),&lf);
	lf.lfWeight = 300;
	lf.lfHeight = -12;
	strcpy(lf.lfFaceName, "Tahoma");
	HFONT hNewFont = CreateFontIndirect(&lf);
	HFONT hOldFont = dc.SelectFont(hNewFont);

	INT nStartX(rect.left), nStartY(rect.top), nWidth(rect.Width()), nHeight(rect.bottom);
	INT nX(nStartX), nY(nStartY);
	INT i(0),j(0);
	INT nValue(0);
	CRect rtSignal;
	TCHAR szError[100] = {0};

	switch(m_eCurrentStatus)
	{
	case UI_STATUS_RUNNING:
		nX += xChar;
		nY += yChar / 2;
		LoadString(m_hResModule,IDS_SIGNAL_COUNT,szBuf,MAX_PATH);
		dc.TextOut(nX,nY,szBuf);

		rtSignal.left = 80;
		rtSignal.top = nY;
		rtSignal.right = 180;
		rtSignal.bottom = rtSignal.top + yChar;
		dc.Rectangle(&rtSignal);
		
		if(m_nSignalCount >= 1 && m_nSignalCount <= 31)
		{
			nValue = (INT)(m_nSignalCount * (100.0 / 31));
		}		
		else
		{
			nValue = 0;
		}

		j = 0;
		for(i = 0; i < nValue; i++)
		{
			j++;
			dc.MoveTo(CPoint(rtSignal.left + i,rtSignal.top + 3));
			dc.LineTo(CPoint(rtSignal.left + i,rtSignal.top + yChar -3));
			if(j == 3)
			{
				j=0;
				i+=1;
			}	
		}
		_sntprintf(szBuf, MAX_PATH, "%d / 100",nValue);
		dc.TextOut(rtSignal.right + 20,rtSignal.top, szBuf);


		nY += (yChar * 3 / 2);
		LoadString(m_hResModule, IDS_SENDED_COUNT, szTemp, MAX_PATH);
		_sntprintf(szBuf, MAX_PATH, szTemp,m_nSendedCount);
		dc.TextOut(nX, nY, szBuf);
		
		nY += (yChar * 3 / 2);
		LoadString(m_hResModule, IDS_DISPATCHED_COUNT,szTemp, MAX_PATH);
		_sntprintf(szBuf, MAX_PATH, szTemp, m_nDispatchedCount);
		dc.TextOut(nX, nY, szBuf);

		nY += (yChar * 3 / 2);
		LoadString(m_hResModule, IDS_CONNECT_INDICATION, szTemp, MAX_PATH);
		_sntprintf(szBuf, MAX_PATH, szTemp, m_nConCount);
		dc.TextOut(nX, nY, szBuf);

		break;
	case UI_STATUS_STARTING:
		LoadString(m_hResModule, IDS_STARTING_INFO,szError,100);
		nX += xChar;
		nY += yChar / 2;
		dc.TextOut(nX, nY, szError);

		break;
	case UI_STATUS_STOPPED:
		LoadString(m_hResModule, IDS_START_FAIL, szError, 100);
		nX += xChar;
		nY += yChar / 2;

		if(m_szErrorMsg[0] != 0)
		{
			dc.TextOut(nX, nY, szError);
			nY += (yChar * 3 / 2);
			dc.TextOut(nX,nY,m_szErrorMsg);
		}
		break;
	}
	
	dc.SelectFont(hOldFont);
	dc.SelectBrush(hOldBrush);
	dc.SelectPen(hOldPen);
	::DeleteObject(hNewPen);
	::DeleteObject(hNewFont);
}
LRESULT CMainDlg::OnPaint(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPaintDC dc(m_hWnd);
	PaintCurrentInformation(dc);
	return 0;
}

LRESULT CMainDlg::OnGetSignal(UINT uMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	 m_nSignalCount = (INT)(wParam);
	
	return 0;
}
LRESULT CMainDlg::OnGetConCount(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	if((INT)wParam != m_nConCount)
	{
		m_nConCount = wParam;
		InvalidateRect(NULL);
	}
	return 0;
}

LRESULT CMainDlg::OnSmsConnect(UINT, WPARAM wparam, LPARAM, BOOL&)
{
// 	INT nCount = (INT)wparam;
// 	if(nCount != m_nConCount)
// 	{
// 		m_nConCount = nCount;
// 		InvalidateRect(NULL);
// 	}
// 	m_nConCount++;
// 	InvalidateRect(NULL);
	return 0;
}
LRESULT CMainDlg::OnSmsDisconnect(UINT, WPARAM wparam, LPARAM, BOOL&)
{
// 	INT nCount = (INT)wparam;
// 	if(nCount != m_nConCount)
// 	{
// 		m_nConCount = nCount;
// 		InvalidateRect(NULL);
// 	}
// 	m_nConCount--;
// 	InvalidateRect(NULL);
	return 0;
}
// LRESULT CMainDlg::OnCopyData(UINT uMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
// {
// 	PCOPYDATASTRUCT pCs = (PCOPYDATASTRUCT)lParam;
// 	CSMSMessage msg(pCs);
// 	
// 	if(msg.GetMsgType() == REQUEST_LOGON)
// 	{
// 		if(m_pAccountManager->CheckCanLogon(&msg))
// 		{
// 			
// 		}
// 	}
// 	else if(msg.GetMsgType() == REQUEST_DISCONNECT)
// 	{
// 		m_pAccountManager->LogoutAccount(msg.GetClientID());
// 
// 		m_nConCount--;
// 
// 	}
// 	
// 	InvalidateRect(NULL);
// 	return 0;
// }

LRESULT CMainDlg::OnStartMessage(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if((INT)wParam==SMS_TYPE_SERVICE)
	{
		if((INT)lParam == SMS_OK)
		{
			m_hServiceAgent = FindAgentWnd();
			if(m_hServiceAgent != NULL)
			{
			//	m_pAccountManager->LoadAccounts();
				SetUIStatus(UI_STATUS_RUNNING);
				SetTimer(1,2000);
			}
			else
			{
				SetUIStatus(UI_STATUS_STOPPED);
			}
		}
		else
		{
			INT nError = (INT)lParam;
			TCHAR szBuf[MAX_PATH] = {0};
			TCHAR szTemp[MAX_PATH] = {0};
			if( nError > SMS_DEVICE_ERROR_BASE && nError < SMS_DEVICE_ERROR_BASE + 100)
			{
				::LoadString(m_hResModule, IDS_DEVICE_ERROR, szTemp, MAX_PATH);	
			}
			else if( nError > SMS_COMMUNICATE_ERROR_BASE && nError < SMS_COMMUNICATE_ERROR_BASE + 100)
			{
				::LoadString(m_hResModule, IDS_COMMU_ERROR, szTemp, MAX_PATH);
			}
			else
			{
				::LoadString(m_hResModule, IDS_OTHER_ERROR, szTemp, MAX_PATH);
			}
			_sntprintf(m_szErrorMsg, MAX_PATH, szTemp, nError);

			SetUIStatus(UI_STATUS_STOPPED);
			m_hServiceAgent = NULL;
		}
	}
	return 0;
}
LRESULT CMainDlg::OnStopMessage(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if((INT)wParam==SMS_TYPE_SERVICE)
	{
		m_szErrorMsg[0] = 0;
		SetUIStatus(UI_STATUS_STOPPED);
	}		
	return 0;
}
LRESULT CMainDlg::OnErrorMessage(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
LRESULT CMainDlg::OnStart(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	KillAgentExist();
	Start();
	return 0;
}

LRESULT CMainDlg::OnStop(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	
	Stop();
	return 0;
}
LRESULT CMainDlg::OnSetting(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	
	if(!(m_pSettingDlg->IsWindow()))
	{
		m_pSettingDlg->Create(m_hWnd);
	}
		
	m_pSettingDlg ->ShowWindow(SW_SHOW);
	return 0;
}
LRESULT CMainDlg::OnReceiveMessage(UINT nMsgType, WPARAM wParam, LPARAM lPram, BOOL& bHandled)
{
	m_nDispatchedCount++;
	Invalidate();
	return 0;
}
LRESULT CMainDlg::OnSendedMessage(UINT nMsgType, WPARAM wParam, LPARAM lPram, BOOL& bHandled)
{
	m_nSendedCount++;
	Invalidate();
	return 0;
}

LRESULT CMainDlg::OnMouseMove(UINT nMsgType,WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_btnSetting.GetIsMouseOn())
	{
		m_btnSetting.SetIsMouseOn(FALSE);
		m_btnSetting.Invalidate();
	}

	if(m_btnStart.GetIsMouseOn())
	{
		m_btnStart.SetIsMouseOn(FALSE);
		m_btnStart.Invalidate();
	}

	if(m_btnStop.GetIsMouseOn())
	{
		m_btnStop.SetIsMouseOn(FALSE);
		m_btnStop.Invalidate();
	}

	return 0;
}
LRESULT CMainDlg::OnDestroy(UINT nMsgType , WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
// 	if(m_hStoppedIcon != NULL)
// 	{
// 		::DestroyIcon(m_hStoppedIcon);
// 		m_hStoppedIcon = NULL;
// 	}
// 
// 	if(m_hStartingIcon != NULL)
// 	{
// 		::DestroyIcon(m_hStartingIcon);
// 		m_hStartingIcon = NULL;
// 	}
// 
// 	if(m_hRunningIcon != NULL)
// 	{
// 		::DestroyIcon(m_hRunningIcon);
// 		m_hRunningIcon = NULL;
// 	}
	return 0;
}
LRESULT CMainDlg::OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_bExit)
	{
		Stop();
		m_pConsoleTray->DeleteTray();
		DestroyWindow();
		::PostQuitMessage(0);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}

	return 0;
}


HWND CMainDlg::FindAgentWnd()
{
	return FindWindow(_T("634BDC03-6A8A-49ca-9374-22E43DCB0DD7"), NULL);
}

BOOL CMainDlg::KillAgentExist()
{
	HWND hWnd = FindAgentWnd();
	if(hWnd != NULL)
	{	
		::SendMessage(hWnd, WM_CLOSE, NULL,NULL);
		return TRUE;
	}			
	return FALSE;
}
BOOL CMainDlg::Stop()
{
	KillAgentExist();
	ResetValues();
	return TRUE;
}
BOOL CMainDlg::Start()
{
	SetUIStatus(UI_STATUS_STARTING);

	KillAgentExist();

	TCHAR sTemp[20] = {0};
	_ltot((UINT)m_hWnd,sTemp,10);

	TCHAR sCmdLine[MAX_PATH] = {0};
	GetModuleFileName(GetModuleHandle(NULL), sCmdLine, MAX_PATH - 1);
	TCHAR* p = _tcsrchr(sCmdLine, '\\');
	*(++p) = '\0';
	lstrcat(sCmdLine,_T("SmsServer.exe "));
	lstrcat(sCmdLine,sTemp);

	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi;

	BOOL b = CreateProcess(NULL,sCmdLine,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);
	if(b)
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
	{
		LoadString(m_hResModule, IDS_CREATE_PROCESS_ERROR, m_szErrorMsg, MAX_PATH);
		SetUIStatus(UI_STATUS_STOPPED);
	}
	return b;
}

VOID CMainDlg::SetUIStatus(ENUM_UI_STATUS s)
{
	TCHAR szBuf[MAX_PATH] = {0};
	TCHAR szCount[10] = {0};
	if(s == UI_STATUS_STOPPED)
	{
		m_statusbar.SetPaneWidth(ID_PANE_STATUS,PANE_WIDTH);
		m_statusbar.SetPaneWidth(ID_PANE_COUNT,0);
		//m_statusbar.SetPaneIcon(ID_PANE_STATUS,m_hStoppedIcon);
		::LoadString(m_hResModule, IDS_STATUS_STOPPED,szBuf,MAX_PATH - 1);
		m_statusbar.SetPaneText(ID_PANE_STATUS,szBuf);
		
		m_btnStop.EnableWindow(FALSE);
		m_btnStart.EnableWindow(TRUE);
	}
	else if(s == UI_STATUS_RUNNING)
	{
		m_statusbar.SetPaneWidth(ID_PANE_STATUS,PANE_WIDTH);
		m_statusbar.SetPaneWidth(ID_PANE_COUNT,0);
		//m_statusbar.SetPaneIcon(ID_PANE_STATUS,m_hRunningIcon);
		::LoadString(m_hResModule, IDS_STATUS_RUNNING,szBuf,MAX_PATH - 1);
		//_itot(m_nConCount,szCount,10);
		m_statusbar.SetPaneText(ID_PANE_STATUS,szBuf);
		//m_statusbar.SetPaneText(ID_PANE_COUNT,szCount);

		m_btnStop.EnableWindow(TRUE);
		m_btnStart.EnableWindow(FALSE);
	}
	else if(s == UI_STATUS_STARTING)
	{
		m_statusbar.SetPaneWidth(ID_PANE_STATUS,PANE_WIDTH);
		m_statusbar.SetPaneWidth(ID_PANE_COUNT,0);
		//m_statusbar.SetPaneIcon(ID_PANE_STATUS,m_hStartingIcon);
		::LoadString(m_hResModule, IDS_STATUS_STARTING,szBuf,MAX_PATH - 1);
		m_statusbar.SetPaneText(ID_PANE_STATUS,szBuf);
		
		m_btnStop.EnableWindow(FALSE);
		m_btnStart.EnableWindow(FALSE);
	}

	m_eCurrentStatus = s;
	m_pConsoleTray->ModifyTray(m_eCurrentStatus);
	Invalidate();
}
LRESULT CMainDlg::OnExit(WORD, WORD, HWND, BOOL&)
{
	m_bExit = TRUE;
	PostMessage(WM_CLOSE, 0, 0);
	return 0;
}
LRESULT CMainDlg::OnAbout(WORD, WORD ,HWND ,BOOL&)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}
LRESULT CMainDlg::OnShowMainDlg(WORD, WORD, HWND, BOOL&)
{
	ShowWindow(SW_SHOW);
	if(IsIconic())
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	SetForegroundWindow(m_hWnd);
	
	return 0;
}
LRESULT CMainDlg::OnTrayMsg(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if(lParam == WM_LBUTTONDBLCLK || lParam == WM_LBUTTONDOWN)
	{
		PostMessage(WM_COMMAND, ID_SHOW_MAIN, 0);
	}
	if(lParam == WM_RBUTTONDOWN)
	{
		HMENU hMenu = ::LoadMenu(GetResourceInstance(), MAKEINTRESOURCE(IDR_TRAY_MENU));
		if(hMenu != NULL)
		{
			HMENU hSubMen = GetSubMenu(hMenu, 0);
			if(hSubMen != NULL)
			{
				MENUITEMINFO mi;
				mi.cbSize = sizeof(mi);
				mi.fMask = MIIM_STATE;
				if(GetMenuItemInfo(hSubMen, ID_SHOW_MAIN, FALSE, &mi))
				{
					mi.fState |= MFS_DEFAULT;
					SetMenuItemInfo(hSubMen, ID_SHOW_MAIN, FALSE, &mi);
				}

				if(m_eCurrentStatus == UI_STATUS_STOPPED)
				{
					EnableMenuItem(hSubMen, IDC_START, MF_BYCOMMAND|MF_ENABLED);
					EnableMenuItem(hSubMen, IDC_STOP, MF_BYCOMMAND|MF_GRAYED);
				}
				else if(m_eCurrentStatus == UI_STATUS_STARTING)
				{
					EnableMenuItem(hSubMen, IDC_START, MF_BYCOMMAND|MF_GRAYED);
					EnableMenuItem(hSubMen, IDC_STOP, MF_BYCOMMAND|MF_GRAYED);
				}
				else
				{
					EnableMenuItem(hSubMen, IDC_START, MF_BYCOMMAND|MF_GRAYED);
					EnableMenuItem(hSubMen, IDC_STOP, MF_BYCOMMAND|MF_ENABLED);
				}
				
				CPoint pt;
				GetCursorPos(&pt);
				SetForegroundWindow(m_hWnd);
				TrackPopupMenu(hSubMen, TPM_LEFTBUTTON|TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
			}
			DestroyMenu(hMenu);
		}
	}	

	return 0;
}



CConsoleTray::CConsoleTray(CMainDlg* pDlg)
{
	_ASSERT(pDlg != NULL);
	m_pDlg = pDlg;
}

VOID CConsoleTray::AddTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = *m_pDlg;
	nid.uID = 1;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage = WM_SHELL_MSG;
	HICON	 hIcon = ::LoadIcon(m_pDlg->GetResourceInstance(), MAKEINTRESOURCE(IDI_STOP));
	nid.hIcon = hIcon;
	lstrcpy(nid.szTip, _T("短信服务 - 已停止"));
	if(hIcon != NULL)
	{
		::DestroyIcon(hIcon);
	}
	Shell_NotifyIcon(NIM_ADD, &nid);
}

VOID CConsoleTray::ModifyTray(CMainDlg::ENUM_UI_STATUS state)
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = *m_pDlg;
	nid.uID = 1;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage = WM_SHELL_MSG;
	HICON hIcon = NULL;
	TCHAR szTip[64] = {0};
	if(state == CMainDlg::UI_STATUS_STOPPED)
	{
		lstrcpy(szTip, _T("短信服务 - 已停止"));
		hIcon = ::LoadIcon(m_pDlg->GetResourceInstance(), MAKEINTRESOURCE(IDI_STOP));
	}
	else if(state == CMainDlg::UI_STATUS_STARTING)
	{
		lstrcpy(szTip, _T("短信服务 - 正在启动"));
		hIcon = ::LoadIcon(m_pDlg->GetResourceInstance(), MAKEINTRESOURCE(IDI_START));
	}
	else
	{
		lstrcpy(szTip, _T("短信服务 - 正在运行"));
		hIcon = ::LoadIcon(m_pDlg->GetResourceInstance(), MAKEINTRESOURCE(IDI_RUN));
	}
	nid.hIcon = hIcon;
	lstrcpy(nid.szTip, szTip);
	if(hIcon != NULL)
	{
		DestroyIcon(hIcon);
	}
	Shell_NotifyIcon(NIM_MODIFY, &nid);
}

VOID CConsoleTray::DeleteTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = *m_pDlg;
	nid.uID = 1;
	Shell_NotifyIcon(NIM_DELETE, &nid);
}