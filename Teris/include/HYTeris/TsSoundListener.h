#pragma  once

#include "TsGameController.h"

class CTsSoundListener: public CTsControlListener
{
public:
	virtual BOOL Start();
	virtual TS_CONTROLLER_TYPE GetTypeCode() { return GAME_SOUND_LISTENER; }
	virtual LRESULT OnDisplayEvent(UINT, WPARAM, LPARAM, BOOL&);

private:
	void _CheckSoundOption();
	void _PlaySound(INT nResID);
};