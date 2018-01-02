#include "TsSoundListener.h"
#include "TsUIInstanceApi.h"
#include "TsResMgr.h"
#include "TerisRes.h"
#include "TsGameContext.h"
#include "TsScoreMgr.h"

#pragma  comment(lib, "Winmm.lib")

void CTsSoundListener::_PlaySound(INT nResID)
{
	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);

	BOOL bPlaySound = pContext->IsSoundOpen();
	if(!bPlaySound) return;


	CTsResMgr* pResMgr = TsGetResMgr();
	_ASSERTE(pResMgr != NULL);

	HINSTANCE hRes = pResMgr->GetResInstance();

	PlaySound(MAKEINTRESOURCE(nResID), hRes, SND_ASYNC | SND_RESOURCE);
}

BOOL CTsSoundListener::Start()
{
	CTsControlListener::Start();


	_PlaySound(IDR_START_GAME_WAVE);


	return TRUE;
}


LRESULT CTsSoundListener::OnDisplayEvent(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	TsControlHeader* pTh = (TsControlHeader*)wParam;
	_ASSERTE(pTh != NULL);	
	
	if(pTh->dwIDFrom == COMPUTER_AI_CONTROLLER_ID) return S_OK;

	INT nSoundId(0);
	switch(pTh->nControlCode)
	{
	case TS_CONTROL_SET_RESULT:
		{
			TS_DISPLAY_RESULT eResult = (TS_DISPLAY_RESULT)lParam;
			if(eResult == WIN_RESULT)
			{
				nSoundId = IDR_WIN_WAVE;
			}
			else if(eResult == LOST_RESULT)
			{
				nSoundId = IDR_LOST_WAVE;
			}
			break;
		}
	case TS_CONTROL_ELE_GROUND_HIT:
		{
			nSoundId = IDR_DROP_DOWN_WAVE;
			break;
		}
	case TS_CONTROL_REMOVED_LINE:
		{
			int nLineCount = pTh->dwUserData;
			if(ITsScoreMgr::GetGarbageLineCount(nLineCount) == 0)
			{
				nSoundId = IDR_REMOVE_LINE_WAVE;
			}
			else
			{
				nSoundId = IDR_KILL_LINE_WAVE;
			}

			break;
		}
	case TS_CONTROL_PRODUCE_GARBAGE:
		{
			nSoundId = IDR_PRODUCE_GARBAGE_WAVE;
			break;
		}

	}

	if(nSoundId != 0)
	{
		_PlaySound(nSoundId);
	}
	
	return S_OK;
}
