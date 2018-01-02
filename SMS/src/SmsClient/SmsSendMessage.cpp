#include "StdAfx.h"
#include "mainfrm.h"
#include "SmsSendMessage.h"
#include "ContactManager.h"
#include "SmsListView.h"
#include "SmsMessageLib.h"

LRESULT CAddTel::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	CenterWindow();
	CEdit edit = GetDlgItem(IDC_TEL);
	edit.SetLimitText(50);
	return 0;
}
LRESULT CAddTel::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}
LRESULT CAddTel::OnCancel(WORD, WORD, HWND, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}
LRESULT CAddTel::OnOK(WORD, WORD, HWND, BOOL&)
{
	TCHAR szBuf[100] = {0};
	GetDlgItemText(IDC_TEL, szBuf, 100);
	m_strTel = szBuf;
	m_strTel.TrimLeft();
	m_strTel.TrimRight();
	if(m_strTel == _T(""))
	{
		MessageBox(_T("请输入你要新增的手机号码!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	EndDialog(IDC_OK);	
	return 0;
}

CSmsSendDlg::CSmsSendDlg(INT nID)
{
	m_nWorkID = nID;
}
LRESULT CSmsSendDlg::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
	EndDialog(IDC_CLOSE);
	return 0;
}
LRESULT CSmsSendDlg::OnTimerClick(WORD wParam, WORD, HWND hWnd, BOOL&)
{
	if(wParam == BN_CLICKED)
	{
		BOOL bTimer = (::SendMessage(hWnd, BM_GETCHECK, 0, 0L) & BST_CHECKED);
		::EnableWindow(GetDlgItem(IDC_DATE), bTimer);
		::EnableWindow(GetDlgItem(IDC_TIME), bTimer);	
	}

	return 0;
}
LRESULT CSmsSendDlg::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	CenterWindow();	
	m_contactTree.SubclassWindow(GetDlgItem(IDC_CONTACT));
	m_contactTree.InitContacts();
	
	m_receiverList = GetDlgItem(IDC_RECEIVER);
	DWORD dwExStyle = m_receiverList.GetExtendedListViewStyle();
	dwExStyle |=  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES;
	m_receiverList.SetExtendedListViewStyle(dwExStyle);
	
	LVCOLUMN lvc = { 0 };
	lvc.mask =  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_CENTER;

	lvc.pszText = _T("名字");
	lvc.cx = 55;
	m_receiverList.InsertColumn(0,&lvc);

	lvc.pszText = _T("手机号码");
	lvc.cx = 110;
	m_receiverList.InsertColumn(1,&lvc);

	lvc.pszText = _T("HTREEITEM");
	lvc.cx = 0;
	m_receiverList.InsertColumn(2,&lvc);

	CEdit edit = GetDlgItem(IDC_CONTENT);
	edit.SetLimitText(980);
	SetDlgItemInt(IDC_CHARACTER_COUNT, 0);
	SetDlgItemInt(IDC_MESSAGE_COUNT, 0);
	::EnableWindow(GetDlgItem(IDC_DATE), FALSE);
	::EnableWindow(GetDlgItem(IDC_TIME), FALSE);

	CEdit dt = GetDlgItem(IDC_CONTENT);
	if(m_nWorkID == ID_SMS_REPLY)
	{
		AddReceiver(m_strName, m_strTel, NULL);
		
		dt.SetWindowText(m_strContent);
		dt.SetFocus();
		dt.SetSel(0, m_strContent.GetLength());
	}
	else if(m_nWorkID == ID_SMS_TRANSFER)
	{
		dt.SetWindowText(m_strContent);
	}
	else if(m_nWorkID == ID_SEND_MSG)
	{
		AddContactById(m_nSenderId);
	}

	PostMessage(WM_COMMAND, IDC_CONTENT|(DWORD(EN_CHANGE)<<16), (LPARAM)GetDlgItem(IDC_CONTENT));
	
	return 0;
}
LRESULT CSmsSendDlg::OnCmdClose(WORD, WORD, HWND, BOOL&)
{
	EndDialog(IDC_CLOSE);
	return 0;
}
LRESULT CSmsSendDlg::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	m_contactTree.UnsubclassWindow();
	
	return 0;
}
LRESULT CSmsSendDlg::OnContentChange(WORD, WORD, HWND, BOOL&)
{
	TCHAR szBuf[1025] = {0};
	GetDlgItemText(IDC_CONTENT, szBuf, 1025);

	INT nLen = lstrlen(szBuf);
	if(nLen>0)
	{
		nLen = MultiByteToWideChar(CP_ACP, 0, szBuf, nLen, NULL, 0);
	}

	SetDlgItemInt(IDC_CHARACTER_COUNT, nLen);
	INT nMsg = nLen/70 ;
	if(nLen%70 != 0)
	{
		nMsg++;
	}
	SetDlgItemInt(IDC_MESSAGE_COUNT, nMsg);

	return 0;
}
LRESULT CSmsSendDlg::OnNew(WORD, WORD, HWND, BOOL&)
{
	CAddTel dlg;
	INT nRet = dlg.DoModal();
	if(nRet == IDC_OK)
	{
		AddReceiver(CString(_T("未知")), dlg.m_strTel, NULL);
	}
	return 0;
}
LRESULT CSmsSendDlg::OnDelete(WORD, WORD, HWND, BOOL&)
{
	INT nSelected = m_receiverList.GetSelectedCount();
	if(nSelected <= 0)
	{
		MessageBox(_T("请选中要删除的接收人!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		for(INT i=m_receiverList.GetItemCount()-1; i>=0; i--)
		{
			if(m_receiverList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
			{
				m_receiverList.DeleteItem(i);
			}
		}
	}
	
	return 0;
}
LRESULT CSmsSendDlg::OnReceiverNotify(int, LPNMHDR pnmh, BOOL&)
{
	if(pnmh->code == NM_RCLICK)
	{
		HINSTANCE hRes = CMainFrame::GetResourceModule();
		HMENU hMenu = LoadMenu(hRes, MAKEINTRESOURCE(IDR_SEND_LIST));
		if(hMenu != NULL)
		{
			HMENU hSub = GetSubMenu(hMenu, 1);
			if(hSub != NULL)
			{
				if(m_receiverList.GetSelectedCount() <= 0)
				{
					EnableMenuItem(hSub, ID_DELETE, MF_BYCOMMAND|MF_GRAYED);
				}
				POINT pt;
				GetCursorPos(&pt);
				TrackPopupMenu(hSub, TPM_LEFTALIGN|TPM_TOPALIGN,pt.x, pt.y, 0, m_hWnd, NULL);
			}
		}
		DestroyMenu(hMenu);
	}
	if(pnmh->code == NM_DBLCLK)
	{
		for(INT i=m_receiverList.GetItemCount()-1; i>=0; i--)
		{
			if(m_receiverList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
			{
				m_receiverList.DeleteItem(i);
				break;
			}
		}
	}
	
	return 0;
}
LRESULT CSmsSendDlg::OnContactNotify(int, LPNMHDR pnmh, BOOL&)
{
	if(pnmh->code == NM_DBLCLK)
	{
		HTREEITEM hItem = m_contactTree.GetSelectedItem();
		CSimpleMap<HTREEITEM, CContact*>& map = m_contactTree.GetItemMap();
		CContact* pContact = map.Lookup(hItem);
		if((pContact != NULL) && (pContact->m_bLeaf))
		{
			if(!IsExistInReceiver(hItem))
			{
				AddReceiver(pContact->m_strName, pContact->m_strTel, hItem);
			}			
		}
	}
	if(pnmh->code == NM_RCLICK)
	{
		CPoint pt;
		GetCursorPos(&pt);
		m_contactTree.ScreenToClient(&pt);
		UINT nFlag(0);
		HTREEITEM hSel = m_contactTree.HitTest(pt, &nFlag);
		if(hSel != NULL)
		{
			m_contactTree.SelectItem(hSel);
			if(nFlag & TVHT_ONITEMLABEL)
			{
				CPoint ptMenu;
				GetCursorPos(&ptMenu);
				
				HMENU hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, IDC_ADD, _T("加入到发送列表"));
				TrackPopupMenu(hMenu, TPM_LEFTALIGN|TPM_TOPALIGN,ptMenu.x, ptMenu.y, 0, m_hWnd, NULL);
				DestroyMenu(hMenu);
			}
		}
	}

	return 0;
}
LRESULT CSmsSendDlg::OnClear(WORD, WORD, HWND, BOOL&)
{
	for(INT i=m_receiverList.GetItemCount()-1; i>=0; i--)
	{
		m_receiverList.DeleteItem(i);
	}	

	return 0;
}
BOOL CSmsSendDlg::IsExistInReceiver(HTREEITEM hItem)
{
	INT nCount = m_receiverList.GetItemCount();
	TCHAR szBuf[20] = {0};
	HTREEITEM hTemp = NULL;
	BOOL bFound(FALSE);

	for(int i=0; i<nCount; i++)
	{
		if(m_receiverList.GetItemText(i, 2, szBuf, 20))
		{
			hTemp = (HTREEITEM)(_ttoi(szBuf));
			if((hTemp != NULL) && (hTemp == hItem))
			{
				bFound = TRUE;
				break;
			}
		}
	}

	return bFound;
}
VOID CSmsSendDlg::AddReceiver(CString& strName, CString& strTel, HTREEITEM hItem)
{
	INT nIndex = m_receiverList.GetItemCount();
	m_receiverList.AddItem(nIndex, 0, strName);
	m_receiverList.AddItem(nIndex, 1, strTel);

	TCHAR szBuf[20] = {0};
	_itot((UINT_PTR)hItem, szBuf, 10);
	m_receiverList.AddItem(nIndex, 2, szBuf);
}
VOID CSmsSendDlg::AddContactByItem(HTREEITEM hItem)
{
	CSimpleMap<HTREEITEM, CContact*>& map = m_contactTree.GetItemMap();
	CContact* pContact = map.Lookup(hItem);
	if(pContact != NULL)
	{
		if(pContact->m_bLeaf)
		{
			if(!IsExistInReceiver(hItem))
			{
				AddReceiver(pContact->m_strName, pContact->m_strTel, hItem);
			}
		}
		else
		{
			CContact* pTemp = NULL;
			HTREEITEM hT1 = NULL;
			for(int k=0; k<map.GetSize(); k++)
			{
				pTemp = map.GetValueAt(k);
				if(pTemp != NULL)
				{
					if(pTemp->m_nParentId == pContact->m_nId)
					{
						hT1 = map.GetKeyAt(k);
						if(!IsExistInReceiver(hT1))
						{
							AddReceiver(pTemp->m_strName, pTemp->m_strTel, hT1);
						}
					}
				}
			}
		}
	}
}

VOID CSmsSendDlg::AddContactById(INT nId)
{
	CSimpleMap<HTREEITEM, CContact*>& map = m_contactTree.GetItemMap();
	CContact* pContact = NULL;
	BOOL bFound(FALSE);
	int i(0);
	for(i=0; i<map.GetSize(); i++)
	{
		pContact = map.GetValueAt(i);
		if(pContact != NULL)
		{
			if(pContact->m_nId == nId)
			{
				bFound = TRUE;
				break;
			}
		}
	}
	if(bFound)
	{
		HTREEITEM hItem = map.GetKeyAt(i);
		if(hItem)
		{
			AddContactByItem(hItem);
		}
	}
}
LRESULT CSmsSendDlg::OnAdd(WORD, WORD, HWND, BOOL&)
{
	HTREEITEM hItem = m_contactTree.GetSelectedItem();
	if(hItem == NULL)
	{
		MessageBox(_T("请在通讯簿中选中要加入的联系人或组!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	AddContactByItem(hItem);

	return 0;
}
VOID CSmsSendDlg::InsertContent(CString& strContent)
{
	CString strContentBuf;
	GetDlgItemText(IDC_CONTENT, strContentBuf.GetBuffer(1024), 1024);
	strContentBuf.ReleaseBuffer();
	strContentBuf += strContent;

	if(strContentBuf.GetLength() > 980)	
	{
		MessageBox(_T("短信长度超过最大限制,部分内容被截断!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
	}

	strContentBuf = strContentBuf.Left(980);
	SetDlgItemText(IDC_CONTENT, strContentBuf);
	PostMessage(WM_COMMAND, IDC_CONTENT|(DWORD(EN_CHANGE)<<16), (LPARAM)GetDlgItem(IDC_CONTENT));

	return ;
}
LRESULT CSmsSendDlg::OnInsertContent(WORD, WORD, HWND, BOOL&)
{
	CMessageLibDlg dlg(this);
	dlg.DoModal();

	return 0;
}
LRESULT CSmsSendDlg::OnSend(WORD, WORD, HWND, BOOL&)
{
	INT nCount = m_receiverList.GetItemCount();

	if(nCount <= 0)
	{
		MessageBox(_T("接收人不能为空!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	
	TCHAR szContent[1025] = {0};
	GetDlgItemText(IDC_CONTENT, szContent, 1025);
	CString strContent = szContent;
	if(strContent == _T(""))
	{
		MessageBox(_T("发送内容不能为空!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);	
		return 0;
	}
	
	CString strDate;
	CButton btn = GetDlgItem(IDC_ON_TIMER);
	BOOL bTimer = (btn.GetCheck() == BST_CHECKED);
	if(bTimer)
	{
		SYSTEMTIME st1,st2;
		CDateTimePickerCtrl dt = GetDlgItem(IDC_DATE);
		dt.GetSystemTime(&st1);

		dt = GetDlgItem(IDC_TIME);
		dt.GetSystemTime(&st2);
	
		strDate.Format(_T("%d-%d-%d %d:%d:%d"), st1.wYear, st1.wMonth, st1.wDay, 
			st2.wHour, st2.wMinute, st2.wSecond);


	}
	
	CString strNow;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strNow.Format(_T("%d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute,st.wSecond);
	
	CToSendMessage msg;
	TCHAR szBuf[120] = {0};
	INT nToSendId;

	CSmsListView<CToSendMessage>* pList = CMainFrame::GetClientView()->GetToSendList();
	
	{
		CWaitCursor wait;
	
		for(INT i=0; i<nCount; i++)
		{
			m_receiverList.GetItemText(i, 0, szBuf, 120);
			msg.m_strName = szBuf;
			m_receiverList.GetItemText(i, 1, szBuf, 120);
			msg.m_strTel = szBuf;
			msg.m_bSendPromptly = (!bTimer);
			msg.m_nUserId = 0;
			msg.m_strContent = strContent;
			msg.m_strCreateTime = strNow;
			msg.m_strToSendTime = strDate;
			
			nToSendId = CToSendMessage::AddMessage(msg);
			if(nToSendId != -1)
			{
				msg.m_nToSendId = nToSendId;
				
				if(pList != NULL)
				{
					pList->InsertMsg(&msg);
				}
			}

		}
	}
	
	CMainFrame::GetMainFrame()->PostMessage(WM_VIEW_CHANGED, 0, 0);
	EndDialog(IDC_SEND);

	return 0;
}

CSmsReadMsgDlg::CSmsReadMsgDlg(CString& strName, CString& strTel, CString& strContent, HWND hParent)
:m_strName(strName), m_strTel(strTel), m_strContent(strContent)
{
	m_hParent = hParent;
}

VOID CSmsReadMsgDlg::Close()	
{
	EndDialog(IDC_CLOSE);
}
LRESULT CSmsReadMsgDlg::OnCmdClose(WORD, WORD, HWND, BOOL&)
{
	Close();
	return 0;
}
LRESULT CSmsReadMsgDlg::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
	Close();
	return 0;
}
LRESULT CSmsReadMsgDlg::OnInitDlg(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CenterWindow();

	SetDlgItemText(IDC_NAME, m_strName);
	SetDlgItemText(IDC_TEL, m_strTel);
	SetDlgItemText(IDC_CONTENT, m_strContent);

	return 0;
}
LRESULT CSmsReadMsgDlg::OnReply(WORD, WORD, HWND, BOOL&)
{

	Close();	
	::PostMessage(m_hParent, WM_COMMAND, ID_SMS_REPLY, 0);
	return 0;
}
LRESULT CSmsReadMsgDlg::OnTransfer(WORD, WORD, HWND, BOOL&)
{
	Close();
	::PostMessage(m_hParent, WM_COMMAND, ID_SMS_TRANSFER, 0);
	return 0;
}
LRESULT CSmsReadMsgDlg::OnSave(WORD, WORD, HWND, BOOL&)
{
	Close();
	::PostMessage(m_hParent, WM_COMMAND, ID_SMS_SAVE, 0);
	return 0;
}