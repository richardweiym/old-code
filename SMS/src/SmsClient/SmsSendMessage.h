#pragma  once

#include <atlwin.h>
#include <atlctrls.h>
#include "resource.h"
#include "SmsContactTree.h"

class CAddTel: public CDialogImpl<CAddTel>
{
public:
	enum {IDD = IDD_ADD_TEL};

	BEGIN_MSG_MAP(CAddTel)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_CANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_OK, OnOK)
	END_MSG_MAP()

	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCancel(WORD, WORD, HWND, BOOL&);
	LRESULT OnOK(WORD, WORD, HWND, BOOL&);

	CString m_strTel;
};

class CSmsSendDlg: public CDialogImpl<CSmsSendDlg>
{
public:
	enum { IDD = IDD_SEND };
	
	BEGIN_MSG_MAP(CSmsSendDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		COMMAND_ID_HANDLER(IDC_CLOSE, OnCmdClose)
		COMMAND_HANDLER(IDC_CONTENT, EN_CHANGE, OnContentChange)
		COMMAND_ID_HANDLER(IDC_ADD, OnAdd)
		COMMAND_ID_HANDLER(IDC_NEW, OnNew)
		COMMAND_ID_HANDLER(IDC_DELETE, OnDelete)
		COMMAND_ID_HANDLER(IDC_CLEAR, OnClear)
		COMMAND_ID_HANDLER(ID_NEW, OnNew)
		COMMAND_ID_HANDLER(ID_DELETE, OnDelete)
		COMMAND_ID_HANDLER(ID_CLEAR, OnClear)
		COMMAND_ID_HANDLER(IDC_SEND, OnSend)
		COMMAND_ID_HANDLER(IDC_INSERT_CONTENT, OnInsertContent)
		COMMAND_ID_HANDLER(IDC_ON_TIMER, OnTimerClick)

		NOTIFY_ID_HANDLER(IDC_RECEIVER, OnReceiverNotify)
		NOTIFY_ID_HANDLER(IDC_CONTACT, OnContactNotify)
	END_MSG_MAP()

	CSmsSendDlg(INT nID);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCmdClose(WORD, WORD, HWND, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnContentChange(WORD, WORD, HWND, BOOL&);
	LRESULT OnAdd(WORD, WORD, HWND, BOOL&);
	LRESULT OnNew(WORD, WORD, HWND, BOOL&);
	LRESULT OnDelete(WORD, WORD, HWND, BOOL&);
	LRESULT OnClear(WORD, WORD, HWND, BOOL&);
	LRESULT OnReceiverNotify(int, LPNMHDR pnmh, BOOL&);
	LRESULT OnContactNotify(int, LPNMHDR pnmh, BOOL&);
	LRESULT OnSend(WORD, WORD, HWND, BOOL&);
	LRESULT OnInsertContent(WORD, WORD, HWND, BOOL&);
	LRESULT OnTimerClick(WORD, WORD, HWND, BOOL&);

	VOID InsertContent(CString& strContent);
	VOID AddContactById(INT nId);
	VOID AddContactByItem(HTREEITEM hItem);

public:
	CString m_strContent;
	CString m_strName;
	CString m_strTel;

	INT m_nSenderId;

private:
	BOOL IsExistInReceiver(HTREEITEM hItem);
	VOID AddReceiver(CString& strName, CString& strTel, HTREEITEM hItem);
	
private:
	INT m_nWorkID;
	CSmsContactTree m_contactTree;
	CListViewCtrl m_receiverList;
};

class CSmsReadMsgDlg: public CDialogImpl<CSmsReadMsgDlg>
{
public:
	enum {IDD = IDD_READ_MSG};

	BEGIN_MSG_MAP(CSmsReadMsgDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		
		COMMAND_ID_HANDLER(IDC_REPLY, OnReply)
		COMMAND_ID_HANDLER(IDC_TRANSFER, OnTransfer)
		COMMAND_ID_HANDLER(IDC_SAVE, OnSave)
		COMMAND_ID_HANDLER(IDC_CLOSE, OnCmdClose)
	END_MSG_MAP()

CSmsReadMsgDlg(CString& strName, CString& strTel, CString& strContent, HWND hParent);
LRESULT OnInitDlg(UINT nMsgType, WPARAM	wParam, LPARAM lParam, BOOL& bHandled);
LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
LRESULT OnCmdClose(WORD, WORD, HWND, BOOL&);
LRESULT OnReply(WORD, WORD, HWND, BOOL&);
LRESULT OnTransfer(WORD, WORD, HWND, BOOL&);
LRESULT OnSave(WORD, WORD, HWND, BOOL&);

VOID Close();

private:
	CString m_strName;
	CString m_strContent;
	CString m_strTel;
	HWND m_hParent;
};