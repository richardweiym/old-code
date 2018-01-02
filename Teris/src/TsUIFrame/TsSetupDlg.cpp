#include "TsSetupDlg.h"
#include "TsUIInstanceApi.h"
#include "TerisRes.h"
#include "TsUtilBase.h"
#include "TsGameMainWnd.h"

using namespace std;

CTsKeysTable* g_gameKeysTable = NULL;

CTsKeysTable::CTsKeysTable()
{
	TS_RUN_ONCE_CHECK
	m_bLoad = FALSE;

	_ASSERTE(g_gameKeysTable == NULL);
	g_gameKeysTable = this;
}

CTsKeysTable::~CTsKeysTable()
{
	m_mapData.clear();
	g_gameKeysTable = NULL;
}

BOOL CTsKeysTable::IsValidKey(INT nKey)
{
	_ASSERTE(m_bLoad);
	return m_mapData.find(nKey) != m_mapData.end();
}

CString CTsKeysTable::GetDescription(INT nKey)
{
	CString strRet(_T(""));
	_ASSERTE(m_bLoad);
	
	map<INT, CString>::iterator itr = m_mapData.find(nKey);
	if(itr != m_mapData.end())
	{
		strRet = itr->second;
	}

	return strRet;
}
INT CTsKeysTable::MapOEM2VK(int nOEM)
{
	_ASSERTE(m_bLoad);
	INT nRet(0);
	map<INT, INT>::iterator itr = m_mapOEM.find(nOEM);
	if(itr != m_mapOEM.end())
	{
		nRet = itr->second;
	}

	return nRet;
}
BOOL CTsKeysTable::LoadTable()
{
	if(m_bLoad)
	{
		_ASSERTE(FALSE);
		return TRUE;
	}

	struct KeyPair
	{
		INT nValue;
		TCHAR szText[32];
	};
	
	KeyPair arKeys[] = 
	{
		0x08, _T("Back"),
		0x20, _T("Space"),	
		0x21, _T("Page Up"),
		0x22, _T("Page Down"),
		0x23, _T("End"),
		0x24, _T("Home"),
		0x25, _T("Left"),
		0x26, _T("Up"),
		0x27, _T("Right"),
		0x28, _T("Down"),
		0x2D, _T("Insert"),
		0x2E, _T("Delete"),
		0x30, _T("0"),
		0x31, _T("1"),
		0x32, _T("2"),
		0x33, _T("3"),
		0x34, _T("4"),
		0x35, _T("5"),
		0x36, _T("6"),
		0x37, _T("7"),
		0x38, _T("8"),
		0x39, _T("9"),
		0x41, _T("A"),
		0x42, _T("B"),
		0x43, _T("C"),
		0x44, _T("D"),		
		0x45, _T("E"),
		0x46, _T("F"),
		0x47, _T("G"),
		0x48, _T("H"),	
		0x49, _T("I"),
		0x4A, _T("J"),
		0x4B, _T("K"),
		0x4C, _T("L"),		
		0x4D, _T("M"),
		0x4E, _T("N"),
		0x4F, _T("O"),
		0x50, _T("P"),	
		0x51, _T("Q"),
		0x52, _T("R"),
		0x53, _T("S"),
		0x54, _T("T"),		
		0x55, _T("U"),
		0x56, _T("V"),
		0x57, _T("W"),
		0x58, _T("X"),	
		0x59, _T("Y"),
		0x5A, _T("Z"),
		0x60, _T("N0"),
		0x61, _T("N1"),
		0x62, _T("N2"),
		0x63, _T("N3"),
		0x64, _T("N4"),
		0x65, _T("N5"),
		0x66, _T("N6"),
		0x67, _T("N7"),
		0x68, _T("N8"),
		0x69, _T("N9"),
		0x6A, _T("N*"),
		0x6B, _T("N+"),
		0x6E, _T("N."),
		0x6D, _T("N-"),
		0x6F, _T("N/"),
		0x70, _T("F1"),
		0x71, _T("F2"),	
		0x72, _T("F3"),
		0x73, _T("F4"),	
		0x74, _T("F5"),
		0x75, _T("F6"),	
		0x76, _T("F7"),
		0x77, _T("F8"),	
		0x78, _T("F9"),
		0x79, _T("F10"),	
		0x7A, _T("F11"),
		0x7B, _T("F12"),
		
		0xBA, _T(";"),
		0xBB, _T("="),
		0xBC, _T(","),
		0xBD, _T("-"),
		0xBE, _T("."),
		0xBF, _T("/"),
		0xC0, _T("~"),
		0xDB, _T("["),
		0xDC, _T("\\"),
		0xDD, _T("]"),
		0xDE, _T("'")
	};
	
	m_mapData.clear();
	INT nCount = sizeof(arKeys) / sizeof(arKeys[0]);
	INT i(0);
	for(i=0; i<nCount; ++i)
	{
		m_mapData.insert(make_pair<INT, CString>(arKeys[i].nValue, arKeys[i].szText));
	}
	
	
	struct OEMPair
	{
		INT nOEM;
		INT nKey;
	};
	
	OEMPair arOEM[] = 
	{
		0x0B, 0x30, //0
		0x02, 0x31,
		0x03, 0x32,
		0x04, 0x33,
		0x05, 0x34,
		0x06, 0x35,
		0x07, 0x36,
		0x08, 0x37,
		0x09, 0x38,
		0x0A, 0x39, //9

		0x10, 0x51, //Q
		0x11, 0x57,
		0x12, 0x45,
		0x13, 0x52,
		0x14, 0x54,
		0x15, 0x59,
		0x16, 0x55,
		0x17, 0x49,
		0x18, 0x4F,
		0x19, 0x50,//P

		0x1E, 0x41, //A
		0x1F, 0x53,
		0x20, 0x44,
		0x21, 0x46,
		0x22, 0x47,
		0x23, 0x48,
		0x24, 0x4A,
		0x25, 0x4B,
		0x26, 0x4C, //L

		0x2C, 0x5A, //Z
		0x2D, 0x58,
		0x2E, 0x43,
		0x2F, 0x56,
		0x30, 0x42,
		0x31, 0x4E,
		0x32, 0x4D,	//M

		0x47, 0x67, //7
		0x48, 0x68,	//8
		0x49, 0x69,	//9
		0x4B, 0x64, //4
		0x4C, 0x65, //5
		0x4D, 0x66, //6
		0x4F, 0x61, //1
		0x50, 0x62, //2
		0x51, 0x63, //3
		0x52, 0x60  //0
	};
	
	m_mapOEM.clear();
	nCount = sizeof(arOEM) / sizeof(arOEM[0]);
	for(i=0; i<nCount; ++i)
	{
		m_mapOEM.insert(make_pair(arOEM[i].nOEM, arOEM[i].nKey));
	}

	m_bLoad = TRUE;

	return TRUE;
}

/////////////////////////	CTsKeyEdit  //////////////
CTsKeyEdit::CTsKeyEdit()
{
	m_nValue = 0;
}
LRESULT CTsKeyEdit::OnKeyDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bSet(FALSE);
	CTsKeysTable* pKeysTable = TsGetKeysTable();
	_ASSERTE(pKeysTable != NULL);

	CTsKeysTable& tk = *pKeysTable;
	
	if(wParam == 0xE5)
	{
		INT nMASK = 0x00FF0000;
		INT nValue = nMASK & lParam;
		nValue >>= 16;
		
		wParam = tk.MapOEM2VK(nValue);
	}
	
	if(m_nValue == wParam)
	{
		return S_OK;
	}


	if(tk.IsValidKey(wParam))
	{
		HWND hParent = GetParent();
		_ASSERTE(hParent != NULL && IsWindow());
		
		bSet = ::SendMessage(hParent, WM_TS_SETTING_CHECK, wParam, GetWindowLong(GWL_ID));
	}
	
	if(bSet)	
	{
		SetValue(wParam);
	}
	else
	{
		MessageBeep(-1);
	}

	return S_OK;
}

LRESULT CTsKeyEdit::OnChar(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return  S_OK;
}
LRESULT CTsKeyEdit::OnKillFocus(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	HWND hParent = GetParent();
	_ASSERTE(hParent != NULL && IsWindow());
		
	::SendMessage(hParent, WM_TS_EDIT_LOST_FOCUS, 0, GetWindowLong(GWL_ID));

	bHandled = FALSE;

	return S_OK;
}
LRESULT CTsKeyEdit::OnContextMenu(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return S_OK;
}
BOOL CTsKeyEdit::SetValue(INT nValue)
{
	BOOL bRet(FALSE);
	CTsKeysTable* pKeysTable = TsGetKeysTable();
	_ASSERTE(pKeysTable != NULL);

	if(pKeysTable->IsValidKey(nValue))
	{
		m_nValue = nValue;
		SetWindowText(pKeysTable->GetDescription(nValue));
		
		bRet = TRUE;
	}

	return bRet;
}
/////////////////////  CTsSetupDialog  ///////////////////
LRESULT CTsSetupDialog::OnClose(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	EndDialog(IDCANCEL);

	return S_OK;
}
void CTsSetupDialog::_CheckGameStatus()
{
	CTsGameBaseInfo* pBaseInfo = TsGetGameBaseInfo();
	_ASSERTE(pBaseInfo != NULL);

	HWND hUseLeft = GetDlgItem(IDC_USE_LEFT);
	HWND hUseRight = GetDlgItem(IDC_USE_RIGHT);

	BOOL bEnable(TRUE);
	if(pBaseInfo->GetGameStatus() != IDLE_STATUS)
	{
		bEnable = FALSE;
	}
	::EnableWindow(hUseLeft, bEnable);
	::EnableWindow(hUseRight, bEnable);
}
LRESULT CTsSetupDialog::OnInitDlg(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CenterWindow();
	_SubclassControls();

	_GetData();
	_UpdateToUI();

	_SetupTip(TRUE);

	_CheckGameStatus();

	CEdit editLeft = GetDlgItem(IDC_LEFT_NAME);
	CEdit editRight = GetDlgItem(IDC_RIGHT_NAME);
	editLeft.SetLimitText(60);
	editRight.SetLimitText(60);

	return S_OK;
}
void CTsSetupDialog::_SetupTip(BOOL bSet)
{
	if(bSet)
	{
		CTsResMgr* pResMgr = TsGetResMgr();
		_ASSERTE(pResMgr != NULL);
		pResMgr->LoadString(IDS_KEY_DUPLICATE, m_szTip, sizeof(m_szTip) / sizeof(TCHAR));


		_ASSERTE(!m_tip.IsWindow());
		if(!m_tip.IsWindow())
		{
			m_tip.Create(m_hWnd);
			_ASSERTE(m_tip.IsWindow());
	
			TOOLINFO ti = {0};
			ti.cbSize = sizeof(ti);
			ti.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
			ti.hinst = NULL;
			ti.hwnd = NULL;
			ti.uId = (UINT)m_hWnd;
			ti.lpszText = m_szTip;

			m_tip.AddTool(&ti);
		}
	}
	else
	{
		if(m_tip.IsWindow())
		{
			m_tip.DelTool(m_hWnd);
			m_tip.DestroyWindow();
		}

		KillTimer(ID_TIP_TIMER);
	}
}
void CTsSetupDialog::_ShowTip(INT nID)
{
	HWND hWnd = GetDlgItem(nID);
	_ASSERTE(hWnd != NULL);
	CRect rtWin;
	::GetWindowRect(hWnd, &rtWin);
	CPoint ptTip((rtWin.left + rtWin.right) / 2 + 10, (rtWin.top + rtWin.bottom) / 2);
	m_tip.TrackPosition(ptTip.x, ptTip.y);

	TOOLINFO ti = {0};
	ti.cbSize = sizeof(ti);
	ti.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
	ti.hinst = NULL;
	ti.hwnd = NULL;
	ti.uId = (UINT)m_hWnd;
	ti.lpszText = m_szTip;

	m_tip.TrackActivate(&ti, TRUE);

	KillTimer(ID_TIP_TIMER);
	SetTimer(ID_TIP_TIMER, TIP_INTERVAL);

}
void CTsSetupDialog::_HideTip()
{
	TOOLINFO ti = {0};
	ti.cbSize = sizeof(ti);
	ti.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
	ti.hinst = NULL;
	ti.hwnd = NULL;
	ti.uId = (UINT)m_hWnd;
	ti.lpszText = m_szTip;

	m_tip.TrackActivate(&ti, FALSE);

	KillTimer(ID_TIP_TIMER);
}
LRESULT CTsSetupDialog::OnTimer(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	_ASSERTE(wParam == ID_TIP_TIMER);
	
	if(wParam == ID_TIP_TIMER)
	{
		_HideTip();
	}

	return S_OK;
}
LRESULT CTsSetupDialog::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	_UnsubclassControls();
	
	_SetupTip(FALSE	);

	return S_OK;
}
void CTsSetupDialog::_GetData()
{
	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);

	CTsControlKeys* pLeftKeys = pContext->GetLeftWinControlKeys();
	_ASSERTE(pLeftKeys != NULL);
	m_LeftControlKeys = *pLeftKeys;

	CTsControlKeys* pRightKeys = pContext->GetRightWinControlKeyS();
	_ASSERTE(pRightKeys != NULL);
	m_RightControlKeys = *pRightKeys;

	m_useWinType = pContext->GetUseWinType();
	m_useColorType = pContext->GetUseColorType();

	m_bOpenSound = pContext->IsSoundOpen();

	lstrcpyn(m_szLeftName, pContext->GetLeftUserName(), 60);
	lstrcpyn(m_szRightName, pContext->GetRightUserName(), 60);
}

void CTsSetupDialog::_UpdateToUI()
{
	m_LeftMoveLeft.SetValue(m_LeftControlKeys.m_nMoveLeft);
	m_LeftMoveRight.SetValue(m_LeftControlKeys.m_nMoveRight);
	m_LeftMoveDown.SetValue(m_LeftControlKeys.m_nMoveDown);
	m_LeftDropDown.SetValue(m_LeftControlKeys.m_nDropDown);
	m_LeftSwitch.SetValue(m_LeftControlKeys.m_nMoveSwitch);

	m_RightMoveLeft.SetValue(m_RightControlKeys.m_nMoveLeft);
	m_RightMoveRight.SetValue(m_RightControlKeys.m_nMoveRight);
	m_RightMoveDown.SetValue(m_RightControlKeys.m_nMoveDown);
	m_RightDropDown.SetValue(m_RightControlKeys.m_nDropDown);
	m_RightSwitch.SetValue(m_RightControlKeys.m_nMoveSwitch);

	CButton btnCheck = GetDlgItem(IDC_LEFT_ANTI_CLOCK_WISE);
	if(m_LeftControlKeys.m_nSwitchDirection == CLOCK_WISE)
	{
		btnCheck = GetDlgItem(IDC_LEFT_CLOCK_WISE);
	}
	btnCheck.SetCheck(TRUE);

	btnCheck = GetDlgItem(IDC_RIGHT_ANTI_CLOCK_WISE);
	if(m_RightControlKeys.m_nSwitchDirection == CLOCK_WISE)
	{
		btnCheck = GetDlgItem(IDC_RIGHT_CLOCK_WISE);
	}
	btnCheck.SetCheck(TRUE);

	btnCheck = GetDlgItem(IDC_USE_LEFT);
	if(m_useWinType == USE_RIGHT_WIN)
	{
		btnCheck = GetDlgItem(IDC_USE_RIGHT);
	}
	btnCheck.SetCheck(TRUE);

	btnCheck = GetDlgItem(IDC_USE_COLOR);
	if(m_useColorType == USE_BLACK_TYPE)
	{
		btnCheck = GetDlgItem(IDC_USE_BLACK);
	}
	btnCheck.SetCheck(TRUE);

	btnCheck = GetDlgItem(IDC_USE_SOUND);
	if(m_bOpenSound)
	{
		btnCheck.SetCheck(TRUE);
	}
	else
	{
		btnCheck.SetCheck(FALSE);
	}

	CEdit editLeft = GetDlgItem(IDC_LEFT_NAME);
	CEdit editRight = GetDlgItem(IDC_RIGHT_NAME);
	editLeft.SetWindowText(m_szLeftName);
	editRight.SetWindowText(m_szRightName);

}

void CTsSetupDialog::_SaveData()
{
	CTsGameContext* pContext = TsGetGameContext();
	_ASSERTE(pContext != NULL);
	
	pContext->SetLeftWinControlKeys(&m_LeftControlKeys);
	pContext->SetRightWinControlKeys(&m_RightControlKeys);
	pContext->SetUseColorType(m_useColorType);
	pContext->SetUseWinType(m_useWinType);
	pContext->SetSoundOpen(m_bOpenSound);

	pContext->SaveToConfigFile();

	pContext->SetLeftUserName(m_szLeftName);
	pContext->SetRightUserName(m_szRightName);

	CTsGameMainWnd* pMainWnd = TsGetGameMainWnd();
	_ASSERTE(pMainWnd != NULL);
	if(pMainWnd != NULL)
	{
		pMainWnd->SendMessage(TS_SETUP_DLG_CHANGED, 0, 0);
	}
}


void CTsSetupDialog::_SubclassControls()
{
	m_LeftMoveLeft.SubclassWindow(GetDlgItem(IDC_LEFT_MOVE_LEFT));
	m_LeftMoveRight.SubclassWindow(GetDlgItem(IDC_LEFT_MOVE_RIGHT));
	m_LeftSwitch.SubclassWindow(GetDlgItem(IDC_LEFT_SWITCH));
	m_LeftMoveDown.SubclassWindow(GetDlgItem(IDC_LEFT_MOVE_DOWN));
	m_LeftDropDown.SubclassWindow(GetDlgItem(IDC_LEFT_DROP_DOWN));

	m_RightMoveLeft.SubclassWindow(GetDlgItem(IDC_RIGHT_MOVE_LEFT));
	m_RightMoveRight.SubclassWindow(GetDlgItem(IDC_RIGHT_MOVE_RIGHT));
	m_RightSwitch.SubclassWindow(GetDlgItem(IDC_RIGHT_SWITCH));
	m_RightMoveDown.SubclassWindow(GetDlgItem(IDC_RIGHT_MOVE_DOWN));
	m_RightDropDown.SubclassWindow(GetDlgItem(IDC_RIGHT_DROP_DOWN));
}

void CTsSetupDialog::_UnsubclassControls()
{
	m_LeftMoveLeft.UnsubclassWindow();
	m_LeftMoveRight.UnsubclassWindow();
	m_LeftSwitch.UnsubclassWindow();
	m_LeftMoveDown.UnsubclassWindow();
	m_LeftDropDown.UnsubclassWindow();

	m_RightMoveLeft.UnsubclassWindow();
	m_RightMoveRight.UnsubclassWindow();
	m_RightSwitch.UnsubclassWindow();
	m_RightMoveDown.UnsubclassWindow();
	m_RightDropDown.UnsubclassWindow();
}
LRESULT CTsSetupDialog::OnEnChanged(WORD, WORD wId, HWND, BOOL& bHandled)
{
	switch(wId)
	{
	case IDC_LEFT_MOVE_LEFT:
		m_LeftControlKeys.m_nMoveLeft = m_LeftMoveLeft.GetValue();
		break;

	case IDC_LEFT_MOVE_RIGHT:
		m_LeftControlKeys.m_nMoveRight = m_LeftMoveRight.GetValue();
		break;

	case IDC_LEFT_SWITCH:
		m_LeftControlKeys.m_nMoveSwitch = m_LeftSwitch.GetValue();
		break;
	
	case IDC_LEFT_MOVE_DOWN:
		m_LeftControlKeys.m_nMoveDown = m_LeftMoveDown.GetValue();
		break;

	case IDC_LEFT_DROP_DOWN:
		m_LeftControlKeys.m_nDropDown = m_LeftDropDown.GetValue();
		break;

	case IDC_RIGHT_MOVE_LEFT:
		m_RightControlKeys.m_nMoveLeft = m_RightMoveLeft.GetValue();
		break;

	case IDC_RIGHT_MOVE_RIGHT:
		m_RightControlKeys.m_nMoveRight = m_RightMoveRight.GetValue();
		break;

	case IDC_RIGHT_SWITCH:
		m_RightControlKeys.m_nMoveSwitch = m_RightSwitch.GetValue();
		break;
	
	case IDC_RIGHT_MOVE_DOWN:
		m_RightControlKeys.m_nMoveDown = m_RightMoveDown.GetValue();
		break;

	case IDC_RIGHT_DROP_DOWN:
		m_RightControlKeys.m_nDropDown = m_RightDropDown.GetValue();
		break;
	case IDC_LEFT_NAME:
		{
			TCHAR szText[100] = {0};
			GetDlgItemText(IDC_LEFT_NAME, szText, 60);
			lstrcpyn(m_szLeftName, szText, 60);
		}
		break;

	case IDC_RIGHT_NAME:
		{
			TCHAR szText[100] = {0};
			GetDlgItemText(IDC_RIGHT_NAME, szText, 60);
			lstrcpyn(m_szRightName, szText, 60);
		}
		break;

	default:
		_ASSERTE(FALSE);
		break;
	}

	return S_OK;
}

LRESULT CTsSetupDialog::OnRadioCommand(WORD nCode, WORD nID, HWND, BOOL& bHandled)
{
	if(nCode != BN_CLICKED) return S_OK;

	switch(nID)
	{
	case IDC_LEFT_CLOCK_WISE:
		m_LeftControlKeys.m_nSwitchDirection = CLOCK_WISE;
		break;

	case IDC_LEFT_ANTI_CLOCK_WISE:
		m_LeftControlKeys.m_nSwitchDirection = ANTI_CLOCK_WISE;
		break;

	case IDC_RIGHT_CLOCK_WISE:
		m_RightControlKeys.m_nSwitchDirection = CLOCK_WISE;
		break;

	case IDC_RIGHT_ANTI_CLOCK_WISE:
		m_RightControlKeys.m_nSwitchDirection = ANTI_CLOCK_WISE;
		break;

	case IDC_USE_LEFT:
		m_useWinType = USE_LEFT_WIN;
		break;

	case IDC_USE_RIGHT:
		m_useWinType = USE_RIGHT_WIN;
		break;

	case IDC_USE_COLOR:
		m_useColorType = USE_COLOR_TYPE;
		break;

	case IDC_USE_BLACK:
		m_useColorType = USE_BLACK_TYPE;
		break;

	default:
		_ASSERTE(FALSE);
		break;
	}

	return S_OK;
}

LRESULT CTsSetupDialog::OnUseSoundCommand(WORD nCode, WORD, HWND, BOOL& bHandled)
{
	if(nCode != BN_CLICKED) return S_OK;

	CButton btnCheck = GetDlgItem(IDC_USE_SOUND);
	m_bOpenSound = btnCheck.GetCheck();

	return S_OK;
}

LRESULT CTsSetupDialog::OnCancel(WORD, WORD, HWND, BOOL&)
{
	EndDialog(IDCANCEL);

	return S_OK;
}

LRESULT CTsSetupDialog::OnOK(WORD, WORD, HWND, BOOL&)
{
	_SaveData();
	EndDialog(IDOK);

	return S_OK;
}

LRESULT CTsSetupDialog::OnSetttingCheck(UINT, WPARAM nKey, LPARAM lParam, BOOL&)
{	
	if(m_LeftControlKeys.m_nDropDown == nKey
		|| m_LeftControlKeys.m_nMoveDown == nKey
		|| m_LeftControlKeys.m_nMoveLeft == nKey
		|| m_LeftControlKeys.m_nMoveRight == nKey
		|| m_LeftControlKeys.m_nMoveSwitch == nKey

		|| m_RightControlKeys.m_nDropDown == nKey
		|| m_RightControlKeys.m_nMoveDown == nKey
		|| m_RightControlKeys.m_nMoveLeft == nKey
		|| m_RightControlKeys.m_nMoveRight == nKey
		|| m_RightControlKeys.m_nMoveSwitch == nKey)
	{
		_ShowTip(lParam);
		return FALSE;
	}
	else
	{
		_HideTip();
		return TRUE;
	}
}

LRESULT CTsSetupDialog::OnEditLostFocus(UINT, WPARAM, LPARAM, BOOL&)
{
	_HideTip();

	return S_OK;
}