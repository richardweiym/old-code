#include "tsmaingame.h"
#include "tsMainUIMgr.h"
#include "tsGameContext.h"
#include "tsGameEngine.h"
#include "tscommonfunction.h"
#include "tsDataKernel.h"
#include "TsResMgr.h"
#include "TsUIInstanceApi.h"


CTsMainGame* g_pGameInstance = NULL;

CTsAgentWindow::CTsAgentWindow(CMessageMap* pMap)
{
	_ASSERTE(pMap != NULL);
	m_pMessageMap = pMap;
}

LRESULT CTsAgentWindow::OnTsUIDataChanged(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRet(0);
	if(m_pMessageMap != NULL)
	{
		m_pMessageMap->ProcessWindowMessage(m_hWnd, nMsgType, wParam, lParam, lRet, 0);
	}

	return S_OK;
}

CTsMainGame* GetGameInstance()
{
	return g_pGameInstance;
}

CTsMainGame::~CTsMainGame()
{
	g_pGameInstance = NULL;
}


CTsMainGame::CTsMainGame()
{	
	TS_RUN_ONCE_CHECK 
		
	m_pGameEngine = NULL;
	m_pGameUIMgr = NULL;

	_ASSERTE(g_pGameInstance == NULL);
	g_pGameInstance = this;
	m_pAgentWnd = NULL;
}

bool CTsMainGame::Initialize()
{
	bool bRet(FALSE);

	
	_ASSERTE(m_pGameUIMgr == NULL);
	m_pGameUIMgr = new CTsMainUIMgr;
	_ASSERTE(m_pGameUIMgr != NULL);
	
	if( m_pGameUIMgr == NULL)
	{
		return bRet;
	}
	
	if(!m_pGameUIMgr->Initialize())
	{
		return bRet;
	}
	

	TCHAR szBuffer[MAX_PATH] = {0};
	HINSTANCE hRes = NULL;
	if(CTsCommonFunction::GetCurrentFilePath(szBuffer, sizeof(szBuffer) / sizeof(TCHAR) - 1, TS_RES_DLL_NAME))
	{	
		hRes = LoadLibrary(szBuffer);
		_ASSERTE(hRes != NULL);
	
		CTsResMgr* tsRes = TsGetResMgr();
		_ASSERTE(tsRes != NULL);
		tsRes->SetResInstance(hRes);
	}
	
	if(hRes == NULL)
	{
		return FALSE;
	}

	m_pAgentWnd = new CTsAgentWindow(this);
	_ASSERTE(m_pAgentWnd != NULL);
	if(m_pAgentWnd == NULL)
	{
		return FALSE;
	}

	bRet = TRUE;

	return bRet;
}

bool CTsMainGame::InitializeUI()
{
	_ASSERTE(m_pAgentWnd != NULL);
	HWND hWnd = m_pAgentWnd->Create(NULL, CRect(0, 0, 0, 0), TS_UNIQUE_WIN_TEXT, WS_POPUP);
	_ASSERTE(hWnd != NULL);
	if(hWnd == NULL)
	{
		return FALSE;
	}

	_ASSERTE(m_pGameUIMgr != NULL);
	if(!m_pGameUIMgr->InitializeUI())
	{
		return FALSE;
	}
	
	return TRUE;
}

void CTsMainGame::UnInitializeUI()
{
	_ASSERTE(m_pGameUIMgr != NULL);

	if(m_pGameEngine != NULL)
	{
		m_pGameEngine->UnInitialize();
	}

	if(m_pGameUIMgr != NULL)
	{
		m_pGameUIMgr->UnInitializeUI();
	}

	if(m_pAgentWnd != NULL && m_pAgentWnd->IsWindow())
	{
		m_pAgentWnd->DestroyWindow();
	}
}

void CTsMainGame::UnInitialize()
{
	CTsResMgr* tsRes = TsGetResMgr();
	if(tsRes != NULL)
	{
		HINSTANCE hRes = tsRes->GetResInstance();
		if(hRes != NULL)
		{
			FreeLibrary(hRes);
			tsRes->SetResInstance(NULL);
		}
	}

	if(m_pGameEngine != NULL)
	{
		delete m_pGameEngine;
		m_pGameEngine = NULL;
	}

	if(m_pGameUIMgr != NULL)
	{
		m_pGameUIMgr->UnInitialize();
		delete m_pGameUIMgr;
		m_pGameUIMgr = NULL;
	}
	
	if(m_pAgentWnd != NULL)
	{
		delete m_pAgentWnd;
		m_pAgentWnd = NULL;
	}
}


LRESULT CTsMainGame::OnUIDataChanged(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch(wParam)
	{
	case TS_WPARAM_GAME_MODE_CHANGED:
		_CheckEngineType();
		break;

	default:
		break;
	}

	return S_OK;
}

void CTsMainGame::_CheckEngineType()
{
	CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
	_ASSERTE(pBaseInfo != NULL);
	
	switch(pBaseInfo->GetPlayMode())
	{
	case SINGLE_PLAYER_MODE:
		if(m_pGameEngine != NULL && m_pGameEngine->GetTypeCode() != SINGLE_PLAYER_MODE)
		{
			_CheckAndDeleteOldEngine();
		}
		if(m_pGameEngine == NULL)
		{
			m_pGameEngine = new CTsSinglePlayerEngine;
			_ASSERTE(m_pGameEngine != NULL);
			m_pGameEngine->Initialize();
		}
		break;

	case TWO_PLAYER_MODE:
		if(m_pGameEngine != NULL && m_pGameEngine->GetTypeCode() != TWO_PLAYER_MODE)
		{
			_CheckAndDeleteOldEngine();
		}
		if(m_pGameEngine == NULL)
		{
			m_pGameEngine = new CTsTwoPlayerEngine;
			_ASSERTE(m_pGameEngine != NULL);
			m_pGameEngine->Initialize();
		}
		break;

	case COMPUTER_PLAYER_MODE:
		if(m_pGameEngine != NULL && m_pGameEngine->GetTypeCode() != COMPUTER_PLAYER_MODE)
		{
			_CheckAndDeleteOldEngine();
		}
		if(m_pGameEngine == NULL)
		{
			m_pGameEngine = new CTsPlayWithComputerEngine;
			_ASSERTE(m_pGameEngine != NULL);
			m_pGameEngine->Initialize();
		}
		break;
	}
}
void CTsMainGame::_CheckAndDeleteOldEngine()
{
	if(m_pGameEngine == NULL) return;

	CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
	_ASSERTE(pBaseInfo != NULL);
	if(pBaseInfo->GetGameStatus() != IDLE_STATUS)
	{
		m_pGameEngine->Stop();
	}

	if(m_pGameEngine->IsInited())
	{
		m_pGameEngine->UnInitialize();
	}

	delete m_pGameEngine;
	m_pGameEngine = NULL;
}