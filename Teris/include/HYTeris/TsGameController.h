#pragma  once 

#include "TsStdAfx.h"
#include "TsDefine.h"
#include "TsDataKernel.h"

enum TS_CONTROLLER_TYPE
{
	PLAYER_CONTROLLER = 0,
	COMPUTER_CONTROLLER,
	NET_PLAYER_CONTROLLER,
	RECORD_PLAY_CONTROLLER,

	NET_OBSERVER_LISTENER,
	RECORD_LISTENER,
	GAME_SOUND_LISTENER,
	GAME_SCORE_LISTENER	,
	GAME_RECORD_LISTENER
};

#define LEFT_CONTROLLER_ID 100
#define RIGHT_CONTROLLER_ID 101
#define SINGLE_CONTROLLER_ID LEFT_CONTROLLER_ID
#define SOUND_LISTENER_ID 102
#define SCORE_LISTENER_ID 103
#define COMPUTER_AI_CONTROLLER_ID 104
#define SIMULATOR_CONTROLLER_ID 110


class CTsControlListener: public CMessageMap
{
public:
	CTsControlListener();
	virtual ~CTsControlListener();

	virtual BOOL Initialize();
	virtual VOID UnInitialize();

	virtual TS_CONTROLLER_TYPE GetTypeCode() = 0;
	virtual VOID OnTimer();
	
	virtual BOOL Start();
	virtual void Stop();
	virtual BOOL Pause();
	virtual void Resume();

	virtual BOOL IsVisible() { return FALSE; }

	BEGIN_MSG_MAP(CTsControlListener)
		MESSAGE_HANDLER(WM_TS_DISPLAY_CONTROL_EVENT, OnDisplayEvent)
	END_MSG_MAP()

	VOID SetID(INT nId) { m_nID = nId; }
	INT GetID() { return m_nID; }


	BOOL IsInited() { return m_bInited; }

	TS_GAME_STATUS GetStatus() { return m_eStatus; }

protected:
	virtual LRESULT OnDisplayEvent(UINT, WPARAM, LPARAM, BOOL&) { return S_OK; }

protected:
	INT m_nID;
	TS_GAME_STATUS m_eStatus;
	BOOL m_bInited;

};

class CTsField;
class CRandom;
class CTsControlKeys;
class CTsGameControllerBase: public CTsControlListener
{
public:
	CTsGameControllerBase();
	virtual ~CTsGameControllerBase();

	virtual BOOL Initialize() ;
	virtual VOID UnInitialize();

	virtual TS_CONTROLLER_TYPE GetTypeCode() = 0;
	virtual BOOL IsVisible() { return TRUE; }

	BEGIN_MSG_MAP(CTsPlayerController)
		CHAIN_MSG_MAP(CTsControlListener)
	END_MSG_MAP()

	virtual LRESULT OnDisplayEvent(UINT, WPARAM, LPARAM, BOOL&);

public:
	void SetEngineMap(CMessageMap* pMap) { m_pEngineMap = pMap; }

	VOID SetDisplayWnd(HWND hWnd);
	HWND GetDisplayWnd() { return m_hWndDisplay; }

	VOID SetNextElementWnd(HWND hWnd);
	HWND GetNextElementWnd() { return m_hWndNextElement; }

	void SetElementCreator(INT nID);
	void SetInitDisplayData(CTsField* pField, INT nCount);

	void SetMoveDownInterval(INT nVal) { m_nDownInterval = nVal; }
	void SetPlayResult(TS_DISPLAY_RESULT eResult);
	LRESULT _SendControlComamnd(INT nType, LPARAM lParam = 0, BOOL bAsyn = FALSE);
	
	void ProduceGarbage(CTsField* pField, INT nCount);

protected:
	void _AttachDisplayWnd();
	void _DetachDisplayWnd();
	void _InitData();
	virtual	void _SendSetMovingEleCommand();

protected:
	HWND m_hWndDisplay;
	HWND m_hWndNextElement;
	CMessageMap* m_pEngineMap;

	CRandom* m_pEleCreator;

	CTsField m_arInitData[TS_GROUND_WIDTH * TS_GROUND_HEIGHT];

	INT m_nNextIndex;
	CTsControlKeys* m_pControlKeys;

	INT m_nDownInterval;
	INT m_nDownCurrent;
};

