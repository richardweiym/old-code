#pragma  once 

#include "TsDrawBase.h"


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
	void SetBkgndBitmap(HBITMAP bmp);
	CBitmapHandle GetBkgndBitmap();
	void SetBtnImagelist(HIMAGELIST img);
	CImageList GetBtnImagelist();
	void SetTooltipText(LPCTSTR strText);
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	void SetTextFont(HFONT hFont) { m_hFont = hFont; }
	HFONT GetTextFont();
	
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
};