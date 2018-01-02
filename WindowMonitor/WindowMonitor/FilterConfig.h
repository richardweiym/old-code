#ifndef __FILTER_CONFIG_H__
#define __FILTER_CONFIG_H__

#include <string>

enum ENUM_METHOD
{
	ENUM_METHOD_FINDWINDOW,
	ENUM_METHOD_ENUMWINDOW
};

class CFilterConfig
{
public:
	static CFilterConfig& GetInstance();

	VOID SetProcessID(LPCTSTR lpszID);
	VOID SetDwordProcessID(DWORD dwProcessID);
	VOID SetClassName(LPCTSTR lpszClass);
	VOID SetText(LPCTSTR lpszText);
	VOID SetVisible(BOOL bVisible);
	VOID SetLayered(BOOL bLayered);
	VOID SetTopMost(BOOL bTopMost);
	VOID SetEnumMethod(ENUM_METHOD eMethod);
	
	LPCTSTR GetProcessID() const;
	DWORD GetDwordProcessID() const;
	LPCTSTR GetClassName() const;
	LPCTSTR GetText() const;
	BOOL GetVisible() const;
	BOOL GetLayered() const;
	BOOL GetTopMost() const;
	ENUM_METHOD GetEnumMethod() const;

private:
	CFilterConfig();
	~CFilterConfig();
	CFilterConfig(const CFilterConfig&);
	CFilterConfig& operator = (const CFilterConfig&);

private:
	std::wstring m_strProcessID;
	std::wstring m_strClassName;
	std::wstring m_strText;
	BOOL m_bLayered;
	BOOL m_bTopMost;
	BOOL m_bVisible;
	ENUM_METHOD m_eMothod;

	static CFilterConfig s_filterConfig;
};

#endif //__FILTER_CONFIG_H__