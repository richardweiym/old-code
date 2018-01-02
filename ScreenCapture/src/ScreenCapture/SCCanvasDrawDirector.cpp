#include "HYUtilBase.h"
#include "SCCanvasDrawDirector.h"
#include "SCCanvasWindow.h"
#include "SCCanvasData.h"
#include "SCCanvasApp.h"
#include "SCCanvasSkin.h"
#include "HYUIBase.h"
#include "SCCanvasDrawer.h"
#include "SCCanvasContext.h"
#include "HYTimer.h"
#include "SCCanvasGraph.h"

CSCCanvasDrawDirector::CSCCanvasDrawDirector()
{
	m_clrShadow = RGB(0, 0, 255);
}

CSCCanvasDrawDirector::~CSCCanvasDrawDirector()
{
		
}
LRESULT CSCCanvasDrawDirector::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	LoadResource();

	return S_OK;
}

LRESULT CSCCanvasDrawDirector::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	CRefPtr<CSCCanvasWindow> pCanvasWindow = GetCanvasWindow();

	CPaintDC dc(pCanvasWindow->m_hWnd);
	CHYMemDC memDC(dc);

#ifdef _DEBUG
	CHYTimeCostCalculator timer(_T("CSCCanvasDrawDirector::OnPaint()"));
#endif //_DEBUG

	DrawScreenSnapshot(memDC.m_hDC);
	DrawCursor(memDC.m_hDC);
	DrawAnnotation(memDC.m_hDC);
	DrawToolView(memDC.m_hDC);	
	
	DrawSelectShadow(memDC.m_hDC);
	

	return S_OK;
}
LRESULT CSCCanvasDrawDirector::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&)
{
	return TRUE;
}

VOID CSCCanvasDrawDirector::DrawScreenSnapshot(CDCHandle dc)
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	_ASSERTE(!pData.IsNull());

	CBitmapHandle bmpScreen = pData->GetScreenBmp();
	_ASSERTE(!bmpScreen.IsNull());

	CSize sizeScreen = pData->GetScreenSize();
	
	CRect rtDraw(0, 0, sizeScreen.cx, sizeScreen.cy);
	CHYUICommonFunctionWrapper::DrawBmp(dc, &rtDraw, bmpScreen);
}

VOID CSCCanvasDrawDirector::DrawAnnotation(CDCHandle dc)
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	int nCount = pData->GetGraphCount();
	for(int i=0; i<nCount; ++i)
	{
		CRefPtr<CSCGraphObject> pGraph = pData->GetGraphByIndex(i);
		_ASSERTE(!pGraph.IsNull());
		
		pGraph->DrawGraph(dc);
	}
}
VOID CSCCanvasDrawDirector::LoadResource()
{
	if(!m_bmpWaterMark.IsNull())
	{
		m_bmpWaterMark.DeleteObject();
	}
	
	CRefPtr<CSCCanvasSkin> pSkin = GetCanvasSkin();
	_ASSERTE(!pSkin.IsNull());

	m_bmpWaterMark = pSkin->GetWaterMark();
	_ASSERTE(!m_bmpWaterMark.IsNull());
	
	m_clrShadow = GetCanvasSkin()->GetCanvasShadowColor();
}


VOID CSCCanvasDrawDirector::DrawWaterMark(CDCHandle memDC)
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	_ASSERTE(!pData.IsNull());

	CSize sizeScreen = pData->GetScreenSize();
	CRect rtDraw;

	if(!m_bmpWaterMark.IsNull()) //draw water mark
	{
		CSize sizeWaterMark = CHYUICommonFunctionWrapper::GetBitmapSize(m_bmpWaterMark);
		int nX = sizeScreen.cx - sizeWaterMark.cx - WATER_MARK_X_OFFSET;
		if(nX < 0) nX = 0;
		int nY = sizeScreen.cy - sizeWaterMark.cy - WATER_MARK_Y_OFFSET;
		if(nY < 0) nY = 0;
		rtDraw.SetRect(nX, nY, nX + sizeWaterMark.cx, nY + sizeWaterMark.cy);
		CHYUICommonFunctionWrapper::DrawBmp((HDC)memDC, &rtDraw, (HBITMAP)m_bmpWaterMark, FALSE, TRUE);
	}
	else
	{
		_ASSERTE(FALSE);
	}
} 
VOID CSCCanvasDrawDirector::DrawSelectShadow(CDCHandle dc)
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	_ASSERTE(!pData.IsNull());

	CSize sizeScreen = pData->GetScreenSize();
	CRect rtSelect = pData->GetSelectRect();

	CDC memDC = CreateCompatibleDC(dc);
	
	CBitmap bmpSelect;
	CBitmapHandle bmpOld;
	if(!rtSelect.IsRectEmpty()) //save select bitmap
	{
		bmpSelect = CreateCompatibleBitmap(dc, rtSelect.Width(), rtSelect.Height());
		bmpOld = memDC.SelectBitmap(bmpSelect);
		memDC.BitBlt(0, 0, rtSelect.Width(), rtSelect.Height(), dc, rtSelect.left, rtSelect.top, SRCCOPY);
		memDC.SelectBitmap(bmpOld);
	}

	//draw Covered shadow
	//if(!m_bmpShadow.IsNull() || LoadShadowBMP())
	{
		CBitmap bmpTemp = CreateCompatibleBitmap(dc, sizeScreen.cx, sizeScreen.cy);
		bmpOld = memDC.SelectBitmap(bmpTemp);
		CRect rtDraw(0, 0, sizeScreen.cx, sizeScreen.cy);
		memDC.FillSolidRect(&rtDraw, m_clrShadow);
		//CHYUICommonFunctionWrapper::FillBmp(memDC.m_hDC, &rtDraw, (HBITMAP)m_bmpShadow); //performance is poor
		
		DrawWaterMark((HDC)memDC);

		BLENDFUNCTION bf = {0};
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = 50;
		AlphaBlend(dc, 0, 0, sizeScreen.cx, sizeScreen.cy, memDC, 0, 0, sizeScreen.cx, sizeScreen.cy, bf);
		memDC.SelectBitmap(bmpOld);
	}

	if(!bmpSelect.IsNull()) //copy select bitmap
	{
		bmpOld = memDC.SelectBitmap(bmpSelect);
		dc.BitBlt(rtSelect.left, rtSelect.top, rtSelect.Width(), rtSelect.Height(), memDC, 0, 0, SRCCOPY);
		memDC.SelectBitmap(bmpOld);
	}
}

VOID CSCCanvasDrawDirector::DrawToolView(CDCHandle dc)
{
	if(!m_pCurrentDrawer.IsNull())	
	{
		m_pCurrentDrawer->DrawCurrentView(dc);
	}
}


VOID CSCCanvasDrawDirector::OnSkinChanged(DWORD dwType)
{
	LoadResource();
}

VOID CSCCanvasDrawDirector::CheckCurrentDrawer()
{
	CRefPtr<CSCCanvasContext> pCanvasContext = GetCanvasContext();
	_ASSERTE(!pCanvasContext.IsNull());

	if(!pCanvasContext.IsNull())
	{
		if(!m_pCurrentDrawer.IsNull())
		{
			m_pCurrentDrawer->OnLostControl();
		}

		DWORD eToolType = pCanvasContext->GetToolType();
		m_pCurrentDrawer = CSCDrawerFactory::GetDrawerByToolType(eToolType);

		if(!m_pCurrentDrawer.IsNull())
		{
			m_pCurrentDrawer->OnGetControl();
		}
	}

}

CRefPtr<CSCCanvasBaseDrawer> CSCCanvasDrawDirector::GetCurrentDrawer() const
{
	return m_pCurrentDrawer;
}

LRESULT CSCCanvasDrawDirector::OnMouseMove(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	return S_OK;
}

VOID CSCCanvasDrawDirector::DrawCursor(CDCHandle dc)
{
	if(!GetCanvasContext()->GetShowCursorFlag()) return;

	const CSCCursorInfo* pInfo = GetCanvasData()->GetCursorData();
	_ASSERTE(pInfo != NULL);

	_ASSERTE(!pInfo->fIcon); //it should be a cursor
	if( pInfo->hbmMask != NULL)
	{	
		CPoint ptLeftTop = pInfo->m_ptCursor;
		ptLeftTop.Offset(-pInfo->xHotspot, -pInfo->yHotspot);

		//	for multi-monitor support
		CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
		if(!pCanvas.IsNull() && pCanvas->IsWindow())
		{
			CRect rtWindow;
			pCanvas->GetWindowRect(&rtWindow);
			ptLeftTop.Offset(-rtWindow.left, -rtWindow.top);
		}
		CSize sizMask = CHYUICommonFunctionWrapper::GetBitmapSize(pInfo->hbmMask);
		
		if(pInfo->hbmColor == NULL) //black and white cursor
		{
			CDC dcMem = CreateCompatibleDC(dc);
			CBitmapHandle bmpOld = dcMem.SelectBitmap(pInfo->hbmMask);
			dc.BitBlt(ptLeftTop.x, ptLeftTop.y, sizMask.cx, sizMask.cy / 2, dcMem, 0, 0, SRCAND);
			dc.BitBlt(ptLeftTop.x, ptLeftTop.y, sizMask.cx, sizMask.cy / 2, dcMem, 0, sizMask.cy / 2, SRCINVERT);
			dcMem.SelectBitmap(bmpOld);
		}
		else //color cursor
		{
			CDC dcMem = CreateCompatibleDC(dc);
			CBitmapHandle bmpOld = dcMem.SelectBitmap(pInfo->hbmMask);
			dc.BitBlt(ptLeftTop.x, ptLeftTop.y, sizMask.cx, sizMask.cy, dcMem, 0, 0, SRCAND);
			dcMem.SelectBitmap(pInfo->hbmColor);
			dc.BitBlt(ptLeftTop.x, ptLeftTop.y, sizMask.cx, sizMask.cy, dcMem, 0, 0, SRCINVERT);
			dcMem.SelectBitmap(bmpOld);			
		}
	}
	else
	{
		_ASSERTE(FALSE);
	}
}

VOID CSCCanvasDrawDirector::RedrawCopyDataDC(BOOL bCopyFromCanvas /* = TRUE */)
{
#ifdef _DEBUG
	CHYTimeCostCalculator timer(_T("CSCCanvasDrawDirector::RedrawCopyDataDC()"));
#endif //_DEBUG
	
	CClientDC dcSrc(GetCanvasWindow()->m_hWnd);

	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	_ASSERTE(!pData.IsNull());

	if(!pData->InitCopyDataDC(dcSrc.m_hDC)) 
	{
		_ASSERTE(FALSE);
		return;
	}
	
	CSize sizeScreen = pData->GetScreenSize();

	CDCHandle dc = pData->GetCopyDataDC();

	if(bCopyFromCanvas) //include shadow, current cursor and float tool window
	{
		dc.BitBlt(0, 0, sizeScreen.cx, sizeScreen.cy, dcSrc, 0, 0, SRCCOPY);
		DrawCurrentCursor(dc);
	}
	else
	{
		DrawScreenSnapshot(dc);
		DrawCursor(dc);
		DrawAnnotation(dc);	
	}
}

VOID CSCCanvasDrawDirector::DrawCurrentCursor(CDCHandle dc)
{
	CURSORINFO ci = {0};
	ci.cbSize = sizeof(ci);
	
	if(!GetCursorInfoFun(&ci))
	{
		_ASSERTE(FALSE);
		return;
	}

	if(ci.flags != CURSOR_SHOWING)
	{
		return;
	}

	HCURSOR hCursor = ci.hCursor;
	_ASSERTE(hCursor != NULL);
	
	ICONINFO iconInfo = {0};
	if(!GetIconInfo(hCursor, &iconInfo))
	{
		_ASSERTE(FALSE);
		return;
	}

	CPoint ptCursor = ci.ptScreenPos;
	//	for multi-monitor support
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(!pCanvas.IsNull() && pCanvas->IsWindow())
	{
		CRect rtWindow;
		pCanvas->GetWindowRect(&rtWindow);
		ptCursor.Offset(-rtWindow.left, -rtWindow.top);
	}

	ptCursor.Offset(-iconInfo.xHotspot, -iconInfo.yHotspot);

	dc.DrawIcon(ptCursor.x, ptCursor.y, hCursor);

	if(iconInfo.hbmColor != NULL)
	{
		DeleteObject(iconInfo.hbmColor);
	}
	if(iconInfo.hbmMask != NULL)
	{
		DeleteObject( iconInfo.hbmMask );
	}
}