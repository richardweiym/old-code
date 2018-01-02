#include "TsGameMainWnd.h"
#include "TerisRes.h"
#include "TsMainGame.h"
#include "TsMainButtonPane.h"
#include "TsMainStatusPane.h"
#include "TsMainControlPane.h"
#include "TsMainDisplayPane.h"
#include "TsSetupDlg.h"
#include "TsDisplayContainer.h"

using namespace std;


#define BUTTON_PANE_HEIGHT 80
#define STATUS_PANE_HEIGHT 10
#define DEFAULT_SPIT_POS 750
#define SPLITTER_WIDTH 2
#define SPLIT_ZERO_RADIO 0.0001

CTsGameMainWnd* g_gameMainWnd = NULL;

CTsGameMainWnd::CTsGameMainWnd()
:m_rtSplitter(0, 0, 0, 0)
{
	TS_RUN_ONCE_CHECK
	m_pMessageMap = NULL;

	_ASSERTE(g_gameMainWnd == NULL);
	g_gameMainWnd = this;

	m_pButtonPane = NULL;
	m_pControlPane = NULL;
	m_pDisplayPane = NULL;
	m_pStatusPane = NULL;

	m_nSplitPos = DEFAULT_SPIT_POS;
	m_dbSplitRadio = 0.0;
	m_bClipChild = FALSE;
}
BOOL CTsGameMainWnd::PreTranslateMessage(MSG* pMsg)
{
	BOOL bRet(FALSE);
	if(m_pControlPane != NULL)
	{
		bRet = m_pControlPane->PreTranslateMessage(pMsg);
	}

	if(pMsg->message >= WM_KEYFIRST
		&& pMsg->message <= WM_KEYLAST)
	{
		int nCount = m_arKeyEventsListeners.size();
		CMessageMap* pMap = NULL;
		LRESULT lRet(0);
		for(int i=0; i<nCount; ++i)
		{
			pMap = m_arKeyEventsListeners[i];
			_ASSERTE(pMap != NULL);
			pMap->ProcessWindowMessage(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam, lRet, 0);
		}
	}

	return bRet;
}
CTsGameMainWnd::~CTsGameMainWnd()
{
	UnInitialize();
	g_gameMainWnd = NULL;
}

LRESULT CTsGameMainWnd::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	SetWindowText(_T("¶íÂÞË¹·½¿é"));

	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	
	_ASSERTE(m_pButtonPane != NULL);
	m_pButtonPane->Create(m_hWnd, CRect(0, 0, 0, 0));
	
	_ASSERTE(m_pControlPane != NULL);
	m_pControlPane->Create(m_hWnd, CRect(0, 0, 0, 0));

	_ASSERTE(m_pDisplayPane != NULL);
	m_pDisplayPane->Create(m_hWnd, CRect(0, 0, 0, 0));

	_ASSERTE(m_pStatusPane != NULL);
	m_pStatusPane->Create(m_hWnd, CRect(0, 0, 0, 0));

	return S_OK;
}

LRESULT CTsGameMainWnd::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;

	PostQuitMessage(1);

	return S_OK;
}

LRESULT CTsGameMainWnd::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&)
{
	return TRUE;
}

LRESULT CTsGameMainWnd::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	
	CPaintDC dc(m_hWnd);
	dc.FillSolidRect(&rtClient, RGB(0, 0, 255));
	

	return S_OK;
}
void CTsGameMainWnd::_DrawDragBar(INT nPos)
{
	CClientDC dc(m_hWnd);
	CBrush brushNew = CreateSolidBrush(RGB(255, 255, 255));
	CBrushHandle brushOld = dc.SelectBrush(brushNew);
	dc.PatBlt(nPos, m_rtSplitter.top, SPLITTER_WIDTH, m_rtSplitter.Height(), PATINVERT);
	dc.SelectBrush(brushOld);
}
INT CTsGameMainWnd::_GetSplitterMovePos(INT nCursorX)
{
	CRect rtClient;
	INT nPos(nCursorX);
	GetClientRect(&rtClient);

	if(nCursorX >= rtClient.Width() - SPLITTER_WIDTH)
	{
		nPos = rtClient.Width() - SPLITTER_WIDTH;
	}
	else if(nCursorX <=  0)
	{
		nPos = 0;
	}

	return nPos;
}
void CTsGameMainWnd::_AdjustSize()
{
	_ASSERTE(IsWindow());
	CRect rtClient(0, 0, 0, 0);
	GetClientRect(&rtClient);


	CRect rtButtonPane(0, 0, rtClient.Width(), BUTTON_PANE_HEIGHT), rtControlPane(0, 0, 0, 0), rtDisplayPane(0, 0, 0, 0), rtStatusPane(0, 0, 0, 0);
	if(rtClient.Height() > BUTTON_PANE_HEIGHT)
	{
		rtButtonPane.right = rtClient.Width();
		rtButtonPane.bottom = BUTTON_PANE_HEIGHT;

		if(rtClient.Height() - BUTTON_PANE_HEIGHT <= STATUS_PANE_HEIGHT)
		{
			rtStatusPane.right = rtClient.Width();
			rtStatusPane.top = rtButtonPane.Height();
			rtStatusPane.bottom = rtStatusPane.top + STATUS_PANE_HEIGHT;
		}
		else
		{
			if( m_dbSplitRadio <= SPLIT_ZERO_RADIO && m_nSplitPos != 0)
			{
				m_dbSplitRadio = m_nSplitPos / (double)rtClient.Width();
			}
			else
			{
				m_nSplitPos = m_dbSplitRadio * rtClient.Width();
			}

			rtStatusPane.right = rtClient.Width();
			rtStatusPane.top = rtClient.Height() - STATUS_PANE_HEIGHT;
			rtStatusPane.bottom = rtStatusPane.top + STATUS_PANE_HEIGHT;

			rtDisplayPane.top = rtButtonPane.bottom;
			rtDisplayPane.right = m_nSplitPos;
			rtDisplayPane.bottom = rtStatusPane.top;

			rtControlPane.left = m_nSplitPos + SPLITTER_WIDTH;
			rtControlPane.top = rtDisplayPane.top;
			rtControlPane.bottom = rtDisplayPane.bottom;
			rtControlPane.right = rtClient.right;

			m_rtSplitter.SetRect(m_nSplitPos, rtControlPane.top, m_nSplitPos + SPLITTER_WIDTH, rtControlPane.bottom);
		}
	}

	m_pButtonPane->SetWindowPos( NULL, rtButtonPane.left, rtButtonPane.top, rtButtonPane.Width(), rtButtonPane.Height(), SWP_NOACTIVATE);
	m_pControlPane->SetWindowPos( NULL, rtControlPane.left, rtControlPane.top, rtControlPane.Width(), rtControlPane.Height(), SWP_NOACTIVATE);
	m_pDisplayPane->SetWindowPos( NULL, rtDisplayPane.left, rtDisplayPane.top, rtDisplayPane.Width(), rtDisplayPane.Height(), SWP_NOACTIVATE);
	m_pStatusPane->SetWindowPos( NULL, rtStatusPane.left, rtStatusPane.top, rtStatusPane.Width(), rtStatusPane.Height(), SWP_NOACTIVATE);

}
LRESULT CTsGameMainWnd::OnSize(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam != SIZE_MINIMIZED)
	{	
		_AdjustSize();
	}

	return S_OK;
}
LRESULT CTsGameMainWnd::OnLButtonDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	
	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	if(!m_rtSplitter.IsRectEmpty() && m_rtSplitter.PtInRect(ptCursor))
	{
		if(GetStyle() & WS_CLIPCHILDREN)
		{
			m_bClipChild = TRUE;
			ModifyStyle(WS_CLIPCHILDREN, 0);
		}

		SetCapture();

		SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		
		_DrawDragBar(ptCursor.x);

		m_nSplitPos = ptCursor.x;

		bHandled = TRUE;
	}
	return S_OK;
}

LRESULT CTsGameMainWnd::OnLButtonUp(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	bHandled = FALSE;
	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	


	if(GetCapture() == m_hWnd)
	{
		CRect rtClient;
		GetClientRect(&rtClient);

		ReleaseCapture();
		_DrawDragBar(m_nSplitPos);
		m_nSplitPos = _GetSplitterMovePos(ptCursor.x);
		m_dbSplitRadio = m_nSplitPos / (double)rtClient.Width();
		_AdjustSize();
		bHandled = TRUE;
	}

	if(m_bClipChild)
	{
		ModifyStyle(0,  WS_CLIPCHILDREN);
		m_bClipChild = FALSE;
	}
	
	return S_OK;
}
LRESULT CTsGameMainWnd::OnMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	if(GetCapture() == m_hWnd)
	{
		INT nPos = _GetSplitterMovePos(ptCursor.x);

		_DrawDragBar(m_nSplitPos); //erase the old

		_DrawDragBar(nPos); //draw new
		m_nSplitPos = nPos;
	}
	else
	{
		if(!m_rtSplitter.IsRectEmpty() && m_rtSplitter.PtInRect(ptCursor))
		{
			SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			bHandled = TRUE;
		}
	}

	return S_OK;
}
void CTsGameMainWnd::SetMessageMap(CMessageMap* pMap)
{
	_ASSERTE(pMap != NULL);
	m_pMessageMap = pMap;
}
BOOL CTsGameMainWnd::AddKeyEventsListener(CMessageMap* pMap)
{
	_ASSERTE(pMap != NULL);
	BOOL bExist(FALSE);
	INT nCount = m_arKeyEventsListeners.size();
	for(int i=0; i<nCount; ++i)
	{
		if(m_arKeyEventsListeners[i] == pMap)
		{
			bExist = TRUE;
			break;
		}
	}
	
	if(bExist)
	{
		_ASSERTE(FALSE);
	}
	else
	{
		m_arKeyEventsListeners.push_back(pMap);
	}

	return !bExist;
}

BOOL CTsGameMainWnd::RemoveKeyEventsListener(CMessageMap* pMap)
{
	_ASSERTE(pMap != NULL);
	vector<CMessageMap*>::iterator itr = m_arKeyEventsListeners.begin();
	INT nCount = m_arKeyEventsListeners.size();
	for(; itr != m_arKeyEventsListeners.end() ; ++itr)
	{
		if(*itr == pMap)
		{
			break;
		}
	}

	if(itr == m_arKeyEventsListeners.end())
	{
		_ASSERTE(FALSE);
		return FALSE;
	}
	else
	{
		m_arKeyEventsListeners.erase(itr);
		return TRUE;
	}
}
void CTsGameMainWnd::ShowSetupDlg()
{
	_ASSERTE(m_pSetupDlg != NULL && IsWindow());
	m_pSetupDlg->DoModal(m_hWnd);
}
bool CTsGameMainWnd::Initialize()
{
	_ASSERTE(m_pButtonPane == NULL);
	_ASSERTE(m_pControlPane == NULL);
	_ASSERTE(m_pDisplayPane == NULL);
	_ASSERTE(m_pStatusPane == NULL);

	m_pButtonPane = new CTsMainButtonPane;
	m_pControlPane = new CTsMainControlPane;
	m_pDisplayPane = new CTsMainDisplayPane;
	m_pStatusPane = new CTsMainStatusPane;
	m_pSetupDlg = new CTsSetupDialog;


	if(m_pButtonPane == NULL
		|| m_pControlPane == NULL
		|| m_pDisplayPane == NULL
		|| m_pStatusPane == NULL
		|| m_pSetupDlg == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

bool CTsGameMainWnd::InitializeUI()
{
	Create(NULL, CRect(0, 0, 1024, 768));
	ShowWindow(SW_SHOW);
	UpdateWindow();

	return TRUE;
}

void CTsGameMainWnd::UnInitialize()
{
	if(m_pButtonPane != NULL)
	{
		delete m_pButtonPane;
		m_pButtonPane = NULL;
	}

	if(m_pControlPane != NULL)
	{
		delete m_pControlPane;
		m_pControlPane = NULL;
	}

	if(m_pDisplayPane != NULL)
	{
		delete m_pDisplayPane;
		m_pDisplayPane = NULL;
	}

	if(m_pStatusPane != NULL)
	{
		delete m_pStatusPane;
		m_pStatusPane = NULL;
	}
	
	if(m_pSetupDlg != NULL)
	{
		delete m_pSetupDlg;
		m_pSetupDlg = NULL;
	}
}

void CTsGameMainWnd::UnInitializeUI()
{
	
}

HWND CTsGameMainWnd::GetMainDisplayWnd()
{
	HWND hRet(NULL);
	if(m_pDisplayPane != NULL)
	{
		hRet = m_pDisplayPane->m_hWnd;
	}

	return hRet;
}

HWND CTsGameMainWnd::GetLeftDisplayWnd()
{
	HWND hRet = NULL;
	if(m_pDisplayPane != NULL)
	{
		CTsDisplayContainer* pWnd = m_pDisplayPane->GetLeftDisplayWnd();
		if(pWnd != NULL)
		{
			hRet = pWnd->m_hWnd;
		}
	}

	return hRet;
}

HWND CTsGameMainWnd::GetRightDisplayWnd()
{
	HWND hRet = NULL;
	if(m_pDisplayPane != NULL)
	{
		CTsDisplayContainer* pWnd = m_pDisplayPane->GetRightDisplayWnd();
		if(pWnd != NULL)
		{
			hRet = pWnd->m_hWnd;
		}
	}
	return hRet;
}

HWND CTsGameMainWnd::GetLeftNextWnd()
{
	HWND hRet = NULL;
	if(m_pDisplayPane != NULL)
	{
		CTsNextElementContainer* pWnd = m_pDisplayPane->GetLeftNextWnd();
		if(pWnd != NULL)
		{
			hRet = pWnd->m_hWnd;
		}
	}
	return hRet;
}

HWND CTsGameMainWnd::GetRightNextWnd()
{
	HWND hRet = NULL;
	if(m_pDisplayPane != NULL)
	{
		CTsNextElementContainer* pWnd = m_pDisplayPane->GetRightNextWnd();
		if(pWnd != NULL)
		{
			hRet = pWnd->m_hWnd;
		}
	}
	return hRet;
}