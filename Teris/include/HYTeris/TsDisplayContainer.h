#pragma  once 

#include <vector>
#include "TsStdAfx.h"
#include "TsDataKernel.h"
#include "TsDefine.h"


class CTsDisplayContainer: public CWindowImpl<CTsDisplayContainer, CWindow, CControlWinTraits>
{
public:
	DECLARE_WND_CLASS(_T("TsDisplayWnd"))

	BEGIN_MSG_MAP(CTsDisplayContainer)
		MESSAGE_HANDLER(WM_TS_DISPLAY_CONTROL, OnTsDisplayControl)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_TS_SET_DISPLAY_SINK, OnTsSetDisplaySink)
	END_MSG_MAP()

	CTsDisplayContainer();
	void SetEnableDisplay(BOOL bDisplay) { m_bDisplayData = bDisplay; }
	BOOL IsPlaying() { return m_bPlaying; }
	static BOOL GetCorrectAutoSize(SIZE& szCheck);
	static CSize GetMinSize();
	void ClearDisplay();

	LRESULT OnTsDisplayControl(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnTsSetDisplaySink(UINT, WPARAM, LPARAM, BOOL& bHandled);

protected:
	LRESULT OnControlInit(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlStart(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlEnd(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlSetMovingEle(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlMoveLeft(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlMoveRight(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlMoveDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlSwitchLeft(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlSwitchRight(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlDropDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlProduceGarbage(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlSetResult(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlQueryStatus(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlGetGroundData(WPARAM wParam, LPARAM lParam);

private:
	VOID _DrawBkgnd(CDCHandle dc);
	VOID _DrawGrounField(CDCHandle dc);
	VOID _DrawMovingElement(CDCHandle dc);
	VOID _DrawDisplayResutl(CDCHandle dc);
	VOID _DrawFieldElement(CDCHandle dc, INT nX, INT nY, CTsField* pField, BOOL bDrawGray);
	BOOL _IsValidDrawRect(CRect& rtTest);

	BOOL _IsValidMovingElement(POINT& ptPos, TsSwitchType eType);
	
	VOID _AddMovingElement2Ground();
	BOOL _IsGameDead();
	INT _RemoveFullLineData();

	void _OnControlEvent(WPARAM wParam, LPARAM lParam);
	void _SetDeadStatus();
	
	void _RequestNewElement(BOOL bFirst = FALSE);
	void _OnRemovedLine(INT nCount);

	void _OnElementHitGround();


private:
	CTsGroundDataMgr m_dataMgr;
	CTsMovingElement m_movingEle;
	BOOL m_bPlaying;
	DWORD m_dwIDFrom;

	CMessageMap* m_pControlSink;
	TS_DISPLAY_RESULT m_eDisplayResutl;
	BOOL m_bOutOfGround;
	BOOL m_bDisplayData;
	BOOL m_bDroppingDown;
};