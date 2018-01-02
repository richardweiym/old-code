#pragma  once 

#include "TsGameController.h"
#include <vector>

struct CTsAICommand
{
	BOOL m_bMoveLeft;
	INT m_nSwitch;
	INT m_nMoveStep;
	DWORD m_nUserData;
	
	CTsAICommand()
	{
		m_bMoveLeft = TRUE;
		m_nSwitch = 0;
		m_nMoveStep = 0;
		m_nUserData = 0;
	}
};

class CTsDisplayContainer;
class CTsAIBase: public CMessageMap
{
public:
	CTsAIBase();
	virtual ~CTsAIBase();

	virtual BOOL Initialize() ;
	virtual void  UnInitialize() ;
	virtual VOID OnTimer();
	virtual BOOL Start();
	virtual void Stop();
	virtual BOOL Pause() ;
	virtual void Resume();

	void SetInitData(CTsField* pData, int nCount);
	void SetCurrentElementIndex(INT nIndex);
	void SetNextElementIndex(INT nIndex);
	LRESULT _SendControlComamnd(INT nType, LPARAM lParam);

	BEGIN_MSG_MAP(CTsAIBase)
		MESSAGE_HANDLER(WM_TS_DISPLAY_CONTROL_EVENT, OnDisplayEvent)
	END_MSG_MAP()
	
	virtual BOOL GetBestAICommand(CTsAICommand* pCmd) = 0;
	virtual LRESULT OnDisplayEvent(UINT, WPARAM, LPARAM, BOOL&) = 0;

protected:
	void GetHeightData(CTsField* pFields, INT* pBuffer);
	void GetGapData(CTsField* pFields, INT* pBuffer);

	INT GetDiffCount(INT * pData, INT nCount);
	INT GetMinDataIndex(INT* pData, INT nCount);
	INT GetMaxDataIndex(INT* pData, INT nCount);
protected:

	INT  m_nCurrentElementIndex;
	INT m_nNextElementIndex;
	CTsField m_arInitData[TS_GROUND_WIDTH * TS_GROUND_HEIGHT];
	CTsDisplayContainer* m_pDisplayContainer;

	INT m_arOldHeigh[TS_GROUND_WIDTH];
	INT m_arOldGap[TS_GROUND_WIDTH];
//	INT m_nOldDiffCount;
};

class CTsSimpleAI: public CTsAIBase
{
public:
	CTsSimpleAI();
	virtual BOOL GetBestAICommand(CTsAICommand* pCmd);
	virtual LRESULT OnDisplayEvent(UINT, WPARAM, LPARAM, BOOL&);
	virtual INT GetCurrentMethodScore();

protected:
	void SendInitCommand();
	void SendMovingEleCommand();
	void SendSwitchCommand(int nSwitch);
	BOOL SendMoveLeftCommand(int nStep);
	BOOL SendMoveRightCommand(int nStep);
	void SendGetDataCommand();
	INT GetechelonValue();


protected:
	void CheckTempAICommand(CTsAICommand* pCmd);
	void SendTempInitCommand();
	void SendTempMovingEleCommand();
	void SendGetTempDataCommand();

	INT GetBigDifferCount(INT* pBuffer, int nCount, int nCheckValue);

private:
	CTsField m_arCurrentData[TS_GROUND_WIDTH * TS_GROUND_HEIGHT];
	INT m_arCurrentHeight[TS_GROUND_WIDTH];
	INT m_arCurrentGap[TS_GROUND_WIDTH];
	INT m_nRemoveLineCount;
//	INT m_nCurrentDiffCount;

	CTsField m_arTempData[TS_GROUND_WIDTH * TS_GROUND_HEIGHT];
	CTsAICommand m_TempCmd;
	INT m_nMaxValue;
	
	BOOL m_bFirstCheck;
	INT  m_nFirstRemove;
	INT  m_nSecondRemove;
};


class CTsComputerController: public CTsGameControllerBase
{
public:
	CTsComputerController(); 
	~CTsComputerController();

	virtual BOOL Initialize();
	virtual VOID UnInitialize();

	virtual TS_CONTROLLER_TYPE GetTypeCode() { return COMPUTER_CONTROLLER; }
	virtual VOID OnTimer();
	
	virtual BOOL Start();
	virtual void Stop();
	virtual BOOL Pause();
	virtual void Resume();	
	BEGIN_MSG_MAP(CTsPlayerController)
		CHAIN_MSG_MAP(CTsGameControllerBase)
	END_MSG_MAP()

protected:
	void CheckAICommand();

	virtual LRESULT OnDisplayEvent(UINT, WPARAM, LPARAM, BOOL&);
	virtual VOID _SendSetMovingEleCommand();
	void SetInitDataForAI();
	
	void SimulateAICommand();



private:
	CTsAIBase* m_pAI;
	CTsAICommand m_AICmd;

	INT m_nAITickCount;
};