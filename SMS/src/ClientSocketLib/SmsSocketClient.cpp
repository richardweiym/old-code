#include "stdAfx.h"
#include "SmsSocketClient.h"
#include "SmsClientAgentWnd.h"


CSocketClient::CSocketClient(ISmsClientSink* pSink)
{
	WSADATA wsaData;
	WSAStartup(0x101,&wsaData);

	m_pAgentWnd = new CSmsClientAgentWnd(this);
	m_pClientSink = pSink;
	ResetAllValues();
	m_bConnecting = FALSE;
}
CSocketClient::~CSocketClient()
{
	Stop();
	if(m_pAgentWnd != NULL)
	{
		delete m_pAgentWnd;
		m_pAgentWnd = NULL;
	}
	
	WSACleanup();
}
BOOL CSocketClient::Connect(LPCTSTR szIP, UINT nPort, LPCTSTR szName, LPCTSTR szPassword)
{
	if(!m_bConnecting)
	{
		m_bConnecting = TRUE;
		if(!Start(szIP, nPort))
		{
			m_pClientSink->OnConnect(FALSE);
			m_bConnecting = FALSE;
		}
		
		CSMSMessage msg(szName, szPassword, REQUEST_LOGON);
		SendMsg(&msg);
	}


	return TRUE;
}
BOOL CSocketClient::DisConnect()
{
	if(m_bRunning)
	{
		Stop();
	}
	m_bConnecting = FALSE;
	return TRUE;
}
VOID CSocketClient::SendMessageToAgent(UINT nMsgType, WPARAM wParam, LPARAM lParam)
{
	if(m_pAgentWnd->IsWindow())
	{
		m_pAgentWnd->PostMessage(nMsgType, wParam, lParam);
	}
}
VOID CSocketClient::ResetAllValues()
{
	m_bLogoned = FALSE;
	m_bRunning = FALSE;
	m_hThread = NULL;
	m_nErrorCode = 0;
	m_socket = INVALID_SOCKET;
}
VOID CSocketClient::OnClientEvent(UINT nMsgType, WPARAM wParam, LPARAM lParam)
{
	if(m_pClientSink == NULL) return;

	switch(nMsgType)
	{
	case SMS_CLIENT_SENDED:
		m_pClientSink->OnSend((CSMSMessage*)wParam)	;
		break;

	case SMS_CLIENT_DISCONNECTED:
		DisConnect();
		m_pClientSink->OnServiceStop();	
		break;

	case SMS_CLIENT_LOGON:
		m_pClientSink->OnConnect(*((BOOL*)(wParam)));
		m_bConnecting = FALSE;
		break;

	case SMS_CLIENT_RECEIVED:
		m_pClientSink->OnReceive((CSMSMessage*)wParam);
		break;
	}

	return;
}
BOOL CSocketClient::Start(LPCTSTR szIP, UINT nPort)
{
	if(m_bRunning) 
	{
		Stop();
	}

	unsigned long nnn(1);	
	RECT rect = {0, 0, 0, 0};
	HWND hWnd = m_pAgentWnd->Create(NULL, rect, NULL, WS_POPUP);
	if(hWnd == NULL)
	{
		m_nErrorCode = SMS_CLIENT_ERROR_CREATE_AGENT;
		goto __Start_End;
	}

	m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_socket == INVALID_SOCKET)
	{
		m_nErrorCode = SMS_CLIENT_ERROR_CREATE_SOCKET;
		goto __Start_End;
	}

	unsigned long nIP;	
	nIP=inet_addr(szIP);
	if(nIP==INADDR_NONE)
	{
		m_nErrorCode = SMS_CLIENT_ERROR_IP_ADDRESS;
		goto __Start_End;
	}
	
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons((unsigned short)nPort);
	server.sin_addr.S_un.S_addr=nIP;

	if(::connect(m_socket,(struct sockaddr*)&server,sizeof(server))!=0)
	{	
		m_nErrorCode = SMS_CLIENT_ERROR_CONNECT;
		goto __Start_End;
	}


	ioctlsocket (m_socket,FIONBIO,&nnn);
	m_hThread=CreateThread(NULL,0,LPTHREAD_START_ROUTINE(SocketThread),this,0,NULL);
	if(m_hThread == NULL)
	{
		m_nErrorCode = SMS_CLIENT_ERROR_CREATE_THREAD;
		goto __Start_End;
	}
	
	m_bRunning = TRUE;
	return TRUE;

__Start_End:
	Stop();
	return FALSE;
}
BOOL CSocketClient::Stop()
{
	if(m_hThread != NULL)
	{
		if(m_bRunning)
		{
			TerminateThread(m_hThread, 0);
			WaitForSingleObject(m_hThread, 1000);
		}
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	
	if(m_pAgentWnd->IsWindow())
	{
		m_pAgentWnd->DestroyWindow();
	}

	if(m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket=INVALID_SOCKET;		
	}
	m_bRunning = FALSE;

	return TRUE;
}
UINT CSocketClient::SocketThread(LPVOID p)
{
	CSocketClient* pClient = (CSocketClient*)p;
	pClient->ThreadWrap();

	return 1;
}
BOOL CSocketClient::SendMsg(CSMSMessage* pMsg)
{
	BOOL bOK(FALSE);
	Enum_Sms_Message_Type msgType(pMsg->GetMsgType());
	SMS_SOCKET_HEADER header={0};
	INT nLen1 = pMsg->GetStr1().GetLength();
	INT nLen2 = pMsg ->GetStr2().GetLength();
	INT nLen3 = pMsg ->GetStr3().GetLength();
	char* pBuf = NULL;
	INT nTotal(0);
	char* p;
	
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("client socket send: %d"), msgType);
	OutputDebugString(strDebug);
#endif

	switch(msgType)
	{
	case REQUEST_LOGON:
		if(!m_bRunning)
		{
			return FALSE;
		}
		strcpy(header.sHeadTag,SMS_SOCKET_LOGON_TAG);
		break;

	case REQUEST_SEND:	
		if(!m_bLogoned || !m_bRunning) 
		{
			m_nErrorCode = SMS_CLIENT_ERROR_NOT_CONNECT;
			return FALSE;
		}
		strcpy(header.sHeadTag,SMS_SOCKET_SEND_TAG);
		break;
	case REQUEST_HEART_BEAT:
		if(!m_bRunning)
		{
			return FALSE;
		}
		strcpy(header.sHeadTag, SMS_SOCKET_HEART_BEAT_TAG);
		break;

	default:
		return FALSE;
	}

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
	
	bOK = SocketSendWrap(pBuf, nTotal);

	delete []pBuf;	

	return bOK;
}
VOID CSocketClient::OnSocketReceive(SMS_SOCKET_HEADER* pHeader)
{
	char* pType = pHeader->sHeadTag;
	ENUM_SMS_SOCKET_PACKAGE_TYPE packageType;
	
	if(lstrcmpi(pType, SMS_SOCKET_LOGON_TAG) == 0)
	{
		packageType = SMSLOGON;
	}
	else if(lstrcmpi(pType, SMS_SOCKET_SENDBACK_TAG) == 0)
	{
		packageType = SMSSENDBACK;
	}
	else if(lstrcmpi(pType, SMS_SOCKET_DISPATCH_TAG) == 0)
	{
		packageType = SMSRECEIVE;
	}
	else if(lstrcmpi(pType, SMS_SOCKET_HEART_BEAT_TAG) == 0)
	{
		packageType = SMSHEARTBEAT;
	}
	else
	{
		return;
	}

#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("client socket receive: %d"), packageType);
	OutputDebugString(strDebug);
#endif

	INT nLen1 = pHeader->nP1Len, nLen2 = pHeader->nP2Len, nLen3 = pHeader->nP3Len,nResult = pHeader->bOK;
	char* p1, *p2, *p3;
	p1 = new char[nLen1 + 1];
	p1[nLen1] = 0;
	p2 = new char[nLen2 + 1]; 
	p2[nLen2] = 0;
	p3 = new char[nLen3 + 1];
	p3[nLen3] = 0;

	if( nLen1 > 0)
	{
		SocketRecvWrap(p1, nLen1);
	}
	if(nLen2 > 0)
	{
		SocketRecvWrap(p2, nLen2);
	}
	if(nLen3 > 0)
	{
		SocketRecvWrap(p3, nLen3);
	}
	
	CSMSMessage* pMsg;

	if(packageType == SMSLOGON)
	{
		if(nResult & SMS_MESSAGE_LOGON_OK_FLAG )
		{
			if(nResult & SMS_MESSAGE_ACCOUNT_USED_FLAG)
			{
				m_nErrorCode = SMS_CLIENT_ERROR_ACCOUNT_USED;
				BOOL* pV = new BOOL(FALSE);
				UINT* pType = new UINT(SMS_CLIENT_LOGON);
				SendMessageToAgent(WM_SMS_CLIENT_AGENT, (WPARAM)pV, (LPARAM)pType);
				m_bLogoned = FALSE;
			}
			else
			{
				m_bLogoned = TRUE;
				BOOL* pV = new BOOL(TRUE);
				UINT* pType = new UINT(SMS_CLIENT_LOGON);
				SendMessageToAgent(WM_SMS_CLIENT_AGENT, (WPARAM)pV, (LPARAM)pType);				
			}
		}
		else
		{
			m_nErrorCode = SMS_CLIENT_ERROR_ACCOUNT_NOTEXIST;
			BOOL* pV = new BOOL(FALSE);
			UINT* pType = new UINT(SMS_CLIENT_LOGON);
			SendMessageToAgent(WM_SMS_CLIENT_AGENT, (WPARAM)pV, (LPARAM)pType);
			m_bLogoned = FALSE;
		}
	}
	else if(packageType == SMSSENDBACK)
	{
		pMsg = new CSMSMessage(p1,p2,p3,REQUEST_SENDBACK,nResult);
		UINT* pType = new UINT(SMS_CLIENT_SENDED);
		SendMessageToAgent(WM_SMS_CLIENT_AGENT, (WPARAM)pMsg, (LPARAM)pType);
	}
	else if(packageType == SMSRECEIVE)
	{
		pMsg = new CSMSMessage(p1, p2, REQUEST_DISPATCH);
		UINT* pType = new UINT(SMS_CLIENT_RECEIVED);
		SendMessageToAgent(WM_SMS_CLIENT_AGENT, (WPARAM)pMsg, (LPARAM)pType);
	}
	else if(packageType == SMSHEARTBEAT)
	{
		if(m_pAgentWnd->IsWindow())
		{
			m_pAgentWnd->PostMessage(WM_SMS_HEART_RETURN, NULL, NULL);
		}
	}

	delete []p1;
	p1 = NULL;
	delete []p2;
	p2 = NULL;	
	delete []p3;
	p3 = NULL;

}
VOID CSocketClient::ThreadWrap()
{
	SMS_SOCKET_HEADER header;
	FD_SET fdr={1,m_socket};
	INT nRet;

	while(TRUE)
	{
		nRet=select(0,&fdr,NULL,NULL,NULL);	
		if(nRet!=0)
		{
			nRet=recv(m_socket,(char*)&header,sizeof(header),0);
			if(nRet==sizeof(header))
			{
				OnSocketReceive(&header);
			}
			else if(nRet <= 0)
			{
//				OutputDebugString(_T("+++receive server stop event"));
				FD_CLR(m_socket,&fdr);
				UINT* pType = new UINT(SMS_CLIENT_DISCONNECTED);
				SendMessageToAgent(WM_SMS_CLIENT_AGENT, NULL, (LPARAM)pType);
				Sleep(1000);
			}
		}

	}
	m_bRunning=false;	
}
BOOL CSocketClient::SocketSendWrap(char* p,int nLen)
{
	BOOL bOK(FALSE);
	char* pCurrnt = p;
	INT nToSend(nLen);
	INT nSended;
	DWORD nCurrentTime = GetTickCount();
	DWORD nEndTime = nCurrentTime + 2000;
//	OutputDebugString(_T("**client start socket send"));
	while(TRUE)
	{	
		nSended = send(m_socket,pCurrnt,nToSend,0);
		nToSend -= nSended;
		pCurrnt += nSended;

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
			bOK = TRUE;
			break;
		}	
	}
//	OutputDebugString(_T("**client end socket send"));
	return bOK;
}

BOOL CSocketClient::SocketRecvWrap(char* p,int nLen)
{
	BOOL bOK(FALSE);
	char* pPos = p;
	INT nToRead(nLen);
	INT nReaded(0);
	DWORD nCurrentTime = GetTickCount();
	DWORD nEndTime = nCurrentTime + 2000;
//	OutputDebugString(_T("@@client start socket read"));
	while(TRUE)
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
			bOK = TRUE;
			break;
		}
	}
//	OutputDebugString(_T("@@client end socket read"));
	return bOK;
}

BOOL CSocketClient::SendHeartBeat()
{
	CSMSMessage msg(_T(""), _T(""), REQUEST_HEART_BEAT);
	return SendMsg(&msg);
}