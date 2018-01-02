#pragma  once 

#include <atlmisc.h>

class CSmsClientConfig
{
public:
	class CViewConfig
	{
	public:
		CViewConfig(INT nMode, INT nLR, INT nTB1, INT nTB2)
		{
			m_nMode = nMode;
			m_nLR = nLR;
			m_nTB1 = nTB1;
			m_nTB2 = nTB2;
		}
	public:
		INT m_nMode;
		INT m_nLR;
		INT m_nTB1;
		INT m_nTB2;
	};
public:
	BOOL GetWinPos(CRect& rect)
	{
		TCHAR szFile[MAX_PATH] = {0};
		GetConfigFileName(szFile, MAX_PATH);
		rect.left = GetPrivateProfileInt(_T("window"), _T("left"), 0, szFile);
		rect.top = GetPrivateProfileInt(_T("window"), _T("top"), 0, szFile);
		rect.right = GetPrivateProfileInt(_T("window"), _T("right"), 500, szFile);
		rect.bottom = GetPrivateProfileInt(_T("window"), _T("bottom"), 500, szFile);
		return TRUE;
	}
	BOOL SaveWinPos(CRect& rect)
	{
		TCHAR szFile[MAX_PATH] = {0};
		GetConfigFileName(szFile, MAX_PATH);
		
		TCHAR szVal[10] = {0};
		_itot(rect.left, szVal, 10);
		WritePrivateProfileString(_T("window"), _T("left"), szVal, szFile);
		_itot(rect.top, szVal, 10);
		WritePrivateProfileString(_T("window"), _T("top"), szVal, szFile);
		_itot(rect.right, szVal, 10);
		WritePrivateProfileString(_T("window"), _T("right"), szVal, szFile);
		_itot(rect.bottom, szVal, 10);
		WritePrivateProfileString(_T("window"), _T("bottom"), szVal, szFile);
		return TRUE;
	}
	BOOL GetViewConfig(CViewConfig& view)
	{
		TCHAR szFile[MAX_PATH] = {0};
		GetConfigFileName(szFile, MAX_PATH);
		view.m_nMode = 15; // GetPrivateProfileInt(_T("view"), _T("mode"), 3, szFile);
		view.m_nLR = GetPrivateProfileInt(_T("view"), _T("LR"), 150, szFile);
		view.m_nTB1 = GetPrivateProfileInt(_T("view"), _T("TB1"), 200, szFile);
		view.m_nTB2 = GetPrivateProfileInt(_T("view"), _T("TB2"), 300, szFile);
		return TRUE;
	}
	BOOL SaveViewConfig(CViewConfig& view)
	{
		TCHAR szFile[MAX_PATH] = {0};
		GetConfigFileName(szFile, MAX_PATH);
		TCHAR szVal[10] = {0};
		_itot(view.m_nMode, szVal, 10);
		WritePrivateProfileString(_T("view"), _T("mode"), szVal, szFile);
		_itot(view.m_nLR, szVal, 10);
		WritePrivateProfileString(_T("view"), _T("LR"), szVal, szFile);
		_itot(view.m_nTB1, szVal, 10);
		WritePrivateProfileString(_T("view"), _T("TB1"), szVal, szFile);
		_itot(view.m_nTB2, szVal, 10);
		WritePrivateProfileString(_T("view"), _T("TB2"), szVal, szFile);
		return TRUE;
	}
private:
	VOID GetConfigFileName(TCHAR* szPathBuf, INT nLen)
	{
		GetModuleFileName(NULL, szPathBuf, nLen);
		TCHAR* p = _tcsrchr(szPathBuf, '\\');
		*(++p) = '\0';
		lstrcat(szPathBuf, _T("WinPos.ini"));
	}
};