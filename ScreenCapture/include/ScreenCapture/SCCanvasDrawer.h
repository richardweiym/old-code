#pragma  once 

#include "HYAutoPtr.h"
#include <vector>
#include "SCCanvasGraph.h"


class CSCCanvasBaseDrawer: public CMessageMap,
						   public CRefCountBase
{
public:
	CSCCanvasBaseDrawer();
	virtual ~CSCCanvasBaseDrawer();
	
	virtual VOID DrawCurrentView(CDCHandle dc);

	virtual VOID OnDrawStart(); //when start draw a new graph
	virtual VOID OnDrawEnd();

	virtual VOID OnGetControl();	//when tool type changed
	virtual VOID OnLostControl();
	

protected:
	VOID DrawSelectFrame(CDCHandle dc, BOOL bCanSizing = FALSE);
	BOOL GetSizeMarkRect(CRect& rtFrame, DWORD dwPosition, CRect& rtMark);
};



class CSCSelectDrawer: public CSCCanvasBaseDrawer
{
public:
	BEGIN_MSG_MAP(CSCSelectDrawer)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	END_MSG_MAP()

	CSCSelectDrawer();
	virtual VOID DrawCurrentView(CDCHandle dc);

	virtual VOID OnDrawEnd();

protected:
	LRESULT OnLButtonDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL&);

protected:
	VOID Reset();

	VOID OutputSelectRect(BOOL bEnd = FALSE);
	VOID OutputMovingRect(BOOL bEnd = FALSE);
	VOID OutputSizingRect(BOOL bEnd = FALSE);
	
	DWORD HitTest(CPoint& pt);

protected:
	CPoint m_ptStart;
	CPoint m_ptEnd;
	CPoint m_ptPrev;

	DWORD m_dwStatus;
	DWORD m_dwHitPos;

	CRect m_rtOldSelect;
};

class CSCWinSelectDrawer: public CSCCanvasBaseDrawer
{
public:
	BEGIN_MSG_MAP(CSCWinSelectDrawer)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	END_MSG_MAP()

	CSCWinSelectDrawer();

	virtual VOID OnGetControl();
	virtual VOID OnLostControl();

	virtual VOID DrawCurrentView(CDCHandle dc);
	virtual VOID OnDrawEnd();

protected:
	LRESULT OnMouseMove(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL&);

	VOID DrawTrackGraph();
	VOID DrawSelectWinFrame(CDCHandle dc); 

protected:
	CRect m_rtCurrent;
	CRect m_rtPrev;
	CPoint m_ptPrev;
};

class CSCTwoPointDrawer: public CSCCanvasBaseDrawer
{
public:
	BEGIN_MSG_MAP(CSCTwoPointDrawer)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	END_MSG_MAP()

	CSCTwoPointDrawer();
	virtual VOID DrawCurrentView(CDCHandle dc);

	virtual VOID OnDrawEnd();

	virtual VOID OnGetControl();
	virtual VOID OnLostControl();

protected:
	LRESULT OnLButtonDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL&);

protected:
	VOID Reset();

	BOOL ResetCurrentPen();
	VOID DestroyCurrentPen();
	virtual VOID DrawTrackGraph();


protected:
	CPoint m_ptStart;
	CPoint m_ptEnd;
	CPoint m_ptPrev;

	CPen m_CurrentPen;
};

class CSCMultiPointDrawer: public CSCCanvasBaseDrawer
{
public:
	BEGIN_MSG_MAP(CSCMultiPointDrawer)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	END_MSG_MAP()

	CSCMultiPointDrawer();
	virtual VOID DrawCurrentView(CDCHandle dc);

	virtual VOID OnDrawEnd();
	
	virtual VOID OnGetControl();
	virtual VOID OnLostControl();

protected:
	virtual LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL& bHandled) { bHandled = FALSE; return S_OK; } 

	LRESULT OnLButtonDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL&);

protected:
	VOID Reset();
	BOOL ResetCurrentPen();
	VOID DestroyCurrentPen();
	virtual VOID DrawTrackGraph();


protected:
	std::vector<CPoint> m_arData;
	CPen m_CurrentPen;
};

class CSCRectDrawer: public CSCTwoPointDrawer
{
public:
	virtual VOID DrawCurrentView(CDCHandle dc);
	virtual VOID OnDrawEnd();
};

class CSCEllipseDrawer: public CSCTwoPointDrawer
{
public:
	virtual VOID DrawCurrentView(CDCHandle dc);
	virtual VOID OnDrawEnd();
};

class CSCLineDrawer: public CSCTwoPointDrawer
{
public:
	virtual VOID DrawCurrentView(CDCHandle dc);
	virtual VOID OnDrawEnd();

protected:
	virtual VOID DrawTrackGraph();
};
class CSCArrowDrawer: public CSCTwoPointDrawer
{
public:
	virtual VOID DrawCurrentView(CDCHandle dc);
	virtual VOID OnDrawEnd();

protected:
	virtual VOID DrawTrackGraph();
};

class CSCStrokeDrawer: public CSCMultiPointDrawer
{
public:
	virtual VOID DrawCurrentView(CDCHandle dc);
	virtual VOID OnDrawEnd();

protected:
	virtual LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL& bHandled);
	virtual VOID DrawTrackGraph();
};


class CSCTextDrawer: public CSCCanvasBaseDrawer
{
public:
	BEGIN_MSG_MAP(CSCTextDrawer)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	END_MSG_MAP()

	CSCTextDrawer();
	virtual VOID DrawCurrentView(CDCHandle dc);

	virtual VOID OnDrawStart();
	virtual VOID OnDrawEnd();
	
	virtual VOID OnGetControl();
	virtual VOID OnLostControl();

protected:
	LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnKillFocus(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnChar(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL&);
	
protected:
	VOID ResetCurrentGraph();
	VOID DeleteCurrentGraph();
	VOID EmptyCurrentText();
	
	VOID OnBackDraw(int nBackCount);
	VOID OnAddDraw(LPCTSTR lpszText);

	VOID OnPlasterCmd();
	BOOL GetClipboardText(CString& strText);
	
	BOOL Undo();

protected:
	CRefPtr<CTextGraph> m_pTextGraph;
};

class CSCEraserDrawer: public CSCCanvasBaseDrawer
{
public:
	BEGIN_MSG_MAP(CSCEraserDrawer)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	END_MSG_MAP()
	
	CSCEraserDrawer();
	virtual VOID OnDrawEnd();

protected:
	LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL&);

protected:
	CPoint m_ptEraser;
};