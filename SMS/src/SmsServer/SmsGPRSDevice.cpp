#include "stdafx.h"
#include "SmsGPRSDevice.h"


CSmsGPRSDevice* g_pDeviceGPRSModem = NULL;

VOID __stdcall DeviceGprsCallback(UINT nMsgType,WPARAM wParam,LPARAM lParam)
{
	g_pDeviceGPRSModem->OnDeviceEvent(nMsgType,wParam,lParam);
}

CSmsGPRSDevice::CSmsGPRSDevice(ISMSDeviceSink* pSink)
{
//	m_pFunPackage=new CFunPackage();
	RegisterDeviceCallback(DeviceGprsCallback);
	g_pDeviceGPRSModem= this;
	m_pCallback = pSink;
}

CSmsGPRSDevice::~CSmsGPRSDevice()
{
// 	ReleaseModule();
// 	delete m_pFunPackage;
// 	m_pFunPackage=NULL;
}
// BOOL CSmsGPRSDevice::HookExport()
// {
// 	BOOL bRet(FALSE);
// 	if(m_hModule!=NULL)
// 	{
// 		m_pFunPackage->pSendFun=(GPRSMODEMSEND)::GetProcAddress(m_hModule,"GPRSSend");
// 		m_pFunPackage->pStartFun=(GPRSMODEMSTART)::GetProcAddress(m_hModule,"GPRSStart");
// 		m_pFunPackage->pStopFun=(GPRSMODEMSTOP)::GetProcAddress(m_hModule,"GPRSStop");
// 		m_pFunPackage->pRegisterCallbackFun=(GPRSMODEMREGISTERCALLBACK)::GetProcAddress(m_hModule,"GPRSRegisterCallback");
// 		m_pFunPackage->pGetErrorCode=(GPRSMODEMERRORCODE)::GetProcAddress(m_hModule,"GPRSGetLastErrorCode");
// 		m_pFunPackage->pSetParameters=(GPRSMODEMSETPATAMS)::GetProcAddress(m_hModule,"GPRSSetParameters");
// 		m_pFunPackage->pIsRunning=(GPRSMODEMISRUNNING)::GetProcAddress(m_hModule,"GPRSIsRunning");	
// 	}
// 	if(m_pFunPackage->IsAllRealize())
// 	{
// 		RegisterDeviceCallback(DeviceGprsCallback);
// 		bRet=TRUE;
// 	}
// 	return bRet;
// }

BOOL CSmsGPRSDevice::StartWork()
{
// 	BOOL bRet(FALSE);
// 	if((m_pFunPackage!=NULL)&&(m_pFunPackage->pStartFun!=NULL))
// 	{
// 		bRet=m_pFunPackage->pStartFun();
// 	}
// 	return bRet;
	return GPRSStart();
}

VOID CSmsGPRSDevice::StopWork()
{
// 	if((m_pFunPackage!=NULL)&&(m_pFunPackage->pStopFun!=NULL))
// 	{
// 		m_pFunPackage->pStopFun();
// 	}
	 GPRSStop();
}

BOOL CSmsGPRSDevice::SendMsg(CSMSMessage* pMsg)
{
// 	BOOL bRet(FALSE);
// 	if((m_pFunPackage!=NULL) && (m_pFunPackage->pSendFun!=NULL))
// 	{
// 		bRet=m_pFunPackage->pSendFun(pMsg);				
// 	}
// 	return bRet;
	return GPRSSend(pMsg);
}

VOID CSmsGPRSDevice::RegisterDeviceCallback(SMS_DEVICE_CALLBACK pCallback)
{
// 	if((m_pFunPackage!=NULL) && (m_pFunPackage->pRegisterCallbackFun!=NULL))
// 	{
// 		m_pFunPackage->pRegisterCallbackFun(pCallback);
// 	}
	GPRSRegisterCallback(pCallback);
}

INT CSmsGPRSDevice::GetLastErrorCode()
{
// 	if((m_pFunPackage!=NULL) && (m_pFunPackage->pGetErrorCode!=NULL))
// 	{
// 		return m_pFunPackage->pGetErrorCode();
// 	}
// 	return -1;
	return GPRSGetLastErrorCode();
}

BOOL CSmsGPRSDevice::SetParameters(LPCTSTR szParameters)
{
// 	BOOL bRet(FALSE);
// 	if((m_pFunPackage!=NULL) && (m_pFunPackage->pSetParameters!=NULL))
// 	{
// 		bRet=m_pFunPackage->pSetParameters(szParameters);
// 	}
// 	return bRet;
	return GPRSSetParameters(szParameters);
}

BOOL CSmsGPRSDevice::IsRunning()
{
// 	BOOL bRet=FALSE;
// 	if((m_pFunPackage!=NULL) && (m_pFunPackage->pIsRunning!=NULL))
// 	{
// 		bRet=m_pFunPackage->pIsRunning();
// 	}
// 	return bRet;
	return GPRSIsRunning();
}