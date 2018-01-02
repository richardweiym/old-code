#pragma  once 

#include "TsGameController.h"

class CTsRecordFile
{
public:
	

private:

};

class CTsRecordListener: public CTsControlListener
{
public:
	virtual BOOL Start();
	virtual TS_CONTROLLER_TYPE GetTypeCode() { return GAME_RECORD_LISTENER; }
	virtual LRESULT OnDisplayEvent(UINT, WPARAM, LPARAM, BOOL&);

private:
	
};