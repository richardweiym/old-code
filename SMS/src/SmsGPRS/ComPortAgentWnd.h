#pragma  once 

#include "GPRSMangaer.h"
#include "../SmsRegister/SmsRegister.h"

const INT WM_SMS_PORT_RECEIVE = WM_USER + 100;
const INT  WM_SMS_READ_MESSAGE_FROM_PORT = WM_USER + 101;
const INT  WM_SMS_SEND_MESSAGE_TO_PORT	= WM_USER +102;

class CSmsComPortAgentWnd: public CWindowImpl<CSmsComPortAgentWnd>
{
public:
	BEGIN_MSG_MAP(CSmsComPortAgentWnd)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SMS_PORT_RECEIVE,OnPortReceive)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_SMS_READ_MESSAGE_FROM_PORT,OnRead);
		MESSAGE_HANDLER(WM_SMS_SEND_MESSAGE_TO_PORT,OnSend);
	END_MSG_MAP()

	CSmsComPortAgentWnd(CGPRSManager* p)
	{
		m_pManager = p;
		m_nTimer = 0;
		m_nTimeCount = 0;
		m_bCheck = TRUE;
		m_nCount = 0;
	}
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
	{
		m_nTimeCount = 0;
		m_nTimer = SetTimer(1, 200);

		m_nCount = 0;
		m_bCheck = TRUE;

		return 0;
	}
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
	{
		if(m_nTimer != 0)
		{
			KillTimer(1);
			m_nTimer = 0;
		}

		INT i;

		CSMSMessage* pMsg = NULL;
		for(i=0; i<m_arToSend.GetSize(); i++)
		{
			pMsg = m_arToSend[i];
			if(pMsg != NULL)
			{
				delete (VOID*)pMsg;
			}
		}
		m_arToSend.RemoveAll();

		m_arReceive.RemoveAll();

		return 0;
	}
	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&)
	{
		if(m_pManager != NULL && m_pManager->IsRunning() && m_pManager->IsCanCheck())
		{
			if(!m_pManager->IsPortBusying())
			{	
				
				while(m_arReceive.GetSize() > 0)
				{
					if(!m_pManager->IsPortBusying())
					{
						m_pManager->SetPortBusying(TRUE);
						m_pManager->DoReadMsg(m_arReceive[0]);
						m_pManager->SetPortBusying(FALSE);
						
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("onTimer read msg:%d"), m_arReceive[0]);
	OutputDebugString(strDebug);
#endif
						m_arReceive.RemoveAt(0);
					}

				}

				CSMSMessage* pMsg = NULL;

				while(m_arToSend.GetSize() > 0)
				{
					pMsg = m_arToSend[0];
					if(pMsg != NULL && !m_pManager->IsPortBusying())
					{
						m_pManager->SetPortBusying(TRUE);
						m_pManager->SendMsg(pMsg, TRUE);
						m_pManager->SetPortBusying(FALSE);
						delete (VOID*)pMsg;
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("onTimer send msg:%d"), m_arToSend[0]);
	OutputDebugString(strDebug);
#endif					
					}
					m_arToSend.RemoveAt(0);
				}
	


				if(m_nTimeCount >= 150 && !m_pManager->IsPortBusying())
				{
					m_nTimeCount = 0;
					m_pManager->SetPortBusying(TRUE);
					m_pManager->CheckStoredMsg();
					m_pManager->SetPortBusying(FALSE);
				}
				else
				{
					m_nTimeCount++;
				}

			}
		}

		//ÊÇ·ñ×¢²á²éÑ¯
		if(m_bCheck)
		{
			m_nCount++;
			if(m_nCount >= 1500)
			{
				m_bCheck = FALSE;
				m_nCount = 0;

				if(!CheckIsRegistered())
				{
#ifdef DEBUG
	OutputDebugString(_T("CSmsComPortAgentWnd not register and stop!"));
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
	LRESULT OnRead(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& )
	{
		if(m_pManager != NULL)
		{
			if((!m_pManager->IsPortBusying()) && (m_arReceive.GetSize() <= 0))
			{
				m_pManager->SetPortBusying(TRUE);
				m_pManager->DoReadMsg((INT)wParam);
				m_pManager->SetPortBusying(FALSE);
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("onRead read msg:%d"), wParam);
	OutputDebugString(strDebug);
#endif
			}
			else
			{
				BOOL bFind(FALSE);
				for(int i=0; i<m_arReceive.GetSize(); i++)
				{
					if(m_arReceive[i] == wParam)
					{
						bFind = TRUE;
						break;
					}
				}
				if(!bFind)
				{
					m_arReceive.Add((UINT)wParam);	
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("onRead add to receive array:%d"), wParam);
	OutputDebugString(strDebug);
#endif
				}		
			}
		}
		return 0;
	}
	LRESULT OnSend(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		if(m_pManager != NULL)
		{
			if(!m_pManager->IsPortBusying() && (m_arToSend.GetSize() <= 0))
			{
				m_pManager->SetPortBusying(TRUE);
				m_pManager->SendMsg((CSMSMessage*) wParam, TRUE);
				m_pManager->SetPortBusying(FALSE);
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("onsend msg:%d"), wParam);
	OutputDebugString(strDebug);
#endif
				VOID* p1 = (VOID*) lParam;
				VOID* p2 = (VOID*) wParam;
				if(p1 != NULL) delete p1;
				if(p2 != NULL) delete p2;
			}
			else
			{	
				CSMSMessage* pMsg = (CSMSMessage*)wParam;
				m_arToSend.Add(pMsg);
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("onSend add to tosend array:%d"), wParam);
	OutputDebugString(strDebug);
#endif
			}
		}
		return 0;
	}
	LRESULT OnPortReceive(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& )
	{	
		if(m_pManager != NULL)
		{
			m_pManager->OnCommandReturn((const char*)wParam,(INT)lParam);
		}

		BYTE* p1 = (BYTE*) wParam;
		//if(p1 != NULL) delete []p1;
 		if(p1 != NULL)
		{
			m_pManager->FreeBuffer(p1);
			//HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, p1);
 		}


		return 0;
	}

private:
	BOOL m_bCheck;
	INT m_nCount;

	INT m_nTimeCount;
	CSimpleArray<UINT> m_arReceive;
	CSimpleArray<CSMSMessage*> m_arToSend;
	DWORD m_nTimer;
	CGPRSManager* m_pManager;
};