// SmsGPRS.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "GPRSMangaer.h"
#include "../SmsServer/SmsDeviceExport.h"
#include "../SmsServer/SmsCommonFunction.h"

CAppModule _Module;
CGPRSManager g_GPRS;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			_Module.Init(NULL,(HINSTANCE)hModule,NULL);
			break;
		case DLL_PROCESS_DETACH:
			_Module.Term();
			break;
	}

	return TRUE;
}


BOOL __stdcall GPRSSetParameters(LPCTSTR szParams)
{
	INT nPort = CSmsCommonFunctions::GetInt(szParams,"com");
	if(nPort == 0) return FALSE;
	g_GPRS.SetPort(nPort);

	char sNum[100] = {0};
	BOOL b = CSmsCommonFunctions::GetString(szParams,"SmsCenter",sNum,99);
	if(!b) return FALSE;
	g_GPRS.SetCenterNum(sNum);
	
	INT nMaxCount = CSmsCommonFunctions::GetInt(szParams, "MaxFailCount");
	g_GPRS.SetMaxFailCount(nMaxCount);

	INT bClear = CSmsCommonFunctions::GetInt(szParams, "ClearTag"); 
	BOOL bVal = ((bClear==0)? FALSE: TRUE);
	g_GPRS.SetClearTag(bVal);

	return TRUE;
}
BOOL __stdcall GPRSStart()
{
	return g_GPRS.Start();
}
BOOL __stdcall  GPRSSend(CSMSMessage* msg)
{
	return g_GPRS.SendMsg(msg);
}
VOID __stdcall GPRSStop()
{
	 g_GPRS.Stop();
}
VOID __stdcall GPRSRegisterCallback(SMS_DEVICE_CALLBACK pCallback)
{
	g_GPRS.SetCallBack(pCallback);
}
INT  __stdcall GPRSGetLastErrorCode()
{
	return g_GPRS.GetLastErrorCode();
}
BOOL __stdcall GPRSIsRunning()
{
	//return TRUE;
	return g_GPRS.IsRunning();
}