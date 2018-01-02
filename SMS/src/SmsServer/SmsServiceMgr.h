#pragma  once 

#include "SmsEventSink.h"

class CSMSMessage;
class CSmsFactory;
class ISmsDevice;
class ISmsCommunicate;
class ISmsConfig;
class CSmsAccountManager;



class CSmsServiceMgr:public ISMSCommunicateSink,public ISMSDeviceSink
{
public :
	CSmsServiceMgr(HWND hWnd);
	virtual ~CSmsServiceMgr();

	BOOL Start();
	VOID Stop();
	VOID FeedbackMessageToController(UINT nMsgType,WPARAM wParam, LPARAM lParam, BOOL bPost = TRUE); 
	VOID ServiceStatusNotify();
	
	VOID ProcessControllerRequest(UINT, WPARAM, LPARAM);
	VOID CheckIsAccountUsing();
	//ISMSCommunicateSink
	virtual VOID OnCommunicateLogon(CSMSMessage* pMsg);
	virtual VOID OnCommunicateSend(CSMSMessage* pMsg);
	virtual VOID OnCommunicateDisconnect(GUID& clientId);
	virtual VOID OnCommunicateStarted();
	virtual VOID OnCommunicateStopped();
	virtual VOID OnCommunicateError(INT* pReason);

	//ISMSDeviceSink
	virtual VOID OnDeviceReceiveMsg(CSMSMessage* msg);
	virtual VOID OnDeviceSendedMsg(CSMSMessage* msg);
	virtual VOID OnDeviceStarted();
	virtual VOID OnDeviceStopped();
	virtual VOID OnDeviceError(INT* pReason);
	virtual VOID OnDeviceSignalCount(INT* pCount);

	BOOL IsRunning();

private:
	CSmsFactory& m_SmsFactory;
	ISmsDevice*  m_pSmsDevice;
	ISmsCommunicate* m_pSmsCommunicate;
	ISmsConfig*  m_pSmsConfig;
	CSmsAccountManager* m_pSmsAccountManager;
	HWND m_hController;
	INT m_nErrorCode;
//	BOOL m_bStarting;
};