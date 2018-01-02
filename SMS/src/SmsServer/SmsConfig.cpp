#include "stdafx.h"
#include "SmsConfig.h"

#define  SMS_CONFIG_FILE_NAME "SmsConfig.ini"

INT CSmsIniConfig::GetUseValueFromIni(LPCTSTR szSection,LPCTSTR szKey,LPTSTR szBuf,INT nBufLen)
{
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(GetModuleHandle(NULL),szPath,MAX_PATH-1);
	TCHAR* p=_tcsrchr(szPath,'\\');
	*(++p) = 0;
	lstrcat(szPath, SMS_CONFIG_FILE_NAME);
	INT n = GetPrivateProfileInt(szSection,"UseIndex",0,szPath);

	TCHAR szKeyBuf[MAX_PATH] = {0};
	lstrcpy(szKeyBuf,szKey);
	TCHAR s[20]={0};
	_itot(n,s,10);
	lstrcat(szKeyBuf,s);

	return GetPrivateProfileString(szSection,szKeyBuf,_T(""),szBuf,nBufLen,szPath);
	
}

INT CSmsIniConfig::SetUserValueToIni(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(GetModuleHandle(NULL),szPath,MAX_PATH-1);
	TCHAR* p=_tcsrchr(szPath,'\\');
	*(++p) = 0;
	lstrcat(szPath, SMS_CONFIG_FILE_NAME);
	INT n = GetPrivateProfileInt(szSection,"UseIndex",0,szPath);

	TCHAR szKeyBuf[MAX_PATH] = {0};
	lstrcpy(szKeyBuf,szKey);
	TCHAR s[20]={0};
	_itot(n,s,10);
	lstrcat(szKeyBuf,s);
	return WritePrivateProfileString(szSection, szKeyBuf, szValue, szPath);
}
INT CSmsIniConfig::GetUseCommunicateParameters(LPTSTR szBuf,INT nBufLen)
{
	return GetUseValueFromIni(_T("Communicate"),_T("Parameters"),szBuf,nBufLen);
}

INT CSmsIniConfig::GetUseCommunicateTag(LPTSTR szBuf,INT nBufLen)
{
	return GetUseValueFromIni(_T("Communicate"),_T("Tag"),szBuf,nBufLen);
}

INT CSmsIniConfig::GetUseDeviceParameters(LPTSTR szBuf,INT nBufLen)
{
	return GetUseValueFromIni(_T("Device"),_T("Parameters"),szBuf,nBufLen);
}
INT CSmsIniConfig::GetUseDeviceTag(LPTSTR szBuf,INT nBufLen)
{
	return GetUseValueFromIni(_T("Device"),_T("Tag"),szBuf,nBufLen);
}

INT CSmsIniConfig::GetUseDeviceDllName(LPTSTR szBuf, INT nBufLen)
{
	return GetUseValueFromIni(_T("Device"),_T("DllName"),szBuf,nBufLen); 
}
INT CSmsIniConfig::SetUserCommunicateParameters(LPCTSTR str)
{
	return SetUserValueToIni(_T("Communicate"),_T("Parameters"),str);
}
INT CSmsIniConfig::SetUserDeviceParameters(LPCTSTR str)
{
	return SetUserValueToIni(_T("Device"),_T("Parameters"),str);
}