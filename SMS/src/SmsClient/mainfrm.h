// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__32CE7B6E_F346_4A9A_82F7_CB44E603CD2F__INCLUDED_)
#define AFX_MAINFRM_H__32CE7B6E_F346_4A9A_82F7_CB44E603CD2F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "../ClientSocketLib/SmsSocketClient.h"
#include <atlframe.h>
#include "SmsClientview.h"
#include "ToolbarHelper.h"
#include <atlctrlx.h>

//socket 回调消息
#define  WM_SMS_CLIENT WM_USER+100

#define  SMS_CLIENT_CONNECT_WP 1
#define  SMS_CLIENT_SENDED_WP  2
#define  SMS_CLIENT_RECEIVED_WP 3
#define  SMS_CLIENT_STOPPED_WP 4

#define  WM_SHELL_MSG WM_USER + 200


class CServiceManager;
class CSmsClientConfig;
class CSmsTray;

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler,
		private CToolBarHelper<CMainFrame>
{
public:
	enum ENUM_SMS_CLIENT_STATUS
	{ 
		STOPPED = 0,
		BUSY,
		IDLE,
		STARTING
	};
	
	enum 
	{
		PANE_INFO = 1,
		PANE_COUNT,
		PANE_COMPANY,
		PANE_STATUS,

		PANE_INFO_WIDTH = 300,
		PANE_COUNT_WIDTH = 200,	
		PANE_STATUS_WIDTH = 80,
		PANE_ALL_MIN_WIDTH = PANE_INFO_WIDTH + PANE_COUNT_WIDTH + PANE_STATUS_WIDTH
	};

	DECLARE_FRAME_WND_CLASS(_T("SmsClientMainFrame_Richard_612"), IDR_MAINFRAME)

	CMainFrame(CServiceManager*);
	~CMainFrame();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	void OnToolBarCombo(HWND hWndCombo, UINT nID, int nSel, LPCTSTR lpszText, DWORD dwItemData);
	virtual void PrepareToolBarMenu(UINT nMenuID, HMENU hMenu);
	
	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_SYSTEM_CONNECT, UPDUI_MENUPOPUP|UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SYSTEM_DISCONNECT, UPDUI_MENUPOPUP|UPDUI_TOOLBAR)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		//MESSAGE_HANDLER(WM_QUERYENDSESSION, OnQueryEndSession)
		MESSAGE_HANDLER(WM_MENUSELECT, OnMenuSelect)

		MESSAGE_HANDLER(WM_VIEW_CHANGED, OnViewChanged)
		MESSAGE_HANDLER(WM_SHELL_MSG, OnTrayMsg)
		
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		
		MESSAGE_HANDLER(WM_SMS_CLIENT, OnSocketClientEvent)

		COMMAND_ID_HANDLER(ID_SYSTEM_CONNECT, OnConnect)
		COMMAND_ID_HANDLER(ID_SYSTEM_DISCONNECT, OnDisconnect)
		COMMAND_ID_HANDLER(ID_SYSTEM_SETTING, OnSetting)
		COMMAND_ID_HANDLER(ID_SYSTEM_EXIT, OnExit)
		COMMAND_ID_HANDLER(ID_TOOL_CONTACTLIST, OnToolContact)
		COMMAND_ID_HANDLER(ID_TOOL_SMSLIB, OnToolLib)
		
		COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnAbout)
		COMMAND_ID_HANDLER(ID_HELP_QUESTION, OnQuestion)

		COMMAND_ID_HANDLER(ID_SYSTEM_SHOW, OnShowWindow)

		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		CHAIN_MSG_MAP(CToolBarHelper<CMainFrame>)

		CHAIN_MSG_MAP_ALT_MEMBER(m_view, 1)

	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	
	VOID LoadSms();
	
	LRESULT OnTrayMsg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMenuSelect(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnViewChanged(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
 	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	LRESULT OnConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDisconnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSetting(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToolContact(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToolLib(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAbout(WORD, WORD, HWND, BOOL&);
	LRESULT OnShowWindow(WORD, WORD, HWND, BOOL&);
	LRESULT OnQuestion(WORD, WORD ,HWND, BOOL&);
	//LRESULT OnQueryEndSession(UINT, WPARAM, LPARAM, BOOL&);

	LRESULT OnSocketClientEvent(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	static HINSTANCE GetResourceModule() { return m_hResModule;}
	static CMainFrame* GetMainFrame() {return m_pMainFrame;}
	static CSmsClientView* GetClientView() {return m_pClientView;}

	CSmsClientConfig* GetSmsClientConfig() {return m_pSmsConfig;}
	CServiceManager* GetServiceMgr() {return m_pServiceManager;}
	VOID StartConnect();
	VOID Disconnect();
	VOID ResetStatusBar();

	BOOL SetExitFlag(BOOL b) {m_bExit = b;}
public:
	CCommandBarCtrl m_CmdBar;

private:
	
	VOID GetErrorMsg(int nError, CString& strMsg);
private:
	CSmsClientView m_view;
	CMultiPaneStatusBarCtrl m_statusBar;

	static HINSTANCE m_hResModule;
	static CMainFrame* m_pMainFrame;
	static CSmsClientView* m_pClientView;

	CServiceManager* m_pServiceManager;
	CSmsClientConfig* m_pSmsConfig;
	INT m_nTimer;
	ENUM_SMS_CLIENT_STATUS m_eCurrentStatus;

	INT m_nSendedCount;
	INT m_nReceivedCount;

	CString m_strInforStatus;
	CSmsTray* m_pTray;
	
	BOOL m_bSaveWinPos;
	BOOL m_bExit;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__32CE7B6E_F346_4A9A_82F7_CB44E603CD2F__INCLUDED_)
