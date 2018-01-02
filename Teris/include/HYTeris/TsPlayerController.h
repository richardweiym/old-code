#pragma  once

#include "TsGameController.h"

class CTsPlayerController: public CTsGameControllerBase
{
public:
	virtual BOOL Initialize();
	virtual VOID UnInitialize();

	virtual TS_CONTROLLER_TYPE GetTypeCode();
	virtual VOID OnTimer();
	
	virtual BOOL Start();
	virtual void Stop();
	virtual BOOL Pause();
	virtual void Resume();	
	BEGIN_MSG_MAP(CTsPlayerController)
		CHAIN_MSG_MAP(CTsGameControllerBase)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

protected:
	LRESULT OnKeyDown(UINT, WPARAM, LPARAM, BOOL&);
	virtual LRESULT OnDisplayEvent(UINT, WPARAM, LPARAM, BOOL&);

protected:
	VOID _AttachKeyEvents();
	VOID _DetachKeyEvents();


};