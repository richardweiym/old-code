#include "StdAfx.h"
#include "SmsClientview.h"
#include "SmsListView.h"
#include "Smscontacttree.h"
#include "SmsBoxTree.h"
#include "SmsDetail.h"
#include "mainfrm.h"
#include "SmsConfig.h"
#include "SmsSendMessage.h"
#include "ServiceManager.h"
#include "ContactManager.h"
#include "SmsSearch.h"
#include "SmsMessageLib.h"
#include "SmsClientDlg.h"
#include "userManager.h"

BOOL CSmsClientView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	
	return FALSE;
}
BOOL CSmsClientView::OnIdle()
{
	return FALSE;
}
CSmsClientView::CSmsClientView(CMainFrame* p):m_SmsSplitterPos(200,300,300)
{
	m_pMainFrame = p;
	m_dwViewMode = SMS_BOX_VISIBLE | SMS_CONTACT_VISIBLE | SMS_CONTENT_VISIBLE | SMS_LIST_VISIBLE;
	m_pSmsBoxTree = new CSmsBoxTree;
	m_pSmsContactTree = new CSmsContactTree;
	m_pToSendMessages = new CSmsListView<CToSendMessage>;
	m_pSendedMessages = new CSmsListView<CSendedMessage>;
	m_pReceiveMessages = new CSmsListView<CReceiveMessage>;
	m_pDeletedMessages = new CSmsListView<CDeletedMessage>;
	m_pSmsDetailDlg = new CSmsDetailDlg;
	m_pInformProxy = new CInformProxy;
	m_eViewMode = SMS_TOSEND_VIEW;
}
CSmsClientView::~CSmsClientView()
{
	if(m_pSmsBoxTree != NULL)
	{
		delete m_pSmsBoxTree;
		m_pSmsBoxTree = NULL;
	}
	if(m_pSmsContactTree != NULL)
	{
		delete m_pSmsContactTree;
		m_pSmsContactTree = NULL;
	}
	if(m_pToSendMessages != NULL)
	{
		delete m_pToSendMessages;
		m_pToSendMessages = NULL;
	}
	if(m_pSendedMessages != NULL)
	{
		delete m_pSendedMessages;
		m_pSendedMessages = NULL;
	}
	if(m_pReceiveMessages != NULL)
	{
		delete m_pReceiveMessages;
		m_pReceiveMessages = NULL;
	}
	if(m_pDeletedMessages != NULL)
	{
		delete m_pDeletedMessages;
		m_pDeletedMessages = NULL;
	}
	if(m_pSmsDetailDlg != NULL)
	{
		delete m_pSmsDetailDlg;
		m_pSmsDetailDlg = NULL;
	}	
	if (m_pInformProxy != NULL)
	{
		delete m_pInformProxy;
		m_pInformProxy = NULL;
	}
}
LRESULT CSmsClientView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	BOOL bHandled = FALSE;
	CPaintDC dc(m_hWnd);
	return 0;
}
LRESULT CSmsClientView::OnSize(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rect;
	GetClientRect(&rect);
	::MoveWindow(m_wndSplitLR,0,0,rect.Width(),rect.Height(),TRUE);	
	return 0;
}
LRESULT CSmsClientView::OnCreate(UINT nMsgType , WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CreateSplitWindow();

	UISetCheck(ID_VIEW_BOX, TRUE);
	UISetCheck(ID_VIEW_CONTACTLIST, TRUE);
	UISetCheck(ID_VIEW_CONTENT, TRUE);
	
	return 0;
}
VOID CSmsClientView::LoadViewSplitConfig()
{
	CSmsClientConfig::CViewConfig viewCfg(15, 150, 200, 300);
	CSmsClientConfig* pConfig = m_pMainFrame->GetSmsClientConfig();
	pConfig->GetViewConfig(viewCfg);
	m_dwViewMode = viewCfg.m_nMode;
	m_SmsSplitterPos.nLR = viewCfg.m_nLR;
	m_SmsSplitterPos.nTB1 = viewCfg.m_nTB1;
	m_SmsSplitterPos.nTB2 = viewCfg.m_nTB2;
}
VOID CSmsClientView::SaveViewSplitConfig()
{
	OnViewModeChanged();

	CSmsClientConfig::CViewConfig viewCfg(15, 150, 200, 300);
	CSmsClientConfig* pConfig = m_pMainFrame->GetSmsClientConfig();
	viewCfg.m_nMode = m_dwViewMode;
	viewCfg.m_nLR = m_SmsSplitterPos.nLR ;
	viewCfg.m_nTB1 = m_SmsSplitterPos.nTB1;
	viewCfg.m_nTB2 = m_SmsSplitterPos.nTB2;
	pConfig->SaveViewConfig(viewCfg);

}

VOID CSmsClientView::CreateSplitWindow()
{
	const DWORD dwSplitStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	const DWORD dwSplitExStyle = WS_EX_CLIENTEDGE;

	//create
	m_wndSplitLR.Create(m_hWnd, rcDefault, NULL, dwSplitStyle, dwSplitExStyle);
	m_wndSplitTB1.Create(m_wndSplitLR, rcDefault, NULL, dwSplitStyle, dwSplitExStyle);
	m_wndSplitTB2.Create(m_wndSplitLR, rcDefault, NULL, dwSplitStyle, dwSplitExStyle);
	
	m_SmsBoxContainer.Create(m_wndSplitTB1);
	TCHAR szBuf[100] = {0};
	::LoadString(CMainFrame::GetResourceModule(), IDS_SMS_BOX, szBuf, 100);
	m_SmsBoxContainer.SetTitle(szBuf);
	m_pSmsBoxTree->Create(m_SmsBoxContainer,rcDefault, NULL, 0, 0, ID_SMS_BOX);
	m_pSmsBoxTree->InitBox();
	m_SmsBoxContainer.SetClient(m_pSmsBoxTree->m_hWnd);

	m_SmsContactContainer.Create(m_wndSplitTB1);
	::LoadString(CMainFrame::GetResourceModule(), IDS_SMS_CONTACT, szBuf, 100);
	m_SmsContactContainer.SetTitle(szBuf);
	m_pSmsContactTree->Create(m_SmsContactContainer,rcDefault,NULL, 0, 0, ID_SMS_CONTACT);
	m_SmsContactContainer.SetClient(m_pSmsContactTree->m_hWnd);

	m_wndSplitTB1.SetSplitterPanes(m_SmsBoxContainer,m_SmsContactContainer);
	
	m_pReceiveMessages->Create(m_wndSplitTB2, rcDefault, NULL, 0, 0, ID_SMS_LISTVIEW_RECEIVED);
	m_pSendedMessages->Create(m_wndSplitTB2, rcDefault, NULL, 0, 0, ID_SMS_LISTVIEW_SENDED);	
	m_pToSendMessages->Create(m_wndSplitTB2, rcDefault, NULL, 0, 0, ID_SMS_LISTVIEW_TOSEND);
	m_pDeletedMessages->Create(m_wndSplitTB2, rcDefault, NULL, 0, 0, ID_SMS_LISTVIEW_DELETED);

	m_pSmsDetailDlg->Create(m_wndSplitTB2);

 	m_wndSplitTB2.SetSplitterPanes(m_pToSendMessages->m_hWnd, m_pSmsDetailDlg->m_hWnd);
	m_wndSplitLR.SetSplitterPanes(m_wndSplitTB1,m_wndSplitTB2);

	m_pInformProxy->Init();
}
VOID CSmsClientView::UpdateSplit()
{
	m_wndSplitLR.SetSplitterPos(m_SmsSplitterPos.nLR);
	m_wndSplitTB1.SetSplitterPos(m_SmsSplitterPos.nTB1);
	m_wndSplitTB2.SetSplitterPos(m_SmsSplitterPos.nTB2);
}
VOID CSmsClientView::LoadContacts()
{
	m_pSmsContactTree->InitContacts();
}
VOID CSmsClientView::LoadListContent(SMS_LIST_VIEW_MODE mode)
{
	switch(mode)
	{
	case SMS_TOSEND_VIEW:
		m_pToSendMessages->LoadListContent();		
		break;

	case SMS_SENDED_VIEW:
		m_pSendedMessages->LoadListContent();
		break;

	case SMS_RECEIVED_VIEW:
		m_pReceiveMessages->LoadListContent();
		break;

	case SMS_DELETED_VIEW:
		m_pDeletedMessages->LoadListContent();	
		break;
	}
	
}
VOID CSmsClientView::OnViewModeChanged()
{
	INT nLRMode = m_wndSplitLR.GetSinglePaneMode();
	INT nTB1Mode = m_wndSplitTB1.GetSinglePaneMode();
	INT nTB2Mode = m_wndSplitTB2.GetSinglePaneMode();

	if(nLRMode == SPLIT_PANE_NONE)
	{
		m_SmsSplitterPos.nLR = m_wndSplitLR.GetSplitterPos();
	}
	if(nTB1Mode == SPLIT_PANE_NONE)
	{
		m_SmsSplitterPos.nTB1 = m_wndSplitTB1.GetSplitterPos();
	}
	if(nTB2Mode == SPLIT_PANE_NONE)
	{
		m_SmsSplitterPos.nTB2 = m_wndSplitTB2.GetSplitterPos();
	}
	
	if((m_dwViewMode & SMS_BOX_VISIBLE) || (m_dwViewMode & SMS_CONTACT_VISIBLE))
	{
		m_wndSplitLR.SetSinglePaneMode(SPLIT_PANE_NONE);
		m_wndSplitLR.SetSplitterPos(m_SmsSplitterPos.nLR);

		if((m_dwViewMode & SMS_BOX_VISIBLE) && (m_dwViewMode & SMS_CONTACT_VISIBLE))
		{
			m_wndSplitTB1.SetSinglePaneMode(SPLIT_PANE_NONE);
			m_wndSplitTB1.SetSplitterPos(m_SmsSplitterPos.nTB1);
		}
		else if(m_dwViewMode & SMS_BOX_VISIBLE)
		{
			m_wndSplitTB1.SetSinglePaneMode(SPLIT_PANE_TOP);
		}
		else
		{
			m_wndSplitTB1.SetSinglePaneMode(SPLIT_PANE_BOTTOM);
		}
	}
	else
	{
		m_wndSplitLR.SetSinglePaneMode(SPLIT_PANE_RIGHT);
	}
	
	if(m_dwViewMode & SMS_CONTENT_VISIBLE)
	{
		m_wndSplitTB2.SetSinglePaneMode(SPLIT_PANE_NONE);
		m_wndSplitTB2.SetSplitterPos(m_SmsSplitterPos.nTB2);
	}
	else
	{	
		m_wndSplitTB2.SetSinglePaneMode(SPLIT_PANE_TOP);
	}
}
LRESULT CSmsClientView::OnViewBox(WORD , WORD , HWND , BOOL& )
{
	BOOL bCheck = m_dwViewMode & SMS_BOX_VISIBLE;
	if(bCheck)
	{
		m_dwViewMode &= ~SMS_BOX_VISIBLE;
	}
	else
	{	
		m_dwViewMode |= SMS_BOX_VISIBLE;
	}
	UISetCheck(ID_VIEW_BOX, !bCheck);
	OnViewModeChanged();

	return 0;
}
LRESULT CSmsClientView::OnViewContactList(WORD , WORD , HWND , BOOL& )
{
	BOOL bCheck = m_dwViewMode & SMS_CONTACT_VISIBLE;
	if(bCheck)
	{
		m_dwViewMode &= ~SMS_CONTACT_VISIBLE;
	}
	else
	{	
		m_dwViewMode |= SMS_CONTACT_VISIBLE;
	}
	UISetCheck(ID_VIEW_CONTACTLIST, !bCheck);
	OnViewModeChanged();
	return 0;
}
LRESULT CSmsClientView::OnViewContent(WORD , WORD , HWND , BOOL& )
{
	BOOL bCheck = m_dwViewMode & SMS_CONTENT_VISIBLE;
	if(bCheck)
	{
		m_dwViewMode &= ~SMS_CONTENT_VISIBLE;
	}
	else
	{	
		m_dwViewMode |= SMS_CONTENT_VISIBLE;
	}
	UISetCheck(ID_VIEW_CONTENT, !bCheck);
	OnViewModeChanged();
	return 0;
}
LRESULT CSmsClientView::OnPaneClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
	BOOL bCheck;
	if(hWndCtl == m_SmsBoxContainer)
	{
		bCheck = m_dwViewMode & SMS_BOX_VISIBLE;
		if(bCheck)
		{
			m_dwViewMode &= ~SMS_BOX_VISIBLE;
		}
		else
		{	
			m_dwViewMode |= SMS_BOX_VISIBLE;
		}
		UISetCheck(ID_VIEW_BOX, !bCheck);	
	}
	else if(hWndCtl == m_SmsContactContainer)
	{
		bCheck = m_dwViewMode & SMS_CONTACT_VISIBLE;
		if(bCheck)
		{
			m_dwViewMode &= ~SMS_CONTACT_VISIBLE;
		}
		else
		{	
			m_dwViewMode |= SMS_CONTACT_VISIBLE;
		}
		UISetCheck(ID_VIEW_CONTACTLIST, !bCheck);	
	}

	OnViewModeChanged();
	return 0;
}
VOID CSmsClientView::ChangeListView(SMS_LIST_VIEW_MODE id)
{
	switch(id)
	{
	case SMS_TOSEND_VIEW:
		m_wndSplitTB2.SetSplitterPane(SPLIT_PANE_TOP, *m_pToSendMessages);
		m_eViewMode = SMS_TOSEND_VIEW;
		if(m_pToSendMessages->IsWindow())	m_pToSendMessages->ShowWindow(SW_SHOW);
		if(m_pSendedMessages->IsWindow())	m_pSendedMessages->ShowWindow(SW_HIDE);
		if(m_pReceiveMessages->IsWindow())	m_pReceiveMessages->ShowWindow(SW_HIDE);
		if(m_pDeletedMessages->IsWindow())  m_pDeletedMessages->ShowWindow(SW_HIDE);
		UISetCheck(ID_VIEW_TOSEND, TRUE);
		UISetCheck(ID_VIEW_SENDED, FALSE);
		UISetCheck(ID_VIEW_RECEIVED, FALSE);
		UISetCheck(ID_VIEW_DELETED, FALSE);		
		break;
	case SMS_SENDED_VIEW:
		m_wndSplitTB2.SetSplitterPane(SPLIT_PANE_TOP, *m_pSendedMessages);
		m_eViewMode = SMS_SENDED_VIEW;
		if(m_pToSendMessages->IsWindow())  m_pToSendMessages->ShowWindow(SW_HIDE);
		if(m_pSendedMessages->IsWindow())  m_pSendedMessages->ShowWindow(SW_SHOW);
		if(m_pReceiveMessages->IsWindow()) m_pReceiveMessages->ShowWindow(SW_HIDE);
		if(m_pDeletedMessages->IsWindow())  m_pDeletedMessages->ShowWindow(SW_HIDE);
		UISetCheck(ID_VIEW_TOSEND, FALSE);
		UISetCheck(ID_VIEW_SENDED, TRUE);
		UISetCheck(ID_VIEW_RECEIVED, FALSE);
		UISetCheck(ID_VIEW_DELETED, FALSE);	
		break;
	case SMS_RECEIVED_VIEW:
		m_wndSplitTB2.SetSplitterPane(SPLIT_PANE_TOP, *m_pReceiveMessages);
		m_eViewMode = SMS_RECEIVED_VIEW;
		if(m_pToSendMessages->IsWindow()) m_pToSendMessages->ShowWindow(SW_HIDE);
		if(m_pSendedMessages->IsWindow()) m_pSendedMessages->ShowWindow(SW_HIDE);
		if(m_pReceiveMessages->IsWindow()) m_pReceiveMessages->ShowWindow(SW_SHOW);
		if(m_pDeletedMessages->IsWindow())  m_pDeletedMessages->ShowWindow(SW_HIDE);
		UISetCheck(ID_VIEW_TOSEND, FALSE);
		UISetCheck(ID_VIEW_SENDED, FALSE);
		UISetCheck(ID_VIEW_RECEIVED, TRUE);
		UISetCheck(ID_VIEW_DELETED, FALSE);	
		break;
	case SMS_DELETED_VIEW:
		m_wndSplitTB2.SetSplitterPane(SPLIT_PANE_TOP, *m_pDeletedMessages);
		m_eViewMode = SMS_DELETED_VIEW;
		if(m_pToSendMessages->IsWindow()) m_pToSendMessages->ShowWindow(SW_HIDE);
		if(m_pSendedMessages->IsWindow()) m_pSendedMessages->ShowWindow(SW_HIDE);
		if(m_pReceiveMessages->IsWindow()) m_pReceiveMessages->ShowWindow(SW_HIDE);
		if(m_pDeletedMessages->IsWindow())  m_pDeletedMessages->ShowWindow(SW_SHOW);
		UISetCheck(ID_VIEW_TOSEND, FALSE);
		UISetCheck(ID_VIEW_SENDED, FALSE);
		UISetCheck(ID_VIEW_RECEIVED, FALSE);
		UISetCheck(ID_VIEW_DELETED, TRUE);
		break;
	}
}
LRESULT CSmsClientView::OnListviewNotify(int , LPNMHDR pnmh , BOOL& )
{
	if(pnmh->code == NM_RCLICK)
	{
		HINSTANCE hRes = CMainFrame::GetResourceModule();
		HMENU hMenu = LoadMenu(hRes, MAKEINTRESOURCE(IDR_MAINFRAME));
		if(hMenu != NULL)
		{
			HMENU hSub = GetSubMenu(hMenu, 1);
			if(hSub != NULL)
			{
				INT nCount = GetCurrentCount(TRUE);
				if(nCount <= 0)
				{
					EnableMenuItem(hSub, ID_SMS_REPLY, MF_BYCOMMAND|MF_GRAYED);
					EnableMenuItem(hSub, ID_SMS_TRANSFER, MF_BYCOMMAND|MF_GRAYED);
					EnableMenuItem(hSub, ID_SMS_DELETE, MF_BYCOMMAND|MF_GRAYED);
					EnableMenuItem(hSub, ID_SMS_SAVE, MF_BYCOMMAND|MF_GRAYED);
				}
				nCount = GetCurrentCount(FALSE);
				if(nCount <= 0)
				{
					EnableMenuItem(hSub, ID_SMS_CLEAR, MF_BYCOMMAND|MF_GRAYED);
				}

				POINT pt;
				GetCursorPos(&pt);
				TrackPopupMenu(hSub, TPM_LEFTALIGN|TPM_TOPALIGN,pt.x, pt.y, 0, *m_pMainFrame, NULL);
			}
		}
		DestroyMenu(hMenu);
	}
	
	if(pnmh->code == LVN_ITEMCHANGED )
	{
		LPNMLISTVIEW lp = (LPNMLISTVIEW)pnmh;
		INT nItem = lp->iItem;

		BOOL bSet(FALSE);
		CString strContactCaption,strContactName,strTimeCapiton,strTimeValue,strContent;
			
		if(pnmh->idFrom == ID_SMS_LISTVIEW_DELETED)
		{
			if(lp->uNewState & LVIS_SELECTED) 
			{
				m_pDeletedMessages->GetSelItemValue(nItem,strContactCaption,strContactName,strTimeCapiton,strTimeValue,strContent);
				bSet = TRUE;
			}
		}
		else if(pnmh->idFrom == ID_SMS_LISTVIEW_RECEIVED)
		{
			if(lp->uNewState & LVIS_SELECTED)
			{
				m_pReceiveMessages->GetSelItemValue(nItem, strContactCaption,strContactName,strTimeCapiton,strTimeValue,strContent);
				bSet = TRUE;
			}
		}
		else if(pnmh->idFrom == ID_SMS_LISTVIEW_SENDED)
		{
			if(lp->uNewState & LVIS_SELECTED) 
			{
				m_pSendedMessages->GetSelItemValue(nItem, strContactCaption,strContactName,strTimeCapiton,strTimeValue,strContent);
				bSet = TRUE;
			}
		}
		else if(pnmh->idFrom == ID_SMS_LISTVIEW_TOSEND)
		{
			if(lp->uNewState & LVIS_SELECTED)
			{
				m_pToSendMessages->GetSelItemValue(nItem, strContactCaption,strContactName,strTimeCapiton,strTimeValue,strContent);
				bSet = TRUE;
			}
		}
		
		if(bSet)
		{
			m_pSmsDetailDlg->SetValues(strContactCaption,strContactName,strTimeCapiton,strTimeValue,strContent);
		}			
	}

	if(pnmh->code == NM_DBLCLK)
	{
		CString strName,strTel,strContent;
		if(GetCurrenSelInfo(strName, strTel, strContent))
		{
			CSmsReadMsgDlg dlg(strName, strTel, strContent, *CMainFrame::GetMainFrame());
			dlg.DoModal();
		}
	}
	return 0;
}

LRESULT CSmsClientView::OnBoxTree(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	if(pnmh->code == TVN_SELCHANGED)
	{
		TCHAR sz[4][100];
		HINSTANCE hRes = CMainFrame::GetResourceModule();
		::LoadString(hRes, IDS_BOX_TOSEND, sz[0], sizeof(sz[0]));
		::LoadString(hRes, IDS_BOX_SENDED, sz[1], sizeof(sz[0]));
		::LoadString(hRes, IDS_BOX_RECEIVED, sz[2], sizeof(sz[0]));
		::LoadString(hRes, IDS_BOX_DELETED, sz[3], sizeof(sz[0]));
		HTREEITEM hItem = m_pSmsBoxTree->GetSelectedItem();
		if(hItem != NULL)
		{
			TCHAR szBuf[100] = {0};
			m_pSmsBoxTree->GetItemText(hItem, szBuf, 100);

			if(lstrcmpi(sz[0], szBuf) == 0)
			{
				ChangeListView(SMS_TOSEND_VIEW);
			}
			else if(lstrcmpi(sz[1], szBuf) == 0)
			{
				ChangeListView(SMS_SENDED_VIEW);
			}
			else if(lstrcmpi(sz[2], szBuf) == 0)
			{
				ChangeListView(SMS_RECEIVED_VIEW);
			}
			else if(lstrcmpi(sz[3], szBuf) == 0)
			{
				ChangeListView(SMS_DELETED_VIEW);	
			}

			m_pMainFrame->PostMessage(WM_VIEW_CHANGED, 0, 0);
		}
	}
	return 0;
}

LRESULT CSmsClientView::OnViewToSend(WORD , WORD , HWND , BOOL& )
{
	m_pSmsBoxTree->SelectViewMode(SMS_TOSEND_VIEW);	
	return 0;
}
LRESULT CSmsClientView::OnViewSended(WORD , WORD , HWND , BOOL& )
{	
	m_pSmsBoxTree->SelectViewMode(SMS_SENDED_VIEW);	
	return 0;
}
LRESULT CSmsClientView::OnViewReceived(WORD , WORD , HWND , BOOL& )
{
	m_pSmsBoxTree->SelectViewMode(SMS_RECEIVED_VIEW);	
	return 0;
}
LRESULT CSmsClientView::OnViewDeleted(WORD , WORD , HWND , BOOL& )
{
	m_pSmsBoxTree->SelectViewMode(SMS_DELETED_VIEW);	
	return 0;
}
LRESULT CSmsClientView::OnViewCurrentButton(WORD , WORD , HWND , BOOL& )
{
	INT nNext = m_eViewMode+1;
	if(nNext == SMS_DELETED_VIEW+1)
	{
		nNext = SMS_TOSEND_VIEW;
	}
	
	m_pSmsBoxTree->SelectViewMode((SMS_LIST_VIEW_MODE)nNext);

	return 0;
}

LRESULT CSmsClientView::OnSmsNew(WORD, WORD, HWND, BOOL&)
{
	CSmsSendDlg dlg(ID_SMS_NEW);
	INT nRet = dlg.DoModal();
	if(nRet == IDC_SEND)
	{
		m_pSmsBoxTree->SelectViewMode(SMS_TOSEND_VIEW);		
	}
	return 0;
}
BOOL CSmsClientView::GetCurrenSelInfo(CString& strName, CString& strTel, CString& strContent)
{
	BOOL bOK(FALSE);
	switch(m_eViewMode)
	{
	case SMS_TOSEND_VIEW:
		bOK = m_pToSendMessages->GetCurrentSelInfo(strName, strTel, strContent);
		break;
	case SMS_SENDED_VIEW:
		bOK = m_pSendedMessages->GetCurrentSelInfo(strName, strTel, strContent);
		break;
	case SMS_RECEIVED_VIEW:
		bOK = m_pReceiveMessages->GetCurrentSelInfo(strName, strTel, strContent);
		break;
	case SMS_DELETED_VIEW:
		bOK = m_pDeletedMessages->GetCurrentSelInfo(strName, strTel, strContent);
		break;
	}

	return bOK;
}
LRESULT CSmsClientView::OnSmsReply(WORD, WORD, HWND, BOOL&)
{
	CString strName;
	CString strTel;
	CString strContent;
	if(GetCurrenSelInfo(strName, strTel, strContent))
	{
		CSmsSendDlg dlg(ID_SMS_REPLY);
		dlg.m_strName = strName;
		dlg.m_strTel = strTel;
		dlg.m_strContent = strContent;
		INT nRet = dlg.DoModal();
		if(nRet == IDC_SEND)
		{
			m_pSmsBoxTree->SelectViewMode(SMS_TOSEND_VIEW);		
		}
	}
	else
	{
		MessageBox(_T("请先选中你要回复的短信!"), _T("提示信息"), MB_ICONINFORMATION|MB_OK);
	}

	return 0;
}
LRESULT CSmsClientView::OnSmsTransfer(WORD, WORD, HWND, BOOL&)
{
	CString strName;
	CString strTel;
	CString strContent;
	if(GetCurrenSelInfo(strName, strTel, strContent))
	{
		CSmsSendDlg dlg(ID_SMS_TRANSFER);
		dlg.m_strContent = strContent;
		INT nRet = dlg.DoModal();
		if(nRet == IDC_SEND)
		{
			m_pSmsBoxTree->SelectViewMode(SMS_TOSEND_VIEW);		
		}
	}
	else
	{
		MessageBox(_T("请先选中你要转发的短信!"), _T("提示信息"), MB_ICONINFORMATION|MB_OK);
	}
	return 0;
}
LRESULT CSmsClientView::OnSmsClear(WORD, WORD, HWND, BOOL&)
{
	DoDelete(TRUE);
	return 0;
}
LRESULT CSmsClientView::OnSmsSearch(WORD, WORD, HWND, BOOL&)
{
	CSmsSearchDlg dlg;
	dlg.DoModal();

	return 0;
}
LRESULT CSmsClientView::OnSmsSave(WORD, WORD, HWND, BOOL&)
{
	CString strName;
	CString strTel;
	CString strContent;
	if(GetCurrenSelInfo(strName, strTel, strContent))
	{
		CMessageLib msgLib;
		msgLib.LoadMsg();
		CMessageDlg dlg(strContent, 0, TRUE, msgLib.GetMsgMap());
		INT nRet = dlg.DoModal();
		if(nRet == IDC_OK)
		{
			if(dlg.m_nIndex >=0)
			{
				CMessage* pKey = msgLib.GetMsgMap().GetKeyAt(dlg.m_nIndex);
				CMessage msg;
				msg.m_bType = FALSE;
				msg.m_nTypeId = pKey->m_nId;
				msg.m_strContent = dlg.m_strContent;
				if(msgLib.AddMessage(&msg))
				{
					MessageBox(_T("短信收藏保存成功!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
				}
			}
			else
			{
				MessageBox(_T("短信收藏失败, 请先新建一个组!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
			}

		}
		msgLib.ClearAll();
	}
	else
	{
		MessageBox(_T("请先选中你要收藏到短信库的短信!"), _T("提示信息"), MB_ICONINFORMATION|MB_OK);
	}

	return 0;
}
INT CSmsClientView::GetCurrentCount(BOOL bSelCount)
{
	INT nCount(0);
	switch(m_eViewMode)
	{
	case SMS_TOSEND_VIEW:
		nCount = m_pToSendMessages->GetCount(bSelCount);
		break;
	case SMS_SENDED_VIEW:
		nCount = m_pSendedMessages->GetCount(bSelCount);
		break;
	case SMS_RECEIVED_VIEW:
		nCount = m_pReceiveMessages->GetCount(bSelCount);
		break;
	case SMS_DELETED_VIEW:
		nCount = m_pDeletedMessages->GetCount(bSelCount);
		break;
	}

	return nCount;
}
VOID CSmsClientView::DoDelete(BOOL bDelAll)
{
	CString  strBox;
	INT nCount = GetCurrentCount(FALSE);

	switch(m_eViewMode)
	{
	case SMS_TOSEND_VIEW:
		m_pToSendMessages->GetTypeName(strBox);
		break;
	case SMS_SENDED_VIEW:
		m_pSendedMessages->GetTypeName(strBox);
		break;
	case SMS_RECEIVED_VIEW:
		m_pReceiveMessages->GetTypeName(strBox);
		break;
	case SMS_DELETED_VIEW:
		m_pDeletedMessages->GetTypeName(strBox);
		break;
	}

	if(bDelAll)
	{
		if(nCount>0)
		{
			CString	strInfo;
			strInfo.Format(_T("你确定要清空 %s 信箱中的所有短信?"), strBox);
			INT nRet = MessageBox(strInfo,  _T("提示信息"), MB_OKCANCEL|MB_ICONQUESTION);
			if(nRet != IDOK)
			{
				return;	
			}
		}
		else
		{
			return;
		}

	}
	else
	{
		nCount = GetCurrentCount(TRUE);
		if(nCount > 0)
		{
			INT nRet = MessageBox(_T("你确定要删除所选中的这些短信?"), _T("提示信息"), MB_OKCANCEL|MB_ICONQUESTION);
			if(nRet != IDOK)
			{
				return;
			}
		}
	}
	

	BOOL bOK(FALSE);
	CSimpleArray<CDeletedMessage*> arDeletedMsg;
	{
		CWaitCursor wait;
	
		switch(m_eViewMode)
		{
		case SMS_TOSEND_VIEW:
			bOK = m_pToSendMessages->DeleteSelMsg(arDeletedMsg, bDelAll);
			if(bOK)
			{
				m_pDeletedMessages->SetDeleteMsg(arDeletedMsg);
			}
			break;
		case SMS_SENDED_VIEW:
			bOK = m_pSendedMessages->DeleteSelMsg(arDeletedMsg, bDelAll);
			if(bOK)
			{
				m_pDeletedMessages->SetDeleteMsg(arDeletedMsg);
			}
			break;
		case SMS_RECEIVED_VIEW:
			bOK = m_pReceiveMessages->DeleteSelMsg(arDeletedMsg, bDelAll);
			if(bOK)
			{
				m_pDeletedMessages->SetDeleteMsg(arDeletedMsg);
			}
			break;
		case SMS_DELETED_VIEW:
			bOK = m_pDeletedMessages->DeleteSelMsg(arDeletedMsg, bDelAll);
			break;
		}
	}
	if(!bOK)
	{
		MessageBox(_T("请先选中你要删除的短信!"), _T("提示信息"), MB_ICONINFORMATION|MB_OK);
	}
	else
	{

		for(int i=0; i<arDeletedMsg.GetSize(); i++)
		{
			if(arDeletedMsg[i] != NULL)
			{
				delete arDeletedMsg[i];
			}		
		}
		arDeletedMsg.RemoveAll();
	}

	m_pMainFrame->PostMessage(WM_VIEW_CHANGED, 0, 0);
}
LRESULT CSmsClientView::OnSmsDelete(WORD, WORD, HWND, BOOL&)
{
	DoDelete(FALSE);
	return 0;
}

BOOL CSmsClientView::DoSend1Message()
{
	CServiceManager* pServiceMgr = CMainFrame::GetMainFrame()->GetServiceMgr();
	ISmsClient* pClient = pServiceMgr->GetSmsClient();
	BOOL bGet = m_pToSendMessages->Get1ToSendMessage(m_SendingMsg);
	if(bGet)
	{
		TCHAR szBuf[20] = {0};
		_itot(m_SendingMsg.m_nToSendId, szBuf, 10);
		CSMSMessage msg(m_SendingMsg.m_strTel, m_SendingMsg.m_strContent, szBuf, REQUEST_SEND, 0);
		return pClient->SendMsg(&msg);
	}
	return FALSE;
}
BOOL CSmsClientView::DoSended1Message(CSMSMessage* pMsg)
{
	int nToSendId = _ttoi(pMsg->GetStr3());
	BOOL bSendedOK = (pMsg->GetResult() & SMS_MESSAGE_SENDBACK_OK_FLAG)? TRUE: FALSE;
	if(nToSendId == m_SendingMsg.m_nToSendId)
	{
		m_pToSendMessages->SetSendedMessage(m_SendingMsg, bSendedOK);
		m_pSendedMessages->SetSendedMessage(m_SendingMsg, bSendedOK);
		return TRUE;
	}

	return FALSE;
}

BOOL CSmsClientView::DoReceivedMessage(CSMSMessage* pMsg)
{
	CReceiveMessage msg;
	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	CString strTime;
	strTime.Format(_T("%d-%d-%d %d:%d:%d"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	

	msg.m_strTel = pMsg->GetStr1();
	msg.m_strContent = pMsg->GetStr2();
	msg.m_nUserId = 0;
	msg.m_strReceiveTime = strTime;
	msg.m_bDeleted = FALSE;
	msg.m_bReaded = FALSE;

	CString strName;
	if(CContactManager::GetNameByTel(msg.m_strTel, strName))
	{
		msg.m_strName = strName;
	}
	else
	{
		msg.m_strName = _T("未知");
	}

	m_pReceiveMessages->SetReceivedMsg(&msg);
	
	BOOL bInform(TRUE);
	CServiceManager* pServiceMgr = m_pMainFrame->GetServiceMgr();
	if(pServiceMgr != NULL)
	{
		CUserManager* pUserManager = pServiceMgr->GetUserManager();
		if(pUserManager != NULL)
		{
			BOOL b;
			CSmsUser* pUser = pUserManager->GetCurrentUser(b);
			if(b && pUser != NULL)
			{
				bInform = pUser->m_bTipOn;
			}
		}
	}
	if(bInform)
	{
		ShowInformWindow(msg);
	}

	return TRUE;
}
VOID CSmsClientView::OnDisconnect()
{
	m_pToSendMessages->ResetSendingMsg(m_SendingMsg.m_nToSendId);
}
LRESULT CSmsClientView::OnContactNotify(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	if(pnmh->code == NM_DBLCLK)
	{
		SendMsgToSel(TRUE);
	}

	if(pnmh->code == NM_RCLICK)
	{
		CPoint pt;
		GetCursorPos(&pt);
		m_pSmsContactTree->ScreenToClient(&pt);
		UINT nFlag(0);
		BOOL bCanSend(FALSE);
		HTREEITEM hSel = m_pSmsContactTree->HitTest(pt, &nFlag);
		if(hSel != NULL)
		{
			m_pSmsContactTree->SelectItem(hSel);
			if(nFlag & TVHT_ONITEMLABEL)
			{
				bCanSend = TRUE;
			}
		}
		
		nFlag = MF_STRING;
		CPoint ptMenu;
		GetCursorPos(&ptMenu);
		
		HMENU hMenu = CreatePopupMenu();
		AppendMenu(hMenu, nFlag, ID_TOOL_CONTACTLIST, _T("通讯簿"));
		AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
		if(!bCanSend) nFlag |= MF_GRAYED;
		AppendMenu(hMenu, nFlag, ID_SEND_MSG, _T("发送短信"));
		TrackPopupMenu(hMenu, TPM_LEFTALIGN|TPM_TOPALIGN, ptMenu.x, ptMenu.y, 0, *m_pMainFrame, NULL);
		DestroyMenu(hMenu);
	}
	return 0;
}
VOID CSmsClientView::SendMsgToSel(BOOL bOnlyLeaf)
{
	HTREEITEM hItem = m_pSmsContactTree->GetSelectedItem();
	CSimpleMap<HTREEITEM, CContact*>& map = m_pSmsContactTree->GetItemMap();
	CContact* pContact = map.Lookup(hItem);
	if(pContact != NULL)
	{
		BOOL bCanSend(TRUE);
		if(bOnlyLeaf) 
		{
			bCanSend = pContact->m_bLeaf;
		}

		if(bCanSend)
		{
			CSmsSendDlg dlg(ID_SEND_MSG);
			dlg.m_nSenderId = pContact->m_nId;
			dlg.m_strContent = _T("");
			INT nRet = dlg.DoModal();
			if(nRet == IDC_SEND)
			{
				m_pSmsBoxTree->SelectViewMode(SMS_TOSEND_VIEW);		
			}
		}

	}
}
LRESULT CSmsClientView::OnSendMsg(WORD, WORD, HWND, BOOL&)
{
	SendMsgToSel();

	return 0;
}
LRESULT CSmsClientView::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	m_pInformProxy->Release();

	return 0;
}
VOID CSmsClientView::ShowInformWindow(CReceiveMessage& msg)
{
	m_pInformProxy->ShowInformation(msg.m_strName, msg.m_strTel, msg.m_strContent);
}