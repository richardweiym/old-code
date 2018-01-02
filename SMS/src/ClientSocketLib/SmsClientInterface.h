#pragma  once 

class CSMSMessage;

class ISmsClient
{
public:
	virtual	BOOL Connect(LPCTSTR szIP, UINT nPort, LPCTSTR szName, LPCTSTR szPassword) = 0;
	virtual BOOL DisConnect() = 0;
	virtual INT  GetLastError() = 0;
	virtual BOOL IsRunning() = 0;
	virtual BOOL SendMsg(CSMSMessage* pMsg) = 0;

	virtual ~ISmsClient() {}
};

class ISmsClientSink
{
public:
	virtual VOID OnConnect(BOOL bOK) = 0;
	virtual VOID OnSend(CSMSMessage* pMsg) = 0;
	virtual VOID OnReceive(CSMSMessage* pMsg) = 0;
	virtual VOID OnServiceStop() = 0;
};

const INT SMS_CLIENT_ERROR_BASE = 100;
const INT SMS_CLIENT_ERROR_CREATE_SOCKET = SMS_CLIENT_ERROR_BASE + 1;
const INT SMS_CLIENT_ERROR_IP_ADDRESS = SMS_CLIENT_ERROR_BASE + 2;
const INT SMS_CLIENT_ERROR_CONNECT = SMS_CLIENT_ERROR_BASE + 3;
const INT SMS_CLIENT_ERROR_CREATE_THREAD = SMS_CLIENT_ERROR_BASE + 4;
const INT SMS_CLIENT_ERROR_CREATE_AGENT = SMS_CLIENT_ERROR_BASE + 5;
const INT SMS_CLIENT_ERROR_ACCOUNT_USED = SMS_CLIENT_ERROR_BASE + 6;
const INT SMS_CLIENT_ERROR_ACCOUNT_NOTEXIST = SMS_CLIENT_ERROR_BASE + 7;
const INT SMS_CLIENT_ERROR_NOT_CONNECT = SMS_CLIENT_ERROR_BASE + 8;