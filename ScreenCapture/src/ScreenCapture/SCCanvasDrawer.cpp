#include "HYUtilBase.h"
#include "SCCanvasDrawer.h"
#include "SCCanvasContext.h"
#include "SCInstanceAPI.h"
#include "SCCanvasWindow.h"
#include "SCCanvasData.h"
#include "SCWinSpy.h"

#define CANVAS_SELECT_FRAME_COLOR RGB(0, 0, 255)
#define CANVAS_SELECT_FRAME_WIDTH 1
#define CANVAS_SELECT_SIZE_MARK_WIDTH 5

#define CANVAS_WIN_SELECT_FRAME_COLOR RGB(255, 0, 0)
#define CANVAS_WIN_SELECT_FRAME_WIDTH	3

//hit test code on the select rect
#define HT_POS_NONE 0
#define HT_POS_LEFT 1
#define HT_POS_TOP 2
#define HT_POS_RIGHT 3
#define HT_POS_BOTTOM 4
#define HT_POS_LEFT_TOP 5
#define HT_POS_LEFT_BOTTOM 6
#define HT_POS_RIGHT_TOP 7
#define HT_POS_RIGHT_BOTTOM 8
#define HT_POS_IN_RECT 9

//operation type when Left Button Click
#define SELECT_STATUS_IDLE 1
#define SELECT_STATUS_MOVING 2
#define SELECT_STATUS_SIZING 3
#define SELECT_STATUS_SELECTING 4

#define DEFAULT_CARET_WIDTH 2



CSCCanvasBaseDrawer::CSCCanvasBaseDrawer()
{

}

CSCCanvasBaseDrawer::~CSCCanvasBaseDrawer()
{

}

VOID CSCCanvasBaseDrawer::DrawCurrentView(CDCHandle dc)
{
	DrawSelectFrame(dc);
}
VOID CSCCanvasBaseDrawer::OnDrawStart()
{
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(WM_CANVAS_DRAW_EVENT, DRAW_EVENT_WPARAM_DRAW_START, 0);
}

VOID CSCCanvasBaseDrawer::OnDrawEnd()
{
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(WM_CANVAS_DRAW_EVENT, DRAW_EVENT_WPARAM_DRAW_END, 0);
}
VOID CSCCanvasBaseDrawer::OnGetControl()
{
	NULL;
}
VOID CSCCanvasBaseDrawer::OnLostControl()
{
	NULL;
}

VOID CSCCanvasBaseDrawer::DrawSelectFrame(CDCHandle dc, BOOL bCanSizing /* = FALSE */)
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	_ASSERTE(!pData.IsNull());

	CRect rtSelect = pData->GetSelectRect();
	if(rtSelect.IsRectEmpty()) return;

	rtSelect.InflateRect(CANVAS_SELECT_FRAME_WIDTH, CANVAS_SELECT_FRAME_WIDTH, CANVAS_SELECT_FRAME_WIDTH, CANVAS_SELECT_FRAME_WIDTH);
	CPen penFrame = CreatePen(PS_DASH, CANVAS_SELECT_FRAME_WIDTH, CANVAS_SELECT_FRAME_COLOR);
	CPenHandle penOld = dc.SelectPen(penFrame);
	CBrushHandle brushOld = dc.SelectBrush((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.Rectangle(rtSelect);

	if(bCanSizing)
	{
		CRect rtMark;
		for(INT i=HT_POS_LEFT; i<HT_POS_IN_RECT; ++i)
		{
			if(GetSizeMarkRect(rtSelect, i, rtMark))
			{
				dc.FillSolidRect(&rtMark, CANVAS_SELECT_FRAME_COLOR);
			}
		}
	}

	dc.SelectBrush(brushOld);
	dc.SelectPen(penFrame);

}
BOOL CSCCanvasBaseDrawer::GetSizeMarkRect(CRect& rtFrame, DWORD dwPosition, CRect& rtMark)
{
	if(rtFrame.Width() < CANVAS_SELECT_SIZE_MARK_WIDTH * 2 	|| rtFrame.Height() < CANVAS_SELECT_SIZE_MARK_WIDTH * 2)
		return FALSE;
	
	BOOL bRet(FALSE);
	switch(dwPosition)
	{
	case HT_POS_LEFT_TOP:
		rtMark = CRect(rtFrame.left, rtFrame.top, rtFrame.left + 1, rtFrame.top + 1);
		bRet = TRUE;
		break;

	case HT_POS_LEFT_BOTTOM:
		rtMark = CRect(rtFrame.left, rtFrame.bottom - 1, rtFrame.left + 1, rtFrame.bottom);
		bRet = TRUE;
		break;

	case HT_POS_RIGHT_TOP:
		rtMark = CRect(rtFrame.right - 1 , rtFrame.top, rtFrame.right , rtFrame.top + 1);
		bRet = TRUE;
		break;

	case HT_POS_RIGHT_BOTTOM:
		rtMark = CRect(rtFrame.right - 1 , rtFrame.bottom - 1, rtFrame.right, rtFrame.bottom);
		bRet = TRUE;
		break;

	case HT_POS_LEFT:
		if(rtFrame.Height() > CANVAS_SELECT_SIZE_MARK_WIDTH * 2)
		{
			rtMark = CRect(rtFrame.left, rtFrame.top + rtFrame.Height() / 2, rtFrame.left + 1, rtFrame.top + rtFrame.Height() / 2 + 1);
			bRet = TRUE;
		}
		break;

	case HT_POS_TOP:
		if(rtFrame.Width() > CANVAS_SELECT_SIZE_MARK_WIDTH * 2)
		{
			rtMark = CRect(rtFrame.left + rtFrame.Width() / 2, rtFrame.top, rtFrame.left + rtFrame.Width() / 2 + 1, rtFrame.top + 1);
			bRet = TRUE;
			break;
		}
	
	case HT_POS_RIGHT:
		{
			rtMark = CRect(rtFrame.right - 1,  rtFrame.top + rtFrame.Height() / 2, rtFrame.right, rtFrame.top + rtFrame.Height() / 2 + 1);
			bRet = TRUE;
			break;
		}

	case HT_POS_BOTTOM:
		{
			rtMark = CRect(rtFrame.left + rtFrame.Width() / 2, rtFrame.bottom - 1, rtFrame.left + rtFrame.Width() / 2 + 1, rtFrame.bottom);
			bRet = TRUE;
			break;
		}

	default:
		return FALSE;
	}
	
	INT nAdd = CANVAS_SELECT_SIZE_MARK_WIDTH / 2;
	rtMark.InflateRect(nAdd, nAdd, nAdd, nAdd);

	return bRet;
}



VOID ValidateSelectRect_Fun(CRect& rtCheck, BOOL bKeepSize = TRUE)
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	_ASSERTE(!pData.IsNull());
	CSize sizeScreen = pData->GetScreenSize();

	INT nWidth = rtCheck.Width();
	INT nHeight = rtCheck.Height();
	if(nWidth > sizeScreen.cx) nWidth = sizeScreen.cx;
	if(nHeight > sizeScreen.cy) nHeight = sizeScreen.cy;


	if(rtCheck.left < 0)
	{
		rtCheck.left = 0;

		if(bKeepSize)
		{
			rtCheck.right =  nWidth;
		}
	}

	if(rtCheck.top < 0)
	{
		rtCheck.top = 0;

		if(bKeepSize)
		{
			rtCheck.bottom = nHeight;
		}
	}

	if(rtCheck.right > sizeScreen.cx)
	{
		rtCheck.right = sizeScreen.cx;

		if(bKeepSize)
		{
			rtCheck.left = rtCheck.right - nWidth;
		}
	}

	if(rtCheck.bottom > sizeScreen.cy)
	{
		rtCheck.bottom = sizeScreen.cy;

		if(bKeepSize)
		{
			rtCheck.top = rtCheck.bottom - nHeight;
		}
	}
}


VOID CSCSelectDrawer::Reset()
{
	m_ptStart = m_ptEnd = m_ptPrev = CPoint(0, 0);
	m_dwStatus = SELECT_STATUS_IDLE;
	m_dwHitPos = HT_POS_NONE;
}


CSCSelectDrawer::CSCSelectDrawer()
:m_rtOldSelect(0, 0, 0, 0)
{
	Reset();
}

VOID CSCSelectDrawer::DrawCurrentView(CDCHandle dc)
{
	DrawSelectFrame(dc, TRUE);
}

VOID CSCSelectDrawer::OnDrawEnd()
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	CRect rtNew = pData->GetSelectRect();
	
	if(rtNew == m_rtOldSelect) return;

	CRectChangeData rtData;
	rtData.rtNew = rtNew;
	rtData.rtOld = m_rtOldSelect;

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(WM_CANVAS_DRAW_EVENT, DRAW_EVENT_WPARAM_SELECT_CHANGED, (LPARAM)&rtData);
}

LRESULT CSCSelectDrawer::OnLButtonDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	CRect rtFrame = pData->GetSelectRect();
	
	m_rtOldSelect = rtFrame;

	SetCapture(pCanvas->m_hWnd);
	if(GetCapture() != pCanvas->m_hWnd) return S_OK;
	

	if(rtFrame.IsRectEmpty())
	{
		m_dwStatus = SELECT_STATUS_SELECTING;		
	}
	else
	{
		rtFrame.InflateRect(CANVAS_SELECT_FRAME_WIDTH, CANVAS_SELECT_FRAME_WIDTH, CANVAS_SELECT_FRAME_WIDTH, CANVAS_SELECT_FRAME_WIDTH);
		DWORD dwPos = HitTest(ptCursor);
		m_dwHitPos = dwPos;
		if(dwPos == HT_POS_NONE)
		{
			m_dwStatus = SELECT_STATUS_SELECTING;
		}
		else if(dwPos == HT_POS_IN_RECT)
		{
			m_dwStatus = SELECT_STATUS_MOVING;
		}
		else
		{
			m_dwStatus = SELECT_STATUS_SIZING;
		}	
	}

	m_ptStart = ptCursor;
	m_ptPrev = m_ptEnd = m_ptStart;

	
	return S_OK;
}

LRESULT CSCSelectDrawer::OnMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); 
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(GetCapture() != pCanvas->m_hWnd) 
	{
		return S_OK;
	}

	if(m_dwStatus == SELECT_STATUS_SELECTING)	
	{
		m_ptPrev = m_ptEnd;
		m_ptEnd = ptCursor;

		OutputSelectRect();
	}
	else if(m_dwStatus == SELECT_STATUS_MOVING)
	{
		m_ptPrev = m_ptEnd;
		m_ptEnd = ptCursor;		

		OutputMovingRect();
	}
	else if(m_dwStatus == SELECT_STATUS_SIZING)
	{
		m_ptPrev = m_ptEnd;
		m_ptEnd = ptCursor;		

		OutputSizingRect();
	}
	else
	{
		_ASSERTE(FALSE);
	}
	

	return S_OK;
}

LRESULT CSCSelectDrawer	::OnLButtonUp(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); 
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(GetCapture() != pCanvas->m_hWnd) 
	{
		Reset();
		return S_OK;
	}


	m_ptPrev = m_ptEnd;
	m_ptEnd = ptCursor;

	if(m_dwStatus == SELECT_STATUS_SELECTING)	
	{
		OutputSelectRect(TRUE);
	}
	else if(m_dwStatus == SELECT_STATUS_MOVING)
	{
		OutputMovingRect(TRUE);
	}	
	else if(m_dwStatus == SELECT_STATUS_SIZING)
	{
		OutputSizingRect(TRUE);
	}
	else
	{
		_ASSERTE(FALSE);
	}

	ReleaseCapture();
	Reset();

	return S_OK;
}
LRESULT CSCSelectDrawer::OnSetCursor(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	ScreenToClient(pCanvas->m_hWnd, &ptCursor);
	
	DWORD dwPos = HitTest(ptCursor);
	
	LPTSTR lpszCursor(0);
	switch(dwPos)
	{
	case HT_POS_LEFT:
	case HT_POS_RIGHT:
		lpszCursor = IDC_SIZEWE;
		break;	

	case HT_POS_BOTTOM:
	case HT_POS_TOP:
		lpszCursor = IDC_SIZENS;
		break;

	case HT_POS_LEFT_BOTTOM:
	case HT_POS_RIGHT_TOP:
		lpszCursor = IDC_SIZENESW;
		break;

	case HT_POS_LEFT_TOP:
	case HT_POS_RIGHT_BOTTOM:
		lpszCursor = IDC_SIZENWSE;
		break;

	case HT_POS_IN_RECT:
		lpszCursor = IDC_SIZEALL;
		break;
	}
	
	if(lpszCursor != NULL)
	{
		SetCursor(LoadCursor(NULL, lpszCursor));
	}
	else
	{
		CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
		SetCursor(pSkin->GetSelectCursor());
	}

	return TRUE;

}
DWORD CSCSelectDrawer::HitTest(CPoint& pt)
{
	DWORD dwRet(HT_POS_NONE);
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	CRect rtFrame = pData->GetSelectRect();
	if(rtFrame.IsRectEmpty()) return dwRet;

	rtFrame.InflateRect(CANVAS_SELECT_FRAME_WIDTH, CANVAS_SELECT_FRAME_WIDTH, CANVAS_SELECT_FRAME_WIDTH, CANVAS_SELECT_FRAME_WIDTH);
	CRect rtTemp;
	DWORD i;
	for( i=HT_POS_LEFT; i<HT_POS_IN_RECT; ++i)
	{
		if(GetSizeMarkRect(rtFrame, i, rtTemp))
		{
			if(PtInRect(&rtTemp, pt))
			{
				dwRet = i;
				break;
			}
		}
	}
	
	if(i == HT_POS_IN_RECT)
	{
		if(PtInRect(&rtFrame, pt))
		{
			dwRet = i;
		}
	}

	return dwRet;
}

VOID CSCSelectDrawer::OutputSizingRect(BOOL bEnd /* = FALSE */)
{
	CRefPtr<CSCCanvasData> pCanvasData = GetCanvasData();
	CRect rtSelect = pCanvasData->GetSelectRect();

	CRect rtSelectNew = rtSelect;
	switch(m_dwHitPos)
	{
		case HT_POS_LEFT:
			if(m_ptEnd.x <= rtSelect.right)
			{
				rtSelectNew.InflateRect(m_ptPrev.x - m_ptEnd.x, 0, 0, 0);
			}
			else
			{
				rtSelectNew.SetRect(rtSelect.right, rtSelect.top, m_ptEnd.x, rtSelect.bottom);
				m_dwHitPos = HT_POS_RIGHT;
			}
			break;

		case HT_POS_RIGHT:
			if(m_ptEnd.x >= rtSelect.left)
			{
				rtSelectNew.InflateRect(0, 0, m_ptEnd.x - m_ptPrev.x, 0);
			}
			else
			{
				rtSelectNew.SetRect(m_ptEnd.x, rtSelect.top, rtSelect.left, rtSelect.bottom);
				m_dwHitPos = HT_POS_LEFT;
			}
			break;	

		case HT_POS_BOTTOM:
			if(m_ptEnd.y >= rtSelect.top)
			{
				rtSelectNew.InflateRect(0, 0, 0, m_ptEnd.y - m_ptPrev.y);
			}
			else
			{
				rtSelectNew.SetRect(rtSelect.left, m_ptEnd.y, rtSelect.right, rtSelect.top);
				m_dwHitPos = HT_POS_TOP;
			}
			break;

		case HT_POS_TOP:
			if(m_ptEnd.y <= rtSelect.bottom)
			{
				rtSelectNew.InflateRect(0, m_ptPrev.y - m_ptEnd.y, 0, 0);
			}
			else
			{	
				rtSelectNew.SetRect(rtSelect.left, rtSelect.bottom, rtSelect.right, m_ptEnd.y );
				m_dwHitPos = HT_POS_BOTTOM;
			}
			break;

		case HT_POS_LEFT_BOTTOM:
			if(m_ptEnd.x <= rtSelect.right && m_ptEnd.y >= rtSelect.top)
			{
				rtSelectNew.InflateRect(m_ptPrev.x - m_ptEnd.x, 0, 0, m_ptEnd.y - m_ptPrev.y);
			}
			else if(m_ptEnd.x > rtSelect.right && m_ptEnd.y < rtSelect.top)
			{
				rtSelectNew.SetRect(rtSelect.right, m_ptEnd.y, m_ptEnd.x, rtSelect.top);
				m_dwHitPos = HT_POS_RIGHT_TOP;
			}
			else if(m_ptEnd.x > rtSelect.right)
			{
				rtSelectNew.SetRect(rtSelect.right, rtSelect.top, m_ptEnd.x, rtSelect.bottom);
				m_dwHitPos = HT_POS_RIGHT_BOTTOM;	
			}
			else if(m_ptEnd.y < rtSelect.top)
			{
				rtSelectNew.SetRect(rtSelect.left, m_ptEnd.y, rtSelect.right, rtSelect.top);
				m_dwHitPos = HT_POS_LEFT_TOP;
			}
			break;

		case HT_POS_RIGHT_TOP:
			if(m_ptEnd.x >= rtSelect.left && m_ptEnd.y <= rtSelect.bottom)
			{
				rtSelectNew.InflateRect(0, m_ptPrev.y - m_ptEnd.y, m_ptEnd.x - m_ptPrev.x, 0);
			}
			else if(m_ptEnd.x < rtSelect.left && m_ptEnd.y > rtSelect.bottom)
			{
				rtSelectNew.SetRect(m_ptEnd.x, rtSelect.bottom, rtSelect.left, m_ptEnd.y);
				m_dwHitPos = HT_POS_LEFT_BOTTOM;
			}
			else if(m_ptEnd.x < rtSelect.left)
			{
				rtSelectNew.SetRect(m_ptEnd.x, rtSelect.top, rtSelect.left, rtSelect.bottom);
				m_dwHitPos = HT_POS_LEFT_TOP;
			}
			else if(m_ptEnd.y > rtSelect.bottom)
			{
				rtSelectNew.SetRect(rtSelect.left, rtSelect.bottom, rtSelect.right, m_ptEnd.y );
				m_dwHitPos = HT_POS_RIGHT_BOTTOM;
			}
			break;

		case HT_POS_LEFT_TOP:
			if(m_ptEnd.x <= rtSelect.right && m_ptEnd.y <= rtSelect.bottom)
			{
				rtSelectNew.InflateRect(m_ptPrev.x - m_ptEnd.x, m_ptPrev.y - m_ptEnd.y, 0, 0);
			}
			else if(m_ptEnd.x > rtSelect.right && m_ptEnd.y > rtSelect.bottom)
			{
				rtSelectNew.SetRect(rtSelect.right, rtSelect.bottom, m_ptEnd.x, m_ptEnd.y);
				m_dwHitPos = HT_POS_RIGHT_BOTTOM;
			}
			else if(m_ptEnd.x > rtSelect.right)
			{
				rtSelectNew.SetRect(rtSelect.right, rtSelect.top, m_ptEnd.x, rtSelect.bottom);
				m_dwHitPos = HT_POS_RIGHT_TOP;
			}
			else if(m_ptEnd.y > rtSelect.bottom)
			{
				rtSelectNew.SetRect(rtSelect.left, rtSelect.bottom, rtSelect.right, m_ptEnd.y );
				m_dwHitPos = HT_POS_LEFT_BOTTOM;
			}
			break;

		case HT_POS_RIGHT_BOTTOM:
			if(m_ptEnd.x >= rtSelect.left && m_ptEnd.y >= rtSelect.top)
			{
				rtSelectNew.InflateRect(0, 0, m_ptEnd.x - m_ptPrev.x, m_ptEnd.y - m_ptPrev.y);
			}
			else if(m_ptEnd.x < rtSelect.left && m_ptEnd.y < rtSelect.top)
			{
				rtSelectNew.SetRect(m_ptEnd.x, m_ptEnd.y, rtSelect.left, rtSelect.top);
				m_dwHitPos = HT_POS_LEFT_TOP;
			}
			else if(m_ptEnd.x < rtSelect.left)
			{
				rtSelectNew.SetRect(m_ptEnd.x, rtSelect.top, rtSelect.left, rtSelect.bottom);
				m_dwHitPos = HT_POS_LEFT_BOTTOM;
			}
			else if(m_ptEnd.y < rtSelect.top)
			{
				rtSelectNew.SetRect(rtSelect.left, m_ptEnd.y, rtSelect.right, rtSelect.top);
				m_dwHitPos = HT_POS_RIGHT_TOP;
			}
			break;

		default:
			_ASSERTE(FALSE);
	}

	ValidateSelectRect_Fun(rtSelectNew);
	pCanvasData->SetSelectRect(rtSelectNew);
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();

	CRect* pRectDraw = NULL;
	
	CRect rtUnion(0, 0, 0, 0);
	if(bEnd)
	{
		rtUnion.UnionRect(&rtSelectNew, &rtSelect);
		int nAdd = CANVAS_SELECT_FRAME_WIDTH + CANVAS_SELECT_SIZE_MARK_WIDTH / 2;
		rtUnion.InflateRect(nAdd, nAdd, nAdd, nAdd);
		pRectDraw = &rtUnion;
	}

	pCanvas->RedrawCanvas(pRectDraw, TRUE);

	if(bEnd)
	{
		OnDrawEnd();
	}
}
VOID CSCSelectDrawer::OutputMovingRect(BOOL bEnd /* = FALSE */)
{
	CRefPtr<CSCCanvasData> pCanvasData = GetCanvasData();
	CRect rtSelect = pCanvasData->GetSelectRect();

	CRect rtSelectNew = rtSelect;
	rtSelectNew.OffsetRect(m_ptEnd.x - m_ptPrev.x, m_ptEnd.y - m_ptPrev.y);

	ValidateSelectRect_Fun(rtSelectNew);
	pCanvasData->SetSelectRect(rtSelectNew);
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();

	CRect* pRectDraw = NULL;
	
	CRect rtUnion(0, 0, 0, 0);
	if(bEnd)
	{
		rtUnion.UnionRect(&rtSelectNew, &rtSelect);
		int nAdd = CANVAS_SELECT_FRAME_WIDTH + CANVAS_SELECT_SIZE_MARK_WIDTH / 2;
		rtUnion.InflateRect(nAdd, nAdd, nAdd, nAdd);
		pRectDraw = &rtUnion;
	}

	pCanvas->RedrawCanvas(pRectDraw, TRUE);

	if(bEnd)
	{
		OnDrawEnd();
	}
}
VOID CSCSelectDrawer::OutputSelectRect(BOOL bEnd)
{
	CRefPtr<CSCCanvasData> pCanvasData = GetCanvasData();

	CRect rtPrev(/*m_ptStart, m_ptPrev*/pCanvasData->GetSelectRect());
	CRect rtSelect(m_ptStart, m_ptEnd);
	
	rtSelect.NormalizeRect();
	rtPrev.NormalizeRect();

	if(rtSelect.IsRectEmpty())
	{
		return;
	}
	
	ValidateSelectRect_Fun(rtSelect);
	pCanvasData->SetSelectRect(rtSelect);

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();

	CRect* pRectDraw = NULL;

	CRect rtUnion(0, 0, 0, 0);
	if(bEnd)
	{
		rtUnion.UnionRect(&rtPrev, &rtSelect);
		int nAdd = CANVAS_SELECT_FRAME_WIDTH + CANVAS_SELECT_SIZE_MARK_WIDTH / 2;
		rtUnion.InflateRect(nAdd, nAdd, nAdd, nAdd);
		pRectDraw = &rtUnion;
	}

	pCanvas->RedrawCanvas(pRectDraw, TRUE);

	if(bEnd)
	{
		OnDrawEnd();
	}
}


CSCWinSelectDrawer::CSCWinSelectDrawer()
{
	m_ptPrev = CPoint(0, 0);
}

VOID CSCWinSelectDrawer::OnGetControl()
{
	m_ptPrev = CPoint(0, 0);
	m_rtCurrent = m_rtPrev = CRect(0, 0, 0, 0);
}

VOID CSCWinSelectDrawer::OnLostControl()
{

}
VOID CSCWinSelectDrawer::OnDrawEnd()
{
	if(m_rtCurrent.IsRectEmpty()) return;

	CRect rtWin = m_rtCurrent;

	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	CRect rtOld = pData->GetSelectRect();
	
	if(rtOld == rtWin) return;

	pData->SetSelectRect(rtWin);

	CRectChangeData rtData;
	rtData.rtNew = rtWin;
	rtData.rtOld = rtOld;

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(WM_CANVAS_DRAW_EVENT, DRAW_EVENT_WPARAM_SELECT_CHANGED, (LPARAM)&rtData);
}

VOID CSCWinSelectDrawer::DrawCurrentView(CDCHandle dc)
{
	DrawSelectWinFrame(dc);
	CSCCanvasBaseDrawer::DrawCurrentView(dc);
}

LRESULT CSCWinSelectDrawer::OnMouseMove(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	const double c_min_dis_check = 5.0;
	CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	if(DistancePt2Pt(pt, m_ptPrev) < c_min_dis_check)
	{
		return S_OK;
	}


	//	for multi-monitor support
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(!pCanvas.IsNull() && pCanvas->IsWindow())
	{
		CRect rtWindow;
		pCanvas->GetWindowRect(&rtWindow);
		pt.Offset(rtWindow.left, rtWindow.top);
	}
	m_ptPrev = pt;

	CRect rtSelect = CSCWinSpy<CSCWinFilter>::GetInstance()->GetWinRectByPoint(pt, FALSE);
	//	for multi-monitor support
	if(!pCanvas.IsNull() && pCanvas->IsWindow()) 
	{
		CRect rtWindow;
		pCanvas->GetWindowRect(&rtWindow);
		rtSelect.OffsetRect(-rtWindow.left, -rtWindow.top);
	}	

	ValidateSelectRect_Fun(rtSelect, FALSE);
	if(rtSelect != m_rtCurrent)
	{
		m_rtPrev = m_rtCurrent;
		m_rtCurrent = rtSelect;

		DrawTrackGraph();
	}
	
	return S_OK;
}
VOID CSCWinSelectDrawer::DrawSelectWinFrame(CDCHandle dc)
{
	CRect rtWin;
	if(!m_rtCurrent.IsRectEmpty())
	{
		CRect rtWin = m_rtCurrent;
		CPen penFrame = CreatePen(PS_SOLID | PS_INSIDEFRAME, CANVAS_WIN_SELECT_FRAME_WIDTH, CANVAS_WIN_SELECT_FRAME_COLOR);
		CBrushHandle brushOld = dc.SelectBrush((HBRUSH)GetStockObject(NULL_BRUSH));
		CPenHandle penOld = dc.SelectPen(penFrame);
		dc.Rectangle(&rtWin);
		dc.SelectPen(penOld);
		dc.SelectBrush(brushOld);
	}
}
VOID CSCWinSelectDrawer::DrawTrackGraph()
{
	CRect rtPrev = m_rtPrev;
	CRect rtNow = m_rtCurrent;

	CRect rtDraw;
	rtDraw.UnionRect(&rtPrev, &rtNow);
	
	
	GetCanvasWindow()->RedrawCanvas(&rtDraw, TRUE);
}

LRESULT CSCWinSelectDrawer::OnLButtonDown(UINT, WPARAM, LPARAM, BOOL&)
{
	if(!m_rtCurrent.IsRectEmpty())
	{
		OnDrawEnd();

		GetCanvasWindow()->RedrawCanvas(NULL, TRUE);
	}

	return S_OK;
}
LRESULT CSCWinSelectDrawer::OnSetCursor(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	SetCursor(pSkin->GetSelectCursor());

	return TRUE;
}


CSCTwoPointDrawer::CSCTwoPointDrawer()
{
	Reset();
}

VOID CSCTwoPointDrawer::Reset()
{
	m_ptEnd = m_ptPrev = m_ptStart = CPoint(0, 0);
}
VOID CSCTwoPointDrawer::DestroyCurrentPen()
{
	if(!m_CurrentPen.IsNull()) m_CurrentPen.DeleteObject();
}
BOOL CSCTwoPointDrawer::ResetCurrentPen()
{
	DestroyCurrentPen();

	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pTa = pContext->GetCurrentToolData();
	_ASSERTE(pTa != NULL);

	m_CurrentPen = CreatePen(SC_PEN_DEF_STYLE, pTa->m_nWidth, pTa->m_clrTool);
	_ASSERTE(!m_CurrentPen.IsNull());

	return (!m_CurrentPen.IsNull());
}

VOID CSCTwoPointDrawer::DrawCurrentView(CDCHandle dc)
{
	_ASSERTE(FALSE);
}

VOID CSCTwoPointDrawer::DrawTrackGraph()
{
	CRect rt1(m_ptStart, m_ptPrev);
	CRect rt2(m_ptStart, m_ptEnd);
	
	rt1.NormalizeRect();
	rt2.NormalizeRect();

	CRect rtDraw;
	rtDraw.UnionRect(&rt1, &rt2);

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->RedrawCanvas(&rtDraw, TRUE);
}

LRESULT CSCTwoPointDrawer::OnLButtonDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	
	SetCapture(pCanvas->m_hWnd);
	if(GetCapture() != pCanvas->m_hWnd) return S_OK;

	m_ptEnd = m_ptPrev = m_ptStart = ptCursor;

	OnDrawStart();

	return S_OK;
}

LRESULT CSCTwoPointDrawer::OnMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(GetCapture() != pCanvas->m_hWnd) return S_OK;
	
	m_ptPrev = m_ptEnd;
	m_ptEnd = ptCursor;

	DrawTrackGraph();

	return S_OK;
}

LRESULT CSCTwoPointDrawer::OnLButtonUp(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(GetCapture() != pCanvas->m_hWnd) return S_OK;
	ReleaseCapture();

	m_ptPrev = m_ptEnd;
	m_ptEnd = ptCursor;

	DrawTrackGraph();

	OnDrawEnd();

	Reset();

	return S_OK;
}
LRESULT CSCTwoPointDrawer::OnCommand(UINT, WPARAM wParam, LPARAM, BOOL& bHandled ) 
{
	WORD nCmdID = LOWORD(wParam);
	
	if(nCmdID == SC_TOOL_COLOR_PICKER_ID
		|| nCmdID == SC_TOOL_SMALL_PEN_ID 
		|| nCmdID == SC_TOOL_MIDDLE_PEN_ID	
		|| nCmdID == SC_TOOL_BIG_PEN_ID)
	{
		ResetCurrentPen();
	}	

	bHandled = FALSE;

	return S_OK;
}

VOID CSCTwoPointDrawer::OnDrawEnd()	
{
	_ASSERTE(FALSE);
}

VOID CSCTwoPointDrawer::OnGetControl()	
{
	ResetCurrentPen();
}

VOID CSCTwoPointDrawer::OnLostControl()
{
	DestroyCurrentPen();
	Reset();
}

/////////////////////////////////////////////////

CSCMultiPointDrawer::CSCMultiPointDrawer()
{
	Reset();
}

VOID CSCMultiPointDrawer::Reset()
{
	m_arData.clear();
}
VOID CSCMultiPointDrawer::DestroyCurrentPen()
{
	if(!m_CurrentPen.IsNull()) m_CurrentPen.DeleteObject();
}
BOOL CSCMultiPointDrawer::ResetCurrentPen()
{
	DestroyCurrentPen();

	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pTa = pContext->GetCurrentToolData();
	_ASSERTE(pTa != NULL);

	m_CurrentPen = CreatePen(SC_PEN_DEF_STYLE, pTa->m_nWidth, pTa->m_clrTool);
	_ASSERTE(!m_CurrentPen.IsNull());

	return (!m_CurrentPen.IsNull());
}

VOID CSCMultiPointDrawer::DrawCurrentView(CDCHandle dc)
{
	_ASSERTE(FALSE);
}

VOID CSCMultiPointDrawer::DrawTrackGraph()
{
	_ASSERTE(FALSE);
}

LRESULT CSCMultiPointDrawer::OnLButtonDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	
	
	SetCapture(pCanvas->m_hWnd);
	if(GetCapture() != pCanvas->m_hWnd) return S_OK;

	m_arData.push_back(ptCursor);

	OnDrawStart();
	
	DrawTrackGraph();

	return S_OK;
}
LRESULT CSCMultiPointDrawer::OnCommand(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	WORD nCmdID = LOWORD(wParam);
	
	if(nCmdID == SC_TOOL_COLOR_PICKER_ID
		|| nCmdID == SC_TOOL_SMALL_PEN_ID 
		|| nCmdID == SC_TOOL_MIDDLE_PEN_ID	
		|| nCmdID == SC_TOOL_BIG_PEN_ID)
	{
		ResetCurrentPen();
	}	
	
	bHandled = FALSE;

	return S_OK;
}

LRESULT CSCMultiPointDrawer::OnMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(GetCapture() != pCanvas->m_hWnd) return S_OK;
	
	m_arData.push_back(ptCursor);

	DrawTrackGraph();

	return S_OK;
}

LRESULT CSCMultiPointDrawer::OnLButtonUp(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(GetCapture() != pCanvas->m_hWnd) return S_OK;
	ReleaseCapture();

	m_arData.push_back(ptCursor);
	
	DrawTrackGraph();

	OnDrawEnd();

	Reset();

	return S_OK;
}

VOID CSCMultiPointDrawer::OnDrawEnd()	
{
	_ASSERTE(FALSE);
}
VOID CSCMultiPointDrawer::OnGetControl()
{
	ResetCurrentPen();
}

VOID CSCMultiPointDrawer::OnLostControl()
{
	DestroyCurrentPen();
	Reset();
}

/////////////////////////////////////////////////




VOID CSCRectDrawer::DrawCurrentView(CDCHandle dc)
{
	if(m_ptStart != m_ptEnd)
	{
		CBrushHandle brushOld = dc.SelectBrush((HBRUSH) GetStockObject(NULL_BRUSH));
		CPenHandle penOld = dc.SelectPen(m_CurrentPen);
		CSCDrawRop2Checker ropChecker(dc);
		dc.Rectangle(m_ptStart.x, m_ptStart.y, m_ptEnd.x, m_ptEnd.y);	

		dc.SelectPen(penOld);
		dc.SelectBrush(brushOld);
	}

	CSCCanvasBaseDrawer::DrawCurrentView(dc);
}

VOID CSCRectDrawer::OnDrawEnd()	
{
	CRect rtDraw(m_ptStart, m_ptEnd);
	rtDraw.NormalizeRect();
	if(rtDraw.IsRectEmpty()) return;

	CRefPtr<CRectGraph> pRectGraph = new CRectGraph(m_ptStart, m_ptEnd);
	if(pRectGraph == NULL) return;

	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pTa = pContext->GetCurrentToolData();
	_ASSERTE(pTa != NULL);

	pRectGraph->m_clrFrame = pTa->m_clrTool;
	pRectGraph->m_nFrameWidth = pTa->m_nWidth;
	pRectGraph->SetGraphID(CSCGraphIDGen::GetUniqueID());
	
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(WM_CANVAS_DRAW_EVENT, DRAW_EVENT_WPARAM_DRAW_END, (LPARAM)pRectGraph.GetPtr());
}

VOID CSCEllipseDrawer::DrawCurrentView(CDCHandle dc)
{
	if(m_ptStart != m_ptEnd) 
	{
		CBrushHandle brushOld = dc.SelectBrush((HBRUSH) GetStockObject(NULL_BRUSH));
		CPenHandle penOld = dc.SelectPen(m_CurrentPen);
		
		CSCDrawRop2Checker ropChecker(dc);
		dc.Ellipse(m_ptStart.x, m_ptStart.y, m_ptEnd.x, m_ptEnd.y);	

		dc.SelectPen(penOld);
		dc.SelectBrush(brushOld);
	}

	CSCCanvasBaseDrawer::DrawCurrentView(dc);
}

VOID CSCEllipseDrawer::OnDrawEnd()	
{
	CRect rtDraw(m_ptStart, m_ptEnd);
	rtDraw.NormalizeRect();
	if(rtDraw.IsRectEmpty()) return;

	CRefPtr<CEllipseGraph> pGraph = new CEllipseGraph(m_ptStart, m_ptEnd);
	if(pGraph == NULL) return;

	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pTa = pContext->GetCurrentToolData();
	_ASSERTE(pTa != NULL);

	pGraph->m_clrFrame = pTa->m_clrTool;
	pGraph->m_nFrameWidth = pTa->m_nWidth;
	pGraph->SetGraphID(CSCGraphIDGen::GetUniqueID());
	
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(WM_CANVAS_DRAW_EVENT, DRAW_EVENT_WPARAM_DRAW_END, (LPARAM)pGraph.GetPtr());
}

VOID CSCLineDrawer::DrawCurrentView(CDCHandle dc)
{
	if(m_ptStart != m_ptEnd) 
	{
		CPenHandle penOld = dc.SelectPen(m_CurrentPen);
		
		CSCDrawRop2Checker ropChecker(dc);
		dc.MoveTo(m_ptStart);
		dc.LineTo(m_ptEnd);

		dc.SelectPen(penOld);
	}

	CSCCanvasBaseDrawer::DrawCurrentView(dc);
}

VOID CSCLineDrawer::OnDrawEnd()	
{
	if(m_ptStart == m_ptEnd) return;

	CRefPtr<CLineGraph> pGraph = new CLineGraph(m_ptStart, m_ptEnd);
	if(pGraph == NULL) return;

	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pTa = pContext->GetCurrentToolData();
	_ASSERTE(pTa != NULL);

	pGraph->m_clrLine = pTa->m_clrTool;
	pGraph->m_nWidth = pTa->m_nWidth;
	pGraph->SetGraphID(CSCGraphIDGen::GetUniqueID());
	
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(WM_CANVAS_DRAW_EVENT, DRAW_EVENT_WPARAM_DRAW_END, (LPARAM)pGraph.GetPtr());
}

VOID CSCLineDrawer::DrawTrackGraph()
{
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pTa = pContext->GetCurrentToolData();
	_ASSERTE(pTa != NULL);
	INT nPenWidth = pTa->m_nWidth;

	CRect rt1(m_ptStart, m_ptPrev);
	CRect rt2(m_ptStart, m_ptEnd);
	
	rt1.NormalizeRect();
	rt2.NormalizeRect();


	rt1.InflateRect(nPenWidth, nPenWidth, nPenWidth, nPenWidth);
	rt2.InflateRect(nPenWidth, nPenWidth, nPenWidth, nPenWidth);

	CRect rtDraw;
	rtDraw.UnionRect(&rt1, &rt2);

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->RedrawCanvas(&rtDraw, TRUE);
}

VOID CSCArrowDrawer::DrawCurrentView(CDCHandle dc)
{
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pTa = pContext->GetCurrentToolData();
	_ASSERTE(pTa != NULL);

	CArrowGraph graph(m_ptStart, m_ptEnd);
	graph.m_clrLine = pTa->m_clrTool;
	graph.m_nWidth = pTa->m_nWidth;
	graph.DrawGraph(dc);
	
	CSCCanvasBaseDrawer::DrawCurrentView(dc);
}

VOID CSCArrowDrawer::OnDrawEnd()	
{
	CRefPtr<CArrowGraph> pGraph = new CArrowGraph(m_ptStart, m_ptEnd);
	if(pGraph == NULL) return;

	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pTa = pContext->GetCurrentToolData();
	_ASSERTE(pTa != NULL);

	pGraph->m_clrLine = pTa->m_clrTool;
	pGraph->m_nWidth = pTa->m_nWidth;
	pGraph->SetGraphID(CSCGraphIDGen::GetUniqueID());

	if(pGraph->IsObjectEmpty()) return;
	
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(WM_CANVAS_DRAW_EVENT, DRAW_EVENT_WPARAM_DRAW_END, (LPARAM)pGraph.GetPtr());
}

VOID CSCArrowDrawer::DrawTrackGraph()
{
	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pTa = pContext->GetCurrentToolData();
	_ASSERTE(pTa != NULL);

	CRect rt1(m_ptStart, m_ptPrev);
	CRect rt2(m_ptStart, m_ptEnd);
	
	rt1.NormalizeRect();
	rt2.NormalizeRect();

	CArrowGraph graph(m_ptStart, m_ptEnd);
	graph.m_clrLine = pTa->m_clrTool;
	graph.m_nWidth = pTa->m_nWidth;
	INT nInflat = graph.GetArrowWidth();

	rt1.InflateRect(nInflat, nInflat, nInflat, nInflat);
	rt2.InflateRect(nInflat, nInflat, nInflat, nInflat);

	CRect rtDraw;
	rtDraw.UnionRect(&rt1, &rt2);

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->RedrawCanvas(&rtDraw, TRUE);
}



VOID CSCStrokeDrawer::DrawCurrentView(CDCHandle dc)
{
	CSCCanvasBaseDrawer::DrawCurrentView(dc);
}
LRESULT CSCStrokeDrawer::OnSetCursor(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	SetCursor(pSkin->GetPenCursor());

	return TRUE;
}
VOID CSCStrokeDrawer::OnDrawEnd()	
{
	int nCount = m_arData.size();
	if(nCount == 0) return;

	CRefPtr<CStrokeGraph> pGraph = new CStrokeGraph;
	if(pGraph == NULL) return;

	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pTa = pContext->GetCurrentToolData();
	_ASSERTE(pTa != NULL);

	pGraph->m_clrStroke = pTa->m_clrTool;
	pGraph->m_nWidth = pTa->m_nWidth;
	pGraph->SetGraphID(CSCGraphIDGen::GetUniqueID());

	for(int i=0; i<nCount; ++i)
	{
		pGraph->AddPoint(m_arData[i]);
	}
	
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(WM_CANVAS_DRAW_EVENT, DRAW_EVENT_WPARAM_DRAW_END, (LPARAM)pGraph.GetPtr());
}

VOID CSCStrokeDrawer::DrawTrackGraph()
{
	int nCount = m_arData.size();
	_ASSERTE(nCount > 0);
	int nPrevIndex(0);
	if(nCount >= 2)
	{
		nPrevIndex = nCount-2;
	}

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	_ASSERTE(!m_CurrentPen.IsNull());

	CClientDC dc(pCanvas->m_hWnd);
	CPenHandle OldPen = dc.SelectPen(m_CurrentPen);
	CSCDrawRop2Checker ropChecker(dc.m_hDC);
	dc.MoveTo(m_arData[nPrevIndex]);
	dc.LineTo(m_arData[nCount - 1]);

	dc.SelectPen(OldPen);
}


CSCTextDrawer::CSCTextDrawer()
{
}

VOID CSCTextDrawer::DrawCurrentView(CDCHandle dc)
{
	if(!m_pTextGraph->IsObjectEmpty())
	{
		m_pTextGraph->DrawGraph(dc);
	}

	CSCCanvasBaseDrawer::DrawCurrentView(dc);
}
VOID CSCTextDrawer::OnDrawStart()
{
	CSCCanvasBaseDrawer::OnDrawStart();

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	INT nHeight = m_pTextGraph->GetLineHeight();
	CreateCaret(pCanvas->m_hWnd, NULL, DEFAULT_CARET_WIDTH, nHeight);
	CPoint ptText = m_pTextGraph->m_ptText;
	SetCaretPos(ptText.x, ptText.y);
	ShowCaret(pCanvas->m_hWnd);	
	SetFocus(pCanvas->m_hWnd);
}

VOID CSCTextDrawer::OnDrawEnd()
{
	DestroyCaret();

	if(m_pTextGraph->IsObjectEmpty()) return;
	
	CRefPtr<CSCGraphObject> pGraph = m_pTextGraph->Clone();
	if(pGraph.IsNull()) return;

	pGraph->SetGraphID(CSCGraphIDGen::GetUniqueID());
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->SendMessage(WM_CANVAS_DRAW_EVENT, DRAW_EVENT_WPARAM_DRAW_END, (LPARAM)pGraph.GetPtr());
}	

VOID CSCTextDrawer::OnGetControl()
{
	ResetCurrentGraph();
}

VOID CSCTextDrawer::OnLostControl()
{
	OnDrawEnd();
	DeleteCurrentGraph();
}
VOID CSCTextDrawer::EmptyCurrentText()
{
	if(!m_pTextGraph.IsNull())
	{
		m_pTextGraph->SetText(_T(""));
	}
}

VOID CSCTextDrawer::DeleteCurrentGraph()
{
	if(!m_pTextGraph.IsNull())
	{
		m_pTextGraph = NULL;
	}
}
VOID CSCTextDrawer::ResetCurrentGraph()
{
	DeleteCurrentGraph();

	m_pTextGraph = new CTextGraph;
	if(m_pTextGraph.IsNull()) return;

	CRefPtr<CSCCanvasContext> pContext = GetCanvasContext();
	CANVAS_TOOL_DATA* pData = pContext->GetCurrentToolData();
	_ASSERTE(pData != NULL);

	LPLOGFONT pLogFnt = (LPLOGFONT)pData->m_dwUserData;
	_ASSERTE(pLogFnt != NULL);

	m_pTextGraph->m_clrText = pData->m_clrTool;
	m_pTextGraph->SetFont(*pLogFnt);
}

LRESULT CSCTextDrawer::OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL&)
{
	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	OnDrawEnd();
	EmptyCurrentText();

	int nHalfLine = m_pTextGraph->GetLineHeight() / 2;
	ptCursor.y -= nHalfLine;

	m_pTextGraph->m_ptText = ptCursor;	
	OnDrawStart();

	return S_OK;
}
LRESULT CSCTextDrawer::OnKillFocus(UINT, WPARAM, LPARAM, BOOL&)
{
	OnDrawEnd();
	EmptyCurrentText();

	return S_OK;
}

LRESULT CSCTextDrawer::OnCommand(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	WORD nCmdID = LOWORD(wParam);
	bHandled = FALSE;	

	 if(nCmdID == SC_TOOL_FONT_BOLD_ID
		|| nCmdID == SC_TOOL_FONT_SIZE_ID
		|| nCmdID == SC_TOOL_COLOR_PICKER_ID
		|| nCmdID == SC_TOOL_FONT_NAME_ID)
	{
		OnDrawEnd();
		ResetCurrentGraph();
	}
	else if(nCmdID == SC_TOOL_PLASTER_CMD_ID)
	{
		OnPlasterCmd();
	}
	else if(nCmdID == SC_TOOL_UNDO_CMD_ID)
	{
		if(	Undo()) bHandled = TRUE;
	}


	return S_OK;
}
BOOL CSCTextDrawer::GetClipboardText(CString& strText)
{
	BOOL bRet(FALSE);
	UINT uForamt(CF_UNICODETEXT);
	if(!IsClipboardFormatAvailable(uForamt)) return bRet;

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(!OpenClipboard(*pCanvas)) return bRet;

	HGLOBAL   hglb = GetClipboardData(uForamt);
	if(hglb != NULL)
	{
		LPCTSTR	lpszText = (LPCTSTR)GlobalLock(hglb);
		if(lpszText != NULL)
		{
			strText = lpszText;
			GlobalUnlock(hglb);
			bRet = TRUE;
		}
	}
	CloseClipboard();

	return bRet;
}
VOID CSCTextDrawer::OnPlasterCmd()
{
	CString strText;
	if(GetClipboardText(strText))
	{
		OnAddDraw(strText);
	}
}
VOID CSCTextDrawer::OnAddDraw(LPCTSTR lpszText)
{
	_ASSERTE(lpszText != NULL);
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	
	BOOL bEmpty = m_pTextGraph->IsObjectEmpty();

	m_pTextGraph->AddStr(lpszText);

	// check if it is out of screen
	// if out of screen,  add a line
	CRect rtClient;
	pCanvas->GetClientRect(&rtClient);
	CRect rtText = m_pTextGraph->GetCurrentTextRect();
	if(rtText.right > rtClient.right && !bEmpty)
	{
		m_pTextGraph->BackChar(lstrlen(lpszText));
		m_pTextGraph->AddChar(_T('\n'));
		m_pTextGraph->AddStr(lpszText);
	}
	//

	HideCaret(pCanvas->m_hWnd);

	CRect rtCurrent = m_pTextGraph->GetCurrentTextRect();
	pCanvas->RedrawCanvas(&rtCurrent, TRUE);	

	CPoint ptCaret = m_pTextGraph->GetLastCaretPos();
	if(ptCaret.x > rtCurrent.right)
	{
		ptCaret.x = rtClient.right;
	}

	SetCaretPos(ptCaret.x, ptCaret.y);
	ShowCaret(pCanvas->m_hWnd);
}

VOID CSCTextDrawer::OnBackDraw(int nBackCount)
{
	if(m_pTextGraph->IsObjectEmpty()) return;
	
	if(nBackCount == -1)
	{
		nBackCount = m_pTextGraph->GetTextLength();
	}

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();

	CRect rtCurrent = m_pTextGraph->GetCurrentTextRect();
	m_pTextGraph->BackChar(nBackCount);
	
	HideCaret(pCanvas->m_hWnd);
	pCanvas->RedrawCanvas(&rtCurrent, TRUE);
	
	CPoint ptCaret = m_pTextGraph->GetLastCaretPos();
	SetCaretPos(ptCaret.x, ptCaret.y);
	ShowCaret(pCanvas->m_hWnd);
}

LRESULT CSCTextDrawer::OnChar(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	TCHAR ch = (TCHAR)wParam;

	switch(ch)
	{
		case _T('\n'):
		case _T('\r'):
			ch = _T('\n');
			break;

		default:
			break;
	}
	
	if(ch == 0x0008) //back space
	{
		OnBackDraw(1);
	}
	else if(ch == 3) //drag the mouse on Win7
	{

	}
	else
	{
		TCHAR szText[2] = { ch, 0};
		LPCTSTR lpszAdd = szText;
		if(ch == _T('\t')) //tab
		{
			lpszAdd = _T("      "); 
		}

		OnAddDraw(lpszAdd);
	}

	return S_OK;
}

LRESULT CSCTextDrawer::OnSetCursor(UINT, WPARAM, LPARAM, BOOL&)
{
	SetCursor(LoadCursor(NULL, IDC_IBEAM));

	return TRUE;
}
BOOL CSCTextDrawer::Undo()
{
	if(!m_pTextGraph->IsObjectEmpty())
	{
		OnBackDraw(-1);
		return TRUE;
	}

	return FALSE;
}

CSCEraserDrawer::CSCEraserDrawer()
:m_ptEraser(0, 0)
{

}

VOID CSCEraserDrawer::OnDrawEnd()
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	int nCount = pData->GetGraphCount();
	
	DWORD dwGraphID(0);
	for(INT i=nCount-1; i>=0; --i)
	{
		CRefPtr<CSCGraphObject> pGraph = pData->GetGraphByIndex(i);
		_ASSERTE(!pGraph.IsNull());

		if(pGraph->HitTest(m_ptEraser))
		{
			dwGraphID = pGraph->GetGraphID();
			break;
		}
	}

	if(dwGraphID != 0)
	{
		CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
		pCanvas->SendMessage(WM_CANVAS_DRAW_EVENT, DRAW_EVENT_WPARAM_ERASER, (LPARAM)dwGraphID);
	}
}

LRESULT CSCEraserDrawer::OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL&)
{
	CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	m_ptEraser = ptCursor;

	OnDrawEnd();

	return S_OK;
}

LRESULT CSCEraserDrawer::OnSetCursor(UINT, WPARAM, LPARAM, BOOL&)
{
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();

	SetCursor(pSkin->GetEraserCursor());

	return TRUE;
}