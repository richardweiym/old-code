#pragma  once 

#include "../SmsServer/SmsConfig.h"
#include "../SmsServer/SmsCommonFunction.h"

class CConsoleConfig
{
public:
	CConsoleConfig()
	{
		m_pConfig = new CSmsIniConfig();
	}
	~CConsoleConfig()
	{
		if(m_pConfig != NULL)
		{
			delete m_pConfig;
		}
	}
	BOOL GetDeviceInfo(CString& strCenterNum,UINT& nComPort, INT& nMaxCount, BOOL& bClear)
	{
		BOOL bOK(FALSE);
		TCHAR szBuf[MAX_PATH] = {0};
		TCHAR szNum[100] = {0};
		if(m_pConfig->GetUseDeviceParameters(szBuf,MAX_PATH) > 0)
		{
			if(CSmsCommonFunctions::GetString(szBuf,_T("SmsCenter"),szNum,100))
			{
				strCenterNum = szNum;	
				nComPort = CSmsCommonFunctions::GetInt(szBuf, _T("com"));
				bOK = TRUE;
			}
			nMaxCount = CSmsCommonFunctions::GetInt(szBuf, "MaxFailCount");
			INT nVal = CSmsCommonFunctions::GetInt(szBuf, "ClearTag");
			bClear = ((nVal == 0)? FALSE: TRUE);
		}
		return bOK;
	}
	BOOL SetDeviceInfo(CString& strCenterNum,UINT& nComPort, INT nCount, BOOL bClear)
	{
		TCHAR szBuf[MAX_PATH] = {0};
		INT nVal =((bClear)? 1 : 0);

		if(nCount>=6)
		{
			nCount=5;
		}
		else if(nCount<=0)
		{
			nCount = 0;
		}

		_sntprintf(szBuf, MAX_PATH, _T("com=%u;SmsCenter=%s;MaxFailCount=%u;ClearTag=%u;"),nComPort,strCenterNum,nCount,nVal);	
		m_pConfig->SetUserDeviceParameters(szBuf);

		return TRUE;
	}
	BOOL GetSocketPort(UINT& nPort)
	{
		BOOL bOK(FALSE);
		TCHAR szBuf[MAX_PATH] = {0};
		if(m_pConfig->GetUseCommunicateParameters(szBuf,MAX_PATH) > 0)
		{
			nPort = CSmsCommonFunctions::GetInt(szBuf, _T("port"));
			bOK = TRUE;
		}

		return bOK;
	}
	BOOL SetSocketPort(UINT nPort)
	{
		TCHAR szBuf[MAX_PATH] = {0};

		_sntprintf(szBuf, MAX_PATH, _T("port=%u;"),nPort);
		m_pConfig->SetUserCommunicateParameters(szBuf);
		
		return TRUE;
	}
private:
	ISmsConfig* m_pConfig;
};