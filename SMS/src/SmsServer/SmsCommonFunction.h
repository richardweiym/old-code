#pragma  once 

#include <atlmisc.h>

class CSmsCommonFunctions
{
public:
	static BOOL GetString(LPCTSTR szSource, LPCTSTR szTag, LPTSTR szBuf, INT nBufLen)
	{
		CString  s1(szSource);
		INT nLen = s1.GetLength();
		INT nTagLen = lstrlen(szTag)+2;
		INT nStart = 0;
		BOOL bFound (FALSE);
		

		TCHAR* szTagBuf = new TCHAR[nTagLen]; 
		lstrcpy(szTagBuf,szTag);
		lstrcat(szTagBuf,_T("="));

		nStart = s1.Find(szTagBuf,nStart);
		if(nStart != -1)
		{
			nStart += (nTagLen - 1);
			INT nEnd = 	s1.Find(_T(";"),nStart);
			INT nDataLen(0);
			if(nEnd == -1)
			{	
				nDataLen = nLen - nStart;
			}
			else
			{
				nDataLen = nEnd - nStart;
			}

			if(nDataLen > 0)
			{
				TCHAR* p = (LPTSTR)(LPCTSTR)s1+nStart;
				if(nBufLen>nDataLen)
				{
					memcpy(szBuf,p,sizeof(TCHAR)*nDataLen);
					szBuf[nDataLen] = 0;
					bFound = TRUE;
				}
			}
			
		}
		delete []szTagBuf;
		szTagBuf = NULL;
		return bFound;
	}
	static INT  GetInt(LPCTSTR szSource, LPCTSTR szTag)
	{
		TCHAR sBuf[20] = {0};
		BOOL b = GetString(szSource,szTag,sBuf,20);
		if(b)
		{
			return _ttol(sBuf);
		}
		return 0;
	}
private:
	
};