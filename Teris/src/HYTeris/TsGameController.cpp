#include "TsGameController.h"
#include "TsUtilBase.h"
#include "TsDataKernel.h"
#include "TsUIInstanceApi.h"

//////////////////// CTsControlListener  ///////////////////////
CTsControlListener::CTsControlListener()
{
	m_eStatus = IDLE_STATUS;
	m_nID = 0;
	m_bInited = FALSE;
}
CTsControlListener::~CTsControlListener()
{
	_ASSERTE(!m_bInited);
}
BOOL CTsControlListener::Initialize()
{
	m_bInited = TRUE;

	return TRUE;
}
VOID CTsControlListener::UnInitialize()
{
	m_eStatus = IDLE_STATUS;
	m_nID = 0;
	m_bInited = FALSE;
}

VOID CTsControlListener::OnTimer()
{
	NULL;
}

BOOL CTsControlListener::Start()
{
	_ASSERTE(m_bInited);
	m_eStatus = RUNNING_STATUS;
	return TRUE;
}

void CTsControlListener::Stop()
{
	_ASSERTE(m_bInited);
	m_eStatus = IDLE_STATUS;
}

BOOL CTsControlListener::Pause()
{
	_ASSERTE(m_bInited);
	m_eStatus = PAUSE_STATUS;
	return TRUE;
}

void CTsControlListener::Resume()
{
	_ASSERTE(m_bInited);
	m_eStatus = RUNNING_STATUS;
}


//////////////////// CTsGameControllerBase  ///////////////////////

CTsGameControllerBase::CTsGameControllerBase()
{
	m_hWndDisplay = NULL;
	m_hWndNextElement = NULL;

	m_pEleCreator = NULL;

	m_pControlKeys = NULL;
	m_nDownCurrent = 0;
	m_nDownInterval = 0;
	m_nNextIndex = 0;

	m_pEngineMap = NULL;
}
BOOL CTsGameControllerBase::Initialize()
{
	BOOL bRet(FALSE);
	bRet = CTsControlListener::Initialize();

	return bRet;
}
VOID CTsGameControllerBase::UnInitialize()
{
	if(m_pEleCreator != NULL)
	{
		delete m_pEleCreator;
		m_pEleCreator = NULL;
	}

	m_hWndDisplay = NULL;
	m_hWndNextElement = NULL;

	m_pEleCreator = NULL;

	m_pControlKeys = NULL;
	m_nDownCurrent = 0;
	m_nDownInterval = 0;
	m_nNextIndex = 0;

	m_pEngineMap = NULL;

	CTsControlListener::UnInitialize();
}
CTsGameControllerBase::~CTsGameControllerBase()
{
	_ASSERTE(!m_bInited);
}

VOID CTsGameControllerBase::SetDisplayWnd(HWND hWnd)
{
	m_hWndDisplay = hWnd;
}

VOID CTsGameControllerBase::SetNextElementWnd(HWND hWnd)
{
	m_hWndNextElement = hWnd;
}


void CTsGameControllerBase::SetElementCreator(INT nID)
{
	if(m_pEleCreator != NULL)
	{
		delete m_pEleCreator;
		m_pEleCreator = NULL;
	}

	m_pEleCreator = new CRandom;
	_ASSERTE(m_pEleCreator != NULL);
	if(m_pEleCreator != NULL)
	{
		m_pEleCreator->SetRandomSeed(nID);	
	}
}

void CTsGameControllerBase::SetInitDisplayData(CTsField* pField, INT nCount)
{
	_ASSERTE(nCount <= sizeof(m_arInitData) / sizeof(m_arInitData[0]));
	for(int i=0; i<nCount; ++i)
	{
		m_arInitData[i] = pField[i];
	}
}

LRESULT CTsGameControllerBase::OnDisplayEvent(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRet(S_OK);
	TsControlHeader* pTh = (TsControlHeader*)wParam;
	_ASSERTE(pTh != NULL);

	if(m_pEngineMap != NULL && pTh->dwIDFrom == m_nID)
	{
		m_pEngineMap->ProcessWindowMessage(NULL, nMsgType, wParam, lParam, lRet, 0);
	}

	return lRet;
}
LRESULT CTsGameControllerBase::_SendControlComamnd(INT nType, LPARAM lParam, BOOL bAsyn)
{
	static TsControlHeader th = {0};
	th.dwIDFrom = m_nID;
	th.nControlCode = nType;	
	
	_ASSERTE(m_hWndDisplay != NULL && ::IsWindow(m_hWndDisplay));
	if(!bAsyn)
	{
		return ::SendMessage(m_hWndDisplay, WM_TS_DISPLAY_CONTROL, WPARAM(&th), lParam);
	}
	else
	{
		::PostMessage(m_hWndDisplay, WM_TS_DISPLAY_CONTROL, WPARAM(&th), lParam);
		return S_OK;
	}
}
void CTsGameControllerBase::SetPlayResult(TS_DISPLAY_RESULT eResult)
{
	_SendControlComamnd(TS_CONTROL_SET_RESULT, eResult);
}

void CTsGameControllerBase::ProduceGarbage(CTsField* pField, INT nCount)
{
	_ASSERTE(pField != NULL);

	if(pField != NULL && nCount >0 )
	{
		TsControlHeader th = {0};
		th.dwIDFrom = m_nID;
		th.nControlCode = TS_CONTROL_PRODUCE_GARBAGE;
		th.dwUserData = nCount;

		_ASSERTE(m_hWndDisplay != NULL && ::IsWindow(m_hWndDisplay));
		::SendMessage(m_hWndDisplay, WM_TS_DISPLAY_CONTROL, WPARAM(&th), LPARAM(pField));		
	}
}

void CTsGameControllerBase::_AttachDisplayWnd()
{
	_ASSERTE(m_hWndDisplay != NULL && ::IsWindow(m_hWndDisplay));
	if(m_hWndDisplay != NULL && ::IsWindow(m_hWndDisplay))
	{
		::SendMessage(m_hWndDisplay, WM_TS_SET_DISPLAY_SINK, (WPARAM)this, 1);
	}
}
void CTsGameControllerBase::_DetachDisplayWnd()
{
	_ASSERTE(m_hWndDisplay != NULL && ::IsWindow(m_hWndDisplay));
	if(m_hWndDisplay != NULL && ::IsWindow(m_hWndDisplay))
	{
		::SendMessage(m_hWndDisplay, WM_TS_SET_DISPLAY_SINK, (WPARAM)this, 0);
	}
}

void CTsGameControllerBase::_InitData()
{
	_ASSERTE(m_hWndDisplay != NULL && ::IsWindow(m_hWndDisplay));
	if(m_hWndDisplay != NULL && ::IsWindow(m_hWndDisplay))
	{
		TsControlHeader th = {0};
		th.dwIDFrom = m_nID;
		th.nControlCode = TS_CONTROL_INIT;
		th.dwUserData = TS_GROUND_HEIGHT * TS_GROUND_WIDTH;

		::SendMessage(m_hWndDisplay, WM_TS_DISPLAY_CONTROL, (WPARAM)&th, (LPARAM)m_arInitData);
	}

	CTsDataFactory* pFactory = TsGetDataFactory();
	_ASSERTE(pFactory != NULL);

	_ASSERTE(m_pEleCreator != NULL);
	if(m_pEleCreator != NULL)
	{
		m_nNextIndex = m_pEleCreator->Random(pFactory->GetElementCount());
	}
}

VOID CTsGameControllerBase::_SendSetMovingEleCommand()
{
	TsControlHeader th = {0};
	th.dwIDFrom = m_nID;
	th.nControlCode = TS_CONTROL_SET_MOVING_ELE;

	CTsDataFactory* pFactory = TsGetDataFactory();
	_ASSERTE(pFactory != NULL);
	INT nCount = pFactory->GetElementCount();

	_ASSERTE(m_nNextIndex>=0 && m_nNextIndex < nCount);
	CTsTypeElement* pEle = pFactory->GetElementByIndex(m_nNextIndex);
	_ASSERTE(m_hWndDisplay != NULL && ::IsWindow(m_hWndDisplay));
	::SendMessage(m_hWndDisplay, WM_TS_DISPLAY_CONTROL, WPARAM(&th), LPARAM(pEle));

	m_nNextIndex = m_pEleCreator->Random(nCount);
	_ASSERTE(m_hWndNextElement != NULL && IsWindow(m_hWndNextElement));
	::SendMessage(m_hWndNextElement, WM_TS_SET_NEXT_ELEMENT, m_nNextIndex, 0);
}