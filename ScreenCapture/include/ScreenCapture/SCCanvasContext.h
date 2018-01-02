#pragma  once 

#include "HYAutoPtr.h"
#include "HYPattern.h"
#include "SCIDDef.h"
#include <map>

//tool data
struct CANVAS_TOOL_DATA
{
	CANVAS_TOOL_DATA()
	{
		m_clrTool = RGB(255, 0, 0);
		m_nWidth = PEN_SMALL_WIDTH;
		m_dwUserData = 0;	
	}

	COLORREF m_clrTool; 
	INT m_nWidth;
	DWORD m_dwUserData; //user define data, only used for TOOL_TYPE_PEN
};

class CSCCanvasContext: public CRefCountBase,
						public CHYDataModuleBase
{
public:
	CSCCanvasContext();
	~CSCCanvasContext();

	BOOL Initialize();
	VOID Uninitialize();

	DWORD SetToolType(DWORD eType);
	DWORD GetToolType() const { return m_eToolType; }
	
	VOID SetToolData(CANVAS_TOOL_TYPE eType, CANVAS_TOOL_DATA ta);
	BOOL GetToolData(CANVAS_TOOL_TYPE eType, CANVAS_TOOL_DATA& taRet);
	
	CANVAS_TOOL_DATA* GetCurrentToolData();
	VOID SetCurrentToolData( const CANVAS_TOOL_DATA& data);

	BOOL GetShowHelpFlag() const { return m_bShowHelp; }
	VOID SetShowHelpFlag(BOOL bSet);

	BOOL GetDoubleClickExitFlag() const { return m_bDoubleClickExit; }
	VOID SetDoubleClickExitFlag(BOOL bSet);

	BOOL GetShowCursorFlag() const { return m_bShowCursor; }
	VOID SetShowCursorFlag(BOOL bShow) { m_bShowCursor = bShow; }

	VOID SetShowStartWin(BOOL bSet) { m_bShowStartWin = bSet; }
	BOOL GetShowStartWin() const { return m_bShowStartWin; }
	
	VOID SetTransparentMark(BOOL bSet) { m_bTransparentMark = bSet; }
	BOOL GetTransparentMark() const { return m_bTransparentMark; }

	VOID SetFileFilterIndex(INT nIndex) { m_nFileFilterIndex = nIndex; }
	INT GetFileFilterIndex() const { return m_nFileFilterIndex; }

	VOID SetExitAfterImageSaved(BOOL bSet) { m_bExitAfterImageSaved = bSet; }
	BOOL GetExitAfterImageSaved() const { return m_bExitAfterImageSaved; }

protected:
	void InitContextData();
	void InitToolsData();

protected:
	typedef std::map<CANVAS_TOOL_TYPE, CANVAS_TOOL_DATA> CToolMap;
	CToolMap m_mapTools;	//tool data (color, font, width)
	CANVAS_TOOL_TYPE m_eToolType;

	BOOL m_bShowHelp;
	BOOL m_bDoubleClickExit;
	BOOL m_bShowCursor;
	
	BOOL m_bShowStartWin;
	BOOL m_bTransparentMark;
	INT m_nFileFilterIndex;

	BOOL m_bExitAfterImageSaved;
};

template<typename T>
class CSCContextObserver
{
public:
	BEGIN_MSG_MAP(CSCContextObserver<T>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_HY_UI_DATA_CHANGED, OnUIDataChanged)
	END_MSG_MAP()

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pThis = static_cast<T*>(this);

		CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
		_ASSERTE(!pContext.IsNull());
		pContext->AddObserver(pThis->m_hWnd);

		return S_OK;
	}
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pThis = static_cast<T*>(this);

		CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
		_ASSERTE(!pContext.IsNull());
		pContext->AddObserver(pThis->m_hWnd);

		return S_OK;
	}
	LRESULT OnDestroy(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pThis = static_cast<T*>(this);

		CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
		_ASSERTE(!pContext.IsNull());
		pContext->RemoveObserver(pThis->m_hWnd);

		return S_OK;
	}

	LRESULT OnUIDataChanged(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pThis = static_cast<T*>(this);
		if(pThis->IsWindow() && wParam == UI_WPARAM_TOOL_CHANGED)
		{
			pThis->DoUIDataChanged(wParam, lParam);
			bHandled = TRUE;
		}

		return S_OK;
	}
	
};