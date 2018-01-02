#include "TsScoreMgr.h"
#include "TsUIDataMgr.h"
#include "TsUIInstanceApi.h"
#include "TsGameController.h"
#include "TsGameMainWnd.h"
//#include "TsGameEngine.h"

INT ITsScoreMgr::GetGarbageLineCount(INT nLineCount)
{
	return (nLineCount >= 2) ? nLineCount : 0;
}

INT ITsScoreMgr::GetScoreForNewElement()
{
	return 4;
}

INT ITsScoreMgr::GetScoreForRemovedLine(INT nLine)
{
	INT nRet(0);
	switch(nLine)
	{
	case 1:
		nRet = 4;
		break;
	
	case 2:
		nRet = 10;
		break;
	
	case 3:
		nRet = 20;
		break;

	case 4:
		nRet = 30;
		break;

	default:
		break;
	}

	return nRet;
}

void ITsScoreMgr::OnInitializeEngine()
{
	CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
	_ASSERTE(pPalyInfo != NULL);

	pPalyInfo->Clear();

}
void ITsScoreMgr::OnGameStart()
{
	_ASSERTE(FALSE);
}
void ITsScoreMgr::OnNewElement(HWND hWndDisplay)
{
	_ASSERTE(hWndDisplay != NULL);

	CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
	_ASSERTE(pPalyInfo != NULL);

	CTsGameMainWnd* pMain = TsGetGameMainWnd();
	_ASSERTE(pMain != NULL);
	HWND hLeft = pMain->GetLeftDisplayWnd();
	HWND hRight = pMain->GetRightDisplayWnd();
	if(hLeft == hWndDisplay)
	{
		pPalyInfo->SetLeftElementCount(pPalyInfo->GetLeftElementCount() + 1);
		pPalyInfo->SetLeftScore(pPalyInfo->GetLeftScore() + GetScoreForNewElement());
	}
	else if(hRight == hWndDisplay)
	{
		pPalyInfo->SetRightElementCount(pPalyInfo->GetRightElementCount() + 1);
		pPalyInfo->SetRightScore(pPalyInfo->GetRightScore() + GetScoreForNewElement());
	}
	else
	{
		_ASSERTE(FALSE);
	}
}


void CTsSingleScoreMgr::OnRemovedLine(HWND hWndDisplay, INT nLineCount)
{
	_ASSERTE(hWndDisplay != NULL);

	CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
	_ASSERTE(pPalyInfo != NULL);

	CTsGameMainWnd* pMain = TsGetGameMainWnd();
	_ASSERTE(pMain != NULL);
	HWND hLeft = pMain->GetLeftDisplayWnd();
	HWND hRight = pMain->GetRightDisplayWnd();
	if(hLeft == hWndDisplay)
	{
		pPalyInfo->SetLeftRemoveLineCount(pPalyInfo->GetLeftRemoveLineCount() + nLineCount);
		pPalyInfo->SetLeftScore(pPalyInfo->GetLeftScore() + GetScoreForRemovedLine(nLineCount));
	}
	else if(hRight == hWndDisplay)
	{
		pPalyInfo->SetRightRemoveLineCount(pPalyInfo->GetRightRemoveLineCount() + nLineCount);
		pPalyInfo->SetRightScore(pPalyInfo->GetRightScore() + GetScoreForRemovedLine(nLineCount));
	}
	else
	{
		_ASSERTE(FALSE);
	}
}

void CTsSingleScoreMgr::OnGameStart()
{
	OnInitializeEngine();
}


void CTsMultiScoreMgr::OnRemovedLine(HWND hWndDisplay, INT nLineCount)
{
	_ASSERTE(hWndDisplay != NULL);

	CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
	_ASSERTE(pPalyInfo != NULL);

	CTsGameMainWnd* pMain = TsGetGameMainWnd();
	_ASSERTE(pMain != NULL);
	HWND hLeft = pMain->GetLeftDisplayWnd();
	HWND hRight = pMain->GetRightDisplayWnd();
	if(hLeft == hWndDisplay)
	{
		pPalyInfo->SetLeftRemoveLineCount(pPalyInfo->GetLeftRemoveLineCount() + nLineCount);
		pPalyInfo->SetLeftScore(pPalyInfo->GetLeftScore() + GetScoreForRemovedLine(nLineCount));
		pPalyInfo->SetLeftKillLineCount(pPalyInfo->GetLeftKillLineCount() + GetGarbageLineCount(nLineCount));
	}
	else if(hRight == hWndDisplay)
	{
		pPalyInfo->SetRightRemoveLineCount(pPalyInfo->GetRightRemoveLineCount() + nLineCount);
		pPalyInfo->SetRightScore(pPalyInfo->GetRightScore() + GetScoreForRemovedLine(nLineCount));
		pPalyInfo->SetRightKillLineCount(pPalyInfo->GetRightKillLineCount() + GetGarbageLineCount(nLineCount));
	}
	else
	{
		_ASSERTE(FALSE);
	}
}
void CTsMultiScoreMgr::OnGameStart()
{
	CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
	_ASSERTE(pPalyInfo != NULL);

	pPalyInfo->SetLeftElementCount(0);
	pPalyInfo->SetLeftKillLineCount(0);
	pPalyInfo->SetLeftRemoveLineCount(0);
	pPalyInfo->SetLeftScore(0);

	pPalyInfo->SetRightElementCount(0);
	pPalyInfo->SetRightKillLineCount(0);
	pPalyInfo->SetRightRemoveLineCount(0);
	pPalyInfo->SetRightScore(0);
	
}
void CTsMultiScoreMgr::OnWinGame(HWND hWndDisplay)
{
	_ASSERTE(hWndDisplay != NULL);

	CTsPlayProcessInfo* pPalyInfo = TsGetGameProcessInfo();
	_ASSERTE(pPalyInfo != NULL);

	CTsGameMainWnd* pMain = TsGetGameMainWnd();
	_ASSERTE(pMain != NULL);
	HWND hLeft = pMain->GetLeftDisplayWnd();
	HWND hRight = pMain->GetRightDisplayWnd();
	if(hLeft == hWndDisplay)
	{
		pPalyInfo->SetLeftWinCount(pPalyInfo->GetLeftWinCount() + 1);
	}
	else if(hRight == hWndDisplay)
	{
		pPalyInfo->SetRightWinCount(pPalyInfo->GetRightWinCount() + 1);
	}
	else
	{
		_ASSERTE(FALSE);
	}
}