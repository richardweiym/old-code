// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Fade_Button.h"

#pragma comment(lib, "duilib.lib")

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
	
	BOOL WINAPI NewInstance(IDUIControl** ppControl);

#ifdef __cplusplus
};
#endif //__cplusplus

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

BOOL WINAPI NewInstance(IDUIControl** ppControl)
{
	*ppControl = new CFadeButton;

	return *ppControl != NULL;
}

