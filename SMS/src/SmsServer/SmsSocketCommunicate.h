#pragma  once 
#include <winsock.h>
#include "SmsCommunicateBase.h"
#include <atlbase.h>
#include "SmsSocketProtocol.h"

class CSmsSocket;

class CSmsLock
{
public:
	CSmsLock(CRITICAL_SECTION& cs):m_cs(cs)
	{
		::EnterCriticalSection(&m_cs);
	}
	~CSmsLock()
	{
		::LeaveCriticalSection(&m_cs);
	}
private:
	CRITICAL_SECTION& m_cs;
};

class CSmsSocketCommunicate:public CSmsCommunicateBase
{
public:
	CSmsSocketCommunicate(ISMSCommunicateSink* pSink);
	virtual ~CSmsSocketCommunicate();

	virtual BOOL SetParameters(LPCTSTR szParams);
	virtual BOOL StartCommunicate();
	virtual BOOL SendMsg(CSMSMessage* pMsg);
	virtual VOID StopCommunicate();
	virtual INT  GetLastErrorCode();
	virtual BOOL IsRunning();
	virtual BOOL CheckIsConnected(GUID& clientId);

	VOID CheckIsAllConnected();
	
protected:
	BOOL AddConnection(CSmsSocket*);
	BOOL RemoveConnection(SOCKET socket);
	VOID OnReceive(SOCKET socket,SMS_SOCKET_HEADER* header);

	static UINT monitor(LPVOID); 
	UINT myWrapMonitor();
	VOID RemoveAll();
	
private:
	CSimpleArray<CSmsSocket*> m_arrSocket;
	fd_set m_fd;
	HANDLE m_hThread;
	INT m_nErrorCode;
	unsigned short m_nPort; 
	BOOL m_bRunning;

	HANDLE m_hStartEvent;
	
	CRITICAL_SECTION m_cs;
};