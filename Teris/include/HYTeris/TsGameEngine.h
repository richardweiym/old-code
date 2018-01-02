#pragma  once 

#include <vector>
#include "TsStdAfx.h"
#include "TsDefine.h"
#include "TsControllerMgr.h"
#include "TsUtilBase.h"

#define TS_TIMER_INTERVAL 100

class CTsGameEngineBase;
class CTsField;
class ITsScoreMgr;
CTsGameEngineBase* GetCurrentEngine();

class CTsGarbageCreator
{
public:
	CTsGarbageCreator();
	VOID GetGarbage(CTsField* pBuffer, INT nLen);

private:
	CRandom m_rndFiller;
	CPrimeSearch m_prmFiller;
};

class ATL_NO_VTABLE CTsGameEngineBase: public CMessageMap
{
public:
	CTsGameEngineBase();
	virtual ~CTsGameEngineBase();

	virtual	BOOL Initialize();
	virtual VOID UnInitialize();

	virtual BOOL Start();
	virtual VOID Stop();
	virtual BOOL Pause();
	virtual void Resume();
	virtual void OnTimer();

	virtual TS_GAME_MODE GetTypeCode() = 0;

	BEGIN_MSG_MAP(CTsGameEngineBase)
		MESSAGE_HANDLER(WM_TS_DISPLAY_CONTROL_EVENT, OnDisplayEvent)
		MESSAGE_HANDLER(WM_TS_UPDATE_DISPLAY, OnSettingChanged);
	END_MSG_MAP()
	virtual LRESULT OnDisplayEvent(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSettingChanged(UINT, WPARAM, LPARAM, BOOL&) { return S_OK;}

	CTsControllerMgr* GetControllerMgr() { return &m_controllerMgr; }

public:
	BOOL IsInited() { return m_bInited; }

protected:
	virtual BOOL _CreateController();
	void _DeleteController();

	BOOL _CreateTimer();
	void _DeleteTimer();

	virtual void _SetMoveDownInterval();
	INT _GetHardLevelInterval();

	virtual BOOL _AttachWindowToController();
	virtual void _SetInitDataForController();
	virtual void _SetRndSeedForController();

	void SetControllerWin(INT nID, TS_DISPLAY_RESULT eResult);

protected:
	CTsControllerMgr m_controllerMgr;
	BOOL m_bInited;
	UINT m_nTimer;
	CTsGarbageCreator m_garbageCreator;
};

class CTsSinglePlayerEngine: public CTsGameEngineBase
{
public:
	virtual TS_GAME_MODE GetTypeCode();
	virtual LRESULT OnDisplayEvent(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSettingChanged(UINT, WPARAM, LPARAM, BOOL&);

protected:
	virtual BOOL _CreateController();

	virtual BOOL _AttachWindowToController();

	virtual void _SetInitDataForController();
	virtual void _SetMoveDownInterval();
	virtual void _SetRndSeedForController();
};


class ATL_NO_VTABLE CTsMultiPlayerEngine: public CTsGameEngineBase
{
protected:
	virtual void _SetInitDataForController();
	virtual void _SetRndSeedForController();
	virtual void _SetMoveDownInterval();
	
	virtual LRESULT OnDisplayEvent(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	void _OnControlEnd(WPARAM wParam, LPARAM lParam, BOOL bFirst);
	void _OnRemovedLine(WPARAM wParam ,LPARAM lParam);
};

class CTsTwoPlayerEngine: public CTsMultiPlayerEngine
{
public:
	virtual TS_GAME_MODE GetTypeCode();
	virtual LRESULT OnSettingChanged(UINT, WPARAM, LPARAM, BOOL&);

protected:
	virtual BOOL _CreateController();
	virtual BOOL _AttachWindowToController();

};

class CTsPlayWithComputerEngine: public CTsMultiPlayerEngine
{
public:
	virtual TS_GAME_MODE GetTypeCode();

	virtual LRESULT OnSettingChanged(UINT, WPARAM, LPARAM, BOOL&);

protected:
	virtual BOOL _CreateController();
	virtual BOOL _AttachWindowToController();

};




















