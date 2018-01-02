#include "TsPlayerController.h"
#include "TsUIInstanceApi.h"
#include "TsGameMainWnd.h"
#include "TsDataKernel.h"
#include "TsGameContext.h"
#include "TsSetupDlg.h"
#include "TsGameEngine.h"



BOOL CTsPlayerController::Initialize()
{
	_ASSERTE(!m_bInited);
	CTsGameControllerBase::Initialize();

	m_bInited = TRUE;
	
	return TRUE;
}

VOID CTsPlayerController::UnInitialize()
{
	CTsGameControllerBase::UnInitialize();

	m_bInited = FALSE;
}

void CTsPlayerController::_AttachKeyEvents()
{
	CTsGameMainWnd* pMainWnd = TsGetGameMainWnd();
	_ASSERTE(pMainWnd != NULL);
	pMainWnd->AddKeyEventsListener(this);

	HWND hLeftDisplay = pMainWnd->GetLeftDisplayWnd();
	HWND hRightDisplay = pMainWnd->GetRightDisplayWnd();

	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);
	
	_ASSERTE(m_hWndDisplay != NULL);

	if(m_hWndDisplay != NULL) 
	{	
		if(m_hWndDisplay == hLeftDisplay)
		{
			m_pControlKeys = pContext->GetLeftWinControlKeys();
		}
		else if(m_hWndDisplay == hRightDisplay)
		{
			m_pControlKeys = pContext->GetRightWinControlKeyS();
		}
		else
		{
			_ASSERTE(FALSE);
		}
	}
	else
	{
		_ASSERTE(FALSE);
	}
}



void CTsPlayerController::_DetachKeyEvents()
{
	CTsGameMainWnd* pMainWnd = TsGetGameMainWnd();
	_ASSERTE(pMainWnd != NULL);
	pMainWnd->RemoveKeyEventsListener(this);
	
	m_pControlKeys = NULL;
}

TS_CONTROLLER_TYPE CTsPlayerController::GetTypeCode()
{
	return PLAYER_CONTROLLER;
}

void CTsPlayerController::OnTimer()
{
	if(m_eStatus == RUNNING_STATUS)
	{
		m_nDownCurrent += TS_TIMER_INTERVAL;
		if(m_nDownCurrent >= m_nDownInterval)
		{
			m_nDownCurrent = 0;
			_SendControlComamnd(TS_CONTROL_MOVE_DOWN);
		}
	}
}



BOOL CTsPlayerController::Start()
{
	_ASSERTE(m_bInited);
	
	CTsGameControllerBase::Start();

	_AttachKeyEvents();
	_AttachDisplayWnd();

	_InitData();

	_SendControlComamnd(TS_CONTROL_START);

	m_nDownCurrent = 0;

	_ASSERTE(m_eStatus == RUNNING_STATUS);

	return TRUE;
}
void CTsPlayerController::Stop()
{
	if(m_eStatus != IDLE_STATUS)
	{
		CTsGameControllerBase::Stop();

		_SendControlComamnd(TS_CONTROL_END);
	
		_DetachKeyEvents();
	//	_DetachDisplayWnd();
	}
}

BOOL CTsPlayerController::Pause()
{
	_ASSERTE(m_eStatus == RUNNING_STATUS);
	if(m_eStatus == RUNNING_STATUS)
	{
		CTsGameControllerBase::Pause();

		m_nDownCurrent = 0;
	}

	return TRUE;
}

void CTsPlayerController::Resume()
{
	_ASSERTE(m_eStatus == PAUSE_STATUS);
	if(m_eStatus == PAUSE_STATUS )
	{
		CTsGameControllerBase::Resume();

		m_nDownCurrent = 0;
	}
}

LRESULT CTsPlayerController::OnDisplayEvent(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TsControlHeader* pTh = (TsControlHeader*)wParam;
	_ASSERTE(pTh != NULL);

	if((pTh->dwIDFrom == m_nID))
	{
		switch(pTh->nControlCode)
		{
		case TS_CONTROL_REQUEST_ELEMENT:
			_SendSetMovingEleCommand();
			break;

		case TS_CONTROL_END:
			Stop();
			break;

		case TS_CONTROL_SET_RESULT:
			_DetachDisplayWnd();
			break;
		}
	}

	CTsGameControllerBase::OnDisplayEvent(nMsgType, wParam, lParam, bHandled);
	
	return S_OK;
}
LRESULT CTsPlayerController::OnKeyDown(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if(m_pControlKeys == NULL || m_eStatus != RUNNING_STATUS) return S_OK;
	
	CTsKeysTable* pKeysTable = TsGetKeysTable();
	_ASSERTE(pKeysTable != NULL);

	CTsKeysTable& tk = *pKeysTable;
	
	if(wParam == 0xE5)
	{
		INT nMASK = 0x00FF0000;
		INT nValue = nMASK & lParam;
		nValue >>= 16;
		
		wParam = tk.MapOEM2VK(nValue);
	}

	INT eType(TS_CONTROL_NONE);
	if(wParam == m_pControlKeys->m_nMoveLeft)
	{
		eType = TS_CONTROL_MOVE_LEFT;
	}
	else if(wParam == m_pControlKeys->m_nMoveRight)
	{
		eType = TS_CONTROL_MOVE_RIGHT;
	}
	else if(wParam ==  m_pControlKeys->m_nMoveSwitch)
	{
			if(m_pControlKeys->m_nSwitchDirection == CLOCK_WISE)
			{
				eType = TS_CONTROL_SWITCH_RIGHT;
			}
			else
			{
				eType = TS_CONTROL_SWITCH_LEFT;
			}
	}
	else if(wParam == m_pControlKeys->m_nMoveDown)
	{
		eType = TS_CONTROL_MOVE_DOWN;
	}
	else if(wParam == m_pControlKeys->m_nDropDown)
	{
		eType = TS_CONTROL_DROP_DOWN;
	}
	

	if(eType != TS_CONTROL_NONE)
	{
		_SendControlComamnd(eType);	
	}

	return S_OK;
}