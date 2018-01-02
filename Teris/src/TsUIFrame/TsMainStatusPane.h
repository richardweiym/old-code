#pragma  once 

#include "TsStdAfx.h"

class CTsMainStatusPane: public CWindowImpl<CTsMainStatusPane>
{
public:
	DECLARE_WND_CLASS(_T("TsMainStatusPane"))
	BEGIN_MSG_MAP(CTsMainStatusPane)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled);
};