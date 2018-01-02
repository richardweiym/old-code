#include "stdafx.h"
#include "WinEnum.h"
#include "FilterConfig.h"

CWinEnum::CWinEnum()
:m_pEnumSink(NULL)
{

}

CWinEnum::~CWinEnum()
{

}

VOID CWinEnum::SetEnumSink(IWinEnumEventSink* pSink)
{
	m_pEnumSink = pSink;
}

VOID EnumWithFindWindow(std::vector<HWND>* pArList)
{
	HWND hWndPrev = NULL;
	
	do 
	{	
		hWndPrev = FindWindowEx(NULL, hWndPrev, NULL, NULL);
		if(hWndPrev != NULL)
		{
			pArList->push_back(hWndPrev);
		}
	} while (hWndPrev != NULL);
}

BOOL CALLBACK EnumWindowCallback(HWND hwnd,LPARAM lParam)
{
	std::vector<HWND>* pArList = (std::vector<HWND>*)lParam;
	pArList->push_back(hwnd);

	return TRUE;
}


BOOL CWinEnum::StartEnum()
{	
	m_arWinList.clear();

	if(m_pEnumSink == NULL) return FALSE;
	
	m_pEnumSink->OnEnumStart();

	if(CFilterConfig::GetInstance().GetEnumMethod() == ENUM_METHOD_FINDWINDOW)
	{
		EnumWithFindWindow(&m_arWinList);
	}
	else
	{
		EnumWindows(&EnumWindowCallback, (LPARAM)&m_arWinList);
	}

	size_t nCount = m_arWinList.size();
	for(INT i=0; i<nCount; ++i)
	{
		m_pEnumSink->OnEnumWindow(m_arWinList[i]);
	}

	m_pEnumSink->OnEnumEnd();

	return TRUE;
}
