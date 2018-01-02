#include "StdAfx.h"
#include "SmsBoxTree.h"
#include "resource.h"
#include "mainfrm.h"

LRESULT CSmsBoxTree::OnCreate(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	//m_imagelist.Create(IDB_SMS_BOX,32,0,RGB(255, 0, 255));
	m_imagelist.Create(32, 32, ILC_COLOR24|ILC_MASK, 10, 0);
	HBITMAP hBmp = LoadBitmap(CMainFrame::GetResourceModule(), MAKEINTRESOURCE(IDB_SMS_BOX));
	if(hBmp != NULL)
	{
		m_imagelist.Add(hBmp, RGB(255,0,255));
		DeleteObject(hBmp);
	}
	return 0;
}
LRESULT CSmsBoxTree::OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_imagelist.Destroy();
	return 0;
}
VOID CSmsBoxTree::InitBox()
{
	HINSTANCE hRes = CMainFrame::GetResourceModule();
	TCHAR szBuf[100] = {0};

	DWORD dwStyle = GetWindowLong(GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_SHOWSELALWAYS ;
	SetWindowLong(GWL_STYLE,dwStyle);
	SetImageList(m_imagelist,TVSIL_NORMAL);
	
	::LoadString(hRes, IDS_BOX_SERVER, szBuf, 100);
	HTREEITEM hRoot = InsertItem(szBuf, 4, 9, NULL, NULL);

	::LoadString(hRes, IDS_BOX_TOSEND, szBuf, 100);
	HTREEITEM h1 = InsertItem(szBuf, 0, 5, hRoot, NULL);
	::LoadString(hRes, IDS_BOX_SENDED, szBuf, 100);
	HTREEITEM h2 = InsertItem(szBuf, 1, 6, hRoot, h1);
	::LoadString(hRes, IDS_BOX_RECEIVED, szBuf, 100);
	HTREEITEM h3 = InsertItem(szBuf, 2, 7, hRoot, h2);
	::LoadString(hRes, IDS_BOX_DELETED, szBuf, 100);
	HTREEITEM h4 = InsertItem(szBuf, 3, 8, hRoot, h3);

	m_hRootItem = hRoot;
	m_hToSendItem = h1;
	m_hSendedItem = h2;
	m_hReceivedItem = h3;
	m_hDeletedItem = h4;

	SelectViewMode(CSmsClientView::SMS_TOSEND_VIEW);
}
VOID CSmsBoxTree::SelectViewMode(CSmsClientView::SMS_LIST_VIEW_MODE mode)
{
	Expand(m_hRootItem);

	switch(mode)
	{
	case CSmsClientView::SMS_TOSEND_VIEW:
		SelectItem(m_hToSendItem);
		break;
	case CSmsClientView::SMS_SENDED_VIEW:
		SelectItem(m_hSendedItem);
		break;
	case CSmsClientView::SMS_RECEIVED_VIEW:
		SelectItem(m_hReceivedItem);
		break;
	case CSmsClientView::SMS_DELETED_VIEW:
		SelectItem(m_hDeletedItem);
		break;
	}
}