#include "stdafx.h"
#include "FilterDialog.h"
#include "resource.h"
#include "FilterConfig.h"
#include "Button.h"
#include "Edit.h"
#include "Combobox.h"

extern HINSTANCE g_hInstance;


INT_PTR CALLBACK DialogCallback(
							  HWND hwndDlg,
							  UINT uMsg,
							  WPARAM wParam,
							  LPARAM lParam
							)
{
	if(uMsg == WM_INITDIALOG)
	{
		SetWindowLong(hwndDlg, GWL_USERDATA, lParam);
	}

	BOOL bHandled(FALSE);
	INT_PTR nRet(0);
	CFilterDialog* pDialog = (CFilterDialog*)GetWindowLong(hwndDlg, GWL_USERDATA);
	BOOL bSave(FALSE);
	BOOL bClose(FALSE);

	switch(uMsg)
	{
	case WM_INITDIALOG:
		nRet = TRUE;
		bHandled = TRUE;
		
		if(pDialog != NULL)
		{
			pDialog->m_hWnd = hwndDlg;
			pDialog->LoadInitData();
		}
		break;

	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDCANCEL:
				bClose = TRUE;
				break;

			case IDOK:
				bSave = bClose = TRUE;
				break;

			case IDAPPLY:
				bSave = TRUE;
				break;
			}
		}
		break;

	case WM_CLOSE:
		bClose = TRUE;
		break;		


	default:
		break;
	}
	
	if(pDialog != NULL)
	{
		if(bSave) pDialog->SaveData();
		if(bClose) pDialog->DestroyDialog();
	}

	if(!bHandled)
	{
		nRet = FALSE;
	}

	return nRet;
}

CFilterDialog::CFilterDialog()
:m_hWnd(NULL)
{

}

CFilterDialog::~CFilterDialog()
{

}

BOOL CFilterDialog::ShowDialog(HWND hWndParent)
{
	if(IsWindow()) return TRUE;
	
	m_hWnd = CreateDialogParam(g_hInstance, MAKEINTRESOURCE(IDD), hWndParent, DialogCallback, (LPARAM)this);
	if(m_hWnd != NULL)
	{
		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);

		return TRUE;
	}

	return FALSE;

}

VOID CFilterDialog::DestroyDialog()
{
	if(IsWindow())
	{
		DestroyWindow(m_hWnd);
	}

	m_hWnd = NULL;
}

BOOL CFilterDialog::IsWindow() const
{
	return (m_hWnd != NULL && ::IsWindow(m_hWnd));
}

BOOL CFilterDialog::IsDialogMessage(LPMSG pMsg)
{
	return ::IsDialogMessage(m_hWnd, pMsg);
}

VOID CFilterDialog::LoadInitData()
{
	HWND hButtonVisible = GetDlgItem(m_hWnd, IDC_VISIBLE);
	HWND hButtonLayered = GetDlgItem(m_hWnd, IDC_LAYERED);
	HWND hButtonTopMost = GetDlgItem(m_hWnd, IDC_TOPMOST);
	HWND hEditProcessID = GetDlgItem(m_hWnd, IDC_PROCESSID);
	HWND hEditClassName = GetDlgItem(m_hWnd, IDC_CLASSNAME);
	HWND hEditText = GetDlgItem(m_hWnd, IDC_TEXT);
	HWND hCombMethod = GetDlgItem(m_hWnd, IDC_ENUM_METHOD);
	
	CFilterConfig& config = CFilterConfig::GetInstance();

	CButtonHandle btn(hButtonVisible);
	btn.SetCheck(config.GetVisible());

	btn = hButtonLayered;
	btn.SetCheck(config.GetLayered());

	btn = hButtonTopMost;
	btn.SetCheck(config.GetTopMost());

	CEditHandle edit(hEditProcessID);
	edit.SetLimitTextLength(128);
	edit.SetWindowText(config.GetProcessID());
	

	edit = hEditClassName;
	edit.SetWindowText(config.GetClassName());
	edit.SetLimitTextLength(128);

	edit = hEditText;
	edit.SetWindowText(config.GetText());
	edit.SetLimitTextLength(128);

	CComboboxHandle comb(hCombMethod);
	INT nIndexEnum = comb.AddString(_T("EnumWindow"));
	comb.SetItemData(nIndexEnum, ENUM_METHOD_ENUMWINDOW);
	INT nIndexFind = comb.AddString(_T("FindWindow"));
	comb.SetItemData(nIndexFind, ENUM_METHOD_FINDWINDOW);
	if(CFilterConfig::GetInstance().GetEnumMethod() == ENUM_METHOD_ENUMWINDOW)
	{
		comb.SetCurSel(nIndexEnum);
	}
	else
	{
		comb.SetCurSel(nIndexFind);
	}
}

VOID CFilterDialog::SaveData()
{
	HWND hButtonVisible = GetDlgItem(m_hWnd, IDC_VISIBLE);
	HWND hButtonLayered = GetDlgItem(m_hWnd, IDC_LAYERED);
	HWND hButtonTopMost = GetDlgItem(m_hWnd, IDC_TOPMOST);
	HWND hEditProcessID = GetDlgItem(m_hWnd, IDC_PROCESSID);
	HWND hEditClassName = GetDlgItem(m_hWnd, IDC_CLASSNAME);
	HWND hEditText = GetDlgItem(m_hWnd, IDC_TEXT);
	HWND hCombMethod = GetDlgItem(m_hWnd, IDC_ENUM_METHOD);

	CFilterConfig& config = CFilterConfig::GetInstance();

	CButtonHandle btn(hButtonVisible);
	config.SetVisible(btn.GetCheck());

	btn = hButtonLayered;
	config.SetLayered(btn.GetCheck());

	btn = hButtonTopMost;
	config.SetTopMost(btn.GetCheck());

	const INT BUFFER_LEN = 256;
	TCHAR szBuffer[BUFFER_LEN] = {0};
	CEditHandle edit(hEditProcessID);
	ZeroMemory(szBuffer, sizeof(szBuffer));
	edit.GetWindowText(szBuffer, BUFFER_LEN);
	DWORD dwProcessID(0);
	if(szBuffer[0] == _T('0') &&
		(szBuffer[1] == _T('x') || szBuffer[1] == _T('X')))
	{
		dwProcessID = wcstol(szBuffer, NULL, 16);
	}
	else
	{
		dwProcessID = wcstol(szBuffer, NULL, 10);
	}
	config.SetDwordProcessID(dwProcessID);

	edit = hEditClassName;
	ZeroMemory(szBuffer, sizeof(szBuffer));
	edit.GetWindowText(szBuffer, BUFFER_LEN);
	config.SetClassName(szBuffer);

	edit = hEditText;
	ZeroMemory(szBuffer, sizeof(szBuffer));
	edit.GetWindowText(szBuffer, BUFFER_LEN);
	config.SetText(szBuffer);

	CComboboxHandle comb(hCombMethod);
	INT nIndex = comb.GetCurSel();
	ENUM_METHOD eMethod = (ENUM_METHOD)comb.GetItemData(nIndex);
	config.SetEnumMethod(eMethod);
}