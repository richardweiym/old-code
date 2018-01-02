#pragma  once

#include "TsStdAfx.h"
#include "TsUIDataMgr.h"
#include "TsButton.h"

class CTsMainButtonPane: public CWindowImpl<CTsMainButtonPane>,
						 public CTsGameBaseInfoObserver<CTsMainButtonPane>
{
public:
	DECLARE_WND_CLASS(_T("TsButtonPane"))

	typedef CTsGameBaseInfoObserver<CTsMainButtonPane> ObserverBase;
	BEGIN_MSG_MAP(CTsMainButtonPane)
		CHAIN_MSG_MAP(ObserverBase)

		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)

		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	VOID DoUIDataChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnCommand(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	void _CheckButtonStatus();
	void _CheckGameStatus();
	void _LoadResouce();
	void _CreateButtons();
	void _AdjustSize();

private:
	CTsButton m_btnSingle;
	CTsButton m_btnNet;
	CTsButton m_btnRecordPlay;
	CTsButton m_btnSetup;
	CBitmap m_bmpBtnBk;
	CImageList m_imgBtn;
};
