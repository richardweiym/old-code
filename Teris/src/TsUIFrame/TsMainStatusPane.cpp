#include "TsMainStatusPane.h"


LRESULT CTsMainStatusPane::OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CPaintDC dc(m_hWnd);
	
	CRect rtClient;
	GetClientRect(&rtClient);
	
	dc.FillSolidRect(&rtClient, RGB(233, 204, 128));

	LPCTSTR lpszText = _T("Status Pane");
	dc.DrawText(lpszText, lstrlen(lpszText), rtClient, DT_CENTER | DT_END_ELLIPSIS | DT_VCENTER);

	return S_OK;
}

LRESULT CTsMainStatusPane::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return TRUE;
}