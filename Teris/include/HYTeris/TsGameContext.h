#pragma  once

#include "TsStdAfx.h"

#define TS_RES_DLL_NAME _T("HYTerisRes.dll")
#define TS_CONFIG_FILE_NAME _T("TsConfig.ini")

enum TS_SWITCH_DIRECTION
{
	CLOCK_WISE = 1,
	ANTI_CLOCK_WISE = 2
};

enum TS_USE_WIN_TYPE
{
	USE_LEFT_WIN = 1,
	USE_RIGHT_WIN = 2
};

enum TS_USE_COLOR_TYPE
{
	USE_COLOR_TYPE = 1,
	USE_BLACK_TYPE = 2
};

class CTsControlKeys
{
public:
	int m_nMoveLeft;
	int m_nMoveRight;
	int m_nMoveSwitch;
	int m_nDropDown;
	int m_nMoveDown;
	TS_SWITCH_DIRECTION m_nSwitchDirection;
};

class CTsKeysTable;
class CTsDataFactory;
class CTsGameBaseInfo;
class CTsPlayProcessInfo;
class CTsUserListData;
class CTsGameContext
{
public:
	CTsGameContext();
	~CTsGameContext();

	bool Initialize();
	void UnInitialize();

	bool LoadFromConfigFile();
	bool SaveToConfigFile();
	
	CTsControlKeys* GetLeftWinControlKeys() { return m_pLeftControlKeys; }
	CTsControlKeys* GetRightWinControlKeyS() { return m_pRightControlKeys; }
	
	VOID SetLeftWinControlKeys(CTsControlKeys* pKeys);
	VOID SetRightWinControlKeys(CTsControlKeys* pKeys);

	TS_USE_WIN_TYPE GetUseWinType() { return m_eUseWinType; }
	TS_USE_COLOR_TYPE GetUseColorType() { return m_eUseColorType; }
	BOOL IsSoundOpen() { return m_bOpenSound; }
	
	void SetUseWinType(TS_USE_WIN_TYPE eType) { m_eUseWinType = eType; }
	void SetUseColorType(TS_USE_COLOR_TYPE eType) { m_eUseColorType = eType; }
	void SetSoundOpen(BOOL bOpen) { m_bOpenSound = bOpen; }
	

	void SetLeftUserName(LPCTSTR lpszName) { lstrcpyn(m_szLeftUserName, lpszName, 60); }
	void SetRightUserName(LPCTSTR lpszName) { lstrcpyn(m_szRightUserName, lpszName, 60); }
	LPCTSTR GetLeftUserName() { return m_szLeftUserName; }
	LPCTSTR GetRightUserName() { return m_szRightUserName; }

private:
	bool GetConfigFilePath(LPTSTR lpszBuffer, int nBufferLen);
	void WriteProfileInt(LPCTSTR lpszAppName, LPCTSTR lpszKey, int nValue, LPCTSTR lpszFile);

private:
	CTsGameContext(const CTsGameContext&);

	CTsControlKeys* m_pLeftControlKeys;
	CTsControlKeys* m_pRightControlKeys;
	TS_USE_WIN_TYPE m_eUseWinType;
	TS_USE_COLOR_TYPE m_eUseColorType;
	BOOL m_bOpenSound;

	TCHAR m_szLeftUserName[64];
	TCHAR m_szRightUserName[64];

	CTsDataFactory* m_pDataFactory;
	CTsGameBaseInfo* m_pBaseInfo;
	CTsPlayProcessInfo* m_pPlayInfo;

	CTsKeysTable* m_pKeysTable;
	CTsUserListData* m_pUserList;
};