#pragma  once 

#include <atlmisc.h>
#include "SmsMessage.h"

class CSmsAccount;

// class CSmsGuard
// {
// public:
// 	CSmsGuard(HANDLE hEvent)
// 	{
// 		_ASSERT(hEvent != NULL);
// 		m_hEvent = hEvent;
// 		DWORD dwRet = ::WaitForSingleObject(hEvent,60000);
// 		if(dwRet == WAIT_OBJECT_0)
// 		{
// 			::SetEvent(hEvent);
// 		}
// 		else
// 		{
// 			OutputDebugString(_T("CSmsGuard wait timeout"));
// 		}
// 	
// 	}
// 	~CSmsGuard()
// 	{
// 		::ResetEvent(m_hEvent)
// 	}
// private:
// 	HANDLE m_hEvent;
// };
// 
// class CAccountFile
// {
// public:
// 
// #pragma  pack(push, 1)
// 	struct FileHead
// 	{
// 		char szTag[12];
// 		INT  nVersion;
// 		INT nCurrentId;
// 		INT bNeedLogon;
// 		INT nSendedCount;
// 		INT nReceivedCount;
// 		INT nCount;
// 
// 		FileHead()
// 		{
// 			memset(szTag, 0, sizeof(szTag));
// 			INT nVersion = 1;
// 			nCurrentId = 0;
// 			bNeedLogon = TRUE;
// 			nSendedCount = 0;
// 			nReceivedCount = 0;
// 			nCount = 0;
// 		}
// 	};
// 
// 	struct SmsIndex
// 	{	
// 		INT nIndex;
// 		INT nId;
// 		INT nStartData;
// 		INT nIsDeleted;
// 
// 		SmsIndex()
// 		{
// 			nIndex = -1;
// 			nId = -1;
// 			nStartData = -1;
// 			nIsDeleted = TRUE;
// 		}
// 	};
// 
// 	struct FileAccount
// 	{
// 		INT nIsLogoned;
// 		char szName[100];
// 		char szPassword[100];
// 		INT nIsDispatch;
// 		INT nIsCanUse;
// 		INT nSendedCount;
// 
// 		FileAccount()
// 		{
// 			memset(szName, 0, sizeof(szName));
// 			memset(szPassword, 0, sizeof(szPassword));
// 			nIsLogoned = FALSE;
// 			nIsDispatch = TRUE;
// 			nIsCanUse = TRUE;
// 			nSendedCount = 0;
// 		}
// 	};
// #pragma  pack(pop)
// 	
// 	enum
// 	{
// 		account_head_size = sizeof(FileHead),
// 		account_index_size = sizeof(SmsIndex),
// 		account_account_size = sizeof(FileAccount),
// 		account_count = 200
// 	};
// 
// 	CAccountFile()
// 	{
// 		m_hEvent = NULL;
// 		m_hFile = NULL;
// 		m_bInited = FALSE;
// 	}
// 	~CAccountFile() {Release();}
// 	BOOL LoadAccounts()
// 	{
// 		if(!m_bInited && !Init())
// 		{
// 			return FALSE;
// 		}
// 		
// 		CSmsGuard  guard(m_hEvent);
// 		INT nRead;
// 		BOOL bRet = ReadFile(m_hFile, &m_Head, account_head_size, &nRead, NULL);
// 		if(!bRet || (nRead != account_head_size)) return FALSE;
// 		
// 		const DWORD dwIndexSize = account_index_size * account_count;
// 		const DWORD dwDataStart = account_head_size + dwIndexSize;
// 		
// 		BYTE* pIndexBuf = new BYTE[dwIndexSize];
// 		bRet = ReadFile(m_hFile, pIndexBuf, dwIndexSize, nRead, NULL);
// 		if(!bRet || (nRead != dwIndexSize)) return FALSE;
// 
// 		INT nCount = m_Head.nCount;
// 		SmsIndex* pIndex = NULL;
// 		INT nFound(0);
// 		BYTE* p = pIndexBuf;
// 		FileAccount account;
// 		for(int i=0; (i<account_count) && (nFound<nCount); i++)
// 		{
// 				pIndex = (SmsIndex*)p;
// 				if(pIndex->nIsDeleted == FALSE)
// 				{
// 					INT nStartData = pIndex->nStartData;
// 
// 				}
// 
// 				p += account_index_size;
// 		}	
// 		return TRUE;
// 	}
// 	BOOL SaveAccounts()
// 	{
// 
// 	}
// 	
// 
// 	BOOL Init()
// 	{
// 		TCHAR szBuf[MAX_PATH] = {0};
// 		GetAccountFile(szBuf, MAX_PATH);
// 		m_hFile = CreateFile(szBuf, FILE_ALL_ACCESS,
// 			FILE_SHARE_READ|FILE_SHARE_WRITE,
// 			NULL, OPEN_ALWAYS, 0, NULL);
// 		if(INVALID_HANDLE_VALUE == m_hFile) 
// 		{
// 			Release();
// 			return FALSE;	
// 		}
// 		m_hEvent = CreateEvent(NULL, TRUE, FALSE, _T("sms_account_file"));
// 		if(m_hEvent == NULL) 
// 		{
// 			Release();
// 			return FALSE;
// 		}
// 		m_bInited = TRUE;
// 		return TRUE;
// 	}
// 	VOID Release()
// 	{
// 		RemoveAll();
// 		if(m_hEvent != NULL)
// 		{
// 			CloseHandle(m_hEvent);
// 			m_hEvent = NULL;
// 		}
// 		if(m_hFile != INVALID_HANDLE_VALUE)
// 		{
// 			CloseHandle(m_hFile);
// 			m_hFile = NULL;
// 		}
// 		m_bInited = FALSE;
// 	}
// 
// protected:
// 	VOID RemoveAll()
// 	{
// 		SmsIndex* pIndex = NULL;
// 		CSmsAccount* pAccount = NULL;
// 		for(INT i=0; i<m_mapAccounts.GetSize(); i++)
// 		{
// 			pIndex = m_mapAccounts.GetKeyAt(i);
// 			pAccount = m_mapAccounts.GetValueAt(i);
// 			if(pIndex != NULL)
// 			{
// 				delete pIndex;
// 				pIndex = NULL;
// 			}
// 			if(pAccount != NULL)
// 			{
// 				delete pAccount;
// 				pAccount = NULL;
// 			}
// 		}
// 		m_mapAccounts.RemoveAll();
// 	}
// 	VOID GetAccountFile(TCHAR* szBuf, INT nLen)
// 	{	
// 		HMODULE hMod = GetModuleHandle();
// 		if(hMod != NULL)
// 		{
// 			GetModuleFileName(hMod, szBuf, nLen);
// 			TCHAR* p = _tcschr(szBuf, '\\');
// 			*(++p) = '\0';
// 			lstrcpy(szBuf, _T("account.db"));
// 		}
// 	}
// private:
// 	CSimpleMap<SmsIndex*, CSmsAccount*> m_mapAccounts;
// 	FileHead m_Head;
// 	HANDLE	m_hEvent;
// 	HANDLE m_hFile;
// 	BOOL m_bInited;
// };

class CSmsAccount
{
public:
	CSmsAccount(LPCTSTR szName,LPCTSTR szPassword,BOOL bCanUse,BOOL bDispatch)
	:m_strName(szName),m_strPassword(szPassword)
	{
		m_bCanUse = bCanUse;
		m_bDispatch = bDispatch;
		m_bLogoned = FALSE;		
	}

public:
	friend class CSmsAccountManager;

	BOOL m_bLogoned;
	CString m_strName;
	CString m_strPassword;
	GUID m_clientId;
	BOOL m_bDispatch;
	BOOL m_bCanUse;
};

class CSmsAccountManager
{
public:
	CSmsAccountManager();
	~CSmsAccountManager();
	BOOL LoadAccounts();
	BOOL SaveAccounts();
	
	BOOL RemoveAccount(const CString& strName);
	BOOL AddAccount(const CString& strName, const CString& strPwd, BOOL bCanUse, BOOL bDispatch);
	BOOL ModifyAccount(CString& strOldName, CString& strName, CString& strPwd, BOOL bCanUse, BOOL bDispatch);
	VOID LogoutAccount(GUID& id);
	BOOL CheckCanLogon(CSMSMessage* pMsg);
	
	INT GetLogonCount();
	BOOL GetIsNeedLogon() {return m_bNeedLogon;}
	VOID SetIsNeedLogon(BOOL b) { m_bNeedLogon = b;}

	CSmsAccount* GetFirst() ;
	CSmsAccount* GetNext();
	CSmsAccountManager& operator = (const CSmsAccountManager& manager);
private:
	VOID RemoveAll();
	VOID GetAccountFileName(LPTSTR sBuf, INT nBufLen);
private:
	CSmsAccountManager(const CSmsAccountManager&);
	CSimpleArray<CSmsAccount*> m_arrAccounts;
	BOOL m_bNeedLogon;
	
	INT m_nCurrentIndex;

#pragma pack(push,1)
	struct FileHeader
	{
		TCHAR szTag[11];
		INT nCount;
		INT bNeedLogon;
	};

	struct Account 
	{
		Account()
		{
			memset(szName,0,sizeof(szName));
			memset(szPassword,0,sizeof(szPassword));
		}
		TCHAR szName[100];
		TCHAR szPassword[100];
		BOOL  bCanUse;
		BOOL  bDispatch;
	};
#pragma pack(pop)

};

// class CSmsDBAccountManager
// {
// public:
// 	CSmsDBAccountManager();
// 	~CSmsDBAccountManager();
// 
// private:
// 	RemoveAll();
// 
// private:
// 	CSmsDBAccountManager<CSmsAccount*> m_arrAccounts;
// 	BOOL m_bNeedLogon;
// 	INT	 m_nReceivedCount;
// 	INT  m_nSendedCount;
// 
// 	INT m_nCurrentIndex;
//};