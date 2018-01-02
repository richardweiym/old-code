// SmsClientView.h : interface of the CSmsClientView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMSCLIENTVIEW_H__35563486_39AB_4441_A769_FD3F5FFBE334__INCLUDED_)
#define AFX_SMSCLIENTVIEW_H__35563486_39AB_4441_A769_FD3F5FFBE334__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <atlctrls.h>
#include <atlctrlw.h>
#include "resource.h"
#include <atlsplit.h>
#include <atlctrlx.h>
#include <atlframe.h>
#include "messageManager.h"

#define  WM_VIEW_CHANGED WM_USER + 1000

class CMainFrame;
class CSmsBoxTree;
class CSmsContactTree;
template<typename c> class CSmsListView;
class CSmsDetailDlg;
class CSMSMessage;
class CInformWindow;
class CInformProxy;

class CSmsClientView : public CWindowImpl<CSmsClientView>,
					   public CUpdateUI<CSmsClientView>
{	
public:
	DECLARE_WND_CLASS(_T("SmsClientView"))

	enum SMS_LIST_VIEW_MODE
	{
		SMS_TOSEND_VIEW = 0,
		SMS_SENDED_VIEW = 1,
		SMS_RECEIVED_VIEW = 2,
		SMS_DELETED_VIEW = 3
	};

	enum 
	{
		SMS_BOX_VISIBLE	    = 0x00000001,
		SMS_CONTACT_VISIBLE = 0x00000002,
		SMS_LIST_VISIBLE    = 0x00000004,
		SMS_CONTENT_VISIBLE = 0x00000008
	};

	enum SMS_SUB_WINDOW_ID
	{
		ID_SMS_BOX = 20,
		ID_SMS_CONTACT,
		ID_SMS_LISTVIEW_TOSEND,
		ID_SMS_LISTVIEW_SENDED,
		ID_SMS_LISTVIEW_RECEIVED,
		ID_SMS_LISTVIEW_DELETED
	};

	struct SMS_SPLITTER_POS
	{
		SMS_SPLITTER_POS(INT x, INT y, INT z)
		{
			nLR = x;
			nTB1 = y;
			nTB2 = z;
		}

		INT nLR;
		INT nTB1;
		INT nTB2;
	};

	CSmsClientView(CMainFrame* pMain);
	~CSmsClientView();
	 BOOL PreTranslateMessage(MSG* pMsg);
	 BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CSmsClientView)
		UPDATE_ELEMENT(ID_VIEW_DELETED, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_TOSEND, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_SENDED, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_RECEIVED, UPDUI_MENUPOPUP)

		UPDATE_ELEMENT(ID_VIEW_BOX, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_CONTACTLIST, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_CONTENT, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CSmsClientView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		
		COMMAND_ID_HANDLER(ID_PANE_CLOSE, OnPaneClose);
		NOTIFY_ID_HANDLER(ID_SMS_BOX, OnBoxTree)
		NOTIFY_RANGE_HANDLER(ID_SMS_LISTVIEW_TOSEND,ID_SMS_LISTVIEW_DELETED, OnListviewNotify)
		NOTIFY_ID_HANDLER(ID_SMS_CONTACT, OnContactNotify)

		ALT_MSG_MAP(1)
		COMMAND_ID_HANDLER(ID_VIEW_TOSEND, OnViewToSend)
		COMMAND_ID_HANDLER(ID_VIEW_SENDED, OnViewSended)
		COMMAND_ID_HANDLER(ID_VIEW_RECEIVED, OnViewReceived)
		COMMAND_ID_HANDLER(ID_VIEW_DELETED, OnViewDeleted)
		COMMAND_ID_HANDLER(ID_VIEW_CURRENT, OnViewCurrentButton)

		COMMAND_ID_HANDLER(ID_VIEW_BOX, OnViewBox)
		COMMAND_ID_HANDLER(ID_VIEW_CONTACTLIST, OnViewContactList)
		COMMAND_ID_HANDLER(ID_VIEW_CONTENT, OnViewContent)	
		
		COMMAND_ID_HANDLER(ID_SMS_NEW, OnSmsNew)
		COMMAND_ID_HANDLER(ID_SMS_REPLY, OnSmsReply)
		COMMAND_ID_HANDLER(ID_SMS_TRANSFER, OnSmsTransfer)
		COMMAND_ID_HANDLER(ID_SMS_DELETE, OnSmsDelete)
		COMMAND_ID_HANDLER(ID_SMS_CLEAR, OnSmsClear)
		COMMAND_ID_HANDLER(ID_SMS_SEARCH, OnSmsSearch)
		COMMAND_ID_HANDLER(ID_SMS_SAVE, OnSmsSave)

		COMMAND_ID_HANDLER(ID_SEND_MSG, OnSendMsg)

		CHAIN_MSG_MAP(CUpdateUI<CSmsClientView>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaneClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT nMsgType , WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnViewBox(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewContactList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewContent(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
 	LRESULT OnBoxTree(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnListviewNotify(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnContactNotify(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	
	LRESULT OnViewToSend(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewSended(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewReceived(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewDeleted(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewCurrentButton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	LRESULT OnSmsNew(WORD, WORD, HWND, BOOL&);
	LRESULT OnSmsReply(WORD, WORD, HWND, BOOL&);	
	LRESULT OnSmsTransfer(WORD, WORD, HWND, BOOL&);
	LRESULT OnSmsDelete(WORD, WORD, HWND, BOOL&);
	LRESULT OnSmsClear(WORD, WORD, HWND, BOOL&);
	LRESULT OnSmsSearch(WORD, WORD, HWND, BOOL&);
	LRESULT OnSmsSave(WORD, WORD, HWND, BOOL&);
	LRESULT OnSendMsg(WORD, WORD, HWND, BOOL&);

	BOOL GetCurrenSelInfo(CString& strName, CString& strTel, CString& strContent);

	VOID UpdateSplit();
	VOID LoadListContent(SMS_LIST_VIEW_MODE mode);
	VOID LoadContacts();

	VOID LoadViewSplitConfig();
	VOID SaveViewSplitConfig();

	CSmsListView<CToSendMessage>* GetToSendList()
	{
		return m_pToSendMessages;
	}
	CSmsListView<CSendedMessage>* GetSendedList()
	{
		return m_pSendedMessages;
	}
	CSmsListView<CReceiveMessage>* GetReceivedList()
	{
		return m_pReceiveMessages;
	}
	CSmsListView<CDeletedMessage>* GetDeletedList()
	{
		return m_pDeletedMessages;
	}
	BOOL DoSend1Message();
	BOOL DoSended1Message(CSMSMessage* pMsg);
	BOOL DoReceivedMessage(CSMSMessage* pMsg);
	VOID OnDisconnect();
	
	INT GetCurrentCount(BOOL bSelCount = FALSE);
	SMS_LIST_VIEW_MODE GetCurrentViewMode() {return m_eViewMode;}
protected:
	
	VOID SendMsgToSel(BOOL bOnlyLeaf = FALSE);
	VOID DoDelete(BOOL bDelAll);
	VOID ChangeListView(SMS_LIST_VIEW_MODE id);
	VOID CreateSplitWindow();
	VOID OnViewModeChanged();
	VOID ShowInformWindow(CReceiveMessage& msg);
private:
	CSmsBoxTree* m_pSmsBoxTree;
	CSmsContactTree* m_pSmsContactTree;
	CSmsListView<CToSendMessage>* m_pToSendMessages;
	CSmsListView<CSendedMessage>* m_pSendedMessages;
	CSmsListView<CReceiveMessage>* m_pReceiveMessages;
	CSmsListView<CDeletedMessage>* m_pDeletedMessages;
	CSmsDetailDlg* m_pSmsDetailDlg;

	CSplitterWindow m_wndSplitLR;
	CHorSplitterWindow m_wndSplitTB1;
	CHorSplitterWindow m_wndSplitTB2;

	CPaneContainer m_SmsBoxContainer;
	CPaneContainer m_SmsContactContainer;

	DWORD m_dwViewMode;
	SMS_SPLITTER_POS m_SmsSplitterPos;
	SMS_LIST_VIEW_MODE m_eViewMode;
	CMainFrame* m_pMainFrame;
	CToSendMessage m_SendingMsg;
	CInformProxy* m_pInformProxy;

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSCLIENTVIEW_H__35563486_39AB_4441_A769_FD3F5FFBE334__INCLUDED_)
