#include "SCIDDef.h"
#include "SCCanvasContext.h"
#include "SCCanvasDrawer.h"

using namespace std;

TOOL_ID_PAIR* GetToolBtnIDTable(int& nCount)
{
	static 	TOOL_ID_PAIR arTools[] = 
	{
		{ TOOL_TYPE_SELECT, SC_TOOL_SELECT_ID },
		{ TOOL_TYPE_WIN_SELECT, SC_TOOL_WIN_SELECT_ID },
		{ TOOL_TYPE_RECTANGLE, SC_TOOL_RECT_ID },
		{ TOOL_TYPE_ELLIPSE, SC_TOOL_ELLIPSE_ID },
		{ TOOL_TYPE_LINE,  SC_TOOL_LINE_ID },
		{ TOOL_TYPE_PEN,  SC_TOOL_PEN_ID },
		{ TOOL_TYPE_TEXT,  SC_TOOL_TEXT_ID },
		{ TOOL_TYPE_ERASER, SC_TOOL_ERASER_ID },
		{ TOOL_TYPE_ARROW, SC_TOOL_ARROW_ID }
	};
	
	nCount = sizeof(arTools) / sizeof(arTools[0]);

	return arTools;
}

LPACCEL GetAllelTable(INT& nCount)
{
	static	ACCEL arAccel[] = 
	{
		FCONTROL | FVIRTKEY, _T('C'), SC_TOOL_SELECT_ID,
		FCONTROL | FVIRTKEY, _T('W'), SC_TOOL_WIN_SELECT_ID,
		FCONTROL | FVIRTKEY, _T('R'), SC_TOOL_RECT_ID,
		FCONTROL | FVIRTKEY, _T('E'), SC_TOOL_ELLIPSE_ID,
		FCONTROL | FVIRTKEY, _T('L'), SC_TOOL_LINE_ID,
		FCONTROL | FVIRTKEY, _T('O'), SC_TOOL_ARROW_ID,
		FCONTROL | FVIRTKEY, _T('P'), SC_TOOL_PEN_ID,
		FCONTROL | FVIRTKEY, _T('T'), SC_TOOL_TEXT_ID,
		FCONTROL | FVIRTKEY, _T('N'), SC_TOOL_ERASER_ID,

		FCONTROL | FVIRTKEY, _T('Z'), SC_TOOL_UNDO_CMD_ID,
		FCONTROL | FVIRTKEY, _T('Y'), SC_TOOL_REDO_CMD_ID,

		FCONTROL | FVIRTKEY, _T('V'), SC_TOOL_PLASTER_CMD_ID,

		FCONTROL | FVIRTKEY, _T('S'), SC_SAVE_CMD_ID,
		FCONTROL | FVIRTKEY, _T('M'), SC_SAVE_EDIT_CMD_ID,

		FCONTROL | FVIRTKEY, _T('Q'), SC_COPY_EXIT_CMD_ID,
		FVIRTKEY, VK_ESCAPE, SC_EXIT_CMD_ID

	};

	nCount = sizeof(arAccel) / sizeof(arAccel[0]);

	return arAccel;
}

PEN_TYPE_ID_PAIR* GetPenTypeIDTable(int& nCount)
{
	static PEN_TYPE_ID_PAIR  arData[] = 
	{
		SMALL_PEN, PEN_SMALL_WIDTH, SC_TOOL_SMALL_PEN_ID,
		MIDDLE_PEN, PEN_MIDDLE_WIDTH, SC_TOOL_MIDDLE_PEN_ID,
		BIG_PEN, PEN_BIG_WIDTH, SC_TOOL_BIG_PEN_ID
	};

	nCount = sizeof(arData) / sizeof(arData[0]);

	return arData;
}

INT* GetFontSizeTable(INT& nCount)
{
	static int arFontSize[] = 
	{
	 20, 40, 60, 80, 100, 150, 200	
	};

	nCount = sizeof(arFontSize) / sizeof(arFontSize[0]);

	return arFontSize;
}

LPCTSTR GetResDllNameByLanType(CANVAS_LAN_TYPE eType)
{
	if(eType == LAN_TYPE_EN)
	{
		return _T("SCRes.Dll");
	}
	else if(eType == LAN_TYPE_SIMPLE_CH)
	{
		return _T("SCSCHRes.Dll");
	}
	else
	{
		_ASSERTE(FALSE);
		return NULL;
	}
}


CSCDrawerFactory::CSCDrawerFactory()
{
	InitializeFactory();
}

VOID CSCDrawerFactory::InitializeFactory()
{
	m_mapData.insert(make_pair<DWORD, CRefPtr<CSCCanvasBaseDrawer> >(TOOL_TYPE_SELECT, new CSCSelectDrawer));
	m_mapData.insert(make_pair(TOOL_TYPE_WIN_SELECT, new CSCWinSelectDrawer));
	m_mapData.insert(make_pair(TOOL_TYPE_RECTANGLE, new CSCRectDrawer));
	m_mapData.insert(make_pair(TOOL_TYPE_ELLIPSE, new CSCEllipseDrawer));
	m_mapData.insert(make_pair(TOOL_TYPE_LINE, new CSCLineDrawer));
	m_mapData.insert(make_pair(TOOL_TYPE_PEN, new CSCStrokeDrawer));
	m_mapData.insert(make_pair(TOOL_TYPE_ARROW, new CSCArrowDrawer));
	m_mapData.insert(make_pair(TOOL_TYPE_TEXT, new CSCTextDrawer));
	m_mapData.insert(make_pair(TOOL_TYPE_ERASER, new CSCEraserDrawer));
}

CRefPtr<CSCCanvasBaseDrawer> CSCDrawerFactory::GetDrawer(DWORD dwType)
{
	_ASSERTE(!m_mapData.empty());
	return m_mapData[dwType];
}

CRefPtr<CSCCanvasBaseDrawer> CSCDrawerFactory::GetDrawerByToolType(DWORD dwToolType)
{
	static CSCDrawerFactory factory;
	return factory.GetDrawer(dwToolType);
}