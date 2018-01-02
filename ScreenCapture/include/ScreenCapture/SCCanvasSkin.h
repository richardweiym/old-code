#pragma  once 

#include "HYResMgr.h"
#include "HYAutoPtr.h"
#include "HYPattern.h"
#include "SCInstanceAPI.h"
#include "SCIDDef.h"
#include "HYCustomMenu.h"

class CSCBlueMenuDrawer: public IHYMenuDrawer
{
public:
	CSCBlueMenuDrawer();
	virtual HBITMAP GetMenuCheckedBMP();

	virtual int GetMenuTextIconGap()
	{
		return 6;
	}
	virtual CSize GetMenuIconRectSize()
	{
		return CSize(20, 20);
	}
	
	virtual CSize GetMenuArrowRectSizeForSubMenu()
	{
		return CSize(20, 20);
	}

protected:
	CBitmap m_bmpMenuCheck;
};

class CSCPurpleMenuDrawer: public IHYMenuDrawer
{
public:
	virtual HBITMAP GetMenuCheckedBMP();

	virtual COLORREF GetMenuSeperatorColor()
	{
		return RGB(81, 46, 100);
	}
	virtual COLORREF GetMenuTextColor(BOOL bGrayed)
	{
		if(!bGrayed)
		{
			return RGB(0, 0, 0);
		}
		else
		{
			return RGB(150, 154, 165);
		}
	}
	
	virtual COLORREF GetMenuTextBKColor(BOOL bGrayed)
	{
		return RGB(226, 210, 223);
	}
	
	virtual COLORREF GetMenuIconBKColor(BOOL bGrayed) 
	{
		return RGB(191, 156, 196);
	}
	
	virtual COLORREF GetMenuFrameColor()
	{
		return RGB(87, 48, 104);
	}
	
	virtual COLORREF GetMenuSelectBKColor()
	{
		return RGB(157, 105, 159);
	}
	
protected:
	CBitmap m_bmpMenuCheck;
};

class CSCCanvasSkin: public CRefCountBase,
					 public CHYDataModuleBase
{
public:
	CSCCanvasSkin();
	~CSCCanvasSkin();

	BOOL Initialize();
	VOID Uninitialize();

	HINSTANCE GetResInstance() const;

	INT GetSkinCount() const { return SKIN_TYPE_COUNT;}
	CString GetSkinNameByIndex(INT nIndex);
	VOID SetSkinType(DWORD eType);
	DWORD GetSkinType() const { return m_eSkinType; }


	INT GetLanCount() const { return LAN_TYPE_COUNT; }
	CString GetLanNameByIndex(INT nIndex);
	VOID SetLanType(DWORD dwType);
	DWORD GetLanType() const { return m_eLanType; }

	CBitmapHandle GetWaterMark() const;
	CBitmapHandle GetCoverShadow() const;
	COLORREF GetCanvasShadowColor() const;

	COLORREF GetMainToolColor() const;
	CBitmapHandle GetToolSeperatorBK() const;
	CBitmapHandle GetToolBtnBK() const;
	CBitmapHandle GetFloatToolBK() const;
	CBitmapHandle GetSubFloatToolBK() const;

	CImageList GetToolSelectBtnImg() const;
	CImageList GetToolWinSelectBtnImg() const;
	CImageList GetToolRectBtnImg() const;
	CImageList GetToolEllipseBtnImg() const;
	CImageList GetToolLineBtnImg() const;
	CImageList GetToolPenBtnImg() const;
	CImageList GetToolTextBtnImg() const;
	CImageList GetToolEraserBtnImg() const;
	CImageList GetToolArrowBtnImg() const;

	CString GetToolMenuTip() const;
	CString GetToolSelectTip() const;
	CString GetToolRectTip() const;
	CString GetToolEllipseTip() const;
	CString GetToolLineTip() const;
	CString GetToolPenTip() const;
	CString GetToolTextTip() const;
	CString GetToolEraserTip() const;
	CString GetToolArrowTip() const;
	CString GetToolWinSelectTip() const;

	CBitmapHandle GetFloatDragBK() const;
	CBitmapHandle GetFloatMenuBK() const;
	CImageList GetFloatDragImg() const;
	CImageList GetFloatMenuImg() const;

	CBitmapHandle GetPenWidthBtnBK() const;
	CImageList GetSmallPenBtnImg() const;
	CImageList GetMiddlePenBtnImg() const;
	CImageList GetBigPenBtnImg() const;
	CImageList GetFontBoldBtnImg() const;

	CString GetDefaultFontName() const;

	HCURSOR GetDefaultCursor() const;
	HCURSOR GetPenCursor() const;
	HCURSOR GetEraserCursor() const;
	HCURSOR GetSelectCursor() const;

	CString GetResString(DWORD dwID) const;
	CBitmapHandle GetResBMP(DWORD dwID) const;

	CBitmapHandle GetStartBtnBK() const;
	CImageList GetStartBtnImg() const;
	CString GetStartBtnText() const;

	CBitmapHandle GetDlgBtnBK() const;
	CImageList GetDlgBtnImg() const;

	CBitmapHandle GetCloseBtnBK() const;
	CImageList GetCloseBtnImg() const;
	CString GetCloseBtnTip() const;

	CBitmapHandle GetInforBubbleBk() const;

	CBitmapHandle GetDlgTitleBK() const;
	CBitmapHandle GetDlgClientBK() const;
	COLORREF GetDlgClientColorBK() const;
	COLORREF GetDlgFrameClolor() ;

	CBitmapHandle GetAboutWaveBK() const;
	
	IHYMenuDrawer* GetCurrentMenuDrawer();

protected:

	typedef	struct tagNameIDPair
	{
		TCHAR szResName[32];
		DWORD dwResID;
	}NameIDPair, *LPNameIDPair;
	
	DWORD GetResIDByName(LPCTSTR lpszName) const;
	DWORD GetResIDFromTable(LPCTSTR lpszName, LPNameIDPair pTable, INT nCount) const;

	BOOL ResetResInstance();
	VOID FreeResInstance();

private:
	CHYResLoader m_resLoader;
	CANVAS_SKIN_TYPE m_eSkinType;

	CSCBlueMenuDrawer m_menuDrawer1;
	CSCPurpleMenuDrawer m_menuDrawer2;

	CANVAS_LAN_TYPE m_eLanType;
};

template<typename T>
class CSCSkinObserver
{
public:
	BEGIN_MSG_MAP(CSCSkinObserver<T>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_HY_UI_DATA_CHANGED, OnUIDataChanged)
	END_MSG_MAP()
	

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
		T* pThis = static_cast<T*>(this);

		CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
		_ASSERTE(!pSkin.IsNull());
		pSkin->AddObserver(pThis->m_hWnd);
		return S_OK;
	}

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;
		T* pThis = static_cast<T*>(this);

		CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
		_ASSERTE(!pSkin.IsNull());
		pSkin->AddObserver(pThis->m_hWnd);
		return S_OK;
	}

	LRESULT OnDestroy(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		T* pThis = static_cast<T*>(this);

		CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
		_ASSERTE(!pSkin.IsNull());
		pSkin->RemoveObserver(pThis->m_hWnd);
		return S_OK;
	}

	LRESULT OnUIDataChanged(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pThis = static_cast<T*>(this);
		if(pThis->IsWindow() 
			&& (wParam == UI_WPARAM_SKIN_CHANGED || wParam == UI_WPARAM_LAN_CHANGED))
		{
			pThis->DoUIDataChanged(wParam, lParam);
			bHandled = TRUE;
		}

		return S_OK;
	}
};

