#pragma  once 

#include "HYUtilBase.h"
#include "HYUIBase.h"
#include "atlctrls.h"
#include <vector>

#define TS_BTN_CENTER_TEXT_STYLE 0x00000001
#define TS_BTN_RGN_STYLE 0x00000002 //use bkgnd bitmap as window regin
#define TS_BTN_AUTO_FIT_STYLE 0x00000004 //auto change the button to bkgnd bitmap size

typedef   CControlWinTraits CTsButtonTraits;
class CTsButton: public CWindowImpl<CTsButton, CButton, CTsButtonTraits>,
				 public CMouseHover<CTsButton>
{
public:
	DECLARE_WND_SUPERCLASS(_T("TsButton"), CButton::GetWndClassName())

	BEGIN_MSG_MAP(CTsButton)
		CHAIN_MSG_MAP(CMouseHover<CTsButton>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
		MESSAGE_HANDLER(BM_SETCHECK, OnSetCheck)
		MESSAGE_HANDLER(BM_GETCHECK, OnGetCheck)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()

	enum
	{
		//imagelist should follow this sequence
		_index_normal = 0,
		_index_hover = 1,
		_index_pressed = 2,
		_index_checked = 3,
		_index_disable = 4
	};


	CTsButton();
	DWORD GetBtnStyle();
	DWORD SetBtnStyle(DWORD dwStyle);

	CBitmapHandle SetBkgndBitmap(HBITMAP bmp);
	CBitmapHandle GetBkgndBitmap();

	void SetBtnImagelist(HIMAGELIST img);
	CImageList GetBtnImagelist();

	void SetTooltipText(LPCTSTR strText);

	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);

	void SetTextFont(HFONT hFont) { m_hFont = hFont; }
	HFONT GetTextFont();
	
	CSize GetIdealSize() const;

	VOID SetTextColor(COLORREF clr) { m_clrText = clr; }

protected:
	LRESULT OnSetCheck(UINT, WPARAM wParam, LPARAM, BOOL& bHandled);
	LRESULT OnGetCheck(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	
protected:
	void _DrawBkgnd(CDCHandle dc, CRect* lpRect);
	void _AdjustSize();
	void _Init();
	void _Release();

	CRect _GetTextRect();
	CRect _GetBitmapRect();
	CSize _GetTextSize();
	CSize _GetBitmapSize();

protected:
	INT m_bChecked;
	CBitmapHandle m_bmpBkgnd;
	CImageList m_imgBtn;
	DWORD m_dwBtnStyle;
	CToolTipCtrl m_tip;
	CString m_strTooltip;
	CFontHandle m_hFont;
	COLORREF m_clrText;
};

enum TOOL_BAR_ITEM_TYPE
{
	BUTTON_TYPE = 0,	//button
	SEP_TYPE,			//separator
	CONTROL_TYPE		//control
};

class CTsToolItem: public CTsButton
{
public:
	CTsToolItem(INT nID, DWORD dwType) { m_dwType = dwType;  m_nID = nID; m_bVisible = TRUE ;}

	BOOL IsSeperator() const { return m_dwType == SEP_TYPE; }
	BOOL IsButton() const { return m_dwType == BUTTON_TYPE; }
	BOOL IsControl() const { return m_dwType == CONTROL_TYPE; }
	
	INT GetID() const { return m_nID; }
	VOID SetID(INT nID) { m_nID = nID; }
	
	CSize GetIdealSize() const;

	BOOL IsVisible() const { return m_bVisible; }

private:
	friend class CTsToolBar;
	VOID SetVisible(BOOL bVisible);
	CTsToolItem();

private:
	DWORD m_dwType;
	INT m_nID;
	BOOL m_bVisible;
};


class CTsToolBar: public CWindowImpl<CTsToolBar, CWindow>,
				  public CHYBkgndDrawMap<CTsToolBar>
{
public:
	DECLARE_WND_CLASS(_T("TsToolBar"))

	BEGIN_MSG_MAP(CTsToolBar)
		CHAIN_MSG_MAP(CHYBkgndDrawMap<CTsToolBar>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_HY_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)

		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()	
		
	~CTsToolBar();

	VOID AddButton(INT nID, CBitmapHandle bmpBkgnd, CImageList imgBtn, LPCTSTR lpszTooltip = NULL, BOOL bVisible = TRUE);
	VOID AddSeperator(CBitmapHandle bmpBkgnd, BOOL bVisible = TRUE);
	VOID AddControl(INT nID, HWND hWnd, BOOL bVisible = TRUE);

	BOOL ShowItemByID(INT nID, BOOL bShow, BOOL bLayout = TRUE);
	BOOL ShowItemByIndex(INT nIndex, BOOL bShow, BOOL bLayout = TRUE); 

	INT GetItemCount() const;
	CTsToolItem* GetItemByIndex(int nIndex);
	CTsToolItem* GetItemByID(INT nId);

	VOID ResetAllItem();
	VOID ResetItemByIDRange(INT nIDStart, INT nIDEnd);

	BOOL SetBtnChecked(DWORD dwID, BOOL bSet);

	CSize GetIdealSize() const;
	
protected:
	LRESULT OnCreate(UINT, WPARAM, LPARAM,	BOOL&);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM,	BOOL&);
	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL&);

protected:
	enum
	{
		TS_TOOL_GAP = 2
	};

	VOID ClearAll();
	BOOL CheckIsItemExist(INT nId);
	
	VOID AdjustWindow();
	
private:
	std::vector<CTsToolItem*> m_arTools;
};