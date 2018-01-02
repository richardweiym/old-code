#include "stdafx.h"
#include "SmsSocket.h"
#include "smssocketprotocol.h"
#include "SmsSocketCommunicate.h"
#include "SmsMessage.h"

CSmsSocketCommunicate* CSmsSocket::m_pSocketManager = NULL;

CSmsSocket::CSmsSocket(SOCKET socket,BOOL bClient)
{
	m_socket = socket;
	m_bClient = bClient;
	m_bLogoned = FALSE;
	m_bDispatch = FALSE;
	m_bConnected = TRUE;
	CoCreateGuid(&m_gId);

	unsigned long n(1);
	ioctlsocket (socket,FIONBIO,&n);
}	
CSmsSocket::~CSmsSocket()
{
	
}

VOID CSmsSocket::SetSocketManager(CSmsSocketCommunicate* pManager)
{
	m_pSocketManager = pManager;
}

BOOL CSmsSocket::SocketSend(CSMSMessage* pMsg)
{
	BOOL bOK(FALSE);
	Enum_Sms_Message_Type msgType(pMsg->GetMsgType());
	SMS_SOCKET_HEADER header={0};
	INT nLen1 = pMsg->GetStr1().GetLength();
	INT nLen2 = pMsg ->GetStr2().GetLength();
	INT nLen3 = pMsg ->GetStr3().GetLength();
	char* pBuf = NULL;
	INT nTotal(0);
	INT nResult = pMsg->GetResult();
	char* p;
	
	switch(msgType)
	{
	case REQUEST_LOGON:
		strcpy(header.sHeadTag,SMS_SOCKET_LOGON_TAG);
		m_bLogoned = (nResult & SMS_MESSAGE_LOGON_OK_FLAG) && (!(nResult & SMS_MESSAGE_ACCOUNT_USED_FLAG));
		m_bDispatch = nResult & SMS_MESSAGE_DISPATCH_RIGHT_FLAG;
		break;

	case REQUEST_DISPATCH:
		strcpy(header.sHeadTag,SMS_SOCKET_DISPATCH_TAG);
		break;

	case REQUEST_SENDBACK:
		strcpy(header.sHeadTag,SMS_SOCKET_SENDBACK_TAG);
		break;
	
	case REQUEST_HEART_BEAT:
		strcpy(header.sHeadTag, SMS_SOCKET_HEART_BEAT_TAG);
		break;

	default:
		return FALSE;
	}

#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("server socket send: %d"), msgType);
	OutputDebugString(strDebug);
#endif

	header.nP1Len = nLen1;
	header.nP2Len = nLen2;
	header.nP3Len = nLen3;
	header.bOK = pMsg->GetResult();
	
	nTotal = nLen1+nLen2+nLen3+sizeof(header);

	pBuf = new char[nTotal];
	p = pBuf;
	memcpy(p,&header,sizeof(header));
	p+=sizeof(header);

	if(nLen1 > 0)
	{
		memcpy(p,(LPCTSTR)(pMsg->GetStr1()),nLen1);
		p+=nLen1;
	}

	if(nLen2 > 0)
	{
		memcpy(p,(LPCTSTR)(pMsg->GetStr2()),nLen2);
		p+=nLen2;
	}
	
	if(nLen3 > 0)
	{
		memcpy(p,(LPCTSTR)(pMsg->GetStr3()),nLen3);
	}
	
	bOK = SendWrap(pBuf,nTotal);

	delete []pBuf;	

	return bOK;
}

VOID CSmsSocket::OnClose()
{
	if(m_bClient && m_bLogoned)
	{
		INT* p = new INT(WM_SMS_COMMUNICATE_DISCONNECT);
		CSMSMessage* pMsg = new CSMSMessage(m_gId, REQUEST_DISCONNECT);

		m_pSocketManager->SendMessageToAgent(WM_SMS_COMMUNICATE_MESSAGE, (WPARAM)pMsg, (LPARAM)p);
	}
	closesocket(m_socket);
}

VOID CSmsSocket::OnRecv(SMS_SOCKET_HEADER* header)
{
	ENUM_SMS_SOCKET_PACKAGE_TYPE packageType;
	CSMSMessage* pMsg;
	
	if(lstrcmpi(header->sHeadTag,SMS_SOCKET_LOGON_TAG) == 0)
	{
		packageType = SMSLOGON;
	}
	else if(lstrcmpi(header->sHeadTag,SMS_SOCKET_SEND_TAG) == 0)
	{
		packageType = SMSSEND;
	}
	else if(lstrcmpi(header->sHeadTag, SMS_SOCKET_HEART_BEAT_TAG) == 0)
	{
		packageType = SMSHEARTBEAT;
	}
	else 
	{
		INT* p = new INT(WM_SMS_COMMUNICATE_ERROR);
		INT* pReason = new  INT(SMS_COMMUNICATE_ERROR_UNKNOWNPACKAGE);
		m_pSocketManager->SendMessageToAgent(WM_SMS_COMMUNICATE_MESSAGE,(WPARAM)pReason,(LPARAM)p);
		return ;
	}
	
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("server Socket receive: %d"), packageType);
	OutputDebugString(strDebug);
#endif

	int	nLen1 = header->nP1Len;
	int	nLen2 = header->nP2Len;
	int	nLen3 = header->nP3Len;
	char* p1 = new char[nLen1+1];
	char* p2 = new char[nLen2+1];
	char* p3 = new char[nLen3+1];
	int nReaded;

	if(nLen1 > 0)
	{
		nReaded = ReadWrap(p1, nLen1);
		p1[nLen1] = 0;
		if(nReaded != nLen1)
		{
			goto __REC_END;
		}
	}

	if(nLen2 > 0)
	{
		nReaded = ReadWrap(p2, nLen2);
		p2[nLen2] = 0;
		if(nReaded != nLen2)
		{
			goto __REC_END;
		}
	}

	if(nLen3 > 0)
	{
		nReaded = ReadWrap(p3, nLen3);
		p3[nLen3] = 0;
		if(nReaded != nLen3)
		{
			goto __REC_END;
		}
	}

	if(packageType == SMSLOGON)
	{
		pMsg = new CSMSMessage(m_gId,p1,p2,REQUEST_LOGON);
		INT* p = new INT(WM_SMS_COMMUNICATE_LOGON);
		m_pSocketManager->SendMessageToAgent(WM_SMS_COMMUNICATE_MESSAGE,(WPARAM)pMsg,(LPARAM)p);
	}
	else if(packageType == SMSSEND)
	{
		if(m_bLogoned)
		{
			pMsg = new CSMSMessage(m_gId,p1,p2,p3,REQUEST_SEND);
			INT* p = new INT(WM_SMS_COMMUNICATE_SEND);
			m_pSocketManager->SendMessageToAgent(WM_SMS_COMMUNICATE_MESSAGE,(WPARAM)pMsg,(LPARAM)p);
		}
	}
	else if(packageType == SMSHEARTBEAT)
	{
		m_bConnected = TRUE;
		CSMSMessage msg(_T(""), _T(""), REQUEST_HEART_BEAT);
		SocketSend(&msg);	
	}

__REC_END:
	if(p1 != NULL)
	{
		delete []p1;
	}
	if(p2 != NULL)
	{
		delete []p2;
	}
	if(p3 != NULL)
	{
		delete []p3;
	}
	return ;
}

INT CSmsSocket::SendWrap(char* p,int nLen)
{

	INT nToSend(nLen);
	INT nSended;
	char* pPos = p;
	DWORD nCurrentTime = GetTickCount();
	DWORD nEndTime = nCurrentTime + 2000;	

	while(TRUE)
	{	
		nSended = send(m_socket, pPos, nToSend, 0);
		nToSend -= nSended;
		pPos += nSended;

		if(nToSend > 0)
		{
			Sleep(5);
			nCurrentTime = GetTickCount();
			if(nCurrentTime > nEndTime)
			{
				break;
			}
		}
		else
		{
			break;
		}	
	}
	
	return nLen;
}

INT CSmsSocket::ReadWrap(char* p,int nLen)
{
	char* pPos = p;
	INT nToRead(nLen);
	INT nReaded(0);
	DWORD nCurrentTime = GetTickCount();
	DWORD nEndTime = nCurrentTime + 2000;
	
	while(1)
	{
		nReaded = recv(m_socket,pPos,nToRead,0);
		nToRead -= nReaded;
		pPos += nReaded;

		if(nToRead > 0)
		{
			Sleep(5);
			nCurrentTime = GetTickCount();
			if(nCurrentTime > nEndTime)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	return nLen;
}