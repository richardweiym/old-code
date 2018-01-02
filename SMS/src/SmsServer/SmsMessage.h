#pragma  once 
#include <atlmisc.h>

enum Enum_Sms_Message_Type 
{
	REQUEST_DISCONNECT = 0,	 //请求断开
	REQUEST_LOGON = 1,		 //请求登陆	
	REQUEST_SEND = 2,		 //请求发送	
	REQUEST_SENDBACK =3,	 //回复发送是否成功	
	REQUEST_DISPATCH =4,	 //收到短信后分发
	REQUEST_HEART_BEAT = 5	 //心跳包
};

//CSMSMessage中 m_bOK 表示的含义
const INT  SMS_MESSAGE_DISPATCH_RIGHT_FLAG  = 0x00000001;
const INT  SMS_MESSAGE_LOGON_OK_FLAG  = 0x00000002;
const INT  SMS_MESSAGE_ACCOUNT_USED_FLAG = 0x00000004;
const INT  SMS_MESSAGE_SENDBACK_OK_FLAG   = 0x00000008;

//内部消息及短信的载体
class CSMSMessage 
{
public:
	CSMSMessage(GUID& clientId,LPCTSTR sName,LPCTSTR sPwd,Enum_Sms_Message_Type m_eMsgType);
	CSMSMessage(GUID& clientId,LPCTSTR szTel,LPCTSTR szContent,LPCTSTR szTag,Enum_Sms_Message_Type type);
	CSMSMessage(GUID& clientId,Enum_Sms_Message_Type type);
	CSMSMessage(const CSMSMessage& msg);
	CSMSMessage(LPCTSTR szTel,LPCTSTR szContent,Enum_Sms_Message_Type t);

	CSMSMessage(PCOPYDATASTRUCT pCs);
	
	CSMSMessage(LPCTSTR szTel,LPCTSTR szContent,LPCTSTR szTag,Enum_Sms_Message_Type type, INT bOK);
	CSMSMessage(LPCTSTR szTel,LPCTSTR szContent,LPCTSTR szTag,Enum_Sms_Message_Type type);
	~CSMSMessage(){}
	Enum_Sms_Message_Type GetMsgType()
	{
		return m_eMsgType;
	}
	VOID SetTel(CString& str)
	{
		m_str1 = str;
	}
	GUID& GetClientID()
	{
		return m_clientId;
	}

	CString& GetStr1()
	{
		return m_str1;
	}

	CString& GetStr2()
	{
		return m_str2;
	}

	CString& GetStr3()
	{
		return m_str3;
	}

	INT GetResult()
	{
		return m_bOK;
	}
	INT SetResult(INT n)
	{
		INT nOld(m_bOK);
		m_bOK = n;
		return nOld;
	}
	Enum_Sms_Message_Type SetMsgType(Enum_Sms_Message_Type eNew)
	{
		Enum_Sms_Message_Type eOld=m_eMsgType;
		m_eMsgType = eNew;
		return eOld;
	}
private:
	CString m_str1;		
	CString m_str2;
	CString m_str3;
	GUID	m_clientId;
	INT     m_bOK;
	Enum_Sms_Message_Type	m_eMsgType;	
};

class CMessagePackage
{
public:

#pragma pack(push,1)
	struct Head
	{
		INT nLen1;
		INT nLen2;
		INT nLen3;
		INT nResult;
		GUID id;
	};
#pragma  pack(pop)

	CMessagePackage(CSMSMessage* pMsg)
	{
		m_pData = NULL;
		m_nLength = 0;
		Head head;
		head.nLen1 = pMsg->GetStr1().GetLength();
		head.nLen2 = pMsg->GetStr2().GetLength();
		head.nLen3 = pMsg->GetStr3().GetLength();
		head.nResult = pMsg->GetResult();
		head.id = pMsg->GetClientID();

		INT nTotal = sizeof(Head) + head.nLen1 + head.nLen2 + head.nLen3;
		m_nLength = nTotal;

		m_pData = new char[nTotal];
		char* p = m_pData;
		memcpy(p,&head,sizeof(head));
		p += sizeof(head);
		memcpy(p,(LPCTSTR)pMsg->GetStr1(),head.nLen1);
		p += head.nLen1;
		memcpy(p,(LPCTSTR)pMsg->GetStr2(),head.nLen2);
		p += head.nLen2;
		memcpy(p,(LPCTSTR)pMsg->GetStr3(),head.nLen3);
		
	}
	INT GetLength()
	{
		return m_nLength;
	}
	TCHAR* GetData()
	{
		return m_pData;
	}
	~CMessagePackage()
	{
		if(m_pData != NULL)
		{
			delete []m_pData;
			m_pData = NULL;
		}
	}

private:
	INT m_nLength;
	char* m_pData;
};