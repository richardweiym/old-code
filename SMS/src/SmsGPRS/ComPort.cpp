#include "StdAfx.h"
#include "ComPort.h"
#include "ComPortAgentWnd.h"
#include <process.h>

CGPRSManager* g_pManager = NULL;

CSerialPortEx::CSerialPortEx(CSmsComPortAgentWnd* p, CGPRSManager* pManager)
{
	g_pManager = pManager;
	m_pAgentWnd=p;
	m_ov.Offset=0;
	m_ov.OffsetHigh=0;	
	m_ov.hEvent=NULL;
	m_hThread=NULL;
	m_hShutdownEvent=NULL;
	m_hComm=INVALID_HANDLE_VALUE;
	m_hWriteEvent=NULL;
	m_bRunning=false;
	m_bOpen=false;
	m_nToSend=0;
	::InitializeCriticalSection(&m_csCommunicationSync);
}

CSerialPortEx::~CSerialPortEx(void)
{
	if(m_bRunning)
	{
		StopMonitoring();
	}
	if(m_bOpen)
	{
		releasePort();
	}
	::DeleteCriticalSection(&m_csCommunicationSync);
}

void CSerialPortEx::releasePort()
{
	if(m_ov.hEvent!=NULL)
	{
		CloseHandle(m_ov.hEvent);
		m_ov.hEvent=NULL;
	}
	m_ov.Offset=0;
	m_ov.OffsetHigh=0;
	if(m_hWriteEvent!=NULL)
	{
		CloseHandle(m_hWriteEvent);
		m_hWriteEvent=NULL;
	}
	if(m_hShutdownEvent!=NULL)
	{
		CloseHandle(m_hShutdownEvent);
		m_hShutdownEvent=NULL;
	}

	if(m_hComm!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComm);
		m_hComm=NULL;
	}
	m_bOpen=false;
}
bool CSerialPortEx::initPort(UINT portnr,UINT baud,char parity,UINT databits,UINT stopbits)
{
	char szPort[50]={0};
	char szBaud[50]={0};
	
	releasePort();
	m_ov.hEvent=::CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hWriteEvent=::CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hShutdownEvent=::CreateEvent(NULL,TRUE,FALSE,NULL);	

	m_hEventArray[0]=m_hShutdownEvent;
	m_hEventArray[1]=m_ov.hEvent;
	m_hEventArray[2]=m_hWriteEvent;
	


	EnterCriticalSection(&m_csCommunicationSync);
	sprintf(szPort,"COM%d",portnr);
	sprintf(szBaud,"baud=%d parity=%c data=%d stop=%d",baud,parity,databits,stopbits);
	m_hComm=::CreateFile(szPort,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return false;
	}

	m_CommTimeouts.ReadIntervalTimeout=100;
	m_CommTimeouts.ReadTotalTimeoutConstant=100;
	m_CommTimeouts.ReadTotalTimeoutMultiplier=1;
	m_CommTimeouts.WriteTotalTimeoutConstant=100;
	m_CommTimeouts.WriteTotalTimeoutMultiplier=1;
	if(::SetCommTimeouts(m_hComm,&m_CommTimeouts))
	{
		if(::SetCommMask(m_hComm,EV_RXCHAR))
		{	
			if(::GetCommState(m_hComm,&m_dcb))
			{
				m_dcb.fRtsControl=RTS_CONTROL_ENABLE;
				if(::BuildCommDCB(szBaud,&m_dcb))
				{
					if(::SetCommState(m_hComm,&m_dcb))
						;
					else
						return false;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;	
		}

	}
	else
	{
		return false;
	}
	::PurgeComm(m_hComm,PURGE_RXCLEAR|PURGE_RXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	::LeaveCriticalSection(&m_csCommunicationSync);
	m_bOpen=true;
	return true;
}

UINT CSerialPortEx::CommThread(LPVOID pParam)
{
	CSerialPortEx* port=(CSerialPortEx*)pParam;
	DWORD Event=0;
	DWORD CommEvent=0;
	DWORD dwError=0;
	COMSTAT comstat;
	BOOL bResult=TRUE;
	if(port->m_hComm!=INVALID_HANDLE_VALUE)	::PurgeComm(port->m_hComm,PURGE_RXCLEAR|PURGE_RXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

	for(;;)
	{
		bResult=::WaitCommEvent(port->m_hComm,&Event,&port->m_ov);
		if(!bResult)
		{
			switch(dwError=GetLastError())
			{
			case ERROR_IO_PENDING:
					break;
			case 87:
					break;
			default:
				{
					break;
				}
			}
		}
		else
		{
			bResult=ClearCommError(port->m_hComm,&dwError,&comstat);
			if(comstat.cbInQue==0)
			{
				continue;
			}
		}
		
		Event=WaitForMultipleObjects(3,port->m_hEventArray,FALSE,INFINITE);

		switch(Event)
		{
		case 0:
			{
				port->m_bRunning=false;
				goto end;
			}

		case 1:
			{
				GetCommMask(port->m_hComm,&CommEvent);
				if(CommEvent&EV_RXCHAR)
				{
					 ReceiveChar(port,comstat);
				}
				
				break;
			}
		case 2:
			{
				WriteChar(port);
				break;
			}
		default:
				break;
		}
	}
end :
	_endthreadex(0);
	port->m_bRunning=false;

	return 0;
}
void CSerialPortEx::ReceiveChar(CSerialPortEx* port,COMSTAT& comstat)
{
//	OutputDebugString("start new\n");
//	BYTE* pBuf = new BYTE[2049];
//	OutputDebugString("end new\n");

	EnterCriticalSection(&port->m_csCommunicationSync);

	BOOL bRead=TRUE;
	BOOL bResult=TRUE;
	DWORD dwError=0;
	DWORD BytesRead=0;
	
	DWORD dwToRead;

	bResult=ClearCommError(port->m_hComm,&dwError,&comstat);
	LeaveCriticalSection(&port->m_csCommunicationSync);
	
	dwToRead = comstat.cbInQue;
	if(0 == dwToRead)
	{
		return ;
	}
	
	
	//dwToRead = ((dwToRead > 2048)? 2048: dwToRead);
//	dwToRead = 2048;

//	BYTE* pBuf = new BYTE[dwToRead + 1];
//	BYTE* pBuf = (BYTE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY|HEAP_NO_SERIALIZE, dwToRead);
//	BYTE* pBuf = new BYTE[1024];
	BYTE* pBuf = g_pManager->GetBuffer((UINT&)dwToRead);
	
	if(pBuf != NULL)
	{	
		dwToRead--;

		EnterCriticalSection(&port->m_csCommunicationSync);	

		bResult=ReadFile(port->m_hComm,pBuf,dwToRead,&BytesRead,&port->m_ov);
		if(!bResult)
		{
			switch(dwError=GetLastError())
			{
				case ERROR_IO_PENDING:
					{
						bRead=FALSE;
						break;
					}
				default:
					{
						break;
					}
			}
		}
		else
		{	
			bRead=TRUE;
		}



		if(!bRead)
		{
			bRead=TRUE;
			bResult=GetOverlappedResult(port->m_hComm,&port->m_ov,&BytesRead,TRUE);
		}
	
		pBuf[BytesRead] = 0;

#ifdef DEBUG
		OutputDebugString ("port receive:\n");
		OutputDebugString((char*)pBuf);
#endif			

		//发送消息给代理窗口
		if((port ->m_pAgentWnd != NULL)&&(port->m_pAgentWnd->IsWindow()))
		{
			(port->m_pAgentWnd)->PostMessage(WM_SMS_PORT_RECEIVE, (WPARAM)pBuf, (LPARAM)BytesRead);
		}
	
		LeaveCriticalSection(&port->m_csCommunicationSync);	
	}

}
void CSerialPortEx::WriteChar(CSerialPortEx* port)
{
	BOOL bWrite=TRUE;
	BOOL bResult=TRUE;
	DWORD ByteSend=0;
	
	ResetEvent(port->m_hWriteEvent);

	EnterCriticalSection(&port->m_csCommunicationSync);


	port->m_ov.Offset=0;
	port->m_ov.OffsetHigh=0;
	::PurgeComm(port->m_hComm,PURGE_RXCLEAR|PURGE_RXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	bResult=WriteFile(port->m_hComm,port->m_szWriteBuffer,port->m_nToSend,&ByteSend,&port->m_ov);

	if(!bResult)
	{
		DWORD dwError=GetLastError();
		switch(dwError)
		{
		case ERROR_IO_PENDING:
			{
				ByteSend=0;
				bWrite=FALSE;
				break;
			}
		default:
			break;
		}
	}

	
	if(!bWrite)
	{
		bWrite=TRUE;
		bResult=GetOverlappedResult(port->m_hComm,&port->m_ov,&ByteSend,TRUE);	
	}
	
	LeaveCriticalSection(&port->m_csCommunicationSync);

#ifdef  DEBUG
	OutputDebugString ("port write:\n");
	OutputDebugString((char*)(port->m_szWriteBuffer));
#endif

}
void CSerialPortEx::WriteToPort(BYTE* string,int nLength)
{
	memcpy(m_szWriteBuffer,string,nLength);
	m_szWriteBuffer[nLength] = 0;
	m_nToSend=nLength;
	SetEvent(m_hWriteEvent);
}

typedef unsigned ( __stdcall *start_address )( void * );
bool CSerialPortEx::StartMonitoring()
{
	 StopMonitoring();
	 if(m_bOpen)
	 {
		m_hThread=_beginthreadex(NULL,0,(start_address)CommThread,(void*)this,0,NULL);
		if(m_hThread != 0) 
		{
			m_bRunning=true;
			return true;
		}
	 }
	 return false;
}
void CSerialPortEx::StopMonitoring()
{
	DWORD dwRet;
	if(m_bOpen)
	{
		if(m_bRunning)
		{
			SetEvent(m_hShutdownEvent);
 			dwRet=WaitForSingleObject((HANDLE)m_hThread,2000);
// 			switch(dwRet)
// 			{
// 			case WAIT_OBJECT_0:
// 				break;
// 			case WAIT_TIMEOUT:
// 				TerminateThread(m_hThread,0);
//  				WaitForSingleObject(m_hThread,2000);
// 
// 			break;
// 			default:
// 				NULL;	
// 			}
 			CloseHandle((HANDLE)m_hThread);
			m_hThread=NULL;
			m_bRunning=false;
		}
	}
}