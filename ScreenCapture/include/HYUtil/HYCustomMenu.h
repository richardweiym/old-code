#pragma  once 

#include <map>
#include "HYUtilBase.h"
#include "atlmisc.h"
#include "atlframe.h"
#include "HYUIBase.h"

typedef struct tagMenuCustomDrawInfo
{
	HDC dc;
	CRect rtItem;
	BOOL bSelected;
	BOOL bGrayed;
	BOOL bChecked;
	BOOL bSubMenu;
	BOOL bRadio;
	BOOL bSeperator;
	LPCTSTR lpszText;
	HBITMAP bmpIcon;
}
MenuCustomDrawInfo, *LPMenuCustomDrawInfo;

typedef struct tagMenuCacheInfo
{
	TCHAR szMenuText[64];
	HBITMAP bmpIcon;
} 
MenuCacheInfo, *LPMenuCacheInfo;

class IHYMenuDrawer
{
public:
	IHYMenuDrawer()
	{
		InitMenuDrawer();
	}
	
	virtual ~IHYMenuDrawer()
	{
		
	}
	
	virtual INT GetSeperatorHeight() const
	{
		return 5;
	}
	
	virtual CSize GetMenuItemSize(LPCTSTR lpszMenuText, HBITMAP hBmpIcon)
	{
		CSize sizeIcon = GetMenuIconRectSize();
		CSize sizeArrow = GetMenuArrowRectSizeForSubMenu();
		CSize sizeText = _GetMenuTextSize(lpszMenuText);
		
		int nHeight = max(max(sizeIcon.cy, sizeText.cy), sizeArrow.cy);
		int nWidth = sizeIcon.cx + sizeArrow.cx + sizeText.cx + /*2 **/ GetMenuTextIconGap();
		
		return CSize(nWidth, nHeight);
	}
	
	virtual VOID DrawMenuItem(LPMenuCustomDrawInfo lpItem)
	{
		DrawMenuBkgnd(lpItem);
		DrawMenuIcon(lpItem);
		DrawMenuText(lpItem);
		DrawMenuArrowForSubMenu(lpItem);
	}
	
	virtual VOID DrawMenuBkgnd(LPMenuCustomDrawInfo pInfo)
	{
		CRect& rtItem = pInfo->rtItem;
		CDCHandle dc = pInfo->dc;
		if(pInfo->bSelected && !pInfo->bGrayed)
		{
			CPen penNew = CreatePen(PS_SOLID, GetMenuFrameWidth(), GetMenuFrameColor());
			CBrush brushNew = CreateSolidBrush(GetMenuSelectBKColor());
			CPenHandle penOld = dc.SelectPen(penNew);
			CBrushHandle brushOld = dc.SelectBrush(brushNew);
			dc.Rectangle(&rtItem);
			dc.SelectPen(penOld); 
			dc.SelectBrush(brushOld);
		}
		else
		{
			CSize sizeIcon = GetMenuIconRectSize();
			CRect rtIcon = rtItem;
			rtIcon.right = rtIcon.left + sizeIcon.cx;
			dc.FillSolidRect(&rtIcon, GetMenuIconBKColor(pInfo->bGrayed));
			
			CRect rtText = rtItem;
			rtText.InflateRect(-rtIcon.Width(), 0, 0, 0);
			dc.FillSolidRect(&rtText, GetMenuTextBKColor(pInfo->bGrayed));
		}
	}
	
	virtual VOID DrawMenuIcon(LPMenuCustomDrawInfo pInfo)
	{
		CRect& rtItem = pInfo->rtItem;
		CDCHandle dc = pInfo->dc; 
		
		CSize sizeIcon = GetMenuIconRectSize();
		CRect rtIcon = rtItem;
		rtIcon.right = rtIcon.left + sizeIcon.cx;
		
		CBitmapHandle bmpDraw = NULL;
		if(pInfo->bChecked)
		{
			CBitmapHandle bmpChecked = GetMenuCheckedBMP();
			if(!bmpChecked.IsNull())
			{
				bmpDraw = bmpChecked;	
			}
		}
		else if(pInfo->bRadio)
		{
			CBitmapHandle bmpRadio = GetMenuRadioBMP();
			_ASSERTE(!bmpRadio.IsNull());
			if(!bmpRadio.IsNull())
			{
				bmpDraw = bmpRadio;	
			}
		}
		else
		{
			CBitmapHandle bmpIcon = pInfo->bmpIcon;
			if(!bmpIcon.IsNull())
			{
				bmpDraw = bmpIcon;
			}
		}

		if(!bmpDraw.IsNull())
		{
			CHYUICommonFunctionWrapper::DrawBmpInCenter(dc, &rtIcon, bmpDraw, TRUE);	
		}
	}
	
	virtual VOID DrawMenuText(LPMenuCustomDrawInfo pInfo)
	{
		CRect& rtItem = pInfo->rtItem;
		CDCHandle dc = pInfo->dc;
		CSize sizeIcon = GetMenuIconRectSize();
		CRect rtIcon = rtItem;
		rtIcon.right = rtIcon.left + sizeIcon.cx;
		
		CSize sizeArrow = GetMenuArrowRectSizeForSubMenu();
		CRect rtText = rtItem;
		rtText.InflateRect(-rtIcon.Width(), 0, 0, 0);
		rtText.left += GetMenuTextIconGap();
		
		dc.SetBkMode(TRANSPARENT);
		if(!pInfo->bSeperator)
		{
			rtText.right -= sizeArrow.cx;
			//rtText.right -= GetMenuTextIconGap();

			LPCTSTR lpszText = pInfo->lpszText;
			int nTabPos = -1;
			int nLen = lstrlen(lpszText);
			for(int i=0; i<nLen; ++i)
			{
				if(lpszText[i] == _T('\t'))
				{
					nTabPos = i;
					break;
				}
			}

			CFontHandle fontOld = dc.SelectFont(_GetMenuTextFont());
			dc.SetTextColor(GetMenuTextColor(pInfo->bGrayed));
			if(nTabPos == -1)
			{
				dc.DrawText(lpszText, nLen, &rtText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				dc.DrawText(lpszText, nTabPos + 1, &rtText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				dc.DrawText(lpszText + nTabPos + 1, (nLen - nTabPos - 1), &rtText, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
			}
			dc.SelectFont(fontOld);
		}
		else
		{
			CPen penNew = CreatePen(PS_SOLID, 1, GetMenuSeperatorColor());
			CPenHandle penOld = dc.SelectPen(penNew);
			int nY = (rtText.top + rtText.bottom ) / 2;
			dc.MoveTo(rtText.left, nY);
			dc.LineTo(rtText.right, nY);
			dc.SelectPen(penOld);
		}
	}
	
	virtual VOID DrawMenuArrowForSubMenu(LPMenuCustomDrawInfo pInfo)
	{
		
	}
	
	virtual COLORREF GetMenuSeperatorColor()
	{
		return RGB(155, 155, 155);
	}
	virtual COLORREF GetMenuTextColor(BOOL bGrayed)
	{
		if(!bGrayed)
		{
			return RGB(0, 0, 0);
		}
		else
		{
			return RGB(177, 184, 170);
		}
	}
	
	virtual HBITMAP GetMenuRadioBMP()
	{
		return NULL;
	}
	
	virtual HBITMAP GetMenuCheckedBMP()
	{
		return NULL;
	}
	
	virtual COLORREF GetMenuTextBKColor(BOOL bGrayed)
	{
		return RGB(238, 244, 249);
	}
	
	virtual COLORREF GetMenuIconBKColor(BOOL bGrayed) 
	{
		return RGB(213, 226, 236);
	}
	
	virtual INT GetMenuFrameWidth()
	{
		return 1;
	}
	
	virtual COLORREF GetMenuFrameColor()
	{
		return RGB(49, 106, 197);
	}
	
	virtual COLORREF GetMenuSelectBKColor()
	{
		return RGB(160, 194, 202);
	}
	
	virtual int GetMenuTextIconGap()
	{
		return 10;
	}
	virtual CSize GetMenuIconRectSize()
	{
		return CSize(25, 25);
	}
	
	virtual CSize GetMenuArrowRectSizeForSubMenu()
	{
		return CSize(25, 25);
	}
	
protected:
	CFontHandle _GetMenuTextFont()
	{
		return m_fntMenuText;
	}
	
	CSize _GetMenuTextSize(LPCTSTR lpszText)
	{
		CRect rtDraw(0, 0, 0, 0);
		CClientDC dc(NULL);
		CFontHandle fntOld = dc.SelectFont(_GetMenuTextFont());
		dc.DrawText(lpszText, lstrlen(lpszText), &rtDraw, DT_CALCRECT | DT_LEFT | DT_SINGLELINE);
		dc.SelectFont(fntOld);
		
		return CSize(rtDraw.Width(), rtDraw.Height());
	}
	
    VOID InitMenuDrawer()
	{
		if(m_fntMenuText.IsNull())
		{
			NONCLIENTMETRICS info = { 0 };
			info.cbSize = sizeof(info);
			BOOL bRet = ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			m_fntMenuText = ::CreateFontIndirect(&info.lfMenuFont);
		}
	}
	
protected:
	CFont m_fntMenuText;
};


#ifndef MIIM_STRING
#define MIIM_STRING      0x00000040
#define MIIM_BITMAP      0x00000080
#define MIIM_FTYPE       0x00000100
#endif //MIIM_STRING

template<typename T>
class CHYCustomMenuMap: public COwnerDraw<T>
{
public:
	BEGIN_MSG_MAP(CHYCustomMenuMap)
		//CHAIN_MSG_MAP(COwnerDraw<T>)
		MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
		MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
	END_MSG_MAP()
		
	LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{

		bHandled = FALSE;
		DrawItem((LPDRAWITEMSTRUCT)lParam);

		return (LRESULT)TRUE;
	}

	LRESULT OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		MeasureItem((LPMEASUREITEMSTRUCT)lParam);
		
		return (LRESULT)TRUE;
	}

	CHYCustomMenuMap()
	{

	}
	
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		//SetMsgHandled(FALSE);
		
		_ASSERTE(lpDrawItemStruct != NULL);
		if(lpDrawItemStruct->CtlType != ODT_MENU) return;

		IHYMenuDrawer* pMenuDrawer = CHYCustomMenuMgr::GetMenuDrawer();
		_ASSERTE(pMenuDrawer != NULL); 
		if(pMenuDrawer == NULL) return;
		
		CRect rtItem = lpDrawItemStruct->rcItem;
		INT nMenuItemID = lpDrawItemStruct->itemID;
		BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED);
		BOOL bGrayed = (lpDrawItemStruct->itemState & ODS_GRAYED);
		BOOL bChecked = (lpDrawItemStruct->itemState & ODS_CHECKED);
		HMENU hMenu = (HMENU)(lpDrawItemStruct->hwndItem);
		CDCHandle dc = lpDrawItemStruct->hDC;
		
		
		MENUITEMINFO mi = {0};
		mi.cbSize = sizeof(mi);
		mi.fMask = MIIM_FTYPE  | MIIM_SUBMENU ;
		GetMenuItemInfo(hMenu, nMenuItemID, MF_BYCOMMAND, &mi);
		BOOL bRadio = (mi.fType & MFT_RADIOCHECK);
		BOOL bSeperator = (mi.fType & MFT_SEPARATOR);
		BOOL bSubMenu = (mi.hSubMenu != NULL);
		

		HBITMAP menuIcon = _GetMenuIconByID(nMenuItemID);
		
		MenuCustomDrawInfo mcdi;
		ZeroMemory(&mcdi, sizeof(mcdi));
		mcdi.bChecked = bChecked;
		mcdi.bGrayed = bGrayed;
		mcdi.bmpIcon = menuIcon;
		mcdi.bRadio = bRadio;
		mcdi.bSelected = bSelected;
		mcdi.bSeperator = bSeperator;
		mcdi.bSubMenu = bSubMenu;
		mcdi.dc = dc;
		mcdi.lpszText = _GetMenuTextByID(nMenuItemID);
		mcdi.rtItem = rtItem;
		
		pMenuDrawer->DrawMenuItem(&mcdi); 

	//	::ExcludeClipRect(dc, rtItem.left, rtItem.top, rtItem.right, rtItem.bottom);

	}
	
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	{
		//SetMsgHandled(FALSE);

		_ASSERTE(lpMeasureItemStruct != NULL);
		if(lpMeasureItemStruct->CtlType != ODT_MENU) return;

		IHYMenuDrawer* pMenuDrawer = CHYCustomMenuMgr::GetMenuDrawer();
		_ASSERTE(pMenuDrawer != NULL); 
		if(pMenuDrawer == NULL) return;
		
		INT nMenuItemID = lpMeasureItemStruct->itemID;
		if( nMenuItemID == ID_SEPARATOR)
		{
			lpMeasureItemStruct->itemWidth = 0;
			lpMeasureItemStruct->itemHeight = pMenuDrawer->GetSeperatorHeight();
		}
		else
		{
			LPCTSTR lpszMenuText = _GetMenuTextByID(nMenuItemID);
			HBITMAP menuIcon = _GetMenuIconByID(nMenuItemID);
			
			CSize sizeMenu = pMenuDrawer->GetMenuItemSize(lpszMenuText, menuIcon);
			
			lpMeasureItemStruct->itemWidth = sizeMenu.cx;
			lpMeasureItemStruct->itemHeight = sizeMenu.cy;
		}
	}
	
	
protected:
	VOID InitMenuCache(HMENU hMenu)
	{
		T* pThis = static_cast<T*>(this);

		int nCount = GetMenuItemCount(hMenu);
		TCHAR szTextBuffer[512] = {0};
		MENUITEMINFO mi = {0};
		for(int i=0; i<nCount; ++i)
		{
			ZeroMemory(&mi, sizeof(mi));
			ZeroMemory(szTextBuffer, sizeof(szTextBuffer));
			mi.cbSize = sizeof(mi);
			mi.dwTypeData = szTextBuffer;
			mi.cch = sizeof(szTextBuffer) / sizeof(TCHAR) - 1;
			mi.fMask = MIIM_STRING | MIIM_ID | MIIM_SUBMENU | MIIM_FTYPE;

			GetMenuItemInfo(hMenu, i, MF_BYPOSITION, &mi);
			
			if(mi.wID != ID_SEPARATOR)
			{
				AddMenuCacheItem((INT)mi.wID, szTextBuffer, pThis->GetMenuIconByID(mi.wID));

				if (mi.hSubMenu != NULL)
				{
					InitMenuCache(mi.hSubMenu);
				}
			}
			
			mi.fType |= MFT_OWNERDRAW;
			mi.fMask = MIIM_FTYPE;
			SetMenuItemInfo(hMenu, i, MF_BYPOSITION, &mi);
		}
	}

	VOID ClearMenuCache()
	{
		CConItrType itr = m_mapMenuCache.begin();
		for(; itr != m_mapMenuCache.end(); ++itr)
		{
			delete itr->second;
		}
		
		m_mapMenuCache.clear();
	}


	//must overload
	HBITMAP GetMenuIconByID(INT nID)
	{
		return NULL;
	}
	
	VOID AddMenuCacheItem(INT nMenuID, LPCTSTR lpszMenuText, HBITMAP bmpIcon = NULL)
	{
		CConItrType itr = m_mapMenuCache.find(nMenuID);
		if(itr != m_mapMenuCache.end())
		{
			_ASSERTE(FALSE);
			return;
		}
		
		LPMenuCacheInfo pInfo = new MenuCacheInfo;
		if(pInfo != NULL)
		{
			if(lpszMenuText != NULL)
			{
				lstrcpyn(pInfo->szMenuText, lpszMenuText, sizeof(pInfo->szMenuText) / sizeof(TCHAR) - 1);
			}
			else
			{
				pInfo->szMenuText[0] = _T('\0');
			}
			pInfo->bmpIcon = bmpIcon;
			m_mapMenuCache.insert(std::make_pair<INT, LPMenuCacheInfo>(nMenuID, pInfo));
		}
	}
	
	LPCTSTR _GetMenuTextByID(INT nMenuID)
	{
		CConItrType itr = m_mapMenuCache.find(nMenuID);
		if(itr != m_mapMenuCache.end())
		{
			return itr->second->szMenuText;
		}
		
		return _T("");
	}
	
	HBITMAP _GetMenuIconByID(INT nMenuID)
	{
		CConItrType itr = m_mapMenuCache.find(nMenuID);
		if(itr != m_mapMenuCache.end())
		{
			return itr->second->bmpIcon;
		}
		
		return NULL;
	}
	
protected:
	typedef std::map<INT, LPMenuCacheInfo> CConType;
	typedef CConType::iterator CConItrType;
	CConType m_mapMenuCache;
};



class CHYCustomMenuMgr
{
public:
	static BOOL InitializeHook();
	static VOID UninitializeHook();

	static VOID SetMenuDrawer(IHYMenuDrawer* pMenuDrawer);
	static IHYMenuDrawer* GetMenuDrawer();

protected:
	static LRESULT CALLBACK CBTProc(INT nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsgType, WPARAM wParam, LPARAM lParam);
	static LRESULT CallOldMenuProc(HWND hWnd, UINT nMsgType, WPARAM wParam, LPARAM lParam );
	static BOOL IsTargetMenuWindow(HWND hWnd);
	static BOOL AddMenuTrack(HWND hWnd);
	static BOOL RemoveMenuTrack(HWND hWnd);
	static VOID RestoreAllMenuTrack();

	static VOID DrawBorder(CDCHandle dc, HWND hWnd);

private:
	static HHOOK s_hkMenu;
	static IHYMenuDrawer* s_pMenuDrawer;

	typedef std::map<HWND, DWORD> CConType;
	typedef CConType::iterator CConItrType;	

	static CConType s_mapMenuProc;
};

