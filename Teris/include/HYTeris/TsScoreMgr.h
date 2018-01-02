#pragma once

#include "TsStdAfx.h"

class ITsScoreMgr
{
public:
	virtual ~ITsScoreMgr() { NULL; }
	virtual void OnNewElement(HWND hWndDisplay);
	virtual void OnRemovedLine(HWND hWndDisplay, INT nLineCount) = 0;
	static	INT GetGarbageLineCount(INT nLineCount);

	virtual void OnInitializeEngine();
	virtual void OnGameStart();
	virtual void OnWinGame(HWND hWndDisplay) { NULL; }
	
protected:
	INT GetScoreForNewElement();
	INT GetScoreForRemovedLine(INT nLine);
};

class CTsSingleScoreMgr: public ITsScoreMgr
{
public:
	virtual void OnRemovedLine(HWND hWndDisplay, INT nLineCount);
	virtual void OnGameStart();
};

class CTsMultiScoreMgr: public ITsScoreMgr
{
public:
	virtual void OnRemovedLine(HWND hWndDisplay, INT nLineCount);
	virtual void OnGameStart();
	virtual void OnWinGame(HWND hWndDisplay);
};