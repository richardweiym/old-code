#include "tsgamecontext.h"
#include "tsCommonfunction.h"
#include "TsUtilBase.h"
#include "TsResMgr.h"
#include "TsDataKernel.h"
#include "TsUIDataMgr.h"
#include "TsSetupDlg.h"

CTsGameContext* g_gameContext = NULL;

CTsGameContext::CTsGameContext()
{
	TS_RUN_ONCE_CHECK

	m_pLeftControlKeys = NULL;
	m_pRightControlKeys = NULL;
	m_bOpenSound = FALSE;
	m_eUseColorType = USE_COLOR_TYPE;
	m_eUseWinType = USE_LEFT_WIN;

	_ASSERTE(g_gameContext == NULL);
	g_gameContext = this;

	m_pDataFactory = NULL;
	m_pBaseInfo = NULL;
	m_pPlayInfo = NULL;
	m_pKeysTable = NULL;
	m_pUserList = NULL;

	m_szLeftUserName[0] = m_szRightUserName[0] = _T('\0');
}

CTsGameContext::~CTsGameContext()
{
	UnInitialize();
	g_gameContext = NULL;
}

bool CTsGameContext::Initialize()
{
	bool bRet(FALSE);
	
	_ASSERTE(m_pRightControlKeys == NULL);
	_ASSERTE(m_pLeftControlKeys == NULL);
	_ASSERTE(m_pDataFactory == NULL);
	_ASSERTE(m_pBaseInfo == NULL);
	_ASSERTE(m_pPlayInfo == NULL);
	_ASSERTE(m_pKeysTable == NULL);
	_ASSERTE(m_pUserList == NULL);
	
	m_pLeftControlKeys = new CTsControlKeys;
	m_pRightControlKeys = new CTsControlKeys;
	m_pDataFactory = new CTsDataFactory;
	m_pBaseInfo = new CTsGameBaseInfo;
	m_pPlayInfo = new CTsPlayProcessInfo;
	m_pKeysTable = new CTsKeysTable;
	m_pUserList = new CTsUserListData;

	_ASSERTE(m_pLeftControlKeys != NULL && m_pRightControlKeys != NULL && m_pDataFactory != NULL
		&& m_pBaseInfo != NULL && m_pPlayInfo != NULL && m_pKeysTable != NULL && m_pUserList != NULL);

	if(m_pRightControlKeys == NULL || m_pLeftControlKeys == NULL || m_pDataFactory == NULL
		|| m_pBaseInfo == NULL || m_pPlayInfo == NULL || m_pKeysTable == NULL || m_pUserList == NULL)
	{
		return bRet;
	}
	
	m_pKeysTable->LoadTable();

	bRet = LoadFromConfigFile();


	return bRet;
}

void CTsGameContext::UnInitialize()
{
	if(m_pLeftControlKeys != NULL)
	{
		delete m_pLeftControlKeys;
		m_pLeftControlKeys = NULL;
	}

	if(m_pRightControlKeys != NULL)
	{
		delete m_pRightControlKeys;
		m_pRightControlKeys = NULL;
	}

	if(m_pDataFactory != NULL)
	{
		delete m_pDataFactory;
		m_pDataFactory = NULL;
	}
	if(m_pBaseInfo != NULL)
	{
		delete m_pBaseInfo;
		m_pBaseInfo = NULL;
	}
	if(m_pPlayInfo != NULL)
	{
		delete m_pPlayInfo;
		m_pPlayInfo = NULL;
	}

	if(m_pKeysTable != NULL)
	{
		delete m_pKeysTable;
		m_pKeysTable = NULL;
	}

	if(m_pUserList != NULL)
	{
		delete m_pUserList;
		m_pUserList = NULL;
	}
}

bool CTsGameContext::GetConfigFilePath(LPTSTR lpszBuffer, int nBufferLen)
{
	bool bRet = CTsCommonFunction::GetCurrentFilePath(lpszBuffer, nBufferLen, TS_CONFIG_FILE_NAME);

	return bRet;
}

bool CTsGameContext::LoadFromConfigFile()
{
	_ASSERTE(m_pLeftControlKeys != NULL && m_pRightControlKeys != NULL);

	TCHAR szFile[MAX_PATH] = {0};
	bool bRet = GetConfigFilePath(szFile, sizeof(szFile) / sizeof(TCHAR) - 1);
	m_pLeftControlKeys->m_nMoveLeft = GetPrivateProfileInt(_T("setting"), _T("left_move_left"), 1, szFile);
	m_pLeftControlKeys->m_nMoveRight = GetPrivateProfileInt(_T("setting"), _T("left_move_right"), 1, szFile); 
	m_pLeftControlKeys->m_nMoveSwitch = GetPrivateProfileInt(_T("setting"), _T("left_move_switch"), 1, szFile); 
	m_pLeftControlKeys->m_nDropDown = GetPrivateProfileInt(_T("setting"), _T("left_drop_down"), 1, szFile); 
	m_pLeftControlKeys->m_nMoveDown = GetPrivateProfileInt(_T("setting"), _T("left_move_down"), 1, szFile); 
	m_pLeftControlKeys->m_nSwitchDirection = (TS_SWITCH_DIRECTION)GetPrivateProfileInt(_T("setting"), _T("left_swtich_direction"), 1, szFile); 
	_ASSERTE(m_pLeftControlKeys->m_nSwitchDirection == CLOCK_WISE 
		|| m_pLeftControlKeys->m_nSwitchDirection == ANTI_CLOCK_WISE);

	m_pRightControlKeys->m_nMoveLeft = GetPrivateProfileInt(_T("setting"), _T("right_move_left"), 1, szFile);
	m_pRightControlKeys->m_nMoveRight = GetPrivateProfileInt(_T("setting"), _T("right_move_right"), 1, szFile); 
	m_pRightControlKeys->m_nMoveSwitch = GetPrivateProfileInt(_T("setting"), _T("right_move_switch"), 1, szFile); 
	m_pRightControlKeys->m_nDropDown = GetPrivateProfileInt(_T("setting"), _T("right_drop_down"), 1, szFile); 
	m_pRightControlKeys->m_nMoveDown = GetPrivateProfileInt(_T("setting"), _T("right_move_down"), 1, szFile); 
	m_pRightControlKeys->m_nSwitchDirection = (TS_SWITCH_DIRECTION)GetPrivateProfileInt(_T("setting"), _T("right_swtich_direction"), 1, szFile); 
	_ASSERTE(m_pRightControlKeys->m_nSwitchDirection == CLOCK_WISE 
		|| m_pRightControlKeys->m_nSwitchDirection == ANTI_CLOCK_WISE);

	m_eUseWinType = (TS_USE_WIN_TYPE)GetPrivateProfileInt(_T("setting"), _T("use_window"), 1, szFile);
	_ASSERTE(m_eUseWinType == USE_LEFT_WIN
		|| m_eUseWinType == USE_RIGHT_WIN);

	m_eUseColorType = (TS_USE_COLOR_TYPE)GetPrivateProfileInt(_T("setting"), _T("use_color"), 1, szFile);
	_ASSERTE(m_eUseColorType == USE_COLOR_TYPE
		|| m_eUseColorType == USE_BLACK_TYPE);

	m_bOpenSound = GetPrivateProfileInt(_T("setting"), _T("use_sound"), 1, szFile);

	GetPrivateProfileString(_T("setting"), _T("left_name"), _T(""), m_szLeftUserName, 60, szFile);
	GetPrivateProfileString(_T("setting"), _T("right_name"), _T(""), m_szRightUserName, 60, szFile);

	return bRet;
}

void CTsGameContext::WriteProfileInt(LPCTSTR lpszAppName, LPCTSTR lpszKey, int nValue, LPCTSTR lpszFile)
{
	TCHAR szBuffer[32] = {0};
	_sntprintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR) - 1, _T("%u"), nValue);

	WritePrivateProfileString(lpszAppName, lpszKey, szBuffer, lpszFile);
}

bool CTsGameContext::SaveToConfigFile()
{
	_ASSERTE(m_pLeftControlKeys != NULL && m_pRightControlKeys != NULL);
	TCHAR szFile[MAX_PATH] = {0};
	bool bRet = GetConfigFilePath(szFile, sizeof(szFile) / sizeof(TCHAR) - 1);
	if(!bRet)
	{
		return bRet;
	}

	WriteProfileInt(_T("setting"), _T("left_move_left"), m_pLeftControlKeys->m_nMoveLeft, szFile);
	WriteProfileInt(_T("setting"), _T("left_move_right"), m_pLeftControlKeys->m_nMoveRight, szFile);
	WriteProfileInt(_T("setting"), _T("left_move_switch"), m_pLeftControlKeys->m_nMoveSwitch, szFile);
	WriteProfileInt(_T("setting"), _T("left_drop_down"), m_pLeftControlKeys->m_nDropDown, szFile);
	WriteProfileInt(_T("setting"), _T("left_move_down"), m_pLeftControlKeys->m_nMoveDown, szFile);
	WriteProfileInt(_T("setting"), _T("left_swtich_direction"), m_pLeftControlKeys->m_nSwitchDirection, szFile);

	WriteProfileInt(_T("setting"), _T("right_move_left"), m_pRightControlKeys->m_nMoveLeft, szFile);
	WriteProfileInt(_T("setting"), _T("right_move_right"), m_pRightControlKeys->m_nMoveRight, szFile);
	WriteProfileInt(_T("setting"), _T("right_move_switch"), m_pRightControlKeys->m_nMoveSwitch, szFile);
	WriteProfileInt(_T("setting"), _T("right_drop_down"), m_pRightControlKeys->m_nDropDown, szFile);
	WriteProfileInt(_T("setting"), _T("right_move_down"), m_pRightControlKeys->m_nMoveDown, szFile);
	WriteProfileInt(_T("setting"), _T("right_swtich_direction"), m_pRightControlKeys->m_nSwitchDirection, szFile);

	WriteProfileInt(_T("setting"), _T("use_window"), m_eUseWinType, szFile);
	WriteProfileInt(_T("setting"), _T("use_color"), m_eUseColorType, szFile);
	WriteProfileInt(_T("setting"), _T("use_sound"), m_bOpenSound, szFile);

	WritePrivateProfileString(_T("setting"), _T("left_name"), m_szLeftUserName, szFile);
	WritePrivateProfileString(_T("setting"), _T("right_name"), m_szRightUserName, szFile);

	return bRet;
}

void CTsGameContext::SetLeftWinControlKeys(CTsControlKeys* pKeys)
{
	*m_pLeftControlKeys = *pKeys;
}

void CTsGameContext::SetRightWinControlKeys(CTsControlKeys* pKeys)
{
	*m_pRightControlKeys = *pKeys;
}