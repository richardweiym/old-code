#pragma  once

#include "HYUIBase.h"
#include "SCCanvasSkin.h"
#include "SCCanvasControl.h"
#include "SCRes.h"
#include "SCInstanceAPI.h"
#include "HYPattern.h"
#include "../../include/waterWave/WaveObject.h"
#include "atlctrlx.h"

#define SC_DLG_FRAME_WIDTH 1

class CSCCloseButton: public CTsButton,
					  public CSCSkinObserver<CSCCloseButton>
{
	BEGIN_MSG_MAP(CSCCloseButton)
		CHAIN_MSG_MAP(CSCSkinObserver<CSCCloseButton>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		CHAIN_MSG_MAP(CTsButton)
	END_MSG_MAP()		

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);

public:
	void DoUIDataChanged(WPARAM wParam, LPARAM lParam);

protected:
	BOOL LoadResource();
	VOID FreeResource();



};

template<typename T>
class CSCDialogBase: public CDialogImpl<T>,
					 public CSCSkinObserver<T>
{
	BEGIN_MSG_MAP(CSCDialogBase)
		CHAIN_MSG_MAP(CSCSkinObserver<T>)
		
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)

		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
		
		LoadResource();

		if(!(GetStyle() & WS_CHILD) && m_bHaveCloseBtn)
		{
			m_btnClose.Create(m_hWnd, CRect(0, 0, 0, 0), NULL
				, 0, 0, SC_CLOSE_BTN_ID);
		}

		LayOut();
		
		CenterWindow();

		return S_OK;
	}

	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
		
		FreeResource();

		return S_OK;
	}
	

	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;

		LayOut();

		return S_OK;
	}

	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		CPaintDC dc(m_hWnd);
		
		if(!(GetStyle() & WS_CHILD))
		{
			DrawFrame(dc);
			DrawTitleBar(dc);
		}

		return S_OK;
	}

	LRESULT OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL&)
	{
		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if(!m_rgnTitleArea.IsNull() && m_rgnTitleArea.PtInRegion(pt))
		{
			PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pt.x, pt.y));
		}

		return S_OK;
	}

	LRESULT OnCtlColorStatic(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HBRUSH hBrushBK = GetClientBKBrush();
		if(hBrushBK != NULL)
		{
			CDCHandle dc((HDC)wParam);
			dc.SetBkMode(TRANSPARENT);

			return (LRESULT)hBrushBK;
		}
		else
		{
			bHandled = FALSE;
			return NULL;
		}
	}

	LRESULT OnCtlColorDlg(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HBRUSH hBrushBK = GetClientBKBrush();
		if(hBrushBK != NULL)
		{
			return (LRESULT)hBrushBK;
		}
		else
		{
			bHandled = FALSE;
			return NULL;
		}
	}

public:
	CSCDialogBase()
	{
		m_hBmpClientBK = NULL;
		m_hBmpTitleBK = NULL;
		m_clrClientBK = GetSysColor(COLOR_WINDOW);
		m_clrFrame = RGB(0, 0, 0);

		m_bHaveCloseBtn = TRUE;
	}

	~CSCDialogBase()
	{

	}

	void DoUIDataChanged(WPARAM wParam, LPARAM lParam)
	{
		if(wParam == UI_WPARAM_SKIN_CHANGED
			|| wParam == UI_WPARAM_LAN_CHANGED)
		{
			_ASSERTE(IsWindow());

			FreeResource();
			LoadResource();

			LayOut();
			Invalidate();
		}
	}

	VOID Close(BOOL bModal, INT nRet = IDCANCEL)
	{
		if(!IsWindow()) return;

		if(bModal)
		{
			EndDialog(nRet);
		}
		else
		{
			DestroyWindow();
		}
	}

	CRect GetDialogClientRect()
	{
		CRect rtRet(0, 0, 0, 0);
		if(!IsWindow()) return rtRet;

		CRect rtClient;
		GetClientRect(&rtClient);

		CRect rtTitle = GetTitleRect();
		rtRet = rtClient;
		rtRet.InflateRect(-SC_DLG_FRAME_WIDTH, 0, -SC_DLG_FRAME_WIDTH, -SC_DLG_FRAME_WIDTH);
		rtRet.top = rtTitle.bottom;

		return rtRet;
	}

protected:
	HBRUSH GetClientBKBrush()
	{
		if(!m_brushBK.IsNull()) return m_brushBK;

		if(m_hBmpClientBK != NULL)
		{
			_ASSERTE(GetObjectType(m_hBmpClientBK) == OBJ_BITMAP);
			m_brushBK = CreatePatternBrush(m_hBmpClientBK);
			return m_brushBK;
		}
		else
		{
			m_brushBK = CreateSolidBrush(m_clrClientBK);
		}
		
		return m_brushBK;
	}

	BOOL LoadResource()
	{
		CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
		m_hBmpTitleBK = pSkin->GetDlgTitleBK();
		_ASSERTE(m_hBmpTitleBK != NULL);
		
		m_hBmpClientBK = pSkin->GetDlgClientBK();
		m_clrClientBK = pSkin->GetDlgClientColorBK();
		m_clrFrame = pSkin->GetDlgFrameClolor();
		
		m_fontTitle = CHYUICommonFunctionWrapper::CreateFont(TRUE, -12, FALSE, FALSE, pSkin->GetDefaultFontName());
		_ASSERTE(!m_fontTitle.IsNull());

		return TRUE;
	}

	VOID FreeResource()
	{
		if(m_hBmpTitleBK != NULL)
		{
			DeleteObject(m_hBmpTitleBK);
			m_hBmpTitleBK = NULL;
		}
		if(m_hBmpClientBK != NULL)
		{
			DeleteObject(m_hBmpClientBK);
			m_hBmpClientBK = NULL;
		}
		if(!m_brushBK.IsNull())
		{
			m_brushBK.DeleteObject();
		}

		if(!m_rgnTitleArea.IsNull())
		{
			m_rgnTitleArea.DeleteObject();
		}

		if(!m_fontTitle	.IsNull())
		{
			m_fontTitle.DeleteObject();
		}
	}

	VOID LayOut()
	{
		if(m_hBmpTitleBK == NULL || !IsWindow()) return;
		
		if((GetStyle() & WS_CHILD)) return; 

		CRect rtTitle = GetTitleRect();
		CBitmap bmpTitle = CHYUICommonFunctionWrapper::GetMiddleStretchBmp(m_hBmpTitleBK, rtTitle.Width(), FALSE);
		_ASSERTE(!bmpTitle.IsNull());
		if(!bmpTitle.IsNull())
		{
			if(!m_rgnTitleArea.IsNull()) m_rgnTitleArea.DeleteObject();
			m_rgnTitleArea = CHYUICommonFunctionWrapper::BitmapToRgn(bmpTitle.m_hBitmap);

			if(m_bHaveCloseBtn)
			{
				CRect rtCloseBtn = GetCloseBtnRect();
				if(!rtCloseBtn.IsRectEmpty())
				{
					m_btnClose.MoveWindow(&rtCloseBtn, TRUE);
				}
			}
		}	
	}

	VOID DrawFrame(CDC& dc)
	{
		CRect rtClient;
		GetClientRect(&rtClient);
		CPen newPen = CreatePen(PS_SOLID, SC_DLG_FRAME_WIDTH, m_clrFrame);
		CPenHandle oldPen = dc.SelectPen(newPen);
		CBrushHandle oldBrush = dc.SelectBrush((HBRUSH)GetStockObject(NULL_BRUSH));
		dc.Rectangle(&rtClient);
		dc.SelectBrush(oldBrush);
		dc.SelectPen(oldPen);
	}

	VOID DrawTitleBar(CDC& dc)
	{
		CRect rtTitle = GetTitleRect();
		CBitmap bmpTitle = CHYUICommonFunctionWrapper::GetMiddleStretchBmp(m_hBmpTitleBK, rtTitle.Width(), FALSE);
		_ASSERTE(!bmpTitle.IsNull());
		if(!bmpTitle.IsNull())
		{
			CHYUICommonFunctionWrapper::DrawBmp(dc.m_hDC, &rtTitle, bmpTitle.m_hBitmap, FALSE, TRUE);
		}
		
		CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
		

		TCHAR szText[128] = {0};
		::GetWindowText(m_hWnd, szText, sizeof(szText) / sizeof(TCHAR) - 1);
		if(lstrlen(szText) > 0)
		{
			HFONT hFontOld = dc.SelectFont(m_fontTitle);
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(RGB(255, 255, 255));
			rtTitle.InflateRect(-10, 0, -10, 0);
			dc.DrawText(szText, lstrlen(szText), &rtTitle, DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);
			dc.SelectFont(hFontOld);
		}
		
	}

	
	CRect GetCloseBtnRect()
	{
		CRect rtRet(0, 0, 0, 0);
		if(!m_bHaveCloseBtn) return rtRet;

		if(!m_btnClose.IsWindow()) return rtRet;
		CRect rtClient;
		GetClientRect(&rtClient);
		
		const int c_nRightGap = 2;
		const int c_nTopGap = 2;

		CSize sizeButton = m_btnClose.GetIdealSize();
		rtRet.right = rtClient.right - SC_DLG_FRAME_WIDTH - c_nRightGap;
		rtRet.top = rtClient.top + SC_DLG_FRAME_WIDTH + c_nTopGap;
		rtRet.left = rtRet.right - sizeButton.cx;
		rtRet.bottom = rtRet.top + sizeButton.cy;

		return rtRet;
	}

	CRect GetTitleRect()
	{
		CRect rtRet(0, 0, 0, 0);
		if(GetStyle() & WS_CHILD) return rtRet;

		CSize sizeTitle = CHYUICommonFunctionWrapper::GetBitmapSize(m_hBmpTitleBK);
		CRect rtClient;
		GetClientRect(&rtClient);

		const int c_nTitleAndButtonGap = 10;
		
		CRect rtCloseBtn = GetCloseBtnRect();
		rtRet.left = rtClient.left;
		rtRet.top = rtClient.top;
		if(!rtCloseBtn.IsRectEmpty())
		{
			rtRet.right = rtCloseBtn.left - c_nTitleAndButtonGap;
			rtRet.bottom = rtRet.top + sizeTitle.cy;
		}
		else
		{
			rtRet.right = rtClient.right - SC_DLG_FRAME_WIDTH;
			rtRet.bottom = rtRet.top + sizeTitle.cy;
		}
		
		return rtRet;
	}
	
protected:
	CSCCloseButton m_btnClose;
	HBITMAP m_hBmpClientBK;
	COLORREF m_clrClientBK;
	CBrush m_brushBK;
	COLORREF m_clrFrame;

	HBITMAP m_hBmpTitleBK;
	CRgn m_rgnTitleArea;
	CFont m_fontTitle;

	BOOL m_bHaveCloseBtn;
};

class CSCWaterWaveControl: public CWindowImpl<CSCWaterWaveControl>
{
	BEGIN_MSG_MAP(CSCWaterWaveControl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()
	
	DECLARE_WND_CLASS(_T("SCWaveControl"))

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);

public:
	CSCWaterWaveControl();
	CSize GetIdealSize();

protected:
	VOID Layout();
	
protected:
	WAVE_OBJECT m_objWave;
	HBITMAP m_bmpBK;
	
	INT m_nOldX;
	INT m_nOldY;
	INT m_nOldTickCount;
};

class CSCAboutInfoDialog: public CSCDialogBase<CSCAboutInfoDialog>
{
public:
	enum { IDD = IDD_ABOUT_INFO };

	BEGIN_MSG_MAP(CSCAboutInfoDialog)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP(CSCDialogBase<CSCAboutInfoDialog>)
	END_MSG_MAP()

	CSCAboutInfoDialog();
	void DoUIDataChanged(WPARAM wParam, LPARAM lParam);

	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);

protected:
	BOOL LoadRes();
	VOID FreeRes();

protected:
	CHyperLink m_link;

	CTsButton m_btnOK;
	HBITMAP m_bmpBtnOK;
	HIMAGELIST m_imgBtnOK;
};

class CSCAboutDialog: public CSCDialogBase<CSCAboutDialog>,	
					  public CRefCountBase
{
public:
	enum { IDD = IDD_ABOUT };

	BEGIN_MSG_MAP(CSCAboutDialog)
		COMMAND_ID_HANDLER(SC_CLOSE_BTN_ID, OnClickCloseBtn)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		CHAIN_MSG_MAP(CSCDialogBase<CSCAboutDialog>)

		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()		

	LRESULT OnClickCloseBtn(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnOK(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled);
	
	void DoUIDataChanged(WPARAM wParam, LPARAM lParam);

protected:
	VOID AdjustSize();
	CRect GetWaveControlMargin();

protected:
	CSCWaterWaveControl m_wndWave;
	CSCAboutInfoDialog m_dlgInfo;
};

class CSCSettinDialog: public CSCDialogBase<CSCSettinDialog>,
					   public CRefCountBase
{
public:
	enum { IDD = IDD_SETTING };

	BEGIN_MSG_MAP(CSCSettinDialog)
		COMMAND_ID_HANDLER(SC_CLOSE_BTN_ID, OnClickCloseBtn)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDC_CHECK_SHOW_ZOOM, OnShowZoom)
		COMMAND_ID_HANDLER(IDC_CHECK_SHOW_CURSOR, OnShowCursor)
		COMMAND_ID_HANDLER(IDC_CHECK_TRANSPARENT_MARK, OnTransparentMark)
		COMMAND_ID_HANDLER(IDC_CHECK_DBCLICK_CLOSE, OnDBClickClose)
		CHAIN_MSG_MAP(CSCDialogBase<CSCSettinDialog>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	CSCSettinDialog();
	VOID DoUIDataChanged(WPARAM wParam, LPARAM lParam);

	LRESULT OnClickCloseBtn(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnOK(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnShowZoom(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnShowCursor(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnTransparentMark(WORD, WORD, HWND, BOOL& bHandled);
	LRESULT OnDBClickClose(WORD, WORD, HWND, BOOL& bHandled);
	
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);

protected:
	BOOL LoadRes();
	VOID FreeRes();

	VOID UpateUIData();
	VOID SaveFileEditorName();

protected:
	CTsButton m_btnOK;
	HBITMAP m_bmpBtnOK;
	HIMAGELIST m_imgBtnOK;
};