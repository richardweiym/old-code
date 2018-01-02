#pragma  once 

#include "HYUtilBase.h"
#include "HYAutoPtr.h"
#include <map>


enum CANVAS_TOOL_TYPE
{
	TOOL_TYPE_NONE = 0,

	TOOL_TYPE_SELECT,
	TOOL_TYPE_WIN_SELECT,

	TOOL_TYPE_RECTANGLE,
	TOOL_TYPE_ELLIPSE,

	TOOL_TYPE_LINE,
	TOOL_TYPE_ARROW,

	TOOL_TYPE_PEN,

	TOOL_TYPE_TEXT,

	TOOL_TYPE_ERASER,

	TOOL_TYPE_END
};

enum CANVAS_SKIN_TYPE
{
	SKIN_TYPE_NONE = 0,

	SKIN_TYPE_1 = 1,
	SKIN_TYPE_2 = 2,
	
	SKIN_TYPE_COUNT = 2
};

enum CANVAS_LAN_TYPE //language type
{
	LAN_TYPE_NONE = 0,

	LAN_TYPE_EN = 1,			// English
	LAN_TYPE_SIMPLE_CH = 2,		// Simple Chinese

	LAN_TYPE_COUNT = 2		
};


#define TOOL_TYPE_START (TOOL_TYPE_NONE + 1)


//Data module change message(wparam) for message WM_HY_UI_DATA_CHANGED
#define UI_WPARAM_SKIN_CHANGED				100
#define UI_WPARAM_TOOL_CHANGED				101
#define UI_WPARAM_SELECT_RECT_CHANGED		102
#define UI_WPARAM_LAN_CHANGED				103

//use define message for canvas
#define WM_CANVAS_BEGIN						WM_USER + 5000
#define WM_CANVAS_DRAW_EVENT				WM_CANVAS_BEGIN + 1
#define DRAW_EVENT_WPARAM_DRAW_START		1	//lparam: not used
#define DRAW_EVENT_WPARAM_DRAW_END			2	//lparam: graph object
#define DRAW_EVENT_WPARAM_ERASER			3	//lparam: graph ID that cursor hit

#define DRAW_EVENT_WPARAM_SELECT_CHANGED	4 //lparam: CRectChangeData*
struct CRectChangeData
{
	CRect rtOld;
	CRect rtNew;
};

//ID smaller than 50000 is used by resource.h, avoid conflicition
#define SC_START_CAPTURE_BTN_ID 50000 + 10		//ready button on start window
#define SC_CLOSE_CAPTURE_BTN_ID 50000 + 11		//close button on start window

#define SC_CLOSE_BTN_ID 50000 + 50		//close button on start window

//button id on float tool panel
#define SC_TOOL_ID_BASE			50000 + 100
#define SC_TOOL_SELECT_ID		SC_TOOL_ID_BASE + 1
#define SC_TOOL_RECT_ID			SC_TOOL_ID_BASE + 2
#define SC_TOOL_ELLIPSE_ID		SC_TOOL_ID_BASE + 3
#define SC_TOOL_LINE_ID			SC_TOOL_ID_BASE + 4
#define SC_TOOL_ARROW_ID		SC_TOOL_ID_BASE + 5
#define SC_TOOL_PEN_ID			SC_TOOL_ID_BASE + 6
#define SC_TOOL_TEXT_ID			SC_TOOL_ID_BASE + 7
#define SC_TOOL_ERASER_ID		SC_TOOL_ID_BASE + 8
#define SC_TOOL_WIN_SELECT_ID	SC_TOOL_ID_BASE + 9
#define SC_TOOL_ID_END			SC_TOOL_ID_BASE + 120

#define SC_TOOL_EXTEND_MENU_ID	SC_TOOL_ID_BASE	+ 121 //float tool extend menu button

#define SC_TOOL_CMD_ID_BASE		(SC_TOOL_ID_BASE + 130)
#define SC_TOOL_UNDO_CMD_ID		SC_TOOL_CMD_ID_BASE + 1 //undo
#define SC_TOOL_REDO_CMD_ID		SC_TOOL_CMD_ID_BASE + 2 //redo
#define SC_TOOL_PLASTER_CMD_ID	SC_TOOL_CMD_ID_BASE + 3 //Ctrl+V plaster in text mode



//pen width ID on Float pen select panel
#define SC_TOOL_SUB_ID_BASE		(SC_TOOL_ID_BASE + 200)
#define SC_TOOL_SMALL_PEN_ID	SC_TOOL_SUB_ID_BASE + 1
#define SC_TOOL_MIDDLE_PEN_ID	SC_TOOL_SUB_ID_BASE + 2
#define SC_TOOL_BIG_PEN_ID		SC_TOOL_SUB_ID_BASE + 3
#define SC_TOOL_COLOR_PICKER_ID	SC_TOOL_SUB_ID_BASE + 4

#define SC_TOOL_FONT_BOLD_ID	SC_TOOL_SUB_ID_BASE + 5
#define SC_TOOL_FONT_SIZE_ID	SC_TOOL_SUB_ID_BASE + 6
#define SC_TOOL_FONT_NAME_ID	SC_TOOL_SUB_ID_BASE + 7

#define SC_TOOL_SUB_ID_BASE_END (SC_TOOL_ID_BASE + 250)

//skin command id
#define SC_SKIN_TYPE_ID_BASE	(SC_TOOL_ID_BASE + 251)
#define SC_SKIN_TYPE_ID_1		SC_SKIN_TYPE_ID_BASE + 1
#define SC_SKIN_TYPE_ID_2		SC_SKIN_TYPE_ID_BASE + 2
#define SC_SKIN_TYPE_ID_3		SC_SKIN_TYPE_ID_BASE + 3
#define SC_SKIN_TYPE_ID_4		SC_SKIN_TYPE_ID_BASE + 4
#define SC_SKIN_TYPE_ID_5		SC_SKIN_TYPE_ID_BASE + 5
#define SC_SKIN_TYPE_ID_END		(SC_TOOL_ID_BASE + 260)

#define SC_SHOW_HELP_TIP_CMD_ID	SC_TOOL_ID_BASE + 262	//display help tip
#define SC_SAVE_CMD_ID			SC_TOOL_ID_BASE + 263	//save...
#define SC_COPY_EXIT_CMD_ID		SC_TOOL_ID_BASE + 264	//copy and exit
#define SC_EXIT_CMD_ID			SC_TOOL_ID_BASE + 265	//exit
#define SC_DOUBLE_CLICK_TO_EXIT SC_TOOL_ID_BASE + 266	//Double click mouse to copy & exit
#define SC_SHOW_CURSOR_CMD_ID	SC_TOOL_ID_BASE + 267	//Show Cursor

#define SC_TRANSPARENT_MARK_ID  SC_TOOL_ID_BASE + 268	//Transparent Annotation

#define SC_ABOUT_ID				SC_TOOL_ID_BASE + 269   //About...
#define SC_SETTING_ID			SC_TOOL_ID_BASE + 270   //Setting...

#define SC_SAVE_EDIT_CMD_ID			SC_TOOL_ID_BASE + 271	//save & Edit...

//language command id
#define SC_LAN_TYPE_ID_BASE		(SC_TOOL_ID_BASE + 281)
#define SC_LAN_TYPE_ID_1		SC_LAN_TYPE_ID_BASE + 1
#define SC_LAN_TYPE_ID_2		SC_LAN_TYPE_ID_BASE + 2
#define SC_LAN_TYPE_ID_END		(SC_TOOL_ID_BASE + 290)

#define PEN_SMALL_WIDTH			2
#define PEN_MIDDLE_WIDTH		10
#define PEN_BIG_WIDTH			20

enum PEN_TYPE
{
	NONE_PEN,
	SMALL_PEN,	//PEN_SMALL_WIDTH
	MIDDLE_PEN, //PEN_MIDDLE_WIDTH
	BIG_PEN		//PEN_BIG_WIDTH 	
};


//offset for water mark
#define WATER_MARK_X_OFFSET 50
#define WATER_MARK_Y_OFFSET 80

//offset for float tool
#define FLOAT_TOOL_X_OFFSET	100
#define FLOAT_TOOL_Y_OFFSET 120

#define HELP_TIP_WIN_WIDTH 290
#define HELP_TIP_WIN_HEIGHT 100


#define SC_PEN_DEF_STYLE (PS_SOLID | PS_INSIDEFRAME)
#define SC_PEN_TRACK_COLOR RGB(255, 255, 255)


#define ERASER_DIST_COUNT 1.0 //error distance when erase

#define SC_HELP_ZOOM_RATE	   2

struct TOOL_ID_PAIR
{
	DWORD dwToolType;
	DWORD dwBtnID;
};	
TOOL_ID_PAIR* GetToolBtnIDTable(int& nCount);

LPACCEL GetAllelTable(INT& nCount);

struct PEN_TYPE_ID_PAIR
{
	DWORD dwType;
	DWORD dwWidth;
	DWORD dwBtnID;
};
PEN_TYPE_ID_PAIR* GetPenTypeIDTable(int& nCount);


INT* GetFontSizeTable(INT& nCount);

LPCTSTR GetResDllNameByLanType(CANVAS_LAN_TYPE eType);

class CSCCanvasBaseDrawer;
class CSCDrawerFactory
{
public:
	static CRefPtr<CSCCanvasBaseDrawer> GetDrawerByToolType(DWORD dwToolType);

protected:
	CSCDrawerFactory();
	VOID InitializeFactory();
	CRefPtr<CSCCanvasBaseDrawer> GetDrawer(DWORD dwType);

protected:
	std::map<DWORD, CRefPtr<CSCCanvasBaseDrawer> > m_mapData;
};