#include "stdafx.h"
#include "SmsMessage.h"
#include "SmsControlDef.h"

CSMSMessage::CSMSMessage(GUID& clientId,LPCTSTR sName,LPCTSTR sPwd,Enum_Sms_Message_Type type)
:m_str1(sName),m_str2(sPwd),m_clientId(clientId),m_eMsgType(type)
{
	m_bOK = 0;
}

CSMSMessage::CSMSMessage(GUID& clientId,LPCTSTR szTel,LPCTSTR szContent,LPCTSTR szTag,Enum_Sms_Message_Type type)
:m_clientId(clientId),m_str1(szTel),m_str2(szContent),m_str3(szTag),m_eMsgType(type)
{
	m_bOK = 0;
}

CSMSMessage::CSMSMessage(GUID& clientId,Enum_Sms_Message_Type type)
:m_clientId(clientId),m_eMsgType(type)
{
	m_bOK = 0;
}

CSMSMessage::CSMSMessage(const CSMSMessage& msg)
:m_str1(msg.m_str1),m_str2(msg.m_str2),m_str3(msg.m_str3),m_clientId(msg.m_clientId)
{
	m_bOK = msg.m_bOK;
	m_eMsgType=msg.m_eMsgType;	
}

CSMSMessage::CSMSMessage(LPCTSTR szTel,LPCTSTR szContent,Enum_Sms_Message_Type t)
:m_str1(szTel),m_str2(szContent),m_str3(""),m_eMsgType(t)
{
	m_bOK = 0;
}
// CSMSMessage::CSMSMessage(PCOPYDATASTRUCT pCs)
// {
// 	if(pCs->dwData == SMS_ON_CONNECT)
// 	{
// 		m_eMsgType = REQUEST_LOGON;
// 	}
// 	else if(pCs->dwData == SMS_ON_DISCONNECT)
// 	{
// 		m_eMsgType = REQUEST_DISCONNECT;
// 	}
// 
// 	CMessagePackage::Head head;
// 	INT nTotal = pCs->cbData;
// 	if(nTotal > 0)
// 	{
// 		char* p =(char*)(pCs->lpData);
// 		memcpy(&head,p,sizeof(head));
// 		p+=sizeof(head);
// 
// 		INT nLen1 = head.nLen1;
// 		INT nLen2 = head.nLen2;
// 		INT nLen3 = head.nLen3;
// 		m_bOK = head.nResult;
// 		m_clientId = head.id;
// 	
// 		char* pBuf = new char[nTotal];
// 		if(nLen1>0)
// 		{
// 			memcpy(pBuf,p,nLen1);
// 			pBuf[nLen1] = 0;
// 			m_str1 = pBuf;
// 			p+=nLen1;
// 		}
// 		else
// 		{
// 			m_str1 = "";
// 		}
// 
// 		if(nLen2>0)
// 		{
// 			memcpy(pBuf,p,nLen2);
// 			pBuf[nLen2] = 0;
// 			m_str2 = pBuf;
// 			p+=nLen2;
// 		}
// 		else
// 		{
// 			m_str2 = "";
// 		}
// 		
// 		if(nLen3>0)
// 		{
// 			memcpy(pBuf,p,nLen3);
// 			pBuf[nLen3] = 0;
// 			m_str3 = pBuf;
// 			p+=nLen3;
// 		}
// 		else
// 		{
// 			m_str3 = "";
// 		}
// 	}
// }
CSMSMessage::CSMSMessage(LPCTSTR szTel,LPCTSTR szContent,LPCTSTR szTag,Enum_Sms_Message_Type type, INT bOK)
:m_str1(szTel),m_str2(szContent),m_str3(szTag)
{
	m_bOK = bOK;
	m_eMsgType = type;
}
CSMSMessage::CSMSMessage(LPCTSTR szTel,LPCTSTR szContent,LPCTSTR szTag,Enum_Sms_Message_Type type)
:m_str1(szTel),m_str2(szContent),m_str3(szTag)
{
	m_eMsgType = type;
}