#pragma once 

#include "TsGameController.h"

class CTsSingleScoreMgr;

class ITsScoreMgr;
class CTsScoreListenerBase: public CTsControlListener
{	
public:
	CTsScoreListenerBase();
	virtual ~CTsScoreListenerBase();
	virtual TS_CONTROLLER_TYPE GetTypeCode();	

protected:
	ITsScoreMgr* m_pScoreMgr;
};

class CTsSingleScoreListener: public CTsScoreListenerBase
{
public:
	virtual BOOL Initialize();
	virtual VOID UnInitialize();


	
	virtual BOOL Start();

protected:
	virtual LRESULT OnDisplayEvent(UINT, WPARAM, LPARAM, BOOL&);


};

class CTsMultiScoreListener: public CTsScoreListenerBase
{
public:
	virtual BOOL Initialize();
	virtual VOID UnInitialize();

	virtual BOOL Start();

protected:
	virtual LRESULT OnDisplayEvent(UINT, WPARAM, LPARAM, BOOL&);
};