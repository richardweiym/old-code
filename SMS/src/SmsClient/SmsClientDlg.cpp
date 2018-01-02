#include "StdAfx.h"
#include "SmsClientDlg.h"
#include "userManager.h"
#include "mainfrm.h"
#include "ServiceManager.h"

CSplashDlg::CSplashDlg(BOOL bSplash)
{
	m_bSplash = bSplash;
}
CSplashDlg::~CSplashDlg()
{

}
LRESULT CSplashDlg::OnInitDlg(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CenterWindow();
	
	if(!m_bSplash)
	{
		CRect rt;
		GetWindowRect(&rt);
		rt.bottom -= 20;
		MoveWindow(rt.left, rt.top, rt.Width(), rt.Height(), TRUE);
	}
	else
	{
		m_pb = GetDlgItem(IDC_PROGRESS);
		m_pb.SetRange(0, 100);
		m_pb.SetStep(1);
	}

	return 0;
}

LRESULT CSplashDlg::OnLButtonUp(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	if(!m_bSplash)
	{
		EndDialog(0);
	}
	else
	{
		bHandled = FALSE;
	}
	 
	return 0;
}

LRESULT CSplashDlg::OnInitMsg(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_bSplash)
	{
		if(!m_pb.IsWindow()) return 1;

		INIT_STEP eStep = (INIT_STEP)wParam;
		switch(eStep)
		{
		case	ON_START:
			m_pb.SetPos(5);
			break;
		case	ON_CREATE:
			m_pb.OffsetPos(10);
			break;
		case	ON_LOAD_TOSEND:
			m_pb.OffsetPos(15);
			break;
		case	ON_LOAD_SENDED:
			m_pb.OffsetPos(15);
			break;
		case	ON_LOAD_RECEIVED:
			m_pb.OffsetPos(15);
			break;
		case	ON_LOAD_DELETED:
			m_pb.OffsetPos(15);
			break;
		case	ON_LOAD_CONTACT:
			m_pb.OffsetPos(10);
			break;
		case	ON_FINISHED:
			m_pb.OffsetPos(15);
			break;
		}
	}
	
	return 0;
}


CSettingDlg::CSettingDlg(CUserManager* pUserManager)
{
	m_pUserManager = pUserManager;
}
LRESULT CSettingDlg::OnInitDlg(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CenterWindow();
	
	BOOL bOK;
	CSmsUser* pUser = m_pUserManager->GetCurrentUser(bOK);
	
	CEdit edit;
	edit = GetDlgItem(IDC_NAME);
	edit.SetLimitText(90);
	edit.SetWindowText(pUser->m_strName);

	edit = GetDlgItem(IDC_PASSWORD);
	edit.SetLimitText(90);
	edit.SetWindowText(pUser->m_strPwd);

	edit = GetDlgItem(IDC_SERVER);
	edit.SetLimitText(40);
	edit.SetWindowText(pUser->m_strServer);

	SetDlgItemInt(IDC_PORT, pUser->m_nPort);

	CButton btn;
	btn = GetDlgItem(IDC_AUTO_INFO);
	btn.SetCheck((pUser->m_bTipOn? BST_CHECKED: BST_UNCHECKED));

	btn = GetDlgItem(IDC_AUTO_CON);
	btn.SetCheck((pUser->m_bAutoConnect? BST_CHECKED: BST_UNCHECKED));

	return 0;
}
LRESULT CSettingDlg::OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close();
	return 0;
}
LRESULT CSettingDlg::OnOK(WORD,WORD,HWND,BOOL&)
{
	Save();
	Close();
	return 0;
}
LRESULT CSettingDlg::OnCancel(WORD,WORD,HWND,BOOL&)
{
	Close();
	return 0;
}
LRESULT CSettingDlg::Close()
{
	EndDialog(0);
	return 0;
}
LRESULT CSettingDlg::Save()
{
	TCHAR szBuf[MAX_PATH] = {0};

	BOOL bOK;
	CSmsUser* pUser = m_pUserManager->GetCurrentUser(bOK);
	GetDlgItemText(IDC_NAME, szBuf, MAX_PATH);
	pUser->m_strName = szBuf;
	GetDlgItemText(IDC_PASSWORD, szBuf, MAX_PATH);
	pUser->m_strPwd = szBuf;
	GetDlgItemText(IDC_SERVER, szBuf, MAX_PATH);
	pUser->m_strServer = szBuf;
	pUser->m_nPort = GetDlgItemInt(IDC_PORT);
	
	CButton btn;
	btn = GetDlgItem(IDC_AUTO_CON);
	BOOL b = (btn.GetCheck() == BST_CHECKED);
	pUser->m_bAutoConnect = b;
	btn = GetDlgItem(IDC_AUTO_INFO);
	b = (btn.GetCheck() == BST_CHECKED);
	pUser->m_bTipOn = b;

	m_pUserManager->SaveCurrentUser();

	return 0;
}



CInformWindow::CInformWindow()
{
	m_hBmpBack = NULL;
	m_imgClose = NULL;
	m_bShow = FALSE;
}
LRESULT CInformWindow::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}
VOID CInformWindow::SetRes(HBITMAP hBmpBack, CImageList& imgClose)
{
	m_hBmpBack = hBmpBack;
	m_imgClose = imgClose;
}
LRESULT CInformWindow::OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CPaintDC dc(m_hWnd);
	bHandled = FALSE;

	if(m_hBmpBack != NULL && m_imgClose != NULL)
	{
		BITMAP bmp = {0};
		GetObject(m_hBmpBack, sizeof(bmp), &bmp);
		INT nWidth = bmp.bmWidth;
		INT nHeight = bmp.bmHeight;
		
		CDC memDC;
		memDC.CreateCompatibleDC(dc);
		memDC.SelectBitmap(m_hBmpBack);
		
		m_imgClose.Draw(memDC, 0, CPoint(CLOSE_LEFT, CLOSE_TOP), ILD_NORMAL);	
		BitBlt (dc, 0, 0, nWidth, nHeight, memDC, 0, 0, SRCCOPY) ;

		dc.SetBkMode(TRANSPARENT);
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(lf),&lf);
		lf.lfWeight = 300;
		lf.lfHeight = -12;
		lf.lfWeight = FW_BOLD;
		strcpy(lf.lfFaceName, "Tahoma");
		HFONT hNewFont1 = CreateFontIndirect(&lf);
		HFONT hOldFont = dc.SelectFont(hNewFont1);
		RECT rt1 = {10, 45, 150, 60};
		
		CString strInfo;
		strInfo.Format(_T("%s(%s)"), m_strName, m_strTel);
		dc.DrawText(strInfo, lstrlen(strInfo), &rt1, DT_LEFT|DT_TOP|DT_END_ELLIPSIS);
		
		RECT rt2 = {10, 60, 150, 115};
		lf.lfWeight = FW_NORMAL;
		HFONT hNewFont2 = CreateFontIndirect(&lf);
		dc.SelectFont(hNewFont2);
		dc.DrawText(m_strContent, lstrlen(m_strContent), &rt2, DT_LEFT|DT_TOP|DT_WORDBREAK|DT_END_ELLIPSIS);

		dc.SelectFont(hOldFont);
		DeleteObject(hNewFont1);
		DeleteObject(hNewFont2);
		memDC.DeleteDC();
		
		bHandled = TRUE;
	}


	return 0;
}



VOID CInformWindow::ShowInformation(CString& strName, CString& strTel, CString& strContent)
{

	if(IsWindow())
	{
		m_strName = strName;
		m_strTel = strTel;
		m_strContent = strContent;

		ShowWindow(SW_SHOW);
		InvalidateRect(NULL, TRUE);
		m_bShow = TRUE;
		SetTimer(1, 10000);		
	}
}
VOID CInformWindow::Close()
{
	KillTimer(1);
	ShowWindow(SW_HIDE);
	m_bShow = FALSE;
}
LRESULT CInformWindow::OnTimer(UINT, WPARAM, LPARAM, BOOL&)
{
	Close();
	return 0;
}
LRESULT CInformWindow::OnLButtonClick(UINT, WPARAM, LPARAM, BOOL&)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	RECT rtClose = {CLOSE_LEFT, CLOSE_TOP, CLOSE_LEFT+CLOSE_WIDTH, CLOSE_TOP+CLOSE_HEIGH};
	if(PtInRect(&rtClose, pt))
	{
		Close();
	}

	return 0;
}
LRESULT CInformWindow::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}


BOOL CInformProxy::Init()
{
	HMODULE hRes = CMainFrame::GetResourceModule();
	m_hBmpBack = LoadBitmap(hRes, MAKEINTRESOURCE(IDB_INFORMATION));
	_ASSERT(m_hBmpBack != NULL);
	
	HBITMAP hBmpClose = LoadBitmap(hRes, MAKEINTRESOURCE(IDB_CLOSE));
	_ASSERT(hBmpClose != NULL);
	m_imgClose.Create(14, 13, ILC_COLOR24, 1, 0);
	m_imgClose.Add(hBmpClose);
	DeleteObject(hBmpClose);
	return TRUE;
}

VOID CInformProxy::ShowInformation(CString& strName, CString& strTel, CString& strContent)
{
	CInformWindow* p = GetAnIdleWin();
	if(p != NULL)
	{
		p->SetRes(m_hBmpBack, m_imgClose);
		p->ShowInformation(strName, strTel, strContent);
	}
}
CInformWindow* CInformProxy::GetAnIdleWin()
{
	BOOL bFind(FALSE);
	CInformWindow* pWin = NULL;
	for(INT i=0; i<m_arWindows.GetSize(); i++)
	{
		pWin = m_arWindows[i];
		if((pWin != NULL) && (pWin->IsWindow()) && (!pWin->m_bShow))
		{
			bFind = TRUE;
			break;
		}
	}

	if(!bFind)
	{
		pWin = new CInformWindow;

		RECT rt = {0, 0, 0, 0};
		pWin->Create(NULL, rt, NULL, WS_POPUP|WS_VISIBLE, WS_EX_TOPMOST|WS_EX_TOOLWINDOW);
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
		pWin->MoveWindow(rt.right - WIN_WIDTH, rt.bottom - WIN_HEIGHT*(1+m_arWindows.GetSize()), WIN_WIDTH, WIN_HEIGHT);
		m_arWindows.Add(pWin);
	}

	return pWin;
}
VOID CInformProxy::Release()
{
	CInformWindow* pWin = NULL;
	for(INT i=0; i<m_arWindows.GetSize(); i++)
	{
		pWin = m_arWindows[i];
		if(pWin != NULL)
		{
			if(pWin->IsWindow())
			{
				pWin->DestroyWindow();
			}
			delete pWin;
		}
	}
	m_arWindows.RemoveAll();

	if(m_hBmpBack != NULL)
	{
		DeleteObject(m_hBmpBack);
	}
	m_imgClose.Destroy();
}

// CInformationDlg::CInformationDlg()
// {
// 	m_hEditBrush = NULL;
// }
// CInformationDlg::~CInformationDlg()
// {
// 	if(m_hEditBrush != NULL)
// 	{
// 		DeleteObject(m_hEditBrush);
// 		m_hEditBrush = NULL;
// 	}
// }
LRESULT CInformationDlg::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{	
	CenterWindow();
	

	CEdit editEmail = GetDlgItem(IDC_EMAIL);
	CEdit editQQ = GetDlgItem(IDC_QQ);

	editEmail.SetWindowText(_T("decent_guhu@sina.com"));
	editQQ.SetWindowText(_T("55841310"));	
	

	return 0;
}
LRESULT CInformationDlg::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
	EndDialog(0);
	return 0;
}
LRESULT CInformationDlg::OnOK(WORD, WORD, HWND, BOOL&)
{
	EndDialog(0);
	return 0;
}
LRESULT CInformationDlg::OnCtlColorEdit(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(GetDlgItem(IDC_EMAIL) == (HWND)lParam || GetDlgItem(IDC_QQ) == (HWND)lParam)
	{
		HDC hDc = (HDC)wParam;
		::SetBkMode(hDc, TRANSPARENT);
		::SetTextColor(hDc, RGB(0, 0, 255));

		return (long)GetSysColorBrush(COLOR_BTNFACE);
	}
	
	bHandled = FALSE;
	return FALSE;
}
