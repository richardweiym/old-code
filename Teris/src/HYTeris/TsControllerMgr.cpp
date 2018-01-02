#include "TsControllerMgr.h"
#include "TsPlayerController.h"
#include "TsScoreListener.h"
#include "TsSoundListener.h"
#include "TsComputerAIPlayer.h"

#include "TsUIInstanceApi.h"
#include "TsUIDataMgr.h"
#include "TsGameContext.h"
#include "TsResMgr.h"
#include "TerisRes.h"

using namespace std;

CTsControllerMgr::CTsControllerMgr()
{
	m_pController1 = NULL;
	m_pController2 = NULL;
}

CTsControllerMgr::~CTsControllerMgr()
{
	DeleteController();
}
BOOL CTsControllerMgr::Start()
{
	BOOL bRet(TRUE);
	if(m_pController1 != NULL)
	{
		bRet = m_pController1->Start();
		if(!bRet)
		{
			return FALSE;
		}
	}


	if(m_pController2 != NULL)
	{
		bRet = m_pController2->Start();
		if(!bRet)
		{
			return FALSE;
		}
	}


	int nCount = m_arObserver.size();
	for(int i=0; i<nCount; ++i)
	{
		CTsControlListener* pController = m_arObserver[i];
		_ASSERTE(pController != NULL);
		if(pController != NULL)
		{
			bRet = pController->Start();
			if(!bRet)
			{
				break;
			}
		}
	}

	return bRet;
}

void CTsControllerMgr::Stop()
{
	if(m_pController1 != NULL)
	{
		m_pController1->Stop();
	}

	if(m_pController2 != NULL)
	{
		m_pController2->Stop();
	}

	int nCount = m_arObserver.size();
	for(int i=0; i<nCount; ++i)
	{
		CTsControlListener* pController = m_arObserver[i];
		_ASSERTE(pController != NULL);
		if(pController != NULL)
		{
			pController->Stop();
		}
	}
}

BOOL CTsControllerMgr::Pause()
{
	BOOL bRet(TRUE);
	if(m_pController1 != NULL)
	{
		bRet = m_pController1->Pause();
		if(!bRet)
		{
			return FALSE;
		}
	}


	if(m_pController2 != NULL)
	{
		bRet = m_pController2->Pause();
		if(!bRet)
		{
			return FALSE;
		}
	}


	int nCount = m_arObserver.size();
	for(int i=0; i<nCount; ++i)
	{
		CTsControlListener* pController = m_arObserver[i];
		_ASSERTE(pController != NULL);
		if(pController != NULL)
		{
			bRet = pController->Pause();
			if(!bRet)
			{
				break;
			}
		}
	}

	return bRet;
}

void CTsControllerMgr::Resume()
{
	if(m_pController1 != NULL)
	{
		m_pController1->Resume();
	}

	if(m_pController2 != NULL)
	{
		m_pController2->Resume();
	}

	int nCount = m_arObserver.size();
	for(int i=0; i<nCount; ++i)
	{
		CTsControlListener* pController = m_arObserver[i];
		_ASSERTE(pController != NULL);
		if(pController != NULL)
		{
			pController->Resume();
		}
	}
}
BOOL CTsControllerMgr::Initialize()
{
	BOOL bRet(TRUE);
	if(m_pController1 != NULL)
	{
		_ASSERTE(!m_pController1->IsInited());
		bRet = m_pController1->Initialize();
	}
	if(!bRet)
	{
		return FALSE;
	}


	if(m_pController2 != NULL)
	{
		_ASSERTE(!m_pController2->IsInited());
		bRet = m_pController2->Initialize();
	}
	if(!bRet)
	{
		return FALSE;
	}

	INT nCount = m_arObserver.size();
	for(int i=0; i<nCount; ++i)
	{
		CTsControlListener* pController = m_arObserver[i];
		_ASSERTE(pController != NULL);
		if(pController != NULL)
		{
			_ASSERTE(!pController->IsInited());
			bRet = pController->Initialize();
		}

		if(!bRet)
		{
			break;
		}
	}
	
	return bRet;
}

VOID CTsControllerMgr::UnInitialize()
{
	if(m_pController1 != NULL)
	{
		m_pController1->UnInitialize();
	}

	if(m_pController2 != NULL)
	{
		m_pController2->UnInitialize();
	}

	int nCount = m_arObserver.size();
	for(int i=0; i<nCount; ++i)
	{
		CTsControlListener* pController = m_arObserver[i];
		_ASSERTE(pController != NULL);
		if(pController != NULL)
		{
			pController->UnInitialize();
		}
	}
}

BOOL CTsControllerMgr::CreateLeftUserController()
{
	_ASSERTE(m_pController1 == NULL);
	if(m_pController1 == NULL)
	{
		m_pController1 = new CTsPlayerController;
		m_pController1->SetID(LEFT_CONTROLLER_ID);
		
		CTsGameContext* pContext = TsGetGameContext();
		_ASSERTE(pContext != NULL);


		CTsUserListData* pUserList = TsGetUserList();
		_ASSERTE(pUserList != NULL);
		if(pUserList != NULL)
		{
			pUserList->AddUser(LEFT_CONTROLLER_ID, LOCAL_CONTROL_TYPE, pContext->GetLeftUserName());
		}

		CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
		_ASSERTE(pPalyInfo != NULL);
		pPalyInfo->SetLeftPlayerName(pContext->GetLeftUserName());

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CTsControllerMgr::CreateRightUserController()
{
	_ASSERTE(m_pController2 == NULL);
	if(m_pController2 == NULL)
	{
		m_pController2 = new CTsPlayerController;
		m_pController2->SetID(RIGHT_CONTROLLER_ID);	

		CTsGameContext* pContext = TsGetGameContext();
		_ASSERTE(pContext != NULL);

		CTsUserListData* pUserList = TsGetUserList();
		_ASSERTE(pUserList != NULL);
		if(pUserList != NULL)
		{
			pUserList->AddUser(RIGHT_CONTROLLER_ID, LOCAL_CONTROL_TYPE, pContext->GetRightUserName());
		}
		CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
		_ASSERTE(pPalyInfo != NULL);
		pPalyInfo->SetRightPlayerName(pContext->GetRightUserName());

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CTsControllerMgr::CreateLeftAIController()
{
	_ASSERTE(m_pController1 == NULL);
	if(m_pController1 == NULL)
	{
		m_pController1 = new CTsComputerController;
		m_pController1->SetID(COMPUTER_AI_CONTROLLER_ID);
		
		CTsResMgr* pResMgr = TsGetResMgr();	
		TCHAR szAIName[64] = {0};
		pResMgr->LoadString(IDS_COMPUTER_NAME, szAIName, 60);

		CTsUserListData* pUserList = TsGetUserList();
		_ASSERTE(pUserList != NULL);
		if(pUserList != NULL)
		{
			pUserList->AddUser(COMPUTER_AI_CONTROLLER_ID, COMPUTER_TYPE, szAIName);
		}

		CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
		_ASSERTE(pPalyInfo != NULL);
		pPalyInfo->SetLeftPlayerName(szAIName);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CTsControllerMgr::CreateRightAIController()
{
	_ASSERTE(m_pController2 == NULL);
	if(m_pController2 == NULL)
	{
		m_pController2 = new CTsComputerController;
		m_pController2->SetID(COMPUTER_AI_CONTROLLER_ID);

		CTsResMgr* pResMgr = TsGetResMgr();	
		TCHAR szAIName[64] = {0};
		pResMgr->LoadString(IDS_COMPUTER_NAME, szAIName, 60);

		CTsUserListData* pUserList = TsGetUserList();
		_ASSERTE(pUserList != NULL);
		if(pUserList != NULL)
		{
			pUserList->AddUser(COMPUTER_AI_CONTROLLER_ID, COMPUTER_TYPE, szAIName);
		}

		CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
		_ASSERTE(pPalyInfo != NULL);
		pPalyInfo->SetRightPlayerName(szAIName);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CTsControllerMgr::CreateSingleController()
{
	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);
	
	BOOL bUseLeft = (pContext->GetUseWinType() == USE_LEFT_WIN);
	if(bUseLeft)
	{
		return CreateLeftUserController();
	}
	else
	{
		return CreateRightUserController();
	}
}
BOOL CTsControllerMgr::CreateSingleScoreListener()
{
	_ASSERTE(GetListenerById(GAME_SCORE_LISTENER) == NULL);

	CTsControlListener* pScoreController = new CTsSingleScoreListener;
	_ASSERTE(pScoreController != NULL);
	if(pScoreController != NULL)
	{
		pScoreController->SetID(SCORE_LISTENER_ID);
		m_arObserver.push_back(pScoreController);
		return TRUE;
	}

	return FALSE;
}
BOOL CTsControllerMgr::CreateMultiScoreListener()
{
	_ASSERTE(GetListenerById(GAME_SCORE_LISTENER) == NULL);

	CTsControlListener* pScoreController = new CTsMultiScoreListener;
	_ASSERTE(pScoreController != NULL);
	if(pScoreController != NULL)
	{
		pScoreController->SetID(SCORE_LISTENER_ID);
		m_arObserver.push_back(pScoreController);
		return TRUE;
	}

	return FALSE;
}
BOOL CTsControllerMgr::CreateSoundListener()
{
	_ASSERTE(GetListenerById(GAME_SCORE_LISTENER) == NULL);

	CTsControlListener* pSoundListener = new CTsSoundListener;
	_ASSERTE(pSoundListener != NULL);
	if(pSoundListener != NULL)
	{
		pSoundListener->SetID(SOUND_LISTENER_ID);
		m_arObserver.push_back(pSoundListener);
		return TRUE;
	}

	return FALSE;
}
CTsGameControllerBase* CTsControllerMgr::GetLeftController()
{
	return m_pController1;
}

CTsGameControllerBase* CTsControllerMgr::GetRightController()
{
	return m_pController2;
}

CTsGameControllerBase* CTsControllerMgr::GetSingleController()
{
	if(m_pController1 != NULL && m_pController1->GetID() == LEFT_CONTROLLER_ID)
	{
		return m_pController1;
	}
	else if(m_pController2 != NULL && m_pController2->GetID() == RIGHT_CONTROLLER_ID)
	{
		return m_pController2;
	}
	else
	{
		return NULL;
	}
}
CTsGameControllerBase* CTsControllerMgr::GetAIController()
{
	if(m_pController1 != NULL && m_pController1->GetID() == COMPUTER_AI_CONTROLLER_ID)
	{
		return m_pController1;
	}
	else if(m_pController2 != NULL && m_pController2->GetID() == COMPUTER_AI_CONTROLLER_ID)
	{
		return m_pController2;
	}
	else
	{
		return NULL;
	}
}

void CTsControllerMgr::DeleteController()
{
	CTsUserListData* pUserList = TsGetUserList();
	_ASSERTE(pUserList != NULL);
	if(pUserList != NULL)
	{
		pUserList->ClearUser();
	}

	if(m_pController1 != NULL)
	{
		_ASSERTE(!m_pController1->IsInited());
		delete m_pController1;
		m_pController1 = NULL;
	}

	if(m_pController2 != NULL)
	{
		_ASSERTE(!m_pController2->IsInited());
		delete m_pController2;
		m_pController2 = NULL;
	}

	int nCount = m_arObserver.size();
	for(int i=0; i<nCount; ++i)
	{
		CTsControlListener* pController = m_arObserver[i];
		_ASSERTE(pController != NULL);
		if(pController != NULL)
		{
			_ASSERTE(!pController->IsInited());
			delete pController;
		}
	}
	m_arObserver.clear();

}

VOID CTsControllerMgr::OnTimer()
{
	if(m_pController1 != NULL)
	{
		m_pController1->OnTimer();
	}

	if(m_pController2 != NULL)
	{
		m_pController2->OnTimer();
	}

	INT nCount = m_arObserver.size();
	for(INT i=0; i<nCount; ++i)
	{
		CTsControlListener* pController = m_arObserver[i];
		pController->OnTimer();
	}
}
void CTsControllerMgr::NotifyOtherController(INT nFromID, UINT nMsgType, WPARAM wParam, LPARAM lParam)
{
	static LRESULT lRet(0);
	if(m_pController1 != NULL && m_pController1->GetID() != nFromID)
	{
		m_pController1->ProcessWindowMessage(NULL, nMsgType, wParam, lParam, lRet, 0);
	}

	if(m_pController2 != NULL && m_pController2->GetID() != nFromID)
	{
		m_pController2->ProcessWindowMessage(NULL, nMsgType, wParam, lParam, lRet, 0);
	}

	INT nCount = m_arObserver.size();
	for(int i=0; i<nCount; ++i)
	{
		CTsControlListener* pController = m_arObserver[i];
		_ASSERTE(pController != NULL);
		if(pController != NULL && pController->GetID() != nFromID)
		{
			pController->ProcessWindowMessage(NULL, nMsgType, wParam, lParam, lRet, 0);
		}		
	}
}

CTsGameControllerBase*  CTsControllerMgr::GetControllerById(INT nID)
{
	CTsGameControllerBase* pRet = NULL;
	if(m_pController1 != NULL && m_pController1->GetID() == nID)
	{
		pRet = m_pController1;
	}
	else if(m_pController2 != NULL && m_pController2->GetID() == nID)
	{
		pRet = m_pController2;
	}


	return pRet;
}

CTsControlListener* CTsControllerMgr::GetListenerById(INT nId)
{
	CTsControlListener* pRet = NULL;
	int nCount = m_arObserver.size();

	for(int i=0; i<nCount; ++i)
	{
		CTsControlListener* pController = m_arObserver[i];
		_ASSERTE(pController != NULL);
		if(pController->GetID() == nId)
		{
			pRet = pController;
			break;
		}
	}

	return pRet;
}
