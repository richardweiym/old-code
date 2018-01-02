#pragma  once 

#include "SmsDeviceBase.h"


// typedef BOOL (__stdcall *GPRSMODEMSTART)();
// typedef VOID (__stdcall *GPRSMODEMSTOP)();
// typedef BOOL (__stdcall *GPRSMODEMSEND)(CSMSMessage* pMsg);
// typedef VOID (__stdcall *GPRSMODEMREGISTERCALLBACK)(SMS_DEVICE_CALLBACK pCallback); 
// typedef INT  (__stdcall *GPRSMODEMERRORCODE)();	
// typedef BOOL (__stdcall *GPRSMODEMSETPATAMS)(LPCTSTR szParameters);
// typedef BOOL (__stdcall *GPRSMODEMISRUNNING)();


class CSmsGPRSDevice:public CSmsDeviceModule
{
public:
	CSmsGPRSDevice(ISMSDeviceSink* pSink);
	virtual ~CSmsGPRSDevice();

	virtual BOOL StartWork();
	virtual BOOL SendMsg(CSMSMessage* pMsg);
	virtual VOID StopWork();
	virtual VOID RegisterDeviceCallback(SMS_DEVICE_CALLBACK pCallback);	
	virtual INT  GetLastErrorCode();
	virtual BOOL SetParameters(LPCTSTR szParameters);
	virtual BOOL IsRunning();

protected:
//	virtual BOOL HookExport();
	
// private:
// 	class CFunPackage
// 	{
// 	public:
// 		GPRSMODEMSTART pStartFun;
// 		GPRSMODEMSTOP  pStopFun;
// 		GPRSMODEMSEND  pSendFun;
// 		GPRSMODEMREGISTERCALLBACK pRegisterCallbackFun;
// 		GPRSMODEMERRORCODE pGetErrorCode;
// 		GPRSMODEMSETPATAMS pSetParameters;
// 		GPRSMODEMISRUNNING pIsRunning;
// 
// 		CFunPackage()
// 		{
// 			pStartFun=NULL;
// 			pStopFun=NULL;
// 			pSendFun=NULL;
// 			pRegisterCallbackFun=NULL;
// 			pGetErrorCode = NULL;
// 			pSetParameters = NULL;
// 			pIsRunning=NULL;
// 		}
// 		BOOL IsAllRealize()
// 		{
// 			return ((pStartFun!=NULL) &&
// 				(pStopFun!=NULL) &&
// 				(pSendFun!=NULL) &&
// 				(pRegisterCallbackFun!=NULL) &&
// 				(pGetErrorCode!=NULL) &&
// 				(pSetParameters!=NULL) && 
// 				(pIsRunning!=NULL));
// 		}
// 	};
// 
// 	CFunPackage* m_pFunPackage;
};

extern CSmsGPRSDevice* g_pDeviceGPRSModem;	
