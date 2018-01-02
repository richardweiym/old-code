#pragma  once

#include <atlctrls.h>
#include "SmsClientview.h"

class CSmsBoxTree: public CWindowImpl<CSmsBoxTree, CTreeViewCtrl, CControlWinTraits>
{
public:
	BEGIN_MSG_MAP(CSmsBoxTree)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	CSmsBoxTree(){}
	LRESULT OnCreate(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	

	VOID InitBox();

	VOID SelectViewMode(CSmsClientView::SMS_LIST_VIEW_MODE mode);
private:
	CImageList m_imagelist;

	HTREEITEM m_hRootItem;
	HTREEITEM m_hToSendItem;
	HTREEITEM m_hSendedItem;
	HTREEITEM m_hReceivedItem;
	HTREEITEM m_hDeletedItem;
};