#include "StdAfx.h"
#include "userManager.h"
#include "DBOperate.h"

CSmsUser::CSmsUser():m_strName(_T("")),m_strPwd(_T("")),m_strServer(_T(""))
{
	m_nPort = 0;
	m_nId = 0;
	m_bTipOn = TRUE;
	m_bAutoConnect = FALSE;
}
CSmsUser::CSmsUser(const CSmsUser& user):
	m_strName(user.m_strName),m_strPwd(user.m_strPwd),m_strServer(user.m_strServer)
{
	m_nPort = user.m_nPort;
	m_nId = user.m_nId;
	m_bTipOn = user.m_bTipOn;
	m_bAutoConnect = user.m_bAutoConnect;
}

CUserManager::CUserManager()
{
	m_pCurrentUser = new CSmsUser;
	m_bLoaded = FALSE;
}
CUserManager::~CUserManager()
{
	if(m_pCurrentUser != NULL)
	{
		delete m_pCurrentUser;
		m_pCurrentUser = NULL;
	}
}
BOOL CUserManager::SaveCurrentUser()
{
	BOOL bOK(FALSE);
	CString strSQL;
	_RecordsetPtr pRst = NULL;
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	
	if(m_bLoaded)
	{
		strSQL.Format(_T("select top 1 * from userList where userId = %u"), m_pCurrentUser->m_nId);
	}
	else
	{
		strSQL = _T("select top 1 * from userList");
	}
	
	if(pDB != NULL)
	{
		BOOL b = pDB->GetRecordset(strSQL, &pRst);
		if(b)
		{
			try
			{
				if(pRst->GetADOEOF())
				{
					pRst->AddNew();
				}

				pRst->PutCollect(_T("name"), _variant_t(m_pCurrentUser->m_strName));
				pRst->PutCollect(_T("password"), _variant_t(m_pCurrentUser->m_strPwd));
				pRst->PutCollect(_T("isAutoConnect"), (_variant_t)long(m_pCurrentUser->m_bAutoConnect));
				pRst->PutCollect(_T("isTipOn"), (_variant_t)long(m_pCurrentUser->m_bTipOn));
				pRst->PutCollect(_T("serverIP"), _variant_t(m_pCurrentUser->m_strServer));
				pRst->PutCollect(_T("port"), (_variant_t)long(m_pCurrentUser->m_nPort));

				pRst->Update();
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

CSmsUser* CUserManager::GetCurrentUser(BOOL& bExist)
{
	bExist = FALSE;

	if(m_bLoaded) 
	{
		bExist = TRUE;
		return m_pCurrentUser;
	}

	LPCTSTR pszSQL = _T("select top 1 * from userList");
	_RecordsetPtr pRst = NULL;
	CDBOperate* pDB = CSmsDB::GetDBOperate();
	if(pDB != NULL)
	{	
		BOOL b = pDB->GetRecordset(pszSQL, &pRst);
		try
		{
			if(b)
			{
				_variant_t vt;
	
				if(!pRst->GetADOEOF())
				{
					vt = pRst->GetCollect(_T("userId"));
					if(vt.vt != VT_NULL)
					{
						m_pCurrentUser->m_nId = (long)vt;
					}
					
					vt = pRst->GetCollect(_T("name"));
					if(vt.vt != VT_NULL)
					{
						m_pCurrentUser->m_strName = (LPCTSTR)_bstr_t(vt);
					}
					
					vt = pRst->GetCollect(_T("password"));
					if(vt.vt != VT_NULL)
					{
						m_pCurrentUser->m_strPwd = (LPCTSTR)_bstr_t(vt);
					}
					
					vt = pRst->GetCollect(_T("isAutoConnect"));
					if(vt.vt != VT_NULL)
					{
						m_pCurrentUser->m_bAutoConnect = (BOOL)(long)vt;
					}

					vt = pRst->GetCollect(_T("isTipOn"));
					if(vt.vt != VT_NULL)
					{
						m_pCurrentUser->m_bTipOn = (BOOL)(long)vt;
					}
					
					vt = pRst->GetCollect(_T("serverIP"));
					if(vt.vt != VT_NULL)
					{
						m_pCurrentUser->m_strServer = (LPCTSTR)_bstr_t(vt);
					}

					vt = pRst->GetCollect(_T("port"));
					if(vt.vt != VT_NULL)
					{
						m_pCurrentUser->m_nPort = (long)vt;
					}
					
					m_bLoaded = TRUE;
				}

				pRst->Close();
				
			}
		}
		catch (...)
		{
			NULL;
		}
	}
	
	bExist = m_bLoaded;
	return m_pCurrentUser;

}