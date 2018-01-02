#pragma  once 

const INT WM_SMS_COMMUNICATE_MESSAGE = WM_USER + 200 ;					//自定义通信消息	

const INT WM_SMS_COMMUNICATE_BASE = 200;
const INT WM_SMS_COMMUNICATE_LOGON = WM_SMS_COMMUNICATE_BASE + 1;		//收到登陆消息
const INT WM_SMS_COMMUNICATE_SEND  = WM_SMS_COMMUNICATE_BASE + 2;		//收到待发短信
const INT WM_SMS_COMMUNICATE_DISCONNECT  = WM_SMS_COMMUNICATE_BASE + 3;	//客户端断开连接 	 	
const INT WM_SMS_COMMUNICATE_STOP = WM_SMS_COMMUNICATE_BASE + 4;		//通信停止
const INT WM_SMS_COMMUNICATE_START = WM_SMS_COMMUNICATE_BASE + 5;		//通信启动
const INT WM_SMS_COMMUNICATE_ERROR = WM_SMS_COMMUNICATE_BASE + 6;		//通信异常
 

const INT   SMS_COMMUNICATE_ERROR_BASE = 300;
const INT   SMS_COMMUNICATE_ERROR_LOADWS = SMS_COMMUNICATE_ERROR_BASE + 1;
const INT   SMS_COMMUNICATE_ERROR_CREATETHREAD = SMS_COMMUNICATE_ERROR_BASE + 2;
const INT   SMS_COMMUNICATE_ERROR_CREATEEVENT = SMS_COMMUNICATE_ERROR_BASE + 3;
const INT   SMS_COMMUNICATE_ERROR_CREATESOCKET = SMS_COMMUNICATE_ERROR_BASE + 4;
const INT   SMS_COMMUNICATE_ERROR_BIND = SMS_COMMUNICATE_ERROR_BASE + 5;
const INT   SMS_COMMUNICATE_ERROR_LISTEN = SMS_COMMUNICATE_ERROR_BASE + 6;
const INT   SMS_COMMUNICATE_ERROR_READ = SMS_COMMUNICATE_ERROR_BASE + 7; 
const INT   SMS_COMMUNICATE_ERROR_WRITE = SMS_COMMUNICATE_ERROR_BASE + 8;
const INT   SMS_COMMUNICATE_ERROR_UNKNOWNPACKAGE = SMS_COMMUNICATE_ERROR_BASE + 9;

const INT  SMS_ACCOUNT_ERROR_LOAD = 400;
const INT  SMS_NOT_ENOUGH_MEMORY = 401;
const INT  SMS_INCORRECT_TAG = 402;
const INT  SMS_INCORRECT_DEVICE_DLL = 403;
const INT  SMS_INCORRECT_DEVICE_PARAM = 404;
const INT  SMS_INCORRECT_COMMU_PARAM = 405;	  



class CSMSMessage;

class ISmsCommunicate
{
public:
	virtual ~ISmsCommunicate() {}

	virtual BOOL InitModule() = 0;
	virtual VOID ReleaseModule() = 0;

	virtual BOOL SetParameters(LPCTSTR szParams) = 0;
	virtual BOOL StartCommunicate() = 0;
	virtual BOOL SendMsg(CSMSMessage* pMsg) = 0;
	virtual VOID StopCommunicate() = 0;
	virtual INT  GetLastErrorCode() = 0;
	virtual BOOL IsRunning() = 0;
	virtual BOOL CheckIsConnected(GUID& clientId) = 0;
};

class ISMSCommunicateSink;
class CSmsCommunicateAgentWnd;

class CSmsCommunicateBase:public ISmsCommunicate
{
public:
	CSmsCommunicateBase();
	virtual ~CSmsCommunicateBase();

	virtual BOOL InitModule();
	virtual VOID ReleaseModule();

	virtual BOOL SetParameters(LPCTSTR szParams) = 0;
	virtual BOOL StartCommunicate() = 0;
	virtual BOOL SendMsg(CSMSMessage* pMsg) = 0;
	virtual VOID StopCommunicate() = 0;
	virtual INT  GetLastErrorCode() = 0;
	virtual BOOL IsRunning() = 0;
	virtual VOID CheckIsAllConnected() = 0;
	virtual BOOL CheckIsConnected(GUID& clientId) = 0;

	VOID SendMessageToAgent(UINT nMsgType, WPARAM wParam, LPARAM lParam);
protected:
	friend class CSmsCommunicateAgentWnd;
	VOID OnCommunicateEvent(UINT nMsgType,WPARAM wParam, LPARAM lParam);

protected:
	ISMSCommunicateSink* m_pCommuSink;
	CSmsCommunicateAgentWnd* m_pAgentWnd;
};

