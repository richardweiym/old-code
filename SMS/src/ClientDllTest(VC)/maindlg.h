// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__D680C148_FD29_4B9A_8E15_676EA73BF4F5__INCLUDED_)
#define AFX_MAINDLG_H__D680C148_FD29_4B9A_8E15_676EA73BF4F5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "../SmsClientCtrl/SmsClientBase.h"
#pragma comment(lib, "smsclientctrl.lib")

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
		COMMAND_ID_HANDLER(IDC_CONNECT, OnConnect)
		COMMAND_ID_HANDLER(IDC_DISCONNECT, OnDisconnect)
		COMMAND_ID_HANDLER(IDC_SEND, OnSend)
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
		
		SetDlgItemText(IDC_SERVERIP, _T("127.0.0.1"));
		SetDlgItemInt(IDC_PORT, 1234);
		SetDlgItemText(IDC_NAME, _T("admin"));
		SetDlgItemText(IDC_PASSWORD, _T("8888"));

		SetDlgItemText(IDC_TEL, _T("10086"));
		SetDlgItemText(IDC_CONTENT, _T("11"));
		
		set_callback(SmsCallback);

		return TRUE;
	}

	LRESULT OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CloseDialog(1);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

	LRESULT OnConnect(WORD, WORD, HWND, BOOL&)
	{
		CString strServer, strName, strPassword;
		INT nPort = GetDlgItemInt(IDC_PORT);
		GetDlgItemText(IDC_SERVERIP, strServer.GetBuffer(256), 256);
		strServer.ReleaseBuffer();

		GetDlgItemText(IDC_NAME, strName.GetBuffer(256), 256);
		strName.ReleaseBuffer();

		GetDlgItemText(IDC_PASSWORD, strPassword.GetBuffer(256), 256);
		strPassword.ReleaseBuffer();

		connect_to_server(strServer, nPort, strName, strPassword);

		return 0;
	}
	LRESULT OnDisconnect(WORD, WORD, HWND, BOOL&)
	{
		disconnect();

		return 0;
	}
	LRESULT OnSend(WORD, WORD, HWND, BOOL&)
	{
		CString strTel, strContent;
		
		GetDlgItemText(IDC_TEL, strTel.GetBuffer(256), 256);
		strTel.ReleaseBuffer();

		GetDlgItemText(IDC_CONTENT, strContent.GetBuffer(256), 256);
		strContent.ReleaseBuffer();

		BOOL bRet = send_message(strTel, strContent, NULL);

		CString	 strInfo;
		if(!bRet)
		{
			strInfo.Format(_T("send message fail, error code: %d"), get_last_error_code());
		}
		else
		{
			strInfo.Format(_T("send to server ok"));
		}
		MessageBox(strInfo, _T(""), MB_OK);

		return 0;
	}

	static void __stdcall SmsCallback(INT nMsgType, LPCTSTR lpszTel, LPCTSTR lpszContent, LPCTSTR lpszTag, bool bOK)
	{
		CString strInfo;
		switch(nMsgType)
		{
		case MESSAGE_TYPE_CONNECT:
			if(bOK)
			{
				strInfo = _T("connect ok!");
			}
			else
			{
				strInfo.Format(_T("connect fail: error code: %d"), get_last_error_code());
			}
			break;
		case MESSAGE_TYPE_SEND_RETURN:
			if(bOK)
			{
				strInfo.Format(_T("message sended ok, tel: %s, content:%s"), lpszTel, lpszContent);
			}
			else
			{
				strInfo.Format(_T("message sended fail, tel: %s, content:%s"), lpszTel, lpszContent);
			}
			break;
		case MESSAGE_TYPE_RECEIVE:
			strInfo.Format(_T("receive message: tel : %s, content: %s"), lpszTel, lpszContent);
			break;
		case MESSAGE_TYPE_SERVICE_STOP:
			strInfo.Format(_T(" service stopped or disconnect"));
			break;
		}

		::MessageBox(NULL, strInfo, _T(""), MB_OK);
	}

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__D680C148_FD29_4B9A_8E15_676EA73BF4F5__INCLUDED_)
