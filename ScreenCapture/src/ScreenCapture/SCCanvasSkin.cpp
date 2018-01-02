#include "SCCanvasSkin.h"
#include "SCRes.h"
#include "HYUIBase.h"
#include "SCIDDef.h"
#include "SCCanvasConfig.h"

CSCBlueMenuDrawer::CSCBlueMenuDrawer()
{

}

HBITMAP CSCBlueMenuDrawer::GetMenuCheckedBMP()
{
	if(m_bmpMenuCheck.IsNull())
	{
		m_bmpMenuCheck = GetCanvasSkin()->GetResBMP(IDB_MENU_CHECK_BOX_1);
	}
	
	return m_bmpMenuCheck;
}

HBITMAP CSCPurpleMenuDrawer::GetMenuCheckedBMP()
{
	if(m_bmpMenuCheck.IsNull())
	{
		m_bmpMenuCheck = GetCanvasSkin()->GetResBMP(IDB_MENU_CHECK_BOX_2);
	}

	return m_bmpMenuCheck;
}

DWORD CSCCanvasSkin::GetResIDByName(LPCTSTR lpszName) const
{
	DWORD dwID(0);
	
	
	static  NameIDPair commonPair[] =
	{
		_T("skin_1"), IDS_SKIN_NAME_1,
		_T("skin_2"), IDS_SKIN_NAME_2,

		_T("lan_1"), IDS_LAN_NAME_1,
		_T("lan_2"), IDS_LAN_NAME_2,

		_T("default_font_name"), IDS_DEFAULT_TEXT_FONT_NAME,
		_T("tool_extend_menu_tip"), IDS_TOOL_EXTEND_MENU_TIP,
		_T("tool_ellipse_tip"), IDS_TOOL_ELLIPSE_TIP,
		_T("tool_rect_tip"), IDS_TOOL_RECT_TIP,
		_T("tool_select_tip"), IDS_TOOL_SELECT_TIP,
		_T("tool_win_select_tip"), IDS_TOOL_WIN_SELECT_TIP,
		_T("tool_line_tip"), IDS_TOOL_LINE_TIP,
		_T("tool_pen_tip"), IDS_TOOL_PEN_TIP,
		_T("tool_text_tip"), IDS_TOOL_TEXT_TIP,
		_T("tool_eraser_tip"), IDS_TOOL_ERASER_TIP,
		_T("tool_arrow_tip"), IDS_TOOL_ARROW_TIP,
		_T("start_btn_text"), IDS_START_CAPTURE,
		_T("close_btn_tip"), IDS_CLOSE_BTN_TIP,

		_T("default_cursor"), IDC_DEFAULT_CURSOR,
		_T("pen_cursor"), IDC_PEN_CURSOR,
		_T("eraser_cursor"), IDC_ERASER_CURSOR,
		_T("select_cursor"), IDC_SELECT_CURSOR,

		_T("about_wave_bk"), IDB_ABOUT_WAVE_BK

	};

	static  NameIDPair Skin1Pair[] =
	{
		_T("cover_shadow"), IDB_CANVAS_SHADOW_1,
		_T("water_mark"), IDB_WATER_MARK_1,
		
		_T("float_tool_bk"), IDB_FLOAT_TOOL_BK_1,
		_T("sub_float_tool_bk"), IDB_SUB_FLOAT_TOOL_BK_1,
		_T("tool_btn_bk"), IDB_TOOL_BTN_BK,
		_T("tool_seperator_bk"), IDB_TOOL_SEP_BK,
		_T("tool_btn_select"), IDB_TOOL_BTN_SELECT,
		_T("tool_btn_win_select"), IDB_TOOL_BTN_WIN_SELECT1,
		_T("tool_btn_rect"), IDB_TOOL_BTN_RECT_1,
		_T("tool_btn_ellipse"), IDB_TOOL_BTN_ELLIPSE_1,
		_T("tool_btn_line"), IDB_TOOL_BTN_LINE_1,
		_T("tool_btn_pen"), IDB_TOOL_BTN_PEN_1,
		_T("tool_btn_text"), IDB_TOOL_BTN_TEXT_1,
		_T("tool_btn_eraser"), IDB_TOOL_BTN_ERASER_1,
		_T("tool_btn_arrow"), IDB_TOOL_BTN_ARROW_1,

		_T("float_drag_bk"), IDB_FLOAT_DRAG_BK_1,
		_T("float_menu_bk"), IDB_FLOAT_MENU_BK_1,
		_T("float_menu_img"), IDB_FLOAT_MENU_1,
		_T("float_drag_img"), IDB_FLOAT_DRAG_1,

		_T("tool_btn_Small_Pen"), IDB_BTN_PEN_SMALL1,
		_T("tool_btn_Middle_Pen"), IDB_BTN_PEN_MIDDLE1,
		_T("tool_btn_Big_Pen"), IDB_BTN_PEN_BIG1,
		_T("tool_btn_Pen_BK"), IDB_BTN_PEN_BK1,
		_T("tool_btn_font_bold"), IDB_BTN_FONT_BOLD_1,

		_T("start_btn_img"), IDB_START_BTN_1,
		_T("start_btn_bk"), IDB_START_BTN_BK_1,

		_T("close_btn_bk"), IDB_CLOSE_BTN_BK_1,
		_T("close_btn_img"), IDB_CLOSE_BTN_1,

		_T("info_bubble_bk"), IDB_INFO_BUBBLE_BK_1,
		_T("dlg_title_bk"), IDB_TITLE_BK_1,

		_T("dlg_btn_bk"), IDB_DLG_BTN_BK,
		_T("dlg_btn_img"), IDB_DLG_BTN_IMG
		
	};
	
	static  NameIDPair Skin2Pair[] =
	{
		_T("cover_shadow"), IDB_CANVAS_SHADOW_2,
		_T("water_mark"), IDB_WATER_MARK_2,

		_T("float_tool_bk"), IDB_FLOAT_TOOL_BK_2,
		_T("sub_float_tool_bk"), IDB_SUB_FLOAT_TOOL_BK_2,
		_T("tool_btn_bk"), IDB_TOOL_BTN_BK_2,
		_T("tool_seperator_bk"), IDB_TOOL_SEP_BK_2,
		_T("tool_btn_select"), IDB_TOOL_BTN_SELECT_2,
		_T("tool_btn_win_select"), IDB_TOOL_BTN_WIN_SELECT2,
		_T("tool_btn_rect"), IDB_TOOL_BTN_RECT_2,
		_T("tool_btn_ellipse"), IDB_TOOL_BTN_ELLIPSE_2,
		_T("tool_btn_line"), IDB_TOOL_BTN_LINE_2,
		_T("tool_btn_pen"), IDB_TOOL_BTN_PEN_2,
		_T("tool_btn_text"), IDB_TOOL_BTN_TEXT_2,
		_T("tool_btn_eraser"), IDB_TOOL_BTN_ERASER_2,
		_T("tool_btn_arrow"), IDB_TOOL_BTN_ARROW_2,

		_T("float_drag_bk"), IDB_FLOAT_DRAG_BK_2,
		_T("float_menu_bk"), IDB_FLOAT_MENU_BK_2,
		_T("float_menu_img"), IDB_FLOAT_MENU_2,
		_T("float_drag_img"), IDB_FLOAT_DRAG_2,

		_T("tool_btn_Small_Pen"), IDB_BTN_PEN_SMALL_2,
		_T("tool_btn_Middle_Pen"), IDB_BTN_PEN_MIDDLE_2,
		_T("tool_btn_Big_Pen"), IDB_BTN_PEN_BIG_2,
		_T("tool_btn_Pen_BK"), IDB_BTN_PEN_BK_2,
		_T("tool_btn_font_bold"), IDB_BTN_FONT_BOLD_2,

		_T("start_btn_img"), IDB_START_BTN_1,
		_T("start_btn_bk"), IDB_START_BTN_BK_1,

		_T("close_btn_bk"), IDB_CLOSE_BTN_BK_1,
		_T("close_btn_img"), IDB_CLOSE_BTN_1,

		_T("info_bubble_bk"), IDB_INFO_BUBBLE_BK_2,
		_T("dlg_title_bk"), IDB_TITLE_BK_2,

		_T("dlg_btn_bk"), IDB_DLG_BTN_BK,
		_T("dlg_btn_img"), IDB_DLG_BTN_IMG
	};

	dwID = GetResIDFromTable(lpszName, commonPair, sizeof(commonPair) / sizeof(commonPair[0]) );
	if(dwID != 0) return dwID; 

	if(m_eSkinType == SKIN_TYPE_1)
	{
		dwID = GetResIDFromTable(lpszName, Skin1Pair, sizeof(Skin1Pair) / sizeof(Skin1Pair[0]) );
	}
	else if(m_eSkinType == SKIN_TYPE_2)
	{
		dwID = GetResIDFromTable(lpszName, Skin2Pair, sizeof(Skin2Pair) / sizeof(Skin2Pair[0]) );
	}

	
	return dwID;
}


DWORD CSCCanvasSkin::GetResIDFromTable(LPCTSTR lpszName, LPNameIDPair pTable, INT nCount) const
{
	_ASSERTE(pTable != NULL && lpszName != NULL);

	DWORD dwRet(0);
	for(int i=0; i<nCount; ++i)
	{
		LPNameIDPair pPair = &pTable[i];
		if(lstrcmpi(pPair->szResName, lpszName) == 0)
		{
			dwRet = pPair->dwResID;
		}
	}

	return dwRet;
}

CSCCanvasSkin::CSCCanvasSkin()
{
	m_eSkinType = SKIN_TYPE_1;
	m_eLanType = LAN_TYPE_SIMPLE_CH;
}

CSCCanvasSkin::~CSCCanvasSkin()
{

}
BOOL CSCCanvasSkin::ResetResInstance()
{
	FreeResInstance();

	LPCTSTR lpszDllName = GetResDllNameByLanType(m_eLanType);
	if(lpszDllName == NULL) return FALSE;

	TCHAR szResPath[MAX_PATH] = {0};
	if(CHYCommonFunctionWrapper::GetCurrentFilePath(szResPath, sizeof(szResPath) / sizeof(TCHAR) - 1, lpszDllName))
	{
		_ASSERTE(m_resLoader.GetResInstance() == NULL);
		HINSTANCE hRes = LoadLibrary(szResPath);
		_ASSERTE(hRes != NULL);
		m_resLoader.SetResInstance(hRes);

		_Module.m_hInstResource = hRes;
	}

	_ASSERTE(m_resLoader.GetResInstance() != NULL);

	return (m_resLoader.GetResInstance() != NULL);
}

VOID CSCCanvasSkin::FreeResInstance()
{
	HINSTANCE hRes = m_resLoader.GetResInstance();
	if(hRes != NULL)
	{
		FreeLibrary(hRes);
		m_resLoader.SetResInstance(NULL);
	}
}
BOOL CSCCanvasSkin::Initialize()
{
	CRefPtr<CSCCanvasConfig> pConfig = GetCanvasConfig();

	INT nLanType = pConfig->GetLanType();
	if(nLanType > LAN_TYPE_NONE && nLanType <= LAN_TYPE_COUNT)
	{
		m_eLanType = (CANVAS_LAN_TYPE)nLanType;
	}

	INT nSkinType = pConfig->GetSkinType();
	if(nSkinType > SKIN_TYPE_NONE && nSkinType <= SKIN_TYPE_COUNT)
	{
		m_eSkinType = (CANVAS_SKIN_TYPE)nSkinType;
	}

	return ResetResInstance();
}

void CSCCanvasSkin::Uninitialize()
{
	FreeResInstance();

	CRefPtr<CSCCanvasConfig> pConfig = GetCanvasConfig();
	_ASSERTE(!pConfig.IsNull());
	
	if(m_eLanType > LAN_TYPE_NONE && m_eLanType <= LAN_TYPE_COUNT)
	{
		pConfig->SetLanType((INT)m_eLanType);
	}

	if(m_eSkinType > SKIN_TYPE_NONE && m_eSkinType <= SKIN_TYPE_COUNT)
	{
		pConfig->SetSkinType(INT(m_eSkinType));
	}	
}

HINSTANCE CSCCanvasSkin::GetResInstance() const
{
	return m_resLoader.GetResInstance();
}

CString CSCCanvasSkin::GetSkinNameByIndex(INT nIndex)
{
	CString strRet;

	TCHAR szName[32] = {0};
	_sntprintf(szName, sizeof(szName) / sizeof(TCHAR) - 1, _T("skin_%d"), nIndex+1);
	DWORD dwResId = GetResIDByName(szName);

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}

CString CSCCanvasSkin::GetLanNameByIndex(INT nIndex)
{
	CString strRet;

	TCHAR szName[32] = {0};
	_sntprintf(szName, sizeof(szName) / sizeof(TCHAR) - 1, _T("lan_%d"), nIndex+1);
	DWORD dwResId = GetResIDByName(szName);

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}

VOID CSCCanvasSkin::SetLanType(DWORD dwType)
{
	if(dwType != m_eLanType)
	{
		_ASSERTE(dwType > LAN_TYPE_NONE && dwType <= LAN_TYPE_COUNT);

		m_eLanType = (CANVAS_LAN_TYPE)dwType;
		
		if(!ResetResInstance())
		{
			::MessageBox(GetActiveWindow(), _T("Load resource failed!"), _T("ERROR"), MB_OK);
			exit(0);
		}

		NotifyObservers(UI_WPARAM_LAN_CHANGED, (LPARAM)dwType);
	}
}

VOID CSCCanvasSkin::SetSkinType(DWORD eType)
{
	if(eType != m_eSkinType)
	{
		_ASSERTE(eType > SKIN_TYPE_NONE  && eType <= SKIN_TYPE_COUNT);

		m_eSkinType = (CANVAS_SKIN_TYPE)eType;

		NotifyObservers(UI_WPARAM_SKIN_CHANGED, (LPARAM)eType);
	}
}

CBitmapHandle CSCCanvasSkin::GetWaterMark() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("water_mark"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}

	return hBmpRet;
}

CBitmapHandle CSCCanvasSkin::GetCoverShadow() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("cover_shadow"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}

	return hBmpRet;
}

CBitmapHandle CSCCanvasSkin::GetToolBtnBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}

CImageList CSCCanvasSkin::GetToolSelectBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_select"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}

CBitmapHandle CSCCanvasSkin::GetToolSeperatorBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_seperator_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}

CBitmapHandle CSCCanvasSkin::GetFloatDragBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("float_drag_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}

CBitmapHandle CSCCanvasSkin::GetFloatMenuBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("float_menu_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}

CImageList CSCCanvasSkin::GetFloatMenuImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("float_menu_img"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}

CImageList CSCCanvasSkin::GetFloatDragImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("float_drag_img"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}


CImageList CSCCanvasSkin::GetToolRectBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_rect"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}
CImageList CSCCanvasSkin::GetToolLineBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_line"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	
	
	return imgRet;
}
CImageList CSCCanvasSkin::GetToolArrowBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_arrow"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	
	
	return imgRet;
}
CImageList CSCCanvasSkin::GetToolPenBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_pen"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}
CImageList CSCCanvasSkin::GetToolTextBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_text"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}


CImageList CSCCanvasSkin::GetToolEllipseBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_ellipse"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}
CImageList CSCCanvasSkin::GetToolEraserBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_eraser"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}


CString CSCCanvasSkin::GetToolSelectTip() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("tool_select_tip"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}

CString CSCCanvasSkin::GetToolRectTip() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("tool_rect_tip"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}

CString CSCCanvasSkin::GetToolEllipseTip() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("tool_ellipse_tip"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}
CString CSCCanvasSkin::GetToolLineTip() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("tool_line_tip"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}
CString CSCCanvasSkin::GetToolPenTip() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("tool_pen_tip"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}
CString CSCCanvasSkin::GetDefaultFontName() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("default_font_name"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}
CString CSCCanvasSkin::GetToolTextTip() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("tool_text_tip"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}
CString CSCCanvasSkin::GetToolEraserTip() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("tool_eraser_tip"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}
CString CSCCanvasSkin::GetToolArrowTip() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("tool_arrow_tip"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}
CString CSCCanvasSkin::GetToolMenuTip() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("tool_extend_menu_tip"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}

CBitmapHandle CSCCanvasSkin::GetPenWidthBtnBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_Pen_BK"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}

CImageList CSCCanvasSkin::GetSmallPenBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_Small_Pen"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}

CImageList CSCCanvasSkin::GetBigPenBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_Big_Pen"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}

CImageList CSCCanvasSkin::GetMiddlePenBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_Middle_Pen"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}

CImageList CSCCanvasSkin::GetFontBoldBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_font_bold"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}

HCURSOR CSCCanvasSkin::GetDefaultCursor() const
{
	HCURSOR hRet = NULL;
	DWORD dwID = GetResIDByName(_T("default_cursor"));
	if(dwID != NULL)
	{
		hRet = m_resLoader.GetCursor(dwID);
	}

	return hRet;
}

HCURSOR CSCCanvasSkin::GetPenCursor() const
{
	HCURSOR hRet = NULL;
	DWORD dwID = GetResIDByName(_T("pen_cursor"));
	if(dwID != NULL)
	{
		hRet = m_resLoader.GetCursor(dwID);
	}

	return hRet;
}

HCURSOR CSCCanvasSkin::GetEraserCursor() const
{
	HCURSOR hRet = NULL;
	DWORD dwID = GetResIDByName(_T("eraser_cursor"));
	if(dwID != NULL)
	{
		hRet = m_resLoader.GetCursor(dwID);
	}

	return hRet;
}
HCURSOR CSCCanvasSkin::GetSelectCursor() const
{
	HCURSOR hRet = NULL;
	DWORD dwID = GetResIDByName(_T("select_cursor"));
	if(dwID != NULL)
	{
		hRet = m_resLoader.GetCursor(dwID);
	}

	return hRet;
}

CString CSCCanvasSkin::GetResString(DWORD dwID) const
{
	CString strRet;

	if(dwID != NULL)
	{
		m_resLoader.LoadString(dwID, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}

CBitmapHandle CSCCanvasSkin::GetResBMP(DWORD dwID) const
{
	if(dwID != 0)
	{
		return m_resLoader.LoadBitmap(dwID);
	}

	return NULL;
}

CImageList CSCCanvasSkin::GetStartBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("start_btn_img"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}


CBitmapHandle CSCCanvasSkin::GetStartBtnBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("start_btn_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}

CString CSCCanvasSkin::GetStartBtnText() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("start_btn_text"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}

CBitmapHandle CSCCanvasSkin::GetDlgBtnBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("dlg_btn_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}

CImageList CSCCanvasSkin::GetDlgBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("dlg_btn_img"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 6);
	}	

	return imgRet;
}

CImageList CSCCanvasSkin::GetCloseBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("close_btn_img"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}

CBitmapHandle CSCCanvasSkin::GetInforBubbleBk()	const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("info_bubble_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}

CBitmapHandle CSCCanvasSkin::GetAboutWaveBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("about_wave_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}

CBitmapHandle CSCCanvasSkin::GetDlgTitleBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("dlg_title_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}


CBitmapHandle CSCCanvasSkin::GetDlgClientBK() const
{
	return CBitmapHandle(NULL);
}

COLORREF CSCCanvasSkin::GetDlgClientColorBK() const
{
	if(m_eSkinType == SKIN_TYPE_1)
	{
		return RGB(211, 229, 246);
	}
	else if(m_eSkinType == SKIN_TYPE_2)
	{
		return RGB(214, 200, 225);
	}	
	else
	{
		_ASSERTE(FALSE);
		return RGB(211, 229, 246);
	}
}

COLORREF CSCCanvasSkin::GetDlgFrameClolor()
{
	IHYMenuDrawer* pMenuDraw = GetCurrentMenuDrawer();
	if(pMenuDraw != NULL)
	{
		return pMenuDraw->GetMenuFrameColor();
	}

	return RGB(0, 0, 0);
}


CBitmapHandle CSCCanvasSkin::GetCloseBtnBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("close_btn_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}

CString CSCCanvasSkin::GetCloseBtnTip() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("close_btn_tip"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}

CBitmapHandle CSCCanvasSkin::GetFloatToolBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("float_tool_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}

CBitmapHandle CSCCanvasSkin::GetSubFloatToolBK() const
{
	CBitmapHandle hBmpRet = NULL;
	DWORD dwID = GetResIDByName(_T("sub_float_tool_bk"));
	if(dwID != NULL)
	{
		hBmpRet = m_resLoader.LoadBitmap(dwID);
	}	
	
	return hBmpRet;
}
COLORREF CSCCanvasSkin::GetCanvasShadowColor() const
{
	if(m_eSkinType == SKIN_TYPE_1)
	{
		return RGB(0, 0, 255);
	}
	else if(m_eSkinType == SKIN_TYPE_2)
	{
		return RGB(255, 0, 0);
	}
	else
	{
		_ASSERTE(FALSE);
		return RGB(0, 0, 255);
	}
}
IHYMenuDrawer* CSCCanvasSkin::GetCurrentMenuDrawer()
{
	if(m_eSkinType == SKIN_TYPE_1)
	{
		return &m_menuDrawer1;
	}
	else if(m_eSkinType == SKIN_TYPE_2)
	{
		return &m_menuDrawer2;
	}
	else
	{
		_ASSERTE(FALSE);
		return NULL;
	}
}
COLORREF CSCCanvasSkin::GetMainToolColor() const
{
	if(m_eSkinType == SKIN_TYPE_1)
	{
		return RGB(223,233,249);
	}
	else if(m_eSkinType == SKIN_TYPE_2)
	{
		return RGB(186,156,194);
	}
	else
	{
		_ASSERTE(FALSE);
		return RGB(223,233,249);
	}

}
CImageList CSCCanvasSkin::GetToolWinSelectBtnImg() const
{
	CImageList imgRet = NULL;
	DWORD dwID = GetResIDByName(_T("tool_btn_win_select"));
	if(dwID != NULL)
	{
		imgRet = m_resLoader.LoadImageList(dwID, 5);
	}	

	return imgRet;
}

CString CSCCanvasSkin::GetToolWinSelectTip() const
{
	CString strRet;

	DWORD dwResId = GetResIDByName(_T("tool_win_select_tip"));

	if(dwResId != NULL)
	{
		m_resLoader.LoadString(dwResId, strRet.GetBuffer(64), 64);
		strRet.ReleaseBuffer();
	}

	return strRet;
}