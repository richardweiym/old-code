#include "TsMainUIMgr.h"
#include "TsGameContext.h"
#include "TsGameMainWnd.h"
#include "TsMainGame.h"
#include "TsSetupDlg.h"
#include "TsResMgr.h"
#include "TsDataKernel.h"
#include "TsMainDisplayPane.h"
#include "TsGameEngine.h"

CTsMainUIMgr::CTsMainUIMgr()
{
	m_pMainWnd = NULL;
	m_pContext = NULL;
	m_pResMgr = NULL;
}

CTsMainUIMgr::~CTsMainUIMgr()
{
	UnInitialize();
}

bool CTsMainUIMgr::Initialize()
{
	_ASSERTE(m_pContext == NULL);
	_ASSERTE(m_pMainWnd == NULL);
	_ASSERTE(m_pResMgr == NULL);
	m_pContext = new CTsGameContext;
	m_pMainWnd = new CTsGameMainWnd;
	m_pResMgr = new CTsResMgr;
	
	if(m_pContext == NULL || m_pMainWnd == NULL || m_pResMgr == NULL)
	{
		return FALSE;
	}
	
	if(!m_pContext->Initialize())
	{
		return FALSE;
	}

	m_pMainWnd->SetMessageMap(this);
	if(!m_pMainWnd->Initialize())
	{
		return FALSE;
	}
	

	return TRUE;
}
void CTsMainUIMgr::_OnSetPlayMode(TS_GAME_MODE eMode)
{
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);
	pInfo->SetPlayMode(eMode);
}

void CTsMainUIMgr::_ResetColorType()
{
	_ASSERTE(m_pContext != NULL);
	CTsDataFactory* pDataFactory = TsGetDataFactory();
	_ASSERTE(pDataFactory != NULL);

	if(m_pContext->GetUseColorType() == USE_COLOR_TYPE)
	{
		CTsDataColorSource colorSource;
		pDataFactory->Init(colorSource);
	}
	else
	{
		CTsDataBlackSource colorSource;
		pDataFactory->Init(colorSource);
	}

	CTsGameMainWnd* pMainWnd = TsGetGameMainWnd();
	_ASSERTE(pMainWnd != NULL);

	HWND hLeftDisplay = pMainWnd ->GetLeftDisplayWnd();
	HWND hRightDisplay = pMainWnd->GetRightDisplayWnd();
	HWND hLeftNext = pMainWnd->GetLeftNextWnd();
	HWND hRightNext = pMainWnd->GetRightNextWnd();
	::InvalidateRect(hLeftDisplay, NULL, TRUE);
	::InvalidateRect(hRightDisplay, NULL, TRUE);
	::InvalidateRect(hLeftNext, NULL, TRUE);
	::InvalidateRect(hRightNext, NULL, TRUE);
}
bool CTsMainUIMgr::InitializeUI()
{
	bool bRet(FALSE);
	_ASSERTE(m_pMainWnd != NULL);
	if(m_pMainWnd != NULL)
	{
		bRet = m_pMainWnd->InitializeUI();
	}
	
	if(bRet)
	{
		_SetDefaultUIData();
	}


	return bRet;// 
}
void CTsMainUIMgr::UnInitializeUI()
{
	_ASSERTE(m_pMainWnd != NULL);
	if(m_pMainWnd != NULL)
	{
		m_pMainWnd->UnInitializeUI();
	}
}
void CTsMainUIMgr::UnInitialize()
{
	if(m_pResMgr != NULL)
	{
		delete m_pResMgr;
		m_pResMgr = NULL;
	}

	if(m_pContext != NULL)
	{
		m_pContext->UnInitialize();
		delete m_pContext;
		m_pContext = NULL;
	}

	if(m_pMainWnd != NULL)
	{
		m_pMainWnd->UnInitialize();
		delete m_pMainWnd;
		m_pMainWnd = NULL;
	}
}

LRESULT CTsMainUIMgr::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;

	CTsMainGame* pGame = GetGameInstance();
	_ASSERTE(pGame != NULL);
	pGame->UnInitializeUI();

	return S_OK;
}

LRESULT CTsMainUIMgr::OnCommand(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{ 
	WORD wNotifyCode = HIWORD(wParam);
	WORD wCmdId = LOWORD(wParam);
	HWND hWndFrom = (HWND)lParam;
	
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);
	TS_GAME_MODE eOld = pInfo->GetPlayMode();

	TS_GAME_MODE eMode(NONE_MODE);
	switch(wCmdId)
	{
		//button 
	case TS_SINGLE_BTN_ID:
		{
			if(eOld == SINGLE_PLAYER_MODE 
				|| eOld == TWO_PLAYER_MODE 
				|| eOld == COMPUTER_PLAYER_MODE)
			{
				eMode = eOld;
			}
			else
			{
				eMode = SINGLE_PLAYER_MODE;
			}
		}

		break;
	case TS_NET_BTN_ID:
		eMode = NET_SELECT_MODE;
		break;
	case TS_REC_PLAY_BTN_ID:
		eMode = RECORD_SELECT_MODE;
		break;

	case TS_SETUP_BTN_ID:
		_ASSERTE(m_pMainWnd != NULL);
		m_pMainWnd->ShowSetupDlg();
		break;
	
		//radio
	case IDC_PLAY_SINGLE:
		eMode = SINGLE_PLAYER_MODE;
		break;
	case IDC_PLAY_TWO:
		eMode = TWO_PLAYER_MODE;
		break;
	case IDC_PLAY_COMPUTER:
		eMode = COMPUTER_PLAYER_MODE;
		break;
		
		//start, pause
	case IDC_START_BUTTON:
		_OnStartCommand();
		break;

	case IDC_PAUSE_BUTTON:
		_OnPauseCommand();
		break;
	}
	
	if(eMode != NONE_MODE)
	{
		_OnSetPlayMode(eMode);
	}

	return S_OK;
}
void CTsMainUIMgr::_OnPauseCommand()
{
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);
	TS_GAME_MODE eMode = pInfo->GetPlayMode();
	TS_GAME_STATUS eStatus = pInfo->GetGameStatus();

   if(eStatus == PAUSE_STATUS)
	{
		_ResumePlay();
	}
	else if(eStatus == RUNNING_STATUS)
	{
		_PausePlay();
	}
	else
	{
		_ASSERTE(FALSE);
	}
}

void CTsMainUIMgr::_OnStartCommand()
{
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);
	TS_GAME_MODE eMode = pInfo->GetPlayMode();
	TS_GAME_STATUS eStatus = pInfo->GetGameStatus();


	if(eStatus == IDLE_STATUS)
	{
		_StartPlay();
	}
	else
	{
		_EndPlay();
	}

}

LRESULT CTsMainUIMgr::OnSetupDlgChanged(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL&)
{	
	_ResetColorType();

	_ASSERTE(m_pMainWnd != NULL);
	HWND hWndMainDisplay = m_pMainWnd->GetMainDisplayWnd();
	_ASSERTE(hWndMainDisplay != NULL && IsWindow(hWndMainDisplay));
	::SendMessage(hWndMainDisplay, WM_TS_UPDATE_DISPLAY, 0, 0);

	CTsGameEngineBase* pEngine = GetCurrentEngine();
	if(pEngine != NULL)
	{
		LRESULT lRet(0);
		pEngine->ProcessWindowMessage(NULL, WM_TS_UPDATE_DISPLAY, 0, 0, lRet);
	}

	return S_OK;
}
void CTsMainUIMgr::_ResetGameOption()
{
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);
	
	pInfo->SetHardLevel(LEVEL_EASY);
	pInfo->SetRecordOption(FALSE);
	pInfo->SetStartLineCount(LINE_0);
}
void CTsMainUIMgr::_SetDefaultUIData()
{
	_ResetColorType();
	_OnSetPlayMode(SINGLE_PLAYER_MODE);
	_ResetGameOption();
}
void CTsMainUIMgr::_StartPlay()
{
	CTsGameEngineBase* pEngine = GetCurrentEngine();
	
	if(pEngine != NULL 
		&& pEngine->IsInited())
	{
		pEngine->Start();
	}
}

void CTsMainUIMgr::_EndPlay()
{
	CTsGameEngineBase* pEngine = GetCurrentEngine();
	
	if(pEngine != NULL 
		&& pEngine->IsInited())
	{
		pEngine->Stop();
	}
}

void CTsMainUIMgr::_PausePlay()
{
	CTsGameEngineBase* pEngine = GetCurrentEngine();
	
	if(pEngine != NULL 
		&& pEngine->IsInited())
	{
		pEngine->Pause();
	}
}

void CTsMainUIMgr::_ResumePlay()
{
	CTsGameEngineBase* pEngine = GetCurrentEngine();
	
	if(pEngine != NULL 
		&& pEngine->IsInited())
	{
		pEngine->Resume();
	}
}