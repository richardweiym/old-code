#include "tscommonfunction.h"

bool CTsCommonFunction::GetCurrentFilePath(LPTSTR pBuffer, int nBufferLen, LPCTSTR lpszName)
{
	_ASSERTE(pBuffer != NULL && nBufferLen > 0 && lpszName != NULL);
	
	bool bRet(FALSE);
	DWORD dwLen = GetModuleFileName(NULL, pBuffer, nBufferLen);
	if(dwLen > 0)
	{
		int nTotal = dwLen + lstrlen(lpszName);
		if(nBufferLen > nTotal)
		{
			TCHAR* p = _tcsrchr(pBuffer, _T('\\'));
			if(p != NULL)
			{
				*++p = _T('\0');
				_tcsncat(pBuffer, lpszName, nBufferLen - dwLen);
				bRet = TRUE;
			}
		}
	}

	return bRet;
}