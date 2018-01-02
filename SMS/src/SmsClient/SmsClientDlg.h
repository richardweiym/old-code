#pragma once 

#include "resource.h"
#include <atlwin.h>

#define  WM_ON_INIT_MSG WM_USER + 100

class CSplashDlg:public CDialogImpl<CSplashDlg>
{
public:
	enum {IDD = IDD_SPLASH};
	
	CSplashDlg(BOOL bSplash = FALSE);
	~CSplashDlg();
	
	BEGIN_MSG_MAP(CSplashDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_ON_INIT_MSG, OnInitMsg)
	END_MSG_MAP()

	LRESULT OnInitDlg(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnInitMsg(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	BOOL m_bSplash;
	CProgressBarCtrl m_pb;

};

class CUserManager;
class CSettingDlg:public CDialogImpl<CSettingDlg>
{
public:

	enum {IDD = IDD_SETTING};
	
	BEGIN_MSG_MAP(CSettingDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_OK, OnOK)
		COMMAND_ID_HANDLER(IDC_CANCEL, OnCancel)
	END_MSG_MAP()

	CSettingDlg(CUserManager* pUserManger);
	LRESULT OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnInitDlg(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD,WORD,HWND,BOOL&);	
	LRESULT OnCancel(WORD,WORD,HWND,BOOL&);	
	
	LRESULT Close();
	LRESULT Save();
private:
	CUserManager* m_pUserManager;	
};

class CInformWindow: public CWindowImpl<CInformWindow>
{
public:
	DECLARE_WND_CLASS(_T("SmsInformation"))

	enum 
	{
		WIN_WIDTH = 160,
		WIN_HEIGHT = 120
	};

	enum
	{
		CLOSE_LEFT = 143,
		CLOSE_TOP = 4,
		CLOSE_WIDTH = 14,
		CLOSE_HEIGH = 13
	};
	
	BEGIN_MSG_MAP(CInformWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonClick)
	END_MSG_MAP()
	
	CInformWindow();
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLButtonClick(UINT, WPARAM, LPARAM, BOOL&);
	VOID ShowInformation(CString& strName, CString& strTel, CString& strContent);
	VOID SetRes(HBITMAP hBmpBack, CImageList& imgClose);
	VOID Close();
public:
	BOOL m_bShow;	

public:
	CString m_strName;
	CString m_strTel;
	CString m_strContent;
	HBITMAP m_hBmpBack;
	CImageList m_imgClose;

};

class CInformProxy 
{
public:
	enum 
	{
		WIN_WIDTH = 160,
		WIN_HEIGHT = 120
	};

public:
	BOOL Init();
	VOID ShowInformation(CString& strName, CString& strTel, CString& strContent);
	VOID Release();
	
private:
	CInformWindow* GetAnIdleWin();
private:
	HBITMAP m_hBmpBack;
	CImageList m_imgClose;
	CSimpleArray<CInformWindow*> m_arWindows;
};

class CInformationDlg: public CDialogImpl<CInformationDlg>
{
public:
	enum {IDD = IDD_INFORMATION};
	
public:
	BEGIN_MSG_MAP(CInformationDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_OK, OnOK)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	END_MSG_MAP()
	
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnOK(WORD, WORD, HWND, BOOL&);
	LRESULT OnCtlColorEdit(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

};