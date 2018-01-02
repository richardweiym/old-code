#include "TsUIDataMgr.h"

#include "TsUIInstanceApi.h"
#include "TerisRes.h"

using namespace std;

CTsGameBaseInfo* g_gameBaseInfo = NULL;
CTsPlayProcessInfo* g_gamePlayInfo = NULL;
CTsUserListData* g_pUserList = NULL;

CTsGameBaseInfo::CTsGameBaseInfo()
{
	TS_RUN_ONCE_CHECK

	m_eGameMode = NONE_MODE;
	m_eDisplayMode = DISPLAY_MULTI;

	m_eLevel = LEVEL_EASY;
	m_eLine = LINE_0;
	m_bRecordGame = FALSE;
	m_eGameStatus = IDLE_STATUS;

	g_gameBaseInfo = this;
}
CTsGameBaseInfo::~CTsGameBaseInfo()
{
	g_gameBaseInfo = NULL;
}

void CTsGameBaseInfo::SetPlayMode(TS_GAME_MODE eMode)
{
	if(m_eGameMode != eMode)
	{
		m_eGameMode = eMode;
		NotifyObservers(TS_WPARAM_GAME_MODE_CHANGED, (LPARAM)eMode);
	}
}

void CTsGameBaseInfo::SetDisplayMode(TS_DISPLAY_MODE eMode)
{
	if(m_eDisplayMode != eMode)
	{
		m_eDisplayMode = eMode;
		NotifyObservers(TS_WPARAM_DISPLAY_MODE_CHANGED, LPARAM(eMode));
	}
}

void CTsGameBaseInfo::SetStartLineCount(TS_START_LINE eLine)
{
	if(m_eLine != eLine)
	{
		m_eLine = eLine;
		NotifyObservers(TS_WPARAM_START_LINE_CHANGED, LPARAM(m_eLine));
	}
}
void CTsGameBaseInfo::SetHardLevel(TS_HARD_LEVEL eLevel)
{
	if(m_eLevel != eLevel)
	{
		m_eLevel = eLevel;
		NotifyObservers(TS_WPARAM_HARD_LEVEL_CHANGED, LPARAM(m_eLevel));
	}
}
void CTsGameBaseInfo::SetRecordOption(BOOL bSet)
{
	if(m_bRecordGame != bSet)
	{
		m_bRecordGame = bSet;
		NotifyObservers(TS_WPARAM_RECORD_OPTION_CHANGED, (LPARAM)bSet);
	}
}
void CTsGameBaseInfo::SetGameStatus(TS_GAME_STATUS eStatus)
{
	if(m_eGameStatus != eStatus)
	{
		m_eGameStatus = eStatus;
		NotifyObservers(TS_WPARAM_GAME_STATUS_CHANGED, (LPARAM)eStatus);
	}
}

///////////////////////////////////////////
CTsPlayProcessInfo::CTsPlayProcessInfo()
:m_strLeftPlayer(_T("")), m_strRightPlayer(_T(""))
{
	Clear();

	g_gamePlayInfo = this;
}
CTsPlayProcessInfo::~CTsPlayProcessInfo()
{
	g_gamePlayInfo = NULL;
}
void CTsPlayProcessInfo::Clear()
{
	m_nLeftEleCount = m_nRightEleCount = 0;
	m_nLeftScore = 	m_nRightScore = 0;
	m_nLeftWinCount = m_nRightWinCount = 0;
	m_nLeftRemoveLineCount = m_nRightRemoveLineCount = 0;
	m_nLeftKillLineCount = m_nRightKillLineCount = 0;
}
void CTsPlayProcessInfo::_NotifyChange()
{
	NotifyObservers(TS_WPARAM_PROCESS_CHANGED, 0);
}

void CTsPlayProcessInfo::SetLeftPlayerName(LPCTSTR lpszName)
{
	_ASSERTE(lpszName != NULL);
	m_strLeftPlayer = lpszName;
	_NotifyChange();
}


void CTsPlayProcessInfo::SetRightPlayerName(LPCTSTR lpszName)
{
	_ASSERTE(lpszName != NULL);
	m_strRightPlayer = lpszName;
	_NotifyChange();
}


void CTsPlayProcessInfo::SetLeftElementCount(INT nCount)
{
	m_nLeftEleCount = nCount;
	_NotifyChange();
}


void CTsPlayProcessInfo::SetRightElementCount(INT nCount)
{
	m_nRightEleCount = nCount;
	_NotifyChange();
}


void CTsPlayProcessInfo::SetLeftScore( int nScore)
{
	m_nLeftScore = nScore;
	_NotifyChange();
}


void CTsPlayProcessInfo::SetRightScore(int nScore)
{
	m_nRightScore = nScore;
	_NotifyChange();
}


void CTsPlayProcessInfo::SetLeftWinCount(int nCount)
{
	m_nLeftWinCount = nCount;
	_NotifyChange();
}


void CTsPlayProcessInfo::SetRightWinCount(int nCount)
{
	m_nRightWinCount = nCount;
	_NotifyChange();
}


void CTsPlayProcessInfo::SetLeftRemoveLineCount(int nCount)
{	
	m_nLeftRemoveLineCount = nCount;
	_NotifyChange();
}


void CTsPlayProcessInfo::SetRightRemoveLineCount(int nCount)
{
	m_nRightRemoveLineCount = nCount;
	_NotifyChange();
}

void CTsPlayProcessInfo::SetLeftKillLineCount(int nCount)
{
	m_nLeftKillLineCount = nCount;
	_NotifyChange();
}


void CTsPlayProcessInfo::SetRightKillLineCount(int nCount)
{
	m_nRightKillLineCount = nCount;
	_NotifyChange();
}



////////////////////////////  CTsUserListData  //////////////////////
CTsUserListData::CTsUserListData()
{
	TS_RUN_ONCE_CHECK
	g_pUserList = this;
}
CTsUserListData::~CTsUserListData()
{
	g_pUserList = NULL;
}

BOOL CTsUserListData::AddUser(INT nId, TS_USER_TYPE eType, LPCTSTR lpszName)
{
	BOOL bRet(FALSE);
	if(GetUserById(nId) == NULL)
	{
		CTsUserItem user(nId, eType, lpszName);
		m_arUser.push_back(user);
		
		NotifyUserChanged();
		bRet  = TRUE;
	}
	else
	{
		_ASSERTE(FALSE);
	}

	return bRet;
}

BOOL CTsUserListData::RemoveUser(INT nId)
{
	BOOL bRet(FALSE);
	vector<CTsUserItem>::iterator itr = m_arUser.begin();
	for(; itr != m_arUser.end(); ++itr)
	{
		CTsUserItem& user = *itr;
		if(user.m_nUserId == nId)
		{
			break;
		}
	}

	if(itr != m_arUser.end())
	{
		m_arUser.erase(itr);
		NotifyUserChanged();
		bRet = TRUE;
	}

	return bRet;
}


void CTsUserListData::ClearUser()
{
	m_arUser.clear();
	NotifyUserChanged();
}

INT CTsUserListData::GetUserCount() const
{
	return m_arUser.size();
}

CTsUserItem* CTsUserListData::GetUserByIndex(INT nIndex) 
{
	int nCount = m_arUser.size();
	_ASSERTE(nIndex >= 0 && nIndex < nCount);
	
	return  &m_arUser[nIndex] ;
}

CTsUserItem* CTsUserListData::GetUserById(INT nId) 
{
	CTsUserItem* pRet = NULL;
	int nCount = m_arUser.size();

	for(int i=0; i<nCount; ++i)
	{
		CTsUserItem* pUser = &m_arUser[i];
		_ASSERTE(pUser != NULL);
		if(pUser->m_nUserId == nId)
		{
			pRet = pUser;
			break;
		}
	}

	return pRet;
}

void CTsUserListData::NotifyUserChanged()
{
	NotifyObservers(TS_WPARAM_USER_CHANGED, 0);
}
