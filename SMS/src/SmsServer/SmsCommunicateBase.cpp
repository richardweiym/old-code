#include "stdafx.h"
#include "SmsCommunicateBase.h"
#include "SmsEventSink.h"
#include "SmsCommunicateAgent.h"
#include "SmsMessage.h"

CSmsCommunicateBase::CSmsCommunicateBase()
{
	m_pCommuSink = NULL;
	m_pAgentWnd = new CSmsCommunicateAgentWnd(this);
}

CSmsCommunicateBase::~CSmsCommunicateBase()
{
	ReleaseModule();
	delete m_pAgentWnd;
	m_pAgentWnd = NULL;
}

VOID CSmsCommunicateBase::OnCommunicateEvent(UINT nMsgType,WPARAM wParam, LPARAM lParam)
{
	switch(nMsgType)
	{
	case WM_SMS_COMMUNICATE_START:
		m_pCommuSink->OnCommunicateStarted();
		break;

	case WM_SMS_COMMUNICATE_ERROR:
		m_pCommuSink->OnCommunicateError((INT*)wParam);
		break;

	case WM_SMS_COMMUNICATE_STOP:
		m_pCommuSink->OnCommunicateStopped();
		break;

	case WM_SMS_COMMUNICATE_SEND:
		m_pCommuSink->OnCommunicateSend((CSMSMessage*)wParam);
		break;

	case WM_SMS_COMMUNICATE_LOGON:
		m_pCommuSink->OnCommunicateLogon((CSMSMessage*)wParam);
		break;
	
	case WM_SMS_COMMUNICATE_DISCONNECT:
		m_pCommuSink->OnCommunicateDisconnect((((CSMSMessage*)wParam)->GetClientID()));
		break;
	default:
		break;
	}
}

BOOL CSmsCommunicateBase::InitModule()
{
	BOOL bRet(FALSE);

	ReleaseModule();
	m_pAgentWnd->Create(NULL,CWindow::rcDefault,_T("SmsCommunicateAgentWnd"),WS_POPUP);
	if(m_pAgentWnd->IsWindow()) 
	{
		bRet = TRUE;
	}

	return bRet;
}
VOID CSmsCommunicateBase::ReleaseModule()
{
	if(m_pAgentWnd->IsWindow())
	{
		m_pAgentWnd->DestroyWindow();
	}
}

VOID CSmsCommunicateBase::SendMessageToAgent(UINT nMsgType, WPARAM wParam, LPARAM lParam)
{
	_ASSERT((m_pAgentWnd!=NULL) && (m_pAgentWnd->IsWindow()));

	if((m_pAgentWnd != NULL) && (m_pAgentWnd->IsWindow()))
	{
		m_pAgentWnd->PostMessage(nMsgType, wParam, lParam);
	}
	else
	{
		VOID* p1 = (VOID*)wParam;
		VOID* p2 = (VOID*)lParam;
		if(p1 != NULL)
		{
			delete p1;
		}
		if(p2 != NULL)
		{
			delete p2;
		}
	}
}