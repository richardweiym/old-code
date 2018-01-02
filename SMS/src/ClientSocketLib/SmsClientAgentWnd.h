#pragma  once 

#include <atlwin.h>
#include "smssocketclient.h"

const INT   WM_SMS_CLIENT_AGENT = WM_USER + 100;
const INT   WM_SMS_HEART_RETURN = WM_USER + 110;

const INT SMS_CLIENT_SENDED = 1;
const INT SMS_CLIENT_RECEIVED = 2;
const INT SMS_CLIENT_DISCONNECTED = 3;
const INT SMS_CLIENT_LOGON = 4;

class CSmsClientAgentWnd: public CWindowImpl<CSmsClientAgentWnd>
{
public:
	DECLARE_WND_CLASS(_T("SmsClientAgentWnd"))
	
	enum 
	{
		HEART_BEAT_INTERVAL = 30000
	};
		
	BEGIN_MSG_MAP(CSmsClientAgentWnd)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SMS_CLIENT_AGENT, OnSocketMessage)	
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SMS_HEART_RETURN, OnHeartBeatReturn)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()

	CSmsClientAgentWnd(CSocketClient* pCommBase)	{m_pManager = pCommBase;}
	
	LRESULT OnSocketMessage(UINT nMsgType, WPARAM wParam, LPARAM lParam,BOOL& bHandled)
	{		
		if(m_pManager != NULL)
		{
			m_pManager->OnClientEvent(*((UINT*)lParam), wParam, NULL);
		}
	
		//发送给代理窗口的消息参数内容是new生成的(因为消息是PostMessage发过来的),这里回调处理后将它delete.
		if(wParam != NULL)
		{
			VOID* p1 =(VOID*) wParam;
			delete p1;
			wParam = NULL;
		}

		if(lParam != NULL)
		{
			VOID* p2 = (VOID*)lParam;
			delete p2;
			lParam = NULL;
		}
		return 0;
	}

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
	{
		m_bReturned = TRUE;
		SetTimer(1, HEART_BEAT_INTERVAL);
		return 0;
	}

	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
	{
		KillTimer(1);
		return 0;
	}

	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&)
	{
		if(m_pManager->IsRunning())
		{
			if(m_bReturned)
			{
				BOOL bRet = m_pManager->SendHeartBeat();
				if(bRet)
				{
					m_bReturned = FALSE;
				}
				else
				{
					if(m_pManager->IsLogoned())
					{
//						OutputDebugString("!!!heart beat stop connect");
						m_pManager->OnClientEvent(SMS_CLIENT_DISCONNECTED, NULL, NULL);	
					}
					
				}				
			}
			else
			{	
				if(m_pManager->IsLogoned())
				{
//					OutputDebugString("!!!heart beat stop connect");
					m_pManager->OnClientEvent(SMS_CLIENT_DISCONNECTED, NULL, NULL);
				}
			}
		}

		return 0;
	}
	LRESULT OnHeartBeatReturn(UINT, WPARAM, LPARAM, BOOL&)
	{
		m_bReturned = TRUE;
		return 0;
	}

private:
	CSocketClient* m_pManager;
	BOOL m_bReturned;
};