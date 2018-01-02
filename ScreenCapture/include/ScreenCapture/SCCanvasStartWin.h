#pragma  once 

#include "HYUtilBase.h"
#include "HYUIBase.h"
#include "HYAutoPtr.h"
#include "SCCanvasControl.h"
#include "SCCanvasSkin.h"

#define SC_MASK_COLOR RGB(255, 0, 255)

typedef CWinTraits<WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, WS_EX_TOPMOST | WS_EX_TOOLWINDOW> CSCStartWinTraits;
class CSCCanvasStartWin: public CWindowImpl<CSCCanvasStartWin, CWindow, CSCStartWinTraits>,
						 public CHYBkgndDrawMap<CSCCanvasStartWin>,
						 public CRefCountBase,
						 public CHYAeroWindowMap<CSCCanvasStartWin, SC_MASK_COLOR, 50>
{
public:
	DECLARE_WND_CLASS(_T("SCCanvasStartWin"))
	
	typedef CHYAeroWindowMap<CSCCanvasStartWin, SC_MASK_COLOR, 50> theBase1;
	BEGIN_MSG_MAP(CSCCanvasStartWin)
		CHAIN_MSG_MAP(CHYBkgndDrawMap<CSCCanvasStartWin>)
		MESSAGE_HANDLER(WM_NCCREATE, OnNCCreate)
		CHAIN_MSG_MAP(theBase1)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_HOTKEY, OnHotKey)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	
	CSCCanvasStartWin();
	~CSCCanvasStartWin();
protected:
	LRESULT OnNCCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnHotKey(UINT, WPARAM wParam, LPARAM lParam, BOOL&);

protected:
	VOID LoadResource();
	VOID FreeResource();

	VOID CreateControl();
	VOID DestroyControl();
	VOID LayoutControl();

	BOOL SetupHotKey();
	VOID UnInstallHotKey();

protected:
	CTsButton m_StartBtn;
	CTsButton m_CloseBtn;
	
	CBitmap m_bmpStartBtnBK;
	CImageList m_imgStartBtn;
	CString m_strStartBtnText;

	CBitmap m_bmpCloseBtnBK;
	CImageList m_imgCloseBtn;
	CString m_strCloseBtnTip;

	CBitmap m_bmpAero;
	CFont m_fntBtn;
};

typedef CWinTraitsOR<0, WS_EX_TRANSPARENT, CAeroWinTraits> CHelpTipTraits;
class CSCHelpTipWindow: public CWindowImpl<CSCHelpTipWindow, CWindow, CHelpTipTraits>,
						public CRefCountBase,
						public CHYAeroWindowMap<CSCHelpTipWindow, RGB(0, 0, 0), 150, FALSE>,
						public CHYBkgndDrawMap<CSCHelpTipWindow>,
						public CSCSkinObserver<CSCHelpTipWindow>
{
public:
	DECLARE_WND_CLASS(_T("SCHelpTipWindow"))

	typedef CHYAeroWindowMap<CSCHelpTipWindow, RGB(0, 0, 0), 150, FALSE>  theBase;
	BEGIN_MSG_MAP(CSCHelpTipWindow)
		CHAIN_MSG_MAP(CHYBkgndDrawMap<CSCHelpTipWindow>)
		CHAIN_MSG_MAP(CSCSkinObserver<CSCHelpTipWindow>)
		CHAIN_MSG_MAP(theBase)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_HY_PAINT, OnPaint)
	END_MSG_MAP()

	CSCHelpTipWindow();	
	VOID UpdateRender();

	void DoUIDataChanged(WPARAM wParam, LPARAM lParam);

protected:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);

protected:
	VOID GetIdealWinPos(CPoint& pt);
	VOID DrawText(CDCHandle dc);
	VOID DrawZoomBmp(CDCHandle dc);
	
	VOID GetTextRect(CRect& rtText);
	VOID GetZoomRect(CRect& rtZoom);

	VOID LoadResource();

private:
	CRect m_rtSelect;
	COLORREF m_clrCursor;
	CString m_strHelpTip;
	CPoint m_ptCursor;
};

class CSCInfoBubbleTip: public CWindowImpl<CSCInfoBubbleTip>,
						public CHYBmpWindowMap<CSCInfoBubbleTip>,
						public CRefCountBase,
						public CSCSkinObserver<CSCInfoBubbleTip>
{
public:
	DECLARE_WND_CLASS(_T("CSCInfoBubbleTip"))

	BEGIN_MSG_MAP(CSCInfoBubbleTip)
		CHAIN_MSG_MAP(CHYBmpWindowMap<CSCInfoBubbleTip>)
		CHAIN_MSG_MAP(CSCSkinObserver<CSCInfoBubbleTip>)
		MESSAGE_HANDLER(WM_HY_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
	END_MSG_MAP()
	
	CSCInfoBubbleTip();
	~CSCInfoBubbleTip();
	
	void DoUIDataChanged(WPARAM wParam, LPARAM lParam);

protected:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaint(UINT, WPARAM wParam, LPARAM, BOOL&);

protected:
	VOID LoadResource();
	VOID FreeResource();
	VOID ApplyResource();

protected:
	CBitmap m_bmpBK;
	CString m_strText;
	CFont m_fntText;
};