#include "TsComputerAIPlayer.h"
#include "TsGameEngine.h"
#include "TsDisplayContainer.h"
#include "TsDataKernel.h"
#include "TsUIInstanceApi.h"
#include "TsUIDataMgr.h"


CTsAIBase::CTsAIBase()
{
	m_nCurrentElementIndex = 0;
	m_nNextElementIndex = 0;
	m_pDisplayContainer = NULL;
}
CTsAIBase::~CTsAIBase()
{
	_ASSERTE(m_pDisplayContainer == NULL);
}


BOOL CTsAIBase::Initialize()
{
	if(m_pDisplayContainer == NULL)
	{
		m_pDisplayContainer = new CTsDisplayContainer;
	}
	_ASSERTE(m_pDisplayContainer != NULL);
	if(m_pDisplayContainer != NULL && !m_pDisplayContainer->IsWindow())
	{
		m_pDisplayContainer->Create(NULL, CRect(0, 0, 100, 200), NULL, WS_POPUP, WS_EX_TOOLWINDOW);
		m_pDisplayContainer->SendMessage(WM_TS_SET_DISPLAY_SINK, (WPARAM)this, 1);
		m_pDisplayContainer->SetEnableDisplay(FALSE);
	}
	
	return TRUE;
}

void CTsAIBase::UnInitialize()
{
	if(m_pDisplayContainer != NULL)
	{
		if(m_pDisplayContainer->IsWindow())
		{
			m_pDisplayContainer->SendMessage(WM_TS_SET_DISPLAY_SINK, (WPARAM)this, 0);
			m_pDisplayContainer->DestroyWindow();
		}

		delete m_pDisplayContainer;
		m_pDisplayContainer = NULL;
	}
}

void CTsAIBase::OnTimer()
{

}

BOOL CTsAIBase::Start()
{
	return TRUE;
}

void  CTsAIBase::Stop()
{
	_SendControlComamnd(TS_CONTROL_END, 0);
}

BOOL CTsAIBase::Pause()
{
	return TRUE;
}

void CTsAIBase::Resume()
{

}

void CTsAIBase::SetInitData(CTsField* pData, int nCount)
{
	_ASSERTE(nCount == sizeof(m_arInitData) / sizeof(m_arInitData[0]));

	for(int i=0; i<nCount; ++i)
	{
		m_arInitData[i] = pData[i];
	}

	GetHeightData(m_arInitData, m_arOldHeigh);
	GetGapData(m_arInitData, m_arOldGap);
//	m_nOldDiffCount = GetDiffCount(m_arOldHeigh, TS_GROUND_WIDTH);
}
void CTsAIBase::GetHeightData(CTsField* pFields, INT* pBuffer)
{
	CTsField* pField = NULL;	
	INT nHeight(0);
	for(int i=0; i<TS_GROUND_WIDTH; ++i)
	{
		nHeight = 0;
		for(int j=0; j<TS_GROUND_HEIGHT; ++j)
		{
			pField = &pFields[j*TS_GROUND_WIDTH + i];
			if(pField->IsEmpty())
			{
				++nHeight;
			}
			else
			{
				break;
			}
		}
		
		pBuffer[i] = TS_GROUND_HEIGHT - nHeight;
	}
}
INT CTsAIBase::GetDiffCount(INT * pData, INT nCount)
{
	const INT nThum = 6;
	INT nMaxIndex = GetMaxDataIndex(pData, nCount);
	
	INT nMaxValue = pData[nMaxIndex];

	INT nRet(0);
	for(int i=0; i<nCount; ++i)
	{
		if(nMaxValue - pData[i] > nThum)
		{
			++nRet;
		}
	}

	return nRet;
}
INT CTsAIBase::GetMinDataIndex(INT* pData, INT nCount)
{
	INT nIndex(0);
	int nMin(0x0FFFFFFF);
	for(int i=0; i<nCount; ++i)
	{
		if(pData[i] < nMin)
		{
			nMin = pData[i];
			nIndex = i;
		}
	}

	return nIndex;
}
INT CTsAIBase::GetMaxDataIndex(INT* pData, INT nCount)
{
	INT nIndex(0);
	int nMax(0);
	for(int i=0; i<nCount; ++i)
	{
		if(pData[i] > nMax)
		{
			nMax = pData[i];
			nIndex = i;
		}
	}

	return nIndex;
}
void CTsAIBase::GetGapData(CTsField* pFields, INT* pBuffer)
{
	CTsField* pField = NULL;
	INT nGap(0);
	BOOL bBegin(FALSE);
	for(int i=0; i<TS_GROUND_WIDTH; ++i)
	{
		nGap = 0;
		bBegin = FALSE;
		for(int j=0; j<TS_GROUND_HEIGHT; ++j)
		{
			pField = &pFields[j*TS_GROUND_WIDTH + i];
			if(!pField->IsEmpty())
			{
				bBegin = TRUE;
			}
			else
			{
				if(bBegin)
				{
					++nGap;
				}
			}
		}
		
		pBuffer[i] = nGap;
	}
}


void CTsAIBase::SetCurrentElementIndex(INT nIndex)
{
	m_nCurrentElementIndex = nIndex;
}

void CTsAIBase::SetNextElementIndex(INT nIndex)
{
	m_nNextElementIndex = nIndex;
}
LRESULT CTsAIBase::_SendControlComamnd(INT nType, LPARAM lParam)
{
	TsControlHeader th = {0};
	th.dwIDFrom = SIMULATOR_CONTROLLER_ID;
	th.nControlCode = nType;	
	
	_ASSERTE(m_pDisplayContainer != NULL && m_pDisplayContainer->IsWindow());
	return m_pDisplayContainer->SendMessage( WM_TS_DISPLAY_CONTROL, WPARAM(&th), lParam);
}
//////////////////////////  CTsSimpleAI  ////////////////////
CTsSimpleAI::CTsSimpleAI()
{	
}
void CTsSimpleAI::CheckTempAICommand(CTsAICommand* pCmd)
{
	_SendControlComamnd(TS_CONTROL_END, 0);
	
	INT nCurrentStep(0);
	INT nCurrentSwitch(0);

	BOOL bMoveLeftDone(FALSE);
	BOOL bMoveRightDone(FALSE);
	
	INT nMaxSwitchType(4);
	if(m_nNextElementIndex == 0)
	{
		nMaxSwitchType = 2; 
	}
	else if(m_nNextElementIndex == 1 )
	{
		nMaxSwitchType = 1;
	}

	while(nCurrentSwitch < nMaxSwitchType)
	{
		nCurrentStep = 0;
		bMoveLeftDone  = bMoveRightDone = FALSE;
		
		while(TRUE)
		{
			m_nSecondRemove = 0;
			m_bFirstCheck = FALSE;

			SendTempInitCommand();
			_SendControlComamnd(TS_CONTROL_START, 0);
			SendTempMovingEleCommand();
			SendSwitchCommand(nCurrentSwitch);

			if(!bMoveLeftDone)
			{
				BOOL bOK = SendMoveLeftCommand(nCurrentStep);
				if(!bOK)
				{
					bMoveLeftDone = TRUE;
				}
				else
				{
					_SendControlComamnd(TS_CONTROL_DROP_DOWN, 0);
				}
				_SendControlComamnd(TS_CONTROL_END, 0);

				if(!bMoveLeftDone)
				{
					SendGetDataCommand();
					int nValue = GetCurrentMethodScore();
					if(nValue >= m_nMaxValue)
					{
						m_nMaxValue = nValue;
						*pCmd = m_TempCmd;
					}
					
					nCurrentStep++;
				}
				else
				{
					nCurrentStep = 1;
				}
			}
			else if(!bMoveRightDone)
			{
				BOOL bOK = SendMoveRightCommand(nCurrentStep);
				if(!bOK)
				{
					bMoveRightDone = TRUE;
				}
				else
				{
					_SendControlComamnd(TS_CONTROL_DROP_DOWN, 0);
				}
				_SendControlComamnd(TS_CONTROL_END, 0);

				if(!bMoveRightDone)
				{
					SendGetDataCommand();
					int nValue = GetCurrentMethodScore();
					if(nValue > m_nMaxValue)
					{
						m_nMaxValue = nValue;
						*pCmd = m_TempCmd;
					}
					
					nCurrentStep++;
				}
			}
			else
			{
				_SendControlComamnd(TS_CONTROL_END, 0);
				break;
			}
		}
		
		nCurrentSwitch++;	
	}

}
BOOL CTsSimpleAI::GetBestAICommand(CTsAICommand* pCmd)
{
	_SendControlComamnd(TS_CONTROL_END, 0);

	m_nMaxValue = 0;
	INT nCurrentStep(0);
	INT nCurrentSwitch(0);

	BOOL bMoveLeftDone(FALSE);
	BOOL bMoveRightDone(FALSE);
	
	
	INT nMaxSwitchType(4);
	if(m_nCurrentElementIndex == 0)
	{
		nMaxSwitchType = 2; 
	}
	else if(m_nCurrentElementIndex == 1 )
	{
		nMaxSwitchType = 1;
	}

	while(nCurrentSwitch < nMaxSwitchType)
	{
		nCurrentStep = 0;
		bMoveLeftDone  = bMoveRightDone = FALSE;
		
		while(TRUE)
		{
			m_nFirstRemove =0;
			m_nSecondRemove = 0;
			m_bFirstCheck = TRUE;

			SendInitCommand();
			_SendControlComamnd(TS_CONTROL_START, 0);
			SendMovingEleCommand();
			SendSwitchCommand(nCurrentSwitch);

			if(!bMoveLeftDone)
			{
				BOOL bOK = SendMoveLeftCommand(nCurrentStep);
				if(!bOK)
				{
					bMoveLeftDone = TRUE;
				}
				else
				{
					_SendControlComamnd(TS_CONTROL_DROP_DOWN, 0);
				}
				_SendControlComamnd(TS_CONTROL_END, 0);

				if(!bMoveLeftDone)
				{
					m_TempCmd.m_bMoveLeft = TRUE;
					m_TempCmd.m_nMoveStep = nCurrentStep;
					m_TempCmd.m_nSwitch = nCurrentSwitch;

					SendGetTempDataCommand();

					CheckTempAICommand(pCmd);
					
					nCurrentStep++;
				}
				else
				{
					nCurrentStep = 1;
				}
			}
			else if(!bMoveRightDone)
			{
				BOOL bOK = SendMoveRightCommand(nCurrentStep);
				if(!bOK)
				{
					bMoveRightDone = TRUE;
				}
				else
				{
					_SendControlComamnd(TS_CONTROL_DROP_DOWN, 0);
				}
				_SendControlComamnd(TS_CONTROL_END, 0);

				if(!bMoveRightDone)
				{
					m_TempCmd.m_bMoveLeft = FALSE;
					m_TempCmd.m_nMoveStep = nCurrentStep;
					m_TempCmd.m_nSwitch = nCurrentSwitch;


					SendGetTempDataCommand();

					CheckTempAICommand(pCmd);
		
					
					nCurrentStep++;
				}
			}
			else
			{
				_SendControlComamnd(TS_CONTROL_END, 0);
				break;
			}
		}
		
		nCurrentSwitch++;	
	}

	return TRUE;
}
INT CTsSimpleAI::GetCurrentMethodScore()
{
	INT nGapAdd(0);
	INT nHeightAdd(0);

	INT nOldMaxHeight(0);
	INT nNewMaxHeight(0);
	INT nDiffCount = GetBigDifferCount(m_arCurrentHeight, TS_GROUND_WIDTH, 6);

	m_nRemoveLineCount = m_nFirstRemove + m_nSecondRemove;

	INT nOldHeight, nNewHeigh, nOldGap, nNewGap;
	for(int i=0; i<TS_GROUND_WIDTH; ++i)
	{
		nOldHeight = m_arOldHeigh[i];
		nNewHeigh = m_arCurrentHeight[i];
		nOldGap = m_arOldGap[i];
		nNewGap = m_arCurrentGap[i];
		
		nGapAdd += nNewGap - nOldGap;

		if(nOldHeight >= nOldMaxHeight)
		{
			nOldMaxHeight = nOldHeight;
		}

		if(nNewHeigh >= nNewMaxHeight)
		{
			nNewMaxHeight = nNewHeigh;
		}
	}
	
	int nHeightFactor(0);
	INT nRemoveCountFactor(0);
	INT nGapAddFactor(0);
//	INT nDiffFactor(0);
	INT nExtraValue(0);

	nHeightAdd = nNewMaxHeight - nOldMaxHeight;
	
	if(nNewMaxHeight <= 10)
	{
		nHeightFactor = 1;
		nGapAddFactor = 8;
		if(m_nRemoveLineCount <= 1)
		{
			nRemoveCountFactor = 1;
		}
		else
		{
			nRemoveCountFactor = 30;
		}
		
		if(nDiffCount >= 2)
		{
			nExtraValue = - 60;
		}
		
	}
// 	else if( nNewMaxHeight	 <= 14)
// 	{
// 		nHeightFactor = nNewMaxHeight ;
// 		nGapAddFactor =  0;
// 		nRemoveCountFactor = 30;
// 	}
	else
	{
		nHeightFactor = 30;
		nGapAddFactor = 1;
		nRemoveCountFactor = 20;
	}


	return 10000 + m_nRemoveLineCount * nRemoveCountFactor	- nGapAddFactor * nGapAdd 
		- nHeightAdd * nHeightFactor /*- nDiffFactor * nDiffCount*/ + nExtraValue;
	

}

INT CTsSimpleAI::GetechelonValue()
{
	INT nRet(0);

	INT nIndex = GetMinDataIndex(m_arCurrentHeight, TS_GROUND_WIDTH);
	INT nVal1(0), nVal2(0);
	INT nIndex1(0), nIndex2(0);
	nIndex1 = nIndex - 1;
	nIndex2 = nIndex - 2;
	if(nIndex1 >=0 && nIndex2 >= 0)
	{
		if(m_arCurrentHeight[nIndex2] > m_arCurrentHeight[nIndex1])
		{
			nRet = 10;
		}
	}
	else
	{
		nIndex1 = nIndex + 1;
		nIndex2 = nIndex + 2;
		if(nIndex1 < TS_GROUND_WIDTH && nIndex2 < TS_GROUND_WIDTH)
		{
			if(m_arCurrentHeight[nIndex2] > m_arCurrentHeight[nIndex1])
			{
				nRet = 5;
			}
		}
	}

	return nRet;
}

INT CTsSimpleAI::GetBigDifferCount(INT* pBuffer, int nCount, int nCheckValue)
{
	int nRet(0);
	
	if(pBuffer[1] - pBuffer[0] >= nCheckValue)
	{
		++nRet;
	}

	for(int i=1; i<nCount-1; ++i)
	{
		if(pBuffer[i-1] - pBuffer[i] >= nCheckValue 
			&& pBuffer[i+1] - pBuffer[i] >= nCheckValue)
		{
			++nRet;
		}
	}
	

	if(pBuffer[nCount - 2] - pBuffer[nCount - 1] >= nCheckValue)
	{
		++nRet;
	}

	return nRet;
}


void CTsSimpleAI::SendGetTempDataCommand()
{
	_ASSERTE(m_pDisplayContainer != NULL && m_pDisplayContainer->IsWindow());

	TsControlHeader th = {0};
	th.dwIDFrom = SIMULATOR_CONTROLLER_ID;
	th.nControlCode = TS_CONTROL_GET_GROUND_DATA;
	th.dwUserData = sizeof(m_arTempData) / sizeof(m_arTempData[0]);

	m_pDisplayContainer->SendMessage(WM_TS_DISPLAY_CONTROL, (WPARAM)&th, (LPARAM)m_arTempData);
}
void CTsSimpleAI::SendGetDataCommand()
{
	_ASSERTE(m_pDisplayContainer != NULL && m_pDisplayContainer->IsWindow());

	TsControlHeader th = {0};
	th.dwIDFrom = SIMULATOR_CONTROLLER_ID;
	th.nControlCode = TS_CONTROL_GET_GROUND_DATA;
	th.dwUserData = sizeof(m_arCurrentData) / sizeof(m_arCurrentData[0]);

	m_pDisplayContainer->SendMessage(WM_TS_DISPLAY_CONTROL, (WPARAM)&th, (LPARAM)m_arCurrentData);

	GetHeightData(m_arCurrentData, m_arCurrentHeight);
	GetGapData(m_arCurrentData, m_arCurrentGap);
//	m_nCurrentDiffCount = GetDiffCount(m_arCurrentHeight, TS_GROUND_WIDTH);
}
BOOL CTsSimpleAI::SendMoveLeftCommand(int nStep)
{
	int nCurrent(0);
	BOOL bOK(TRUE);
	
	while(nCurrent++ < nStep && bOK)
	{
		bOK = (BOOL)_SendControlComamnd(TS_CONTROL_MOVE_LEFT, 0);
	}

	return bOK;
}
BOOL CTsSimpleAI::SendMoveRightCommand(int nStep)
{
	int nCurrent(0);
	BOOL bOK(TRUE);
	
	while(nCurrent++ < nStep && bOK)
	{
		bOK = (BOOL)_SendControlComamnd(TS_CONTROL_MOVE_RIGHT, 0);
	}

	return bOK;
}
void CTsSimpleAI::SendSwitchCommand(int nSwitch)
{
	_ASSERTE(nSwitch >=0 && nSwitch < 4);

	while(--nSwitch >= 0)
	{
		_SendControlComamnd(TS_CONTROL_SWITCH_LEFT, 0);
	}
}
void CTsSimpleAI::SendTempMovingEleCommand()
{
	_ASSERTE(m_pDisplayContainer != NULL && m_pDisplayContainer->IsWindow());

	TsControlHeader th = {0};
	th.dwIDFrom = SIMULATOR_CONTROLLER_ID;
	th.nControlCode = TS_CONTROL_SET_MOVING_ELE;

	CTsDataFactory* pFactory = TsGetDataFactory();
	_ASSERTE(pFactory != NULL);
	INT nCount = pFactory->GetElementCount();

	_ASSERTE(m_nNextElementIndex>=0 && m_nNextElementIndex < nCount);
	CTsTypeElement* pEle = pFactory->GetElementByIndex(m_nNextElementIndex);
	m_pDisplayContainer->SendMessage( WM_TS_DISPLAY_CONTROL, WPARAM(&th), LPARAM(pEle));	
}
void CTsSimpleAI::SendMovingEleCommand()
{
	_ASSERTE(m_pDisplayContainer != NULL && m_pDisplayContainer->IsWindow());

	TsControlHeader th = {0};
	th.dwIDFrom = SIMULATOR_CONTROLLER_ID;
	th.nControlCode = TS_CONTROL_SET_MOVING_ELE;

	CTsDataFactory* pFactory = TsGetDataFactory();
	_ASSERTE(pFactory != NULL);
	INT nCount = pFactory->GetElementCount();

	_ASSERTE(m_nCurrentElementIndex>=0 && m_nCurrentElementIndex < nCount);
	CTsTypeElement* pEle = pFactory->GetElementByIndex(m_nCurrentElementIndex);
	m_pDisplayContainer->SendMessage( WM_TS_DISPLAY_CONTROL, WPARAM(&th), LPARAM(pEle));	
}
void CTsSimpleAI::SendInitCommand()
{
	_ASSERTE(m_pDisplayContainer != NULL && m_pDisplayContainer->IsWindow());

	TsControlHeader th = {0};
	th.dwIDFrom = SIMULATOR_CONTROLLER_ID;
	th.nControlCode = TS_CONTROL_INIT;
	th.dwUserData = TS_GROUND_HEIGHT * TS_GROUND_WIDTH;

	m_pDisplayContainer->SendMessage(WM_TS_DISPLAY_CONTROL, (WPARAM)&th, (LPARAM)m_arInitData);
}
void CTsSimpleAI::SendTempInitCommand()
{
	_ASSERTE(m_pDisplayContainer != NULL && m_pDisplayContainer->IsWindow());

	TsControlHeader th = {0};
	th.dwIDFrom = SIMULATOR_CONTROLLER_ID;
	th.nControlCode = TS_CONTROL_INIT;
	th.dwUserData = TS_GROUND_HEIGHT * TS_GROUND_WIDTH;

	m_pDisplayContainer->SendMessage(WM_TS_DISPLAY_CONTROL, (WPARAM)&th, (LPARAM)m_arTempData);
}
LRESULT CTsSimpleAI::OnDisplayEvent(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	TsControlHeader* pTh = (TsControlHeader*)wParam;
	_ASSERTE(pTh != NULL);

	switch(pTh->nControlCode)
	{
	case TS_CONTROL_END:
		break;
		
	case TS_CONTROL_REQUEST_ELEMENT:
		{
			
		}
		break;
	case TS_CONTROL_REMOVED_LINE:
		{
			if(m_bFirstCheck)
			{
				m_nFirstRemove = pTh->dwUserData;
			}	
			else
			{
				m_nSecondRemove = pTh->dwUserData;
			}

			break;
		}

	}

	return S_OK;
}


////////////////////////  CTsComputerController  ///////////////////////
CTsComputerController::CTsComputerController()
{
	m_pAI = NULL;
}

CTsComputerController::~CTsComputerController()
{
	_ASSERTE(m_pAI == NULL);
}
BOOL CTsComputerController::Initialize()
{
	_ASSERTE(!m_bInited);
	CTsGameControllerBase::Initialize();

	m_bInited = TRUE;
	
	_ASSERTE(m_pAI == NULL);
	m_pAI = new CTsSimpleAI;
	_ASSERTE(m_pAI != NULL);
	m_pAI->Initialize();

	return TRUE;
}

VOID CTsComputerController::UnInitialize()
{
	CTsGameControllerBase::UnInitialize();

	if(m_pAI != NULL)
	{
		m_pAI->UnInitialize();
		delete m_pAI;
		m_pAI = NULL;
	}

	m_bInited = FALSE;
}
void CTsComputerController::CheckAICommand()
{
	m_nAITickCount += TS_TIMER_INTERVAL;


	BOOL bDropDown(FALSE);

	if(m_nAITickCount >= m_nDownInterval + TS_TIMER_INTERVAL/*200*/)
	{
		bDropDown = TRUE;
	}
	
	if(m_AICmd.m_nUserData == 1)
	{
		SimulateAICommand();
		m_nAITickCount = 0;
	}
	

	if(bDropDown && m_AICmd.m_nUserData == 2)
	{	
		m_AICmd.m_nUserData = 0;
		m_nAITickCount = 0;
		
		_SendControlComamnd(TS_CONTROL_DROP_DOWN, 0, FALSE);

	}

}
void CTsComputerController::OnTimer()
{
	if(m_eStatus == RUNNING_STATUS)
	{
		CheckAICommand();
		m_nDownCurrent += TS_TIMER_INTERVAL;
		if(m_nDownCurrent >= m_nDownInterval)
		{
			m_nDownCurrent = 0;
			_SendControlComamnd(TS_CONTROL_MOVE_DOWN);
		}
		
	}

	m_pAI->OnTimer();
}
void CTsComputerController::SimulateAICommand()
{
	for(int i=0; i<m_AICmd.m_nSwitch; ++i)
	{
		_SendControlComamnd(TS_CONTROL_SWITCH_LEFT);
	}

	INT nCmd(TS_CONTROL_MOVE_LEFT);
	if(!m_AICmd.m_bMoveLeft)
	{
		nCmd = TS_CONTROL_MOVE_RIGHT;
	}
	for(int k=0; k<m_AICmd.m_nMoveStep; ++k)
	{
		_SendControlComamnd(nCmd);
	}
	
	m_AICmd.m_nUserData = 2;
}	
BOOL CTsComputerController::Start()
{
	_ASSERTE(m_bInited);
	
	CTsGameControllerBase::Start();

	_AttachDisplayWnd();

	_InitData();

	_SendControlComamnd(TS_CONTROL_START);

	m_nDownCurrent = 0;
	m_nAITickCount = 0;

	_ASSERTE(m_pAI != NULL);
	
	m_pAI->Start();

	_ASSERTE(m_eStatus == RUNNING_STATUS);

	return TRUE;
}

void CTsComputerController::Stop()
{
	if(m_eStatus != IDLE_STATUS)
	{
		CTsGameControllerBase::Stop();

		_SendControlComamnd(TS_CONTROL_END);
		
		_ASSERTE(m_pAI != NULL);
		m_pAI->Stop();

	}
}

BOOL CTsComputerController::Pause()
{
	_ASSERTE(m_eStatus == RUNNING_STATUS);
	if(m_eStatus == RUNNING_STATUS)
	{
		CTsGameControllerBase::Pause();

		m_nDownCurrent = 0;
		m_nAITickCount = 0;

		_ASSERTE(m_pAI != NULL);
		m_pAI->Pause();
	}

	return TRUE;
}

void CTsComputerController::Resume()
{
	_ASSERTE(m_eStatus == PAUSE_STATUS);
	if(m_eStatus == PAUSE_STATUS )
	{
		CTsGameControllerBase::Resume();

		m_nDownCurrent = 0;
		m_nAITickCount = 0;

		_ASSERTE(m_pAI != NULL);
		m_pAI->Resume();
	}
}

LRESULT CTsComputerController::OnDisplayEvent(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TsControlHeader* pTh = (TsControlHeader*)wParam;
	_ASSERTE(pTh != NULL);

	if((pTh->dwIDFrom == m_nID))
	{
		switch(pTh->nControlCode)
		{
		case TS_CONTROL_REQUEST_ELEMENT:
			_SendSetMovingEleCommand();
			SetInitDataForAI();

			_ASSERTE(m_pAI != NULL);
			m_pAI->GetBestAICommand(&m_AICmd);
			m_AICmd.m_nUserData = 1;
			
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
VOID CTsComputerController::_SendSetMovingEleCommand()
{
	TsControlHeader th = {0};
	th.dwIDFrom = m_nID;
	th.nControlCode = TS_CONTROL_SET_MOVING_ELE;

	CTsDataFactory* pFactory = TsGetDataFactory();
	_ASSERTE(pFactory != NULL);
	INT nCount = pFactory->GetElementCount();
	
	int nCurrentIndex = m_nNextIndex;
	_ASSERTE(m_nNextIndex>=0 && m_nNextIndex < nCount);
	CTsTypeElement* pEle = pFactory->GetElementByIndex(m_nNextIndex);
	_ASSERTE(m_hWndDisplay != NULL && ::IsWindow(m_hWndDisplay));
	::SendMessage(m_hWndDisplay, WM_TS_DISPLAY_CONTROL, WPARAM(&th), LPARAM(pEle));

	m_nNextIndex = m_pEleCreator->Random(nCount);
	_ASSERTE(m_hWndNextElement != NULL && IsWindow(m_hWndNextElement));
	::SendMessage(m_hWndNextElement, WM_TS_SET_NEXT_ELEMENT, m_nNextIndex, 0);

	_ASSERTE(m_pAI != NULL);
	m_pAI->SetNextElementIndex(m_nNextIndex);
	m_pAI->SetCurrentElementIndex(nCurrentIndex);
}
void CTsComputerController::SetInitDataForAI()
{
	CTsField arBuffer[TS_GROUND_HEIGHT * TS_GROUND_WIDTH];
	_ASSERTE(m_hWndDisplay != NULL && IsWindow(m_hWndDisplay));

	TsControlHeader th = {0};
	th.dwIDFrom = m_nID;
	th.nControlCode = TS_CONTROL_GET_GROUND_DATA;
	th.dwUserData = sizeof(arBuffer) / sizeof(arBuffer[0]);

	::SendMessage(m_hWndDisplay, WM_TS_DISPLAY_CONTROL, (WPARAM)&th, (LPARAM)arBuffer);

	_ASSERTE(m_pAI != NULL);
	m_pAI->SetInitData(arBuffer, sizeof(arBuffer) / sizeof(arBuffer[0]));
}
