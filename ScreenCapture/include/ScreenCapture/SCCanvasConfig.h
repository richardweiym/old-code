#pragma  once 

#include "HYUtilBase.h"
#include "HYUIBase.h"
#include "HYAutoPtr.h"

#define SC_CONFIG_NAME _T("SCConfig.ini")

// config data from ini file
class CSCCanvasConfig: public CRefCountBase
{
public:
	CSCCanvasConfig()
	{
		m_dwSkinType = m_nShowTipFlag = m_nDoubleClickExit = -1;
		ZeroMemory(m_szDefaultPath, sizeof(m_szDefaultPath));

		ZeroMemory(m_szFileEditor, sizeof(m_szFileEditor));
		lstrcpyn(m_szFileEditor, _T("mspaint.exe"), sizeof(m_szFileEditor) / sizeof(TCHAR) - 1);
	}

	BOOL LoadConfig()
	{
		TCHAR szFile[MAX_PATH] = { 0 };
		if(!GetConfigFileName(szFile, sizeof(szFile) / sizeof(TCHAR) - 1)) return FALSE;
		if(!PathFileExists(szFile)) return FALSE;
		
		m_nLanType = GetPrivateProfileInt(_T("system"), _T("lanType"), -1, szFile);
		m_dwSkinType = GetPrivateProfileInt(_T("system"), _T("skinType"), -1, szFile);
		m_nShowTipFlag = GetPrivateProfileInt(_T("system"), _T("showTip"), -1, szFile);
		m_nShowCursor = GetPrivateProfileInt(_T("system"), _T("showCursor"), -1, szFile);
		m_nDoubleClickExit = GetPrivateProfileInt(_T("system"), _T("doubleClickExit"), -1, szFile);
		m_nTransparentMark = GetPrivateProfileInt(_T("system"), _T("transparentMark"), -1, szFile);
		m_nFileFilterIndex = GetPrivateProfileInt(_T("system"), _T("fileFilterIndex"), -1, szFile);
		m_nExitAfterImageSaved = GetPrivateProfileInt(_T("system"), _T("exitAfterImageSaved"), -1, szFile);
		GetPrivateProfileString(_T("system"), _T("defaultPath"), _T(""), m_szDefaultPath, sizeof(m_szDefaultPath) / sizeof(TCHAR), szFile);
		GetPrivateProfileString(_T("system"), _T("imageEditor"), _T(""), m_szFileEditor, sizeof(m_szFileEditor) / sizeof(TCHAR), szFile);
		if(lstrlen(m_szFileEditor) <= 0)
		{
			lstrcpyn(m_szFileEditor, _T("mspaint.exe"), sizeof(m_szFileEditor) / sizeof(TCHAR) - 1);
		}

		return TRUE;
	}

	BOOL SaveConfig()
	{
		TCHAR szFile[MAX_PATH] = { 0 };
		if(!GetConfigFileName(szFile, sizeof(szFile) / sizeof(TCHAR) - 1)) return FALSE;
		
		TCHAR szBuffer[32] = {0};
		_ltot(m_nLanType, szBuffer, 10);
		WritePrivateProfileString(_T("system"), _T("lanType"), szBuffer, szFile);

		_ltot(m_dwSkinType, szBuffer, 10);
		WritePrivateProfileString(_T("system"), _T("skinType"), szBuffer, szFile);

		_ltot(m_nShowTipFlag, szBuffer, 10);
		WritePrivateProfileString(_T("system"), _T("showTip"), szBuffer, szFile);

		_ltot(m_nShowCursor, szBuffer, 10);
		WritePrivateProfileString(_T("system"), _T("showCursor"), szBuffer, szFile);

		_ltot(m_nDoubleClickExit, szBuffer, 10);
		WritePrivateProfileString(_T("system"), _T("doubleClickExit"), szBuffer, szFile);

		_ltot(m_nTransparentMark, szBuffer, 10);
		WritePrivateProfileString(_T("system"), _T("transparentMark"), szBuffer, szFile);

		_ltot(m_nFileFilterIndex, szBuffer, 10);
		WritePrivateProfileString(_T("system"), _T("fileFilterIndex"), szBuffer, szFile);

		_ltot(m_nExitAfterImageSaved, szBuffer, 10);
		WritePrivateProfileString(_T("system"), _T("exitAfterImageSaved"), szBuffer, szFile);
		
		WritePrivateProfileString(_T("system"), _T("defaultPath"), m_szDefaultPath, szFile);

		WritePrivateProfileString(_T("system"), _T("imageEditor"), m_szFileEditor, szFile);

		return TRUE;
	}

	INT GetSkinType() const
	{
		return m_dwSkinType;
	}

	VOID SetSkinType(INT dwType)
	{
		m_dwSkinType = dwType;
	}

	INT GetLanType() const
	{
		return m_nLanType;
	}

	VOID SetLanType(INT nType)	
	{
		m_nLanType = nType;
	}

	INT GetShowHelpTipFlag() const
	{
		return m_nShowTipFlag;
	}
	VOID SetShowHelpTipFlag(INT nVal)
	{
		m_nShowTipFlag = nVal;
	}

	INT GetShowCursor() const
	{
		return m_nShowCursor;
	}

	VOID SetShowCursor(INT nVal)
	{
		m_nShowCursor = nVal;
	}

	INT GetDoubleClickExit() const
	{
		return m_nDoubleClickExit;
	}

	VOID SetDoubleClickExit(INT nVal)
	{
		m_nDoubleClickExit = nVal;
	}

	INT GetTransparentMark() const 
	{
		return m_nTransparentMark;
	}

	VOID SetTransparentMark(INT nVal)
	{
		m_nTransparentMark = nVal;
	}

	INT GetFileFilterIndex()
	{
		return m_nFileFilterIndex;
	}

	VOID SetFileFilterIndex(INT nVal)
	{
		m_nFileFilterIndex = nVal;
	}

	INT GetExitAfterImageSaved()
	{
		return m_nExitAfterImageSaved;
	}

	VOID SetExitAfterImageSaved(INT nVal)
	{
		m_nExitAfterImageSaved = nVal; 
	}

	BOOL GetDefaultSavePath(LPTSTR lpszBuffer, INT nBufferLen)
	{
		if(lstrlen(m_szDefaultPath) > 0)
		{
			lstrcpyn(lpszBuffer, m_szDefaultPath, nBufferLen - 1);
			return TRUE;
		}

		return FALSE;
	}

	VOID SetDefaultSavePath(LPCTSTR lpszPath)
	{
		_ASSERTE(lpszPath != NULL);
		lstrcpyn(m_szDefaultPath, lpszPath, sizeof(m_szDefaultPath) / sizeof(TCHAR) - 1);
	}

	BOOL GetFileEditor(LPTSTR lpszBuffer, INT nBufferLen)
	{
		if(lstrlen(m_szFileEditor) > 0)
		{
			lstrcpyn(lpszBuffer, m_szFileEditor, nBufferLen - 1);
			return TRUE;
		}
		
		return FALSE;
	}

	VOID SetFileEditor(LPCTSTR lpszFileName)
	{
		_ASSERTE(lpszFileName != NULL);
		lstrcpyn(m_szFileEditor, lpszFileName, sizeof(m_szFileEditor) / sizeof(TCHAR) - 1);
	}

protected:
	BOOL GetConfigFileName(LPTSTR lpszBuffer, INT nBufferLen)
	{
		return CHYCommonFunctionWrapper::GetCurrentFilePath(lpszBuffer, nBufferLen, SC_CONFIG_NAME);
	}

private:
	INT m_nLanType;
	INT m_dwSkinType;
	INT m_nShowTipFlag;
	INT m_nShowCursor;
	INT m_nDoubleClickExit;
	INT m_nTransparentMark;
	INT m_nFileFilterIndex;
	INT m_nExitAfterImageSaved;
	TCHAR m_szDefaultPath[MAX_PATH];
	TCHAR m_szFileEditor[MAX_PATH];
};