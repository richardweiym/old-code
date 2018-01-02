// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__5D306CC1_F6B2_4A0C_8D77_C2951FC7B942__INCLUDED_)
#define AFX_MAINDLG_H__5D306CC1_F6B2_4A0C_8D77_C2951FC7B942__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <atlmisc.h>
#include "../SmsRegister/SmsRegister.h"

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_CLOSE, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_COPY, OnCopy)
		COMMAND_HANDLER(IDC_MACHINE_CODE, EN_CHANGE, OnMachineCodeChange)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);
		
		CEdit edit = GetDlgItem(IDC_REGISTER_CODE);
		edit.SetLimitText(80);
		edit = GetDlgItem(IDC_MACHINE_CODE);
		edit.SetLimitText(80);

		return TRUE;
	}
	LRESULT OnCopy(WORD, WORD, HWND, BOOL&)
	{
		CEdit edit = GetDlgItem(IDC_REGISTER_CODE);
		
		edit.SetSelAll();
		edit.Copy();

		return 0;
	}

	LRESULT OnMachineCodeChange(WORD, WORD, HWND, BOOL&)
	{
		CString strMachineCode, strRegisterCode;
		
		GetDlgItemText(IDC_MACHINE_CODE, strMachineCode.GetBuffer(100), 100);
		strMachineCode.ReleaseBuffer();

		if(strMachineCode != _T(""))
		{
			if(GetRegisterCodeByMachineCode(strMachineCode, strRegisterCode))
			{
				SetDlgItemText(IDC_REGISTER_CODE, strRegisterCode);
			}
		}

		return 0;
	}

	LRESULT OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CloseDialog(0);
		return 0;
	}

	LRESULT OnCloseCmd(WORD, WORD, HWND, BOOL&)
	{
		CloseDialog(0);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__5D306CC1_F6B2_4A0C_8D77_C2951FC7B942__INCLUDED_)
