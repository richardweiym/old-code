#include "StdAfx.h"
#include "messageManager.h"

CToSendMessage::CToSendMessage()
:m_strTel(_T("")), m_strContent(_T("")), m_strToSendTime(_T("")), m_strCreateTime(_T("")), m_strName(_T(""))
{
	 m_nToSendId = 0;
	 m_bSendPromptly = TRUE;
	 m_nUserId = 0;	
}

BOOL CToSendMessage::GetRecordset(_Recordset** pRst, BOOL bDeleted /* = FALSE */)
{
	BOOL bOK(FALSE);
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		LPCTSTR pszSQL = _T("select * from toSendMessage order by toSendId asc");
		bOK = pDB->GetRecordset(pszSQL, pRst);		
	}
	return bOK;
}
VOID CToSendMessage::ResetSendingMsg(CListViewCtrl& list, INT nId)
{
	INT nCount = list.GetItemCount();
	
	TCHAR szBuf[20] = {0};
	INT n(0);
	for(int i=0; i<nCount; i++)
	{
		list.GetItemText(i, 6, szBuf, 20);
		n = _ttoi(szBuf);
		if(n == nId)
		{
			LVITEM lvItem;
			lvItem.iItem = i;
			lvItem.iSubItem = 0;
			lvItem.mask = LVIF_IMAGE;
			list.GetItem(&lvItem);
			lvItem.iImage = 0;
			list.SetItem(&lvItem);
		}
	}
}
BOOL CToSendMessage::SetSendedMsg(CListViewCtrl& list, CToSendMessage& msg, BOOL bRet)
{
	INT nCount = list.GetItemCount();
	INT nId = msg.m_nToSendId;
	TCHAR szBuf[20] = {0};
	BOOL bOK(FALSE);
	for(int i=0; i<nCount; i++)
	{
		list.GetItemText(i, 6, szBuf, 20);
		if(_ttoi(szBuf) == nId)
		{
			list.DeleteItem(i);
			RemoveMessage(nId);
			bOK = TRUE;
			break;
		}
	}

	return bOK;
}
INT CompareTime(SYSTEMTIME& st, INT a[6])
{
	if(st.wYear > a[0])								return 1;
	else if( st.wYear < a[0])						return -1;
	else
	{
		if(st.wMonth > a[1])						return 1;
		else if(st.wMonth < a[1])					return -1;
		else
		{
			if(st.wDay > a[2])						return 1;
			else if(st.wDay < a[2])					return -1;
			else
			{
				if(st.wHour > a[3])					return 1;
				else if(st.wHour < a[3])			return -1;
				else
				{
					if(st.wMinute > a[4])			return 1;
					else if(st.wMinute < a[4])		return -1;
					else
					{
						if(st.wSecond > a[5])		return 1;
						else if(st.wSecond < a[5])	return -1;
						else						return 0;
					}	
				}
			}
		}
	}
}
BOOL CToSendMessage::Get1ToSendMsg(CListViewCtrl& list, CToSendMessage& msg)
{
	TCHAR szBuf[1025] = {0};
	INT nCount = list.GetItemCount();
	BOOL bSendPromptly(FALSE); 


	BOOL bOK(FALSE);
	INT nIndex(0);
	INT arN[6] = {0};
	TCHAR szM[100] ={0};
	for(int i=0; i<nCount; i++)
	{
		list.GetItemText(i, 7, szBuf, 1025);
		bSendPromptly = (BOOL)(_ttoi(szBuf));
		if(!bSendPromptly)
		{
			SYSTEMTIME st = {0};
			GetLocalTime(&st);

			szM[0] = 0;
			list.GetItemText(i, 5, szBuf, 1025);					
			_stscanf(szBuf, _T("%d-%d-%d %d:%d:%d"), 
				&arN[0], &arN[1], &arN[2],  &arN[3], &arN[4], &arN[5]);
			
			if(CompareTime(st, arN) < 0) 
			{
				continue;
			}
		}

		nIndex = 1;
		list.GetItemText(i, nIndex++, szBuf, 1025);
		msg.m_strName = szBuf;
		list.GetItemText(i, nIndex++, szBuf, 1025);
		msg.m_strTel = szBuf;
		list.GetItemText(i, nIndex++, szBuf, 1025);
		msg.m_strContent = szBuf;
		list.GetItemText(i, nIndex++, szBuf, 1025);
		msg.m_strCreateTime = szBuf;
		list.GetItemText(i, nIndex++, szBuf, 1025);
		msg.m_strToSendTime = szBuf;
		list.GetItemText(i, nIndex++, szBuf, 1025);
		msg.m_nToSendId = _ttoi(szBuf);
		nIndex++;
		msg.m_bSendPromptly = bSendPromptly;
		list.GetItemText(i, nIndex++, szBuf, 1025);
		msg.m_nUserId = _ttoi(szBuf);			

		LVITEM lvItem;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.mask = LVIF_IMAGE;
		list.GetItem(&lvItem);
		lvItem.iImage = 5;
		list.SetItem(&lvItem);

		bOK	 = TRUE;
		break;
	}

	return bOK;
}

VOID CToSendMessage::GetListItem(CListViewCtrl& list, INT nItem, CString& strContactCaption , CString& strContactName, CString& strTimeCaption, CString& strTimeValue, CString& strContent)
{
	strContactCaption = _T("收信人:");
	list.GetItemText(nItem, 1, strContactName);
	strTimeCaption = _T("创建时间:");
	list.GetItemText(nItem, 4, strTimeValue );
	list.GetItemText(nItem, 3, strContent );
}
VOID CToSendMessage::GetItemByIndex(CListViewCtrl& list, INT nIndex, CToSendMessage& msg)
{
	TCHAR szbBuf[20] = {0};
	INT nSub(1);
	list.GetItemText(nIndex, nSub++, msg.m_strName);
	list.GetItemText(nIndex, nSub++, msg.m_strTel);
	list.GetItemText(nIndex, nSub++, msg.m_strContent);
	list.GetItemText(nIndex, nSub++, msg.m_strCreateTime);
	list.GetItemText(nIndex, nSub++, msg.m_strToSendTime);
	list.GetItemText(nIndex, nSub++, szbBuf, 20);
	msg.m_nToSendId = _ttoi(szbBuf);
	list.GetItemText(nIndex, nSub++, szbBuf, 20);
	msg.m_bSendPromptly = (BOOL)_ttoi(szbBuf);
	list.GetItemText(nIndex, nSub++, szbBuf, 20);
	msg.m_nUserId = _ttoi(szbBuf);
}
BOOL CToSendMessage::DeleteSelMsg(CListViewCtrl& list, CSimpleArray<CDeletedMessage*>& arDelete, BOOL bDelAll)
{
	INT nCount = list.GetItemCount();
	INT nSel(0);
	BOOL bSel(FALSE);
	CToSendMessage msg;
	CDeletedMessage* pDelMsg = NULL;

	for(int i=nCount-1; i>=0; i--)
	{
		if(!bDelAll)
		{
			bSel = list.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED;
		}
		else
		{
			bSel = TRUE;
		}
		
		if(bSel)
		{
			GetItemByIndex(list, i, msg);
			list.DeleteItem(i);
			RemoveMessage(msg.m_nToSendId);
			pDelMsg = new CDeletedMessage(msg);
			arDelete.Add(pDelMsg);
			nSel++;
		}
	}

	if(nSel > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
VOID CToSendMessage::CreateListHeader(CListViewCtrl& list)
{
		LVCOLUMN lvc = { 0 };
		lvc.mask =  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.fmt = LVCFMT_LEFT;

		int nIndex(0);

		lvc.pszText = (LPTSTR)_T("");
		lvc.cx =list.GetStringWidth(_T("")) + 50;
		list.InsertColumn(nIndex++,&lvc);
		
		lvc.pszText = _T("收信人");
		lvc.cx = 100;
		list.InsertColumn(nIndex++,&lvc);
		
		lvc.pszText = _T("手机号码");
		lvc.cx = 100;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("内容");
		lvc.cx = 400;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("创建时间");
		lvc.cx = 150;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("定时发送时间");
		lvc.cx = 150;
		list.InsertColumn(nIndex++,&lvc);	
		
		lvc.pszText = _T("toSendId");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);
		lvc.pszText = _T("isSendPromptly");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);
		lvc.pszText = _T("userId");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);
		
}
VOID CToSendMessage::InsertItem(CListViewCtrl& list,CToSendMessage* pMsg, BOOL bTop)
{
	INT nCount = list.GetItemCount();
	if(bTop)
	{
		nCount = 0;
	}
	INT nIndex(0);
	
	list.AddItem(nCount, nIndex++, _T(""), 0);
	list.AddItem(nCount, nIndex++, pMsg->m_strName);
	list.AddItem(nCount, nIndex++, pMsg->m_strTel);
	list.AddItem(nCount, nIndex++, pMsg->m_strContent);
	list.AddItem(nCount, nIndex++, pMsg->m_strCreateTime);
	if(!pMsg->m_bSendPromptly)
	{
		list.AddItem(nCount, nIndex++, pMsg->m_strToSendTime);
	}
	else
	{
		nIndex++;
	}
	TCHAR szBuf[10];
	_itot(pMsg->m_nToSendId, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_bSendPromptly, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_nUserId, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
}

BOOL CToSendMessage::GetSmsMessage(_RecordsetPtr pRst, CToSendMessage* pMsg)
{
	_variant_t vt;
	BOOL bOK(FALSE);
	try
	{
		vt = pRst->GetCollect(_T("toSendId"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_nToSendId = (long)vt;
		}
		vt = pRst->GetCollect(_T("tel"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strTel = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("content"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strContent = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("isSendPromptly"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_bSendPromptly = (long)vt;
		}
		vt = pRst->GetCollect(_T("toSendTime"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strToSendTime = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("createTime"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strCreateTime = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("userId"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_nUserId = (long)vt;
		}
		vt = pRst->GetCollect(_T("name"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strName = (LPCTSTR)_bstr_t(vt);
		}
		bOK	 = TRUE;
	}
	catch (...)
	{
		NULL;	
	}

	return bOK;
}
INT CToSendMessage::AddMessage(CToSendMessage& msg)
{
	LPCTSTR pszSQL = _T("select top  1  *  from toSendMessage");
	INT bOK(-1);
	
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	_RecordsetPtr pRst;
	if(pDB != NULL)
	{
		BOOL b = pDB->GetRecordset(pszSQL, &pRst);
		if(b)
		{
			try
			{
				pRst->AddNew();
				pRst->PutCollect(_T("tel"), (_variant_t)LPCTSTR(msg.m_strTel));
				pRst->PutCollect(_T("content"), (_variant_t)LPCTSTR(msg.m_strContent));
				pRst->PutCollect(_T("isSendPromptly"), (_variant_t)long(msg.m_bSendPromptly));
				if(msg.m_strToSendTime != _T(""))
				{
					pRst->PutCollect(_T("toSendTime"), (_variant_t)LPCTSTR(msg.m_strToSendTime));
				}
				pRst->PutCollect(_T("createTime"), (_variant_t)LPCTSTR(msg.m_strCreateTime));
				pRst->PutCollect(_T("userId"), (_variant_t)long(msg.m_nUserId));
				pRst->PutCollect(_T("name"), (_variant_t)LPCTSTR(msg.m_strName));
				pRst->Update();
				_variant_t vt;
				vt = pRst->GetCollect(_T("toSendId"));
				if(vt.vt != VT_NULL)
				{
					bOK = (long)vt;
					msg.m_nToSendId = bOK;
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
BOOL CToSendMessage::RemoveMessage(INT nId)
{
	CString strSQL;
	BOOL bOK(FALSE);
	
	strSQL.Format(_T("delete from toSendMessage where toSendId = %d"), nId);
	
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		bOK = pDB->ExecuteSQL(strSQL);
	}
	
	return bOK;
}


CSendedMessage::CSendedMessage()
:m_strTel(_T("")), m_strContent(_T("")), m_strToSendTime(_T("")),
m_strSendedTime(_T("")), m_strCreateTime(_T("")), m_strName(_T(""))
{
	 m_nSendedId = 0;
	 m_nToSendId = 0;
	 m_bSendPromptly = TRUE;
	 m_nUserId = 0;
	 m_bSendedOK = TRUE;
	 m_bDeleted = FALSE;
}

CSendedMessage::CSendedMessage(const CSendedMessage& msg)
:m_strTel(msg.m_strTel), m_strContent(msg.m_strContent), m_strToSendTime(msg.m_strToSendTime),
m_strSendedTime(msg.m_strToSendTime), m_strCreateTime(msg.m_strCreateTime), m_strName(msg.m_strName)
{
	m_nSendedId = msg.m_nSendedId;
	m_nToSendId = msg.m_nToSendId;
	m_bSendPromptly = msg.m_bSendPromptly;
	m_nUserId = msg.m_nUserId;
	m_bSendedOK = msg.m_bSendedOK;
	m_bDeleted = msg.m_bDeleted;
}

CSendedMessage::CSendedMessage(const CToSendMessage& msg, BOOL bOK)
:m_strTel(msg.m_strTel), m_strContent(msg.m_strContent), m_strToSendTime(msg.m_strToSendTime),
m_strCreateTime(msg.m_strCreateTime), m_strName(msg.m_strName)
{
	m_nSendedId = 0;
	m_nToSendId = msg.m_nToSendId;
	m_bSendPromptly = msg.m_bSendPromptly;
	m_nUserId = msg.m_nUserId;
	m_bSendedOK = bOK;
	m_bDeleted = FALSE;
	
	SYSTEMTIME st;
	GetLocalTime(&st);
	m_strSendedTime.Format(_T("%d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

BOOL CSendedMessage::GetRecordset(_Recordset** pRst, BOOL bDeleted /* = FALSE */)
{
	LPCTSTR lpszSQL = _T("select * from sendedMessage  order by sendedId desc");


	BOOL bOK(FALSE);
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		bOK = pDB->GetRecordset(lpszSQL, pRst);		
	}

	return bOK;
}
VOID CSendedMessage::GetListItem(CListViewCtrl& list, INT nItem, CString& strContactCaption , CString& strContactName, CString& strTimeCaption, CString& strTimeValue, CString& strContent)
{
	strContactCaption = _T("收信人:");
	list.GetItemText(nItem, 1, strContactName);
	strTimeCaption = _T("发送时间:");
	list.GetItemText(nItem, 6, strTimeValue );
	list.GetItemText(nItem, 3, strContent );
}
BOOL CSendedMessage::DeleteSelMsg(CListViewCtrl& list, CSimpleArray<CDeletedMessage*>& arDelete, BOOL bDelAll)
{
	INT nCount = list.GetItemCount();
	INT nSel(0);
	BOOL bSel(FALSE);
	CSendedMessage msg;
	CDeletedMessage* pDelMsg = NULL;

	for(int i=nCount-1; i>=0; i--)
	{
		if(!bDelAll)
		{
			bSel = list.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED;
		}
		else
		{
			bSel = TRUE;
		}
		if(bSel)
		{
			GetItemByIndex(list, i, msg);
			list.DeleteItem(i);
			RemoveMessage(msg.m_nSendedId);
			pDelMsg = new CDeletedMessage(msg);
			arDelete.Add(pDelMsg);
			nSel++;
		}
	}

	if(nSel > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
VOID CSendedMessage::GetItemByIndex(CListViewCtrl& list, INT nIndex, CSendedMessage& msg)
{
	INT nSub(1);
	list.GetItemText(nIndex, nSub++, msg.m_strName);
	list.GetItemText(nIndex, nSub++, msg.m_strTel);
	list.GetItemText(nIndex, nSub++, msg.m_strContent);
	TCHAR szBuf[20] = {0};
	nSub++;
	list.GetItemText(nIndex, nSub++, msg.m_strCreateTime);
	list.GetItemText(nIndex, nSub++, msg.m_strSendedTime);
	list.GetItemText(nIndex, nSub++, msg.m_strToSendTime);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_nToSendId = _ttoi(szBuf);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_nSendedId = _ttoi(szBuf);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_nUserId = _ttoi(szBuf);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_bDeleted = (BOOL)_ttoi(szBuf);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_bSendPromptly = (BOOL)_ttoi(szBuf);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_bSendedOK = (BOOL)_ttoi(szBuf);
}
VOID CSendedMessage::CreateListHeader(CListViewCtrl& list)
{
		LVCOLUMN lvc = { 0 };
		lvc.mask =  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.fmt = LVCFMT_LEFT;
		int nIndex(0);

		lvc.pszText = (LPTSTR)_T("");
		lvc.cx =list.GetStringWidth(_T("")) + 50;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("收信人");
		lvc.cx = 100;
		list.InsertColumn(nIndex++,&lvc);	

		lvc.pszText = _T("手机号码");
		lvc.cx = 100;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("内容");
		lvc.cx = 400;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("状态");
		lvc.cx = 100;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("创建时间");
		lvc.cx = 150;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("发送时间");
		lvc.cx = 150;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("toSendTime");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("toSendId");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("sendedId");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("userId");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("isDeleted");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("isSendPromptly");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("isSendOK");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);
}
BOOL CSendedMessage::SetSendedMsg(CListViewCtrl& list, CToSendMessage& msg, BOOL bSendedOK)
{
	CSendedMessage sendedMsg(msg, bSendedOK);
	
	AddMessage(sendedMsg);
	InsertItem(list, &sendedMsg, TRUE);
	return TRUE;
}
VOID CSendedMessage::InsertItem(CListViewCtrl& list,CSendedMessage* pMsg, BOOL bTop)
{
	INT nCount = list.GetItemCount();
	if(bTop)
	{
		nCount = 0;
	}

	INT nIndex(0);

	list.AddItem(nCount, nIndex++, _T(""), 1);
	list.AddItem(nCount, nIndex++, pMsg->m_strName);
	list.AddItem(nCount, nIndex++, pMsg->m_strTel);
	list.AddItem(nCount, nIndex++, pMsg->m_strContent);
	if(pMsg->m_bSendedOK)
	{
		list.AddItem(nCount, nIndex++, _T("发送成功"));
	}
	else
	{
		list.AddItem(nCount, nIndex++, _T("发送失败"));
	}
	list.AddItem(nCount, nIndex++, pMsg->m_strCreateTime);
	list.AddItem(nCount, nIndex++, pMsg->m_strSendedTime);

	if(!pMsg->m_bSendPromptly)
	{
		list.AddItem(nCount, nIndex++, pMsg->m_strToSendTime);
	}
	else
	{
		nIndex++;
	}
	
	TCHAR szBuf[10] = {0};
	_itot(pMsg->m_nToSendId, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_nSendedId, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_nUserId, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_bDeleted, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_bSendPromptly, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_bSendedOK, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
}
BOOL CSendedMessage::GetSmsMessage(_RecordsetPtr pRst, CSendedMessage* pMsg)
{
	_variant_t vt;
	BOOL bOK(FALSE);
	
	try
	{
		vt = pRst->GetCollect(_T("sendedId"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_nSendedId = (long)vt;
		}
		vt = pRst->GetCollect(_T("toSendId"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_nToSendId = (long)vt;
		}
		vt = pRst->GetCollect(_T("tel"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strTel = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("content"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strContent = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("isSendPromptly"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_bSendPromptly = (long)vt;
		}
		vt = pRst->GetCollect(_T("toSendTime"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strToSendTime = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("createTime"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strCreateTime = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("sendedTime"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strSendedTime = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("userId"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_nUserId = (long)vt;
		}
		vt = pRst->GetCollect(_T("isSendOK"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_bSendedOK = (long)vt;
		}
		vt = pRst->GetCollect(_T("name"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strName = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("isDeleted"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_bDeleted = (long)vt;
		}
		bOK = TRUE;
	}
	catch (...)
	{
		NULL;
	}

	return bOK;
}
INT CSendedMessage::AddMessage(CSendedMessage& msg)
{
	LPCTSTR pszSQL = _T("select top  1  *  from sendedMessage");
	BOOL bOK(-1);
	
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	_RecordsetPtr pRst;
	if(pDB != NULL)
	{
		BOOL b = pDB->GetRecordset(pszSQL, &pRst);
		if(b)
		{
			try
			{
				pRst->AddNew();
				
				pRst->PutCollect(_T("toSendId"),(_variant_t)long(msg.m_nToSendId));
				pRst->PutCollect(_T("tel"), (_variant_t)LPCTSTR(msg.m_strTel));
				pRst->PutCollect(_T("content"), (_variant_t)LPCTSTR(msg.m_strContent));
				pRst->PutCollect(_T("isSendPromptly"), (_variant_t)long(msg.m_bSendPromptly));
				if(!msg.m_bSendPromptly)
				{
					pRst->PutCollect(_T("toSendTime"), (_variant_t)LPCTSTR(msg.m_strToSendTime));
				}	
				pRst->PutCollect(_T("createTime"), (_variant_t)LPCTSTR(msg.m_strCreateTime));
				pRst->PutCollect(_T("userId"), (_variant_t)long(msg.m_nUserId));
				pRst->PutCollect(_T("name"), (_variant_t)LPCTSTR(msg.m_strName));
				pRst->PutCollect(_T("sendedTime"), (_variant_t)LPCTSTR(msg.m_strSendedTime));
				pRst->PutCollect(_T("isDeleted"), (_variant_t)long(msg.m_bDeleted));
				pRst->PutCollect(_T("isSendOK"), (_variant_t)long(msg.m_bSendedOK));
				pRst->Update();
				msg.m_nSendedId = (long)(pRst->GetCollect(_T("sendedId")));
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

BOOL CSendedMessage::RemoveMessage(INT nId)
{
	CString strSQL;
	BOOL bOK(FALSE);
	
	strSQL.Format(_T("delete from sendedMessage where sendedId = %d"), nId);
	
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		bOK = pDB->ExecuteSQL(strSQL);
	}
	
	return bOK;
}



CReceiveMessage::CReceiveMessage()
:m_strTel(_T("")), m_strContent(_T("")), m_strReceiveTime(_T("")), m_strName(_T(""))
{
	 m_nReceiveId = 0;
	 m_bReaded = FALSE;
	 m_nUserId = 0;
	 m_bDeleted = FALSE;
}
BOOL CReceiveMessage::GetRecordset(_Recordset** pRst, BOOL bDeleted /* = FALSE */)
{
	LPCTSTR lpszSQL = _T("select * from receiveMessage  order by receiveId desc");


	BOOL bOK(FALSE);
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		bOK = pDB->GetRecordset(lpszSQL, pRst);		
	}

	return bOK;
}
BOOL CReceiveMessage::DeleteSelMsg(CListViewCtrl& list, CSimpleArray<CDeletedMessage*>& arDelete, BOOL bDelAll)
{
	INT nCount = list.GetItemCount();
	INT nSel(0);
	BOOL bSel(FALSE);
	CReceiveMessage msg;
	CDeletedMessage* pDelMsg = NULL;

	for(int i=nCount-1; i>=0; i--)
	{
		if(!bDelAll)
		{
			bSel = list.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED;
		}
		else
		{
			bSel = TRUE;
		}
		if(bSel)
		{
			GetItemByIndex(list, i, msg);
			list.DeleteItem(i);
			RemoveMessage(msg.m_nReceiveId);
			pDelMsg = new CDeletedMessage(msg);
			arDelete.Add(pDelMsg);
			nSel++;
		}
	}

	if(nSel > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CReceiveMessage::GetSmsMessage(_RecordsetPtr pRst, CReceiveMessage* pMsg)
{
	_variant_t vt;
	BOOL bOK(FALSE);
	
	try
	{
		vt = pRst->GetCollect(_T("receiveId"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_nReceiveId = (long)vt;
		}
		vt = pRst->GetCollect(_T("tel"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strTel = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("content"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strContent = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("isReaded"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_bReaded = (long)vt;
		}
		vt = pRst->GetCollect(_T("receiveTime"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strReceiveTime = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("userId"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_nUserId = (long)vt;
		}
		vt = pRst->GetCollect(_T("name"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strName = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("isDeleted"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_bDeleted = (long)vt;
		}
		bOK = TRUE;
	}
	catch (...)
	{
		NULL;
	}

	return bOK;
}
INT CReceiveMessage::AddMessage(CReceiveMessage& msg)
{
	LPCTSTR pszSQL = _T("select top  1  *  from receiveMessage");
	BOOL bOK(-1);
	
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	_RecordsetPtr pRst;
	if(pDB != NULL)
	{
		BOOL b = pDB->GetRecordset(pszSQL, &pRst);
		if(b)
		{
			try
			{
				pRst->AddNew();

				pRst->PutCollect(_T("tel"), (_variant_t)LPCTSTR(msg.m_strTel));
				pRst->PutCollect(_T("content"), (_variant_t)LPCTSTR(msg.m_strContent));
				pRst->PutCollect(_T("isReaded"), (_variant_t)long(msg.m_bReaded));
				pRst->PutCollect(_T("receiveTime"), (_variant_t)LPCTSTR(msg.m_strReceiveTime));
				pRst->PutCollect(_T("userId"), (_variant_t)long(msg.m_nUserId));
				pRst->PutCollect(_T("name"), (_variant_t)LPCTSTR(msg.m_strName));
				pRst->PutCollect(_T("isDeleted"), (_variant_t)long(msg.m_bDeleted));
				pRst->Update();
				msg.m_nReceiveId = (long)(pRst->GetCollect(_T("receiveId")));
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

BOOL CReceiveMessage::RemoveMessage(INT nId)
{
	CString strSQL;
	BOOL bOK(FALSE);
	
	strSQL.Format(_T("delete from receiveMessage where receiveId = %d"), nId);
	
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		bOK = pDB->ExecuteSQL(strSQL);
	}
	
	return bOK;
}
BOOL CReceiveMessage::SetReceivedMsg(CListViewCtrl& list, CReceiveMessage& msg)
{
	AddMessage(msg);
	InsertItem(list, &msg, TRUE);
	
	return TRUE;
}
BOOL CReceiveMessage::SetReadedMsg(CListViewCtrl& list, INT nIndex)
{
	list.SetItemText(nIndex, 6, _T("1"));
	LVITEM lvItem;
	lvItem.iItem = nIndex;
	lvItem.iSubItem = 0;
	lvItem.mask = LVIF_IMAGE;
	list.GetItem(&lvItem);
	lvItem.iImage = 2;
	list.SetItem(&lvItem);
	
	TCHAR szBuf[20] = {0};
	list.GetItemText(nIndex, 8, szBuf, 20);
	CString strSQL;
	strSQL.Format(_T("update receiveMessage set isReaded = TRUE where receiveId = %s"), szBuf);
	
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		pDB->ExecuteSQL(strSQL);
	}
	return TRUE;
}
VOID CReceiveMessage::GetListItem(CListViewCtrl& list, INT nItem, CString& strContactCaption , CString& strContactName, CString& strTimeCaption, CString& strTimeValue, CString& strContent)
{
	strContactCaption = _T("发信人:");
	list.GetItemText(nItem, 1, strContactName);
	strTimeCaption = _T("接收时间:");
	list.GetItemText(nItem, 4, strTimeValue );
	list.GetItemText(nItem, 3, strContent );

	//set readed flag
	TCHAR szBuf[20] = {0};
	list.GetItemText(nItem, 6, szBuf, 20);
	BOOL bReaded = (BOOL)(_ttoi(szBuf));
	if(!bReaded)
	{
		SetReadedMsg(list, nItem);
	}
	
}
VOID CReceiveMessage::GetItemByIndex(CListViewCtrl& list, INT nIndex, CReceiveMessage& msg)
{
	INT nSub(1);
	TCHAR szBuf[20] = {0};

	list.GetItemText(nIndex, nSub++, msg.m_strName);
	list.GetItemText(nIndex, nSub++, msg.m_strTel);
	list.GetItemText(nIndex, nSub++, msg.m_strContent);
	list.GetItemText(nIndex, nSub++, msg.m_strReceiveTime);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_nUserId = _ttoi(szBuf);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_bReaded = (BOOL)_ttoi(szBuf);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_bDeleted = (BOOL)_ttoi(szBuf);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_nReceiveId = _ttoi(szBuf);
}
VOID CReceiveMessage::CreateListHeader(CListViewCtrl& list)
{
		LVCOLUMN lvc = { 0 };
		lvc.mask =  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.fmt = LVCFMT_LEFT;
		int nIndex(0);

		lvc.pszText = (LPTSTR)_T("");
		lvc.cx =list.GetStringWidth(_T("")) + 50;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("发信人");
		lvc.cx = 100;
		list.InsertColumn(nIndex++,&lvc);	

		lvc.pszText = _T("手机号码");
		lvc.cx = 100;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("内容");
		lvc.cx = 400;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("接收时间");
		lvc.cx = 150;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("userId");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("isReaded");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("isDeleted");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = _T("receiveId");
		lvc.cx = 0;
		list.InsertColumn(nIndex++,&lvc);
}
VOID CReceiveMessage::InsertItem(CListViewCtrl& list,CReceiveMessage* pMsg, BOOL bTop)
{
	INT nCount = list.GetItemCount();
	if(bTop)
	{
		nCount = 0;
	}
	INT nIndex(0);

	if(pMsg->m_bReaded)
	{
		list.AddItem(nCount, nIndex++, _T(""), 2);
	}
	else
	{
		list.AddItem(nCount, nIndex++, _T(""), 3);
	}
	list.AddItem(nCount, nIndex++, pMsg->m_strName);
	list.AddItem(nCount, nIndex++, pMsg->m_strTel);
	list.AddItem(nCount, nIndex++, pMsg->m_strContent);
	list.AddItem(nCount, nIndex++, pMsg->m_strReceiveTime);
	
	TCHAR szBuf[10] = {0};
	_itot(pMsg->m_nUserId, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_bReaded, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_bDeleted, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_nReceiveId, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
}
CDeletedMessage::CDeletedMessage()
:m_strTel(_T("")), m_strName(_T("")),m_strContent(_T("")),m_strEventTime(_T(""))
{
	 m_nId = 0;
	 m_nUserId = 0;
	 m_nStatus = 1;
}
CDeletedMessage::CDeletedMessage(const CToSendMessage& msg)
:m_strTel(msg.m_strTel), m_strContent(msg.m_strContent),  m_strName(msg.m_strName), m_strEventTime(msg.m_strCreateTime)
{
	m_nId = msg.m_nToSendId;
	m_nUserId = msg.m_nUserId;
	m_nStatus = 4;
}
CDeletedMessage::CDeletedMessage(const CSendedMessage& msg)
:m_strTel(msg.m_strTel), m_strName(msg.m_strName), m_strContent(msg.m_strContent), m_strEventTime(msg.m_strSendedTime)
{
	m_nId = msg.m_nSendedId;
	m_nUserId = msg.m_nUserId;
	if(msg.m_bSendedOK)
	{
		m_nStatus = 2;
	}
	else
	{
		m_nStatus = 1;
	}
}
CDeletedMessage::CDeletedMessage(const CReceiveMessage& msg)
:m_strTel(msg.m_strTel), m_strContent(msg.m_strContent), m_strName(msg.m_strName), m_strEventTime(msg.m_strReceiveTime)
{
	m_nId = msg.m_nReceiveId;
	m_nUserId = msg.m_nUserId;
	m_nStatus = 3;
}

BOOL CDeletedMessage::GetRecordset(_Recordset** pRst, BOOL bDeleted /* = FALSE */)
{
	LPCTSTR lpszSQL;

	_RecordsetPtr pReceivedRst = NULL;
	lpszSQL = _T("select * from deletedMessage order by deletedId desc");
	
	BOOL bOK(FALSE);
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		bOK = pDB->GetRecordset(lpszSQL, pRst);
	}	

	return bOK;
}

BOOL CDeletedMessage::GetSmsMessage(_RecordsetPtr pRst, CDeletedMessage* pMsg)
{
	_variant_t vt;
	BOOL bOK(FALSE);

	try
	{
		vt = pRst->GetCollect(_T("deletedId"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_nId	 = (long)vt;
		}
		vt = pRst->GetCollect(_T("status"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_nStatus = (long)vt;
		}
		vt = pRst->GetCollect(_T("userId"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_nUserId = long(vt);
		}
		vt = pRst->GetCollect(_T("eventTime"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strEventTime = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("name"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strName = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("tel"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strTel = (LPCTSTR)_bstr_t(vt);
		}
		vt = pRst->GetCollect(_T("content"));
		if(vt.vt != VT_NULL)
		{
			pMsg->m_strContent = (LPCTSTR)_bstr_t(vt);
		}
		bOK = TRUE;
	}
	catch (...)
	{
		NULL;
	}

	return bOK;
}
INT CDeletedMessage::AddMessage(CDeletedMessage& msg)
{
	LPCTSTR pszSQL = _T("select top  1  *  from deletedMessage");
	BOOL bOK(-1);
	
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	_RecordsetPtr pRst;
	if(pDB != NULL)
	{
		BOOL b = pDB->GetRecordset(pszSQL, &pRst);
		if(b)
		{
			try
			{
				pRst->AddNew();

				pRst->PutCollect(_T("tel"), (_variant_t)LPCTSTR(msg.m_strTel));
				pRst->PutCollect(_T("content"), (_variant_t)LPCTSTR(msg.m_strContent));
				pRst->PutCollect(_T("eventTime"), (_variant_t)LPCTSTR(msg.m_strEventTime));
				pRst->PutCollect(_T("userId"), (_variant_t)long(msg.m_nUserId));
				pRst->PutCollect(_T("name"), (_variant_t)LPCTSTR(msg.m_strName));
				pRst->PutCollect(_T("status"), (_variant_t)long(msg.m_nStatus));
				pRst->Update();
				msg.m_nId = (long)(pRst->GetCollect(_T("deletedId")));
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
BOOL CDeletedMessage::RemoveMessage(INT nId)
{
	CString strSQL;
	BOOL bOK(FALSE);
	
	strSQL.Format(_T("delete from deletedMessage where deletedId = %d"), nId);
	
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		bOK = pDB->ExecuteSQL(strSQL);
	}
	
	return bOK;
}
VOID CDeletedMessage::GetListItem(CListViewCtrl& list, INT nItem, CString& strContactCaption , CString& strContactName, CString& strTimeCaption, CString& strTimeValue, CString& strContent)
{
	strContactCaption = _T("发信/收信人:");
	list.GetItemText(nItem, 1, strContactName);
	strTimeCaption = _T("发送/接收时间:");
	list.GetItemText(nItem, 4, strTimeValue );
	list.GetItemText(nItem, 3, strContent );
}
BOOL CDeletedMessage::DeleteSelMsg(CListViewCtrl& list, CSimpleArray<CDeletedMessage*>& arDelete, BOOL bDelAll)
{
	INT nCount = list.GetItemCount();
	INT nSel(0);
	BOOL bSel(FALSE);
	CDeletedMessage msg;
	CDeletedMessage* pDelMsg = NULL;

	for(int i=nCount-1; i>=0; i--)
	{
		if(!bDelAll)
		{
			bSel = list.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED;
		}
		else
		{
			bSel = TRUE;
		}
		if(bSel)
		{
			GetItemByIndex(list, i, msg);
			list.DeleteItem(i);
			RemoveMessage(msg.m_nId);
			pDelMsg = new CDeletedMessage(msg);
			arDelete.Add(pDelMsg);
			nSel++;
		}
	}

	if(nSel > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CDeletedMessage::SetDeleteMsg(CListViewCtrl& list, CSimpleArray<CDeletedMessage*>& arDelete)
{
	CDeletedMessage* pMsg = NULL;
	for(int i=0; i<arDelete.GetSize(); i++)
	{
		pMsg = arDelete[i];
		if(pMsg != NULL)
		{
			AddMessage(*pMsg);
			InsertItem(list, pMsg, TRUE);	
		}
	}
	return TRUE;
}
VOID CDeletedMessage::GetItemByIndex(CListViewCtrl& list, INT nIndex, CDeletedMessage& msg)
{
	INT nSub(1);
	TCHAR szBuf[20] = {0};

	list.GetItemText(nIndex, nSub++, msg.m_strName);
	list.GetItemText(nIndex, nSub++, msg.m_strTel);
	list.GetItemText(nIndex, nSub++, msg.m_strContent);
	list.GetItemText(nIndex, nSub++, msg.m_strEventTime);
	nSub++;
	
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_nId = _ttoi(szBuf);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_nUserId = _ttoi(szBuf);
	list.GetItemText(nIndex, nSub++, szBuf, 20);
	msg.m_nStatus = _ttoi(szBuf);
}
VOID CDeletedMessage::CreateListHeader(CListViewCtrl& list)
{
		LVCOLUMN lvc = { 0 };
		lvc.mask =  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.fmt = LVCFMT_LEFT;
		int nIndex(0);

		lvc.pszText = (LPTSTR)_T("");
		lvc.cx =list.GetStringWidth(_T("")) + 50;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = (LPTSTR)_T("发信/收信人");
		lvc.cx =100;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = (LPTSTR)_T("手机号码");
		lvc.cx =100;
		list.InsertColumn(nIndex++,&lvc);
		
		lvc.pszText = (LPTSTR)_T("内容");
		lvc.cx =300;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = (LPTSTR)_T("发送/接收时间");
		lvc.cx =200;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = (LPTSTR)_T("状态");
		lvc.cx =100;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = (LPTSTR)_T("deletedId");
		lvc.cx =0;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = (LPTSTR)_T("userId");
		lvc.cx =0;
		list.InsertColumn(nIndex++,&lvc);

		lvc.pszText = (LPTSTR)_T("status");
		lvc.cx =0;
		list.InsertColumn(nIndex++,&lvc);
}
VOID CDeletedMessage::InsertItem(CListViewCtrl& list,CDeletedMessage* pMsg, BOOL bTop)
{
	INT nCount = list.GetItemCount();
	if(bTop)
	{
		nCount = 0;
	}
	INT nIndex(0);

	list.AddItem(nCount, nIndex++, _T(""), 4);
	list.AddItem(nCount, nIndex++, pMsg->m_strName);
	list.AddItem(nCount, nIndex++, pMsg->m_strTel);
	list.AddItem(nCount, nIndex++, pMsg->m_strContent);
	list.AddItem(nCount, nIndex++, pMsg->m_strEventTime);
		
	CString ar[4] ={_T("发送失败"), _T("发送成功"), _T("接收成功"), _T("未发短信")};
	list.AddItem(nCount, nIndex++, ar[pMsg->m_nStatus - 1]);
	TCHAR szBuf[10] = {0};
	_itot(pMsg->m_nId, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_nUserId, szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
	_itot(pMsg->m_nStatus ,szBuf, 10);
	list.AddItem(nCount, nIndex++, szBuf);
}

VOID CToSendMessage::GetTypeName(CString& str)
{
	str = _T("待发短信");
}
VOID CSendedMessage::GetTypeName(CString& str)
{
	str = _T("已发短信");
}
VOID CReceiveMessage::GetTypeName(CString& str)
{
	str = _T("已收短信");
}
VOID CDeletedMessage::GetTypeName(CString& str)
{
	str = _T("已删短信");
}