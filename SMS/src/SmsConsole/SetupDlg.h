#pragma  once 

#include "resource.h"
#include "SmsDlgBk.h"
#include <atlctrls.h>
#include <atlmisc.h>

class CDeviceDlg;
class CCommuDlg;
class CMainDlg;
class CAccountDlg;
class CConsoleConfig;
class CRegisterDlg;

//class COtherDlg;
class CSmsAccountManager;

class CSetupDlg: public CDialogImpl<CSetupDlg>,
				 public CDialogSkin	
{
public:
	enum { IDD = IDD_SETUP };
	
	CSetupDlg(CMainDlg* p);
	~CSetupDlg();

	BEGIN_MSG_MAP(CSetupDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_LIST, LBN_SELCHANGE, OnSelectChange)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		
		COMMAND_ID_HANDLER(IDC_OK, OnOK)
		COMMAND_ID_HANDLER(IDC_CANCEL,OnCancel)
		COMMAND_ID_HANDLER(IDC_APPLY, OnApply)
		CHAIN_MSG_MAP(CDialogSkin)
	END_MSG_MAP()
	
	LRESULT OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnInitDialog(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSelectChange(WORD,WORD,HWND,BOOL&);
	
	LRESULT OnOK(WORD,WORD,HWND,BOOL&);
	LRESULT OnCancel(WORD,WORD,HWND,BOOL&);
	LRESULT OnApply(WORD,WORD,HWND,BOOL&);
	
	CSmsAccountManager* GetAccountManager() {return m_pAccountManager;}
private:
	BOOL SaveAllConfig();
	VOID ShowCurrentSelect();

private:
	CMainDlg* m_pMainDlg;
	CDeviceDlg* m_pDeviceDlg;
	CCommuDlg* m_pCommuDlg;
	CAccountDlg* m_pAccountDlg;
	CRegisterDlg* m_pRegisterDlg;
//	COtherDlg* m_pOtherDlg;
	CListBox m_list;
	CRect m_rectChild;
	CSmsAccountManager* m_pAccountManager;

	CConsoleConfig* m_pConfig;
};