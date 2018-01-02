#pragma  once 

#include "TsStdAfx.h"
#include "TsResMgr.h"
#include "TsUIInstanceApi.h"

class CTsDialogBKMap
{
public:
	BEGIN_MSG_MAP(CTsDialogBKMap)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
	END_MSG_MAP()

	LRESULT OnCtlColorStatic(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCtlColorDlg(UINT, WPARAM, LPARAM, BOOL&);

private:
	VOID _CreateBrush();

	CBrushHandle m_brushBk;
};

template<typename T>
class CTsDialogBase: public CDialogImpl<CTsDialogBase>,
					 public CTsDialogBKMap
{
public:
	BEGIN_MSG_MAP(CTsDialogBase)
		CHAIN_MSG_MAP(CTsDialogBKMap)
	END_MSG_MAP()
	int DoModal(HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL)
	{
		ATLASSERT(m_hWnd == NULL);
		_Module.AddCreateWndData(&m_thunk.cd, this);
#ifdef _DEBUG
		m_bModal = true;
#endif //_DEBUG
	
		HINSTANCE hRes = NULL;
		CTsResMgr* pResMgr = TsGetResMgr();
		_ASSERTE(pResMgr != NULL);
		hRes = pResMgr->GetResInstance();
		_ASSERTE(hRes != NULL);

		if(hRes == NULL)
		{
			hRes = _Module.GetResourceInstance();
		}
		return ::DialogBoxParam(hRes, MAKEINTRESOURCE(T::IDD),
					hWndParent, (DLGPROC)T::StartDialogProc, dwInitParam);
	}

	// modeless dialogs
	HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL)
	{
		ATLASSERT(m_hWnd == NULL);
		_Module.AddCreateWndData(&m_thunk.cd, this);
#ifdef _DEBUG
		m_bModal = false;
#endif //_DEBUG

		HINSTANCE hRes = NULL;
		CTsResMgr* pResMgr = TsGetResMgr();
		_ASSERTE(pResMgr != NULL);
		hRes = pResMgr->GetResInstance();
		_ASSERTE(hRes != NULL);

		if(hRes == NULL)
		{
			hRes = _Module.GetResourceInstance();
		}

		HWND hWnd = ::CreateDialogParam(hRes, MAKEINTRESOURCE(T::IDD),
					hWndParent, (DLGPROC)T::StartDialogProc, dwInitParam);
		ATLASSERT(m_hWnd == hWnd);
		return hWnd;
	}		
};