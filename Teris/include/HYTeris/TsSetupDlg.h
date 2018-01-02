#pragma  once 

#include "TsDialogBase.h"
#include "TerisRes.h"
#include "TsGameContext.h"
#include "TsDefine.h"
#include <map>

class CTsKeysTable
{
public:
	CTsKeysTable();
	~CTsKeysTable();

	BOOL LoadTable();
	BOOL IsLoaded() { return m_bLoad; }
	CString GetDescription(INT nKey);
	BOOL IsValidKey(INT nKey);
	
	INT MapOEM2VK(int nOEM);
protected:

	CTsKeysTable(const CTsKeysTable& tk);

private:
	std::map<INT, CString> m_mapData;
	BOOL m_bLoad;
	std::map<INT, INT> m_mapOEM;
};

class CTsKeyEdit: public CWindowImpl<CTsKeyEdit, CEdit>
{
public:
	DECLARE_WND_SUPERCLASS(_T("TsKeyEdit"), CEdit::GetWndClassName())
	BEGIN_MSG_MAP(CTsKeyEdit)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
	END_MSG_MAP()
	
	CTsKeyEdit();
	LRESULT OnKeyDown(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnChar(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnContextMenu(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnKillFocus(UINT, WPARAM, LPARAM, BOOL& bHandled);
	
	BOOL SetValue(INT nValue);
	INT GetValue() { return m_nValue; }
private:
	INT m_nValue;
};

class CTsSetupDialog: public CTsDialogBase<CTsSetupDialog>
{
public:
	enum { IDD = IDD_SETUP_DLG };

	BEGIN_MSG_MAP(CTsSetupDialog)
		CHAIN_MSG_MAP(CTsDialogBase<CTsSetupDialog>)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_CODE_HANDLER(EN_CHANGE, OnEnChanged)
		COMMAND_ID_HANDLER(IDC_LEFT_CLOCK_WISE, OnRadioCommand)
		COMMAND_ID_HANDLER(IDC_LEFT_ANTI_CLOCK_WISE, OnRadioCommand)
		COMMAND_ID_HANDLER(IDC_RIGHT_CLOCK_WISE, OnRadioCommand)
		COMMAND_ID_HANDLER(IDC_RIGHT_ANTI_CLOCK_WISE, OnRadioCommand)
		COMMAND_ID_HANDLER(IDC_USE_LEFT, OnRadioCommand)
		COMMAND_ID_HANDLER(IDC_USE_RIGHT, OnRadioCommand)
		COMMAND_ID_HANDLER(IDC_USE_COLOR, OnRadioCommand)
		COMMAND_ID_HANDLER(IDC_USE_BLACK, OnRadioCommand)
		COMMAND_ID_HANDLER(IDC_USE_SOUND, OnUseSoundCommand)

		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

		MESSAGE_HANDLER(WM_TS_SETTING_CHECK, OnSetttingCheck)
		MESSAGE_HANDLER(WM_TS_EDIT_LOST_FOCUS, OnEditLostFocus)
		
		MESSAGE_HANDLER(WM_TIMER, OnTimer)

		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL& bHandled);
	
	LRESULT OnEnChanged(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnRadioCommand(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnUseSoundCommand(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnOK(WORD, WORD, HWND, BOOL&);
	LRESULT OnCancel(WORD, WORD, HWND, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	
	LRESULT OnSetttingCheck(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEditLostFocus(UINT, WPARAM, LPARAM, BOOL&);

	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&);
protected:
	void _SubclassControls();
	void _UnsubclassControls();

	void _GetData();
	void _UpdateToUI();

	void _SaveData();
	
	void _SetupTip(BOOL bSet);
	void _ShowTip(INT nID);
	void _HideTip();

	void _CheckGameStatus();

private:
	enum 
	{ 
		ID_TIP_TIMER = 100,
		TIP_INTERVAL = 3 * 1000
	};

	CToolTipCtrl m_tip;
	CTsControlKeys m_LeftControlKeys;
	CTsControlKeys m_RightControlKeys;

	TS_USE_WIN_TYPE m_useWinType;
	TS_USE_COLOR_TYPE m_useColorType;
	BOOL m_bOpenSound;

	TCHAR m_szLeftName[64];
	TCHAR m_szRightName[64];

	CTsKeyEdit m_LeftMoveLeft, m_LeftMoveRight, m_LeftSwitch, m_LeftMoveDown, m_LeftDropDown;
	CTsKeyEdit m_RightMoveLeft, m_RightMoveRight, m_RightSwitch, m_RightMoveDown, m_RightDropDown;

	TCHAR m_szTip[64];
};