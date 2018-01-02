#pragma  once 

#include "resource.h"
#include "ToolbarHelper.h"
#include <atlwin.h>
#include <atlctrls.h>
#include <atlsplit.h>

class CMessage
{
public:
	CMessage();
	CMessage(const CMessage& msg);
	CMessage& operator = (const CMessage& msg);
public:
	INT m_nId;
	INT m_nTypeId;
	BOOL m_bType;
	CString m_strContent;
};

class CMessageLibDlg;

class CMessageLib
{
public:
	~CMessageLib();
	BOOL LoadMsg();
	VOID ClearAll();

	BOOL AddMessage(CMessage* pMsg);
	BOOL ModifyMessage(CMessage* pMsg);
	BOOL DeleteMessage(CMessage* pMsg);

	CSimpleMap<CMessage*, CSimpleArray<CMessage*>* >& GetMsgMap() {return m_map;}
private:
	CSimpleMap<CMessage*, CSimpleArray<CMessage*>* > m_map;
};

typedef CWinTraitsOR<LVS_REPORT  | LVS_NOSORTHEADER | LVS_SINGLESEL | LVS_SHOWSELALWAYS> CListTraits2;
class CTypeList :public CWindowImpl<CTypeList, CListViewCtrl, CListTraits2>
{
public:
	BEGIN_MSG_MAP(CTypeList)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()
	
	CTypeList(CMessageLibDlg* p) {m_pDlg = p;}
	LRESULT OnCreate(UINT nMsgType , WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	VOID LoadTypes();
private:
	CImageList m_imagelist;
	CMessageLibDlg* m_pDlg;
};

typedef CWinTraitsOR<LVS_REPORT  | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS> CListTraits1;
class CMessageList :public CWindowImpl<CMessageList, CListViewCtrl, CListTraits1>
{
public:
	BEGIN_MSG_MAP(CMessageList)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	CMessageList(CMessageLibDlg* p) {m_pDlg = p;}
	LRESULT OnCreate(UINT nMsgType , WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	VOID LoadMessage(INT nIndex);

private:
	CImageList m_imagelist;
	CMessageLibDlg* m_pDlg;
};

typedef CWinTraitsOR<ES_MULTILINE | WS_VSCROLL > CEditTrans;
class CContentEdit: public CWindowImpl<CContentEdit, CEdit, CEditTrans>
{
public:
	BEGIN_MSG_MAP(CContentEdit)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
	//	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()
	CContentEdit()
	{
		m_hFont = NULL;
	}
	LRESULT OnCreate(UINT nMsgType, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
// 		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
// 		if(hFont != NULL)
// 		{
// 			LOGFONT lf;
// 			GetObject(hFont, sizeof(lf), &lf);
// 		//	lf.lfWeight = FW_NORMAL;
// 			 m_hFont = CreateFontIndirect(&lf);
// 		}
		return 0;
	}
// 	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
// 	{
// 		if(m_hFont != NULL)
// 		{
// 			DeleteObject(m_hFont);
// 			m_hFont = NULL;
// 		}
// 		return 0;
// 	}
// 	VOID SetDefaultFont()
// 	{
// 		if(m_hFont != NULL)
// 		{
// 			SetFont(m_hFont);
// 		}
// 	}
	
private:
	HFONT m_hFont;
};

class CTransferMsgDlg: public CDialogImpl<CTransferMsgDlg>
{
public:
	enum {IDD = IDD_TRANSFER};
	
	BEGIN_MSG_MAP(CTransferMsgDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_OK, OnOK)
		COMMAND_ID_HANDLER(IDC_CANCEL, OnCancel)
	END_MSG_MAP()
	
	CTransferMsgDlg(CMessageLibDlg* pDlg, INT nIndex);
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT ,WPARAM, LPARAM, BOOL&);
	LRESULT OnOK(WORD, WORD, HWND, BOOL&);
	LRESULT OnCancel(WORD, WORD, HWND, BOOL&);
public:
	INT m_nIndex;
	CMessageLibDlg* m_pMsgLibDlg;
};

class CMessageDlg: public CDialogImpl<CMessageDlg>
{
public:
	enum {IDD = IDD_MESSAGE};

	BEGIN_MSG_MAP(CMessageDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose);
		
		COMMAND_ID_HANDLER(IDC_OK, OnOK)
		COMMAND_ID_HANDLER(IDC_CANCEL, OnCancel)
	END_MSG_MAP()
	
	CMessageDlg(CString& strContent, INT nIndex, BOOL bAdd, CSimpleMap<CMessage*, CSimpleArray<CMessage*>*>& map);

	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnOK(WORD, WORD, HWND, BOOL&);
	LRESULT OnCancel(WORD, WORD, HWND, BOOL&);

public:
	CString m_strContent;
	BOOL m_bAdd;
	//CMessageLibDlg* m_pMsgLibDlg;
	INT m_nId;
	INT m_nTypeId;
	INT m_nIndex;
	CSimpleMap<CMessage*, CSimpleArray<CMessage*>*>& m_map;
};

class CSmsSendDlg;
class CMessageLibDlg: public CDialogImpl<CMessageLibDlg>,
						  private CToolBarHelper<CMessageLibDlg>
{
public:
	enum {IDD = IDD_MESSAGE_LIB};
	
	enum
	{
		ID_TYPE_LIST = 1,
		ID_MESSAGE_LIST = 2,
		ID_CONTENT = 3
	};

	BEGIN_MSG_MAP(CMessageLibDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		
		COMMAND_ID_HANDLER(ID_ADD_TYPE, OnAddType)
		COMMAND_ID_HANDLER(ID_MODIFY_TYPE, OnModifyType)
		COMMAND_ID_HANDLER(ID_DELETE_TYPE, OnDeleteType)
		COMMAND_ID_HANDLER(ID_EXIT, OnExit)
		
		COMMAND_ID_HANDLER(ID_ADD_MESSAGE, OnAddMessage)
		COMMAND_ID_HANDLER(ID_MODIFY_MESSAGE, OnModifyMessage)
		COMMAND_ID_HANDLER(ID_DELETE_MESSAGE, OnDeleteMessage)
		COMMAND_ID_HANDLER(ID_TRANSFER_MESSAGE, OnTransferMsg)
		COMMAND_ID_HANDLER(ID_INSERT_CONTENT, OnInsertContent)

		NOTIFY_ID_HANDLER(ID_TYPE_LIST, OnTypeListNotify)
		NOTIFY_ID_HANDLER(ID_MESSAGE_LIST, OnMessageListNotify)
	END_MSG_MAP()
	
	void OnToolBarCombo(HWND hWndCombo, UINT nID, int nSel, LPCTSTR lpszText, DWORD dwItemData){}
	virtual void PrepareToolBarMenu(UINT nMenuID, HMENU hMenu){}
	CMessageLibDlg(CSmsSendDlg* pSendDlg = NULL);
	~CMessageLibDlg();

	LRESULT OnInitDlg(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExit(WORD, WORD, HWND, BOOL&);
	CMessageLib& GetMsgLib() {return m_msgLib;}
	
	LRESULT OnTypeListNotify(int, LPNMHDR, BOOL&);
	LRESULT OnMessageListNotify(int, LPNMHDR, BOOL&);

	LRESULT	 OnAddType(WORD, WORD, HWND, BOOL&);
	LRESULT	 OnModifyType(WORD, WORD, HWND, BOOL&);
	LRESULT	 OnDeleteType(WORD, WORD, HWND, BOOL&);
	LRESULT  OnInsertContent(WORD, WORD, HWND, BOOL&);

	LRESULT OnAddMessage(WORD, WORD, HWND, BOOL&);
	LRESULT OnModifyMessage(WORD, WORD, HWND, BOOL&);
	LRESULT OnDeleteMessage(WORD, WORD, HWND, BOOL&);
	LRESULT OnTransferMsg(WORD, WORD, HWND, BOOL&);
protected:
	BOOL GetCurrentSelInfo(CString& strContent);
	VOID SetSelectedType(INT nIndex);
	VOID UpdateLayout();
	VOID Close();
	VOID SetItemImage(CListViewCtrl& list, INT nIndex, INT nImage);
private:
	CSmsSendDlg* m_pSendDlg;
	CTypeList* m_pTypeList;
	CSplitterWindow m_wndSplitLR;
	CMessageList* m_pMessageList;
	CSplitterWindowT<FALSE> m_wndSplitTB;
	CContentEdit* m_pContent;
	HWND m_hWndToolBar;
	HWND m_hWndStatusBar;
	HWND m_hWndClient;
	CMessageLib m_msgLib;
	INT m_nCurrentIndex;

};