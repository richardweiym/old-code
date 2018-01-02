#include "StdAfx.h"
#include "SmsSearch.h"
#include "SmsSendMessage.h"
#include "SmsMessageLib.h"
#include <atlctrlx.h>

CSmsSearchDlg::CSmsSearchDlg()
{
	m_pRst = NULL;
	m_eType = ALL;
}
VOID CSmsSearchDlg::Close()
{
	EndDialog(IDC_CLOSE);
	CloseRst();
}
LRESULT CSmsSearchDlg::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
	Close();
	return 0;
}
LRESULT CSmsSearchDlg::OnExit(WORD ,WORD, HWND, BOOL&)
{
	Close();
	return 0;
}
LRESULT CSmsSearchDlg::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	CenterWindow();

	CListViewCtrl list =GetDlgItem(IDC_MESSAGE);
	DWORD dwExStyle = list.GetExtendedListViewStyle();
	dwExStyle |=  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES;
	list.SetExtendedListViewStyle(dwExStyle);
	
	LVCOLUMN lvc = { 0 };
	lvc.mask =  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	//lvc.fmt = LVCFMT_CENTER;

	lvc.pszText = _T("收信人/发信人");
	lvc.cx = 100;
	list.InsertColumn(0,&lvc);

	lvc.pszText = _T("手机号码");
	lvc.cx = 120;
	list.InsertColumn(1,&lvc);

	lvc.pszText = _T("内容");
	lvc.cx = 600;
	list.InsertColumn(2,&lvc);

	CComboBox cb = GetDlgItem(IDC_BOX);
	cb.AddString(_T("所有信箱"));
	cb.AddString(_T("待发短信"));
	cb.AddString(_T("已发短信"));
	cb.AddString(_T("已收短信"));
	cb.AddString(_T("已删短信"));

	CEdit edit = GetDlgItem(IDC_NAME);
	edit.SetLimitText(100);
	edit = GetDlgItem(IDC_TEL);
	edit.SetLimitText(50);
	edit = GetDlgItem(IDC_CONTENT);
	edit.SetLimitText(100);

	Clear();

	return 0;
}
VOID CSmsSearchDlg::CloseRst()
{
	if(m_pRst != NULL)
	{
		try
		{
			if(m_pRst->State == adStateOpen)
			{
				m_pRst->Close();
				m_pRst = NULL;
			}
		}
		catch (...)
		{
			NULL;
		}
	}
}
LRESULT CSmsSearchDlg::OnClear(WORD, WORD, HWND, BOOL&)
{
	Clear();
	return 0;
}
LRESULT CSmsSearchDlg::OnSearch(WORD, WORD, HWND, BOOL&)
{
	CWaitCursor wait;

	CListViewCtrl list = GetDlgItem(IDC_MESSAGE);
	list.DeleteAllItems();

	INT nIndex(0);
	CString strSQL;
	if(GetCurrentSQL(strSQL))
	{
		CDBOperate* pDB = CSmsDB::GetDBOperate();
		if(pDB != NULL)
		{
			if(pDB->GetRecordset(strSQL, &m_pRst))
			{
				try
				{
					_variant_t vt;
					while(!m_pRst->GetADOEOF())
					{
						vt = m_pRst->GetCollect(_T("name"));
						if(vt.vt != VT_NULL)
						{
							list.AddItem(nIndex, 0, (LPCTSTR)_bstr_t(vt));
						}
						vt = m_pRst->GetCollect(_T("tel"));
						if(vt.vt != VT_NULL)
						{
							list.AddItem(nIndex, 1, (LPCTSTR)_bstr_t(vt));
						}
						vt = m_pRst->GetCollect(_T("content"));
						if(vt.vt != VT_NULL)
						{
							list.AddItem(nIndex, 2, (LPCTSTR)_bstr_t(vt));
						}
						m_pRst->MoveNext();
						nIndex++;
					}
				}
				catch (...)
				{
					CloseRst();
				}
				CloseRst();
			}
		}
	}
	
	CString strInfo;
	strInfo.Format(_T("共查询到%d条符合条件的短信"), list.GetItemCount());
	SetDlgItemText(IDC_INFO, strInfo);

	return 0;
}
VOID CSmsSearchDlg::Clear()
{	
	CComboBox cb = GetDlgItem(IDC_BOX);
	cb.SetCurSel((INT)ALL);
	CButton btn = GetDlgItem(IDC_CHECK_NAME);
	btn.SetCheck(BST_UNCHECKED);
	btn = GetDlgItem(IDC_CHECK_TEL);
	btn.SetCheck(BST_UNCHECKED);
	btn = GetDlgItem(IDC_CHECK_CONTENT);
	btn.SetCheck(BST_UNCHECKED);
	
	CListViewCtrl list = GetDlgItem(IDC_MESSAGE);
	list.DeleteAllItems();

	SetDlgItemText(IDC_NAME, _T(""));
	SetDlgItemText(IDC_TEL, _T(""));
	SetDlgItemText(IDC_CONTENT, _T(""));
	SetDlgItemText(IDC_INFO, _T(""));
}

BOOL CSmsSearchDlg::GetCurrentSQL(CString& strSQL)
{
	CString strName, strTel, strContent;
	CButton btn = GetDlgItem(IDC_CHECK_NAME);
	CEdit edit = GetDlgItem(IDC_NAME);
	if(btn.GetState() & BST_CHECKED)
	{
		edit.GetWindowText(strName.GetBuffer(MAX_PATH), MAX_PATH);
		strName.ReleaseBuffer();
	}
	
	btn = GetDlgItem(IDC_CHECK_TEL);
	edit = GetDlgItem(IDC_TEL);
	if(btn.GetState() & BST_CHECKED)
	{
		edit.GetWindowText(strTel.GetBuffer(MAX_PATH), MAX_PATH);
		strTel.ReleaseBuffer();
	}

	btn = GetDlgItem(IDC_CHECK_CONTENT);
	edit = GetDlgItem(IDC_CONTENT);
	if(btn.GetState() & BST_CHECKED)
	{
		edit.GetWindowText(strContent.GetBuffer(MAX_PATH), MAX_PATH);
		strContent.ReleaseBuffer();
	}
	CComboBox cb = GetDlgItem(IDC_BOX);
	m_eType = (ENUM_SEARCH_TYPE)cb.GetCurSel();

	TCHAR szAr[5][30] = {"", "toSendMessage", "sendedMessage", "receiveMessage", "deletedMessage"};
	
	
	if(m_eType != ALL)
	{
		CString str;
		str.Format(_T("select * from %s where name like '%%%s%%' and tel like '%%%s%%' and content like '%%%s%%'"),	\
			szAr[m_eType], strName, strTel, strContent);
		strSQL = str;
	}
	else
	{
		CString str1, str2, str3, str4;
		str1.Format(_T("select [name] as name,[tel] as tel,[content] as content from %s where name like '%%%s%%' and tel like '%%%s%%' and content like '%%%s%%'"),	\
			szAr[1], strName, strTel, strContent);
		str2.Format(_T("select [name] as name,[tel] as tel,[content] as content from %s where name like '%%%s%%' and tel like '%%%s%%' and content like '%%%s%%'"),	\
			szAr[2], strName, strTel, strContent);
		str3.Format(_T("select [name] as name,[tel] as tel,[content] as content from %s where name like '%%%s%%' and tel like '%%%s%%' and content like '%%%s%%'"),	\
			szAr[3], strName, strTel, strContent);
		str4.Format(_T("select [name] as name,[tel] as tel,[content] as content from %s where name like '%%%s%%' and tel like '%%%s%%' and content like '%%%s%%'"),	\
			szAr[4], strName, strTel, strContent);

		strSQL.Format(_T("%s union all %s union all %s union all %s"), str1, str2, str3, str4);
	}
	

	return TRUE;
}
BOOL CSmsSearchDlg::GetCurrentSelInfo(CString& strName, CString& strTel, CString& strContent)
{
	CListViewCtrl list = GetDlgItem(IDC_MESSAGE);
	INT nIndex = list.GetSelectedIndex();
	if(nIndex >= 0)
	{
		list.GetItemText(nIndex, 0, strName);
		list.GetItemText(nIndex, 1, strTel);
		list.GetItemText(nIndex, 2, strContent);
		return TRUE;
	}

	return FALSE;
}
LRESULT CSmsSearchDlg::OnListviewNotify(INT, LPNMHDR pnmh , BOOL&)
{
	if(pnmh->code == NM_DBLCLK)
	{
		CString strName, strTel, strContent;
		if(GetCurrentSelInfo(strName, strTel, strContent))
		{
			CSmsReadMsgDlg dlg(strName, strTel, strContent, m_hWnd);
			dlg.DoModal();
		}
	}

	if(pnmh->code == NM_RCLICK)
	{
		HMENU hParent = CreatePopupMenu();
		if(hParent != NULL)
		{
			CString strName, strTel, strContent;
			UINT nFlag(MF_STRING);
			if(!GetCurrentSelInfo(strName, strTel, strContent))
			{
				nFlag |= MF_GRAYED;
			}

			AppendMenu(hParent, nFlag, ID_SMS_REPLY, _T("回复"));
			AppendMenu(hParent, nFlag, ID_SMS_TRANSFER, _T("转发"));
			AppendMenu(hParent, MF_SEPARATOR, NULL, NULL);
			AppendMenu(hParent, nFlag, ID_SMS_SAVE, _T("收藏"));

			POINT pt;
			GetCursorPos(&pt);
			TrackPopupMenu(hParent, TPM_LEFTALIGN|TPM_TOPALIGN,pt.x, pt.y, 0, *this, NULL);
			
			DestroyMenu(hParent);
		}
	}

	return 0;
}

LRESULT CSmsSearchDlg::OnSmsReply(WORD, WORD, HWND, BOOL&)
{
	CString strName, strTel, strContent;
	if(GetCurrentSelInfo(strName, strTel, strContent))
	{
		CSmsSendDlg dlg(ID_SMS_REPLY);
		dlg.m_strName = strName;
		dlg.m_strTel = strTel;
		dlg.m_strContent = strContent;

		dlg.DoModal();
	}

	return 0;
}
LRESULT CSmsSearchDlg::OnSmsTransfer(WORD, WORD, HWND, BOOL&)
{
	CString strName, strTel, strContent;
	if(GetCurrentSelInfo(strName, strTel, strContent))
	{
		CSmsSendDlg dlg(ID_SMS_TRANSFER);
		dlg.m_strName = strName;
		dlg.m_strTel = strTel;
		dlg.m_strContent = strContent;

		dlg.DoModal();
	}
	return 0;
}
LRESULT CSmsSearchDlg::OnSmsSave(WORD, WORD, HWND, BOOL&)
{
	CString strName, strTel, strContent;
	if(GetCurrentSelInfo(strName, strTel, strContent))
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
	return 0;
}