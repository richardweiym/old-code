#pragma once

#include "TsStdAfx.h"
#include <vector>


class CTsGameControllerBase;
class CTsControlListener;
class CTsControllerMgr
{
public:
	BOOL Initialize();
	VOID UnInitialize();
	
	BOOL Start();
	VOID Stop();
	BOOL Pause();
	void Resume();

	void OnTimer();

	CTsControllerMgr();
	~CTsControllerMgr();

	BOOL CreateLeftUserController();
	BOOL CreateRightUserController();
	BOOL CreateLeftAIController();
	BOOL CreateRightAIController();

	BOOL CreateSingleController();
	BOOL CreateSingleScoreListener();
	BOOL CreateMultiScoreListener();
	BOOL CreateSoundListener();

	CTsGameControllerBase* GetLeftController();
	CTsGameControllerBase* GetRightController();
	CTsGameControllerBase* GetSingleController();
	CTsGameControllerBase* GetAIController();
	
	void NotifyOtherController(INT nFromID, UINT nMsgType, WPARAM wParam, LPARAM lParam);
	void DeleteController();
	CTsGameControllerBase* GetControllerById(INT nID);
	CTsControlListener* GetListenerById(INT nId);

protected:
	CTsControllerMgr(const CTsControllerMgr&);

	CTsGameControllerBase* m_pController1;
	CTsGameControllerBase* m_pController2;


	std::vector<CTsControlListener*> m_arObserver;
};