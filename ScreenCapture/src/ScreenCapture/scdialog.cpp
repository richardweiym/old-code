#include "SCDialog.h"
#include "SCCanvasWindow.h"
#include "SCCanvasConfig.h"

#pragma  comment(lib, "WaveObject.lib")


LRESULT CSCCloseButton::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	
	LoadResource();

	return S_OK;
}

LRESULT CSCCloseButton::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;

	FreeResource();

	return S_OK;
}

VOID CSCCloseButton::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
		if(wParam == UI_WPARAM_SKIN_CHANGED
			|| wParam == UI_WPARAM_LAN_CHANGED)
		{
			_ASSERTE(IsWindow());

			FreeResource();
			LoadResource();

			_AdjustSize();
			Invalidate();
		}
}

BOOL CSCCloseButton::LoadResource()
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	
	SetBtnStyle(TS_BTN_RGN_STYLE | TS_BTN_CENTER_TEXT_STYLE);
	m_imgBtn = pSkin->GetCloseBtnImg();
	m_bmpBkgnd = pSkin->GetCloseBtnBK();
	m_strTooltip = pSkin->GetCloseBtnTip();

	return TRUE;
}

VOID CSCCloseButton::FreeResource()
{
	if(!m_bmpBkgnd.IsNull()) m_bmpBkgnd.DeleteObject();
	if(!m_imgBtn.IsNull()) m_imgBtn.Destroy();
}



CSCWaterWaveControl::CSCWaterWaveControl()
{
	m_bmpBK = NULL;
	m_nOldTickCount = 0;
	m_nOldX = m_nOldY = 0;
}

LRESULT CSCWaterWaveControl::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	m_bmpBK = pSkin->GetAboutWaveBK();
	_ASSERTE(m_bmpBK != NULL);
	
	_WaveInit(&m_objWave, m_hWnd, m_bmpBK, 30, 0);
	
	m_nOldTickCount = 0;
	m_nOldX = m_nOldY = 0;

	Layout();

	return S_OK;
}

LRESULT CSCWaterWaveControl::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	_WaveFree(&m_objWave); 

	if(m_bmpBK != NULL) 
	{
		DeleteObject(m_bmpBK);
		m_bmpBK = NULL;
	}
	
	return S_OK;
}

LRESULT CSCWaterWaveControl::OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	Layout();

	return S_OK;
}

LRESULT CSCWaterWaveControl::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	CPaintDC dc(m_hWnd);

	_WaveUpdateFrame(&m_objWave, dc, TRUE);	

	return S_OK;
}

LRESULT CSCWaterWaveControl::OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
	INT nX = GET_X_LPARAM(lParam);
	INT nY = GET_Y_LPARAM(lParam);
	_WaveDropStone(&m_objWave,nX,nY,3,800);

	return S_OK;
}
LRESULT CSCWaterWaveControl::OnMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	INT nX = GET_X_LPARAM(lParam);
	INT nY = GET_Y_LPARAM(lParam);
	INT nTicket = GetTickCount();
	
	if(m_nOldTickCount == 0)
	{
		m_nOldTickCount = nTicket;
	}
	if(m_nOldX == 0) m_nOldX = nX;
	if(m_nOldY == 0) m_nOldY = nY;

	DWORD dwCheckTime = 200;
	if(MK_LBUTTON&wParam) dwCheckTime = 20;

	if(nTicket - m_nOldTickCount > dwCheckTime)
	{
		if(m_nOldX != nX || m_nOldY != nY)
		{
			_WaveDropStone(&m_objWave,nX,nY,1,400);

			m_nOldTickCount = nTicket;
			m_nOldX = nX;
			m_nOldY = nY;
		}
	}

	return S_OK;
}

CSize CSCWaterWaveControl::GetIdealSize()
{
	CSize sizeRet(0, 0);
	if(m_bmpBK == NULL) return sizeRet;

	return CHYUICommonFunctionWrapper::GetBitmapSize(m_bmpBK);
}

VOID CSCWaterWaveControl::Layout()
{
	if(!IsWindow()) return;

	CSize sizeControl = GetIdealSize();
	CRect rtWindow;
	GetWindowRect(&rtWindow);
	if(rtWindow.Width() != sizeControl.cx || rtWindow.Height() != sizeControl.cy)
	{
		SetWindowPos(NULL, 0, 0, sizeControl.cx, sizeControl.cy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
	}
}

CSCAboutInfoDialog::CSCAboutInfoDialog()
{
	m_bmpBtnOK = NULL;
	m_imgBtnOK = NULL;
}

VOID CSCAboutInfoDialog::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == UI_WPARAM_SKIN_CHANGED)
	{
		FreeRes();
		LoadRes();
	}

	CSCDialogBase<CSCAboutInfoDialog>::DoUIDataChanged(wParam, lParam);
}

LRESULT CSCAboutInfoDialog::OnCommand(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hWndParent = GetParent();
	if(hWndParent != NULL)
	{
		return ::SendMessage(hWndParent, nMsgType, wParam, lParam);
	}

	return S_OK;
}

BOOL CSCAboutInfoDialog::LoadRes()
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	m_bmpBtnOK = pSkin->GetDlgBtnBK();
	m_imgBtnOK = pSkin->GetDlgBtnImg();

	m_btnOK.SetBtnStyle(TS_BTN_RGN_STYLE | TS_BTN_CENTER_TEXT_STYLE);
	m_btnOK.SetBkgndBitmap(m_bmpBtnOK);
	m_btnOK.SetBtnImagelist(m_imgBtnOK);

	return TRUE;
}

VOID CSCAboutInfoDialog::FreeRes()
{
	if(m_bmpBtnOK != NULL)
	{
		DeleteObject(m_bmpBtnOK);
		m_bmpBtnOK = NULL;
	}

	if(m_imgBtnOK != NULL)
	{
		ImageList_Destroy(m_imgBtnOK);
		m_imgBtnOK = NULL;
	}
}

LRESULT CSCAboutInfoDialog::OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	
	LoadRes();

	m_link.SubclassWindow(GetDlgItem(IDC_SITE));
	m_btnOK.SubclassWindow(GetDlgItem(IDOK));

	return S_OK;
}

LRESULT CSCAboutInfoDialog::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	
	m_link.UnsubclassWindow();
	m_btnOK.UnsubclassWindow();
	
	FreeRes();

	return S_OK;
}

LRESULT CSCAboutDialog::OnClickCloseBtn(WORD, WORD, HWND, BOOL& bHandled)
{
	Close(FALSE);

	return S_OK;
}

LRESULT CSCAboutDialog::OnOK(WORD, WORD, HWND, BOOL& bHandled)
{
	Close(FALSE);

	return S_OK;
}

LRESULT CSCAboutDialog::OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	SetWindowText(pSkin->GetResString(IDS_ABOUT_TITLE));
	
	m_wndWave.Create(m_hWnd, CRect(0, 0, 0, 0));
	m_dlgInfo.Create(m_hWnd);
	m_dlgInfo.ShowWindow(SW_SHOW);

	AdjustSize();

	bHandled = FALSE;
	return S_OK;
}

LRESULT CSCAboutDialog::OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	AdjustSize();
		
	bHandled = FALSE;
	return S_OK;
}

VOID CSCAboutDialog::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(UI_WPARAM_LAN_CHANGED == wParam)
	{
		PostMessage(WM_COMMAND, SC_CLOSE_BTN_ID, NULL);
	}
	else if(wParam == UI_WPARAM_SKIN_CHANGED)
	{
		CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
		SetWindowText(pSkin->GetResString(IDS_ABOUT_TITLE));

		CSCDialogBase<CSCAboutDialog>::DoUIDataChanged(wParam, lParam);

		AdjustSize();
		Invalidate();
	}
}

CRect CSCAboutDialog::GetWaveControlMargin()
{
	return CRect(10, 10, 10, 10);
}

VOID CSCAboutDialog::AdjustSize()
{
	CRect rtInfoClient;
	m_dlgInfo.GetClientRect(&rtInfoClient);

	
	CRect rtClient = GetDialogClientRect();
	CSize sizeControl = m_wndWave.GetIdealSize();
	CRect rtWaveMargin = GetWaveControlMargin();
	
	INT nWaveInfoGap = 10;
	
	INT nNeedWidth = sizeControl.cx + SC_DLG_FRAME_WIDTH * 2 + rtWaveMargin.left + rtWaveMargin.right;
	INT nNeedHeight = sizeControl.cy + rtClient.top + rtWaveMargin.top + rtWaveMargin.bottom + rtInfoClient.Height() + nWaveInfoGap;

	SetWindowPos(NULL, 0, 0, nNeedWidth, nNeedHeight, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);

	rtClient = GetDialogClientRect();
	CRect rtWave(rtClient);
	rtWave.left += rtWaveMargin.left;
	rtWave.top += rtWaveMargin.top;
	rtWave.right -= rtWaveMargin.right;
	rtWave.bottom = rtWave.top + sizeControl.cy;

	m_wndWave.MoveWindow(&rtWave);

	CRect rtInfo(rtWave);
	rtInfo.top = rtWave.bottom + nWaveInfoGap;
	rtInfo.bottom = rtInfo.top + rtInfoClient.Height();
	m_dlgInfo.MoveWindow(&rtInfo);
}

CSCSettinDialog::CSCSettinDialog()
{
	m_bmpBtnOK = NULL;
	m_imgBtnOK = NULL;
}

BOOL CSCSettinDialog::LoadRes()
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	m_bmpBtnOK = pSkin->GetDlgBtnBK();
	m_imgBtnOK = pSkin->GetDlgBtnImg();

	m_btnOK.SetBtnStyle(TS_BTN_RGN_STYLE | TS_BTN_CENTER_TEXT_STYLE);
	m_btnOK.SetBkgndBitmap(m_bmpBtnOK);
	m_btnOK.SetBtnImagelist(m_imgBtnOK);

	return TRUE;
}

VOID CSCSettinDialog::FreeRes()
{
	if(m_bmpBtnOK != NULL)
	{
		DeleteObject(m_bmpBtnOK);
		m_bmpBtnOK = NULL;
	}

	if(m_imgBtnOK != NULL)
	{
		ImageList_Destroy(m_imgBtnOK);
		m_imgBtnOK = NULL;
	}
}

LRESULT CSCSettinDialog::OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;

	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	SetWindowText(pSkin->GetResString(IDS_SETTING_TITLE));

	LoadRes();

	m_btnOK.SubclassWindow(GetDlgItem(IDOK));
	
	UpateUIData();

	return S_OK;
}

LRESULT CSCSettinDialog::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;

	m_btnOK.UnsubclassWindow();
	
	FreeRes();

	return S_OK;
}

LRESULT CSCSettinDialog::OnClickCloseBtn(WORD, WORD, HWND, BOOL& bHandled)
{
	Close(FALSE);

	return S_OK;
}

LRESULT CSCSettinDialog::OnOK(WORD, WORD, HWND, BOOL& bHandled)
{
	SaveFileEditorName();
	Close(FALSE);

	return S_OK;
}


LRESULT CSCSettinDialog::OnShowZoom(WORD, WORD, HWND, BOOL& bHandled)
{
	CRefPtr<CSCCanvasWindow> pCanvs = GetCanvasWindow();
	pCanvs->SendMessage(WM_COMMAND, SC_SHOW_HELP_TIP_CMD_ID, 0);
	UpateUIData();
	return S_OK;
}

LRESULT CSCSettinDialog::OnShowCursor(WORD, WORD, HWND, BOOL& bHandled)
{
	CRefPtr<CSCCanvasWindow> pCanvs = GetCanvasWindow();
	pCanvs->SendMessage(WM_COMMAND, SC_SHOW_CURSOR_CMD_ID, 0);
	UpateUIData();
	return S_OK;
}

LRESULT CSCSettinDialog::OnTransparentMark(WORD, WORD, HWND, BOOL& bHandled)
{
	CRefPtr<CSCCanvasWindow> pCanvs = GetCanvasWindow();
	pCanvs->SendMessage(WM_COMMAND, SC_TRANSPARENT_MARK_ID, 0);
	UpateUIData();
	return S_OK;
}

LRESULT CSCSettinDialog::OnDBClickClose(WORD, WORD, HWND, BOOL& bHandled)
{
	CRefPtr<CSCCanvasWindow> pCanvs = GetCanvasWindow();
	pCanvs->SendMessage(WM_COMMAND, SC_DOUBLE_CLICK_TO_EXIT, 0);
	UpateUIData();
	return S_OK;
}

VOID CSCSettinDialog::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if( wParam == UI_WPARAM_SKIN_CHANGED)
	{
		CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
		SetWindowText(pSkin->GetResString(IDS_SETTING_TITLE));

		FreeRes();
		LoadRes();

		CSCDialogBase<CSCSettinDialog>::DoUIDataChanged(wParam, lParam);
	}
	else if(wParam == UI_WPARAM_LAN_CHANGED)
	{
		PostMessage(WM_COMMAND, SC_CLOSE_BTN_ID, NULL);
	}
}


VOID CSCSettinDialog::UpateUIData()
{
	CButton btnShowZoom = GetDlgItem(IDC_CHECK_SHOW_ZOOM);
	CButton btnShowCursor = GetDlgItem(IDC_CHECK_SHOW_CURSOR);
	CButton btnTransparent = GetDlgItem(IDC_CHECK_TRANSPARENT_MARK);
	CButton btnDBClickExit = GetDlgItem(IDC_CHECK_DBCLICK_CLOSE);

	_ASSERTE(btnShowZoom.IsWindow());
	_ASSERTE(btnShowCursor.IsWindow());
	_ASSERTE(btnTransparent.IsWindow());
	_ASSERTE(btnDBClickExit.IsWindow());

	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	btnShowZoom.SetCheck(pContext->GetShowHelpFlag());
	btnShowCursor.SetCheck(pContext->GetShowCursorFlag());
	btnTransparent.SetCheck(pContext->GetTransparentMark());
	btnDBClickExit.SetCheck(pContext->GetDoubleClickExitFlag());

	CRefPtr<CSCCanvasConfig> pConfig = GetCanvasConfig();
	TCHAR szFileEditor[MAX_PATH] = {0};
	pConfig->GetFileEditor(szFileEditor, sizeof(szFileEditor) / sizeof(TCHAR));
	CEdit editorFile = GetDlgItem(IDC_EDIT_IMAGE_EDITOR);
	editorFile.SetLimitText(MAX_PATH - 1);
	editorFile.SetWindowText(szFileEditor);
}

VOID CSCSettinDialog::SaveFileEditorName()
{
	TCHAR szFileEditor[MAX_PATH] = {0};
	CRefPtr<CSCCanvasConfig> pConfig = GetCanvasConfig();
	CEdit editorFile = GetDlgItem(IDC_EDIT_IMAGE_EDITOR);
	editorFile.GetWindowText(szFileEditor, sizeof(szFileEditor) / sizeof(TCHAR));
	CString strName = szFileEditor;
	strName.TrimLeft();
	strName.TrimRight();
	if(strName.GetLength() > 0)
	{
		pConfig->SetFileEditor(strName);
	}
}