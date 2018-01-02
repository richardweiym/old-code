#include "SCCanvasWindow.h"
#include "sccanvasdata.h"
#include "HYUIBase.h"
#include "SCCanvasDrawDirector.h"
#include "SCIDDef.h"
#include "SCCanvasContext.h"
#include "SCCanvasFloatTool.h"
#include "SCCanvasGraph.h"
#include "SCCanvasCmdMgr.h"
#include "SCCanvasStartWin.h"
#include "SCCanvasConfig.h"
#include "SCRes.h"
#include "SCWinSpy.h"
#include "HYTimer.h"
#include "SCXImage.h"
#include "SCDialog.h"

HWND g_hWndSaveDlg = NULL; //save dialog handle
BOOL g_bSaveAndEdit = FALSE;

#define INFO_TIP_TIMER_ID 100
#define INFO_TIP_SHOW_ELAPSE 2000

struct OPENFILENAMEEX : public OPENFILENAME { 
  void *        pvReserved;
  DWORD         dwReserved;
  DWORD         FlagsEx;
};

class CSCFileFilterMgr
{
public:
	LPCTSTR GetFileFilter() const
	{
		LPCTSTR lpszImageFilter = _T("BMP File (*.BMP)\0*.BMP\0\
								PNG File (*.PNG)\0*.PNG\0\
								JPEG File (*.JPEG;*.JPG)\0*.JPEG;*.JPG\0\
								TIFF File (*.TIF;*.TIFF)\0*.TIF;*.TIFF\0\
								GIF File (*.GIF)\0*.GIF\0\0");
								
		return lpszImageFilter;
	}

	INT GetFilterIndex(LPCTSTR lpszFileName)
	{
		INT nIndex(-1);

		LPTSTR lpszExt = (LPTSTR)_tcsrchr(lpszFileName, _T('.'));
		if(lpszExt == NULL) return nIndex;

		++lpszExt;
		CString strExt = lpszExt;
		strExt.MakeUpper();	

		if(strExt == _T("BMP"))
		{
			nIndex = 1;
		}
		else if(strExt == _T("PNG"))
		{
			nIndex = 2;
		}
		else if(strExt == _T("JPG") || strExt == _T("JPEG"))
		{
			nIndex = 3;
		}
		else if(strExt == _T("TIF") || strExt == _T("TIFF"))
		{
			nIndex = 4;
		}
		else if(strExt == _T("GIF"))
		{
			nIndex = 5;
		}

		return nIndex;
	}

	LPCTSTR GetFilterName(INT nIndex)
	{
		switch(nIndex)
		{
		case 1:
			return _T("BMP");
			break;

		case 2:
			return _T("PNG");
			break;

		case 3:
			return _T("JPG");
			break;

		case 4:
			return _T("TIF");
			break;

		case 5:
			return _T("GIF");
			break;

		default:
			_ASSERTE(FALSE);
		}

		return NULL;
	}

	INT GetMaxFilterIndex()
	{
		return 5;
	}
};

CSCCanvasWindow::CSCCanvasWindow()
{
	m_bExiting = FALSE;
	m_hAccel = NULL;

	WNDCLASSEX& wndClass = GetWndClassInfo().m_wc;
	wndClass.style |= CS_OWNDC;

}

CSCCanvasWindow::~CSCCanvasWindow()
{

}
BOOL CSCCanvasWindow::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYFIRST) return FALSE;

 	if(m_hAccel != NULL && m_hWnd != NULL && ::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
 	{
 		return TRUE;
 	}

	return FALSE;
}

BOOL CSCCanvasWindow::OnIdle()
{
	return FALSE;
}

CRefPtr<CSCCanvasData> CSCCanvasWindow::GetCanvasData() const
{
	return m_pCanvasData;
}


CRefPtr<CSCCanvasContext> CSCCanvasWindow::GetCanvasContext() const
{
	return m_pCanvasContext;
}

BOOL CSCCanvasWindow::InitCanvasData()
{
	CSCScreenSnapshot Snapshot;
	
	CBitmapHandle bmpScreen = Snapshot;
	if(bmpScreen.IsNull()) return FALSE;

	_ASSERTE(!m_pCanvasData.IsNull());
	m_pCanvasData->SetScreenBmp(bmpScreen);

	return TRUE;
}
BOOL CSCCanvasWindow::Initialize()
{
	_ASSERTE(m_pStartWin.IsNull());
	if(m_pStartWin.IsNull())
	{
		m_pStartWin = new CSCCanvasStartWin;
	}
	if(m_pStartWin.IsNull()) return FALSE;

	_ASSERTE(m_pCmdMgr.IsNull());
	if(m_pCmdMgr.IsNull())
	{
		m_pCmdMgr = new CSCCanvasCmdMgr;
	}
	if(m_pCmdMgr.IsNull()) return FALSE;

	_ASSERTE(m_pCanvasData.IsNull());
	if(m_pCanvasData.IsNull())
	{
		m_pCanvasData = new CSCCanvasData;
	}
	if(m_pCanvasData.IsNull()) return FALSE;
	
	if(!InitCanvasData()) return FALSE;

	_ASSERTE(m_pCanvasContext.IsNull());
	if(m_pCanvasContext.IsNull())
	{
		m_pCanvasContext = new CSCCanvasContext;
	}
	if(m_pCanvasContext.IsNull()
		|| !m_pCanvasContext->Initialize()) 
	{
		return FALSE;
	}

	_ASSERTE(m_pDrawDirector.IsNull());
	if(m_pDrawDirector.IsNull())
	{
		m_pDrawDirector = new CSCCanvasDrawDirector();
	}
	if(m_pDrawDirector.IsNull()) return FALSE;
	
	_ASSERTE(m_pFloatTool.IsNull());
	m_pFloatTool = new CSCCanvasFloatTool;
	if(m_pFloatTool.IsNull()) return FALSE;

	_ASSERTE(m_pHelpTipWin.IsNull());
	if(m_pHelpTipWin.IsNull())
	{
		m_pHelpTipWin = new CSCHelpTipWindow;
	}
	if(m_pHelpTipWin.IsNull()) return FALSE;

	if(m_pAboutDialog.IsNull())
	{
		m_pAboutDialog = new CSCAboutDialog;
	}
	if(m_pAboutDialog.IsNull()) return FALSE;

	if(m_pSettingDialog.IsNull())
	{
		m_pSettingDialog = new CSCSettinDialog;
	}
	if(m_pSettingDialog.IsNull()) return FALSE;

	return TRUE;
}

BOOL CSCCanvasWindow::InitializeUI()
{
	CHYCustomMenuMgr::InitializeHook();
	CHYCustomMenuMgr::SetMenuDrawer(GetCanvasSkin()->GetCurrentMenuDrawer());

	CreateAccelTable();

	if(!CreateCanvas() || !CreateToolPanel()) 
	{
		return FALSE;
	}

	InitCanvasTool();

	m_pStartWin->Create(NULL, CRect(0, 0, 150, 100));

	BOOL bShowStartWin(TRUE);
	if(!m_pCanvasContext.IsNull())
	{
		bShowStartWin = m_pCanvasContext->GetShowStartWin();
	}
	if(bShowStartWin)
	{
		if(m_pStartWin->IsWindow())
		{
			m_pStartWin->CenterWindow();
			m_pStartWin->ShowWindow(SW_SHOW);
			m_pStartWin->UpdateWindow();
		}
	}
	else
	{
		PostMessage(WM_COMMAND, MAKEWPARAM(SC_START_CAPTURE_BTN_ID, 1), 0);
	}

	
	return TRUE;
}

VOID CSCCanvasWindow::Uninitialize()
{
	if(!m_pAboutDialog.IsNull())
	{
		m_pAboutDialog = NULL;
	}

	if(!m_pSettingDialog.IsNull())
	{
		m_pSettingDialog = NULL;
	}

	if(!m_pStartWin.IsNull())
	{
		m_pStartWin = NULL;
	}
	
	if(!m_pFloatTool.IsNull())
	{
		m_pFloatTool = NULL;
	}
	
	if(!m_pHelpTipWin.IsNull())
	{
		m_pHelpTipWin = NULL;
	}

	if(!m_pDrawDirector.IsNull())
	{
		m_pDrawDirector = NULL;
	}

	if(!m_pCanvasContext.IsNull())
	{
		m_pCanvasContext->Uninitialize();
		m_pCanvasContext = NULL;
	}

	if(!m_pCanvasData.IsNull())
	{
		m_pCanvasData->ClearData();
		m_pCanvasData = NULL;
	}

	if(!m_pCmdMgr.IsNull())
	{
		m_pCmdMgr = NULL;
	}
	
	CSCWinSpy<CSCWinFilter>::DeleteInstance();
}

VOID CSCCanvasWindow::UninitializeUI()
{
	CHYCustomMenuMgr::UninitializeHook();
	
	if(m_pAboutDialog->IsWindow())
	{
		m_pAboutDialog->Close(FALSE);
	}

	if(m_pSettingDialog->IsWindow())
	{
		m_pSettingDialog->Close(FALSE);
	}

	if(m_pHelpTipWin->IsWindow())
	{
		m_pHelpTipWin->DestroyWindow();
	}

	if(m_pStartWin->IsWindow())
	{
		m_pStartWin->DestroyWindow();
	}

	if(!m_pFloatTool.IsNull())
	{
		if(m_pFloatTool->IsWindow())
		{
			m_pFloatTool->DestroyWindow();
		}
	}

	if(IsWindow())
	{
		DestroyWindow();
	}

	DestroyAccelTable();
}


BOOL CSCCanvasWindow::CreateCanvas()
{
	if(IsWindow()) return TRUE;

//	for multi-monitor support
	INT nLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
	INT nTop = GetSystemMetrics(SM_YVIRTUALSCREEN);

	CRect rtWin(nLeft, nTop, m_pCanvasData->GetScreenSize().cx, m_pCanvasData->GetScreenSize().cy);
	Create(NULL, rtWin);

	m_pHelpTipWin->Create(m_hWnd, CRect(0, 0, HELP_TIP_WIN_WIDTH, HELP_TIP_WIN_HEIGHT));

	return IsWindow();
}
BOOL CSCCanvasWindow::CreateToolPanel()
{
	if(m_pFloatTool->IsWindow()) return TRUE;

	CRect rtWin(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	m_pFloatTool->Create(m_hWnd, CRect(0, 0, 0, 0));

	CRect rtFloat;
	m_pFloatTool->GetWindowRect(&rtFloat);

	CPoint ptFloat(rtWin.BottomRight());
	ptFloat.Offset(-rtFloat.Width(), -rtFloat.Height());
	ptFloat.Offset(-FLOAT_TOOL_X_OFFSET, -FLOAT_TOOL_Y_OFFSET);

	m_pFloatTool->SetWindowPos(NULL, ptFloat.x, ptFloat.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);

	return m_pFloatTool->IsWindow();
}

LRESULT CSCCanvasWindow::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	
	return S_OK;
}
LRESULT CSCCanvasWindow::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	PostQuitMessage(0);
	m_bExiting = FALSE;
	return S_OK;
}


LRESULT CSCCanvasWindow::OnLButtonDBClick(UINT, WPARAM, LPARAM, BOOL&)
{
	if(m_pCanvasContext->GetDoubleClickExitFlag())
	{
		OnCopyAndExitCmd();
	}

	return S_OK;
}

void CSCCanvasWindow::DoUIDataChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam == UI_WPARAM_SKIN_CHANGED
		|| wParam == UI_WPARAM_LAN_CHANGED)
	{
		_ASSERTE(!m_pDrawDirector.IsNull());
		m_pDrawDirector->OnSkinChanged(lParam);
		RedrawCanvas(NULL);
		
		if(wParam == UI_WPARAM_LAN_CHANGED)
		{
			CloseSaveDlg();
		}

		CHYCustomMenuMgr::SetMenuDrawer(GetCanvasSkin()->GetCurrentMenuDrawer());
	}
	else if(wParam == UI_WPARAM_TOOL_CHANGED)
	{
		m_pDrawDirector->CheckCurrentDrawer();
		RedrawCanvas(NULL);
	}
	else if(wParam == UI_WPARAM_SELECT_RECT_CHANGED)
	{
		UpdateHelpTip();
	}
}

LRESULT CSCCanvasWindow::OnSetCursor(UINT, WPARAM, LPARAM, BOOL&)
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	
	SetCursor(pSkin->GetDefaultCursor());

	return TRUE;
}
LRESULT CSCCanvasWindow::OnCommand(UINT nMsgTye, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WORD wCmdID = LOWORD(wParam);

	switch(wCmdID)
	{
	case SC_START_CAPTURE_BTN_ID:
		if((HIWORD(wParam) == 1)
			|| (m_pStartWin->IsWindow() && m_pStartWin->IsWindowVisible()))
		{
			OnStartCaptureCmd();
		}
		break;

	case SC_CLOSE_CAPTURE_BTN_ID:
		ExitApp();
		break;

	case SC_TOOL_UNDO_CMD_ID:
		if(IsWindowVisible()) m_pCmdMgr->Undo();
		break;

	case SC_TOOL_REDO_CMD_ID:
		if(IsWindowVisible()) m_pCmdMgr->Redo();	
		break;

	case SC_SHOW_HELP_TIP_CMD_ID:
		{
			if(IsWindowVisible())
			{
				m_pCanvasContext->SetShowHelpFlag(!(m_pCanvasContext->GetShowHelpFlag()));
				
				if(m_pCanvasContext->GetShowHelpFlag())
				{
					UpdateHelpTip();
					m_pHelpTipWin->ShowWindow(SW_SHOW);
				}
				else
				{
					m_pHelpTipWin->ShowWindow(SW_HIDE);
				}
			}
		}
		break;

	case SC_DOUBLE_CLICK_TO_EXIT:
		{
			if(IsWindowVisible())
			{
				m_pCanvasContext->SetDoubleClickExitFlag(!(m_pCanvasContext->GetDoubleClickExitFlag()));
			}
		}
		break;
	
	case SC_SHOW_CURSOR_CMD_ID:
		{
			if(IsWindowVisible())
			{
				m_pCanvasContext->SetShowCursorFlag(!(m_pCanvasContext->GetShowCursorFlag()));
			}

			RedrawCanvas(NULL);
		}
		break;

	case SC_TRANSPARENT_MARK_ID:
		{
			if(IsWindowVisible())
			{
				m_pCanvasContext->SetTransparentMark(!(m_pCanvasContext->GetTransparentMark()));
			}

			RedrawCanvas(NULL);
		}
		break;

	case SC_SAVE_CMD_ID:
		if(IsWindowVisible())
		{
			OnSaveCommand();
		}
		break;

	case SC_SAVE_EDIT_CMD_ID:
		if(IsWindowVisible())
		{
			OnSaveEditCommand();
		}
		break;

	case SC_COPY_EXIT_CMD_ID:
		if(IsWindowVisible())	OnCopyAndExitCmd();	
		break;
	
	case SC_EXIT_CMD_ID:
		ExitApp();
		break;

	case SC_ABOUT_ID:
		{
			if(!m_pAboutDialog->IsWindow())
			{
				m_pAboutDialog->Create(m_hWnd);
			}
			m_pAboutDialog->ShowWindow(SW_SHOW);
			m_pAboutDialog->BringWindowToTop();
		}
		break;

	case SC_SETTING_ID:
		{
			if(!m_pSettingDialog->IsWindow())
			{
				m_pSettingDialog->Create(m_hWnd);
			}
			m_pSettingDialog->ShowWindow(SW_SHOW);
			m_pSettingDialog->BringWindowToTop();
		}
		break;

	default:
		if(wCmdID > SC_TOOL_ID_BASE && wCmdID < SC_TOOL_ID_END) //tool select command
		{
			if(IsWindowVisible()) OnToolSelectCmd(wCmdID);
		}
		else if(wCmdID > SC_SKIN_TYPE_ID_BASE && wCmdID < SC_SKIN_TYPE_ID_END) //skin change command
		{
			CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
			if(IsWindowVisible()) pSkin->SetSkinType(wCmdID - SC_SKIN_TYPE_ID_BASE);
		}
		else if(wCmdID > SC_LAN_TYPE_ID_BASE && wCmdID < SC_LAN_TYPE_ID_END) // language change command
		{
			CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
			if(IsWindowVisible()) pSkin->SetLanType(wCmdID - SC_LAN_TYPE_ID_BASE);
		}
	}

	return S_OK;
}

VOID CSCCanvasWindow::OnAddGraph(CRefPtr<CSCGraphObject> pGraph)
{
	m_pCanvasData->AddGraphObject(pGraph);
	RedrawCanvas(NULL);
}

VOID CSCCanvasWindow::OnEraseGraph(CRefPtr<CSCGraphObject> pGraph)
{
	_ASSERTE(!pGraph.IsNull());
	DWORD dwGraphID = pGraph->GetGraphID();

	m_pCanvasData->RemoveGraphObject(dwGraphID);

	RedrawCanvas(NULL);
}

LRESULT CSCCanvasWindow::OnDrawEvent(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam == DRAW_EVENT_WPARAM_DRAW_START)
	{
		NULL;
	}
	else if(wParam == DRAW_EVENT_WPARAM_DRAW_END)
	{
		CRefPtr<CSCGraphObject> pGraph = (CSCGraphObject*)lParam;
		_ASSERTE(!pGraph.IsNull());
		OnAddGraph(pGraph);
	
		m_pCmdMgr->OnAddGraphDone(pGraph);
	}
	else if(wParam == DRAW_EVENT_WPARAM_ERASER)
	{
		DWORD dwGraphID = lParam;
		_ASSERTE(dwGraphID != 0);
	
		CRefPtr<CSCGraphObject> pGraph = m_pCanvasData->GetGraphByID(dwGraphID);
		_ASSERTE(!pGraph.IsNull());
		OnEraseGraph(pGraph);

		m_pCmdMgr->OnEraseGraphDone(pGraph);
	}
	else if(wParam == DRAW_EVENT_WPARAM_SELECT_CHANGED)
	{
		CRectChangeData* pRectChangeData = (CRectChangeData*)lParam;
		_ASSERTE(pRectChangeData != NULL);

		m_pCmdMgr->OnSelectRectChangeDone(pRectChangeData->rtOld, pRectChangeData->rtNew);
	}
	
	return S_OK;
}

LRESULT CSCCanvasWindow::OnMouseMove(UINT, WPARAM, LPARAM, BOOL&)
{
	UpdateHelpTip();

	return S_OK;
}

LRESULT CSCCanvasWindow::OnRButtonUp(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	CPoint ptTrack(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	
	//for multi-monitor support
	CRect rtWin;
	GetWindowRect(&rtWin);
	ptTrack.Offset(rtWin.left, rtWin.top);

	m_pFloatTool->TrackCanvasMenu(ptTrack);

	return S_OK;
}

LRESULT CSCCanvasWindow::OnTimer(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	_ASSERTE(wParam == INFO_TIP_TIMER_ID);
	if(wParam == INFO_TIP_TIMER_ID)
	{
		ShowBmpCopiedBubble(FALSE);
		ExitApp();
	}

	return S_OK;
}

LRESULT CSCCanvasWindow::OnToolSelectCmd(WORD nCmd)
{
	WORD wCmdID = nCmd;
	_ASSERTE(wCmdID > SC_TOOL_ID_BASE && wCmdID < SC_TOOL_ID_END);
	
	//tool select command

	int nCount(0);
	DWORD dwToolType(TOOL_TYPE_NONE);
	TOOL_ID_PAIR* arTools = GetToolBtnIDTable(nCount);
	for(int i=0; i<nCount; ++i)
	{
		if(arTools[i].dwBtnID == wCmdID)
		{
			dwToolType = arTools[i].dwToolType;
			break;
		}
	}
	
	m_pCanvasContext->SetToolType(dwToolType);
	
	return S_OK;
}

VOID CSCCanvasWindow::InitCanvasTool()
{
	m_pCanvasContext->SetToolType(TOOL_TYPE_SELECT);
}

VOID CSCCanvasWindow::CreateAccelTable()
{
	if(m_hAccel != NULL)
	{
		_ASSERTE(FALSE); 
		return;
	}

	INT nCount(0);
	LPACCEL pAccelTable = GetAllelTable(nCount);

	m_hAccel = CreateAcceleratorTable(pAccelTable, nCount);
}

VOID CSCCanvasWindow::DestroyAccelTable()
{
	if(m_hAccel != NULL)
	{
		DestroyAcceleratorTable(m_hAccel);
		m_hAccel = NULL;
	}
}

HWND GetSaveMainDlg()
{
	if(g_hWndSaveDlg == NULL|| !::IsWindow(g_hWndSaveDlg)) return NULL;

	HWND hWndParent = g_hWndSaveDlg;
	while(::GetWindowLong(hWndParent, GWL_STYLE) & WS_CHILD)
	{
		hWndParent = ::GetParent(hWndParent);
	}
		
	return hWndParent;
}

VOID UpdateExitAfterSaveOption(HWND hWndDlg, BOOL bRead)
{
	CButton btnCheck = ::GetDlgItem(hWndDlg, IDC_CHECK_CLOSE_SCREEN_CAPTURE);
	_ASSERTE(btnCheck.IsWindow());
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();

	if(bRead)
	{
		if(g_bSaveAndEdit)
		{
			btnCheck.EnableWindow(FALSE);
			btnCheck.SetCheck(TRUE);
		}
		else
		{
			btnCheck.SetCheck(pContext->GetExitAfterImageSaved());
		}
	}
	else
	{
		if(!g_bSaveAndEdit)
		{
			pContext->SetExitAfterImageSaved(btnCheck.GetCheck());
		}
	}
}

UINT_PTR CALLBACK OFNHookProc(          HWND hdlg,
    UINT uiMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		g_hWndSaveDlg = hdlg;
		{
			CWindow winDlg = GetSaveMainDlg();
			winDlg.CenterWindow(GetDesktopWindow());

			UpdateExitAfterSaveOption(hdlg, TRUE);
		}
		break;
		
	case WM_DESTROY:
		g_hWndSaveDlg = NULL;
		break;

	case WM_NOTIFY:
		{
			LPOFNOTIFY lpOFN = (LPOFNOTIFY)lParam;
			if(lpOFN-> hdr.code == CDN_FILEOK)
			{
				UpdateExitAfterSaveOption(hdlg, FALSE);
			}
		}
		break;

	default:
		break;
	}

	return FALSE;
}



VOID CSCCanvasWindow::CloseSaveDlg()
{
	HWND hMainDlg = GetSaveMainDlg();
	if(hMainDlg != NULL && ::IsWindow(hMainDlg))
	{
		::SendMessage(hMainDlg, WM_CLOSE, 0, 0);
	}
}


CString CSCCanvasWindow::GetSaveFile()
{
	CString strRet(_T(""));
	OPENFILENAMEEX ofn;       // common dialog box structure
	TCHAR szFile[MAX_PATH] = {0} ;      // buffer for file name

	TCHAR szInitPath[MAX_PATH] = {0};
	CRefPtr<CSCCanvasConfig> pConfig = GetCanvasConfig();
	pConfig->GetDefaultSavePath(szInitPath, sizeof(szInitPath) / sizeof(TCHAR) - 1);

	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	CString strTitle = pSkin->GetResString(IDS_SAVE_DLG_TITLE); //save dialog title
	CString strDefName = pSkin->GetResString(IDS_SAVE_FILE_DEF_NAME);

	lstrcpyn(szFile, strDefName, sizeof(szFile) / sizeof(TCHAR) - 1); //set default name
	
	CSCFileFilterMgr fileFilter;
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	INT nFilterIndex = pContext->GetFileFilterIndex();
	if(nFilterIndex <= 0 || nFilterIndex > fileFilter.GetMaxFilterIndex())
	{
		nFilterIndex = 1;
	}

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile) / sizeof(TCHAR) - 1;
	ofn.lpstrFilter = fileFilter.GetFileFilter();
	ofn.nFilterIndex = nFilterIndex;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrDefExt = fileFilter.GetFilterName(nFilterIndex);
	ofn.nMaxFileTitle = 0;
	ofn.lpstrTitle = strTitle;
	ofn.Flags = OFN_ENABLEHOOK | OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING;
	ofn.lpfnHook = (LPOFNHOOKPROC)&OFNHookProc;

	ofn.Flags |= OFN_ENABLETEMPLATE;
	ofn.lpTemplateName = MAKEINTRESOURCE(IDD_SAVE_IMAGE);
	ofn.hInstance = _Module.GetResourceInstance();

	if(lstrlen(szInitPath) > 0)
	{
		ofn.lpstrInitialDir = szInitPath;
	}
	else
	{
		ofn.lpstrInitialDir = NULL;
	}

	if(GetSaveFileName(&ofn))
	{	
		strRet = szFile;

		// save last path
		LPTSTR p = _tcsrchr(szFile, _T('\\'));
		if(p != NULL)
		{
			*(++p) = _T('\0');
			pConfig->SetDefaultSavePath(szFile);
		}

		//check extend file name
		INT nIndex = fileFilter.GetFilterIndex(strRet);
		if(nIndex <= 0)
		{
			LPCTSTR lpszTempFilterName = fileFilter.GetFilterName(ofn.nFilterIndex);
			_ASSERTE(lpszTempFilterName != NULL);
			strRet += _T(".");
			strRet += lpszTempFilterName;
		}
		nIndex = fileFilter.GetFilterIndex(strRet);
		_ASSERTE(nIndex > 0);
		pContext->SetFileFilterIndex(nIndex);
	}

	return strRet;
}

VOID CSCCanvasWindow::OnSaveCommand()
{
	if(g_hWndSaveDlg != NULL 
		&& ::IsWindow(g_hWndSaveDlg))
	{
		::SetActiveWindow(GetSaveMainDlg());
	}
	else
	{
		g_bSaveAndEdit = FALSE;

		CString strFileName = GetSaveFile();
		if(strFileName != _T(""))
		{
			m_pDrawDirector->RedrawCopyDataDC(FALSE);
			CBitmapHandle bmpSelect = m_pCanvasData->GetSelectBMP();
			if(bmpSelect != NULL)
			{
				SC_SaveBitmap2File(bmpSelect, strFileName);
			}
			bmpSelect.DeleteObject();
			
			if(m_pCanvasContext->GetExitAfterImageSaved())
			{
				ExitApp();
			}
		}
	}
}

VOID CSCCanvasWindow::OnSaveEditCommand()
{
	if(g_hWndSaveDlg != NULL 
		&& ::IsWindow(g_hWndSaveDlg))
	{
		::SetActiveWindow(GetSaveMainDlg());
	}
	else
	{
		g_bSaveAndEdit = TRUE;

		CString strFileName = GetSaveFile();
		if(strFileName != _T(""))
		{
			m_pDrawDirector->RedrawCopyDataDC(FALSE);
			CBitmapHandle bmpSelect = m_pCanvasData->GetSelectBMP();
			if(bmpSelect != NULL)
			{
				SC_SaveBitmap2File(bmpSelect, strFileName);
			}
			bmpSelect.DeleteObject();

			ExitApp();

			StartEditImage(strFileName);
		}
	}
}

VOID CSCCanvasWindow::OnCopyAndExitCmd()
{
	if(m_bExiting) return;

	CopySelectToClipboard();
	if(ShowBmpCopiedBubble(TRUE))
	{
		if(SetTimer(INFO_TIP_TIMER_ID, INFO_TIP_SHOW_ELAPSE) != NULL)
		{
			m_bExiting = TRUE;
		}
	}
	
	if(!m_bExiting)
	{
		ExitApp();
	}
}


VOID CSCCanvasWindow::ExitApp()
{
	DestroyWindow();
}

VOID CSCCanvasWindow::StartCapture()
{
	m_pCanvasData->SnapshotCurrentCursor();

	InitCanvasData();

	ShowWindow(SW_SHOW);
	m_pFloatTool->ShowWindow(SW_SHOW);
	UpdateWindow();

	m_pDrawDirector->RedrawCopyDataDC(FALSE); 

	if(m_pCanvasContext->GetShowHelpFlag())
	{
		m_pHelpTipWin->ShowWindow(SW_SHOWNA);
	}

	::SetForegroundWindow(*m_pFloatTool);

	CSCWinSpy<CSCWinFilter>::GetInstance()->SnapshotAllWinRect();
}

VOID CSCCanvasWindow::OnStartCaptureCmd()
{
	if(m_pStartWin->IsWindow())
	{
		m_pStartWin->ShowWindow(SW_HIDE);
	}
	
	Sleep(200); //avoid desktop flash issue

	StartCapture();
}


VOID CSCCanvasWindow::CopySelectToClipboard()
{
	m_pDrawDirector->RedrawCopyDataDC(FALSE);
	CBitmapHandle bmpClipboard = m_pCanvasData->GetSelectBMP();

	if(!bmpClipboard.IsNull() && OpenClipboard()) 
	{ 
		EmptyClipboard(); 
		SetClipboardData(CF_BITMAP, bmpClipboard); 
		CloseClipboard(); 
	} 	
}

VOID CSCCanvasWindow::UpdateHelpTip()
{
	if(m_pCanvasContext->GetShowHelpFlag())
	{
		m_pDrawDirector->RedrawCopyDataDC(TRUE);
		m_pHelpTipWin->UpdateRender();
	}
}

VOID CSCCanvasWindow::RedrawCanvas(CRect* pRtDraw, BOOL bUpdate /* = TRUE */)
{
	InvalidateRect(pRtDraw, TRUE);

	if(bUpdate)
	{
		UpdateWindow();
	}

	UpdateHelpTip();
}

BOOL CSCCanvasWindow::CanUndo()
{
	return !(m_pCmdMgr->IsUndoStackEmpty());
}

BOOL CSCCanvasWindow::CanRedo()
{
	return !(m_pCmdMgr->IsRedoStackEmtpy());
}

BOOL CSCCanvasWindow::ShowBmpCopiedBubble(BOOL bShow)
{
	if(bShow)
	{
		if(m_pInfoBubble.IsNull())
		{
			m_pInfoBubble = new CSCInfoBubbleTip;
		}
		
		if(m_pInfoBubble.IsNull()) return FALSE;
		

		if(!m_pInfoBubble->IsWindow())
		{
			m_pInfoBubble->Create(m_hWnd, CRect(0, 0, 0, 0), NULL, WS_POPUP);
		}

		if(!m_pInfoBubble->IsWindow()) return FALSE;

		m_pInfoBubble->CenterWindow();
		m_pInfoBubble->ShowWindow(SW_SHOW);
		m_pInfoBubble->UpdateWindow();
	}
	else
	{
		if(!m_pInfoBubble.IsNull())
		{
			if(m_pInfoBubble->IsWindow())
			{
				m_pInfoBubble->DestroyWindow();
			}

			m_pInfoBubble = NULL;
		}
	}

	return TRUE;
}

VOID CSCCanvasWindow::StartEditImage(LPCTSTR lpszFileName)
{
	if(!PathFileExists(lpszFileName)) return;
	TCHAR szFileEdit[MAX_PATH] = {0};
	CRefPtr<CSCCanvasConfig> pConfig = GetCanvasConfig();
	pConfig->GetFileEditor(szFileEdit, sizeof(szFileEdit) / sizeof(TCHAR));
	if(lstrlen(szFileEdit) <= 0) return;

	CString strCmdLine;
	strCmdLine.Format(_T("\"%s\" \"%s\""), szFileEdit, lpszFileName);

	STARTUPINFO si = {0};
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow  = SW_SHOW;
	
	PROCESS_INFORMATION  pi = {0};
	BOOL bRet = CreateProcess(NULL, (LPTSTR)(LPCTSTR)strCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bRet)
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}