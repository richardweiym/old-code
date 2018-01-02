#include "stdafx.h"
#include "SmsServiceMgr.h"
#include "SmsConfig.h"
#include "SmsAccountManager.h"
#include "SmsFactory.h"
#include "SmsControlDef.h"

CSmsServiceMgr::CSmsServiceMgr(HWND hWnd)
:m_SmsFactory(CSmsFactory::GetFactoryInstance())
{
	m_hController = hWnd;
	m_pSmsAccountManager = new CSmsAccountManager();
	_ASSERT(m_pSmsAccountManager!=NULL);
	m_pSmsConfig = new CSmsIniConfig();
	_ASSERT(m_pSmsConfig!=NULL);
	m_pSmsCommunicate = NULL;
	m_pSmsDevice = NULL;
	m_nErrorCode = 0;
	//m_bStarting = FALSE;
}

CSmsServiceMgr::~CSmsServiceMgr()
{
	Stop();
	if(m_pSmsAccountManager != NULL)
	{
		delete m_pSmsAccountManager;
		m_pSmsAccountManager = NULL;
	}
	if(m_pSmsConfig != NULL)
	{
		delete m_pSmsConfig;
		m_pSmsConfig = NULL;
	}
}

BOOL CSmsServiceMgr::Start()
{
	TCHAR szDeviceTag[30] = {0};
	TCHAR szDeviceParameters[MAX_PATH]={0};
	TCHAR szDeviceDllPath[MAX_PATH] = {0};
	TCHAR szCommuTag[30] = {0};
	TCHAR szCommuParameters[MAX_PATH] = {0};

	//m_bStarting = TRUE;

	//载入帐号
	if(m_pSmsAccountManager !=NULL)
	{
		if(!(m_pSmsAccountManager->LoadAccounts()))
		{
			m_nErrorCode = SMS_ACCOUNT_ERROR_LOAD;
			goto __Start_end;
		}
	}
	 
	//载入配置信息
	if(m_pSmsConfig != NULL)
	{
		m_nErrorCode = SMS_NOT_ENOUGH_MEMORY;
		m_pSmsConfig->GetUseCommunicateTag(szCommuTag,29);
		m_pSmsConfig->GetUseCommunicateParameters(szCommuParameters,MAX_PATH-1);
		m_pSmsConfig->GetUseDeviceDllName(szDeviceDllPath,MAX_PATH-1);
		m_pSmsConfig->GetUseDeviceParameters(szDeviceParameters,MAX_PATH-1);
		m_pSmsConfig->GetUseDeviceTag(szDeviceTag,29);
	}
	
	//创建设备和通信对象
	m_pSmsDevice = m_SmsFactory.CreateDeviceObject(szDeviceTag,this);
	m_pSmsCommunicate = m_SmsFactory.CreateCommunicateObject(szCommuTag,this);

	if((m_pSmsCommunicate==NULL) ||(m_pSmsDevice==NULL))
	{
		m_nErrorCode = SMS_INCORRECT_TAG;
		goto __Start_end;
	}

	//启动硬件收发模块
	if(!(m_pSmsDevice->InitModule(szDeviceDllPath)))
	{
		m_nErrorCode = SMS_INCORRECT_DEVICE_DLL;
		goto __Start_end;
	}
	if(!(m_pSmsDevice->SetParameters(szDeviceParameters)))
	{
		m_nErrorCode = SMS_INCORRECT_DEVICE_PARAM;
		goto __Start_end;
	}
	if(!(m_pSmsDevice->StartWork()))
	{
		m_nErrorCode = m_pSmsDevice -> GetLastErrorCode();
		goto __Start_end;
	}

	//启动通信模块
	if(!(m_pSmsCommunicate->InitModule()))
	{
		m_nErrorCode = SMS_NOT_ENOUGH_MEMORY;
		goto __Start_end;
	}
	if(!(m_pSmsCommunicate->SetParameters(szCommuParameters)))
	{
		m_nErrorCode = SMS_INCORRECT_COMMU_PARAM;
		goto __Start_end;
	}
	if(!(m_pSmsCommunicate->StartCommunicate()))
	{
		m_nErrorCode = m_pSmsCommunicate ->GetLastErrorCode();
		goto __Start_end;
	}

	FeedbackMessageToController(WM_SMS_ON_START,SMS_TYPE_SERVICE,SMS_OK);
	//m_bStarting = FALSE;
	return TRUE;

__Start_end:
	//m_bStarting = FALSE;
	Stop();
	FeedbackMessageToController(WM_SMS_ON_START,SMS_TYPE_SERVICE,m_nErrorCode);
	return FALSE;
}

VOID CSmsServiceMgr::Stop()
{
	//if(m_bStarting) return;

	BOOL bStopped(FALSE);
	if(m_pSmsDevice != NULL)
	{	
		if(m_pSmsDevice->IsRunning())
		{
			m_pSmsDevice->StopWork();
			bStopped = TRUE;
		}

		m_pSmsDevice->ReleaseModule();
		delete m_pSmsDevice;

		m_pSmsDevice = NULL;
	}

	if(m_pSmsCommunicate!=NULL)
	{
		if(m_pSmsCommunicate->IsRunning())
		{
			m_pSmsCommunicate->StopCommunicate();
			bStopped = TRUE;
		}

		m_pSmsCommunicate->ReleaseModule();
		delete m_pSmsCommunicate;

		m_pSmsCommunicate = NULL;
	}	

	if(bStopped)
	{
		FeedbackMessageToController(WM_SMS_ON_STOP,SMS_TYPE_SERVICE,SMS_OK);
	}

}
BOOL CSmsServiceMgr::IsRunning()
{
	return ((m_pSmsCommunicate!=NULL)
		&&(m_pSmsDevice!=NULL)
		&&(m_pSmsCommunicate->IsRunning())
		&&(m_pSmsDevice->IsRunning()));
}

VOID CSmsServiceMgr::OnCommunicateLogon(CSMSMessage* pMsg)
{
	if(m_pSmsAccountManager!=NULL)
	{
		BOOL bRet = m_pSmsAccountManager->CheckCanLogon(pMsg);
		if(m_pSmsCommunicate != NULL)
		{
			m_pSmsCommunicate->SendMsg(pMsg);
		}
		if(bRet)
		{
// 			COPYDATASTRUCT cs;
// 			CMessagePackage package(pMsg);
// 			cs.dwData = SMS_ON_CONNECT;
// 			cs.lpData=package.GetData();
// 			cs.cbData = package.GetLength();
// 			FeedbackMessageToController(WM_COPYDATA,NULL,(LPARAM)&cs,FALSE);

//			FeedbackMessageToController(WM_SMS_ON_CONNECT,NULL,NULL);
			INT nCount = m_pSmsAccountManager->GetLogonCount();
			FeedbackMessageToController(WM_SMS_ON_CONNECT_COUNT, (WPARAM)nCount, NULL);
		}
	}	
}

VOID CSmsServiceMgr::OnCommunicateSend(CSMSMessage* pMsg)
{
	if(m_pSmsDevice!=NULL)
	{
		m_pSmsDevice->SendMsg(pMsg);
	}
}

VOID CSmsServiceMgr::OnCommunicateDisconnect(GUID& clientId)
{
	if(m_pSmsAccountManager != NULL)
	{
		m_pSmsAccountManager->LogoutAccount(clientId);
		INT nCount = m_pSmsAccountManager->GetLogonCount();
		FeedbackMessageToController(WM_SMS_ON_CONNECT_COUNT, (WPARAM)nCount, NULL);
	}

// 	CSMSMessage msg(clientId,REQUEST_DISCONNECT);
// 	COPYDATASTRUCT cs;
// 	CMessagePackage package(&msg);
// 	cs.dwData = SMS_ON_DISCONNECT;
// 	cs.lpData = package.GetData();
// 	cs.cbData = package.GetLength();
// 	FeedbackMessageToController(WM_COPYDATA,NULL,(LPARAM)&cs, FALSE);
}

VOID CSmsServiceMgr::OnCommunicateError(INT* pReason)
{
}

VOID CSmsServiceMgr::OnCommunicateStarted()
{
	FeedbackMessageToController(WM_SMS_ON_START,SMS_TYPE_COMMUNICATE,SMS_OK);
}

VOID CSmsServiceMgr::OnCommunicateStopped()
{
	FeedbackMessageToController(WM_SMS_ON_STOP,SMS_TYPE_COMMUNICATE,SMS_OK);
}

VOID CSmsServiceMgr::OnDeviceReceiveMsg(CSMSMessage* msg)
{
	if(m_pSmsCommunicate!=NULL)
	{
		msg->SetMsgType(REQUEST_DISPATCH);
		m_pSmsCommunicate->SendMsg(msg);
	}

	FeedbackMessageToController(WM_SMS_ON_RECEIVE_MESSAGE,NULL,NULL);
}
VOID CSmsServiceMgr::OnDeviceSendedMsg(CSMSMessage* msg)
{
	if(m_pSmsCommunicate!=NULL)
	{
		msg->SetMsgType(REQUEST_SENDBACK);
		m_pSmsCommunicate->SendMsg(msg);
	}
	FeedbackMessageToController(WM_SMS_ON_SEND_MESSAGE,NULL,NULL);

}

VOID CSmsServiceMgr::OnDeviceStarted()
{
	FeedbackMessageToController(WM_SMS_ON_START,SMS_TYPE_DEVICE,SMS_OK);
}
VOID CSmsServiceMgr::OnDeviceStopped()
{
	FeedbackMessageToController(WM_SMS_ON_STOP,SMS_TYPE_DEVICE,SMS_OK);
}
VOID CSmsServiceMgr::OnDeviceError(INT* pReason)
{
}

VOID CSmsServiceMgr::FeedbackMessageToController(UINT nMsgType,WPARAM wParam, LPARAM lParam, BOOL bPost)
{
	if(IsWindow(m_hController))
	{
		if(bPost)
		{
			PostMessage(m_hController, nMsgType,wParam,lParam);	
		}
		else
		{
			SendMessage(m_hController, nMsgType,wParam,lParam);
		}	
	}
}

VOID CSmsServiceMgr::ServiceStatusNotify()
{
	INT nStatus(SMS_FAIL);
	if(IsRunning())
	{
		nStatus = SMS_OK;
	}

	FeedbackMessageToController(WM_SMS_ON_STATUS_INDICATION,(WPARAM)nStatus,NULL);
}
VOID CSmsServiceMgr::OnDeviceSignalCount(INT* pCount)
{
	FeedbackMessageToController(WM_SMS_ON_DEVICE_SIGNAL, (WPARAM)(*pCount), NULL);
 
}

VOID CSmsServiceMgr::ProcessControllerRequest(UINT, WPARAM, LPARAM)
{

}

VOID CSmsServiceMgr::CheckIsAccountUsing()
{
	INT nConCount(0);
	if(IsRunning() && (m_pSmsAccountManager != NULL))
	{
		CSmsAccount* pAccount = m_pSmsAccountManager->GetFirst();
		while(pAccount != NULL)
		{
			if((pAccount->m_bLogoned))
			{
				if(!(m_pSmsCommunicate->CheckIsConnected(pAccount->m_clientId)))
				{
					m_pSmsAccountManager->LogoutAccount(pAccount->m_clientId);
				}
				else
				{
					nConCount++;
				}
			}
			pAccount = m_pSmsAccountManager->GetNext();
		}
	}
	FeedbackMessageToController(WM_SMS_ON_CONNECT_COUNT, (WPARAM)(nConCount), NULL);
}