#include "StdAfx.h"
#include "ContactManager.h"
#include "SmsContactTree.h"
#include "mainfrm.h"
#include "SmsSendMessage.h"

#import "msxml3.dll" rename_namespace("MSXML3")
using namespace MSXML3;

CContact::CContact()
:m_strName(_T("")), m_strTel(_T(""))
{
	m_nId = 0;
	m_nParentId = 0;
	m_bLeaf = FALSE;
}

CContact::CContact(const CContact& c)
:m_strName(c.m_strName), m_strTel(c.m_strTel)
{
	m_nId = c.m_nId;
	m_nParentId = c.m_nParentId;
	m_bLeaf = c.m_bLeaf;
}
CContact& CContact::operator = (const CContact& c)
{
	m_strName = c.m_strName;
	m_strTel = c.m_strTel;
	m_nId = c.m_nId;
	m_bLeaf = c.m_bLeaf;
	m_nParentId = c.m_nParentId;

	return *this;
}


CContactManager::CContactManager()
{
	m_pCurrent = new CContact;
	m_pRst = NULL;
}
CContactManager::~CContactManager()
{
	if(m_pCurrent != NULL)
	{
		delete m_pCurrent;
		m_pCurrent = NULL;
	}
	CloseRecordset();
}
VOID CContactManager::CloseRecordset()
{
	if(m_pRst != NULL)
	{
		try
		{
			if(m_pRst->State == adStateOpen)
			{
				m_pRst->Close();
			}
			m_pRst = NULL;
		}
		catch (...)
		{
			NULL;	
		}
	}
}
BOOL CContactManager::GetNameByTel(CString& Tel, CString& strName)
{
	BOOL bOK(FALSE);
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	_RecordsetPtr pRst = NULL;
	CString strSQL;
	strSQL.Format(_T("select top 1 [name] as person from contactlist where RIGHT('%s',11) = RIGHT(tel,11)"), Tel);
	
	if(pDB != NULL)
	{
		if(pDB->GetRecordset(strSQL, &pRst))
		{
			try
			{
				if(!pRst->GetADOEOF())
				{
					_variant_t vt = pRst->GetCollect(_T("person"));
					if(vt.vt != VT_NULL)
					{
						strName = (LPCTSTR)_bstr_t(vt);
						if(strName != _T(""))
						{
							bOK = TRUE;
						}
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
BOOL CContactManager::DeleteContact(CContact* pContact)
{
	BOOL bOK(FALSE);
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{	
		CString strSQL;
		if(!pContact->m_bLeaf)
		{	
			strSQL.Format(_T("delete from contactList where parentId = %d"), pContact->m_nId);	
			bOK = pDB->ExecuteSQL(strSQL);
		}
		strSQL.Format(_T("delete from contactList where contactId = %d"), pContact->m_nId);
		bOK = pDB->ExecuteSQL(strSQL);
	}
	return bOK;
}
INT CContactManager::GetContactCount(CSimpleArray<CContact*>& arTeam)
{
	INT nCount(0);
	if(arTeam.GetSize() == 0)
	{
		return 0;
	}

	CString strSQL(_T("select count(*) as [num] from contactList where isLeaf = true and ("));
	CContact* pContact = arTeam[0];
	INT nID = pContact->m_nId;
	CString strTemp;
	strTemp.Format(_T("parentId = %d"), nID);
	strSQL += strTemp;
	for(int i=1; i<arTeam.GetSize(); i++)
	{
		strTemp.Format(_T(" or parentId = %d"), arTeam[i]->m_nId);
		strSQL += strTemp;
	}
	strSQL += _T(")");

	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		_RecordsetPtr pRst = NULL;
		try
		{
			BOOL b = pDB->GetRecordset(strSQL, &pRst);
			if(pRst != NULL)
			{
				if(!pRst->GetADOEOF())
				{
					_variant_t vt;
					vt = pRst->GetCollect(_variant_t(_T("num")));
					if(vt.vt != VT_NULL)
					{
						nCount = long(vt);
					}
				}
				pRst->Close();
			}
		}
		catch (...)
		{
			if(pRst->State == adStateOpen)
			{
				pRst->Close();
			}
		}
	}
	
	return nCount;
} 
INT CContactManager::CheckTeamByName(CString& strName)
{
	INT nRet(-1);
	CString strSQL;
	strSQL.Format(_T("select top 1 * from contactList where name = '%s' and isLeaf = false"), strName);
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	_RecordsetPtr pRst;
	if(pDB != NULL)
	{
		BOOL b = pDB->GetRecordset(strSQL, &pRst);
		if(b)
		{
			try
			{
				if(!pRst->GetADOEOF())
				{
					_variant_t vt;
					vt = pRst->GetCollect(_variant_t(_T("contactId")));
					if(vt.vt != VT_NULL)
					{
						nRet = (long)vt;
					}	
					pRst->Close();
				}
				else
				{
					CContact c;
					c.m_nId = 0;
					c.m_bLeaf = FALSE;
					c.m_nParentId = 1;
					c.m_strName = strName;
					c.m_strTel = _T("");
					if(AddContact(&c))
					{
						nRet = c.m_nId;
					}
				}
			}
			catch (...)
			{
				if(pRst->State == adStateOpen)
				{
					pRst->Close();
				}
			}
		}
	}

	return nRet;
}
BOOL CContactManager::ModifyContact(CContact* pContact)
{
	if(CheckIsExist(pContact, FALSE)) return FALSE;

	CString strSQL;
	strSQL.Format(_T("select top  1  *  from contactList where contactId = %d"), pContact->m_nId);
	BOOL bOK(FALSE);
	
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	_RecordsetPtr pRst;
	if(pDB != NULL)
	{
		BOOL b = pDB->GetRecordset(strSQL, &pRst);
		if(b)
		{
			try
			{
				if(!pRst->GetADOEOF())
				{
					pRst->PutCollect(_T("name"), (_variant_t)LPCTSTR(pContact->m_strName));
					pRst->PutCollect(_T("tel"), (_variant_t)LPCTSTR(pContact->m_strTel));
					pRst->PutCollect(_T("parentId"), (_variant_t)long(pContact->m_nParentId));
					pRst->PutCollect(_T("isLeaf"), (_variant_t)long(pContact->m_bLeaf));
					pRst->Update();
					
					bOK = TRUE;
				}
				pRst->Close();
			}
			catch (...)
			{
				if(pRst->State == adStateOpen)
				{
					pRst->Close();
				}
			}
		}
	}
	return bOK;
}
BOOL CContactManager::CheckIsExist(CContact* pContact, BOOL bAddORModify /* = TRUE */)
{
	BOOL bExist(TRUE);
	CString strSQL;
	TCHAR szIsLeaf[10] = {0};
	if(pContact->m_bLeaf)
	{
		lstrcpy(szIsLeaf, _T("true"));
	}
	else
	{
		lstrcpy(szIsLeaf, _T("false"));
	}

	if(bAddORModify)
	{
		if(pContact->m_bLeaf)
		{
			strSQL.Format(_T("select top 1 * from contactList where isLeaf = %s and tel = '%s'"), szIsLeaf, pContact->m_strTel);
		}
		else
		{
			strSQL.Format(_T("select top 1 * from contactList where isLeaf = %s and name = '%s'"), szIsLeaf, pContact->m_strName);
		}
		
	}
	else
	{
		if(pContact->m_bLeaf)
		{
			strSQL.Format(_T("select top 1 * from contactList where isLeaf = %s and tel = '%s' and contactId <> %d"), szIsLeaf, pContact->m_strTel, pContact->m_nId);
		}
		else
		{
			strSQL.Format(_T("select top 1 * from contactList where isLeaf = %s and name = '%s' and contactId <> %d"), szIsLeaf, pContact->m_strName, pContact->m_nId);
		}
	}
	
	
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	_RecordsetPtr pRst;
	if(pDB != NULL)
	{
		BOOL b = pDB->GetRecordset(strSQL, &pRst);
		if(b)
		{
			try
			{
				if(pRst->GetADOEOF())
				{
					bExist = FALSE;
				}
				pRst->Close();
			}
			catch (...)
			{
				NULL;
			}
		}
	}
	
	return bExist;
}
BOOL CContactManager::AddContact(CContact* pContact)
{
	if(CheckIsExist(pContact, TRUE))
	{
		return FALSE;
	}

	LPCTSTR pszSQL = _T("select top  1  *  from contactList");
	BOOL bOK(FALSE);
	
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
				pRst->PutCollect(_T("name"), (_variant_t)LPCTSTR(pContact->m_strName));
				pRst->PutCollect(_T("tel"), (_variant_t)LPCTSTR(pContact->m_strTel));
				pRst->PutCollect(_T("parentId"), (_variant_t)long(pContact->m_nParentId));
				pRst->PutCollect(_T("isLeaf"), (_variant_t)long(pContact->m_bLeaf));
				pRst->Update();
				pContact->m_nId = (long)(pRst->GetCollect(_variant_t(_T("contactId"))));
				pRst->Close();
				bOK = TRUE;
			}
			catch (...)
			{
				if(pRst->State == adStateOpen)
				{
					pRst->Close();
				}
			}
		}
	}
	return bOK;
}
VOID CContactManager::GetCurrentContact()
{
	_variant_t vt;

 	try
	{
		vt = m_pRst->GetCollect(_T("contactId"));
		if(vt.vt != VT_NULL)
		{
			m_pCurrent->m_nId = (long)vt;
		}
		
		vt = m_pRst->GetCollect(_T("name"));
		if(vt.vt != VT_NULL)
		{
			m_pCurrent->m_strName = (LPCTSTR)_bstr_t(vt);
		}

		vt = m_pRst->GetCollect(_T("tel"));
		if(vt.vt != VT_NULL)
		{
			m_pCurrent->m_strTel = (LPCTSTR)_bstr_t(vt);
		}

		vt = m_pRst->GetCollect(_T("parentId"));
		if(vt.vt != VT_NULL)
		{
			m_pCurrent->m_nParentId = (long)vt;
		}

		vt = m_pRst->GetCollect(_T("isLeaf"));
		if(vt.vt != VT_NULL)
		{
			m_pCurrent->m_bLeaf = (long)vt;
		}
	}
	catch (...)
	{
		NULL;
	}
}
CContact* CContactManager::GetFirstContact(INT nParentId)
{
	CContact* pContact = NULL;
	CString strSQL;
	strSQL.Format(_T("select * from contactList where parentId = %d"), nParentId);
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		CloseRecordset();

		BOOL b = pDB->GetRecordset(strSQL, &m_pRst);
		if(b)
		{
			try
			{
				if(!m_pRst->GetADOEOF())
				{
					GetCurrentContact();
					m_pRst->MoveNext();
					pContact = m_pCurrent;
				}
				else
				{
					CloseRecordset();
				}
			}
			catch (...)
			{
				NULL;	
			}
		}
	}
	return pContact;
}
CContact* CContactManager::GetNextContact()
{
	return GetNextTeam();
}
CContact* CContactManager::GetFirstTeam()
{
	CContact* pContact = NULL;
	TCHAR* pszSQL = _T("select * from contactList where parentId = 1 and isLeaf = FALSE");
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{
		CloseRecordset();

		BOOL b = pDB->GetRecordset(pszSQL, &m_pRst);
		if(b)
		{
			try
			{
				if(!m_pRst->GetADOEOF())
				{
					GetCurrentContact();
					m_pRst->MoveNext();
					pContact = m_pCurrent;
				}
				else
				{
					CloseRecordset();
				}
			}
			catch (...)
			{
				NULL;	
			}
		}
	}
	return pContact;
}
CContact* CContactManager::GetNextTeam()
{
	CContact* pContact = NULL;
	if(m_pRst != NULL)
	{
		try
		{
			if(!m_pRst->GetADOEOF())
			{
				GetCurrentContact();
				m_pRst->MoveNext();
				pContact = m_pCurrent;
			}
			else
			{
				CloseRecordset();
			}
		}
		catch (...)
		{
			NULL;	
		}
	}

	return pContact;
}

LRESULT CContactList::OnCreate(UINT nMsgType , WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//m_imagelist.Create(MAKEINTRESOURCE(IDB_SMS_CONTACT), 16, 0, RGB(0,255,0));
	m_imagelist.Create(32, 32, ILC_COLOR24|ILC_MASK, 3, 0);
	HBITMAP hBmp = LoadBitmap(CMainFrame::GetResourceModule(), MAKEINTRESOURCE(IDB_SMS_CONTACT));
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
	
	lvc.pszText = _T("名字");
	lvc.cx = 200;
	InsertColumn(nIndex++,&lvc);
	
	lvc.pszText = _T("手机号码");
	lvc.cx = 200;
	InsertColumn(nIndex++, &lvc);
	
	lvc.pszText = _T("id");
	lvc.cx = 0;
	InsertColumn(nIndex++, &lvc);

	lvc.pszText = _T("parentId");
	lvc.cx = 0;
	InsertColumn(nIndex++, &lvc);

	lvc.pszText = _T("isLeaf");
	lvc.cx = 0;
	InsertColumn(nIndex++, &lvc);

	return 0;
}
VOID CContactList::AddContactItem(CContact* pContact)
{
	INT nCount = GetItemCount();
	INT nIndex(0);
	
	AddItem(nCount, nIndex++, _T(""), 2);
	AddItem(nCount, nIndex++, pContact->m_strName);	
	AddItem(nCount, nIndex++, pContact->m_strTel);

	TCHAR szBuf[10];
	_itot(pContact->m_nId, szBuf, 10);
	AddItem(nCount, nIndex++, szBuf);
	_itot(pContact->m_nParentId, szBuf, 10);
	AddItem(nCount, nIndex++, szBuf);
	_itot(pContact->m_bLeaf, szBuf, 10);
	AddItem(nCount, nIndex++, szBuf);
}
LRESULT CContactList::OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_imagelist.Destroy();
	return 0;
}
BOOL CContactList::GetContactByIndex(INT nIndex, CContact& c)
{
	if(nIndex < GetItemCount())
	{
		TCHAR szBuf[10] = {0};
		GetItemText(nIndex, 1, c.m_strName);
		GetItemText(nIndex, 2, c.m_strTel);
		GetItemText(nIndex, 3, szBuf, 10);
		c.m_nId = _ttoi(szBuf);
		GetItemText(nIndex, 4, szBuf, 10);
		c.m_nParentId = _ttoi(szBuf);
		GetItemText(nIndex, 5, szBuf, 10);
		c.m_bLeaf = _ttoi(szBuf);

		return TRUE;
	}
	
	return FALSE;
}
VOID CContactList::UpdateData(CSmsContactTree* pTree)
{
	HTREEITEM pItem = pTree->GetSelectedItem();
	if(pItem != NULL)
	{
		CSimpleMap<HTREEITEM, CContact*>& map = pTree->GetItemMap();
		
		DeleteAllItems();
		CContact* pContact = map.Lookup(pItem);
		if(pContact != NULL)
		{
			if(pContact->m_bLeaf)
			{
				AddContactItem(pContact);
			}
			else
			{
				HTREEITEM pChild = pTree->GetChildItem(pItem);
				while(pChild != NULL)
				{
					pContact = map.Lookup(pChild);
					if(pContact != NULL)
					{
						AddContactItem(pContact);
					}
					pChild = pTree->GetNextSiblingItem(pChild);
				}
			}
		}	
	}
}

CTeamDlg::CTeamDlg(LPCTSTR strName, BOOL bAdd)
:m_strName(strName)
{
	m_bAdd = bAdd;
}
LRESULT CTeamDlg::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	CenterWindow();
	CEdit edit = GetDlgItem(IDC_NAME);
	edit.SetLimitText(90);
	if(m_bAdd)
	{
		SetWindowText(_T("新增组"));
	}
	else
	{
		SetWindowText(_T("修改组"));
	
	}
	edit.SetWindowText(m_strName);	

	return 0;
}
LRESULT CTeamDlg::OnClose(UINT ,WPARAM, LPARAM, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}
LRESULT CTeamDlg::OnCancel(WORD, WORD, HWND, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}
LRESULT CTeamDlg::OnOK(WORD, WORD, HWND, BOOL&)
{
	TCHAR szBuf[100] = {0};
	GetDlgItemText(IDC_NAME, szBuf, 100);
	if(lstrcmp(szBuf, _T("")) == 0)
	{
		MessageBox(_T("组名不能为空!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		m_strName = szBuf;
		EndDialog(IDC_OK);
	}

	return 0;
}

CContactDlg::CContactDlg(LPCTSTR strName, LPCTSTR strTel, INT nId, CSimpleMap<HTREEITEM, CContact*>& map, BOOL bAdd)
:m_strName(strName), m_strTel(strTel), m_ItemMap(map)
{
	m_nId = nId;
	m_bAdd = bAdd;
	m_bChanged = FALSE;
}
LRESULT CContactDlg::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	CenterWindow();
	if(m_bAdd)
	{
		SetWindowText(_T("新增联系人"));
	}
	else
	{
		SetWindowText(_T("修改联系人"));
	}
	CEdit edit = GetDlgItem(IDC_NAME);
	edit.SetLimitText(90);
	edit = GetDlgItem(IDC_TEL);
	edit.SetLimitText(50);
	SetDlgItemText(IDC_NAME, m_strName);
	SetDlgItemText(IDC_TEL, m_strTel);

	CComboBox cmb = GetDlgItem(IDC_TEAM);
	CContact* pContact;
	INT nIndex(0);
	INT nSel(0);
	for(int i=0; i<m_ItemMap.GetSize(); i++)
	{
		pContact = m_ItemMap.GetValueAt(i);
		if(pContact != NULL && !pContact->m_bLeaf)
		{
			nIndex = cmb.AddString(pContact->m_strName);
			cmb.SetItemData(nIndex, pContact->m_nId);
			if(m_nId == pContact->m_nId)
			{
				nSel = nIndex;
			}
		}
	}
	cmb.SetCurSel(nSel);

	return 0;
}
LRESULT CContactDlg::OnClose(UINT ,WPARAM, LPARAM, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}
LRESULT CContactDlg::OnOK(WORD, WORD, HWND, BOOL&)
{
	TCHAR szBuf[100] = {0};
	GetDlgItemText(IDC_NAME, szBuf, 100);
	if(lstrcmp(szBuf, _T("")) == 0)
	{
		MessageBox(_T("联系人名字不能为空!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);	
	}
	else
	{
		GetDlgItemText(IDC_TEL, szBuf, 100);
		if(lstrcmp(szBuf, _T("")) == 0)
		{
			MessageBox(_T("手机号码不能为空!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);	
		}
		else		
		{
			GetDlgItemText(IDC_NAME, szBuf, 100);
			m_strName = szBuf;
			GetDlgItemText(IDC_TEL, szBuf, 100);
			m_strTel = szBuf;
			CComboBox cmb = GetDlgItem(IDC_TEAM);
			m_nId = cmb.GetItemData(cmb.GetCurSel());
			
			EndDialog(IDC_OK);
		}
	}

	return 0;
}
LRESULT CContactDlg::OnCancel(WORD, WORD, HWND, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}

CTransferDlg::CTransferDlg(INT nId, CSimpleMap<HTREEITEM, CContact*>& map)
:m_ItemMap(map)
{
	m_nId = nId;
}
LRESULT CTransferDlg::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	CenterWindow();
	SetWindowText(_T("转移联系人"));
	CComboBox cmb = GetDlgItem(IDC_TEAM);
	CContact* pContact;
	INT nIndex(0);
	INT nSel(0);
	for(int i=0; i<m_ItemMap.GetSize(); i++)
	{
		pContact = m_ItemMap.GetValueAt(i);
		if(pContact != NULL && !pContact->m_bLeaf)
		{
			nIndex = cmb.AddString(pContact->m_strName);
			cmb.SetItemData(nIndex, pContact->m_nId);
			if(m_nId == pContact->m_nId)
			{
				nSel = nIndex;
			}
		}
	}
	cmb.SetCurSel(nSel);
	return 0;
}
LRESULT CTransferDlg::OnClose(UINT ,WPARAM, LPARAM, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}
LRESULT CTransferDlg::OnOK(WORD, WORD, HWND, BOOL&)
{
	CComboBox cmb = GetDlgItem(IDC_TEAM);
	m_nId = cmb.GetItemData(cmb.GetCurSel());

	EndDialog(IDC_OK);
	return 0;
}
LRESULT CTransferDlg::OnCancel(WORD, WORD, HWND, BOOL&)
{
	EndDialog(IDC_CANCEL);
	return 0;
}


CContactManagerDlg* CContactManagerDlg::m_pMangerDlg = NULL;
CContactManagerDlg::CContactManagerDlg()
{
	m_pMangerDlg = this;
	m_pContactTree = new CSmsContactTree;
	m_pContactList = new CContactList;
	m_bModifyFlag = FALSE;
	m_pImportManager = new CImportManager;
}
CContactManagerDlg::~CContactManagerDlg()
{
	if(m_pContactTree != NULL)
	{
		delete m_pContactTree;
		m_pContactTree = NULL;
	}
	if(m_pContactList != NULL)
	{
		delete m_pContactList;
		m_pContactList = NULL;
	}
	if(m_pImportManager != NULL)
	{
		delete m_pImportManager;
		m_pImportManager = NULL;
	}
}
VOID CContactManagerDlg::UpdateLayout()
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
void CContactManagerDlg::OnToolBarCombo(HWND hWndCombo, UINT nID, int nSel, LPCTSTR lpszText, DWORD dwItemData)
{

}
void CContactManagerDlg::PrepareToolBarMenu(UINT nMenuID, HMENU hMenu)
{
	
}
LRESULT CContactManagerDlg::OnInitDlg(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	const DWORD dwSplitStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	const DWORD dwSplitExStyle = WS_EX_CLIENTEDGE;
	
	CenterWindow();

	m_wndSplit.Create(m_hWnd, rcDefault, NULL, dwSplitStyle, dwSplitExStyle);
	m_pContactTree->Create(m_wndSplit, rcDefault, NULL, 0, 0, ID_CONTACT_TREE);
	m_pContactList->Create(m_wndSplit, rcDefault, NULL, 0, 0, ID_CONTACT_LIST);
	m_wndSplit.SetSplitterPanes(*m_pContactTree, *m_pContactList);	
	
	m_hWndClient = m_wndSplit;
	m_hWndToolBar = CMainFrame::CreateSimpleReBarCtrl(m_hWnd, ATL_SIMPLE_REBAR_NOBORDER_STYLE, ATL_IDW_TOOLBAR);
	HWND hToolBar = CMainFrame::CreateSimpleToolBarCtrl(m_hWnd, IDR_CONTACT, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE_EX);
	AddToolbarButtonText(hToolBar, ID_ADD_TEAM, _T("新建组"));
	AddToolbarButtonText(hToolBar, ID_MODIFY_TEAM, _T("修改组"));
	AddToolbarButtonText(hToolBar, ID_DELETE_TEAM, _T("删除组"));
	AddToolbarButtonText(hToolBar, ID_ADD_CONTACT, _T("新建联系人"));
	AddToolbarButtonText(hToolBar, ID_MODIFY_CONTACT, _T("修改联系人"));
	AddToolbarButtonText(hToolBar, ID_DELETE_CONTACT, _T("删除联系人"));
	AddToolbarButtonText(hToolBar, ID_TRANSFER_CONTACT, _T("转移联系人"));
	AddToolbarButtonText(hToolBar, ID_IMPORT, _T("导入"));
	AddToolbarButtonText(hToolBar, ID_EXPORT, _T("导出"));
	AddToolbarButtonText(hToolBar, ID_EXIT, _T("关闭"));

	CMainFrame::AddSimpleReBarBandCtrl(m_hWndToolBar, hToolBar, 0, NULL, FALSE, 0, FALSE);

	m_hWndStatusBar = ::CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, _T("就绪"), m_hWnd, ATL_IDW_STATUS_BAR);

	UpdateLayout();
	m_wndSplit.SetSplitterPos(150);

	m_pContactTree->InitContacts();


	HTREEITEM hItem = m_pContactTree->GetFirstVisibleItem();
	if(hItem != NULL)
	{
		m_pContactTree->SelectItem(hItem);
	}

	return 0;
}
LRESULT CContactManagerDlg::OnExit(WORD, WORD, HWND, BOOL& bHandled)
{
	Close();
	return 0;
}
LRESULT CContactManagerDlg::OnDeleteTeam(WORD, WORD, HWND, BOOL& bHandled)
{
	INT nRet(0);
	HTREEITEM hItem = m_pContactTree->GetSelectedItem();
	if(hItem != NULL)
	{
		CSimpleMap<HTREEITEM, CContact*>& pMap = m_pContactTree->GetItemMap();
		CContact* pContact = pMap.Lookup(hItem);
		if(pContact != NULL)
		{
			if(!pContact->m_bLeaf)
			{
				CString strInfo;
				strInfo.Format(_T("删除组将删除属于该组的所有联系人,\n你确定要删除 %s 组及其成员?"), pContact->m_strName);
				INT n = MessageBox(strInfo, _T("提示信息"), MB_OKCANCEL|MB_ICONQUESTION);
				if(n == IDOK)
				{
					if(!m_pContactTree->DeleteTeam(pContact->m_nId))
					{
						MessageBox(_T("删除组操作失败!"), _T("错误提示"), MB_OK|MB_ICONERROR);
					}
					else
					{
						CWaitCursor wait;
						m_pContactTree->InitContacts();
						SetModifyFlag(TRUE);
						m_pContactList->DeleteAllItems();
					}
				}
			}
			else
			{
				nRet = 1;
			}
		}
	}
	else
	{
		nRet = 1;
	}
	
	if(nRet == 1)
	{
		MessageBox(_T("请先选中你要删除的组!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
	}
	return 0;
}
LRESULT CContactManagerDlg::OnModifyTeam(WORD, WORD, HWND, BOOL& bHandled)
{
	INT nRet(0);
	HTREEITEM hItem = m_pContactTree->GetSelectedItem();
	if(hItem != NULL)
	{
		CSimpleMap<HTREEITEM, CContact*>& pMap = m_pContactTree->GetItemMap();
		CContact* pContact = pMap.Lookup(hItem);
		if(pContact != NULL)
		{
			if(!pContact->m_bLeaf)
			{
				CTeamDlg dlg(pContact->m_strName, FALSE);
				DWORD wRet = dlg.DoModal();
				if(wRet == IDC_OK)
				{
					if(!m_pContactTree->ModifyTeam(pContact->m_nId, dlg.m_strName))
					{
						MessageBox(_T("修改组操作失败，请检查该组是否已存在!"), _T("错误提示"), MB_OK|MB_ICONERROR);
					}
					else
					{
						CWaitCursor wait;
						m_pContactTree->InitContacts();
						SetModifyFlag(TRUE);
						m_pContactTree->SelectItemById(pContact->m_nId);
					}			
				}
			}
			else
			{
				nRet = 1;
			}
		}
	}
	else
	{
		nRet = 1;
	}
	
	if(nRet == 1)
	{
		MessageBox(_T("请先选中你要修改的组!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
	}
	return 0;
}
LRESULT CContactManagerDlg::OnAddContact(WORD, WORD, HWND, BOOL& bHandled)
{
	INT nSel(0);
	HTREEITEM hItem = m_pContactTree->GetSelectedItem();	
	CSimpleMap<HTREEITEM, CContact*>& map = m_pContactTree->GetItemMap();
	CContact* pContact = NULL;

	if(map.GetSize() == 0)
	{
		MessageBox(_T("没有组存在,请先进行新增组操作!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}

	if(hItem != NULL)
	{
		pContact = map.Lookup(hItem);
		if(pContact != NULL)
		{
			nSel = pContact->m_nId;
		}
	}

	CContactDlg dlg(_T(""), _T(""), nSel, map, TRUE);
	INT nRet = dlg.DoModal();
	if(nRet == IDC_OK)
	{
		if(!m_pContactTree->AddContact(dlg.m_strName, dlg.m_strTel, dlg.m_nId))
		{
			MessageBox(_T("新增联系人操作失败，请检查该手机号的联系人是否已存在!"), _T("错误提示"), MB_OK|MB_ICONERROR);
		}
		else
		{
			CWaitCursor wait;
			m_pContactTree->InitContacts();
			SetModifyFlag(TRUE);
			m_pContactTree->SelectItemById(dlg.m_nId);
		}
	}

	return 0;
}
LRESULT CContactManagerDlg::OnModifyContact(WORD, WORD, HWND, BOOL& bHandled)
{
	HTREEITEM hItem = m_pContactTree->GetSelectedItem();
	CContact c;
	CContact* pContact = NULL;
	CSimpleMap<HTREEITEM, CContact*>& map = m_pContactTree->GetItemMap();
	BOOL bSel(FALSE);
	if(hItem != NULL)
	{	
		pContact = 	map.Lookup(hItem);
		if((pContact != NULL) && pContact->m_bLeaf)
		{
			c = *pContact;
			bSel = TRUE;
		}
	}

	if(!bSel)
	{
		INT nIndex(-1);
		for(int i=0; i<m_pContactList->GetItemCount(); i++)
		{
			if((m_pContactList->GetItemState(i, LVIS_SELECTED))&LVIS_SELECTED)
			{
				nIndex = i;
				break;
			}
		}
		if(nIndex != -1)
		{
			bSel = m_pContactList->GetContactByIndex(nIndex, c);
		}	
	}
	
	if(!bSel)
	{
		MessageBox(_T("请选中你要修改的联系人!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	
	CContactDlg dlg(c.m_strName, c.m_strTel, c.m_nParentId, map, FALSE);
	INT nRet = dlg.DoModal();
	if(nRet == IDC_OK)
	{
		c.m_nParentId = dlg.m_nId;
		c.m_strName = dlg.m_strName;
		c.m_strTel = dlg.m_strTel;
		if(!m_pContactTree->ModifyContact(c))
		{
			MessageBox(_T("修改联系人操作失败，请检查该手机号的联系人是否已存在!"), _T("错误提示"), MB_OK|MB_ICONERROR);
		}
		else
		{
			CWaitCursor wait;
			m_pContactTree->InitContacts();
			SetModifyFlag(TRUE);
			m_pContactTree->SelectItemById(dlg.m_nId);
		}
	}
	return 0;
}
LRESULT CContactManagerDlg::OnDeleteContact(WORD, WORD, HWND, BOOL& bHandled)
{
	CSimpleArray<INT> arDelete;
	HTREEITEM hItem = m_pContactTree->GetSelectedItem();
	CContact c;
	CContact* pContact = NULL;
	CSimpleMap<HTREEITEM, CContact*>& map = m_pContactTree->GetItemMap();
	BOOL bSel(FALSE);
	if(hItem != NULL)
	{	
		pContact = 	map.Lookup(hItem);
		if((pContact != NULL) && pContact->m_bLeaf)
		{
			arDelete.Add(pContact->m_nId);
			c.m_nParentId = pContact->m_nParentId;
			bSel = TRUE;
		}
	}

	if(!bSel)
	{
		for(int i=0; i<m_pContactList->GetItemCount(); i++)
		{
			if((m_pContactList->GetItemState(i, LVIS_SELECTED))&LVIS_SELECTED)
			{
				bSel = TRUE;
				m_pContactList->GetContactByIndex(i, c);
				arDelete.Add(c.m_nId);
			}
		}
	}
	
	if(!bSel)
	{
		MessageBox(_T("请选中你要删除的联系人!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	
	BOOL bDelete(TRUE);
	INT nCount = arDelete.GetSize();
	if(nCount>= 1)
	{
		CString strInfo;
		strInfo.Format(_T("你确定要删除你所选中的这些(%d个)联系人?"), nCount);
		INT nRet = MessageBox(strInfo, _T("提示信息"), MB_OKCANCEL|MB_ICONQUESTION);
		if(nRet == IDCANCEL)
		{
			bDelete = FALSE;
		}
	}
	if(bDelete)
	{
		CWaitCursor wait;

		for(INT k=0; k<nCount; k++)
		{
			m_pContactTree->DeleteContact(arDelete[k]);
		}
		m_pContactTree->InitContacts();
		SetModifyFlag(TRUE);
		m_pContactTree->SelectItemById(c.m_nParentId);	
	}
	
	return 0;
}
LRESULT CContactManagerDlg::OnTransferContact(WORD, WORD, HWND, BOOL& bHandled)
{
	CSimpleArray<CContact> arDelete;
	HTREEITEM hItem = m_pContactTree->GetSelectedItem();
	CContact c;
	CContact* pContact = NULL;
	CSimpleMap<HTREEITEM, CContact*>& map = m_pContactTree->GetItemMap();
	BOOL bSel(FALSE);
	if(hItem != NULL)
	{	
		pContact = 	map.Lookup(hItem);
		if((pContact != NULL) && pContact->m_bLeaf)
		{
			arDelete.Add(*pContact);
			bSel = TRUE;
		}
	}

	if(!bSel)
	{
		for(int i=0; i<m_pContactList->GetItemCount(); i++)
		{
			if((m_pContactList->GetItemState(i, LVIS_SELECTED))&LVIS_SELECTED)
			{
				bSel = TRUE;
				m_pContactList->GetContactByIndex(i, c);
				arDelete.Add(c);
			}
		}
	}
	
	if(!bSel)
	{
		MessageBox(_T("请选中你要转移的联系人!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}

	INT nOrg = c.m_nParentId;
	CTransferDlg dlg(nOrg, m_pContactTree->GetItemMap());
	INT nRet = dlg.DoModal();
	if(nRet = IDC_OK)
	{
		INT nNew = dlg.m_nId;
		if(nNew != nOrg)
		{
			CWaitCursor wait;

			for(int i=0; i<arDelete.GetSize(); i++)
			{
				CContact& temp = arDelete[i];
				temp.m_nParentId = nNew;
				m_pContactTree->ModifyContact(temp);
			}
			arDelete.RemoveAll();
			
			m_pContactTree->InitContacts();
			SetModifyFlag(TRUE);
			m_pContactTree->SelectItemById(nNew);				
		}
	}

	return 0;
}
LRESULT CContactManagerDlg::OnAddTeam(WORD, WORD, HWND, BOOL& bHandled)
{
	CTeamDlg dlg(_T(""), TRUE);
	DWORD wRet = dlg.DoModal();
	if(wRet == IDC_OK)
	{
		if(!m_pContactTree->AddTeam(dlg.m_strName))
		{
			MessageBox(_T("新增组操作失败,请检查该组是否已存在!"), _T("错误提示"), MB_OK|MB_ICONERROR);
		}
		else
		{
			CWaitCursor wait;
			m_pContactTree->InitContacts();
			SetModifyFlag(TRUE);
		}
	}
	return 0;
}
BOOL CContactManagerDlg::GetCurrentSelInfo(CString& strName, CString& strTel)
{
	INT nIndex(-1);
	for(int i=0; i<m_pContactList->GetItemCount(); i++)
	{
		if((m_pContactList->GetItemState(i, LVIS_SELECTED))&LVIS_SELECTED)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex != -1)
	{
		CContact c;
		if(m_pContactList->GetContactByIndex(nIndex, c))
		{
			strName = c.m_strName;
			strTel = c.m_strTel;
			return TRUE;
		}
	}

	return FALSE;
}
LRESULT CContactManagerDlg::OnListNotify(int , LPNMHDR pnmh, BOOL& )
{
	if(pnmh->code == NM_DBLCLK)
	{
		PostMessage(WM_COMMAND, ID_MODIFY_CONTACT, 0);
	}

	if(pnmh->code == NM_RCLICK)
	{
		HMENU hParent = CreatePopupMenu();
		UINT nFlag(MF_STRING);

		CString strName, strTel;
		if(!GetCurrentSelInfo(strName, strTel))
		{
			nFlag |= MF_GRAYED;	
		}
		AppendMenu(hParent, MF_STRING, ID_ADD_CONTACT, _T("新建联系人"));
		AppendMenu(hParent, nFlag, ID_MODIFY_CONTACT, _T("修改联系人"));
		AppendMenu(hParent, nFlag, ID_DELETE_CONTACT, _T("删除联系人"));
		AppendMenu(hParent, nFlag, ID_TRANSFER_CONTACT, _T("转移联系人"));
		AppendMenu(hParent, MF_SEPARATOR, NULL, NULL);
		AppendMenu(hParent, nFlag, ID_SMS_REPLY, _T("发送短信"));

		POINT pt;
		GetCursorPos(&pt);
		TrackPopupMenu(hParent, TPM_LEFTALIGN|TPM_TOPALIGN, pt.x, pt.y, 0, *this, NULL);

		DestroyMenu(hParent);
	}

	return 0;
}
LRESULT CContactManagerDlg::OnTreeNotify(int , LPNMHDR pnmh, BOOL& )
{
	if(pnmh->code == TVN_SELCHANGED)
	{
		m_pContactList->UpdateData(m_pContactTree);

		//统计该组人数
		HTREEITEM pItem = m_pContactTree->GetSelectedItem();
		if(pItem != NULL)
		{
			CSimpleMap<HTREEITEM, CContact*>& map = m_pContactTree->GetItemMap();
			CContact* pContact = map.Lookup(pItem);
			if(pContact != NULL)
			{
				if(!pContact->m_bLeaf)
				{
					INT nCount(0);
					HTREEITEM pChild = m_pContactTree->GetChildItem(pItem);
					while(pChild != NULL)
					{
						nCount++;
						pChild = m_pContactTree->GetNextSiblingItem(pChild);
					}
					
					CString str;
					str.Format(_T("%s 组里共有%d个联系人"), pContact->m_strName, nCount);
					::SendMessage(m_hWndStatusBar, SB_SIMPLE, TRUE, 0L);
					::SendMessage(m_hWndStatusBar, SB_SETTEXT, (255 | SBT_NOBORDERS), (LPARAM)(LPCTSTR)str);
				}
			}	
		}
	}
	return 0;
}
VOID CContactManagerDlg::Close()
{
	if(GetModifyFlag())
	{
		CSmsClientView* pClient = CMainFrame::GetClientView();
		if(pClient != NULL)
		{
			pClient->LoadContacts();
		}
	}
	EndDialog(0);
}
LRESULT CContactManagerDlg::OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close();
	return 0;
}
LRESULT CContactManagerDlg::OnSize(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam != SIZE_MINIMIZED)
	{
		UpdateLayout();
	}
	bHandled = FALSE;	
	
	return 0;
}
LRESULT CContactManagerDlg::OnSmsSendMsg(WORD, WORD, HWND, BOOL&)
{
	CString strName, strTel;
	if(GetCurrentSelInfo(strName, strTel))
	{
		CSmsSendDlg dlg(ID_SMS_REPLY);
		dlg.m_strName = strName;
		dlg.m_strTel = strTel;
		dlg.m_strContent = _T("");
		dlg.DoModal();
	}
	
	return 0;
}
BOOL CContactManagerDlg::GetImportFileName(CString& strName)
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name


	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = m_pImportManager->GetFilter();
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = _T("打开导入文件");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn) != 0)
	{
		strName = szFile;
		return TRUE;
	}

	return FALSE;
}
VOID CContactManagerDlg::ReloadData(INT nId)
{
	m_pContactTree->InitContacts();
	m_pContactTree->SelectItemById(nId);
	m_bModifyFlag = TRUE;
}
LRESULT CContactManagerDlg::OnImport(WORD, WORD ,HWND, BOOL&)
{
	CString strName;
	if(GetImportFileName(strName))
	{
		if(!m_pImportManager->DoImport(strName))
		{
			MessageBox(_T("导入失败，文件格式不支持!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		}
	}

	return 0;
}

LRESULT CContactManagerDlg::OnExport(WORD, WORD, HWND, BOOL&)
{
	CExportXmlDlg dlg;
	dlg.DoExport();
	return 0;
}




BOOL CImportBroker::RegisterImport( CString& strExt, IImportContact* pImport)
{
	strExt.MakeUpper();
	return m_mapImport.Add(strExt, pImport);					
}
BOOL CImportBroker::DoImport(CString& strFile)
{
	CString strExt;
	if(GetFileExt(strFile, strExt))
	{
		strExt.MakeUpper();
		IImportContact* pImport = m_mapImport.Lookup(strExt);
		if(pImport != NULL)
		{
			pImport->DoImport(strFile);
			return TRUE;
		}
	}

	return FALSE;
}
BOOL  CImportBroker::GetFileExt(CString& strFile, CString& strExt)
{
	INT nPos = strFile.ReverseFind('.');
	if(nPos != -1)
	{
		
		INT nLen = strFile.GetLength() - nPos - 1;
		if(nLen > 0)
		{
			strExt = strFile.Right(nLen);
			return TRUE;
		}
	}

	return FALSE;
}
VOID CImportBroker::ClearAll()
{
	IImportContact* pImport = NULL;
	for(int i=0; i<m_mapImport.GetSize(); i++)
	{
		pImport = m_mapImport.GetValueAt(i);
		if(pImport != NULL)
		{
			delete pImport;
		}
	}
	m_mapImport.RemoveAll();
}



CImportManager::CImportManager()
{
	m_bInited = FALSE;
	m_szFilter[0] = 0;
	LoadAllImport();
}
BOOL CImportManager::DoImport(CString& strFile)
{
	if(!m_bInited)
	{
		LoadAllImport();
	}
	return m_broker.DoImport(strFile);
}
CImportManager::~CImportManager()
{
	m_broker.ClearAll();
}
VOID CImportManager::LoadAllImport()
{
	IImportContact* pImport = new CExcelImport();
	if(!m_broker.RegisterImport(CString(_T("xls")), pImport))
	{
		delete pImport;
	}
	pImport = new CXmlImport();
	if(!m_broker.RegisterImport(CString(_T("xml")), pImport))
	{
		delete pImport;
	}

	TCHAR szFilter[512] = _T("All File(*.*)\0*.*\0Microsoft Office Excel(*.xls)\0*.xls\0XML File(*.XML)\0*.XML\0\0");
	memcpy(m_szFilter, szFilter, sizeof(szFilter));
	m_bInited = TRUE;
}

VOID CExcelImport::DoImport(CString& strFile)
{
	CImportExcelDlg dlg;
	dlg.DoImport(strFile);
}

CExcelDB::CExcelDB()
: m_strFile(_T(""))
{
	m_pOperate = new CDBOperate;
}
CExcelDB::~CExcelDB()
{
	if(m_pOperate != NULL)
	{
		if(m_pOperate->IsOPen())
		{
			m_pOperate->Close();
		}

		delete m_pOperate;
		m_pOperate = NULL;
	}
}
VOID CExcelDB::SetExcelFile(CString& strFile)
{
	m_strFile = strFile;
}
CDBOperate* CExcelDB::GetDBOperate()
{
	if(!m_pOperate->IsOPen())
	{
		if(m_strFile != _T(""))
		{
			CString strCon;
			strCon.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data source=%s; \
				Extended Properties=Excel 8.0;"), m_strFile);
			m_pOperate->Open(strCon);
		}
	}

	if(m_pOperate->IsOPen())
	{
		return m_pOperate;
	}
	else
	{
		return NULL;
	}
}

CImportExcelDlg::CImportExcelDlg()
{
	m_bImporting = FALSE;
}
CImportExcelDlg::~CImportExcelDlg()
{
	ClearData();
}
LRESULT CImportExcelDlg::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	CenterWindow();

	CComboBox cb = GetDlgItem(IDC_SHEET);
	for(int i=0; i<m_mapSheet.GetSize(); i++)
	{
		cb.InsertString(i, m_mapSheet.GetKeyAt(i));
	}
	if(m_mapSheet.GetSize() > 0)
	{
		cb.SetCurSel(0);
		SendMessage(WM_COMMAND, MAKEWPARAM(IDC_SHEET, CBN_SELCHANGE) ,(LONG)cb.m_hWnd);
	}

	cb = GetDlgItem(IDC_TEAM);
	CContact* pContact = NULL;
	for(int j=0; j<m_arTeam.GetSize(); j++)
	{
		pContact = m_arTeam[j];
		if(pContact != NULL)
		{
			cb.InsertString(j, pContact->m_strName);
			cb.SetItemData(j, pContact->m_nId);
		}
	}
	if(m_arTeam.GetSize() > 0)
	{
		cb.SetCurSel(0);
	}
	return 0;
}

LRESULT CImportExcelDlg::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
	if(!m_bImporting)
	{
		EndDialog(0);
	}

	return 0;
}
LRESULT CImportExcelDlg::OnCloseCmd(WORD, WORD, HWND, BOOL&)
{
	if(!m_bImporting)
	{
		EndDialog(0);
	}
	
	return 0;
}
LRESULT CImportExcelDlg::OnSheetSelectChanged(WORD, WORD, HWND, BOOL&)
{
	TCHAR szBuf[256] = {0};
	GetDlgItemText(IDC_SHEET, szBuf, 256);
	CSimpleArray<CString>* pArField = m_mapSheet.Lookup(CString(szBuf));
	if(pArField != NULL)
	{
		CComboBox cb1 = GetDlgItem(IDC_NAME);
		CComboBox cb2 = GetDlgItem(IDC_TEL);
		cb1.ResetContent();
		cb2.ResetContent();
		
		for(int i=0; i<pArField->GetSize(); i++)
		{
			cb1.InsertString(i, pArField-> operator[](i));
			cb2.InsertString(i, pArField-> operator[](i));
		}
		if(pArField->GetSize() > 0)
		{
			cb1.SetCurSel(0);
			cb2.SetCurSel(0);
		}
	}	

	return 0;
}
VOID CImportExcelDlg::DoImport(CString& strFile)
{
	ClearData();

	if(!LoadTeam())
	{
		::MessageBox(NULL, _T("导入失败，地址簿中没有组存在，请先建立一个组!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return ;
	}

	if(!ParseExcel(strFile))
	{
		::MessageBox(NULL, _T("导入失败, Excel文件解析失败!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return;
	}
	
	DoModal();
	
}
BOOL CImportExcelDlg::LoadTeam()
{
	CContactManager manager;
	CContact* pContact = NULL;
	INT nCount(0);
	pContact = manager.GetFirstTeam();
	while(pContact != NULL)
	{
		nCount++;
		CContact* pTemp = new CContact(*pContact);
		m_arTeam.Add(pTemp);
		pContact = manager.GetNextTeam();
	}
	if(nCount > 0) 
		return TRUE;
	else 
		return FALSE;
}
BOOL CImportExcelDlg::ParseExcel(CString& strFile)
{
	BOOL bOK(FALSE);
	m_ExcelDB.SetExcelFile(strFile);
	CDBOperate* pDBOperate = m_ExcelDB.GetDBOperate();
	if(pDBOperate != NULL)
	{
		_RecordsetPtr pRst = NULL;
		_ConnectionPtr pCon = pDBOperate->GetConnection();
		try
		{
			pRst = pCon->OpenSchema(adSchemaTables);
			if(pRst != NULL)
			{
				_RecordsetPtr pRstField = NULL;
				CString strSQL;
				CString strSheet;
				_variant_t vtVal;
				CSimpleArray<CString>* pArField = NULL;
				while (!pRst->GetADOEOF())
				{
					vtVal = pRst->GetCollect(_variant_t(_T("table_name")));
					if(vtVal.vt != VT_EMPTY)
					{
						strSheet = (LPCTSTR)_bstr_t(vtVal);
						strSQL.Format(_T("select top 1 * from [%s]"), strSheet);
						BOOL b = pDBOperate->GetRecordset(strSQL, &pRstField);
						if(b)
						{
							pArField = new CSimpleArray<CString>;
							m_mapSheet.Add(strSheet, pArField);
							for(long i=0; i<pRstField->GetFields()->GetCount(); ++i)
							{
								pArField->Add((CString)(LPCTSTR)(_bstr_t) \
									(pRstField->GetFields()->GetItem(_variant_t(i))->GetName()));
							}

							pRstField->Close();
						}
					}
					pRst->MoveNext();
				}
				pRst->Close();
				bOK = TRUE;
			}
		}
		catch (...)
		{
			
		}
	}

	return bOK;
}
VOID CImportExcelDlg::ClearData()
{
	CSimpleArray<CString>* pAr = NULL;
	int i;
	for( i=0; i<m_mapSheet.GetSize(); ++i)
	{
		pAr = m_mapSheet.GetValueAt(i);
		if(pAr != NULL)
		{
			delete pAr;
		}
	}
	m_mapSheet.RemoveAll();

	CContact* p = NULL;
	for( i=0; i<m_arTeam.GetSize(); ++i)
	{
		p = m_arTeam[i];
		if(p!= NULL) delete p;
	}
	m_arTeam.RemoveAll();
}

LRESULT CImportExcelDlg::OnImport(WORD, WORD, HWND, BOOL&)
{
	CString strSheet, strName, strTel;
	GetDlgItemText(IDC_SHEET, strSheet.GetBuffer(256), 256);
	strSheet.ReleaseBuffer();

	GetDlgItemText(IDC_NAME, strName.GetBuffer(256), 256);
	strName.ReleaseBuffer();

	GetDlgItemText(IDC_TEL, strTel.GetBuffer(256), 256);
	strTel.ReleaseBuffer();

	CComboBox cb = GetDlgItem(IDC_TEAM);
	INT nIndex = cb.GetCurSel();
	INT nTeam(-1);
	if(nIndex != CB_ERR)
	{
		nTeam = cb.GetItemData(nIndex);
	}
	
	if(strSheet == _T(""))
	{
		MessageBox(_T("Excel页不能为空!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	if(strName == _T(""))
	{
		MessageBox(_T("名字所在列不能为空!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	if(strTel == _T(""))
	{
		MessageBox(_T("手机号码所在列不能为空!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	if(nTeam == -1)
	{
		MessageBox(_T("地址簿导入组不能为空!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}

	StartImport(strSheet, strName, strTel, nTeam);

	return 0;
}

VOID CImportExcelDlg::StartImport(CString& strSheet, CString& strName, CString& strTel, INT nTeamId)
{
	CButton btn1 = GetDlgItem(IDC_IMPORT);
	btn1.EnableWindow(FALSE);
	CButton btn2 = GetDlgItem(IDC_CLOSE);
	btn2.EnableWindow(FALSE);
	m_bImporting = TRUE;

	CString strSQL;
	 strSQL.Format(_T("select [%s],[%s] from [%s]"), strName, strTel, strSheet);
	 CDBOperate* pDBOperate = m_ExcelDB.GetDBOperate();
	CContactManager contactManager;
	INT nTotal(0);
	INT nAll(0);
	CProgressBarCtrl pb = GetDlgItem(IDC_PROGRESS);
	
	 if(pDBOperate != NULL)
	 {
		_RecordsetPtr pRst = NULL;
		if(pDBOperate->GetRecordset(strSQL, &pRst))
		{
			try
			{
				CWaitCursor wait;

				 nAll = pRst->GetRecordCount();
				if(nAll > 0)
				{
					pb.SetRange(0, nAll);
					pb.SetPos(0);

					CContact contact;
					contact.m_bLeaf = TRUE;
					contact.m_nParentId = nTeamId;
					
					_variant_t vtVal;
					while(!pRst->GetADOEOF())
					{	
						vtVal = pRst->GetCollect(_variant_t(strName));	
						if(vtVal.vt == VT_NULL)
						{
							pb.OffsetPos(1);
							pRst->MoveNext();
							continue;
						}
						contact.m_strName = (LPCTSTR)_bstr_t(vtVal);

						vtVal = pRst->GetCollect(_variant_t(strTel));	
						if(vtVal.vt == VT_NULL)
						{
							pb.OffsetPos(1);
							pRst->MoveNext();
							continue;
						}
						contact.m_strTel = (LPCTSTR)_bstr_t(vtVal);
						
						if(contactManager.AddContact(&contact))
						{
							nTotal++;
						}
						pb.OffsetPos(1);
						pRst->MoveNext();

					}

					pRst->Close();
				}

			}
			catch (...)
			{
				pb.SetPos(nAll);	
			}	
			
		}
	 }
	
	 btn1.EnableWindow(TRUE);
	 btn2.EnableWindow(TRUE);
	m_bImporting = FALSE;

	 CString strInfo;
	 strInfo.Format(_T("导入结果:%d条数据导入成功，%d条数据导入失败!"), nTotal, nAll-nTotal);
	 MessageBox(strInfo, _T("提示信息"), MB_ICONINFORMATION|MB_OK);
	 if(nTotal > 0)
	 {
		 CContactManagerDlg* pMangerDlg = CContactManagerDlg::GetContactManagerDlg();
		 if(pMangerDlg != NULL && pMangerDlg->IsWindow())
		 {
			 pMangerDlg->ReloadData(nTeamId);
		 }
	 }

}

CExportXmlDlg::CExportXmlDlg()
{
	m_bExporting = FALSE;
}
CExportXmlDlg::~CExportXmlDlg()
{
	ClearData();
}
VOID CExportXmlDlg::ClearArray(CSimpleArray<CContact*>& arTeam)
{
	CContact* p = NULL;
	for(int i=0; i<arTeam.GetSize(); i++)
	{
		p = arTeam[i];
		if(p != NULL)
		{
			delete p;
		}
	}
	arTeam.RemoveAll();
}
VOID CExportXmlDlg::ClearData()
{
	INT i;
	CContact* p = NULL;
	for( i=0; i<m_arTeam.GetSize(); ++i)
	{
		p = m_arTeam[i];
		if(p!= NULL) delete p;
	}
	m_arTeam.RemoveAll();
}
BOOL CExportXmlDlg::LoadTeam()
{
	CContactManager manager;
	CContact* pContact = NULL;
	INT nCount(0);
	pContact = manager.GetFirstTeam();
	while(pContact != NULL)
	{
		nCount++;
		CContact* pTemp = new CContact(*pContact);
		m_arTeam.Add(pTemp);
		pContact = manager.GetNextTeam();
	}
	if(nCount > 0) 
		return TRUE;
	else 
		return FALSE;
}
LRESULT CExportXmlDlg::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	CenterWindow();

	m_teamTree = GetDlgItem(IDC_TREE);
	CContact* pContact = NULL;
	HTREEITEM hPre = NULL;
	for(int i=0; i<m_arTeam.GetSize(); i++)
	{
		pContact = m_arTeam[i];
		if(pContact != NULL)
		{
			hPre = m_teamTree.InsertItem(pContact->m_strName, 0, 0, NULL, hPre);
			m_teamTree.SetItemData(hPre, pContact->m_nId);
		}
	}

	return 0;
}
LRESULT CExportXmlDlg::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
	if(!m_bExporting)
	{
		EndDialog(0);
	}
	
	return 0;
}
LRESULT CExportXmlDlg::OnCloseCmd(WORD, WORD, HWND, BOOL&)
{
	if(!m_bExporting)
	{
		EndDialog(0);
	}
	
	return 0;
}
VOID CExportXmlDlg::DoExport()
{
	CExportXmlDlg dlg;
	if(!dlg.LoadTeam())
	{
		::MessageBox(NULL, _T("没有联系人存在，导出失败!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return ;
	}
	dlg.DoModal();
}
LRESULT CExportXmlDlg::OnContactNotify(int, LPNMHDR pnmh, BOOL&)
{
	if(pnmh->code == NM_CLICK)
	{
		CPoint pt;
		GetCursorPos(&pt);
		m_teamTree.ScreenToClient(&pt);
		UINT nFlag(0);
		HTREEITEM hSel = m_teamTree.HitTest(pt, &nFlag);
		if(hSel != NULL)
		{
			m_teamTree.SelectItem(hSel);
		}
	}

	return 0;
}

LRESULT CExportXmlDlg::OnAllCheck(WORD, WORD, HWND, BOOL&)
{
	CButton btn;
	btn = GetDlgItem(IDC_ALL_CHECK);
	BOOL bChecked = (btn.GetCheck() == BST_CHECKED);
	INT i(0);
	HTREEITEM hItem = NULL;

	hItem = m_teamTree.GetFirstVisibleItem();
	while(hItem != NULL)
	{
		m_teamTree.SetCheckState(hItem, bChecked);
		hItem = m_teamTree.GetNextVisibleItem(hItem);
	}
	
	return 0;
}

LRESULT CExportXmlDlg::OnExplore(WORD, WORD, HWND, BOOL&)
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260] = {0} ;      // buffer for file name


	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("XML File(*.XML)\0*.XML\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrDefExt = _T("XML");
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = _T("保存导出文件");
	ofn.Flags = 0;

	if(GetSaveFileName(&ofn))
	{	
		CString strName = szFile;
		CString strExt = strName.Right(4);
		strExt.MakeUpper();
		if(strExt != _T(".XML"))
		{
			strName += _T(".XML");
		}
		
		SetDlgItemText(IDC_FILE, strName);
	}
	return 0;
}

LRESULT CExportXmlDlg::OnExport(WORD, WORD, HWND, BOOL&)
{
	CSimpleArray<CContact*> arTeam;
	HTREEITEM hItem = NULL;
	INT nCount(0);
	CContact* pContact = NULL;

	hItem = m_teamTree.GetFirstVisibleItem();
	CString strText;
	while(hItem != NULL)
	{
		if(m_teamTree.GetCheckState(hItem))
		{
			INT dwVal = m_teamTree.GetItemData(hItem);
			m_teamTree.GetItemText(hItem, strText);
			pContact = new CContact;
			pContact->m_nId = dwVal;
			pContact->m_bLeaf = FALSE;
			pContact->m_strName = strText;
			arTeam.Add(pContact);
			nCount++;
		}
		hItem = m_teamTree.GetNextVisibleItem(hItem);
	}

	if(nCount == 0)
	{
		ClearArray(arTeam);
		MessageBox(_T("请先选中你要导出的组!"), _T("提示信息"), MB_ICONINFORMATION|MB_OK);
		return 0;
	}

	CString strFile;
	GetDlgItemText(IDC_FILE, strFile.GetBuffer(256), 256);
	strFile.ReleaseBuffer();
	if(strFile == _T(""))
	{
		ClearArray(arTeam);
		MessageBox(_T("请先选择你要到导出的文件路径!"), _T("提示信息"), MB_ICONINFORMATION|MB_OK);
		return 0;
	}
	
	ExportFile(strFile, arTeam);
	
	ClearArray(arTeam);

	return 0;
}
VOID CExportXmlDlg::ExportFile(CString& strFile, CSimpleArray<CContact*>& arTeam)
{
	MSXML3::IXMLDOMDocumentPtr oDoc;
	MSXML3::IXMLDOMElementPtr oRoot, oTeam, oContact;

	CContactManager manager;
	CButton btn1 = GetDlgItem(IDC_CLOSE);
	CButton btn2 = GetDlgItem(IDC_EXPORT);
	CProgressBarCtrl pb = GetDlgItem(IDC_PROGRESS);
	BOOL bOK(FALSE);
	INT nExportCount(0);
	INT nCount;

	try
	{
		CWaitCursor wait;

		HRESULT hr = oDoc.CreateInstance(__uuidof(MSXML3::DOMDocument));
		if(FAILED(hr))
		{
			return ;
		}
		
		nCount = manager.GetContactCount(arTeam);
		m_bExporting = TRUE;
		btn1.EnableWindow(FALSE);
		btn2.EnableWindow(FALSE);
		pb.SetRange(0, nCount+1);
		pb.SetPos(0);
		CContact* pContact = NULL;
		CString strSQL;
		CDBOperate* pDBOperate = CSmsDB::GetDBOperate();
		_RecordsetPtr pRst = NULL;
		CString strName, strTel;
		_variant_t vtVal;

		if(pDBOperate != NULL)
		{
			oRoot = oDoc->createElement(_bstr_t((_T("contactList"))));
			for(int i=0; i< arTeam.GetSize(); i++)
			{
				pContact = arTeam[i];
				if(pContact == NULL) continue;

				oTeam = oDoc->createElement(_bstr_t(_T("team")));
				oTeam->setAttribute(_bstr_t(_T("name")), _variant_t(pContact->m_strName));
				oRoot->appendChild(oTeam);
				
				strSQL.Format(_T("select * from contactList where parentId = %d"), pContact->m_nId);
				BOOL b = pDBOperate->GetRecordset(strSQL, &pRst);
				if(b)
				{
					while(!pRst->GetADOEOF())
					{
						vtVal = pRst->GetCollect(_variant_t(_T("name")));
						if(vtVal.vt == VT_NULL)
						{
							pb.OffsetPos(1);
							pRst->MoveNext();
							continue;
						}
						strName = (LPCTSTR)_bstr_t(vtVal);

						vtVal = pRst->GetCollect(_variant_t(_T("tel")));
						if(vtVal.vt == VT_NULL)
						{
							pb.OffsetPos(1);
							pRst->MoveNext();
							continue;
						}
						strTel = (LPCTSTR)_bstr_t(vtVal);

						oContact = oDoc->createElement(_bstr_t(_T("contact")));
						oContact->setAttribute(_bstr_t(_T("name")), _variant_t(strName));
						oContact->setAttribute(_bstr_t(_T("tel")), _variant_t(strTel));
						oTeam->appendChild(oContact);

						nExportCount++;
						pb.OffsetPos(1);
						pRst->MoveNext();
					}
					pRst->Close();
				}
			}
			bOK = TRUE;
			if(bOK)
			{
				bOK	 = FALSE;
				if(oDoc->loadXML(oRoot->xml) == VARIANT_TRUE)
				{
					oDoc->async = VARIANT_FALSE;
					oDoc->save(_variant_t(strFile));
					bOK = TRUE;
				}

			}
		}		
		
	}
	catch (...)
	{
		NULL;
	}

	pb.SetPos(nCount+1);
	m_bExporting = FALSE;
	btn1.EnableWindow(TRUE);
	btn2.EnableWindow(TRUE);
	
	if(bOK)
	{
		CString strInfo;
		strInfo.Format(_T("导出成功，共导出数据%d条!"), nExportCount);
		MessageBox(strInfo, _T("提示信息"), MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(_T("导出失败!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
	}
}

VOID CXmlImport::DoImport(CString& strFile)
{
	CImportXmlDlg dlg;
	dlg.DoImport(strFile);
}


VOID CImportXmlDlg::DoImport(CString& strFile)
{
	m_strFile = strFile;
	if(!ParseXML(strFile))
	{
		::MessageBox(NULL, _T("导入失败，XML文件解析失败!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return ;
	}
	DoModal();
}
CImportXmlDlg::CImportXmlDlg()
{
	m_bImporting = FALSE;
}
LRESULT CImportXmlDlg::OnAllCheck(WORD, WORD, HWND, BOOL&)
{
	CButton btn;
	btn = GetDlgItem(IDC_ALL_CHECK);
	BOOL bChecked = (btn.GetCheck() == BST_CHECKED);
	INT i(0);
	HTREEITEM hItem = NULL;

	hItem = m_teamTree.GetFirstVisibleItem();
	while(hItem != NULL)
	{
		m_teamTree.SetCheckState(hItem, bChecked);
		hItem = m_teamTree.GetNextVisibleItem(hItem);
	}
	
	return 0;	
}
LRESULT CImportXmlDlg::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
	if(!m_bImporting)
	{
		EndDialog(0);
	}
	return 0;
}
LRESULT CImportXmlDlg::OnCloseCmd(WORD, WORD, HWND, BOOL&)
{
	if(!m_bImporting)
	{
		EndDialog(0);
	}
	return 0;
}
LRESULT CImportXmlDlg::OnContactNotify(int, LPNMHDR pnmh, BOOL&)
{
	if(pnmh->code == NM_CLICK)
	{
		CPoint pt;
		GetCursorPos(&pt);
		m_teamTree.ScreenToClient(&pt);
		UINT nFlag(0);
		HTREEITEM hSel = m_teamTree.HitTest(pt, &nFlag);
		if(hSel != NULL)
		{
			m_teamTree.SelectItem(hSel);
		}
	}

	return 0;
}
LRESULT CImportXmlDlg::OnImport(WORD, WORD ,HWND, BOOL&)
{
	CSimpleArray<CString> arTeam;
	CString strName;
	
	HTREEITEM hItem = m_teamTree.GetFirstVisibleItem();
	while(hItem != NULL)
	{
		if(m_teamTree.GetCheckState(hItem))
		{
			m_teamTree.GetItemText(hItem, strName);
			arTeam.Add(strName);
		}
		hItem = m_teamTree.GetNextVisibleItem(hItem);
	}
	
	if(arTeam.GetSize() <= 0)
	{
		MessageBox(_T("请先选择你要导入的组!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}

	ImportFile(arTeam);

	return 0; 
}
LRESULT CImportXmlDlg::OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	CenterWindow();

	m_teamTree = GetDlgItem(IDC_TREE);
	HTREEITEM hPre = NULL;


	for(int i=0; i<m_arTeam.GetSize(); i++)
	{
		hPre = m_teamTree.InsertItem(m_arTeam[i], 0, 0, NULL, hPre);
	}

	return 0;
}
BOOL CImportXmlDlg::ParseXML(CString& strFile)
{
	MSXML3::IXMLDOMDocumentPtr oDoc;
	MSXML3::IXMLDOMNodeListPtr pNodes;
	MSXML3::IXMLDOMElementPtr pTeam;
	int nCount(0);

	BOOL bOK(FALSE);
	
	try
	{
		HRESULT hr = oDoc.CreateInstance(__uuidof(MSXML3::DOMDocument));
		if(FAILED(hr))
		{
			return bOK;
		}
		
		oDoc->async = VARIANT_FALSE;
		if(oDoc->load(_variant_t(strFile)) == VARIANT_FALSE)
		{
			return bOK;
		}
		
		pNodes = oDoc->selectNodes(_bstr_t(_T("/contactList/team")));
		int n = pNodes->length;
		CString strName;
		_variant_t vt;

		if(n >= 1)
		{
			for(int i=0; i<n; i++)
			{
				pTeam = pNodes->Getitem(i);	
				if(pTeam != NULL)
				{
					vt = pTeam->getAttribute(_bstr_t(_T("name")));
					if(vt.vt != VT_NULL)
					{
						strName = (LPCTSTR)_bstr_t(vt);
						m_arTeam.Add(strName);
						nCount++;
					}
				}
			}
		}
		
	}
	catch (...)
	{
		NULL;	
	}

	if(nCount > 0)
	{
		bOK = TRUE;
	}
	return bOK;
}

VOID CImportXmlDlg::ImportFile(CSimpleArray<CString>& arTeam)
{
	CContactManager manager;
	MSXML3::IXMLDOMDocumentPtr pDoc;
	MSXML3::IXMLDOMNodeListPtr pNodes;
	MSXML3::IXMLDOMElementPtr pContact;
	

	INT nTotal(0);
	CString strXPath;
	CProgressBarCtrl pb = GetDlgItem(IDC_PROGRESS);
	BOOL bNeedReset(FALSE);
	CButton btn1 = GetDlgItem(IDC_IMPORT);
	CButton btn2 = GetDlgItem(IDC_CLOSE);
	INT nImportContact(0);
	INT nTotalContact(0);
	BOOL bNeedReload(FALSE);
	INT nTeamId;
	
	try
	{	
		int i;
		HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML3::DOMDocument));
		pDoc->async = VARIANT_FALSE;
		if(pDoc->load(_variant_t(m_strFile)) != VARIANT_TRUE)
		{
			MessageBox(_T("导入失败，加载XML文件失败!"), _T("提示信息"), MB_OK|MB_ICONINFORMATION);
			return;
		}
		
		{
			CWaitCursor wait;
	
			for( i=0; i<arTeam.GetSize(); i++)
			{
				strXPath.Format(_T("/contactList/team[@name=\"%s\"]/contact"), arTeam[i]);
				pNodes = pDoc->selectNodes(_bstr_t(strXPath));
				nTotal += pNodes->length;
			}
			nTotalContact = nTotal;
			nTotal += arTeam.GetSize();

			pb.SetRange(0, nTotal);
			pb.SetPos(0);
			btn1.EnableWindow(FALSE);
			btn2.EnableWindow(FALSE);
			m_bImporting = TRUE;
			bNeedReset = TRUE;
			CString strName;
			CString strTel;
			_variant_t vt;
			CContact c;
			
			for( i=0; i<arTeam.GetSize(); i++)
			{
				strXPath.Format(_T("/contactList/team[@name=\"%s\"]/contact"), arTeam[i]);
				pNodes = pDoc->selectNodes(_bstr_t(strXPath));
				
				INT nID = manager.CheckTeamByName(arTeam[i]);
				pb.OffsetPos(1);
				if(nID != -1)
				{
					bNeedReload = TRUE;
					nTeamId = nID;
					c.m_nParentId = nID;
					c.m_bLeaf = TRUE;
					c.m_nId = 0;
					for(int j=0; j<pNodes->length; j++)
					{
						pContact = pNodes->Getitem(j);
						vt = pContact->getAttribute(_bstr_t(_T("name")));
						if(vt.vt == VT_NULL)
						{
							pb.OffsetPos(1);
							continue;
						}
						strName = (LPCTSTR)_bstr_t(vt);

						vt = pContact->getAttribute(_bstr_t(_T("tel")));
						if(vt.vt == VT_NULL)
						{
							pb.OffsetPos(1);
							continue;
						}
						strTel = (LPCTSTR)_bstr_t(vt);
						
						if(strName == _T("") || strTel == _T(""))
						{
							pb.OffsetPos(1);
							continue;
						}
						
						c.m_strName = strName;
						c.m_strTel = strTel;

						if(manager.AddContact(&c))
						{
							nImportContact++;
						}

						pb.OffsetPos(1);
					}
				}
			}
		}

	}
	catch (...)
	{
		NULL;
	}

	CString strText;
	if(bNeedReset)
	{
		pb.SetPos(nTotal);
		btn1.EnableWindow(TRUE);
		btn2.EnableWindow(TRUE);
		m_bImporting = FALSE;
	}

	strText.Format(_T("导入结果:%d条数据导入成功，%d条数据导入失败!"), nImportContact, nTotalContact-nImportContact);
	MessageBox(strText, _T("提示信息"), MB_OK|MB_ICONINFORMATION);
	
	if(bNeedReload)
	{
		 CContactManagerDlg* pMangerDlg = CContactManagerDlg::GetContactManagerDlg();
		 if(pMangerDlg != NULL && pMangerDlg->IsWindow())
		 {
			 pMangerDlg->ReloadData(0);
		 }
	}


}	