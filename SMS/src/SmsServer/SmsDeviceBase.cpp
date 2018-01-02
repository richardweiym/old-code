#include "stdafx.h"
#include "SmsDeviceBase.h"
#include "SmsEventSink.h"

CSmsDeviceModule::CSmsDeviceModule()
{
//	m_hModule=NULL;
	m_pCallback=NULL;
}

CSmsDeviceModule::~CSmsDeviceModule()
{
//	ReleaseModule();
}

 BOOL CSmsDeviceModule::InitModule(LPCTSTR szPath)
 {
// 	BOOL bRet(FALSE);
// 
// 	m_hModule=::LoadLibrary(szPath);
// 	if(m_hModule!=NULL)
// 	{
// 		if(HookExport())
// 		{
// 			bRet=TRUE;
// 		}
// 	}
// 	
 	return TRUE;
 }

 VOID CSmsDeviceModule::ReleaseModule()
 {
// 	if(m_hModule!=NULL)
// 	{
// 		StopWork();
// 		::FreeLibrary(m_hModule);
// 		m_hModule=NULL;
// 		m_pCallback=NULL;
// 	}
 }

VOID CSmsDeviceModule::OnDeviceEvent(UINT nMsgType,WPARAM wParam,LPARAM lParam)
{
	if(m_pCallback!=NULL)
	{
		switch(nMsgType)
		{
		case WM_SMS_DEVICE_RECEIVE:
			m_pCallback->OnDeviceReceiveMsg((CSMSMessage*)wParam);
			break;

		case WM_SMS_DEVICE_SENDED:
			m_pCallback->OnDeviceSendedMsg((CSMSMessage	*)wParam);
			break;

		case WM_SMS_DEVICE_STARTED:
			m_pCallback->OnDeviceStarted();
			break;

		case WM_SMS_DEVICE_STOPPED:
			m_pCallback->OnDeviceStopped();
			break;

		case WM_SMS_DEVICE_ERROR:
			m_pCallback->OnDeviceError((INT*)wParam);
			break;
		
		case WM_SMS_DEVICE_SIGNAL_COUNT:
			m_pCallback ->OnDeviceSignalCount((INT*)wParam);
			break;

		default:
			break;
		}
	}
}
