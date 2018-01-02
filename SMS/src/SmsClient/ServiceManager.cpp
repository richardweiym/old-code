#include "StdAfx.h"
#include "ServiceManager.h"
#include "mainfrm.h"
#include "userManager.h"
#include "../ClientSocketLib/SmsSocketClient.h"
#include "SmsClientDlg.h"

CServiceManager::CServiceManager()
{
	m_pMainFrame = new CMainFrame(this);
	m_pUserManager = new CUserManager;
	m_pClient = new CSocketClient(this);
	m_pSplashWnd = new CSplashDlg(TRUE);
}

CServiceManager::~CServiceManager()
{
	if(m_pMainFrame != NULL)
	{
		delete m_pMainFrame;
		m_pMainFrame = NULL;
	}
	if(m_pUserManager != NULL)
	{
		delete m_pUserManager;
		m_pUserManager = NULL;
	}
	if(m_pClient != NULL)
	{
		delete m_pClient;
		m_pClient = NULL;
	}
	if(m_pSplashWnd != NULL)
	{
		delete m_pSplashWnd;
		m_pSplashWnd = NULL;
	}
}
VOID CServiceManager::StartService()
{	
	m_pSplashWnd->Create(NULL);
	m_pSplashWnd->ShowWindow(SW_SHOW);
	m_pSplashWnd->UpdateWindow();

	m_pMainFrame->CreateEx();
 	if(m_pMainFrame->IsWindow())
 	{
		m_pMainFrame->LoadSms();
 	}

	m_pSplashWnd->DestroyWindow();
	
	m_pMainFrame->ShowWindow(SW_SHOW);
	m_pMainFrame->UpdateWindow();

	BOOL bOK;
	CSmsUser* pUser = m_pUserManager->GetCurrentUser(bOK);
	if(pUser->m_bAutoConnect)
	{
		m_pMainFrame->StartConnect();
	}

}
VOID CServiceManager::OnInitStep(INIT_STEP eStep)
{
	m_pSplashWnd->SendMessage(WM_ON_INIT_MSG, (WPARAM)eStep, 0);
}
CUserManager* CServiceManager::GetUserManager()
{
	return m_pUserManager;
}
CSocketClient* CServiceManager::GetSmsClient()
{
	return m_pClient;
}
VOID CServiceManager::OnConnect(BOOL bOK)
{
	if(m_pMainFrame->IsWindow())
	{
		m_pMainFrame->SendMessage(WM_SMS_CLIENT, SMS_CLIENT_CONNECT_WP, bOK);
	}
}
VOID CServiceManager::OnSend(CSMSMessage* pMsg)
{
	if(m_pMainFrame->IsWindow())
	{
		m_pMainFrame->SendMessage(WM_SMS_CLIENT, SMS_CLIENT_SENDED_WP, (LPARAM)pMsg);
	}
}
VOID CServiceManager::OnReceive(CSMSMessage* pMsg)
{
	if(m_pMainFrame->IsWindow())
	{
		m_pMainFrame->SendMessage(WM_SMS_CLIENT, SMS_CLIENT_RECEIVED_WP, (LPARAM)pMsg);
	}
}
VOID CServiceManager::OnServiceStop()
{
	if(m_pMainFrame->IsWindow())
	{
		m_pMainFrame->SendMessage(WM_SMS_CLIENT, SMS_CLIENT_STOPPED_WP, 0);
	}
}