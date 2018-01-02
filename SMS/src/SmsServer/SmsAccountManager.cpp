#include "stdafx.h"
#include "SmsAccountManager.h"


CSmsAccountManager::CSmsAccountManager()
{
	m_bNeedLogon = TRUE;
}

CSmsAccountManager::~CSmsAccountManager()
{
	RemoveAll();
}
CSmsAccountManager& CSmsAccountManager::operator = (const CSmsAccountManager& m)
{
	RemoveAll();
	m_bNeedLogon = m.m_bNeedLogon;
	m_nCurrentIndex = m.m_nCurrentIndex;

	CSmsAccount* pAccount;
	for( INT i = 0; i < m.m_arrAccounts.GetSize(); i++)
	{
		pAccount = new CSmsAccount(*(m.m_arrAccounts[i]));
		m_arrAccounts.Add(pAccount);
	}

	return *this;
}
CSmsAccount* CSmsAccountManager::GetFirst()
{
	m_nCurrentIndex = 0;
	if(m_arrAccounts.GetSize() > 0)
	{
		return m_arrAccounts[0];
	}
	else
	{
		return NULL;
	}
}
CSmsAccount* CSmsAccountManager::GetNext()
{
	m_nCurrentIndex++;
	if(m_arrAccounts.GetSize() <= m_nCurrentIndex)
	{
		return NULL;
	}
	else
	{
		return m_arrAccounts[m_nCurrentIndex];
	}
}

VOID CSmsAccountManager::RemoveAll()
{
	CSmsAccount* p;
	for(INT i=0; i < m_arrAccounts.GetSize();i++)
	{
		p = m_arrAccounts[i];
		delete p;
	}
	m_arrAccounts.RemoveAll();
}
VOID CSmsAccountManager::LogoutAccount(GUID& id)
{
	CSmsAccount* p;

	for(int i=0; i<m_arrAccounts.GetSize();i++)
	{
		p = m_arrAccounts[i];
		if(p->m_clientId == id)
		{
			p->m_bLogoned = FALSE;
			break;
		}
	}

	return;
}
BOOL CSmsAccountManager::CheckCanLogon(CSMSMessage* pMsg)
{
	CSmsAccount* p;
	INT nRight(0);
	
	for(int i=0 ;i<m_arrAccounts.GetSize();i++)
	{
		p = m_arrAccounts[i];
		if((p->m_strName == pMsg->GetStr1()) && (p->m_strPassword == pMsg->GetStr2()) && p->m_bCanUse)
		{
			nRight |= SMS_MESSAGE_LOGON_OK_FLAG;

			if(p->m_bDispatch)
			{
				nRight |= SMS_MESSAGE_DISPATCH_RIGHT_FLAG;
			}
			
			if(p->m_bLogoned)
			{
				nRight |= SMS_MESSAGE_ACCOUNT_USED_FLAG;
			}
			else
			{
				
				p->m_bLogoned = TRUE;
				p->m_clientId = pMsg->GetClientID();
			}
			break;
		}
	}

	if(!m_bNeedLogon)
	{
		nRight = SMS_MESSAGE_LOGON_OK_FLAG | SMS_MESSAGE_DISPATCH_RIGHT_FLAG;
	}

	pMsg->SetResult(nRight);

	return (nRight & SMS_MESSAGE_LOGON_OK_FLAG) && (!(nRight & SMS_MESSAGE_ACCOUNT_USED_FLAG));
}
BOOL CSmsAccountManager::RemoveAccount(const CString& strName)
{
	BOOL bFind(FALSE);

	CSmsAccount* pAccount;

	for(INT i = 0; i < m_arrAccounts.GetSize(); i++)
	{
		pAccount = m_arrAccounts[i];

		if(pAccount->m_strName == strName)
		{
			bFind = TRUE;
			break;
		}
	}
	
	if(bFind)
	{
		m_arrAccounts.Remove(pAccount);
		delete pAccount;
	}

	return bFind;
}
BOOL CSmsAccountManager::ModifyAccount(CString& strOldName, CString& strName, CString& strPwd, BOOL bCanUse, BOOL bDispatch)
{
	CSmsAccount* pOldAccount = NULL;
	BOOL bFind(FALSE);
	BOOL bOK(FALSE);

	for(INT i = 0; i < m_arrAccounts.GetSize(); i++)
	{
		pOldAccount = m_arrAccounts[i];
		if(pOldAccount->m_strName == strOldName)
		{
			bFind = TRUE;
			break;
		}
	}
	
	if(bFind && (pOldAccount != NULL))
	{
		bFind = FALSE;
		CSmsAccount* pAccount = NULL;
		for(INT i = 0; i < m_arrAccounts.GetSize(); i++)
		{
			pAccount = m_arrAccounts[i];
			if((pAccount->m_strName == strName) && (pAccount != pOldAccount))
			{
				bFind = TRUE;
				break;
			}
		}
	}
	
	if(!bFind)
	{
		pOldAccount->m_strName = strName;
		pOldAccount->m_strPassword = strPwd;
		pOldAccount->m_bCanUse = bCanUse;
		pOldAccount->m_bDispatch = bDispatch;
		bOK = TRUE;
	}

	return bOK;

}
BOOL CSmsAccountManager::AddAccount(const CString& strName, const CString& strPwd, BOOL bCanUse, BOOL bDispatch)
{
	CSmsAccount* pAccount;
	BOOL bFind(FALSE);

	for(INT i = 0; i < m_arrAccounts.GetSize(); i++)
	{
		pAccount = m_arrAccounts[i];
		if(pAccount->m_strName == strName)
		{
			bFind = TRUE;
			break;
		}
	}

	if(!bFind)
	{
		pAccount = new CSmsAccount(strName, strPwd, bCanUse, bDispatch);
		m_arrAccounts.Add(pAccount);
	}
	return !bFind;
}
VOID CSmsAccountManager::GetAccountFileName(LPTSTR sBuf, INT nBufLen)
{
	GetModuleFileName(GetModuleHandle(NULL),sBuf,MAX_PATH);
	TCHAR* p = _tcsrchr(sBuf,'\\');
	*(++p) = 0;
	lstrcat(sBuf,_T("SmsAccount.db"));
}

BOOL CSmsAccountManager::SaveAccounts()
{
	TCHAR sFileName[MAX_PATH] = {0};
	GetAccountFileName(sFileName, MAX_PATH);
	DWORD nWritten;

	HANDLE hFile = CreateFile(sFileName,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,0,0);
	if(hFile == INVALID_HANDLE_VALUE) return FALSE;
	
	BOOL bOK(TRUE);
	FileHeader header;
	lstrcpy(header.szTag,_T("SMSACCOUNT"));
	header.nCount = m_arrAccounts.m_nSize;
	header.bNeedLogon = m_bNeedLogon;
	
	if(!WriteFile(hFile,&header,sizeof(header),&nWritten,NULL))
	{
		bOK = FALSE;
	}
	if(nWritten != sizeof(header))
	{
		bOK = FALSE;
	}
	
	if(bOK)
	{
		Account account;
		CSmsAccount* pA;
		for(int i=0 ;i<m_arrAccounts.m_nSize;i++)
		{
			pA=m_arrAccounts[i];
			lstrcpyn(account.szName,pA->m_strName,99);
			lstrcpyn(account.szPassword,pA->m_strPassword,99);
			account.bCanUse = pA->m_bCanUse;
			account.bDispatch = pA->m_bDispatch;
			
			if(!WriteFile(hFile,&account,sizeof(account),&nWritten,NULL))
			{
				bOK = FALSE;
				break;
			}
			if(nWritten != sizeof(account))
			{
				bOK = FALSE;
				break;
			}
		}
	}
	CloseHandle(hFile);

	return  bOK;
}
BOOL CSmsAccountManager::LoadAccounts()
{
	RemoveAll();

	TCHAR sFileName[MAX_PATH] = {0};
	GetAccountFileName(sFileName, MAX_PATH);
	DWORD nReaded;

	HANDLE hFile = CreateFile(sFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFile == INVALID_HANDLE_VALUE) return FALSE;
	
	BOOL bOK = TRUE;
	FileHeader header;
	if(!ReadFile(hFile,&header,sizeof(header),&nReaded,NULL))
	{
		bOK = FALSE;
	}
	if(nReaded != sizeof(header))
	{
		bOK = FALSE;
	}
	
	if(lstrcmpi(header.szTag,_T("SMSACCOUNT"))!=0) 		
	{
		bOK = FALSE;
	}
	if(bOK)
	{
		INT nCount = header.nCount;
		m_bNeedLogon = header.bNeedLogon;
		
		Account account;
		CSmsAccount* pA;
		for (int i=0 ;i<nCount;i++)
		{
			if(ReadFile(hFile,&account,sizeof(account),&nReaded,NULL))
			{
				if(nReaded == sizeof(Account))
				{
					pA = new CSmsAccount(account.szName,account.szPassword,account.bCanUse,account.bDispatch);	
					m_arrAccounts.Add(pA);
				}
				else
				{
					bOK = FALSE;
					break;
				}

			}
			else
			{
				bOK = FALSE;
				break;
			}
		}
	}


	CloseHandle(hFile);

	return bOK;
}

INT CSmsAccountManager::GetLogonCount()
{
	CSmsAccount* pAccount = NULL;
	INT nCount(0);
	for(INT i = 0; i < m_arrAccounts.GetSize(); i++)
	{
		pAccount = m_arrAccounts[i];
		if(pAccount != NULL && pAccount->m_bLogoned)
		{
			nCount++;
		}
	}

	return nCount;
}