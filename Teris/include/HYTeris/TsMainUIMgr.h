#pragma  once

#include "TsStdAfx.h"
#include "TsUIDataMgr.h"

class CTsGameContext;
class CTsGameMainWnd;
class CTsResMgr;

class CTsMainUIMgr: public CMessageMap
{
public:
	CTsMainUIMgr();
	~CTsMainUIMgr();

	BEGIN_MSG_MAP(CTsMainUIMgr)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(TS_SETUP_DLG_CHANGED, OnSetupDlgChanged)
	END_MSG_MAP()

	bool Initialize();
	bool InitializeUI();
	void UnInitializeUI();
	void UnInitialize();
	
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnSetupDlgChanged(UINT, WPARAM, LPARAM, BOOL&);

protected:
	void _OnSetPlayMode(TS_GAME_MODE eMode);
	void _OnStartCommand();
	void _OnPauseCommand();
	
	void _ResetColorType();
	void _ResetGameOption();

	void _SetDefaultUIData();

	void _StartPlay();
	void _EndPlay();
	void _PausePlay();
	void _ResumePlay();

private:
	CTsGameContext* m_pContext;
	CTsGameMainWnd* m_pMainWnd;
	CTsResMgr* m_pResMgr;
};