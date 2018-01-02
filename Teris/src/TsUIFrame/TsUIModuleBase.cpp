#include "TsUIModuleBase.h"
#include <functional>
#include <algorithm>

using namespace std;

CTsModuleBase::CTsModuleBase()
{
	m_bEnableNotify = TRUE;
}

int CTsModuleBase::AddObserver(HWND hWnd)
{
	int nRet(-1);
	vector<HWND>::iterator itr = find(m_arData.begin(), m_arData.end(), hWnd);
	if(itr == m_arData.end())
	{
		m_arData.push_back(hWnd);
		nRet = m_arData.size();
	}
	else
	{
		_ASSERTE(FALSE);
		nRet = distance(m_arData.begin(), itr);
	}

	return nRet;
}

bool CTsModuleBase::RemoveObserver(HWND hWnd)
{
	bool bRet(FALSE);
	vector<HWND>::iterator itr = find(m_arData.begin(), m_arData.end(), hWnd);
	if(itr != m_arData.end())
	{
		m_arData.erase(itr);
		bRet = TRUE;
	}
	else
	{
		_ASSERTE(FALSE);
	}

	return bRet;
}

int CTsModuleBase::GetObserverCount()
{
	return m_arData.size();
}

HWND CTsModuleBase::GetObserverByIndex(INT nIndex)
{
	HWND hRet(NULL);
	_ASSERTE(nIndex < GetObserverCount() && nIndex > 0);
	if(nIndex < GetObserverCount())
	{
		hRet = m_arData[nIndex];
	}

	return hRet;
}

class CNotifyFunObj: public unary_function<void, HWND>
{
public:
	CNotifyFunObj(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL bAsyn)
		:m_nMsg(nMsg), m_wParam(wParam), m_lParam(lParam), m_bAsyn(bAsyn)
	{	
		NULL;
	}

	void operator()(const HWND& hWnd)
	{
		_ASSERTE(IsWindow(hWnd));
		
		if(m_bAsyn)
		{
			PostMessage(hWnd, m_nMsg, m_wParam, m_lParam);
		}
		else
		{
			SendMessage(hWnd, m_nMsg, m_wParam, m_lParam);
		}
	}
private:
	UINT m_nMsg;
	WPARAM m_wParam;
	LPARAM m_lParam;
	BOOL m_bAsyn;
};
void CTsModuleBase::NotifyObservers(WPARAM wParam, LPARAM lParam, UINT nMsg /* = WM_TS_UI_DATA_CHANGED */, BOOL bAsyn /* = FALSE */)
{
	if(m_bEnableNotify)
	{
		for_each(m_arData.begin(), m_arData.end(), CNotifyFunObj(nMsg, wParam, lParam, bAsyn));
	}
}