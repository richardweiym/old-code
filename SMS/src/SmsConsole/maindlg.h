// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma  once 

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <atlctrlx.h>
#include "bmpbutton.h"
#include "SmsDlgBk.h"
#include "../SmsServer/SmsControlDef.h"
#include "SetupDlg.h"

#define  WM_SHELL_MSG WM_USER + 1000

class CSmsAccountManager;
class CSetupDlg;
class CConsoleTray;

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler,public CDialogSkin
{
public:
 	enum { IDD = IDD_MAINDLG };
	enum {ID_PANE_STATUS = 1, ID_PANE_COUNT = 2};
	enum {PANE_WIDTH = 310, COUNT_WIDTH = 50, STATUS_WIDTH = 250};
	enum  ENUM_UI_STATUS {UI_STATUS_STOPPED, UI_STATUS_RUNNING,UI_STATUS_STARTING};

	CMainDlg();
	~CMainDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(m_pSettingDlg->IsWindow())
		{
			return m_pSettingDlg->IsDialogMessage(pMsg);
		}

		return IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)

		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
//		MESSAGE_HANDLER(WM_COPYDATA,OnCopyData)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SHELL_MSG, OnTrayMsg)
		MESSAGE_HANDLER(WM_SMS_ON_STOP,OnStopMessage)
		MESSAGE_HANDLER(WM_SMS_ON_START,OnStartMessage)
		MESSAGE_HANDLER(WM_SMS_ON_ERROR,OnErrorMessage)
		MESSAGE_HANDLER(WM_SMS_ON_DEVICE_SIGNAL, OnGetSignal)
		MESSAGE_HANDLER(WM_SMS_ON_CONNECT_COUNT, OnGetConCount)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	//	MESSAGE_HANDLER(WM_SMS_ON_STATUS_INDICATION,OnStatusIndication)
		MESSAGE_HANDLER(WM_SMS_ON_RECEIVE_MESSAGE, OnReceiveMessage)
		MESSAGE_HANDLER(WM_SMS_ON_SEND_MESSAGE, OnSendedMessage)
		MESSAGE_HANDLER(WM_SMS_ON_CONNECT, OnSmsConnect)
		MESSAGE_HANDLER(WM_SMS_ON_DISCONNECT, OnSmsDisconnect)

		COMMAND_ID_HANDLER(IDC_START, OnStart)
		COMMAND_ID_HANDLER(IDC_STOP, OnStop)
		COMMAND_ID_HANDLER(IDC_SETTING, OnSetting)
		COMMAND_ID_HANDLER(ID_SHOW_MAIN, OnShowMainDlg)
		COMMAND_ID_HANDLER(ID_ABOUT, OnAbout)
		COMMAND_ID_HANDLER(ID_EXIT, OnExit)
		
		CHAIN_MSG_MAP(CDialogSkin)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnTimer(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);	
	LRESULT OnPaint(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetSignal(UINT uMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetConCount(UINT, WPARAM wParam, LPARAM, BOOL&);
//	LRESULT OnCopyData(UINT uMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	LRESULT OnStatusIndication(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnStartMessage(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnStopMessage(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnErrorMessage(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnStart(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnStop(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT nMsgType,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT nMsgType , WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnReceiveMessage(UINT nMsgType, WPARAM wParam, LPARAM lPram, BOOL& bHandled);
	LRESULT OnSendedMessage(UINT nMsgType, WPARAM wParam, LPARAM lPram, BOOL& bHandled);
	LRESULT OnSetting(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSmsConnect(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSmsDisconnect(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnTrayMsg(UINT, WPARAM, LPARAM, BOOL&);
	
	LRESULT	OnShowMainDlg(WORD, WORD, HWND, BOOL&);
	LRESULT OnAbout(WORD, WORD ,HWND ,BOOL&);
	LRESULT OnExit(WORD, WORD, HWND, BOOL&);

	HINSTANCE GetResourceInstance() {return m_hResModule;}
//	CSmsAccountManager* GetAccountManager() {return m_pAccountManager;}
	
	BOOL IsServiceRunning() {return (FindAgentWnd() != NULL);}
	VOID ResetCaption();
private:
	VOID PaintCurrentInformation(CPaintDC& dc);
	HWND FindAgentWnd();
	BOOL KillAgentExist();
	BOOL Stop();
	BOOL Start();
	VOID SetUIStatus(ENUM_UI_STATUS s);
	VOID ResetValues();
	

private:
	CIEButton m_btnStart;
	CIEButton m_btnStop;
	CIEButton m_btnSetting;
	
// 	HICON	m_hStartingIcon;
// 	HICON   m_hStoppedIcon;
// 	HICON   m_hRunningIcon;

	HINSTANCE m_hResModule;
	
	CMultiPaneStatusBarCtrl m_statusbar;
	
	//CSmsAccountManager* m_pAccountManager;
	ENUM_UI_STATUS m_eCurrentStatus;
	HWND m_hServiceAgent;
	CSetupDlg* m_pSettingDlg;
	TCHAR m_szErrorMsg[MAX_PATH];
	CConsoleTray* m_pConsoleTray;

	INT m_nConCount;
	INT m_nSignalCount;
	INT m_nSendedCount;
	INT m_nDispatchedCount;
	BOOL m_bExit;
};


/////////////////////////////////////////////////////////////////////////////


class CConsoleTray
{
public:
	CConsoleTray(CMainDlg* pDlg);
	VOID AddTray();
	VOID ModifyTray(CMainDlg::ENUM_UI_STATUS eStatus);
	VOID DeleteTray();

private:
	CMainDlg* m_pDlg;
};