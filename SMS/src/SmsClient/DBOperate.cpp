#include "StdAfx.h"
#include "DBOperate.h"

CDBOperate::CDBOperate()
{
	m_pDBCon = NULL;
}
CDBOperate::~CDBOperate()
{
	Close();
}
BOOL CDBOperate::Open(LPCTSTR szConString)
{
	HRESULT hr;
	BOOL bOK(FALSE);
	if(IsOPen())
	{
		Close();
	}
	
	try
	{
		if(m_pDBCon == NULL)
		{
			hr = m_pDBCon.CreateInstance(__uuidof(Connection));
			if(FAILED(hr))
			{
				goto __Open_END;
			}
		}
		m_pDBCon->PutConnectionTimeout(long(5));
		m_pDBCon->PutCursorLocation(adUseClient);
		hr = m_pDBCon->Open((_bstr_t)szConString, _T(""), _T(""), -1);
		if(SUCCEEDED(hr))
		{
			bOK = TRUE; 
		}
	}
	catch (...)
	{
		goto __Open_END;
	}
	
	return bOK;

__Open_END:
	Close();
	return bOK;
}
BOOL CDBOperate::IsOPen()
{
	BOOL bOpen(FALSE);
	
	try
	{
		if(m_pDBCon != NULL && (m_pDBCon->State == adStateOpen))
		{
			bOpen = TRUE;
		}
	}
	catch (...)
	{
		NULL;
	}

	return bOpen;
}
VOID CDBOperate::Close()
{
	try
	{
		if(m_pDBCon != NULL)
		{
			if(m_pDBCon->State == adStateOpen)
			{
				m_pDBCon->Close();
			}
			m_pDBCon = NULL;
		}
	}
	catch (...)
	{	
		NULL;
	}
}
BOOL CDBOperate::ExecuteSQL(LPCTSTR szSQL)
{
	BOOL bOK(FALSE);

	if(IsOPen())
	{
		try
		{
			m_pDBCon->Execute(_bstr_t(szSQL), NULL, adExecuteNoRecords);
			bOK = TRUE;
		}
		catch (...)
		{
			NULL;
		}
	}
	return bOK;
}
BOOL CDBOperate::GetRecordset(LPCTSTR szSQL, _Recordset** ppRst)
{
	BOOL bOK(FALSE);
	if(IsOPen())
	{
		try
		{
			HRESULT hr;
			_RecordsetPtr pRs = NULL;
			hr = pRs.CreateInstance(__uuidof(Recordset));
			if(SUCCEEDED(hr))
			{
				pRs->CursorLocation = adUseClient;
				hr = pRs->Open(_variant_t(szSQL),m_pDBCon.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
				if(SUCCEEDED(hr))
				{
					*(ppRst) = pRs; 
					pRs.Detach();
					bOK =TRUE;
				}
			}
		}
		catch (...)
		{
			NULL;
		}
	}
	return bOK;
}


CDBOperate CSmsDB::s_DBOperate;
CDBOperate* CSmsDB::GetDBOperate()
{
	BOOL bOK(TRUE);

	if(!s_DBOperate.IsOPen())
	{
		TCHAR szPath[MAX_PATH] = {0};
		TCHAR szConString[1024] = {0};

		GetModuleFileName(GetModuleHandle(NULL), szPath, MAX_PATH);
		TCHAR* p = _tcsrchr(szPath, '\\');
		*(++p) = '\0';
		lstrcat(szPath, _T("sms.dat"));

		_sntprintf(szConString, 1024, _T("Provider=Microsoft.Jet.OLEDB.4.0;Data source=%s; \
			Persist Security Info=False;Jet OLEDB:Database Password = guhu_55841310;"),szPath);

		if(!s_DBOperate.Open(szConString))
		{
			bOK	= FALSE;
		}
	}
	
	if(bOK)
	{
		return &s_DBOperate;
	}
	else
	{
		return NULL;
	}
}

// CDBOperate CSmsServerDB::GetDBOperate();
// CDBOperate* CSmsServerDB::GetDBOperate()
// {
// 	BOOL bOK(TRUE);
// 
// 	if(!s_DBOperate.IsOPen())
// 	{
// 		TCHAR szPath[MAX_PATH] = {0};
// 		TCHAR szConString[1024] = {0};
// 
// 		GetModuleFileName(GetModuleHandle(NULL), szPath, MAX_PATH);
// 		TCHAR* p = _tcsrchr(szPath, '\\');
// 		*(++p) = '\0';
// 		lstrcat(szPath, _T("smsAccount.mdb"));
// 
// 		_sntprintf(szConString, 1024, _T("Provider=Microsoft.Jet.OLEDB.4.0;Data source=%s"),szPath);
// 
// 		if(!s_DBOperate.Open(szConString))
// 		{
// 			bOK	= FALSE;
// 		}
// 	}
// 	
// 	if(bOK)
// 	{
// 		return &s_DBOperate;
// 	}
// 	else
// 	{
// 		return NULL;
// 	}
//}