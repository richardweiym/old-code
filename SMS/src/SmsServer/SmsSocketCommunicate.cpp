#include "stdafx.h"
#include "SmsSocketCommunicate.h"
#include "SmsCommonFunction.h"
#include "SmsSocket.h"
#include "SmsSocketProtocol.h"
#include "SmsMessage.h"

CSmsSocketCommunicate::CSmsSocketCommunicate(ISMSCommunicateSink* pSink)
{
	m_pCommuSink = pSink;
	m_nPort = 0;
	m_nErrorCode = 0;
	m_hStartEvent = NULL;
	m_hThread = NULL;
	m_bRunning = FALSE;
	CSmsSocket::SetSocketManager(this);
	::InitializeCriticalSection(&m_cs);
	FD_ZERO(&m_fd);
}

CSmsSocketCommunicate::~CSmsSocketCommunicate()
{
	StopCommunicate();
	::DeleteCriticalSection(&m_cs);
}

BOOL CSmsSocketCommunicate::SetParameters(LPCTSTR szParams)
{
	m_nPort = (unsigned short)CSmsCommonFunctions::GetInt(szParams,_T("port"));
	if(m_nPort != 0)
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
}

BOOL CSmsSocketCommunicate::IsRunning()
{
	return m_bRunning;
}

INT CSmsSocketCommunicate::GetLastErrorCode()
{
	return m_nErrorCode;
}

BOOL CSmsSocketCommunicate::StartCommunicate()
{
	if(!m_bRunning)
	{
		WSADATA wsaData;
		WORD sockVersion=MAKEWORD(0x01, 0x01);
		if(::WSAStartup(sockVersion, &wsaData) != 0)
		{
			m_nErrorCode = SMS_COMMUNICATE_ERROR_LOADWS;
			return FALSE;
		}
	}

	StopCommunicate();

	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if( m_hStartEvent == NULL)
	{
		m_nErrorCode = SMS_COMMUNICATE_ERROR_CREATEEVENT;
		return FALSE;
	}

	m_hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)monitor,this,0,NULL);

	if(m_hThread == NULL)
	{
		m_nErrorCode = SMS_COMMUNICATE_ERROR_CREATETHREAD;
		return FALSE;
	}


	DWORD wRet = WaitForSingleObject(m_hStartEvent,3000);
	if(wRet != WAIT_OBJECT_0)
	{
		return FALSE;
	}

	m_bRunning = TRUE;
	return m_bRunning;
}

VOID CSmsSocketCommunicate::StopCommunicate()
{
	if(m_bRunning)
	{
		if(m_bRunning)
		{
			if(m_hThread != NULL)
			{
				TerminateThread(m_hThread,0);
				WaitForSingleObject(m_hThread,1000);
			}
			//发送已停止消息
			INT* pType=new INT(WM_SMS_COMMUNICATE_STOP);
			SendMessageToAgent(WM_SMS_COMMUNICATE_MESSAGE,0,(LPARAM)pType);
		}
	}

	if(m_hThread != NULL)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}


	if(m_hStartEvent != NULL)
	{
		CloseHandle(m_hStartEvent);
		m_hStartEvent = NULL;
	}

	if(m_bRunning)
	{
		WSACleanup();
	}

	RemoveAll();

	FD_ZERO(&m_fd);
	m_nErrorCode = 0;
	m_bRunning = FALSE;

	return;	
}

BOOL CSmsSocketCommunicate::AddConnection(CSmsSocket* p)
{
	BOOL bRet(FALSE);
	{
		CSmsLock lock(m_cs);	
		bRet = m_arrSocket.Add(p);
		if(bRet)
		{
			FD_SET(p->m_socket,&m_fd);
		}
	}
	
	return bRet;
}

BOOL CSmsSocketCommunicate::RemoveConnection(SOCKET s)
{
	CSmsSocket* pSocket;
	BOOL bFound(FALSE);
	
	{
		CSmsLock lock(m_cs);
		for(int i = 0; i < m_arrSocket.m_nSize; i++)
		{
			pSocket = m_arrSocket[i];

			if(pSocket->m_socket == s)
			{
				bFound = TRUE;
				break;
			}
		}

		if(bFound)
		{
			pSocket->OnClose();
			delete pSocket;
			m_arrSocket.Remove(pSocket);
			FD_CLR(s,&m_fd);
		}
	}

	return bFound;
}

VOID CSmsSocketCommunicate::RemoveAll()
{
	fd_set fd = m_fd;
	for(unsigned int i=0;i<fd.fd_count;i++)
	{
		RemoveConnection(fd.fd_array[i]);
	}	
}

UINT CSmsSocketCommunicate::monitor(LPVOID p)
{
	CSmsSocketCommunicate* pSms=(CSmsSocketCommunicate*)p;
	pSms->myWrapMonitor();

	pSms->m_bRunning=FALSE;
	return 0;
} 

BOOL CSmsSocketCommunicate::SendMsg(CSMSMessage* pMsg)
{
	Enum_Sms_Message_Type msgType(pMsg->GetMsgType());
	CSmsSocket* pSocket;
	int i;
	BOOL bFound(FALSE);
	INT  nCount(0);
	{
		CSmsLock lock(m_cs);
		switch(msgType)
		{
		case REQUEST_LOGON:
		case REQUEST_SENDBACK:
			for(i=0; i<m_arrSocket.GetSize(); i++)
			{
				pSocket = m_arrSocket[i];
				if(pMsg->GetClientID() == pSocket->m_gId)
				{
					bFound = TRUE;
					break;
				}
			}

			if(bFound)
			{
				pSocket->SocketSend(pMsg);
			}

			break;
		
		case REQUEST_DISPATCH:
			for(i=0; i<m_arrSocket.GetSize();i++)
			{
				pSocket = m_arrSocket[i];

				if((pSocket->m_bClient) && (pSocket->m_bDispatch) && (pSocket->m_bLogoned))
				{
					pSocket->SocketSend(pMsg);
					nCount++;
				}
			}
			break;

		default:
			break;
		}
	}

	if((msgType == REQUEST_DISPATCH) && (nCount <= 0))
	{
		return FALSE;
	}
	
	return TRUE;
}

UINT CSmsSocketCommunicate::myWrapMonitor()
{
	unsigned short nPort=m_nPort;
	SMS_SOCKET_HEADER header={0};
	sockaddr_in addrRemote;
	INT nAddrLen=sizeof(addrRemote);
	INT nHeaderLen=sizeof(header);
	INT nConCount(0);
	fd_set fdRead;

	SOCKET sListen=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sListen==INVALID_SOCKET) 
	{
		m_nErrorCode = SMS_COMMUNICATE_ERROR_CREATESOCKET;
		return 1;
	}

	sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(nPort);
	sin.sin_addr.s_addr=INADDR_ANY;

	if(::bind(sListen,(sockaddr*)&sin,sizeof(sin))!=0)
	{
		closesocket(sListen);
		m_nErrorCode = SMS_COMMUNICATE_ERROR_BIND;
		return 1;
	}

	if(listen(sListen,10)!=0)
	{
		closesocket(sListen);
		m_nErrorCode = SMS_COMMUNICATE_ERROR_LISTEN;
		return 1;
	}
	
	FD_ZERO(&m_fd);
	CSmsSocket* pSocket=new CSmsSocket(sListen,FALSE);
	AddConnection(pSocket);
	
	//发送已启动消息
	SetEvent(m_hStartEvent);
	INT* p = new INT(WM_SMS_COMMUNICATE_START);
	SendMessageToAgent(WM_SMS_COMMUNICATE_MESSAGE,0,(LPARAM)p);
	
	while(TRUE)
	{
		fdRead=m_fd;
		int nRet=::select(0,&fdRead,NULL,NULL,NULL);
		if(nRet>0)
		{
			for(int i=0;i<(int)m_fd.fd_count;i++)
			{
				if(FD_ISSET(m_fd.fd_array[i],&fdRead))
				{
					if(m_fd.fd_array[i]==sListen)
					{
						if(m_fd.fd_count<FD_SETSIZE)
						{
							SOCKET sNew=::accept(sListen,(sockaddr*)& addrRemote,&nAddrLen);
							CSmsSocket* pSocket=new CSmsSocket(sNew,TRUE);
							AddConnection(pSocket);
						}
					}
					else
					{
						int nRecv=::recv(m_fd.fd_array[i],(char*)&header,nHeaderLen,0);
						if(nRecv>0)
						{
							if(nRecv==nHeaderLen)
							{
								OnReceive(m_fd.fd_array[i],&header);
							}
							else
							{
								//发送错误消息
								INT* pType = new INT(WM_SMS_COMMUNICATE_ERROR);
								INT* pReason = new INT(SMS_COMMUNICATE_ERROR_READ);
								SendMessageToAgent(WM_SMS_COMMUNICATE_MESSAGE,(WPARAM)pReason,(LPARAM)pType);
							}
						}
						else
						{
							RemoveConnection(m_fd.fd_array[i]);
						}
						
					}
				}
			}
		}
	}

	return 0;	
}

VOID CSmsSocketCommunicate::OnReceive(SOCKET socket,SMS_SOCKET_HEADER* header)
{
	CSmsSocket* pSocket;
	BOOL bFound(FALSE);

	{	
		CSmsLock lock(m_cs);
		for(int i = 0; i < m_arrSocket.m_nSize; i++)
		{
			pSocket = m_arrSocket[i];
			if(pSocket->m_socket == socket)
			{
				bFound = TRUE;
				break;
			}
		}
	}

	if(bFound)
	{
		pSocket->OnRecv(header);
	}

	return;
}

VOID  CSmsSocketCommunicate::CheckIsAllConnected()
{
#ifdef DEBUG
		OutputDebugString(_T("server CheckIsAllConnected"));
#endif
	CSmsSocket* pSocket;
	if(m_bRunning)
	{
		CSmsLock lock(m_cs);
		for(int i = 0; i < m_arrSocket.m_nSize; i++)
		{
			pSocket = m_arrSocket[i];
			if(pSocket->m_bClient )
			{
				if(pSocket->m_bConnected)
				{
					pSocket->m_bConnected = FALSE;
				}
				else
				{
#ifdef DEBUG
					OutputDebugString(_T("server remove timeout connection"));
#endif
					RemoveConnection(pSocket->m_socket);	
				}
			}
		}
	}
}

BOOL CSmsSocketCommunicate::CheckIsConnected(GUID& clientId)
{
	BOOL bFind(FALSE);
	CSmsSocket* pSocket;
	if(m_bRunning)
	{
		CSmsLock lock(m_cs);
		for(int i = 0; i < m_arrSocket.m_nSize; i++)
		{
			pSocket = m_arrSocket[i];
			if((pSocket != NULL) && (pSocket->m_bClient) && (pSocket->m_gId == clientId))
			{
				bFind = TRUE;
				break;
			}
		}
	}

	return bFind;
}