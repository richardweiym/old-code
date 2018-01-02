#include "stdAfx.h"
#include "SmsClientImp.h"

CAppModule _Module;

BOOL APIENTRY DllMain( HINSTANCE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			_Module.Init(NULL, hModule);
			break;
			
		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;

		case DLL_PROCESS_DETACH:
			_Module.Term();
			break;

    }
    return TRUE;
}

 CSmsClientImp g_clientImp;

 bool __stdcall connect_to_server(const char* lpszServerIP, int uPort, const char* lpszLogonName, const char* lpszLogonPassword )
 {
	 return g_clientImp.connect(lpszServerIP, uPort, lpszLogonName, lpszLogonPassword);
 }

 bool __stdcall disconnect()
 {
	  return g_clientImp.disconnect();
 }

 bool __stdcall send_message(const char* lpszTel, const char* lpszContent, const char* lpszTag)
 {
	 return g_clientImp.sendMsg(lpszTel, lpszContent, lpszTag);
 }

 void __stdcall set_callback(SMS_CLIENT_CALLBACK p)
 {
	 g_clientImp.setCallback(p);
 }

 int __stdcall get_last_error_code()
 {
	 return g_clientImp.getLastErrorCode();
 }