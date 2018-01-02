#include "StdAfx.h"
#include "SmsMessageLib.h"
#include "mainfrm.h"
#include "DBOperate.h"
#include "ContactManager.h"
#include "SmsSendMessage.h"

CMessage::CMessage():m_strContent(_T(""))
{
	m_nId = 0;
	m_bType = FALSE;
	m_nTypeId = 0;
}
CMessage::CMessage(const CMessage& msg)
:m_strContent(msg.m_strContent)
{
	m_nTypeId = msg.m_nTypeId;
	m_bType = msg.m_bType;
	m_nId = msg.m_nId;
}
CMessage& CMessage::operator =(const CMessage& msg)
{
	m_strContent = msg.m_strContent;
	m_nTypeId = msg.m_nTypeId;
	m_bType = msg.m_bType;
	m_nId = msg.m_nId;

	return *this;
}

CMessageLib::~CMessageLib()
{
	ClearAll();
}
BOOL CMessageLib::LoadMsg()
{
	BOOL bOK(FALSE);
	_RecordsetPtr pRst = NULL;
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	CString strSql(_T("select * from messageLibrary where isType = TRUE"));
	if(pDB != NULL)
	{
		if(pDB->GetRecordset(strSql, &pRst))
		{
			try
			{
				_variant_t vt;
				while(!pRst->GetADOEOF())
				{
					CMessage* pMsg = new CMessage;
					pMsg->m_bType = TRUE;
					vt = pRst->GetCollect(_T("Id"));
					if(vt.vt != VT_NULL)
					{
						pMsg->m_nId = (long)vt;
					}
					vt = pRst->GetCollect(_T("content"));
					if(vt.vt != VT_NULL)
					{
						pMsg->m_strContent = (LPCTSTR)_bstr_t(vt);
					}
					pRst->MoveNext();
					m_map.Add(pMsg, new CSimpleArray<CMessage*>);
				}
				pRst->Close();
				pRst = NULL;

				for(int i=0; i<m_map.GetSize(); i++)
				{
					CMessage* pKey = m_map.GetKeyAt(i);
					CSimpleArray<CMessage*>* pVal = m_map.GetValueAt(i);
					strSql.Format(_T("select * from messageLibrary where isType = 0 and typeId = %d"), pKey->m_nId);
					if(pDB->GetRecordset(strSql, &pRst))
					{
						while(!pRst->GetADOEOF())
						{
							CMessage* p = new CMessage;
							p->m_bType = FALSE;
							p->m_nTypeId = pKey->m_nId;
							
							vt = pRst->GetCollect(_T("Id"));
							if(vt.vt != VT_NULL)
							{
								p->m_nId = (long)vt;
							}
							vt = pRst->GetCollect(_T("content"));
							if(vt.vt != VT_NULL)
							{
								p->m_strContent = (LPCTSTR)_bstr_t(vt);
							}
							pRst->MoveNext();
							pVal->Add(p);
						}
						pRst->Close();
						pRst = NULL;
					}
				}
			}
			catch (...)
			{
				NULL;
			}
			
		}
	}


	return bOK;
}
VOID CMessageLib::ClearAll()
{
	for(int i=0; i<m_map.GetSize(); i++)
	{
		CMessage* pMsg = m_map.GetKeyAt(i);
		if(pMsg != NULL)
		{
			delete pMsg;
		}

		CSimpleArray<CMessage*>* pAMsg = m_map.GetValueAt(i);
		if(pAMsg != NULL)
		{
			for(int j=0; j<pAMsg->GetSize(); j++)
			{
				pMsg = pAMsg->operator [](j);
				if(pMsg != NULL)
				{
					delete pMsg;
				}
			}	
			pAMsg->RemoveAll();
			delete pAMsg;
		}
	}
	m_map.RemoveAll();
}

BOOL CMessageLib::AddMessage(CMessage* pMsg)
{
	BOOL bOK(FALSE);
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		_RecordsetPtr pRst = NULL;
		LPCTSTR strSQL = _T("select top 1 * from messageLibrary");
		if(pDB->GetRecordset(strSQL, &pRst))
		{
			try
			{
				pRst->AddNew();
				pRst->PutCollect(_T("isType"), (_variant_t)long(pMsg->m_bType));
				pRst->PutCollect(_T("typeId"), (_variant_t)long(pMsg->m_nTypeId));
				pRst->PutCollect(_T("content"), (_variant_t)_bstr_t(pMsg->m_strContent));
				pRst->Update();

				pMsg->m_nId = (long)pRst->GetCollect(_T("Id"));
				pRst->Close();
				bOK = TRUE;
			}
			catch (...)
			{
				NULL;
			}
		}
	}

	return bOK;
}
BOOL CMessageLib::ModifyMessage(CMessage* pMsg)
{
	BOOL bOK(FALSE);
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		_RecordsetPtr pRst = NULL;
		CString strSQL;
		 strSQL.Format(_T("select top 1 * from messageLibrary where Id = %d"), pMsg->m_nId);
		 if(pDB->GetRecordset(strSQL, &pRst))
		 {
			 try
			 {
				 if(!pRst->GetADOEOF())
				 {
					pRst->PutCollect(_T("isType"), (_variant_t)long(pMsg->m_bType));
					pRst->PutCollect(_T("typeId"), (_variant_t)long(pMsg->m_nTypeId));
					pRst->PutCollect(_T("content"), (_variant_t)_bstr_t(pMsg->m_strContent));
					pRst->Update();
					bOK = TRUE;
				 }
				 pRst->Close();
			 }
			 catch (...)
			 {
			 	NULL;
			 }
		 }
	}
	return bOK;
}
BOOL CMessageLib::DeleteMessage(CMessage* pMsg)
{
	BOOL bOK(FALSE);
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		CString strSQL1;
		if(pMsg->m_bType)
		{
			CString strSQL1;
			strSQL1.Format(_T("delete from messageLibrary where isType = false and typeId = %d"), pMsg->m_nId);
			bOK = pDB->ExecuteSQL(strSQL1);
		}
		CString strSQL2;
		strSQL2.Format(_T("delete from messageLibrary where id = %d"), pMsg->m_nId);
		bOK = pDB->ExecuteSQL(strSQL2);
	}

	return bOK;
}

LRESULT CTypeList::OnCreate(UINT nMsgType , WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	//m_imagelist.Create(IDB_MESSAGE_LIB, 16, 0, RGB(255,0,255));
	m_imagelist.Create(32, 32, ILC_COLOR24|ILC_MASK, 3, 0);
	HBITMAP hBmp = LoadBitmap(CMainFrame::GetResourceModule(), MAKEINTRESOURCE(IDB_MESSAGE_LIB));
	if(hBmp != NULL)
	{
		m_imagelist.Add(hBmp, RGB(255,0,255));
		DeleteObject(hBmp);
	}
	SetImageList(m_imagelist,LVSIL_SMALL);
	bHandled = FALSE;
	
	DWORD dwExStyle = GetExtendedListViewStyle();
	dwExStyle |=  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES;
	SetExtendedListViewStyle(dwExStyle);
	

	LVCOLUMN lvc = { 0 };
	lvc.mask =  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;

	int nIndex(0);
	lvc.pszText = (LPTSTR)_T("");
	lvc.cx = 50;
	InsertColumn(nIndex++, &lvc);
	
	lvc.pszText = _T("类别");
	lvc.cx = 120;
	InsertColumn(nIndex++,&lvc);
	return 0;
}
LRESULT CTypeList::OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_imagelist.Destroy();
	return 0;
}
VOID CTypeList::LoadTypes()
{
	DeleteAllItems();
	CSimpleMap<CMessage*, CSimpleArray<CMessage*>* >& map = m_pDlg->GetMsgLib().GetMsgMap();
	for(int i=0; i<map.GetSize(); i++)
	{
		CMessage* pMsg = map.GetKeyAt(i);
		if(pMsg != NULL)
		{
			AddItem(i, 0, _T(""), 0);
			AddItem(i, 1, pMsg->m_strContent);
		}
	}
}

LRESULT CMessageList::OnCreate(UINT nMsgType , WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	//m_imagelist.Create(IDB_MESSAGE_LIB, 16, 0, RGB(255,0,255));
	m_imagelist.Create(32, 32, ILC_COLOR24|ILC_MASK, 3, 0);
	HBITMAP hBmp = LoadBitmap(CMainFrame::GetResourceModule(), MAKEINTRESOURCE(IDB_MESSAGE_LIB));
	if(hBmp != NULL)
	{
		m_imagelist.Add(hBmp, RGB(255,0,255));
		DeleteObject(hBmp);
	}

	SetImageList(m_imagelist,LVSIL_SMALL);
	bHandled = FALSE;
	
	DWORD dwExStyle = GetExtendedListViewStyle();
	dwExStyle |=  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES ;
	SetExtendedListViewStyle(dwExStyle);
	

	LVCOLUMN lvc = { 0 };
	lvc.mask =  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;

	int nIndex(0);
	lvc.pszText = (LPTSTR)_T("");
	lvc.cx = 50;
	InsertColumn(nIndex++, &lvc);
	
	lvc.pszText = _T("内容");
	lvc.cx = 550;
	InsertColumn(nIndex++,&lvc);
	return 0;
}	
LRESULT CMessageList::OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_imagelist.Destroy();
	return 0;
}
VOID CMessageList::LoadMessage(INT nIndex)
{
	DeleteAllItems();

	CSimpleMap<CMessage*, CSimpleArray<CMessage*>* >& map = m_pDlg->GetMsgLib().GetMsgMap();
	CSimpleArray<CMessage*>* pMsgList = map.GetValueAt(nIndex);
	if(pMsgList != NULL)
	{
		DeleteAllItems();
		for(int i=0; i<pMsgList->GetSize(); i++)
		{
			AddItem(i, 0, _T(""), 2);
			CMessage* pMsg = pMsgList->operator [](i);
			AddItem(i, 1, pMsg->m_strContent);
		}
	}
}


CTransferMsgDlg::CTransferMsgDlg(CMessageLibDlg* pDlg, INT nIndex)
{
	m_pMsgLibDlg = pDlg;
	m_nIndex = nIndex;
}
LRESULT CTransferMsgDlg::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	CenterWindow();
	SetWindowText(_T("转移短语"));
	CComboBox cb = GetDlgItem(IDC_TEAM);
	CMessageLib& lib = m_pMsgLibDlg->GetMsgLib();
	CSimpleMap<CMessage*, CSimpleArray<CMessage*>*>& map = lib.GetMsgMap();
	CMessage* pKey = NULL;

	for(int i=0; i<map.GetSize(); i++)
	{
		pKey = map.GetKeyAt(i);
		if(pKey != NULL)
		{
			cb.AddString(pKey->m_strContent);
		}
	}	
	cb.SetCurSel(m_nIndex);
	return 0;
}
LRESULT CTransferMsgDlg::OnOK(WORD, WORD, HWND, BOOL&)
{
	CComboBox cb = GetDlgItem(IDC_TEAM);
	m_nIndex = cb.GetCurSel();
	EndDialog(IDC_OK);
	return 0;
}
LRESULT CTransferMsgDlg::OnCancel(WORD, WORD, HWND, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}
LRESULT CTransferMsgDlg::OnClose(UINT ,WPARAM, LPARAM, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}


CMessageDlg::CMessageDlg(CString& strContent, INT nIndex, BOOL bAdd, CSimpleMap<CMessage*, CSimpleArray<CMessage*>*>& map)
:m_strContent(strContent), m_map(map)
{
	m_nIndex = nIndex;
	m_bAdd = bAdd;
//	m_pMsgLibDlg = pDlg;
}
LRESULT CMessageDlg::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	CenterWindow();

	CComboBox cb = GetDlgItem(IDC_TYPE);
	CSimpleMap<CMessage*, CSimpleArray<CMessage*>*>& map = m_map;
	CMessage* pKey = NULL;

	for(int i=0; i<map.GetSize(); i++)
	{
		pKey = map.GetKeyAt(i);
		if(pKey != NULL)
		{
			cb.AddString(pKey->m_strContent);
		}
	}	
	cb.SetCurSel(m_nIndex);
	
	CEdit edit = GetDlgItem(IDC_CONTENT);
	edit.SetLimitText(980);
	if(m_bAdd)
	{
		SetDlgItemText(IDC_CONTENT, m_strContent);
		SetWindowText(_T("新增短语"));
	}
	else
	{
		SetDlgItemText(IDC_CONTENT, m_strContent);
		SetWindowText(_T("修改短语"));
	}

	return 0;
}
LRESULT CMessageDlg::OnCancel(WORD, WORD, HWND, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}
LRESULT CMessageDlg::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}
LRESULT CMessageDlg::OnOK(WORD, WORD, HWND, BOOL&)
{

	GetDlgItemText(IDC_CONTENT, m_strContent.GetBuffer(1024), 1024);
	m_strContent.ReleaseBuffer();

	if(m_strContent == _T(""))
	{
		MessageBox(_T("短语内容不能为空!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}

	CComboBox cb = GetDlgItem(IDC_TYPE);
	m_nIndex =	cb.GetCurSel();
	EndDialog(IDC_OK);
	
	return 0;
}

CMessageLibDlg::CMessageLibDlg(CSmsSendDlg* pDlg)
{
	m_pContent = new CContentEdit;
	m_pTypeList = new CTypeList(this);
	m_pMessageList = new CMessageList(this);
	m_nCurrentIndex = 0;
	m_pSendDlg = pDlg;
}
CMessageLibDlg::~CMessageLibDlg()
{
	delete m_pContent;
	delete m_pTypeList;
	delete m_pMessageList;
}
VOID CMessageLibDlg::UpdateLayout()
{
	RECT rect = { 0 };
	GetClientRect(&rect);	
	BOOL bResizeBars = TRUE;

	if(m_hWndToolBar != NULL && ((DWORD)::GetWindowLong(m_hWndToolBar, GWL_STYLE) & WS_VISIBLE))
	{
		if(bResizeBars)
		{
			::SendMessage(m_hWndToolBar, WM_SIZE, 0, 0);
			::InvalidateRect(m_hWndToolBar, NULL, FALSE);
		}
		RECT rectTB = { 0 };
		::GetWindowRect(m_hWndToolBar, &rectTB);
		rect.top += rectTB.bottom - rectTB.top;
	}

	if(m_hWndStatusBar != NULL && ((DWORD)::GetWindowLong(m_hWndStatusBar, GWL_STYLE) & WS_VISIBLE))
	{
		if(bResizeBars)
			::SendMessage(m_hWndStatusBar, WM_SIZE, 0, 0);
		RECT rectSB = { 0 };
		::GetWindowRect(m_hWndStatusBar, &rectSB);
		rect.bottom -= rectSB.bottom - rectSB.top;
	}		

	if(m_hWndClient != NULL)
		::SetWindowPos(m_hWndClient, NULL, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_NOZORDER | SWP_NOACTIVATE);
}
LRESULT CMessageLibDlg::OnInitDlg(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	const DWORD dwSplitStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	const DWORD dwSplitExStyle = WS_EX_CLIENTEDGE;
	
	CenterWindow();
	
	m_wndSplitLR.Create(m_hWnd, rcDefault, NULL, dwSplitStyle, dwSplitExStyle);
	m_wndSplitTB.Create(m_wndSplitLR, rcDefault, NULL, dwSplitStyle, dwSplitExStyle);
	

	m_pContent->Create(m_wndSplitTB, rcDefault, NULL, 0, 0, ID_CONTENT);
	m_pMessageList->Create(m_wndSplitTB, rcDefault, NULL, 0, 0, ID_MESSAGE_LIST);

	m_pTypeList->Create(m_wndSplitLR, rcDefault, NULL, 0, 0, ID_TYPE_LIST);
	m_wndSplitLR.SetSplitterPanes(*m_pTypeList, m_wndSplitTB);	
	m_wndSplitTB.SetSplitterPanes(*m_pMessageList, *m_pContent);
	
	m_hWndClient = m_wndSplitLR;
	m_hWndToolBar = CMainFrame::CreateSimpleReBarCtrl(m_hWnd, ATL_SIMPLE_REBAR_NOBORDER_STYLE, ATL_IDW_TOOLBAR);
	HWND hToolBar = CMainFrame::CreateSimpleToolBarCtrl(m_hWnd, IDR_MESSAGE_LIB, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE_EX);
	AddToolbarButtonText(hToolBar, ID_ADD_TYPE, _T("新建组"));
	AddToolbarButtonText(hToolBar, ID_MODIFY_TYPE, _T("修改组"));
	AddToolbarButtonText(hToolBar, ID_DELETE_TYPE, _T("删除组"));
	AddToolbarButtonText(hToolBar, ID_ADD_MESSAGE, _T("新建短语"));
	AddToolbarButtonText(hToolBar, ID_MODIFY_MESSAGE, _T("修改短语"));
	AddToolbarButtonText(hToolBar, ID_DELETE_MESSAGE, _T("删除短语"));
	AddToolbarButtonText(hToolBar, ID_TRANSFER_MESSAGE, _T("转移短语"));
	AddToolbarButtonText(hToolBar, ID_INSERT_CONTENT, _T("插入短语"));
	AddToolbarButtonText(hToolBar, ID_EXIT, _T("关闭"));

	CMainFrame::AddSimpleReBarBandCtrl(m_hWndToolBar, hToolBar, 0, NULL, FALSE, 0, FALSE);

	m_hWndStatusBar = ::CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, _T("就绪"), m_hWnd, ATL_IDW_STATUS_BAR);

	UpdateLayout();
	m_wndSplitLR.SetSplitterPos(150);
	m_wndSplitTB.SetSplitterPos(320);
	

	m_pContent->SetLimitText(980);
	m_pContent->SetReadOnly(TRUE);
	m_pContent->SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	
	//m_pContent->SetFont((HFONT)GetStockObject(SYSTEM_FONT));

	m_msgLib.LoadMsg();
	m_pTypeList->LoadTypes();
	m_nCurrentIndex = 0;
	if(m_pTypeList->GetItemCount() > 0)
	{	
		m_pTypeList->SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
	
	return 0;
}

LRESULT CMessageLibDlg::OnSize(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam != SIZE_MINIMIZED)
	{
		UpdateLayout();
	}
	bHandled = FALSE;	
	return 0;
}
LRESULT CMessageLibDlg::OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close();
	return 0;
}
VOID CMessageLibDlg::Close()
{
	m_msgLib.ClearAll();
	EndDialog(0);
}
LRESULT CMessageLibDlg::OnExit(WORD, WORD, HWND, BOOL&)
{
	Close();
	return 0;
}
BOOL CMessageLibDlg::GetCurrentSelInfo(CString& strContent)
{
	BOOL bFind(FALSE);
	INT i(-1);
	for(i=0; i<m_pMessageList->GetItemCount(); i++)
	{
		INT nState = m_pMessageList->GetItemState(i, LVIS_SELECTED);
		if( nState & LVIS_SELECTED)
		{
			bFind = TRUE;
			break;
		}
	}
	if(bFind)
	{
		m_pMessageList->GetItemText(i, 1, strContent);
	}

	return bFind;

}
LRESULT CMessageLibDlg::OnMessageListNotify(int, LPNMHDR pnmh, BOOL&)
{
	if(pnmh->code == LVN_ITEMCHANGED)
	{
		LPNMLISTVIEW lp = (LPNMLISTVIEW)pnmh;
		INT nItem = lp->iItem;

		if(lp->uNewState & LVIS_SELECTED)
		{
			CString strContent;
			m_pMessageList->GetItemText(nItem, 1, strContent);
			m_pContent->SetWindowText(strContent);
		}
	}

	if(pnmh->code == NM_DBLCLK)
	{
		PostMessage(WM_COMMAND, ID_INSERT_CONTENT, 0);
	}
	
	if(pnmh->code == NM_RCLICK)
	{
		CString strContent;
		UINT nFlag(MF_STRING);
		if(!GetCurrentSelInfo(strContent))
		{
			nFlag |= MF_GRAYED;
		}

		HMENU hParent = CreatePopupMenu();
		if(hParent != NULL)
		{
			AppendMenu(hParent, MF_STRING, ID_ADD_MESSAGE, _T("新建短语"));
			AppendMenu(hParent, nFlag, ID_MODIFY_MESSAGE, _T("修改短语"));
			AppendMenu(hParent, nFlag, ID_DELETE_MESSAGE, _T("删除短语"));
			AppendMenu(hParent, nFlag, ID_TRANSFER_MESSAGE, _T("转移短语"));
			AppendMenu(hParent, MF_SEPARATOR, NULL, NULL);

			if(m_pSendDlg == NULL)
			{
				nFlag |= MF_GRAYED;
			}
			AppendMenu(hParent, nFlag, ID_INSERT_CONTENT, _T("插入短语"));

			POINT pt;
			GetCursorPos(&pt);
			TrackPopupMenu(hParent, TPM_LEFTALIGN|TPM_TOPALIGN, pt.x, pt.y, 0, *this, NULL);

			DestroyMenu(hParent);
		}
	}

	return 0;
}
VOID CMessageLibDlg::SetItemImage(CListViewCtrl& list, INT nIndex, INT nImage)
{
	LVITEM lvItem;
	lvItem.iItem = nIndex;
	lvItem.iSubItem = 0;
	lvItem.mask = LVIF_IMAGE;
	if(list.GetItem(&lvItem))
	{
		if(lvItem.iImage != nImage)
		{
			lvItem.iImage = nImage;
			list.SetItem(&lvItem);
		}
	}
}
VOID CMessageLibDlg::SetSelectedType(INT nIndex)
{
	for(int i=0; i<m_pTypeList->GetItemCount(); i++)
	{
		if(i == nIndex)
		{
			SetItemImage(*m_pTypeList, i, 1);
		}
		else
		{
			SetItemImage(*m_pTypeList, i, 0);
		}
		
	}
}

LRESULT CMessageLibDlg::OnTypeListNotify(int, LPNMHDR pnmh, BOOL&)
{
	if(pnmh->code == LVN_ITEMCHANGED)
	{
		LPNMLISTVIEW lp = (LPNMLISTVIEW)pnmh;
		INT nItem = lp->iItem;

		if(lp->uNewState & LVIS_SELECTED)
		{
			m_nCurrentIndex = nItem;
			m_pMessageList->LoadMessage(m_nCurrentIndex);	
				
			SetSelectedType(m_nCurrentIndex);

			CString str;
			m_pTypeList->GetItemText(nItem, 1, str);
			
			CString strInfo;
			strInfo.Format(_T("%s 类别下共有%d条短语"), str, m_pMessageList->GetItemCount());
			::SendMessage(m_hWndStatusBar, SB_SIMPLE, TRUE, 0L);
			::SendMessage(m_hWndStatusBar, SB_SETTEXT, (255 | SBT_NOBORDERS), (LPARAM)(LPCTSTR)strInfo);
		}
	}

	return 0;
}

LRESULT CMessageLibDlg::OnAddType(WORD, WORD, HWND, BOOL&)
{
	CTeamDlg dlg(_T(""), TRUE);
	INT nRet = dlg.DoModal();
	if(nRet == IDC_OK)
	{
		CWaitCursor wait;

		CMessage msg;
		msg.m_bType = TRUE;
		msg.m_nTypeId = 0;
		msg.m_strContent = dlg.m_strName;

		if(m_msgLib.AddMessage(&msg))
		{
			CSimpleMap<CMessage*, CSimpleArray<CMessage*>* >&  map = m_msgLib.GetMsgMap();
			map.Add(new CMessage(msg), new CSimpleArray<CMessage*>);
			m_pTypeList->LoadTypes();
			//m_pMessageList->LoadMessage(m_nCurrentIndex);
			m_nCurrentIndex = map.GetSize()-1;
			m_pTypeList->SetItemState(m_nCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
	return 0;
}
LRESULT CMessageLibDlg::OnModifyType(WORD, WORD, HWND, BOOL&)
{
	INT nIndex = m_pTypeList->GetSelectedIndex();
	if(nIndex < 0)
	{
		MessageBox(_T("请先选中你要修改的组!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	CString strType;
	m_pTypeList->GetItemText(nIndex, 1, strType);

	CTeamDlg dlg(strType, FALSE);
	INT nRet = dlg.DoModal();
	if(nRet == IDC_OK)
	{
		CWaitCursor wait;

		CSimpleMap<CMessage*, CSimpleArray<CMessage*>* >&  map = m_msgLib.GetMsgMap();
		CMessage* pMsg = map.GetKeyAt(nIndex);
		if(pMsg != NULL)
		{
			pMsg->m_strContent = dlg.m_strName;
			if(m_msgLib.ModifyMessage(pMsg))
			{
				m_pTypeList->LoadTypes();
				//m_pMessageList->LoadMessage(m_nCurrentIndex);	
				m_pTypeList->SetItemState(m_nCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
	}
	return 0;
}

LRESULT CMessageLibDlg::OnDeleteType(WORD, WORD, HWND, BOOL&)
{
	INT nIndex = m_pTypeList->GetSelectedIndex();
	if(nIndex < 0)
	{
		MessageBox(_T("请先选中你要删除的组!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}

	CString strType;
	m_pTypeList->GetItemText(nIndex, 1, strType);
	
	CString strInfo;
	strInfo.Format(_T("删除组将删除属于该组的所有联系人,\n你确定要删除 %s 组及其成员?"), strType);
	INT nRet = MessageBox(strInfo, _T("提示信息"), MB_OKCANCEL|MB_ICONQUESTION);
	if(nRet == IDOK)
	{
		CWaitCursor wait;

		CSimpleMap<CMessage*, CSimpleArray<CMessage*>* >&  map = m_msgLib.GetMsgMap();
		CMessage* pMsg = map.GetKeyAt(nIndex);
		CSimpleArray<CMessage*>* pMsgList = map.GetValueAt(nIndex);

		
		if(m_msgLib.DeleteMessage(pMsg))
		{
			map.Remove(pMsg);
			delete pMsg;
			for(int i=0; i< pMsgList->GetSize(); i++)
			{
				delete (pMsgList->operator [](i));
			}
			pMsgList->RemoveAll();
			delete pMsgList;

			m_pTypeList->LoadTypes();
			if(m_pTypeList->GetItemCount() >0)
			{
				m_nCurrentIndex = 0;
			//	m_pMessageList->LoadMessage(m_nCurrentIndex);
				m_pTypeList->SetItemState(m_nCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);
			}
			else
			{
				m_pMessageList->DeleteAllItems();
			}

		}	
	}
	
	return 0;
}

LRESULT CMessageLibDlg::OnAddMessage(WORD, WORD, HWND, BOOL&)
{
	CSimpleMap<CMessage*, CSimpleArray<CMessage*>* >&  map = m_msgLib.GetMsgMap();
	if(map.GetSize() == 0)
	{
		MessageBox(_T("没有组存在,请先进行新增组操作!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	
	CMessageDlg dlg(CString(_T("")), m_nCurrentIndex, TRUE, map);
	INT nRet = dlg.DoModal();
	if(nRet == IDC_OK)
	{	
		CWaitCursor wait;

		if(dlg.m_nIndex >= 0)
		{
			m_nCurrentIndex = dlg.m_nIndex;

			CMessage* pKey = map.GetKeyAt(m_nCurrentIndex);
			CSimpleArray<CMessage*>* pMsgList = map.GetValueAt(m_nCurrentIndex);

			if(pKey != NULL && pMsgList != NULL)
			{
				CMessage msg;
				msg.m_bType = FALSE;
				msg.m_nTypeId = pKey->m_nId;
				msg.m_strContent = dlg.m_strContent;

				if(m_msgLib.AddMessage(&msg))
				{
					CMessage* pM = new CMessage(msg);
					pMsgList->Add(pM);
				//	m_pMessageList->LoadMessage(m_nCurrentIndex);
					m_pTypeList->SetItemState(m_nCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);
				}
			}
		}
	}

	return 0;
}
LRESULT CMessageLibDlg::OnInsertContent(WORD, WORD, HWND, BOOL&)
{
	if(m_pSendDlg == NULL)
	{
		MessageBox(_T("只有在发送状态下才能插入短语! "), _T("提示信息"), MB_ICONINFORMATION|MB_OK);
		return 0;
	}

	INT nSel(-1);
	for(int i=0; i<m_pMessageList->GetItemCount(); i++)
	{
		if((m_pMessageList->GetItemState(i, LVIS_SELECTED)) & LVIS_SELECTED)
		{
			nSel = i;
			break;
		}
	}
	if(nSel == -1)
	{
		MessageBox(_T("请先选中你要插入的短语!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}	


	CString strContent;
	m_pMessageList->GetItemText(nSel, 1, strContent);
	
	Close();

	m_pSendDlg->InsertContent(strContent);
	
	return 0;
}
LRESULT CMessageLibDlg::OnModifyMessage(WORD, WORD, HWND, BOOL&)
{
	INT nSel(-1);
	for(int i=0; i<m_pMessageList->GetItemCount(); i++)
	{
		if((m_pMessageList->GetItemState(i, LVIS_SELECTED)) & LVIS_SELECTED)
		{
			nSel = i;
			break;
		}
	}
	if(nSel == -1)
	{
		MessageBox(_T("请先选中你要修改的短语!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	
	CSimpleMap<CMessage*, CSimpleArray<CMessage*>* >&  map = m_msgLib.GetMsgMap();
	CMessage* pKey = map.GetKeyAt(m_nCurrentIndex);
	CSimpleArray<CMessage*>* pMsgList = map.GetValueAt(m_nCurrentIndex);
	CMessage* pModifyMsg = pMsgList->operator [](nSel);

	CMessageDlg dlg(pModifyMsg->m_strContent, m_nCurrentIndex, FALSE, map);
	INT nRet = dlg.DoModal();
	if(nRet == IDC_OK)
	{
		CWaitCursor wait;

		pModifyMsg->m_strContent = dlg.m_strContent;

		INT nIndex = dlg.m_nIndex;
		if(m_nCurrentIndex == nIndex)
		{
			if(m_msgLib.ModifyMessage(pModifyMsg))
			{
				m_pMessageList->LoadMessage(m_nCurrentIndex);
				//m_pTypeList->SetItemState(m_nCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
		else
		{
			//INT n = nIndex;
			//m_nCurrentIndex = nIndex;
			pKey = map.GetKeyAt(nIndex);
			pModifyMsg->m_nTypeId = pKey->m_nId;

			if(m_msgLib.ModifyMessage(pModifyMsg))
			{
				pMsgList->RemoveAt(nSel);
				pMsgList = map.GetValueAt(nIndex);
				pMsgList->Add(pModifyMsg);
				m_pMessageList->LoadMessage(m_nCurrentIndex);
			//	m_pTypeList->SetItemState(m_nCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
	}

	return 0;
}
LRESULT CMessageLibDlg::OnDeleteMessage(WORD, WORD, HWND, BOOL&)
{
	INT nCount = m_pMessageList->GetSelectedCount();
	if(nCount <= 0)
	{
		MessageBox(_T("请先选中你要删除的短语!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}

	INT nRet = MessageBox(_T("你确定要删除你所选中的这些短语吗?"), _T("提示信息"), MB_OKCANCEL|MB_ICONQUESTION);
	if(nRet != IDOK) return 0;
	
	{
		CWaitCursor wait;
	
		CSimpleMap<CMessage*, CSimpleArray<CMessage*>* >&  map = m_msgLib.GetMsgMap();
		CMessage* pKey = map.GetKeyAt(m_nCurrentIndex);
		CSimpleArray<CMessage*>* pMsgList = map.GetValueAt(m_nCurrentIndex);

		for(int i=m_pMessageList->GetItemCount(); i>=0; i--)
		{
			if((m_pMessageList->GetItemState(i, LVIS_SELECTED)) & LVIS_SELECTED)
			{
				CMessage* pDelMsg = pMsgList->operator [](i);

				if(m_msgLib.DeleteMessage(pDelMsg))
				{
					pMsgList->RemoveAt(i);
					delete pDelMsg;
				}
			}
		}
		//m_pTypeList->SetItemState(m_nCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);
		m_pMessageList->LoadMessage(m_nCurrentIndex);
	}
	return 0;
}

LRESULT CMessageLibDlg::OnTransferMsg(WORD, WORD, HWND, BOOL&)
{
	INT nCount = m_pMessageList->GetSelectedCount();
	if(nCount <= 0)
	{
		MessageBox(_T("请先选中你要转移的短语!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	
	CTransferMsgDlg dlg(this, m_nCurrentIndex);
	INT nRet = dlg.DoModal();
	if(nRet != IDC_OK) return 0;

	INT nIndex = dlg.m_nIndex;
	if(m_nCurrentIndex == nIndex) return 0;
	
	{
		CWaitCursor wait;
	
		CSimpleMap<CMessage*, CSimpleArray<CMessage*>* >&  map = m_msgLib.GetMsgMap();
		CMessage* pKey = map.GetKeyAt(m_nCurrentIndex);
		CSimpleArray<CMessage*>* pMsgList = map.GetValueAt(m_nCurrentIndex);

		CMessage* pKey1 = map.GetKeyAt(nIndex);
		CSimpleArray<CMessage*>* pMsgList1 = map.GetValueAt(nIndex);

		for(int i=m_pMessageList->GetItemCount(); i>=0; i--)
		{
			if((m_pMessageList->GetItemState(i, LVIS_SELECTED)) & LVIS_SELECTED)
			{
				CMessage* pModifyMsg = pMsgList->operator [](i);
				pModifyMsg->m_nTypeId = pKey1->m_nId;
				
				if(m_msgLib.ModifyMessage(pModifyMsg))
				{
					pMsgList->RemoveAt(i);
					pMsgList1->Add(pModifyMsg);
				}
			}
		}
		m_pMessageList->LoadMessage(m_nCurrentIndex);
	}
	//m_nCurrentIndex = nIndex;
	//m_pTypeList->SetItemState(m_nCurrentIndex, LVIS_SELECTED, LVIS_SELECTED);

	return 0;
}