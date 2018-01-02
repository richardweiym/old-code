#include "smsclientinterface.h"
#include "SmsClientBase.h"
#include "smssocketclient.h"


class CSmsClientImp: public ISmsClientSink
{
public:
	virtual VOID OnConnect(BOOL bOK)
	{
		if(m_pCallback != NULL)
		{
			m_pCallback(MESSAGE_TYPE_CONNECT, NULL, NULL, NULL, bOK);
			m_bSending = FALSE;
		}		
	}

	virtual VOID OnSend(CSMSMessage* pMsg)
	{
		if(m_pCallback != NULL)
		{
			BOOL bSendedOK = (pMsg->GetResult() & SMS_MESSAGE_SENDBACK_OK_FLAG)? TRUE: FALSE;
			m_pCallback(MESSAGE_TYPE_SEND_RETURN, pMsg->GetStr1(), pMsg->GetStr2(), pMsg->GetStr3(), bSendedOK);
			m_bSending = FALSE;
		}
	}

	virtual VOID OnReceive(CSMSMessage* pMsg) 
	{
		if(m_pCallback != NULL)
		{
			m_pCallback(MESSAGE_TYPE_RECEIVE, pMsg->GetStr1(), pMsg->GetStr2(), pMsg->GetStr3(), TRUE);
		}
	}

	virtual VOID OnServiceStop() 
	{
		if(m_pCallback != NULL)
		{
			m_pCallback(MESSAGE_TYPE_SERVICE_STOP, NULL, NULL, NULL, TRUE);
			m_bSending = FALSE;
		}
	}
	
	CSmsClientImp()
	{
		m_pCallback = NULL;
		m_pClient = new CSocketClient(this);
		m_bSending = FALSE;
	}

	~CSmsClientImp()
	{
		if(m_pClient != NULL)
		{
			delete m_pClient;
			m_pClient = NULL;
		}
	}

	VOID setCallback(SMS_CLIENT_CALLBACK p) { m_pCallback = p; }

	bool connect(LPCTSTR lpszIP, INT nPort, LPCTSTR lpszName, LPCTSTR lpszPwd)
	{
		if(m_pClient != NULL)
		{
			return m_pClient->Connect(lpszIP, nPort, lpszName, lpszPwd);
		}

		return FALSE;
	}

	bool disconnect()
	{
		m_bSending = FALSE;
		if(m_pClient != NULL)
		{
			return m_pClient->DisConnect();
		}
		

		return false;
	}

	bool sendMsg(LPCTSTR lpszTel, LPCTSTR lpszContent, LPCTSTR lpszTag)
	{
		BOOL bOK(FALSE);
		if(m_pClient != NULL)
		{
			if(!m_bSending)
			{
				CSMSMessage msg(lpszTel, lpszContent, lpszTag, REQUEST_SEND);
				m_bSending = m_pClient->SendMsg(&msg);
				bOK = m_bSending;
			}
			else
			{
				m_pClient->SetLastError(CLIENT_ERROR_BUSY_SENDING);
			}
		}
		
		return bOK;
	}

	int getLastErrorCode()
	{
		if(m_pClient != NULL)
		{
			return m_pClient->GetLastError();
		}
		
		return 0;
	}

private:
	CSocketClient* m_pClient;
	SMS_CLIENT_CALLBACK m_pCallback;
	BOOL m_bSending;
};