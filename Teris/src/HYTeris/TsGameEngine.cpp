#include "TsGameEngine.h"
#include "TsUIInstanceApi.h"
#include "TsGameContext.h"
#include "TsGameController.h"
#include "TsGameMainWnd.h"
#include "TsDisplayContainer.h"
#include "TsScoreMgr.h"
#include "TsResMgr.h"
#include "TerisRes.h"


CTsGameEngineBase* g_pGameEngine = NULL;

////////////////////////////  CTsGarbageCreator ////////////////
CTsGarbageCreator::CTsGarbageCreator()
:m_prmFiller(TS_GROUND_WIDTH)
{
}

void CTsGarbageCreator::GetGarbage(CTsField* pBuffer, INT nLen)
{
	_ASSERTE(pBuffer != NULL);
	CTsDataFactory* pDataFactory = TsGetDataFactory();
	_ASSERTE(pDataFactory != NULL);
	INT nCount = pDataFactory->GetElementCount();
	_ASSERTE(nCount > 0);

	CTsField* pStart = pBuffer;
	INT nLeft(nLen);
	INT nIndex(0);
	INT nTemp(0);
	while(nLeft >= TS_GROUND_WIDTH)
	{
		for(int i=0; i<TS_GROUND_WIDTH; ++i)
		{
			nIndex = m_rndFiller.Random(nCount);
			pStart[i] = pDataFactory->GetElementByIndex(nIndex);
		}
		
		nIndex = m_rndFiller.Random(TS_GROUND_WIDTH / 2 + 2);// 0 - 6
		++nIndex; // 1 --- 7

		m_prmFiller.Restart();
		while(nIndex > 0)
		{
			nTemp = m_prmFiller.GetNext();
			pStart[nTemp].SetEmpty();

			--nIndex;
		}

		nLeft -= TS_GROUND_WIDTH;
		pStart += TS_GROUND_WIDTH;
	}
}


///////////////////  CTsGameEngineBase  /////////////////////////
CTsGameEngineBase* GetCurrentEngine()
{
	return g_pGameEngine;
}

VOID CALLBACK TimerProc(HWND hwnd,  UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	_ASSERTE(g_pGameEngine != NULL);
	g_pGameEngine->OnTimer();
}

CTsGameEngineBase::CTsGameEngineBase()
{
	m_bInited = FALSE;

	g_pGameEngine = this;
	m_nTimer = 0;
}


CTsGameEngineBase::~CTsGameEngineBase()
{
	_ASSERTE(!m_bInited);
	g_pGameEngine = NULL;
}
BOOL CTsGameEngineBase::_CreateTimer()
{
	_ASSERTE(m_nTimer == 0);
	if(m_nTimer != 0)
	{
		_DeleteTimer();
	}

	m_nTimer = SetTimer(NULL, NULL, TS_TIMER_INTERVAL, (TIMERPROC)TimerProc);
	if(m_nTimer == 0)
	{
		_ASSERTE(FALSE);
		return FALSE;
	}

	return TRUE;
}
BOOL CTsGameEngineBase::_CreateController()
{
	_ASSERTE(FALSE);
	return TRUE;
}
void CTsGameEngineBase::_DeleteController()
{
	m_controllerMgr.DeleteController();
}
void CTsGameEngineBase::_DeleteTimer()
{
	if(m_nTimer != 0)
	{
		KillTimer(NULL, m_nTimer);
		m_nTimer = 0;
	}
}
BOOL CTsGameEngineBase::Initialize()
{
	if(m_bInited)
	{
		_ASSERTE(FALSE);
		return TRUE;
	}

	if(!_CreateTimer())
	{
		goto __end;
	}


	if(!_CreateController())
	{
		goto __end;
	}

	if(!m_controllerMgr.Initialize())
	{
		goto __end;
	}
	
	m_bInited = TRUE;

	return TRUE;

__end:	
	UnInitialize();
	return FALSE;
}
BOOL CTsGameEngineBase::Start()
{
	_ASSERTE(m_bInited);
	if(!m_bInited)
	{
		return FALSE;
	}
	

	_AttachWindowToController();
	_SetRndSeedForController();
	_SetMoveDownInterval();
	_SetInitDataForController();


	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL && pInfo->GetGameStatus() != RUNNING_STATUS);
	if(pInfo->GetGameStatus() != RUNNING_STATUS)
	{
		if(m_controllerMgr.Start())
		{
			pInfo->SetGameStatus(RUNNING_STATUS);
		}
		else
		{
			goto _end_handle;
		}
	}


	return TRUE;

_end_handle:
	Stop();
	return FALSE;
}	
VOID CTsGameEngineBase::Stop()
{
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);
	if(pInfo->GetGameStatus() != IDLE_STATUS)
	{
		m_controllerMgr.Stop();
		pInfo->SetGameStatus(IDLE_STATUS);
	}
	
}
BOOL CTsGameEngineBase::Pause()
{
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);
	if(pInfo->GetGameStatus() == RUNNING_STATUS)
	{
		if(m_controllerMgr.Pause())
		{
			pInfo->SetGameStatus(PAUSE_STATUS);
			return TRUE;
		}
	}

	return FALSE;
}

void CTsGameEngineBase::Resume()
{
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);
	if(pInfo->GetGameStatus() == PAUSE_STATUS)
	{
		m_controllerMgr.Resume();
		pInfo->SetGameStatus(RUNNING_STATUS);
	}
}

void CTsGameEngineBase::UnInitialize()
{
	
	Stop();

	m_controllerMgr.UnInitialize();
	m_controllerMgr.DeleteController();
	_DeleteTimer();

	m_bInited = FALSE;
}

void CTsGameEngineBase::OnTimer()
{
	m_controllerMgr.OnTimer();
}


void CTsGameEngineBase::_SetMoveDownInterval()
{
	_ASSERTE(FALSE);
}
void CTsGameEngineBase::_SetInitDataForController()
{
	_ASSERTE(FALSE);
}
BOOL CTsGameEngineBase::_AttachWindowToController()
{
	_ASSERTE(FALSE);
	return TRUE;
}
void CTsGameEngineBase::_SetRndSeedForController()
{
	_ASSERTE(FALSE);
}
INT CTsGameEngineBase::_GetHardLevelInterval()
{
	CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
	_ASSERTE(pBaseInfo != NULL);

	TS_HARD_LEVEL eLevel = pBaseInfo->GetHardLevel();

	INT nTerval(1000);
	if(eLevel == LEVEL_EASY)
	{
		nTerval = 1000;
	}
	else if(eLevel == LEVEL_MIDDLE)
	{
		nTerval = 500;
	}
	else if(eLevel == LEVEL_HARD)
	{
		nTerval = 200;
	}
	else if(eLevel == LEVEL_CRAZY)
	{
		nTerval = 100;
	}

	return nTerval;
}
LRESULT CTsGameEngineBase::OnDisplayEvent(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_ASSERTE(FALSE);
	return S_OK;
}

void CTsGameEngineBase::SetControllerWin(INT nID, TS_DISPLAY_RESULT eResult)
{
	CTsGameControllerBase* pController = m_controllerMgr.GetControllerById(nID);
	if(pController != NULL)
	{
		pController->SetPlayResult(eResult);
	}
}

///////////////////////////////   CTsSinglePlayerEngine  ///////////////
TS_GAME_MODE CTsSinglePlayerEngine::GetTypeCode()
{
	return SINGLE_PLAYER_MODE;
}


BOOL CTsSinglePlayerEngine::_CreateController()
{
	_ASSERTE(m_controllerMgr.GetSingleController() == NULL);
	m_controllerMgr.CreateSingleController();

	CTsGameControllerBase* pController = m_controllerMgr.GetSingleController();
	_ASSERTE(pController != NULL);
	if(pController == NULL) return FALSE;

	pController->SetEngineMap(this);

	m_controllerMgr.CreateSingleScoreListener();
	m_controllerMgr.CreateSoundListener();

	return TRUE;
}
BOOL CTsSinglePlayerEngine::_AttachWindowToController()
{
	CTsGameMainWnd* pMainWnd = TsGetGameMainWnd();
	_ASSERTE(pMainWnd != NULL);
	HWND hDisplay(pMainWnd->GetLeftDisplayWnd()), HNextEle(pMainWnd->GetLeftNextWnd());

	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);
	TS_USE_WIN_TYPE useWin = pContext->GetUseWinType();
	if(useWin == USE_RIGHT_WIN)
	{
		hDisplay = pMainWnd->GetRightDisplayWnd();
		HNextEle = pMainWnd->GetRightNextWnd();
	}
	_ASSERTE(hDisplay != NULL && ::IsWindow(hDisplay));
	_ASSERTE(HNextEle != NULL && ::IsWindow(HNextEle));

	CTsGameControllerBase* pController = m_controllerMgr.GetSingleController();
	_ASSERTE(pController != NULL);
	pController->SetDisplayWnd(hDisplay);
	pController->SetNextElementWnd(HNextEle);
	
	return TRUE;
}
void CTsSinglePlayerEngine::_SetRndSeedForController()
{
	CTsGameControllerBase* pController = m_controllerMgr.GetSingleController();
	_ASSERTE(pController != NULL);
	pController->SetElementCreator(GetTickCount());
}
void CTsSinglePlayerEngine::_SetMoveDownInterval()
{
	CTsGameControllerBase* pController = m_controllerMgr.GetSingleController();
	_ASSERTE(pController != NULL);
	pController->SetMoveDownInterval(_GetHardLevelInterval());
}


void CTsSinglePlayerEngine::_SetInitDataForController()
{
	CTsField arData[TS_GROUND_WIDTH * TS_GROUND_HEIGHT];
	
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);

	INT nLineCount = pInfo->GetStartLineCount();
	if(nLineCount > 0)
	{
		CTsField* pStart = &arData[TS_GROUND_WIDTH * TS_GROUND_HEIGHT - nLineCount * TS_GROUND_WIDTH];
		m_garbageCreator.GetGarbage(pStart, nLineCount * TS_GROUND_WIDTH);
	}

	CTsGameControllerBase* pController = m_controllerMgr.GetSingleController();
	_ASSERTE(pController != NULL);

	pController->SetInitDisplayData(arData, TS_GROUND_WIDTH * TS_GROUND_HEIGHT);

}


LRESULT CTsSinglePlayerEngine::OnDisplayEvent(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	TsControlHeader* pTh = (TsControlHeader*)wParam;
	_ASSERTE(pTh != NULL);	
	
	switch(pTh->nControlCode)
	{
	case TS_CONTROL_START:
		{
			break;
		}

	case TS_CONTROL_END:
		{
			TS_DISPLAY_RESULT eRet(NONE_RESULT);
			if(lParam == 1) //dead
			{
				eRet = LOST_RESULT;
			}

			SetControllerWin(pTh->dwIDFrom, eRet);
			Stop();
			break;
		}
	case TS_CONTROL_SET_MOVING_ELE:
		{
			break;
		}

	case TS_CONTROL_REMOVED_LINE:
		{
			break;
		}
	}

	m_controllerMgr.NotifyOtherController(pTh->dwIDFrom, nMsgType, wParam, lParam);

	return S_OK;
}
LRESULT CTsSinglePlayerEngine::OnSettingChanged(UINT, WPARAM, LPARAM, BOOL&)
{
	CTsGameControllerBase* pController = m_controllerMgr.GetSingleController();
	_ASSERTE(pController != NULL);
	INT nID = pController->GetID();

	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);

	LPCTSTR lpszName = NULL;
	if(pContext->GetUseWinType()== USE_LEFT_WIN)
	{
		lpszName = pContext->GetLeftUserName();
	}
	else
	{
		lpszName = pContext->GetRightUserName();
	}

	CTsUserListData* pUserList = TsGetUserList();
	_ASSERTE(pUserList != NULL);
	CTsUserItem* pUser = pUserList->GetUserById(nID);
	_ASSERTE(pUser != NULL);
	if(pUser != NULL)
	{
		lstrcpy(pUser->m_szUserName, lpszName);
		pUserList->NotifyUserChanged();
	}
	
	return S_OK;
}
/////////////////////////  CTsMultiPlayerEngine  ///////////////////////////
void CTsMultiPlayerEngine::_SetInitDataForController()
{
	CTsField arData[TS_GROUND_WIDTH * TS_GROUND_HEIGHT];
	
	CTsGameBaseInfo* pInfo = TsGetGameBaseInfo();
	_ASSERTE(pInfo != NULL);

	INT nLineCount = pInfo->GetStartLineCount();
	if(nLineCount > 0)
	{
		CTsField* pStart = &arData[TS_GROUND_WIDTH * TS_GROUND_HEIGHT - nLineCount * TS_GROUND_WIDTH];
		m_garbageCreator.GetGarbage(pStart, nLineCount * TS_GROUND_WIDTH);
	}

	CTsGameControllerBase* pLeftController = m_controllerMgr.GetLeftController();
	_ASSERTE(pLeftController != NULL);
	CTsGameControllerBase* pRightController = m_controllerMgr.GetRightController();
	_ASSERTE(pRightController != NULL);

	if(pLeftController != NULL)
	{
		pLeftController->SetInitDisplayData(arData, TS_GROUND_WIDTH * TS_GROUND_HEIGHT);
	}
	if(pRightController != NULL)
	{
		pRightController->SetInitDisplayData(arData, TS_GROUND_WIDTH * TS_GROUND_HEIGHT);
	}
}
void CTsMultiPlayerEngine::_SetRndSeedForController()
{
	DWORD dwTickCount = GetTickCount();
	CTsGameControllerBase* pLeftController = m_controllerMgr.GetLeftController();
	_ASSERTE(pLeftController != NULL);
	CTsGameControllerBase* pRightController = m_controllerMgr.GetRightController();
	_ASSERTE(pRightController != NULL);

	if(pLeftController != NULL)
	{
		pLeftController->SetElementCreator(dwTickCount);
	}
	if(pRightController != NULL)
	{
		pRightController->SetElementCreator(dwTickCount);
	}
}
void CTsMultiPlayerEngine::_SetMoveDownInterval()
{
	INT nInterval = _GetHardLevelInterval();

	CTsGameControllerBase* pLeftController = m_controllerMgr.GetLeftController();
	CTsGameControllerBase* pRightController = m_controllerMgr.GetRightController();
	_ASSERTE(pLeftController != NULL);
	_ASSERTE(pRightController != NULL);

	if(pLeftController != NULL)
	{
		pLeftController->SetMoveDownInterval(nInterval);
	}
	if(pRightController != NULL)
	{
		pRightController->SetMoveDownInterval(nInterval);
	}
}

LRESULT CTsMultiPlayerEngine::OnDisplayEvent(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	TsControlHeader* pTh = (TsControlHeader*)wParam;
	_ASSERTE(pTh != NULL);	
	
	static BOOL s_bFirstEnd = FALSE;

	switch(pTh->nControlCode)
	{
	case TS_CONTROL_START:
		{
			s_bFirstEnd = FALSE;
			break;
		}

	case TS_CONTROL_END:
		{
			_OnControlEnd(wParam, lParam, !s_bFirstEnd);
			s_bFirstEnd = TRUE;
			break;
		}
	case TS_CONTROL_SET_MOVING_ELE:
		{
			break;
		}

	case TS_CONTROL_REMOVED_LINE:
		{
			_OnRemovedLine(wParam, lParam);
			break;
		}
	}

	m_controllerMgr.NotifyOtherController(pTh->dwIDFrom, nMsgType, wParam, lParam);

	return S_OK;
}
void CTsMultiPlayerEngine::_OnRemovedLine(WPARAM wParam ,LPARAM lParam)
{
	TsControlHeader* pTh = (TsControlHeader*)wParam;
	_ASSERTE(pTh != NULL);
	
	INT nIDFrom = pTh->dwIDFrom;
	INT nRemovedLine = pTh->dwUserData;

	INT nGarbageLine = ITsScoreMgr::GetGarbageLineCount(nRemovedLine);
	if(nGarbageLine <= 0)  return;

	CTsGameControllerBase* pLeftController = m_controllerMgr.GetLeftController();
	CTsGameControllerBase* pRightController = m_controllerMgr.GetRightController();
	_ASSERTE(pLeftController != NULL && pRightController != NULL);
	
	CTsField arBuffer[ 4 * TS_GROUND_WIDTH ]; //max is 4
	_ASSERTE(nGarbageLine <= 4);

	INT nCount = TS_GROUND_WIDTH * nGarbageLine;
	m_garbageCreator.GetGarbage(arBuffer, nCount);

	if(pLeftController->GetID() == nIDFrom)
	{
		pRightController->ProduceGarbage(arBuffer, nCount);
	}
	else if(pRightController->GetID() == nIDFrom)
	{
		pLeftController->ProduceGarbage(arBuffer, nCount);
	}
	else
	{
		_ASSERTE(FALSE);
	}
}
void CTsMultiPlayerEngine::_OnControlEnd(WPARAM wParam, LPARAM lParam, BOOL bFirst)
{
	TsControlHeader* pTh = (TsControlHeader*)wParam;
	_ASSERTE(pTh != NULL);	

	INT nIDFrom = pTh->dwIDFrom;
	BOOL bDead = (BOOL)lParam;

	CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
	_ASSERTE(pBaseInfo != NULL);

	CTsGameControllerBase* pLeftController = m_controllerMgr.GetLeftController();
	CTsGameControllerBase* pRightController = m_controllerMgr.GetRightController();
	_ASSERTE(pLeftController != NULL && pRightController != NULL);


	if(pBaseInfo->GetGameStatus() != IDLE_STATUS && bFirst)
	{
		if(!bDead)
		{
			pLeftController->SetPlayResult(NONE_RESULT);
			pRightController->SetPlayResult(NONE_RESULT);
		}
		else
		{
			if(pLeftController->GetID() == nIDFrom)
			{
				pLeftController->SetPlayResult(LOST_RESULT);
				pRightController->SetPlayResult(WIN_RESULT);
			}
			else if(pRightController->GetID() == nIDFrom)
			{
				pRightController->SetPlayResult(LOST_RESULT);
				pLeftController->SetPlayResult(WIN_RESULT);
			}
			else
			{
				_ASSERTE(FALSE);
			}
		}

		Stop();
	}
}
/////////////////////////  CTsTwoPlayerEngine  ///////////////////////////
TS_GAME_MODE CTsTwoPlayerEngine::GetTypeCode()
{
	return TWO_PLAYER_MODE;
}
LRESULT CTsTwoPlayerEngine::OnSettingChanged(UINT, WPARAM, LPARAM, BOOL&)
{
	CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
	_ASSERTE(pPalyInfo != NULL);

	CTsGameControllerBase* pController = m_controllerMgr.GetLeftController();
	_ASSERTE(pController != NULL);
	INT nLeftID = pController->GetID();

	pController = m_controllerMgr.GetRightController();
	_ASSERTE(pController != NULL);
	INT nRightId = pController->GetID();

	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);

	CTsUserListData* pUserList = TsGetUserList();
	_ASSERTE(pUserList != NULL);
	CTsUserItem* pUser = pUserList->GetUserById(nLeftID);
	_ASSERTE(pUser != NULL);
	if(pUser != NULL)
	{
		lstrcpy(pUser->m_szUserName, pContext->GetLeftUserName());
		pPalyInfo->SetLeftPlayerName(pContext->GetLeftUserName());
	
	}
	pUser = pUserList->GetUserById(nRightId);
	_ASSERTE(pUser != NULL);
	if(pUser != NULL)
	{
		lstrcpy(pUser->m_szUserName, pContext->GetRightUserName());
		pPalyInfo->SetRightPlayerName(pContext->GetRightUserName());
	}

	pUserList->NotifyUserChanged();

	return S_OK;
}
BOOL CTsTwoPlayerEngine::_CreateController()
{
	_ASSERTE(m_controllerMgr.GetLeftController() == NULL 
		&& m_controllerMgr.GetRightController() == NULL);

	m_controllerMgr.CreateLeftUserController();
	m_controllerMgr.CreateRightUserController();

	_ASSERTE(m_controllerMgr.GetLeftController() != NULL 
		&& m_controllerMgr.GetRightController() != NULL);

	m_controllerMgr.GetLeftController()->SetEngineMap(this);
	m_controllerMgr.GetRightController()->SetEngineMap(this);

	m_controllerMgr	.CreateMultiScoreListener();
	m_controllerMgr.CreateSoundListener();

	return TRUE;
}
BOOL CTsTwoPlayerEngine::_AttachWindowToController()
{
	CTsGameControllerBase* pLeftController = m_controllerMgr.GetLeftController();
	CTsGameControllerBase* pRightController = m_controllerMgr.GetRightController();
	_ASSERTE(pLeftController != NULL && pRightController != NULL);

	CTsGameMainWnd* pMainWnd = TsGetGameMainWnd();
	_ASSERTE(pMainWnd != NULL);
	HWND hLeftDisplay = pMainWnd->GetLeftDisplayWnd();
	HWND hRightDisplay = pMainWnd->GetRightDisplayWnd();
	HWND hLeftNext = pMainWnd->GetLeftNextWnd();
	HWND hRightNext = pMainWnd->GetRightNextWnd();

	if(pLeftController != NULL)
	{
		pLeftController->SetDisplayWnd(hLeftDisplay);
		pLeftController->SetNextElementWnd(hLeftNext);
	}

	if(pRightController != NULL)
	{
		pRightController->SetDisplayWnd(hRightDisplay);
		pRightController->SetNextElementWnd(hRightNext);
	}

	return TRUE;
}




////////////////////////  CTsPlayWithComputerEngine  /////////////////////////////////////
TS_GAME_MODE CTsPlayWithComputerEngine::GetTypeCode()
{
	return COMPUTER_PLAYER_MODE;
}
BOOL CTsPlayWithComputerEngine::_CreateController()
{
	_ASSERTE(m_controllerMgr.GetLeftController() == NULL 
		&& m_controllerMgr.GetRightController() == NULL);

	m_controllerMgr.CreateSingleController();
	CTsGameControllerBase* pController = m_controllerMgr.GetSingleController();
	_ASSERTE(pController != NULL);
	if(pController->GetID() == LEFT_CONTROLLER_ID)
	{
		m_controllerMgr.CreateRightAIController();
	}
	else
	{
		m_controllerMgr.CreateLeftAIController();
	}
	

	_ASSERTE(m_controllerMgr.GetLeftController() != NULL 
		&& m_controllerMgr.GetRightController() != NULL);

	m_controllerMgr.GetLeftController()->SetEngineMap(this);
	m_controllerMgr.GetRightController()->SetEngineMap(this);

	m_controllerMgr	.CreateMultiScoreListener();
	m_controllerMgr.CreateSoundListener();

	return TRUE;
}
BOOL CTsPlayWithComputerEngine::_AttachWindowToController()
{
	CTsGameControllerBase* pAIController = m_controllerMgr.GetAIController();
	CTsGameControllerBase* pUserController = m_controllerMgr.GetSingleController();
	_ASSERTE(pAIController != NULL && pUserController != NULL);

	CTsGameMainWnd* pMainWnd = TsGetGameMainWnd();
	_ASSERTE(pMainWnd != NULL);
	HWND hLeftDisplay = pMainWnd->GetLeftDisplayWnd();
	HWND hRightDisplay = pMainWnd->GetRightDisplayWnd();
	HWND hLeftNext = pMainWnd->GetLeftNextWnd();
	HWND hRightNext = pMainWnd->GetRightNextWnd();

	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);
	BOOL bUseLeft = (pContext->GetUseWinType() == USE_LEFT_WIN);

	if(!bUseLeft)
	{
		pAIController->SetDisplayWnd(hLeftDisplay);
		pAIController->SetNextElementWnd(hLeftNext);
		pUserController->SetDisplayWnd(hRightDisplay);
		pUserController->SetNextElementWnd(hRightNext);
	}
	else
	{
		pAIController->SetDisplayWnd(hRightDisplay);
		pAIController->SetNextElementWnd(hRightNext);
		pUserController->SetDisplayWnd(hLeftDisplay);
		pUserController->SetNextElementWnd(hLeftNext);
	}


	return TRUE;
}

LRESULT CTsPlayWithComputerEngine::OnSettingChanged(UINT, WPARAM, LPARAM, BOOL&)
{
	CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
	_ASSERTE(pPalyInfo != NULL);


	CTsGameControllerBase* pController = m_controllerMgr.GetSingleController();
	_ASSERTE(pController != NULL);
	INT nID = pController->GetID();

	CTsGameControllerBase* pAI = m_controllerMgr.GetAIController();
	_ASSERTE(pAI != NULL);
	INT nAIId = pAI->GetID();
	CTsUserItem* pItem = TsGetUserList()->GetUserById(nAIId);
	_ASSERTE(pItem != NULL);
	LPCTSTR lpszAIName = pItem->m_szUserName;

	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);

	LPCTSTR lpszName = NULL;
	if(pContext->GetUseWinType()== USE_LEFT_WIN)
	{
		lpszName = pContext->GetLeftUserName();
		pPalyInfo->SetLeftPlayerName(lpszName);
		pPalyInfo->SetRightPlayerName(lpszAIName);
	}
	else
	{
		lpszName = pContext->GetRightUserName();
		pPalyInfo->SetRightPlayerName(lpszName);
		pPalyInfo->SetLeftPlayerName(lpszAIName);
	}

	CTsUserListData* pUserList = TsGetUserList();
	_ASSERTE(pUserList != NULL);
	CTsUserItem* pUser = pUserList->GetUserById(nID);
	_ASSERTE(pUser != NULL);
	if(pUser != NULL)
	{
		lstrcpy(pUser->m_szUserName, lpszName);
		pUserList->NotifyUserChanged();
	}
	
	return S_OK;
}