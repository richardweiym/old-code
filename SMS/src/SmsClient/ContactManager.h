#pragma once 

#include <atlwin.h>
#include <atlmisc.h>
#include "DBOperate.h"
#include "resource.h"
#include <atlsplit.h>
#include "ToolbarHelper.h"

class CContact
{
public:
	CContact();
	CContact(const CContact& c);
	CContact& operator = (const CContact& c);
public:
	INT m_nId;
	CString m_strName;
	CString m_strTel;
	INT m_nParentId;
	BOOL m_bLeaf;
};

class CContactManager
{
public:
	CContactManager();
	~CContactManager();
	
	CContact* GetFirstTeam();
	CContact* GetNextTeam();

	CContact* GetFirstContact(INT nParentId);
	CContact* GetNextContact();

	BOOL AddContact(CContact* pContact);
	BOOL ModifyContact(CContact* pContact);
	BOOL DeleteContact(CContact* pContact);
	BOOL CheckIsExist(CContact* pContact, BOOL bAddORModify = TRUE);
	
	INT GetContactCount(CSimpleArray<CContact*>& arTeam);
	INT CheckTeamByName(CString& strName);

	static BOOL GetNameByTel(CString& Tel, CString& strName);
private:
	VOID GetCurrentContact();
	VOID CloseRecordset();
	
private:
	_RecordsetPtr m_pRst;

	CContact* m_pCurrent;
};

class CSmsContactTree;
typedef CWinTraitsOR<LVS_REPORT  | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS> CListTraits;
class CContactList :public CWindowImpl<CContactList, CListViewCtrl, CListTraits>
{
public:
	BEGIN_MSG_MAP(CSmsListView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	LRESULT OnCreate(UINT nMsgType , WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	VOID UpdateData(CSmsContactTree* pTree);
	BOOL GetContactByIndex(INT nIndex, CContact& c);
private:
	VOID AddContactItem(CContact* pContact);

private:
	CImageList m_imagelist;
};

class CTeamDlg: public CDialogImpl<CTeamDlg>
{
public:
	enum {IDD = IDD_TEAM};
	
	BEGIN_MSG_MAP(CTeamDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_OK, OnOK)
		COMMAND_ID_HANDLER(IDC_CANCEL, OnCancel)
	END_MSG_MAP()
	
	CTeamDlg(LPCTSTR strName, BOOL bAdd);
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT ,WPARAM, LPARAM, BOOL&);
	LRESULT OnOK(WORD, WORD, HWND, BOOL&);
	LRESULT OnCancel(WORD, WORD, HWND, BOOL&);
public:
	CString m_strName;
	BOOL m_bAdd;
};
class CContactDlg: public CDialogImpl<CContactDlg>
{
public:
	enum {IDD = IDD_CONTACT};
	
	BEGIN_MSG_MAP(CContactDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_OK, OnOK)
		COMMAND_ID_HANDLER(IDC_CANCEL, OnCancel)
	END_MSG_MAP()
	
	CContactDlg(LPCTSTR strName, LPCTSTR strTel, INT nId, CSimpleMap<HTREEITEM, CContact*>& map, BOOL bAdd);
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT ,WPARAM, LPARAM, BOOL&);
	LRESULT OnOK(WORD, WORD, HWND, BOOL&);
	LRESULT OnCancel(WORD, WORD, HWND, BOOL&);
public:
	CSimpleMap<HTREEITEM, CContact*>& m_ItemMap;
	CString m_strName;
	CString m_strTel;
	INT m_nId;
	BOOL m_bAdd;
	BOOL m_bChanged;
};
class CTransferDlg: public CDialogImpl<CTransferDlg>
{
public:
	enum {IDD = IDD_TRANSFER};
	
	BEGIN_MSG_MAP(CTransferDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_OK, OnOK)
		COMMAND_ID_HANDLER(IDC_CANCEL, OnCancel)
	END_MSG_MAP()
	
	CTransferDlg(INT nId, CSimpleMap<HTREEITEM, CContact*>& map);
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT ,WPARAM, LPARAM, BOOL&);
	LRESULT OnOK(WORD, WORD, HWND, BOOL&);
	LRESULT OnCancel(WORD, WORD, HWND, BOOL&);
public:
	INT m_nId;
	CSimpleMap<HTREEITEM, CContact*>& m_ItemMap;
};

class CSmsContactTree;
class CImportManager;
class CContactManagerDlg: public CDialogImpl<CContactManagerDlg>,
						  private CToolBarHelper<CContactManagerDlg>
{
public:
	enum {IDD = IDD_CONTACT_MANAGER};
	
	enum
	{
		ID_CONTACT_TREE = 1,
		ID_CONTACT_LIST = 2
	};

	BEGIN_MSG_MAP(CContactManagerDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		
		COMMAND_ID_HANDLER(ID_ADD_TEAM, OnAddTeam)
		COMMAND_ID_HANDLER(ID_MODIFY_TEAM, OnModifyTeam)
		COMMAND_ID_HANDLER(ID_DELETE_TEAM, OnDeleteTeam)
		COMMAND_ID_HANDLER(ID_ADD_CONTACT, OnAddContact)
		COMMAND_ID_HANDLER(ID_MODIFY_CONTACT, OnModifyContact)
		COMMAND_ID_HANDLER(ID_DELETE_CONTACT, OnDeleteContact)
		COMMAND_ID_HANDLER(ID_TRANSFER_CONTACT, OnTransferContact)
		COMMAND_ID_HANDLER(ID_SMS_REPLY, OnSmsSendMsg)
		COMMAND_ID_HANDLER(ID_EXPORT, OnExport)
		COMMAND_ID_HANDLER(ID_IMPORT, OnImport)
		COMMAND_ID_HANDLER(ID_EXIT, OnExit)

		NOTIFY_ID_HANDLER(ID_CONTACT_TREE, OnTreeNotify)
		NOTIFY_ID_HANDLER(ID_CONTACT_LIST, OnListNotify)
	END_MSG_MAP()
	
	void OnToolBarCombo(HWND hWndCombo, UINT nID, int nSel, LPCTSTR lpszText, DWORD dwItemData);
	virtual void PrepareToolBarMenu(UINT nMenuID, HMENU hMenu);
	CContactManagerDlg();
	~CContactManagerDlg();

	LRESULT OnSmsSendMsg(WORD, WORD, HWND, BOOL&);
	LRESULT OnInitDlg(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTreeNotify(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnListNotify(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnAddTeam(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnModifyTeam(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnDeleteTeam(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnExit(WORD, WORD, HWND, BOOL& bHandled);	
	LRESULT OnAddContact(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnModifyContact(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnDeleteContact(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnTransferContact(WORD, WORD, HWND, BOOL& bHandled);	

	VOID SetModifyFlag(BOOL b) {m_bModifyFlag = b;}
	BOOL GetModifyFlag() {return m_bModifyFlag;}
	
	LRESULT OnExport(WORD, WORD, HWND, BOOL&);
	LRESULT OnImport(WORD, WORD ,HWND, BOOL&);

	VOID ReloadData(INT nId);
	static CContactManagerDlg* GetContactManagerDlg() {return m_pMangerDlg;}
protected:
	BOOL GetCurrentSelInfo(CString& strName, CString& strTel);
	VOID Close();
	VOID UpdateLayout();
	BOOL GetImportFileName(CString& strName);

private:
	static CContactManagerDlg* m_pMangerDlg;
	CSmsContactTree* m_pContactTree;
	CSplitterWindow m_wndSplit;
	CContactList* m_pContactList;
	CImportManager* m_pImportManager;
	HWND m_hWndToolBar;
	HWND m_hWndStatusBar;
	HWND m_hWndClient;
	
	BOOL m_bModifyFlag;
};

class CExcelDB
{
public:
	CExcelDB();
	~CExcelDB();
	VOID SetExcelFile(CString& strFile);
	CDBOperate* GetDBOperate();

private:
	CString m_strFile;
	CDBOperate* m_pOperate;
};

class CImportExcelDlg: public CDialogImpl<CImportExcelDlg>
{
public:
	CImportExcelDlg();
	~CImportExcelDlg();
	enum { IDD = IDD_IMPORT_EXCEL };
	BEGIN_MSG_MAP(CImportExcelDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDC_SHEET, CBN_SELCHANGE, OnSheetSelectChanged)
		COMMAND_ID_HANDLER(IDC_CLOSE, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_IMPORT, OnImport)
	END_MSG_MAP()
	
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCloseCmd(WORD, WORD, HWND, BOOL&);
	LRESULT OnSheetSelectChanged(WORD, WORD, HWND, BOOL&);
	LRESULT OnImport(WORD, WORD, HWND, BOOL&);

	VOID DoImport(CString& strFile);
private:
	BOOL ParseExcel(CString& strFile);
	BOOL LoadTeam();
	VOID ClearData();

	VOID StartImport(CString& strSheet, CString& strName, CString& strTel, INT nTeamId);

private:
	BOOL m_bImporting;
	CExcelDB m_ExcelDB;
	CSimpleMap<CString, CSimpleArray<CString>* > m_mapSheet;
	CSimpleArray<CContact*> m_arTeam;
};
	
class CImportXmlDlg: public CDialogImpl<CImportXmlDlg>
{
public:
	enum { IDD = IDD_IMPORT_XML };

	BEGIN_MSG_MAP(CImportXmlDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_CLOSE, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_IMPORT, OnImport)
		COMMAND_ID_HANDLER(IDC_ALL_CHECK, OnAllCheck)
		NOTIFY_ID_HANDLER(IDC_TREE, OnContactNotify)
	END_MSG_MAP()

	CImportXmlDlg();
	VOID DoImport(CString& strFile);
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCloseCmd(WORD, WORD, HWND, BOOL&);
	LRESULT OnImport(WORD, WORD ,HWND, BOOL&);
	LRESULT OnAllCheck(WORD, WORD, HWND, BOOL&);
	LRESULT OnContactNotify(int, LPNMHDR pnmh, BOOL&);

private:
	VOID ImportFile(CSimpleArray<CString>& arTeam);
	BOOL ParseXML(CString& strFile);	

private:
	CSimpleArray<CString> m_arTeam;
	CString m_strFile;
	CTreeViewCtrl m_teamTree;
	BOOL m_bImporting;
};

class CExportXmlDlg: public CDialogImpl<CExportXmlDlg>
{
public:
	enum { IDD = IDD_EXPORT_XML };

	BEGIN_MSG_MAP(CExportXmlDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_CLOSE, OnCloseCmd)
		NOTIFY_ID_HANDLER(IDC_TREE, OnContactNotify)
		COMMAND_ID_HANDLER(IDC_ALL_CHECK, OnAllCheck)
		COMMAND_ID_HANDLER(IDC_EXPLORE, OnExplore)
		COMMAND_ID_HANDLER(IDC_EXPORT, OnExport)
	END_MSG_MAP()
	
	CExportXmlDlg();
	~CExportXmlDlg();
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCloseCmd(WORD, WORD, HWND, BOOL&);
	LRESULT OnExplore(WORD, WORD, HWND, BOOL&);
	VOID DoExport();
	LRESULT OnContactNotify(int, LPNMHDR pnmh, BOOL&);
	LRESULT OnAllCheck(WORD, WORD, HWND, BOOL&);
	LRESULT OnExport(WORD, WORD, HWND, BOOL&);
	VOID ExportFile(CString& strFile, CSimpleArray<CContact*>& arTeam);
private:
	BOOL LoadTeam();
	VOID ClearData();
	VOID ClearArray(CSimpleArray<CContact*>& arTeam);

private:
	CTreeViewCtrl m_teamTree;
	CSimpleArray<CContact*> m_arTeam;
	BOOL m_bExporting;
};

class IImportContact
{
public:
	virtual VOID DoImport(CString& strFile) = 0;
	virtual ~IImportContact() {}
};

class CImportBroker
{
public:
	BOOL RegisterImport( CString& strExt, IImportContact* pImport);
	BOOL DoImport(CString& strFile);

	VOID ClearAll();
private:
	BOOL  GetFileExt(CString& strFile, CString& strExt);
private:
	CSimpleMap<CString, IImportContact*> m_mapImport;
};

class CImportManager
{
public:
	CImportManager();
	BOOL DoImport(CString& strFile);
	TCHAR* GetFilter() {return m_szFilter;}
	~CImportManager();
private:
	VOID LoadAllImport();

private:
	CImportBroker m_broker;
	TCHAR m_szFilter[512];
	BOOL m_bInited;

};

class CExcelImport: public IImportContact
{
public:
	virtual VOID DoImport(CString& strFile);
};

class CXmlImport: public IImportContact
{
public:
	 virtual VOID DoImport(CString& strFile);
};