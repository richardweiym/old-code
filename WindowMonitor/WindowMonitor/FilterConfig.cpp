#include "stdafx.h"
#include "FilterConfig.h"

CFilterConfig CFilterConfig::s_filterConfig;

CFilterConfig::CFilterConfig()
:m_bTopMost(FALSE), m_bVisible(FALSE), m_bLayered(FALSE),
m_strClassName(_T("")), m_strProcessID(_T("0")), m_strText(_T(""))
{
	m_eMothod = ENUM_METHOD_FINDWINDOW;
}

CFilterConfig::~CFilterConfig()
{

}

CFilterConfig& CFilterConfig::GetInstance()
{
	return s_filterConfig;
}

VOID CFilterConfig::SetProcessID(LPCTSTR lpszID)
{
	if(lpszID == NULL) lpszID = _T("");
	m_strProcessID = lpszID;
}

VOID CFilterConfig::SetClassName(LPCTSTR lpszClass)
{
	if(lpszClass == NULL) lpszClass = _T("");
	m_strClassName = lpszClass;
}

VOID CFilterConfig::SetText(LPCTSTR lpszText)
{
	if(lpszText == NULL) lpszText = _T("");
	m_strText = lpszText;
}

VOID CFilterConfig::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
}

VOID CFilterConfig::SetLayered(BOOL bLayered)
{
	m_bLayered = bLayered;
}

VOID CFilterConfig::SetTopMost(BOOL bTopMost)
{
	m_bTopMost = bTopMost;
}

LPCTSTR CFilterConfig::GetProcessID() const
{
	return m_strProcessID.c_str();
}

LPCTSTR CFilterConfig::GetClassName() const
{
	return m_strClassName.c_str();
}

LPCTSTR CFilterConfig::GetText() const
{
	return m_strText.c_str();
}

BOOL CFilterConfig::GetVisible() const
{
	return m_bVisible;
}

BOOL CFilterConfig::GetLayered() const
{
	return m_bLayered;
}

BOOL CFilterConfig::GetTopMost() const
{
	return m_bTopMost;
}


VOID CFilterConfig::SetDwordProcessID(DWORD dwProcessID)
{
	TCHAR szBuffer[32] = {0};
	_itot(dwProcessID, szBuffer, 10);
	m_strProcessID = szBuffer;
}

DWORD CFilterConfig::GetDwordProcessID() const 
{
	return _ttoi(m_strProcessID.c_str());
}

VOID CFilterConfig::SetEnumMethod(ENUM_METHOD eMethod)
{
	m_eMothod = eMethod;
}

ENUM_METHOD CFilterConfig::GetEnumMethod() const
{
	return m_eMothod;
}