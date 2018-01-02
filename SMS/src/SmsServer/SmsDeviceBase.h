#pragma once 

#include "smsdeviceexport.h"

class ISmsDevice
{
public:
	virtual ~ISmsDevice() {};

	virtual BOOL InitModule(LPCTSTR szPath) = 0;
	virtual BOOL SetParameters(LPCTSTR szParams) = 0 ;
	virtual BOOL StartWork() = 0;
	virtual BOOL SendMsg(CSMSMessage* pMsg) = 0;
	virtual VOID StopWork() = 0;
	virtual VOID ReleaseModule() = 0;
	virtual VOID RegisterDeviceCallback(SMS_DEVICE_CALLBACK pCallback) = 0;
	virtual INT  GetLastErrorCode() = 0;
	virtual BOOL IsRunning() = 0;
};

class ISMSDeviceSink;

class CSmsDeviceModule:public ISmsDevice
{
public:
	CSmsDeviceModule();
	virtual ~CSmsDeviceModule();

	virtual BOOL InitModule(LPCTSTR szPath);
	virtual BOOL SetParameters(LPCTSTR szParams) = 0;
	virtual BOOL StartWork() = 0;
	virtual BOOL SendMsg(CSMSMessage* pMsg) = 0;
	virtual VOID StopWork() = 0;
	virtual VOID ReleaseModule();
	virtual VOID RegisterDeviceCallback(SMS_DEVICE_CALLBACK pCallback) = 0;
	virtual INT  GetLastErrorCode() = 0;
	virtual BOOL IsRunning() = 0;
	
public:
	VOID OnDeviceEvent(UINT nMsgType,WPARAM wParam,LPARAM lParam);

//protected:
//	virtual BOOL HookExport() = 0;

protected:
//	HMODULE m_hModule;
	ISMSDeviceSink* m_pCallback;
};