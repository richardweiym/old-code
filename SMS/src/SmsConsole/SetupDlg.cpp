#include "StdAfx.h"
#include "SetupDlg.h"
#include "maindlg.h"
#include "DeviceDlg.h"
#include "ConsoleConfig.h"
#include "../SmsServer/smsaccountmanager.h"


CSetupDlg::CSetupDlg(CMainDlg* p)
{
	m_pMainDlg = p;
	m_pDeviceDlg = new CDeviceDlg(p);
	m_pCommuDlg = new CCommuDlg(p);
	m_pAccountDlg = new CAccountDlg(p,this);
	m_pConfig = new CConsoleConfig;
//	m_pOtherDlg = new COtherDlg(p);
	m_pAccountManager = new CSmsAccountManager;
	m_pRegisterDlg = new CRegisterDlg(p);
}
CSetupDlg::~CSetupDlg()
{
	if(m_pDeviceDlg != NULL)
	{
		delete m_pDeviceDlg;
		m_pDeviceDlg = NULL;
	}

	if(m_pCommuDlg != NULL)
	{
		delete m_pCommuDlg;
		m_pCommuDlg = NULL;
	}

	if(m_pAccountDlg != NULL)
	{
		delete m_pAccountDlg;
		m_pAccountDlg = NULL;
	}
	if(m_pConfig != NULL)
	{
		delete m_pConfig;
		m_pConfig = NULL;
	}
	if(m_pAccountManager != NULL)
	{
		delete m_pAccountManager;
		m_pAccountManager = NULL;
	}
	if(m_pRegisterDlg != NULL)
	{
		delete m_pRegisterDlg;
		m_pRegisterDlg = NULL;
	}
// 	if(m_pOtherDlg != NULL)
// 	{
// 		delete m_pOtherDlg;
// 		m_pOtherDlg = NULL;
// 	}
}
LRESULT CSetupDlg::OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyWindow();
	return 0;
}

LRESULT CSetupDlg::OnInitDialog(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ModifyStyleEx(0,WS_EX_APPWINDOW);
	CenterWindow(m_pMainDlg->m_hWnd);
	
	HICON hIcon = LoadIcon(m_pMainDlg->GetResourceInstance(), MAKEINTRESOURCE(IDI_SETTING));
	if(hIcon != NULL)
	{
		SetIcon(hIcon, TRUE);
		SetIcon(hIcon, FALSE);
		::DestroyIcon(hIcon);
	}

	

	m_list = GetDlgItem(IDC_LIST);
	TCHAR szBuf[100] = {0};
	HINSTANCE hMod = m_pMainDlg->GetResourceInstance();

	INT i = 0;
	LoadString(hMod, IDS_DEVICE_SETTING, szBuf, 100);
	m_list.InsertString(i++,szBuf);
	LoadString(hMod, IDS_COMMU_SETTING, szBuf, 100);
	m_list.InsertString(i++,szBuf);
	LoadString(hMod, IDS_ACCOUNT_SETTING, szBuf, 100);
	m_list.InsertString(i++,szBuf);
	m_list.InsertString(i++, _T("注册信息"));
// 	LoadString(hMod, IDS_OTHER_SETTING, szBuf, 100);
// 	m_list.InsertString(i++,szBuf);
	m_list.SetCurSel(0);


	//设置内容
	CString str;
	UINT nPort;
	INT nMaxFailCount;
	BOOL bClearWhenStart;
//	(*m_pAccountManager) = *(m_pMainDlg->GetAccountManager());
	m_pAccountManager->LoadAccounts();	
	if(m_pConfig->GetDeviceInfo(str, nPort, nMaxFailCount, bClearWhenStart))
	{
		m_pDeviceDlg->SetCenterNum((LPSTR)(LPCTSTR)str);
		m_pDeviceDlg->SetComPort(nPort);
		m_pDeviceDlg->SetMaxFailCount(nMaxFailCount);
		m_pDeviceDlg->SetClearTag(bClearWhenStart);
	}
	if(m_pConfig->GetSocketPort(nPort))
	{
		m_pCommuDlg->SetPort(nPort);
	}
	if(m_pAccountManager->GetIsNeedLogon())
	{
		m_pCommuDlg->SetIsNeedLogon(TRUE);
	}
	else
	{
		m_pCommuDlg->SetIsNeedLogon(FALSE);
	}

	//创建子窗口
	HWND hWnd = GetDlgItem(IDC_STATICCONTAINER);
	::GetClientRect(hWnd, &m_rectChild);
	m_rectChild.SetRect(CPoint(m_rectChild.left + 10,m_rectChild.top + 20),CPoint(m_rectChild.right -10,m_rectChild.bottom -10));

	m_pDeviceDlg->Create(hWnd,m_rectChild,NULL);
	m_pDeviceDlg->MoveWindow(&m_rectChild);

	m_pCommuDlg->Create(hWnd, m_rectChild, NULL);
	m_pCommuDlg->MoveWindow(&m_rectChild);
	
	m_pAccountDlg->Create(hWnd, m_rectChild, NULL);
	m_pAccountDlg->MoveWindow(&m_rectChild);
	
	m_pRegisterDlg->Create(hWnd, m_rectChild, NULL);
	m_pRegisterDlg->MoveWindow(&m_rectChild);
// 	m_pOtherDlg->Create(hWnd, m_rectChild, NULL);
// 	m_pOtherDlg->MoveWindow(&m_rectChild);

	ShowCurrentSelect();

	return 0;
}
BOOL CSetupDlg::SaveAllConfig()
{
	TCHAR szBuf[MAX_PATH] = {0};
	UINT nPort;
	INT nMaxFailCount;
	BOOL bClearWhenStart;
	BOOL bOK(FALSE);

	m_pDeviceDlg->GetCenterNum(szBuf,MAX_PATH);
	nPort = m_pDeviceDlg->GetComPort();
	nMaxFailCount = m_pDeviceDlg->GetMaxFailCount();
	bClearWhenStart = m_pDeviceDlg->GetClearTag();
	m_pConfig->SetDeviceInfo(CString(szBuf), nPort,nMaxFailCount,bClearWhenStart);

	nPort = m_pCommuDlg->GetPort();
	m_pConfig->SetSocketPort(nPort);
	
	BOOL bNeedLogon = m_pCommuDlg->GetIsNeedLogon();
	m_pAccountManager->SetIsNeedLogon(bNeedLogon);
	bOK = m_pAccountManager->SaveAccounts();

	return bOK;
}
LRESULT CSetupDlg::OnOK(WORD,WORD,HWND,BOOL&)
{
	if(!SaveAllConfig())
	{
		CInfoDlg dlg;
		dlg.SetText(CString(_T("配置信息保存失败!")));
		dlg.DoModal();
	}
	else
	{
		DestroyWindow();
	}


// 	if(!(m_pMainDlg->IsServiceRunning()))
// 	{
// 		m_pMainDlg->GetAccountManager()->LoadAccounts();
// 	}
	return 0;
}
LRESULT CSetupDlg::OnCancel(WORD,WORD,HWND,BOOL&)
{
	DestroyWindow();
	return 0;
}
LRESULT CSetupDlg::OnApply(WORD,WORD,HWND,BOOL&)
{
	if(!SaveAllConfig())
	{
		CInfoDlg dlg;
		dlg.SetText(CString(_T("配置信息保存失败!")));
		dlg.DoModal();
	}
// 	if(!(m_pMainDlg->IsServiceRunning()))
// 	{
// 		m_pMainDlg->GetAccountManager()->LoadAccounts();
// 	}

	return 0;
}
LRESULT CSetupDlg::OnSelectChange(WORD,WORD,HWND hWnd,BOOL&)
{
	ShowCurrentSelect();
	return 0;
}
LRESULT CSetupDlg::OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_pDeviceDlg->IsWindow())
	{
		m_pDeviceDlg->DestroyWindow();
	}
	if(m_pCommuDlg->IsWindow())
	{
		m_pCommuDlg->DestroyWindow();
	}
	if(m_pAccountDlg->IsWindow())
	{
		m_pAccountDlg->DestroyWindow();
	}
	if(m_pRegisterDlg->IsWindow())
	{
		m_pRegisterDlg->DestroyWindow();
	}
// 	if(m_pOtherDlg->IsWindow())
// 	{
// 		m_pOtherDlg->DestroyWindow();
// 	}
	return 0;
}
VOID CSetupDlg::ShowCurrentSelect()
{
	INT nIndex = m_list.GetCurSel();
	
	TCHAR szBuf[100] = {0};
	m_list.GetText(m_list.GetCurSel(),szBuf);
	::SetWindowText(GetDlgItem(IDC_STATICCONTAINER),szBuf);

	switch (nIndex)
	{
	case 0:		
		m_pDeviceDlg -> ShowWindow(SW_SHOW);
		m_pCommuDlg -> ShowWindow(SW_HIDE);
		m_pAccountDlg-> ShowWindow(SW_HIDE);
		m_pRegisterDlg->ShowWindow(SW_HIDE);
	//	m_pOtherDlg->ShowWindow(SW_HIDE);
		break;

	case 1:
		m_pDeviceDlg -> ShowWindow(SW_HIDE);
		m_pCommuDlg -> ShowWindow(SW_SHOW);
		m_pAccountDlg-> ShowWindow(SW_HIDE);
		m_pRegisterDlg->ShowWindow(SW_HIDE);
	//	m_pOtherDlg->ShowWindow(SW_HIDE);
		break;

	case 2:
		m_pDeviceDlg -> ShowWindow(SW_HIDE);
		m_pCommuDlg -> ShowWindow(SW_HIDE);
		m_pAccountDlg-> ShowWindow(SW_SHOW);
		m_pRegisterDlg->ShowWindow(SW_HIDE);
	//	m_pOtherDlg->ShowWindow(SW_HIDE);
		break;
	
	case 3:
		m_pDeviceDlg -> ShowWindow(SW_HIDE);
		m_pCommuDlg -> ShowWindow(SW_HIDE);
		m_pAccountDlg-> ShowWindow(SW_HIDE);
		m_pRegisterDlg->ShowWindow(SW_SHOW);
	//	m_pOtherDlg->ShowWindow(SW_SHOW);
	}
}