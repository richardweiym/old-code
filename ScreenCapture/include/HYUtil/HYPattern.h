#pragma once 

#include <vector>
#include "HYUtilBase.h"

//singleton
#ifdef _DEBUG
#define HY_RUN_ONCE_CHECK {static int s_nRunOnce(0); _ASSERTE(++s_nRunOnce == 1);}
#else
#define HY_RUN_ONCE_CHECK \
 {static int s_nRunOnce(0); \
if(++s_nRunOnce != 1) ::MessageBox(NULL, _T("Singleton Run Twice"), _T("TS_RUN_ONCE_CHECK"), MB_OK);}
#endif // _DEBUG

template<typename T>
class CHYSingleton
{
public:
	static T* GetInstance()
	{
		if(s_pInstance == NULL)
		{
			s_pInstance = new T;
		}

		return s_pInstance;
	}

	static VOID DeleteInstance()
	{
		if(s_pInstance != NULL)
		{
			delete s_pInstance;
			s_pInstance = NULL;
		}
	}

protected:
	 static T* s_pInstance;
};

template<typename T> T* CHYSingleton<T>::s_pInstance = NULL;


// template<typename T>
// class CHYObserverBase
// {
// public:
// 	virtual ~CHYObserverBase() {}
// 
// 	BEGIN_MSG_MAP(CHYObserverBase<T>)
// 		MESSAGE_HANDLER(WM_CREATE, OnCreate)
// 		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
// 		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
// 		MESSAGE_HANDLER(WM_HY_UI_DATA_CHANGED, OnUIDataChanged)
// 	END_MSG_MAP()
// 	
// 	// class T should realize these functions:
// 	// void DoInit();
// 	// void DoRelease();
// 	// void DoUIDataChanged(WPARAM, LPARAM);
// 
// 	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
// 	{
// 		bHandled = FALSE;
// 		_Init();
// 		return S_OK;
// 	}
// 	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
// 	{
// 		bHandled = FALSE;
// 		_Init();
// 		return S_OK;
// 	}
// 	LRESULT OnDestroy(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
// 	{
// 		bHandled = FALSE;
// 		_Release();
// 		return S_OK;
// 	}
// 	LRESULT OnUIDataChanged(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
// 	{
// 		bHandled = FALSE;
// 
// 		T* pThis = static_cast<T*>(this);
// 		if(pThis->IsWindow())
// 		{
// 			pThis->DoUIDataChanged(wParam, lParam);
// 		}
// 		else
// 		{
// 			_ASSERTE(FALSE);
// 		}
// 
// 		return S_OK;
// 	}
// 	
// protected:
// 	void _Init()
// 	{
// 		T* pThis = static_cast<T*>(this);
// 		if(pThis->IsWindow())
// 		{
// 			pThis->DoInit();
// 		}
// 		else
// 		{
// 			_ASSERTE(FALSE);
// 		}
// 	}
// 	void _Release()
// 	{
// 		T* pThis = static_cast<T*>(this);
// 		if(pThis->IsWindow())
// 		{
// 			pThis->DoRelease();
// 		}
// 		else
// 		{
// 			_ASSERTE(FALSE);
// 		}
// 	}
// };

class CHYDataModuleBase
{
public:
	CHYDataModuleBase();
	virtual ~CHYDataModuleBase() { }

	int AddObserver(HWND hWnd);
	bool RemoveObserver(HWND hWnd);
	
	int GetObserverCount();
	HWND GetObserverByIndex(INT nIndex);
	
	void NotifyObservers(WPARAM wParam, LPARAM lParam, UINT nMsg = WM_HY_UI_DATA_CHANGED, BOOL bAsyn = FALSE);
	
	void SetEnableNotify(BOOL bEnable) { m_bEnableNotify = bEnable; }
	BOOL GetEnableNotify() { return m_bEnableNotify; }

private:
	std::vector<HWND> m_arData;
	BOOL m_bEnableNotify;
};
