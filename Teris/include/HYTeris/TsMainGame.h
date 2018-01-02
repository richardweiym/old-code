#pragma  once

#include "TsStdAfx.h"
#include <TsUtilBase.h>
#include "TsDefine.h"
#include "TsUIDataMgr.h"

#define TS_UNIQUE_WIN_TEXT _T("3F16ECAF-7723-459a-8052-D9D4E11AC5B3")

class CTsAgentWindow: public CWindowImpl<CTsAgentWindow>,
					  public CTsGameBaseInfoObserver<CTsAgentWindow>
{
public:
	DECLARE_WND_CLASS(TS_UNIQUE_WIN_TEXT)	

	BEGIN_MSG_MAP(CTsAgentWindow)
		MESSAGE_HANDLER(WM_TS_UI_DATA_CHANGED, OnTsUIDataChanged)
		CHAIN_MSG_MAP(CTsGameBaseInfoObserver<CTsAgentWindow>)
	END_MSG_MAP()
	
	CTsAgentWindow(CMessageMap* pMap);
	LRESULT OnTsUIDataChanged(UINT, WPARAM, LPARAM, BOOL& bHandled);

private:
	CMessageMap* m_pMessageMap;
};



class CTsGameEngineBase;
class CTsMainUIMgr;
class CMainDlg;

class CTsMainGame: CMessageMap
{
public:
	CTsMainGame();
	~CTsMainGame();

	BEGIN_MSG_MAP(CTsMainGame)
		MESSAGE_HANDLER(WM_TS_UI_DATA_CHANGED, OnUIDataChanged)
	END_MSG_MAP()

	bool Initialize();
	bool InitializeUI();
	void UnInitializeUI();
	void UnInitialize();	
	
	CTsGameEngineBase* GetGameEngine() { return m_pGameEngine; }
	CTsMainUIMgr* GetGameUIMgr() { return m_pGameUIMgr; }
	
	LRESULT OnUIDataChanged(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	void _CheckEngineType();
	void _CheckAndDeleteOldEngine();

protected:
	CTsMainGame(const CTsMainGame& game);

	CTsGameEngineBase* m_pGameEngine;
	CTsMainUIMgr* m_pGameUIMgr;

	CTsAgentWindow* m_pAgentWnd;
};


CTsMainGame* GetGameInstance();