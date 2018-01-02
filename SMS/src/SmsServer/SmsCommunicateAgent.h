#pragma  once 

#include <atlwin.h>
#include "SmsCommunicateBase.h"
#include "../SmsRegister/SmsRegister.h"

class CSmsCommunicateBase;
class CSmsCommunicateAgentWnd: public CWindowImpl<CSmsCommunicateAgentWnd>
{
public:
	DECLARE_WND_CLASS(_T("SmsCommunicateAgentWnd"))
	
	BEGIN_MSG_MAP(CSmsCommunicateAgentWnd)
		MESSAGE_HANDLER(WM_SMS_COMMUNICATE_MESSAGE, OnCommunicateMessage)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()
	
	enum { HEART_BEAT_TERVAL = 60000 };

	CSmsCommunicateAgentWnd(CSmsCommunicateBase* pCommBase)	{m_pCommunicateBase = pCommBase;}
	
	LRESULT OnCommunicateMessage(UINT nMsgType, WPARAM wParam, LPARAM lParam,BOOL& bHandled)
	{
		_ASSERT(m_pCommunicateBase != NULL);
			
		if(m_pCommunicateBase != NULL)
		{
			m_pCommunicateBase->OnCommunicateEvent(*((INT*)lParam), wParam, NULL);
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

	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&)
	{
		if(m_pCommunicateBase->IsRunning())
		{
			m_pCommunicateBase->CheckIsAllConnected();
		}

		static BOOL bCheck = TRUE;
		static INT nCount = 0;
		if(bCheck)
		{
			nCount++;
			if(nCount >= 5)
			{
				nCount = 0;
				bCheck = FALSE;

				if(!CheckIsRegistered())
				{
#ifdef DEBUG
	OutputDebugString(_T("CSmsCommunicateAgentWnd not register and stop!"));
#endif
					HWND hServiceAgent = FindWindow(_T("634BDC03-6A8A-49ca-9374-22E43DCB0DD7"), NULL);
					if(hServiceAgent != NULL)
					{
						::PostMessage(hServiceAgent, WM_CLOSE, NULL,NULL);
					}
				}
			}
		}

		return 0;
	}
	LRESULT OnCreate(UINT,WPARAM, LPARAM, BOOL&)
	{
		SetTimer(1, HEART_BEAT_TERVAL);
		return 0;
	}
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
	{
		KillTimer(1);
		return 0;
	}
private:
	CSmsCommunicateBase* m_pCommunicateBase;
};