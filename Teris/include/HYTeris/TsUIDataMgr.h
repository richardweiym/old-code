#pragma  once 

#include <TsUIModuleBase.h>
#include "TsDefine.h"
#include "TsUIInstanceApi.h"
#include <vector>


class CTsGameBaseInfo: public CTsModuleBase
{
public:
	CTsGameBaseInfo();
	~CTsGameBaseInfo();

	void SetPlayMode(TS_GAME_MODE eMode);
	TS_GAME_MODE GetPlayMode() { return m_eGameMode; }

	void SetDisplayMode(TS_DISPLAY_MODE eMode);
	TS_DISPLAY_MODE GetDisplayMode() { return m_eDisplayMode; }

	void SetStartLineCount(TS_START_LINE eLine);
	TS_START_LINE GetStartLineCount() { return m_eLine; }

	void SetHardLevel(TS_HARD_LEVEL eLevel);
	TS_HARD_LEVEL GetHardLevel() { return m_eLevel; }
	
	void SetRecordOption(BOOL bSet);
	BOOL GetRecordOption() { return m_bRecordGame; }

	void SetGameStatus(TS_GAME_STATUS eStatus);
	TS_GAME_STATUS GetGameStatus() { return m_eGameStatus; }
protected:
	CTsGameBaseInfo(const CTsGameBaseInfo&);
	TS_GAME_MODE m_eGameMode;
	TS_DISPLAY_MODE m_eDisplayMode;

	TS_START_LINE m_eLine;
	TS_HARD_LEVEL m_eLevel;
	BOOL m_bRecordGame;

	TS_GAME_STATUS m_eGameStatus;
};


class CTsPlayProcessInfo: public CTsModuleBase
{
public:
	CTsPlayProcessInfo();
	~CTsPlayProcessInfo();

	void Clear();

	void SetLeftPlayerName(LPCTSTR lpszName);
	LPCTSTR GetLeftPlayerName() { return m_strLeftPlayer; }

	void SetRightPlayerName(LPCTSTR lpszName);
	LPCTSTR GetRightPlayerName() { return m_strRightPlayer; }

	void SetLeftElementCount(INT nCount);
	INT GetLeftElementCount() { return m_nLeftEleCount; }

	void SetRightElementCount(INT nCount);
	INT GetRightElementCount() { return m_nRightEleCount; }

	void SetLeftScore( int nScore);
	INT GetLeftScore() { return m_nLeftScore; }

	void SetRightScore(int nScore);
	INT GetRightScore() { return m_nRightScore; }

	void SetLeftWinCount(int nCount);
	INT GetLeftWinCount() { return m_nLeftWinCount; }

	void SetRightWinCount(int nCount);
	int GetRightWinCount() { return m_nRightWinCount; }

	void SetLeftRemoveLineCount(int nCount);
	int GetLeftRemoveLineCount() { return m_nLeftRemoveLineCount; }

	void SetRightRemoveLineCount(int nCount);
	int GetRightRemoveLineCount() { return m_nRightRemoveLineCount; }

	void SetLeftKillLineCount(int nCount);
	int GetLeftKillLineCount() { return m_nLeftKillLineCount; }

	void SetRightKillLineCount(int nCount);
	int GetRightKillLineCount() { return m_nRightKillLineCount; }

protected:
	void _NotifyChange();
	CTsPlayProcessInfo(const CTsPlayProcessInfo& t);

	CString m_strLeftPlayer;
	CString m_strRightPlayer;

	INT m_nLeftEleCount;
	INT m_nRightEleCount;
	
	INT m_nLeftScore;
	INT m_nRightScore;
	
	INT m_nLeftWinCount;
	INT m_nRightWinCount;

	INT m_nLeftRemoveLineCount;
	INT m_nRightRemoveLineCount;

	INT m_nLeftKillLineCount;
	INT m_nRightKillLineCount;
	
};

enum TS_USER_TYPE
{
	COMPUTER_TYPE = 0,
	LOCAL_CONTROL_TYPE = 1,
	NET_CONTROL_TYPE = 2,
	NET_OBSERVER_TYPE = 3
};

class CTsUserItem
{
public:
	CTsUserItem(INT nId, TS_USER_TYPE eType, LPCTSTR lpszName)
	{
		m_nUserId = nId;
		m_eUserType = eType;
		lstrcpyn(m_szUserName, lpszName, sizeof(m_szUserName) / sizeof(TCHAR) - 1);
	}

public:
	INT m_nUserId;
	TCHAR m_szUserName[128];
	TS_USER_TYPE m_eUserType;
};

class CTsUserListData: public CTsModuleBase
{
public:
	CTsUserListData();
	~CTsUserListData();

	BOOL AddUser(INT nId, TS_USER_TYPE eType, LPCTSTR lpszName);
	BOOL RemoveUser(INT nId);
	void ClearUser();
	
	INT GetUserCount() const;
	CTsUserItem* GetUserByIndex(INT nIndex) ;
	CTsUserItem* GetUserById(INT nId) ;

	void NotifyUserChanged();

private:
	std::vector<CTsUserItem> m_arUser;
};



template<typename T>
class CTsGameBaseInfoObserver: public CTsObserverBase<T>
{
public:
	void DoInit()
	{
		T* pThis = static_cast<T*>(this);

		CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
		_ASSERTE(pBaseInfo != NULL);
		pBaseInfo->AddObserver(pThis->m_hWnd);
	}

	void DoRelease()
	{
		T* pThis = static_cast<T*>(this);

		CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
		_ASSERTE(pBaseInfo != NULL);
		pBaseInfo->RemoveObserver(pThis->m_hWnd);
	}

	void DoUIDataChanged(WPARAM wParam, LPARAM lParam)
	{
		_ASSERTE(FALSE);
	}
};

template<typename T>
class CTsPlayProcessObserver: public CTsObserverBase<T>
{
public:
	void DoInit()
	{
		T* pThis = static_cast<T*>(this);

		CTsPlayProcessInfo* pPlayInfo = TsGetGameProcessInfo();
		_ASSERTE(pPlayInfo != NULL);
		pPlayInfo->AddObserver(pThis->m_hWnd);
	}

	void DoRelease()
	{
		T* pThis = static_cast<T*>(this);

		CTsPlayProcessInfo* pPlayInfo = TsGetGameProcessInfo();
		_ASSERTE(pPlayInfo != NULL);
		pPlayInfo->RemoveObserver(pThis->m_hWnd);
	}

	void DoUIDataChanged(WPARAM wParam, LPARAM lParam)
	{
		_ASSERTE(FALSE);
	}
};

template<typename T>
class CTsUserInfObserver: public CTsObserverBase<T>
{
public:
	void DoInit()
	{
		T* pThis = static_cast<T*>(this);

		CTsUserListData* pUserInfo = TsGetUserList();
		_ASSERTE(pUserInfo != NULL);
		pUserInfo->AddObserver(pThis->m_hWnd);
	}

	void DoRelease()
	{
		T* pThis = static_cast<T*>(this);

		CTsUserListData* pUserInfo = TsGetUserList();
		_ASSERTE(pUserInfo != NULL);
		pUserInfo->RemoveObserver(pThis->m_hWnd);
	}

	void DoUIDataChanged(WPARAM wParam, LPARAM lParam)
	{
		_ASSERTE(FALSE);
	}
};