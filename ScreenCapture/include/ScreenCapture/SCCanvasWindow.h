#pragma once 

#include "HYUtilbase.h"
#include "HYUIBase.h"
#include "hyautoptr.h"
#include "SCCanvasSkin.h"
#include "SCCanvasDrawDirector.h"
#include "SCCanvasContext.h"
#include "SCCanvasData.h"

class CSCCanvasData;
class CSCCanvasContext;
class CSCCanvasFloatTool;
class CSCCanvasCmdMgr;
class CSCCanvasStartWin;	
class CSCHelpTipWindow;
class CSCInfoBubbleTip;
class CSCAboutDialog;
class CSCSettinDialog;

// Canvas main window
// 
//
#ifdef _DEBUG
typedef  CWinTraits<WS_POPUP, WS_EX_TOOLWINDOW /*| WS_EX_TOPMOST*/> CCanvasTraits;
#else
typedef  CWinTraits<WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST> CCanvasTraits;
#endif //_DEBUG

class CSCCanvasWindow: public CWindowImpl<CSCCanvasWindow, CWindow, CCanvasTraits>,
					   public CRefCountBase,
					   public CSCSkinObserver<CSCCanvasWindow>,
					   public CSCContextObserver<CSCCanvasWindow>,
					   public CSCDataObserver<CSCCanvasWindow>,
					   public CMessageFilter,
					   public CIdleHandler
{
public:
	DECLARE_WND_CLASS(_T("CSCCanvasClass"))

	BEGIN_MSG_MAP(CSCCanvasWindow)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
		CHAIN_MSG_MAP(CSCSkinObserver<CSCCanvasWindow>)
		CHAIN_MSG_MAP(CSCContextObserver<CSCCanvasWindow>)
		CHAIN_MSG_MAP(CSCDataObserver<CSCCanvasWindow>)
		CHAIN_MSG_MAP_MEMBER_REF_PTR(m_pDrawDirector)

		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDBClick)
		
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_CANVAS_DRAW_EVENT, OnDrawEvent)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)

		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)

		MESSAGE_HANDLER(WM_TIMER, OnTimer)

	END_MSG_MAP()
	
	CSCCanvasWindow();
	~CSCCanvasWindow();

	BOOL Initialize();
	BOOL InitializeUI();

	VOID Uninitialize();
	VOID UninitializeUI();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	CRefPtr<CSCCanvasData> GetCanvasData() const;
	CRefPtr<CSCCanvasContext> GetCanvasContext() const;
	
	void DoUIDataChanged(WPARAM wParam, LPARAM lParam);

	VOID OnAddGraph(CRefPtr<CSCGraphObject> pGraph);
	VOID OnEraseGraph(CRefPtr<CSCGraphObject> pGraph);
	VOID UpdateHelpTip();
	
	VOID RedrawCanvas(CRect* pRtDraw, BOOL bUpdate = TRUE);
	
	VOID StartCapture();
	
	BOOL CanUndo();
	BOOL CanRedo();

protected:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT	OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLButtonDBClick(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCommand(UINT nMsgTye, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawEvent(UINT, WPARAM wParam, LPARAM lParam,	BOOL& bHandled);
	LRESULT OnRButtonUp(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnMouseMove(UINT, WPARAM, LPARAM, BOOL&);

	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&);

protected:
	
	BOOL ShowBmpCopiedBubble(BOOL bShow);

	LRESULT OnToolSelectCmd(WORD nCmd);
	VOID CreateAccelTable();
	VOID DestroyAccelTable();
	
	VOID CopySelectToClipboard();

	VOID OnSaveCommand();
	VOID OnSaveEditCommand();
	VOID OnCopyAndExitCmd();
	VOID OnStartCaptureCmd();

	CString GetSaveFile();
	VOID CloseSaveDlg();

	VOID StartEditImage(LPCTSTR lpszFileName);

protected:
	BOOL InitCanvasData();

	BOOL CreateCanvas();
	BOOL CreateToolPanel();

	VOID InitCanvasTool();

	VOID ExitApp();

private:
	CRefPtr<CSCCanvasData> m_pCanvasData;
	CRefPtr<CSCCanvasDrawDirector> m_pDrawDirector;
	CRefPtr<CSCCanvasContext> m_pCanvasContext;
	CRefPtr<CSCCanvasFloatTool> m_pFloatTool;
	CRefPtr<CSCCanvasCmdMgr> m_pCmdMgr;
	CRefPtr<CSCCanvasStartWin> m_pStartWin;
	CRefPtr<CSCHelpTipWindow> m_pHelpTipWin;

	CRefPtr<CSCInfoBubbleTip> m_pInfoBubble;
	
	CRefPtr<CSCAboutDialog> m_pAboutDialog;
	CRefPtr<CSCSettinDialog> m_pSettingDialog;

	HACCEL m_hAccel;
	BOOL m_bExiting;
};