#include "StdAfx.h"
#include <atlmisc.h>
#include "GPRSMangaer.h"
#include "ComPortAgentWnd.h"
#include "ComPort.h"
#include "../SmsServer/SmsMessage.h"
#include "../SmsServer/SmsDeviceExport.h"
#include "../gprs/MemPool.h"


bool CGPRSManager::checkIsNumberString(const CString& str)
{
	char c;
	bool bRet(true);
	int n=str.GetLength();
	for(int i=0;i<n;i++)
	{
		c=str.GetAt(i);
		if((c!='1')&&(c!='2')&&(c!='3')&&(c!='4')&&(c!='5')
		&&(c!='6')&&(c!='7')&&(c!='8')&&(c!='9')&&(c!='0'))
		{
			bRet=false;
			break;
		}
	}
	return bRet;
}
char* CGPRSManager::EncodeToUnicode(const char* pMsg)
{
	char sBuf[8];//十六进制转换暂存，防止溢出
	int wLen=MultiByteToWideChar(CP_ACP,0,pMsg,-1,NULL,0);
	wchar_t *pWchar=new wchar_t[wLen + 10];
	::MultiByteToWideChar(CP_ACP,0,pMsg,-1,pWchar,wLen);

	int cLen=wLen*sizeof(wchar_t);
	BYTE* pCchar=(BYTE*)pWchar;
	char* pTemp=new char[cLen*2 + 10];//保存转换后的

	int m[2];
	
	int nCur=0;
	for(int i=1;i<cLen-2;i=i+2)
	{
		//需要将高字节和低字节转换成16进制,互换后才能在网络上传送
		//低字节
		memset(sBuf,0,2);
		m[0]=*(pCchar+i);
		itoa(m[0],sBuf,16);
		if(sBuf[1]==0)
		{
			pTemp[nCur++]='0';
			pTemp[nCur++]=sBuf[0];
		}
		else
		{
			pTemp[nCur++]=sBuf[0];
			pTemp[nCur++]=sBuf[1];
		}

		//高字节
		memset(sBuf,0,2);
		m[1]=*(pCchar+i-1);
		itoa(m[1],sBuf,16);
		if(sBuf[1]==0)
		{
			pTemp[nCur++]='0';		
			pTemp[nCur++]=sBuf[0];
		}
		else
		{
			pTemp[nCur++]=sBuf[0];
			pTemp[nCur++]=sBuf[1];
		}
	}
	pTemp[cLen*2-4]=0;//字符串结束符号
	
	delete [] pWchar;
	return pTemp;	
}


CGPRSManager::CGPRSManager() 
{
	m_pCallBack	 = NULL;
	m_pAgentWnd = new CSmsComPortAgentWnd(this);
	m_pPort = new CSerialPortEx(m_pAgentWnd, this);
	m_pCMTICommand = new CCMTICommand();
	m_pCMGLCommand = new CCMGLCommand(0, m_arCMGL);
	m_pMemPool = new CMemPool(2048, 30);
	m_pCurrntCommand = NULL;
	m_nPort = 0;
	m_nError = 0;
	m_bRunning = FALSE;
	m_bPortBusying = FALSE;
	m_bCommandReturn = FALSE;
	m_bNeedFireStop = FALSE;
	m_bCanCheck = FALSE;
}

CGPRSManager::~CGPRSManager()
{
	m_arCMGL.RemoveAll();
	if(m_pMemPool != NULL)
	{
		delete m_pMemPool;
		m_pMemPool = NULL;
	}
	if(m_pCMGLCommand != NULL)
	{
		delete m_pCMGLCommand;
		m_pCMGLCommand = NULL;
	}
	if(m_pAgentWnd != NULL)
	{
		delete m_pAgentWnd;
		m_pAgentWnd = NULL;
	}
	if(m_pPort != NULL)
	{
		delete m_pPort;
		m_pPort = NULL;	
	}
	if(m_pCurrntCommand != NULL)
	{
		delete m_pCurrntCommand;
		m_pCurrntCommand = NULL;
	}
	if(m_pCMTICommand != NULL)
	{
		delete m_pCMTICommand;
		m_pCMTICommand = NULL;
	}
}
VOID CGPRSManager::SetMaxFailCount(INT n)
{
	if(n >= 5)
	{
		m_nMaxFailCount = 5;
	}
	else if(n <= 0)
	{
		m_nMaxFailCount = 0;
	}
	else
	{
		m_nMaxFailCount = n;
	}
}
VOID CGPRSManager::SetClearTag(BOOL b)
{
	m_bClearWhenStart = b;
}
VOID CGPRSManager::SetCallBack(SMS_DEVICE_CALLBACK p)
{
	m_pCallBack = p;
}
VOID CGPRSManager::SetPort(int n)
{
	m_nPort = n;
}
VOID CGPRSManager::SetCenterNum(const char* sNum)
{
	lstrcpyn(m_szCenterNum,sNum, 100);
}
BOOL CGPRSManager::DoATCommand(INT nTimeOut)
{
	DWORD dwStart = GetTickCount();
	DWORD dwEnd;
	//SetPortBusying(TRUE);
	//m_bPortBusying = TRUE;
	m_bCommandReturn = FALSE;
	BOOL bTimeOut(FALSE);

	if(m_pPort->isMonitorRunning())
	{
		m_pPort ->WriteToPort((BYTE*)(m_pCurrntCommand->GetCommand()),m_pCurrntCommand->GetLength());
		MSG msg;	
		
		while(TRUE)
		{
			Sleep(5);
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				DispatchMessage(&msg);
			}

			if(m_bCommandReturn)
			{
				break;
			}
			else
			{
				dwEnd = GetTickCount();
				if((dwEnd - dwStart) > (DWORD)nTimeOut)
				{
					bTimeOut = TRUE;
					break;
				}
			}
		}

	//	m_bPortBusying = FALSE;
	}

	return  !bTimeOut;
}
VOID CGPRSManager::ResetCommand()
{
	if(m_pCurrntCommand != NULL)
	{
		delete m_pCurrntCommand;
		m_pCurrntCommand = NULL;
	}
}

BOOL CGPRSManager::SendMsg(CSMSMessage* pMsg, BOOL bSendAtOnce)
{
	if( !bSendAtOnce)
	{	
		if(m_pAgentWnd->IsWindow())
		{
			CSMSMessage* p = new CSMSMessage(*pMsg);
			m_pAgentWnd->PostMessage(WM_SMS_SEND_MESSAGE_TO_PORT,(WPARAM)p,NULL);
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("SendMsg: pMsg:%d, post tosend msg"), p);
	OutputDebugString(strDebug);
#endif
		}
		else
		{
			delete pMsg;
		}
	}
	else
	{
		//m_bPortBusying = TRUE;
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("start send msg:%d"), pMsg);
	OutputDebugString(strDebug);
#endif
		CString strTel = pMsg->GetStr1();
		strTel.TrimLeft(" ");
		strTel.TrimRight(" ");
		strTel.TrimLeft("+");
		strTel.TrimLeft("86");
		
		//号码不都是数字，直接返回失败
		if(!checkIsNumberString(strTel))
		{
			pMsg->SetResult(pMsg->GetResult() & ~SMS_MESSAGE_SENDBACK_OK_FLAG);
			pMsg->SetMsgType(REQUEST_SENDBACK);

			if(m_pCallBack != NULL)
			{
				m_pCallBack(WM_SMS_DEVICE_SENDED, (WPARAM)pMsg, NULL);
				//m_bPortBusying = FALSE;
				return TRUE;
			}
		}

		BOOL bOK(FALSE);
		char* pString = EncodeToUnicode((LPCTSTR)pMsg->GetStr2());
		INT nTotal = strlen(pString);
		CString sSend;
		char* pStart = pString;
		INT nFailCount(0);
		do
		{
			if(nTotal > 280)
			{
				ResetCommand();

				sSend = CString(pStart,280);
				m_pCurrntCommand = new CCMGSCommand(sSend, strTel);
__SendAgaint1:
				if((!DoATCommand(30000)) || (m_eResult == AT_COMMAND_FAIL))
				{
					nFailCount++;

					if(nFailCount <= m_nMaxFailCount) 
					{
						goto __SendAgaint1;
					}
					else
					{
						break;
					}	
				}

				pStart += 280;
				nTotal -= 280;
				nFailCount = 0;
			}
			else
			{
				ResetCommand();

				sSend = CString(pStart, nTotal);
				m_pCurrntCommand = new CCMGSCommand(sSend,strTel);
__SendAgain2:
				if((!DoATCommand(30000)) || (m_eResult == AT_COMMAND_FAIL))
				{
					nFailCount++;
					if(nFailCount <= m_nMaxFailCount) 
					{
						goto __SendAgain2;
					}
					else
					{
						break;
					}
				}
				else 
				{
					bOK =TRUE;
					break;
				}
			}
		}
		while(nTotal > 0);
		
		if(pString != NULL)
		{
			delete []pString;
			pString = NULL;
		}


		if(bOK)
		{
			pMsg->SetResult(pMsg->GetResult() | SMS_MESSAGE_SENDBACK_OK_FLAG);	
		}
		else
		{
			pMsg->SetResult(pMsg->GetResult() & ~SMS_MESSAGE_SENDBACK_OK_FLAG);
		}

		pMsg->SetMsgType(REQUEST_SENDBACK);
		if(m_pCallBack != NULL)
		{
			m_pCallBack(WM_SMS_DEVICE_SENDED, (WPARAM)pMsg, NULL);
		}
#ifdef DEBUG
	strDebug.Format(_T("end send msg:%d"), pMsg);
	OutputDebugString(strDebug);
#endif
		//m_bPortBusying = FALSE;
	}
	
	return TRUE;
}
VOID CGPRSManager::CheckStoredMsg()
{
	ResetCommand();
	m_pCurrntCommand = new CCMGLCommand(1, m_arCMGL);
	DoATCommand(30000);

	ResetCommand();
	m_pCurrntCommand = new CCMGLCommand(0, m_arCMGL);
	DoATCommand(30000);

	for(int i=0; i< m_arCMGL.GetSize(); i++)
	{
		if(m_pAgentWnd->IsWindow())
		{
			m_pAgentWnd->PostMessage(WM_SMS_READ_MESSAGE_FROM_PORT,(WPARAM)m_arCMGL[i],NULL);
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("CheckStoredMsg: CMGL-%d,Post Read message"), m_arCMGL[i]);
	OutputDebugString(strDebug);
#endif
		}
	}
	m_arCMGL.RemoveAll();
}
VOID CGPRSManager::DoReadMsg(INT nIndex)
{
#ifdef DEBUG
	CString strDebug;
	strDebug.Format(_T("start DoReadMsg:%d"), nIndex);
	OutputDebugString(strDebug);
#endif
	ResetCommand();
	
	static char szContentBuf[1024] = {0};
	static char szTelBuf[100] = {0};

	m_pCurrntCommand = new CCMGRCommand(nIndex, szContentBuf, szTelBuf, 1024,100);
	if(DoATCommand(5000))
	{
		if(m_eResult == AT_COMMAND_OK)
		{
			if(m_pCallBack != NULL)
			{
				CSMSMessage* pMsg = new CSMSMessage(szTelBuf,szContentBuf,REQUEST_DISPATCH);

				m_pCallBack(WM_SMS_DEVICE_RECEIVE,(WPARAM)pMsg,NULL);

				delete pMsg;
			}
			ResetCommand();
			m_pCurrntCommand = new CCMGDCommand(nIndex);
			DoATCommand(10000);			
		}
	}
#ifdef DEBUG
	strDebug.Format(_T("end DoReadMsg:%d"), nIndex);
	OutputDebugString(strDebug);
#endif

	
}
VOID CGPRSManager::OnCommandReturn(const char* pData, INT)
{
	if(IsPortBusying())
	{
		m_eResult = m_pCurrntCommand->ParseResult((char*)pData);
		if(m_eResult != AT_COMMAND_OTHER ) 
		{
			m_bCommandReturn = TRUE;
			return;
		}
	}
	
	SMS_AT_COMMAND_RESULT ret =	m_pCMTICommand->ParseResult((char*)pData);
	if(ret == AT_COMMAND_OK)
	{
		int nIndex = m_pCMTICommand->GetResult();
		if(nIndex >= 0)
		{
			if(m_pAgentWnd->IsWindow())
			{
#ifdef DEBUG
				CString strDebug;
				strDebug.Format(_T("OnCommandReturn: CMTI-%d,Post read message"), nIndex);
				OutputDebugString(strDebug);
#endif
				m_pAgentWnd->PostMessage(WM_SMS_READ_MESSAGE_FROM_PORT,(WPARAM)nIndex,NULL);
			}
			return;
		}
		
	}
	
	m_pCMGLCommand->ParseResult((char*)pData);

	//OutputDebugString("OnCommandReturn ,unexpected result\n");


}
VOID CGPRSManager::FreeBuffer(BYTE* pBuf)
{
	if(m_pMemPool != NULL)
	{
		m_pMemPool->FreeMemory(pBuf);
	}
}
BYTE* CGPRSManager::GetBuffer(UINT& nSize)
{
	if(m_pMemPool != NULL)
	{
		return m_pMemPool->GetMemory(nSize);
	}
	return NULL;
}
BOOL CGPRSManager::Start()
{
	Stop();

	if(m_pAgentWnd != NULL)
	{
		CRect rt(0,0,0,0);
		m_pAgentWnd ->Create(NULL,rt,NULL,WS_POPUP);
		if(!(m_pAgentWnd ->IsWindow()))
		{
			m_nError = SMS_DEVICE_ERROR_AGENT_CREATE;
			goto __Start_end;
		}
	}

	if(m_pPort != NULL)
	{
		if(!(m_pPort->initPort(m_nPort)))
		{
			m_nError = SMS_DEVICE_ERROR_PORT_OPEN;
			goto __Start_end;
		}
		
		if(!(m_pPort->StartMonitoring()))
		{
			m_nError = SMS_DEVICE_ERROR_CREATE_THREAD;
			goto __Start_end;
		}
	}
	
	if(!Init())
	{
		goto __Start_end;
	}
	
	if(m_pCallBack != NULL)
	{
		m_pCallBack(WM_SMS_DEVICE_STARTED,NULL,NULL);
	}
	m_bNeedFireStop = TRUE;
	return TRUE;

__Start_end:
	Stop();
	return FALSE;

}
BOOL CGPRSManager::Stop()
{	
	m_bCanCheck = FALSE;
	if(m_pPort != NULL)
	{
		if(m_pPort->isMonitorRunning())
		{
			m_pPort->StopMonitoring();
		}
		if(m_pPort->isPortOpen())
		{
			m_pPort->releasePort();
		}
	}
	if(m_pAgentWnd != NULL)
	{
		if(m_pAgentWnd ->IsWindow())
		{
			m_pAgentWnd->DestroyWindow();
		}
	}

	if(m_bNeedFireStop)
	{
		if(m_pCallBack != NULL)
		{
			m_pCallBack(WM_SMS_DEVICE_STOPPED,NULL,NULL);
		}
		m_bNeedFireStop = FALSE;
	}
	m_pMemPool->SetAllMemFree();	
	return TRUE;
}
BOOL CGPRSManager::IsRunning()
{
	if(m_pPort != NULL)
	{
		return m_pPort->isMonitorRunning();
	}
	return FALSE;
}

BOOL CGPRSManager::Init()
{
	ResetCommand();
	m_pCurrntCommand = new CCPINCommand();
	SetPortBusying(TRUE);
	BOOL bOK = DoATCommand(5000);
	SetPortBusying(FALSE);
	if(!bOK)
	{
		m_nError = SMS_DEVICE_ERROR_COMMAND_TIMEOUT;
		return FALSE;
	}
	else
	{
		if(m_eResult == AT_COMMAND_FAIL)
		{
			m_nError = SMS_DEVICE_ERROR_CPIN;
			return FALSE;
		}
	}

	ResetCommand();
	m_pCurrntCommand = new CCMEECommand();
	SetPortBusying(TRUE);
	bOK = DoATCommand(5000);
	SetPortBusying(FALSE);
	if(!bOK)
	{
		m_nError = SMS_DEVICE_ERROR_COMMAND_TIMEOUT;
		return FALSE;
	}
	else
	{
		if(m_eResult == AT_COMMAND_FAIL)
		{
			m_nError = SMS_DEVICE_ERROR_CMEE;
			return FALSE;
		}
	}

	ResetCommand();
	m_pCurrntCommand = new CCSCACommand(m_szCenterNum);
	SetPortBusying(TRUE);
	bOK = DoATCommand(5000);
	SetPortBusying(FALSE);
	if(!bOK)
	{
		m_nError = SMS_DEVICE_ERROR_COMMAND_TIMEOUT;
		return FALSE;
	}
	else
	{
		if(m_eResult == AT_COMMAND_FAIL)
		{
			m_nError = SMS_DEVICE_ERROR_CSCA;
			return FALSE;
		}
	}
	
	ResetCommand();
	m_pCurrntCommand = new CCMGFCommand();
	SetPortBusying(TRUE);
	bOK = DoATCommand(5000);
	SetPortBusying(FALSE);
	if(!bOK)
	{
		m_nError = SMS_DEVICE_ERROR_COMMAND_TIMEOUT;
		return FALSE;
	}
	else
	{
		if(m_eResult == AT_COMMAND_FAIL)
		{
			m_nError = SMS_DEVICE_ERROR_CMGF;
			return FALSE;
		}
	}

	if(m_bClearWhenStart)
	{
		ResetCommand();
  		m_pCurrntCommand = new CCMGDALLCommand();
		SetPortBusying(TRUE);
		DoATCommand(60000);
		SetPortBusying(FALSE);
  		
	}


	ResetCommand();
	m_pCurrntCommand = new CCNMICommand();
	SetPortBusying(TRUE);
	bOK = DoATCommand(5000);
	SetPortBusying(FALSE);
	if(!bOK)
	{
		m_nError = SMS_DEVICE_ERROR_COMMAND_TIMEOUT;
		return FALSE;
	}
	else
	{
		if(m_eResult == AT_COMMAND_FAIL)
		{
			m_nError = SMS_DEVICE_ERROR_CNMI;
			return FALSE;
		}
	}
	
	ResetCommand();
	m_pCurrntCommand = new CCSQCommand();
	SetPortBusying(TRUE);
	bOK = DoATCommand(5000);
	SetPortBusying(FALSE);
	if(bOK)
	{
		if(m_eResult == AT_COMMAND_OK)
		{
			if(m_pCallBack != NULL)
			{
				INT nCount = m_pCurrntCommand->GetResult();
				if(nCount != -1)
				{
					m_pCallBack(WM_SMS_DEVICE_SIGNAL_COUNT,(WPARAM)(&nCount),NULL);
				}
			}
		}
	}
	
	m_bCanCheck = TRUE;
	return TRUE;
}