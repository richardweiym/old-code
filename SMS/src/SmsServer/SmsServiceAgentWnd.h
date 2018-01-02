#pragma  once 

#include <atlwin.h>
#include "SmsServiceMgr.h"
#include "SmsControlDef.h"
#include "../SmsRegister/SmsRegister.h"


class CSmsServiceAgentWnd:public CWindowImpl<CSmsServiceAgentWnd>
{
public:
	enum 
	{
		CHECK_IS_CONNECTED_INTERVAL = 60000
	};

	DECLARE_WND_CLASS(_T("634BDC03-6A8A-49ca-9374-22E43DCB0DD7"))

	BEGIN_MSG_MAP(CSmsCommunicateAgentWnd)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		//MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		m_SmsServiceMgr.ProcessControllerRequest(uMsg,wParam,lParam);
	END_MSG_MAP()

	
	CSmsServiceAgentWnd(HWND hWnd):m_SmsServiceMgr(hWnd) 
	{	
		m_nTimer = 0;
		m_bStarting = FALSE;
	}
	LRESULT OnTimer(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if(m_SmsServiceMgr.IsRunning())
		{
			m_SmsServiceMgr.CheckIsAccountUsing();
		}
		
		static INT nCount = 0;
		static BOOL bCheck = TRUE;
		if(bCheck)
		{
			nCount ++;
			if(nCount >= 5)
			{
				if(!CheckIsRegistered())
				{
#ifdef DEBUG
	OutputDebugString(_T("CSmsServiceAgentWnd not register and stop!"));
#endif
					PostMessage(WM_CLOSE, 0, 0);
				}
				nCount = 0;
				bCheck = FALSE;
			}
		}

		return 0;
	}
	LRESULT OnCreate(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_bStarting = TRUE;

		if(!m_SmsServiceMgr.Start())
		{
			DestroyWindow();
		}
		else
		{
			m_nTimer = SetTimer(1, CHECK_IS_CONNECTED_INTERVAL);
		}

		m_bStarting = FALSE;

		return 0;
	}
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		if(!m_bStarting)
		{
			if(m_nTimer != 0)
			{
				KillTimer(1);
				m_nTimer = 0;
			}
			
			m_SmsServiceMgr.Stop();
			
			DestroyWindow();

			PostQuitMessage(0);
		}
		else
		{
			bHandled = TRUE;	
		}


		return 0;
	}
// 	LRESULT OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
// 	{
// 
// 		bHandled = FALSE ;
// 		return 0;
// 	}
private:
	CSmsServiceMgr m_SmsServiceMgr;
	DWORD m_nTimer;
	BOOL m_bStarting;
};