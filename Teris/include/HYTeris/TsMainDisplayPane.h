#pragma  once 

#include "TsStdAfx.h"
#include "TsDefine.h"
#include "TsUIDataMgr.h"

class CTsTypeElement;
class CTsNextElementContainer: public CWindowImpl<CTsNextElementContainer>
{
public:
	DECLARE_WND_CLASS(_T("TsNextElement"))
	BEGIN_MSG_MAP(CTsNextElementContainer)
		MESSAGE_HANDLER(WM_TS_SET_NEXT_ELEMENT, OnSetNextElement)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	CTsNextElementContainer();
	CSize GetIdealSize();
	
	LRESULT OnSetNextElement(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM,  BOOL& bHandled);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled);

protected:
	void _DrawBkgnd(CDCHandle dc);
	void _DrawElement(CDCHandle dc);

private:
	CTsTypeElement* m_pElement;
};

class CTsSingleScoreContainer: public CWindowImpl<CTsSingleScoreContainer>,
							   public CTsPlayProcessObserver<CTsSingleScoreContainer>
{
public:
	DECLARE_WND_CLASS(_T("TsSingleScore"))

	typedef CTsPlayProcessObserver<CTsSingleScoreContainer> _base;
	BEGIN_MSG_MAP(CTsSingleScoreContainer)
		CHAIN_MSG_MAP(_base)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

	void SetTextFont(HFONT hFont) { m_fntText = hFont; }
	CSize GetIdealSize();
	VOID DoUIDataChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled);

private:
	enum 
	{	
		ELE_COUNT_CAPTION = 0,
		ELE_COUNT_VALUE,
		
		SCORE_CAPTION,
		SCORE_VALUE,
		
		REMOVE_LINE_CAPTION,
		REMOVE_LINE_VALUE
	};

	CRect _GetTextRect(int nIndex);

	void _DrawBkgnd(CDCHandle dc);
	void _DrawScore(CDCHandle dc);
	CFontHandle _GetTextFont();
	void _DrawCaption(CDCHandle dc);
	void _DrawValue(CDCHandle dc);

private:
	CFontHandle m_fntText;
	TCHAR m_szEleCount[64];
	TCHAR m_szScore[64];
	TCHAR m_szRemoveCount[64];
};

class CTsMutiScoreContainer: public CWindowImpl<CTsMutiScoreContainer>,
							 public CTsPlayProcessObserver<CTsMutiScoreContainer>
{
public:
	DECLARE_WND_CLASS(_T("TsMultiScore"))
	typedef CTsPlayProcessObserver<CTsMutiScoreContainer> _base;
	BEGIN_MSG_MAP(CTsMutiScoreContainer)
		CHAIN_MSG_MAP(_base)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

	void SetTextFont(HFONT hFont) { m_fntText = hFont; }
	CSize GetIdealSize();
	VOID DoUIDataChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled);

private:
	enum 
	{	
		PLAYER_CAPTION = 0,
		PLAYER_NAME,

		ELE_COUNT_CAPTION,
		ELE_COUNT_VALUE,
		
		SCORE_CAPTION,
		SCORE_VALUE,
		
		REMOVE_LINE_CAPTION,
		REMOVE_LINE_VALUE,
		KILL_LINE_VALUE,

		WIN_COUNT_CAPTION,
		WIN_COUNT_VALUE
	};
	void _DrawBkgnd(CDCHandle dc);
	CFontHandle _GetTextFont();
	CRect _GetTextRect(int nIndex, INT nType);//nType: Total(0), left Half(1), right Half(2)
	void _DrawScore(CDCHandle dc);
	void _DrawCaption(CDCHandle dc);
	void _DrawValue(CDCHandle dc);
private:
	CFontHandle m_fntText;
	TCHAR m_szEleCount[64];
	TCHAR m_szScore[64];
	TCHAR m_szRemoveCount[64];
	TCHAR m_szPlayer[64];
	TCHAR m_szWinCount[64];
};


class CTsDisplayContainer;
class ITsWinFrame;
class CTsMainDisplayPane: public CWindowImpl<CTsMainDisplayPane>,
						  public CTsGameBaseInfoObserver<CTsMainDisplayPane>
{
public:
	DECLARE_WND_CLASS(_T("TsMainDisplayPane"))

	typedef  CTsGameBaseInfoObserver<CTsMainDisplayPane> _base;
	BEGIN_MSG_MAP(CTsMainDisplayPane)
		CHAIN_MSG_MAP(_base)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)

		MESSAGE_HANDLER(WM_TS_UPDATE_DISPLAY, OnUpdateDisplay)
	END_MSG_MAP()

	CTsMainDisplayPane();
	~CTsMainDisplayPane();
	
	VOID DoUIDataChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateDisplay(UINT, WPARAM, LPARAM, BOOL&);

	CTsDisplayContainer* GetLeftDisplayWnd() { return m_pLeftDisplayContainer; }
	CTsDisplayContainer* GetRightDisplayWnd() { return m_pRightDisplayContainer; }

	CTsNextElementContainer* GetLeftNextWnd() { return m_pLeftNextContainer; }
	CTsNextElementContainer* GetRightNextWnd() { return m_pRightNextContainer; }

protected:
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled);

protected:
	void _CheckDisplayStatus();
	void _CheckGameMode();
	void _ClearDisplayData();

	BOOL _NewObj();
	void _DeleteObj();
	BOOL _CreateObj();

	void _AdjustSingleFun(BOOL bLeft);
	void _AdjustSingle();
	void _AdjustMulti();

	void _SetSingleStatus();
	void _SetMultiStatus();
	void _SetNoneStatus();

	void _DrawNoneBkgnd(CDCHandle dc);
	void _DrawSingleBkgnd(CDCHandle dc);
	void _DrawMultiBkgnd(CDCHandle dc);
	void _DrawSingleFrame(CDCHandle dc);
	void _DrawMultiFrame(CDCHandle dc);
	
private:
	CTsDisplayContainer* m_pLeftDisplayContainer;
	CTsDisplayContainer* m_pRightDisplayContainer;

	CTsNextElementContainer* m_pLeftNextContainer;
	CTsNextElementContainer* m_pRightNextContainer;

	CTsSingleScoreContainer* m_pSingleScoreContainer;
	CTsMutiScoreContainer* m_pMultiScoreContainer;

	ITsWinFrame* m_pLeftDisplayFrame;
	ITsWinFrame* m_pRightDisplayFrame;
	ITsWinFrame* m_pLeftNextFrame;
	ITsWinFrame* m_pRightNextFrame;
	ITsWinFrame* m_pSingleScoreFrame;
	ITsWinFrame* m_pMultiScoreFrame;

};