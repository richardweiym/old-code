#pragma  once 

#include "SmsClientInterface.h"
#include "../SmsServer/SmsSocketProtocol.h"

class CSmsClientAgentWnd;

class CSocketClient : public ISmsClient
{
public:
	CSocketClient(ISmsClientSink* pSink);
	~CSocketClient();

	virtual	BOOL Connect(LPCTSTR szIP, UINT nPort, LPCTSTR szName, LPCTSTR szPassword) ;
	virtual BOOL DisConnect();
	virtual INT  GetLastError(){return m_nErrorCode;}
	virtual BOOL IsRunning() { return m_bRunning;}
	virtual BOOL SendMsg(CSMSMessage* pMsg);
	
	BOOL SendHeartBeat();
	VOID OnClientEvent(UINT nMsgType, WPARAM wParam, LPARAM lParam);
	VOID SetLastError(DWORD dwErrCode ) { m_nErrorCode = dwErrCode;}
private:
	static UINT SocketThread(LPVOID p);
	VOID ThreadWrap();
	VOID OnSocketReceive(SMS_SOCKET_HEADER* pHeader);
	BOOL Start(LPCTSTR szIP, UINT nPort);
	BOOL Stop();
	VOID ResetAllValues();
	
	VOID SendMessageToAgent(UINT nMsgType, WPARAM wParam, LPARAM lParam);
public:
	BOOL SocketSendWrap(char* p ,INT nLen);
	BOOL SocketRecvWrap(char* pBuf, INT nLen);
	BOOL IsLogoned() {return m_bLogoned;}
private:
	BOOL m_bConnecting;
	HANDLE m_hThread;
	ISmsClientSink* m_pClientSink;
	INT m_nErrorCode;
	BOOL m_bRunning;
	SOCKET m_socket;
	BOOL m_bLogoned;
	CSmsClientAgentWnd* m_pAgentWnd;
};