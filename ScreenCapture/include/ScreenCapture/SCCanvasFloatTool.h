#pragma  once 

#include <vector>
#include "HYUtilBase.h"
#include "HYAutoPtr.h"
#include "SCCanvasControl.h"
#include "SCCanvasSkin.h"
#include "SCCanvasContext.h"
#include "HYCustomMenu.h"

typedef CWinTraits<WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS> CFloatTraints; 
typedef CWinTraitsOR<0, WS_EX_TOPMOST, CFloatTraints> CFloatTopMostTraints; 

#define COLOR_PALETTE_COUNT 16
class CColorPickerControl: public CWindowImpl<CColorPickerControl>,
						   public CHYBkgndDrawMap<CColorPickerControl>
{
public:
	DECLARE_WND_CLASS(_T("CanvasColorPicker"))

	BEGIN_MSG_MAP(CColorPickerControl)
		CHAIN_MSG_MAP(CHYBkgndDrawMap<CColorPickerControl>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_HY_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()
	
	CColorPickerControl();
	CSize GetIdealSize() const;
	VOID SetCurrentColor(COLORREF clr);
	COLORREF GetCurrentColor() const { return m_clrCurrent; }
	VOID OnSkinChanged();

protected:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnPaint(UINT, WPARAM, LPARAM,	BOOL& bHandled);
	LRESULT OnLButtonDown(UINT, WPARAM, LPARAM,	 BOOL& bHandled);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled);
	
protected:
	VOID AdjustLayout();
	VOID DrawSelect(CDCHandle dc);
	VOID DrawPalette(CDCHandle dc);

	VOID GetSelectRect(CRect& rtRet);
	VOID GetPaletteRectByIndex(INT nIndex, CRect& rtRet);
	INT HitTest(CPoint& pt);
	
	VOID OnColorChanged(COLORREF clr);

private:
	COLORREF m_clrCurrent;
	static COLORREF s_Palette[COLOR_PALETTE_COUNT];
};

class CFontSizePickerControl: public CWindowImpl<CFontSizePickerControl>,
							  public CMouseHover<CFontSizePickerControl>,
							  public CHYCustomMenuMap<CFontSizePickerControl>
{
public:
	DECLARE_WND_CLASS(_T("FontSizePickerControl"))

	BEGIN_MSG_MAP(CFontSizePickerControl)
		CHAIN_MSG_MAP(CMouseHover<CFontSizePickerControl>)
		CHAIN_MSG_MAP(CHYCustomMenuMap<CFontSizePickerControl>)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	END_MSG_MAP()
	
	CFontSizePickerControl();
	CSize GetIdealSize() const;
	VOID SetCurrentValue(INT nValue);
	INT GetCurrentValue() const { return m_nCurrentValue; }
	VOID OnSkinChanged();

protected:	
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL&);

protected:
	CFontHandle GetTextFont();
	VOID GetFrameRect(CRect& rtFrame);
	VOID GetTextRect(CRect& rtText);
	VOID GetArrowRect(CRect& rtArrow);

	VOID DrawFrame(CDCHandle dc);
	VOID DrawArrow(CDCHandle dc);
	VOID DrawText(CDCHandle dc);

	VOID AdjustLayout();

	BOOL IsInHotStatus() const { return m_bMenuTracking || m_fMouseOver; }
	HMENU CreateTrackMenu();
	VOID GetTrackPos(CPoint& pt, BOOL& bBottomAlign);

private:
	COLORREF m_clrFrameColor;
	COLORREF m_clrArrow;
	COLORREF m_clrSelect;
	COLORREF m_clrText;
	INT m_nCurrentValue;
	BOOL m_bMenuTracking;
};

class CPenPickerTool: public CWindowImpl<CPenPickerTool, CWindow, CFloatTraints>,
					  public CHYBmpWindowMap<CPenPickerTool>,
					  public CSCSkinObserver<CPenPickerTool>
{
public:
	BEGIN_MSG_MAP(_T("PenPickerTool"))
		CHAIN_MSG_MAP(CHYBmpWindowMap<CPenPickerTool>)
		CHAIN_MSG_MAP(CSCSkinObserver<CPenPickerTool>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_HY_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	END_MSG_MAP()

	CPenPickerTool();
	~CPenPickerTool();

	void DoUIDataChanged(WPARAM wParam, LPARAM lParam);
	VOID SetPenType(DWORD dwType);
	VOID SetPaletteColor(COLORREF clr);
	VOID SetNotifyWnd(HWND hWnd) { m_hWndNotify = hWnd;}
	VOID SetFontSize(INT nSize);
	VOID SetFontBold(BOOL bBold);

	INT GetFontSize() { return m_fntSizePicker.GetCurrentValue(); }
	DWORD GetPenType();
	COLORREF GetPaletteColor() const { return m_colorPicker.GetCurrentColor(); }
	BOOL GetFontBold();
	
	VOID ShowPenWidthTools(BOOL bShow, BOOL bUpdate = TRUE);
	VOID ShowFontTools(BOOL bShow, BOOL bUpdate = TRUE);

protected:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaint(UINT, WPARAM wParam, LPARAM, BOOL&);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL&);

protected:
	VOID CreateTools();
	VOID AdjustLayout();

	VOID ReleaseResource();
	VOID LoadResource();
	VOID ApplyResource();	
	
//	VOID DrawFrame(CDCHandle dc);

private:
	CColorPickerControl m_colorPicker;
	CFontSizePickerControl m_fntSizePicker;
	CTsToolBar m_penTool;
	COLORREF m_clrMainTool;
//	COLORREF m_clrFrame;

	CBitmap m_bmpBtnBK;
	CImageList m_imgSmallPen;
	CImageList m_imgMiddlePen;
	CImageList m_imgBigPen;
	CImageList m_imgFontBold;
	
	CBitmap m_bmpSeperator;
	CBitmap m_bmpBK;
	HWND m_hWndNotify;
};


class CSCDragButton: public CTsButton
{
public:
	BEGIN_MSG_MAP(CSCDragButton)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		CHAIN_MSG_MAP(CTsButton)
	END_MSG_MAP()

protected:	
	LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL& );
};


class CSCCanvasFloatTool: public CWindowImpl<CSCCanvasFloatTool, CWindow, CFloatTraints>,
						  public CRefCountBase,
						  public CHYBkgndDrawMap<CSCCanvasFloatTool>,
						  public CSCSkinObserver<CSCCanvasFloatTool>,
						  public CSCContextObserver<CSCCanvasFloatTool>,
						  public CHYCustomMenuMap<CSCCanvasFloatTool>
{
public:
	DECLARE_WND_CLASS(_T("CanvasFloatTool"))
	BEGIN_MSG_MAP(CSCCanvasFloatTool)
		CHAIN_MSG_MAP(CHYBkgndDrawMap<CSCCanvasFloatTool>)
		CHAIN_MSG_MAP(CSCSkinObserver<CSCCanvasFloatTool>)
		CHAIN_MSG_MAP(CSCContextObserver<CSCCanvasFloatTool>)
		CHAIN_MSG_MAP(CHYCustomMenuMap<CSCCanvasFloatTool>)

		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_MOVE, OnMove)
		
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)

		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	
	CSCCanvasFloatTool();
	~CSCCanvasFloatTool();

	void DoUIDataChanged(WPARAM wParam, LPARAM lParam);
	VOID TrackCanvasMenu(CPoint& ptCursor, int nFlag = 0);
	
	HBITMAP GetMenuIconByID(INT nID);

protected:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCommand(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT, WPARAM, LPARAM, BOOL& );

protected:
	VOID CreateMainTools();
	VOID AdjustLayout();

	VOID ReleaseResource();
	VOID LoadResource();
	VOID ApplyResource();
	
	VOID CheckCurrentTool();
	VOID ShowSubFloatTool(DWORD dwBtnID);

	VOID ShowPenSelectTool();
	VOID OutputPenToolData();
	VOID InputPenToolData();

	VOID ShowFontSelectTool();
	VOID OutputFontToolData();
	VOID InputFontToolData();

	VOID CreateSubTools();
	VOID DestroySubTools();
	
	VOID GetSubFloatToolPos(CPoint& pt);
	VOID GetMenuTrackPos(CPoint& pt, BOOL& bBottomAlign);

	VOID TrackToolExtendMenu();

protected:
//	VOID DrawFrame(CDCHandle dc);

	BOOL CreateFontToolData();
	VOID DeleteFontToolData();
	VOID ResetFontToolData();

	HMENU CreateMainMenu();
	VOID AppendToolMenu(HMENU hParent);
	VOID AppendSkinMenu(HMENU hParent);
	VOID AppendLanMenu(HMENU hParent);
	VOID AppendOperationMenu(HMENU hParent);

private:
	CBitmap m_bmpBtnBK;
	CBitmap m_bmpSeperator;

	CImageList m_imgBtnSelect;
	CString m_strTipBtnSelect;
	CImageList m_imgBtnWinSelect;
	CString m_strTipBtnWinSelect;
	CImageList m_imgBtnRect;
	CString m_strTipBtnRect;
	CImageList m_imgBtnEllipse;
	CString m_strTipBtnEllipse;
	CImageList m_imgBtnLine;
	CString m_strTipBtnLine;
	CImageList m_imgBtnPen;
	CString m_strTipBtnPen;
	CImageList m_imgBtnText;
	CString m_strTipBtnText;
	CImageList m_imgBtnEraser;
	CString m_strTipBtnEraser;
	CImageList m_imgBtnArrow;
	CString m_strTipBtnArrow;

	COLORREF m_clrMainTool;
//	COLORREF m_clrFrame;

	CSCDragButton m_btnDrag;
	CTsButton m_btnMenu;

	CImageList m_imgBtnDrag;
	CBitmap m_bmpBtnDrag;

	CImageList m_imgBtnMenu;
	CBitmap m_bmpBtnMenu;
	CString m_strTipBtnMenu;

	CBitmap m_bmpWinBK;

private:
	CPenPickerTool m_penPicker;
	CTsToolBar m_mainToolBar;
};
