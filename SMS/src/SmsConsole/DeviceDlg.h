#pragma  once 

#include "resource.h"
#include "smsdlgbk.h"
#include "../SmsServer/SmsAccountManager.h"
#include "../SmsRegister/SmsRegister.h"

class CAboutDlg: public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUT};
	
	BEGIN_MSG_MAP(CAboutDlg)
		COMMAND_ID_HANDLER(IDC_OK, OnOK)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
	END_MSG_MAP()

	LRESULT OnOK(WORD ,WORD, HWND, BOOL&)
	{
		EndDialog(0);
		return 0;
	}
	LRESULT OnInitDlg(UINT nMsgType, WPARAM, LPARAM, BOOL&)
	{
		CenterWindow();
		return 0;
	}
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&)
	{
		EndDialog(0);
		return 0;
	}

};

class CMainDlg;
class CDeviceDlg: public CDialogImpl<CDeviceDlg>, public CDialogSkin	
{
public:
	enum { IDD = IDD_DEVICE };
	
	CDeviceDlg(CMainDlg* p) 
	{
		m_pMainDlg = p; 
		m_nPort = 0;
	}

	BEGIN_MSG_MAP(CDeviceDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CDialogSkin)
	END_MSG_MAP()
	LRESULT OnInitDialog(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_ctlNum = GetDlgItem(IDC_NUM);
		m_ctlNum.LimitText(90);
		m_ctlNum.SetWindowText(m_strNum);
		
		m_ctlCOm = GetDlgItem(IDC_COM_PORT);
		m_ctlCOm.SetDroppedWidth(100);
		m_ctlCOm.InsertString(0, _T("COM1"));
		m_ctlCOm.InsertString(1, _T("COM2"));
		m_ctlCOm.InsertString(2, _T("COM3"));
		m_ctlCOm.InsertString(3, _T("COM4"));
		m_ctlCOm.InsertString(4, _T("COM5"));
		m_ctlCOm.SetCurSel(m_nPort-1);
		
		SetDlgItemInt(IDC_COUNT,m_nMaxCount);
		
		CButton btn;
		btn = GetDlgItem(IDC_CLEAR);
		btn.SetCheck((m_bClear? BST_CHECKED: BST_UNCHECKED));

		return 0;
	}
	VOID SetMaxFailCount(INT n)	
	{
		m_nMaxCount = n;
	}
	VOID SetClearTag(BOOL b)
	{
		m_bClear = b;
	}
	VOID SetComPort(INT n)
	{
		m_nPort = n;
	}
	VOID SetCenterNum(TCHAR* pStr)
	{
		m_strNum = pStr;
	}
	INT GetMaxFailCount()
	{
		return GetDlgItemInt(IDC_COUNT);
	}
	BOOL GetClearTag()
	{
		CButton btn;
		btn = GetDlgItem(IDC_CLEAR);
		m_bClear = btn.GetCheck();
		if(m_bClear == BST_CHECKED)
		{
			m_bClear = TRUE;
		}
		else
		{
			m_bClear = FALSE;
		}
		return m_bClear;
	}
	INT GetCenterNum(TCHAR* pBuf, INT nBufLen)
	{  
		m_ctlNum.GetWindowText(pBuf, nBufLen);
		m_strNum = pBuf;
		return m_strNum.GetLength();
	}
	INT GetComPort()
	{
		m_nPort = m_ctlCOm.GetCurSel() + 1;
		return m_nPort;
	}
private:
	INT m_nMaxCount;
	BOOL m_bClear;
	INT m_nPort;
	CEdit m_ctlNum;
	CComboBox m_ctlCOm;
	CString m_strNum;
	CMainDlg* m_pMainDlg;
};

class CCommuDlg: public CDialogImpl<CCommuDlg>, public CDialogSkin
{
public:
	enum { IDD = IDD_COMMU };
	CCommuDlg(CMainDlg* p) 
	{ 
		m_pMainDLg = p; 
		m_bNeedLogon = TRUE;
		m_nPort = 0;
	}
	
	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CDialogSkin)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_ctlLogon = GetDlgItem(IDC_LOGON_CHECK);
		m_ctlPort = GetDlgItem(IDC_SOCKET_PORT);
		SetDlgItemInt(IDC_SOCKET_PORT,m_nPort);
		m_ctlPort.SetLimitText(10);
		m_ctlLogon.SetCheck((m_bNeedLogon? BST_CHECKED: BST_UNCHECKED));

		return 0;		
	}

	VOID SetPort(UINT n)
	{
		m_nPort = n;
	}
	UINT GetPort()
	{
		m_nPort = GetDlgItemInt(IDC_SOCKET_PORT);
		return m_nPort;
	}
	VOID SetIsNeedLogon(BOOL b)
	{
		m_bNeedLogon = b;
	}
	BOOL GetIsNeedLogon()
	{
		m_bNeedLogon = m_ctlLogon.GetCheck();
		if(m_bNeedLogon == BST_CHECKED)
		{
			m_bNeedLogon = TRUE;
		}
		else
		{
			m_bNeedLogon = FALSE;
		}
		return m_bNeedLogon;
	}

private:
	UINT m_nPort;
	BOOL m_bNeedLogon;
	CEdit m_ctlPort;
	CButton m_ctlLogon;
	CMainDlg* m_pMainDLg;
};
class CInfoDlg: public CDialogImpl<CInfoDlg> , public CDialogSkin
{
public:
	enum { IDD =  IDD_INFOR };
	BEGIN_MSG_MAP(CInfoDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_OK, OnOK)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		CHAIN_MSG_MAP(CDialogSkin)
	END_MSG_MAP()

	LRESULT OnOK(WORD, WORD,HWND,BOOL&)
	{
		EndDialog(0);
		return 0;
	}
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
	{
		CenterWindow();

		return 0;
	}
	LRESULT OnClose(UINT, WPARAM ,LPARAM ,BOOL&)
	{
		EndDialog(0);
		return 0;
	}
	LRESULT OnPaint(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{

		CRect rect;
		HWND hWnd = GetDlgItem(IDC_TEXT);
		::GetWindowRect(hWnd, &rect);
		ScreenToClient(&rect);

		CPaintDC dc(m_hWnd);
		dc.SetBkMode(TRANSPARENT);

		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(lf),&lf);
		lf.lfWeight = 300;
		lf.lfHeight = -12;
		strcpy(lf.lfFaceName, "Tahoma");
		HFONT hNewFont = CreateFontIndirect(&lf);
		HFONT hOldFont = dc.SelectFont(hNewFont);

		dc.DrawText(m_strText,m_strText.GetLength(), &rect, DT_CENTER);

		HICON hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_INFORMATION));
		dc.DrawIcon(20, rect.top, hIcon);

		
		dc.SelectFont(hOldFont);
		::DeleteObject(hNewFont);

		return 0;
	}

	VOID SetText(CString& str)
	{
		m_strText = str;
	}
private:
	CString m_strText;
};


class CAddAccountDlg: public CDialogImpl<CAddAccountDlg>,public CDialogSkin
{
public:
	enum { IDD = IDD_ADD_ACCOUNT };

	BEGIN_MSG_MAP(CAddAccountDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		COMMAND_ID_HANDLER(IDC_CANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_OK,OnOK)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		
		CHAIN_MSG_MAP(CDialogSkin)
	END_MSG_MAP()
	CAddAccountDlg(CSmsAccountManager* p, BOOL bAdd = TRUE)
	{
		m_pAccountManager = p;
		m_bAdd = bAdd;
	}
	LRESULT OnInitDlg(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CenterWindow();

		CComboBox	comb1 = GetDlgItem(IDC_DISPATCH);
		comb1.AddString(_T("是"));
		comb1.AddString(_T("否"));
		comb1.SetCurSel(0);

		CComboBox	comb2 = GetDlgItem(IDC_CAN_USE);
		comb2.AddString(_T("是"));
		comb2.AddString(_T("否"));	
		comb2.SetCurSel(0);
		
		CEdit edit;
		edit = GetDlgItem(IDC_NAME);
		edit.SetLimitText(80);
		edit = GetDlgItem(IDC_PASSWORD);
		edit.SetLimitText(80);
	
		if(!m_bAdd)
		{
			m_strOldName = m_strName;
			SetDlgItemText(IDC_NAME, m_strName);
			SetDlgItemText(IDC_PASSWORD, m_strPwd1);
			if(m_bDispatch)
			{
				comb1.SetCurSel(0);
			}
			else
			{
				comb1.SetCurSel(1);
			}
			if(m_bCanUse)
			{
				comb2.SetCurSel(0);
			}
			else
			{
				comb2.SetCurSel(1);
			}
			SetWindowText(_T("修改账号"));
		}
		return 0;
	}
	INT CheckIsValid()
	{
		INT nRet(0);
		m_strName.TrimLeft();
		m_strName.TrimRight();
		m_strPwd1.TrimLeft();
		m_strPwd1.TrimRight();


		if(!m_strName.IsEmpty()) 
		{
			if(!m_strPwd1.IsEmpty())// && !m_strPwd2.IsEmpty())
			{
// 				if(m_strPwd1 != m_strPwd2)
// 				{
// 					nRet = 3;
// 					goto _END;
// 				}
			}
			else
			{
				nRet = 2;
				goto _END;
			}
		}
		else
		{
			nRet = 1;
			goto _END;
		}
		
		if( m_bAdd && !(m_pAccountManager->AddAccount(m_strName,m_strPwd1,m_bCanUse,m_bDispatch)))
		{
			nRet = 4;
			goto _END;
		}
		if(!m_bAdd && !(m_pAccountManager->ModifyAccount(m_strOldName, m_strName, m_strPwd1, m_bCanUse, m_bDispatch)))
		{
			nRet = 5;
			goto _END;
		}
_END:
		return nRet;
	}
	VOID GetValue()
	{
		TCHAR szBuf[100] = {0};
		GetDlgItemText(IDC_NAME, szBuf, 100);
		m_strName = szBuf;
		GetDlgItemText(IDC_PASSWORD, szBuf, 100);
		m_strPwd1 = szBuf;
		
		CComboBox comb;
		comb = GetDlgItem(IDC_DISPATCH);
		if(comb.GetCurSel() == 0)
		{
			m_bDispatch = TRUE;
		}
		else
		{
			m_bDispatch = FALSE;
		}
		comb = GetDlgItem(IDC_CAN_USE);
		if(comb.GetCurSel() == 0)
		{
			m_bCanUse =TRUE;
		}
		else
		{
			m_bCanUse = FALSE;
		}
	}
	LRESULT OnOK(WORD, WORD, HWND, BOOL&)
	{
		GetValue();
		INT n = CheckIsValid();
		TCHAR szBuf[MAX_PATH] = {0};	
		switch(n)
		{
		case 0:
			CloseDialog(1);
			return 0;
			break;
		case 1:
			::LoadString(GetModuleHandle(NULL),IDS_NAME_EMPTY, szBuf, MAX_PATH);
			break;
		case 2:
			::LoadString(GetModuleHandle(NULL),IDS_PWD_EMPTY,szBuf,MAX_PATH);
			break;
		case 5:
			::LoadString(GetModuleHandle(NULL),IDS_MODIFY_NAME_EXIST,szBuf, MAX_PATH);
			break;
		case 4:
			::LoadString(GetModuleHandle(NULL), IDS_NAME_EXIST,szBuf, MAX_PATH);
			break;
		}
		
		CInfoDlg dlg;
		dlg.SetText(CString(szBuf));
		dlg.DoModal();

		return 0;
	}
	VOID CloseDialog(UINT n)
	{
		EndDialog(n);
	}
	LRESULT OnCancel(WORD ,WORD, HWND ,BOOL&)
	{
		CloseDialog(0);	
		return 0;
	}
	LRESULT OnClose(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CloseDialog(0);
		return 0;
	}

public:
	BOOL m_bAdd;
	CString m_strName;
	CString m_strPwd1;
	BOOL m_bDispatch;
	BOOL m_bCanUse;
	CString m_strOldName;

	CSmsAccountManager* m_pAccountManager;
};
class CQuesDlg: public CDialogImpl<CQuesDlg> , public CDialogSkin
{
public:
	enum { IDD =  IDD_QUES };
	BEGIN_MSG_MAP(CQuesDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_QUES_YES, OnYes)
		COMMAND_ID_HANDLER(IDC_QUES_NO, OnNo)
		CHAIN_MSG_MAP(CDialogSkin)
	END_MSG_MAP()

	LRESULT OnYes(WORD, WORD,HWND,BOOL&)
	{
		EndDialog(1);
		return 0;
	}
	LRESULT OnNo(WORD, WORD,HWND,BOOL&)
	{
		EndDialog(0);
		return 0;
	}
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
	{
		CenterWindow();

		return 0;
	}

	LRESULT OnClose(UINT, WPARAM ,LPARAM ,BOOL&)
	{
		EndDialog(0);
		return 0;
	}

};
class CAccountDlg: public CDialogImpl<CAccountDlg>,public CDialogSkin
{
public:
	enum { IDD = IDD_ACCOUNT };
	CAccountDlg(CMainDlg* p, CSetupDlg* p1) { m_pMainDlg = p; m_pSetDlg = p1; }
	
	BEGIN_MSG_MAP(CAccountDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		COMMAND_ID_HANDLER(IDC_DELETE, OnDelete)
		COMMAND_ID_HANDLER(IDC_ADD, OnAddAccount)
		COMMAND_ID_HANDLER(IDC_MODIFY, OnModifyAccount)
		CHAIN_MSG_MAP(CDialogSkin)
	END_MSG_MAP()
	
	LRESULT OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_imagelist.Destroy();
	
		return 0;
	}
	LRESULT OnDelete(WORD, WORD, HWND, BOOL&)
	{
		if(m_ctlList.GetSelectedCount() > 0)
		{
			CQuesDlg dlg;
			
			INT nRet = dlg.DoModal();
			DWORD nState;
			TCHAR szBuf[100] = {0};
			CSmsAccountManager* pManager = m_pSetDlg->GetAccountManager();

			if(nRet == 1)
			{
				for( INT i =0; i < m_ctlList.GetItemCount(); i++)
				{
					 nState = m_ctlList.GetItemState(i, LVIS_SELECTED);

					if( nState & LVIS_SELECTED)
					{
						m_ctlList.GetItemText(i, 1, szBuf,100);
						pManager->RemoveAccount(szBuf);
					}
				}

				LoadContent();
				
			}
		}
		return 0;
	}
	LRESULT OnModifyAccount(WORD, WORD, HWND, BOOL&)
	{
		BOOL bSel(FALSE);
		CAddAccountDlg dlg(m_pSetDlg->GetAccountManager(), FALSE);
		INT i(0);
		for(i=0; i<m_ctlList.GetItemCount(); i++)
		{
			if(m_ctlList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
			{
				bSel = TRUE;
				break;
			}
		}

		if(bSel)
		{
			TCHAR szBuf[MAX_PATH] = {0};
			m_ctlList.GetItemText(i, 1, szBuf, MAX_PATH);
			dlg.m_strName = szBuf;
			m_ctlList.GetItemText(i, 2, szBuf, MAX_PATH);
			if(lstrcmpi(szBuf, _T("是")) == 0)
			{
				dlg.m_bDispatch = TRUE;
			}
			else
			{
				dlg.m_bDispatch	 = FALSE;
			}
			m_ctlList.GetItemText(i, 3, szBuf, MAX_PATH);
			if(lstrcmpi(szBuf, _T("是")) == 0)
			{
				dlg.m_bCanUse = TRUE;
			}
			else
			{
				dlg.m_bCanUse = FALSE;
			}
			m_ctlList.GetItemText(i, 4, szBuf, MAX_PATH);
			dlg.m_strPwd1 = szBuf;

			if(dlg.DoModal() == 1)
			{
				LoadContent();
			}
		}
		return 0;
	}
	LRESULT OnAddAccount(WORD, WORD, HWND, BOOL&)
	{
		CAddAccountDlg dlg(m_pSetDlg->GetAccountManager());
		if(dlg.DoModal() == 1)
		{
			LoadContent();
		}
		return 0;
	}
	VOID LoadContent()
	{
		m_ctlList.DeleteAllItems();

		CSmsAccount* pAccount;
		CSmsAccountManager* pManager = m_pSetDlg->GetAccountManager();
		if(pManager == NULL) return ;
		
		INT nIndex(0);
		pAccount = pManager->GetFirst();

		while(pAccount != NULL)
		{
// 			if(pAccount->m_bLogoned)
// 			{
// 				m_ctlList.AddItem(nIndex,0,_T(""),1);
// 			}
// 			else
// 			{
				m_ctlList.AddItem(nIndex,0,_T(""),0);
//			}
			
			m_ctlList.AddItem(nIndex,1,pAccount->m_strName);

			if(pAccount->m_bDispatch)
			{
				m_ctlList.AddItem(nIndex,2,_T("是"));
			}
			else
			{
				m_ctlList.AddItem(nIndex,2,_T("否"));
			}

			if(pAccount->m_bCanUse)
			{
				m_ctlList.AddItem(nIndex,3,_T("是"));
			}
			else
			{
				m_ctlList.AddItem(nIndex,3,_T("否"));
			}
			
			m_ctlList.AddItem(nIndex,4,pAccount->m_strPassword);

			pAccount = pManager->GetNext();
			nIndex++;	
		}
	}
	LRESULT OnInitDialog(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	    m_imagelist.Create(MAKEINTRESOURCE(IDB_ACCOUNT_HEAD),16,0,RGB(255,0,255));

		m_ctlList = GetDlgItem(IDC_ACCOUNT_LIST);
		m_ctlList.SetImageList(m_imagelist,LVSIL_SMALL);
	
		
		DWORD dwExStyle = m_ctlList.GetExtendedListViewStyle();
		dwExStyle |=  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES;
		m_ctlList.SetExtendedListViewStyle(dwExStyle);

		TCHAR szBuf[100] = {0};
		HINSTANCE hMod = m_pMainDlg->GetResourceInstance();
		LoadString(hMod,IDS_ACCOUNT_NAME, szBuf, 100);
		
		LVCOLUMN lvc = { 0 };
		lvc.mask =  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.fmt = LVCFMT_CENTER;

		lvc.pszText = (LPTSTR)_T("");
		lvc.cx =m_ctlList.GetStringWidth(_T("")) + 20;
		m_ctlList.InsertColumn(0,&lvc);

		lvc.pszText = szBuf;
		lvc.cx = m_ctlList.GetStringWidth(szBuf) + 15;
		m_ctlList.InsertColumn(1,&lvc);

		LoadString(hMod, IDS_ACCOUNT_DISPATCH, szBuf, 100);
		lvc.pszText = szBuf;
		lvc.cx = m_ctlList.GetStringWidth(szBuf) + 15;
		m_ctlList.InsertColumn(2,&lvc);

		LoadString(hMod,IDS_ACCOUNT_CANUSE,szBuf, 100);
		lvc.pszText = szBuf;
		lvc.cx = m_ctlList.GetStringWidth(szBuf) + 15;
		m_ctlList.InsertColumn(3,&lvc);

		LoadString(hMod,IDS_ACCOUNT_PASSWORD, szBuf, 100);
		lvc.pszText = szBuf;
		lvc.cx = 0;
		m_ctlList.InsertColumn(4,&lvc);
		
		LoadContent();
		
		return 0;
	}
private:
	CImageList m_imagelist;
	CListViewCtrl m_ctlList;
	CMainDlg* m_pMainDlg;
	CSetupDlg* m_pSetDlg;
};

class CRegisterDlg:
public CDialogImpl<CRegisterDlg>,
public CDialogSkin
{
public:	

	enum { IDD = IDD_REGISTER };

	BEGIN_MSG_MAP(CRegisterDlg)
		
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		COMMAND_ID_HANDLER(IDC_REGISTER, OnRegister)
		CHAIN_MSG_MAP(CDialogSkin)
		
	END_MSG_MAP()
	
	CRegisterDlg(CMainDlg* p)
	{
		m_pMainDlg = p;
	}

	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&)
	{
		SetDlgItemText(IDC_EMAIL, _T("decent_guhu@sina.com"));
		SetDlgItemText(IDC_QQ, _T("55841310"));

		CString strMachineCode;
		GetMachineCode(strMachineCode);
		SetDlgItemText(IDC_MACHINE_CODE, strMachineCode);
		if(!CheckIsRegistered())
		{
			SetDlgItemText(IDC_STATUS, _T("未注册(试用版)"));
			::EnableWindow(GetDlgItem(IDC_REGISTER), TRUE);
		}
		else
		{
			SetDlgItemText(IDC_STATUS, _T("已注册(正式版)"));
			::EnableWindow(GetDlgItem(IDC_REGISTER), FALSE);
			CString strRegisterCode;
			GetRegisterCodeByMachineCode(strMachineCode, strRegisterCode);
			SetDlgItemText(IDC_REGISTER_CODE, strRegisterCode);
		}

		return 0;
	}

	LRESULT OnRegister(WORD, WORD, HWND, BOOL& bHandled)
	{
		CString strMachineCode, strRegisterCode;
		GetDlgItemText(IDC_MACHINE_CODE, strMachineCode.GetBuffer(100), 90);
		GetDlgItemText(IDC_REGISTER_CODE, strRegisterCode.GetBuffer(100), 90);
		strMachineCode.ReleaseBuffer();
		strRegisterCode.ReleaseBuffer();

		CString strInfo;
		if(RegisterAccount(strMachineCode, strRegisterCode))
		{
			SetDlgItemText(IDC_STATUS, _T("已注册(正式版)"));
			::EnableWindow(GetDlgItem(IDC_REGISTER), FALSE);
			strInfo = _T("注册成功!");
		}
		else
		{
			strInfo = _T("注册失败, 请输入正确的注册码!");
		}


		m_pMainDlg->ResetCaption();	

		CInfoDlg dlg;
		dlg.SetText(strInfo);
		dlg.DoModal();

	

		return 0;
	}

private:
	CMainDlg* m_pMainDlg;
};