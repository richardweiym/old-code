#pragma  once 

class CSMSMessage;

class ISMSDeviceSink
{
public:
	~ISMSDeviceSink(){}
 
	virtual VOID OnDeviceReceiveMsg(CSMSMessage* msg) = 0;
	virtual VOID OnDeviceSendedMsg(CSMSMessage* msg) = 0;
	virtual VOID OnDeviceStarted() = 0;
	virtual VOID OnDeviceStopped() = 0;
	virtual VOID OnDeviceError(INT* pReason ) = 0;
	virtual VOID OnDeviceSignalCount(INT* pCount) = 0;
};

class ISMSCommunicateSink
{
public:
	virtual VOID OnCommunicateLogon(CSMSMessage* pMsg) = 0;
	virtual VOID OnCommunicateSend(CSMSMessage* pMsg) = 0;
	virtual VOID OnCommunicateDisconnect(GUID& clientId) = 0;
	virtual VOID OnCommunicateStarted() = 0;
	virtual VOID OnCommunicateStopped() = 0;
	virtual VOID OnCommunicateError(INT* pReason) = 0;
};