#include "StdAfx.h"
#include "SmsContactTree.h"
#include "ContactManager.h"
#include "resource.h"
#include "mainfrm.h"
#include "ContactManager.h"

CSmsContactTree::CSmsContactTree()
{
	m_pContactManager = new CContactManager;
	m_imagelist = NULL;
}
CSmsContactTree::~CSmsContactTree()
{
	if(m_pContactManager != NULL)
	{
		delete m_pContactManager;
		m_pContactManager = NULL;
	}
	ClearMap();
}
VOID CSmsContactTree::ClearMap()
{
	INT nCount = m_MapItem.GetSize();
	CContact* pContact;
	for(int i=0; i<nCount; i++)
	{
		pContact = m_MapItem.GetValueAt(i);
		if(pContact != NULL)
		{
			delete pContact;
			pContact = NULL;
		}
	}
	m_MapItem.RemoveAll();
}

LRESULT CSmsContactTree::OnCreate(UINT nMsgType, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}
LRESULT CSmsContactTree::OnDestroy(UINT nMsgType, WPARAM, LPARAM, BOOL& bHandled)
{
	if(m_imagelist != NULL)
	{
		m_imagelist.Destroy();
	}
	
	return 0;
}
BOOL CSmsContactTree::DeleteTeam(INT nId)
{
	CContact c;
	c.m_nId = nId;
	c.m_bLeaf = FALSE;
	if(m_pContactManager->DeleteContact(&c))
	{
		//InitContacts();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CSmsContactTree::ModifyContact(CContact& c)
{
	if(m_pContactManager->ModifyContact(&c))
	{

	//	InitContacts();
	
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CSmsContactTree::DeleteContact(INT nId)
{
	CContact c;
	c.m_nId = nId;
	c.m_bLeaf = TRUE;
	if(m_pContactManager->DeleteContact(&c))
	{
		//InitContacts();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CSmsContactTree::AddContact(CString& strName, CString& strTel, INT nParentId)
{
	CContact c;
	c.m_bLeaf = TRUE;
	c.m_nParentId = nParentId;
	c.m_strName = strName;
	c.m_strTel = strTel;
	if(m_pContactManager->AddContact(&c))
	{
		//InitContacts();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CSmsContactTree::ModifyTeam(INT nId, CString& strName)
{
	CContact c;
	c.m_nId = nId;
	c.m_bLeaf = FALSE;
	c.m_nParentId = 1;
	c.m_strName = strName;
	c.m_strTel = _T("");
	if(m_pContactManager->ModifyContact(&c))
	{
		//InitContacts();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
VOID CSmsContactTree::SelectItemById(INT nId)
{
	CContact* pContact = NULL;
	HTREEITEM hItem = NULL;
	for(INT i=0; i<m_MapItem.GetSize(); i++)
	{
		pContact = m_MapItem.GetValueAt(i);
		if((pContact->m_nId == nId) && !pContact->m_bLeaf)
		{
			hItem = m_MapItem.GetKeyAt(i);
			break;
		}
	}
	if(hItem != NULL)
	{
		SelectItem(hItem);
	}
}
BOOL CSmsContactTree::AddTeam(CString& strName)
{
	CContact c;
	c.m_bLeaf = FALSE;
	c.m_nParentId = 1;
	c.m_strName = strName;
	c.m_strTel = _T("");
	if(m_pContactManager->AddContact(&c))
	{
		//InitContacts();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
VOID CSmsContactTree::InitContacts()
{
	if( m_imagelist == NULL)
	{
		//m_imagelist.Create(IDB_SMS_CONTACT,32,0,RGB(255, 0, 255));
		m_imagelist.Create(32, 32, ILC_COLOR24|ILC_MASK, 3, 0);
		HBITMAP hBmp = LoadBitmap(CMainFrame::GetResourceModule(), MAKEINTRESOURCE(IDB_SMS_CONTACT));
		if(hBmp != NULL)
		{
			m_imagelist.Add(hBmp, RGB(255,0,255));
			DeleteObject(hBmp);
		}

	}
	
	DeleteAllItems();
	ClearMap();
	
	DWORD dwStyle = GetWindowLong(GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS ;
	SetWindowLong(GWL_STYLE,dwStyle);
	SetImageList(m_imagelist,TVSIL_NORMAL);
	

	CContact* pContact = NULL;
	HTREEITEM hItem = NULL;

	//add team
	pContact = m_pContactManager->GetFirstTeam();
	while(pContact != NULL)
	{
		hItem = InsertItem(pContact->m_strName, 0, 1, NULL, hItem);
		m_MapItem.Add(hItem, new CContact(*pContact));

		pContact = m_pContactManager->GetNextTeam();
	}

	//add member
	hItem = GetChildItem(NULL);
	while(hItem != NULL)
	{
		CContact* pTeam = m_MapItem.Lookup(hItem);
		if(pTeam != NULL)
		{
			pContact = m_pContactManager->GetFirstContact(pTeam->m_nId);
			HTREEITEM itemTemp = NULL;
			while(pContact != NULL)
			{
				itemTemp = InsertItem(pContact->m_strName, 2, 2, hItem, itemTemp);
				m_MapItem.Add(itemTemp, new CContact(*pContact));			
				pContact = m_pContactManager->GetNextContact();
			}
		}

		hItem = GetNextSiblingItem(hItem);
	}
}