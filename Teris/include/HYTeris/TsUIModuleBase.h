#pragma  once

#include "TsStdAfx.h"
#include <vector>
#include "TsUtilBase.h"
#include "TsDefine.h"

template<typename T>
class CTsObserverBase
{
public:
	virtual ~CTsObserverBase() {}

	BEGIN_MSG_MAP(CTsObserverBase<T>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_TS_UI_DATA_CHANGED, OnUIDataChanged)
	END_MSG_MAP()
	
	// class T should realize these functions:
	// void DoInit();
	// void DoRelease();
	// void DoUIDataChanged(WPARAM, LPARAM);

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
		_Init();
		return S_OK;
	}
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
		_Init();
		return S_OK;
	}
	LRESULT OnDestroy(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		_Release();
		return S_OK;
	}
	LRESULT OnUIDataChanged(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T* pThis = static_cast<T*>(this);
		if(pThis->IsWindow())
		{
			pThis->DoUIDataChanged(wParam, lParam);
		}
		else
		{
			_ASSERTE(FALSE);
		}

		return S_OK;
	}
	
protected:
	void _Init()
	{
		T* pThis = static_cast<T*>(this);
		if(pThis->IsWindow())
		{
			pThis->DoInit();
		}
		else
		{
			_ASSERTE(FALSE);
		}
	}
	void _Release()
	{
		T* pThis = static_cast<T*>(this);
		if(pThis->IsWindow())
		{
			pThis->DoRelease();
		}
		else
		{
			_ASSERTE(FALSE);
		}
	}
};

class CTsModuleBase
{
public:
	CTsModuleBase();
	virtual ~CTsModuleBase() { }

	int AddObserver(HWND hWnd);
	bool RemoveObserver(HWND hWnd);
	
	int GetObserverCount();
	HWND GetObserverByIndex(INT nIndex);
	
	void NotifyObservers(WPARAM wParam, LPARAM lParam, UINT nMsg = WM_TS_UI_DATA_CHANGED, BOOL bAsyn = FALSE);
	
	void SetEnableNotify(BOOL bEnable) { m_bEnableNotify = bEnable; }
	BOOL GetEnableNotify() { return m_bEnableNotify; }

private:
	std::vector<HWND> m_arData;
	BOOL m_bEnableNotify;
};
