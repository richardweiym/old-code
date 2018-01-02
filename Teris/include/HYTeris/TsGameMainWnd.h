#pragma  once 

#include "TsStdAfx.h"
#include "TsUIDataMgr.h"
#include "TsDefine.h"
#include <vector>

class CTsMainButtonPane;
class CTsMainControlPane;
class CTsMainStatusPane;
class CTsMainDisplayPane;
class CTsSetupDialog;

class CTsGameMainWnd: public CWindowImpl<CTsGameMainWnd, CWindow, CFrameWinTraits>,
					  public CMessageFilter, 
				      public CIdleHandler
{
public:
	DECLARE_WND_CLASS(_T("TsMainWnd"))

	CTsGameMainWnd();
	~CTsGameMainWnd();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL OnIdle()
	{
		return FALSE;
	}
	
	BEGIN_MSG_MAP(CTsGameMainWnd)
		CHAIN_MSG_MAP_MEMBER_PTR(m_pMessageMap)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	END_MSG_MAP()

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetMessageMap(CMessageMap* pMap);
	BOOL AddKeyEventsListener(CMessageMap* pMap);
	BOOL RemoveKeyEventsListener(CMessageMap* pMap);

	bool Initialize();
	bool InitializeUI();
	void UnInitializeUI();
	void UnInitialize();	
	
	void ShowSetupDlg();

	HWND GetMainDisplayWnd();
	HWND GetLeftDisplayWnd();
	HWND GetRightDisplayWnd();
	HWND GetLeftNextWnd();
	HWND GetRightNextWnd();

protected:
	void _AdjustSize();
	void _DrawDragBar(INT nPos);
	INT _GetSplitterMovePos(INT nCursorX);

private:
	CTsGameMainWnd(const CTsGameMainWnd&);
	
	CMessageMap* m_pMessageMap;
	std::vector<CMessageMap*> m_arKeyEventsListeners;

	CTsMainButtonPane* m_pButtonPane;
	CTsMainControlPane* m_pControlPane;
	CTsMainStatusPane* m_pStatusPane;
	CTsMainDisplayPane* m_pDisplayPane;
	CTsSetupDialog* m_pSetupDlg;

	INT m_nSplitPos;
	double m_dbSplitRadio;
	CRect m_rtSplitter;
	BOOL m_bClipChild;

};