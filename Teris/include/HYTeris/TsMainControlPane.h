#pragma  once 

#include "TsStdAfx.h"
#include "TsDialogBase.h"
#include "TerisRes.h"
#include "TsUIDataMgr.h"

class CTsControlPaneFrame;
class CTsSubControlContainer: public CWindowImpl<CTsSubControlContainer>
{
public:
	DECLARE_WND_CLASS(_T("TsSubControl"))
	BEGIN_MSG_MAP(CTsSubControlContainer)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		FORWARD_NOTIFICATIONS() //send message to parent window
	END_MSG_MAP()
	
	CTsSubControlContainer();
	~CTsSubControlContainer();
	
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);

	void SetView(HWND hWnd);
	HWND GetView() { return m_hWndView; }

	void SetTitleText(LPCTSTR lpszTitle);
	void SetTitleIcon(HBITMAP bmp);

protected:
	void _AdjustView();

private:
	CTsControlPaneFrame* m_pFrame;
	HWND m_hWndView;
};

class CTsControlTypeDlg: public CTsDialogBase<CTsControlTypeDlg>,
						 public CTsGameBaseInfoObserver<CTsControlTypeDlg>
{
public:
	enum { IDD = IDD_SINGLE_TYPE_DLG };
	BEGIN_MSG_MAP(CTsControlTypeDlg)
		CHAIN_MSG_MAP(CTsGameBaseInfoObserver<CTsControlTypeDlg>)
		CHAIN_MSG_MAP(CTsDialogBase<CTsControlTypeDlg>)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()
	
	CSize GetIdealSize();
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& );

	VOID DoUIDataChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled);
private:
	void _AdjustSize();
	void _CheckTypeMode();
	
};


class CTsControlOptionDlg: public CTsDialogBase<CTsControlOptionDlg>,
						   public CTsGameBaseInfoObserver<CTsControlOptionDlg>
{
public:
	enum { IDD = IDD_GAME_OPTION_DLG };
	BEGIN_MSG_MAP(CTsControlOptionDlg)
		CHAIN_MSG_MAP(CTsGameBaseInfoObserver<CTsControlOptionDlg>)
		CHAIN_MSG_MAP(CTsDialogBase<CTsControlOptionDlg>)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_LEVEL, CBN_SELCHANGE, OnLevelChanged)
		COMMAND_HANDLER(IDC_START_LINE, CBN_SELCHANGE, OnStartLineChanged)
		COMMAND_ID_HANDLER(IDC_RECORD, OnRecordOption)
	END_MSG_MAP()

	VOID DoUIDataChanged(WPARAM wParam, LPARAM lParam);
	CSize GetIdealSize();
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLevelChanged(WORD, WORD, HWND, BOOL&);
	LRESULT OnStartLineChanged(WORD, WORD, HWND, BOOL&);
	LRESULT OnRecordOption(WORD, WORD, HWND, BOOL&);

private:
	void _AdjustSize();
	void _LoadData();
	void _CheckGameOption(WPARAM wType, BOOL bAll = FALSE);
};

class CTsStartControlDlg: public CTsDialogBase<CTsStartControlDlg>,
						  public CTsGameBaseInfoObserver<CTsStartControlDlg>
{
public:
	enum { IDD = IDD_START_CONTROL_DLG };
	BEGIN_MSG_MAP(CTsStartControlDlg)
		CHAIN_MSG_MAP(CTsDialogBase<CTsStartControlDlg>)
		CHAIN_MSG_MAP(CTsGameBaseInfoObserver<CTsStartControlDlg>)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	END_MSG_MAP()

	VOID DoUIDataChanged(WPARAM wParam, LPARAM lParam);

	CSize GetIdealSize();
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL&);

private:
	void _AdjustSize();	
	void _CheckGameStatus();
};

typedef CWinTraitsOR<LVS_REPORT  | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS> CListTraits;
class CTsUserListCtrl: public CWindowImpl<CTsUserListCtrl, CListViewCtrl, CListTraits>,
					   public CTsUserInfObserver<CTsUserListCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(_T("TsListCtrl"), CListViewCtrl::GetWndClassName())
	BEGIN_MSG_MAP(CTsUserListCtrl)
		CHAIN_MSG_MAP(CTsUserInfObserver<CTsUserListCtrl>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	VOID DoUIDataChanged(WPARAM wParam, LPARAM lParam);
	
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	CSize GetMinSize();

private:
	void _LoadUserData();
	void _CreateColumnHeader();
	void _LoadResource();

private:
	CImageList m_imgIcon;
	TCHAR m_arType[4][32];

};

class CTsSingleControlPane: public CWindowImpl<CTsSingleControlPane>
{
public:
	BEGIN_MSG_MAP(CTsSingleControlPane)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	CTsSingleControlPane();

	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM,	BOOL&);

protected:
	void _AdjustSize();
	BOOL _CreateSubWindow();
	void _DestroySubWindow();

private:
	CTsControlTypeDlg m_typeDlg;
	CTsSubControlContainer m_typeContainer;
	CBitmap m_typeIcon;
	TCHAR m_szType[64];

	CTsControlOptionDlg m_optionDlg;
	CTsSubControlContainer m_optionContainer;
	CBitmap m_optionIcon;
	TCHAR m_szOption[64];

	CTsUserListCtrl m_userlist;
	CTsSubControlContainer m_userlistContainer;
	CBitmap m_userlistIcon;
	TCHAR m_szUserlist[64];

	CTsStartControlDlg m_startControlDlg;
};

class CTsMainControlPane: public CWindowImpl<CTsMainControlPane>,
						  public CMessageFilter
{
public:
	DECLARE_WND_CLASS(_T("TsMainControlPane"))
	BEGIN_MSG_MAP(CTsMainControlPane)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled);

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);

protected:

private:
	CTsSingleControlPane m_singleControlPane;
};