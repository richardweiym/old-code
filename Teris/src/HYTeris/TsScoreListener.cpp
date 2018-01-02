#include "TsScoreListener.h"
#include "TsScoreMgr.h"
#include "TsGameEngine.h"

///////////////////////  CTsScoreListenerBase  ////////////////////
CTsScoreListenerBase::CTsScoreListenerBase()
{
	m_pScoreMgr = NULL;
}

CTsScoreListenerBase::~CTsScoreListenerBase()
{
	_ASSERTE(m_pScoreMgr == NULL);
}

TS_CONTROLLER_TYPE CTsScoreListenerBase::GetTypeCode()
{
	return GAME_SCORE_LISTENER;
}

///////////////////////  CTsSingleScoreListener  ////////////////////
BOOL CTsSingleScoreListener::Initialize()
{
	CTsControlListener::Initialize();

	_ASSERTE(m_pScoreMgr == NULL);
	m_pScoreMgr = new CTsSingleScoreMgr;
	_ASSERTE(m_pScoreMgr != NULL);

	if(m_pScoreMgr == NULL)
	{
		return FALSE;
	}

	if(m_pScoreMgr != NULL)
	{
		m_pScoreMgr->OnInitializeEngine();
	}

	return TRUE;
}
VOID CTsSingleScoreListener::UnInitialize()
{
	if(m_pScoreMgr != NULL)
	{
		delete m_pScoreMgr;
		m_pScoreMgr = NULL;
	}

	CTsControlListener::UnInitialize();
}
	
BOOL CTsSingleScoreListener::Start()
{
	CTsControlListener::Start();

	if(m_pScoreMgr != NULL)
	{
		m_pScoreMgr->OnGameStart();
	}
	else
	{
		_ASSERTE(FALSE);
	}

	return TRUE;
}


LRESULT CTsSingleScoreListener::OnDisplayEvent(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	TsControlHeader* pTh = (TsControlHeader*)wParam;
	_ASSERTE(pTh != NULL);	
	
	switch(pTh->nControlCode)
	{
	case TS_CONTROL_SET_MOVING_ELE:
		{
			CTsGameEngineBase* pEngine = GetCurrentEngine();
			if(pEngine != NULL)
			{
				CTsControllerMgr* pControlMgr = pEngine->GetControllerMgr();
				_ASSERTE(pControlMgr != NULL);

				CTsGameControllerBase* pController = pControlMgr->GetControllerById(pTh->dwIDFrom);
				if(pController != NULL )
				{
					m_pScoreMgr->OnNewElement(((CTsGameControllerBase*)pController)->GetDisplayWnd());
				}
			}
			break;
		}

	case TS_CONTROL_REMOVED_LINE:
		{
			CTsGameEngineBase* pEngine = GetCurrentEngine();
			if(pEngine != NULL)
			{
				CTsControllerMgr* pControlMgr = pEngine->GetControllerMgr();
				_ASSERTE(pControlMgr != NULL);

				CTsGameControllerBase* pController = pControlMgr->GetControllerById(pTh->dwIDFrom);
				if(pController != NULL )
				{
					m_pScoreMgr->OnRemovedLine(((CTsGameControllerBase*)pController)->GetDisplayWnd(), pTh->dwUserData);
				}
			}
			break;
		}
	}
	
	return S_OK;
}

//////////////////   CTsMultiScoreListener  /////////////////
BOOL CTsMultiScoreListener::Initialize()
{
	CTsControlListener::Initialize();

	_ASSERTE(m_pScoreMgr == NULL);
	m_pScoreMgr = new CTsMultiScoreMgr;
	_ASSERTE(m_pScoreMgr != NULL);

	if(m_pScoreMgr == NULL)
	{
		return FALSE;
	}

	if(m_pScoreMgr != NULL)
	{
		m_pScoreMgr->OnInitializeEngine();
	}

	return TRUE;
}
VOID CTsMultiScoreListener::UnInitialize()
{
	if(m_pScoreMgr != NULL)
	{
		delete m_pScoreMgr;
		m_pScoreMgr = NULL;
	}

	CTsControlListener::UnInitialize();
}
	
BOOL CTsMultiScoreListener::Start()
{
	CTsControlListener::Start();

	if(m_pScoreMgr != NULL)
	{
		m_pScoreMgr->OnGameStart();
	}
	else
	{
		_ASSERTE(FALSE);
	}

	return TRUE;
}

LRESULT CTsMultiScoreListener::OnDisplayEvent(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	TsControlHeader* pTh = (TsControlHeader*)wParam;
	_ASSERTE(pTh != NULL);	
	
	switch(pTh->nControlCode)
	{
	case TS_CONTROL_SET_MOVING_ELE:
		{
			CTsGameEngineBase* pEngine = GetCurrentEngine();
			if(pEngine != NULL)
			{
				CTsControllerMgr* pControlMgr = pEngine->GetControllerMgr();
				_ASSERTE(pControlMgr != NULL);

				CTsGameControllerBase* pController = pControlMgr->GetControllerById(pTh->dwIDFrom);
				if(pController != NULL )
				{
					m_pScoreMgr->OnNewElement(((CTsGameControllerBase*)pController)->GetDisplayWnd());
				}
			}
			break;
		}

	case TS_CONTROL_REMOVED_LINE:
		{
			CTsGameEngineBase* pEngine = GetCurrentEngine();
			if(pEngine != NULL)
			{
				CTsControllerMgr* pControlMgr = pEngine->GetControllerMgr();
				_ASSERTE(pControlMgr != NULL);

				CTsGameControllerBase* pController = pControlMgr->GetControllerById(pTh->dwIDFrom);
				if(pController != NULL )
				{
					m_pScoreMgr->OnRemovedLine(((CTsGameControllerBase*)pController)->GetDisplayWnd(), pTh->dwUserData);
				}
			}
			break;
		}

	case TS_CONTROL_SET_RESULT:
		{
			TS_DISPLAY_RESULT eResult = (TS_DISPLAY_RESULT)lParam;
			if(eResult == WIN_RESULT)
			{
				CTsGameEngineBase* pEngine = GetCurrentEngine();
				if(pEngine != NULL)
				{
					CTsControllerMgr* pControlMgr = pEngine->GetControllerMgr();
					_ASSERTE(pControlMgr != NULL);

					CTsGameControllerBase* pController = pControlMgr->GetControllerById(pTh->dwIDFrom);
					if(pController != NULL)
					{
						m_pScoreMgr->OnWinGame(((CTsGameControllerBase*)pController)->GetDisplayWnd());
					}
				}
			}

			break;
		}
	}
	
	return S_OK;
}	