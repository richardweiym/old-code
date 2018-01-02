#include "SCCanvasGraph.h"
#include "SCIDDef.h"
#include "math.h"
#include "SCCanvasContext.h"
#include "SCInstanceAPI.h"


using namespace std;

double DistancePt2Pt(CPoint& pt1, CPoint& pt2)
{
	return	sqrt(pow((double)pt1.x - pt2.x, 2) + pow((double)pt1.y - pt2.y, 2));	
}

double DistancePt2Line(CPoint& ptStart, CPoint& ptEnd, CPoint& ptTest)
{
	double dbRet(0.0);
	if(ptStart == ptEnd)
	{
		dbRet = DistancePt2Pt(ptStart, ptTest);
	}
	else if(ptEnd.x == ptStart.x)
	{
		dbRet = abs(ptTest.x - ptStart.x);
	}
	else
	{
		double dbK = (ptEnd.y - ptStart.y) / (double)(ptEnd.x - ptStart.x);
		double dbNum = dbK * ptTest.x - ptTest.y - dbK * ptStart.x + ptStart.y;
		dbNum = (dbNum >=0 ? dbNum : -dbNum);
		double dbDem = sqrt(1 + pow(dbK, 2));
		dbRet = dbNum / dbDem;
	}

	return dbRet;
}

CSCDrawRop2Checker::CSCDrawRop2Checker(CDCHandle dc)
{
	m_dc = dc;

	CRefPtr<CSCCanvasContext> pCanvasContext = GetCanvasContext();
	
	DWORD dwRop2 = R2_COPYPEN;
	if(pCanvasContext->GetTransparentMark())
	{
		dwRop2 = R2_MASKPEN;
	}
	m_dwOldRop2 = m_dc.SetROP2(dwRop2);
}

CSCDrawRop2Checker::~CSCDrawRop2Checker()
{
	m_dc.SetROP2(m_dwOldRop2);
}


DWORD CSCGraphIDGen::GetUniqueID()
{
	return GetTickCount();
}

CSCGraphObject::CSCGraphObject()
{
	m_dwID = 0;
}

CSCGraphObject::~CSCGraphObject()
{
	NULL;
}



CRectGraph::CRectGraph()
{
	m_rect.SetRectEmpty();
	m_nFrameWidth = 0;
	m_clrFrame = RGB(0, 0, 0);
}

CRectGraph::CRectGraph(int x1, int y1, int x2, int y2)
:m_rect(x1, y1, x2, y2)
{
	m_rect.NormalizeRect();
	m_nFrameWidth = 0;
	m_clrFrame = RGB(0, 0, 0);	
}

CRectGraph::CRectGraph(CPoint ptStart, CPoint ptEnd)
:m_rect(ptStart, ptEnd)
{
	m_rect.NormalizeRect();
	m_nFrameWidth = 0;
	m_clrFrame = RGB(0, 0, 0);	
}


BOOL CRectGraph::HitTest(CPoint pt)
{
	int nWidth = m_nFrameWidth;
	nWidth += ERASER_DIST_COUNT;

	if(m_rect.Width() <= nWidth * 2 
		|| m_rect.Height() <= nWidth * 2)
	{
		return m_rect.PtInRect(pt);
	}
	else
	{
		CRect rtTemp(m_rect);
		rtTemp.InflateRect(-nWidth, -nWidth);

		return (m_rect.PtInRect(pt) && !rtTemp.PtInRect(pt));
	}
}

BOOL CRectGraph::IsObjectEmpty()
{
	return m_rect.IsRectEmpty();
}

CSCGraphObject* CRectGraph::Clone()
{
	CSCGraphObject* pRet =  new CRectGraph(*this);
	if(pRet != NULL)
	{
		pRet->ResetRefCount();
	}

	return pRet;
}

VOID CRectGraph::DrawGraph(CDCHandle dc)
{
	if(IsObjectEmpty()) return;
	_ASSERTE(m_nFrameWidth > 0);
	
	CPen newPen = CreatePen(SC_PEN_DEF_STYLE, m_nFrameWidth, m_clrFrame);
	CPenHandle oldPen = dc.SelectPen(newPen);
	CBrushHandle oldBrush = dc.SelectBrush((HBRUSH)GetStockObject(NULL_BRUSH));
	CSCDrawRop2Checker ropChecker(dc);
	dc.Rectangle(m_rect);
	dc.SelectBrush(oldBrush);
	dc.SelectPen(oldPen);
}



CEllipseGraph::CEllipseGraph()
:CRectGraph()
{
	NULL;
}

CEllipseGraph::CEllipseGraph(int x1, int y1, int x2, int y2)
:CRectGraph(x1, y1, x2, y2)
{
	NULL;
}

CEllipseGraph::CEllipseGraph(CPoint ptStart, CPoint ptEnd)
:CRectGraph(ptStart, ptEnd)
{
	NULL;
}


BOOL CEllipseGraph::HitTest(CPoint ptTest)
{
	BOOL bRet(FALSE);
	
	CRect rtGraph(m_rect);
	INT nWidth = m_nFrameWidth;

	rtGraph.InflateRect(ERASER_DIST_COUNT, ERASER_DIST_COUNT);
	nWidth += ERASER_DIST_COUNT;
	
	CRgn rgnOut;
	rgnOut.CreateEllipticRgn(rtGraph.left, rtGraph.top, rtGraph.right, rtGraph.bottom);

	if(rtGraph.Width() <= nWidth * 2
		|| rtGraph.Height() <= nWidth * 2)
	{
		bRet = rgnOut.PtInRegion(ptTest);
	}
	else
	{
		if(rtGraph.PtInRect(ptTest))
		{
			CRect rtInner(rtGraph);
			rtInner.InflateRect(-nWidth, -nWidth);
			CRgn rgnInner;
			rgnInner.CreateEllipticRgn(rtInner.left, rtInner.top, rtInner.right, rtInner.bottom);
			
			CRgn rgnCheck;
			rgnCheck.CreateRectRgn(0, 0, 0, 0);
			rgnCheck.CombineRgn(rgnOut, rgnInner, RGN_DIFF);

			if(rgnCheck.PtInRegion(ptTest))
			{
				bRet = TRUE;
			}
		}
	}

	return bRet;
}


CSCGraphObject* CEllipseGraph::Clone()
{
	CSCGraphObject* pRet =  new CEllipseGraph(*this);
	if(pRet != NULL)
	{
		pRet->ResetRefCount();
	}

	return pRet;
}

VOID CEllipseGraph::DrawGraph(CDCHandle dc)
{
	if(IsObjectEmpty()) return;
	_ASSERTE(m_nFrameWidth > 0);
	
	CPen newPen = CreatePen(SC_PEN_DEF_STYLE, m_nFrameWidth, m_clrFrame);
	CPenHandle oldPen = dc.SelectPen(newPen);
	CBrushHandle oldBrush = dc.SelectBrush((HBRUSH)GetStockObject(NULL_BRUSH));
	CSCDrawRop2Checker ropChecker(dc);
	dc.Ellipse(m_rect);
	dc.SelectBrush(oldBrush);
	dc.SelectPen(oldPen);
}


CLineGraph::CLineGraph()
:m_ptStart(0, 0), m_ptEnd(0, 0)
{
	m_clrLine = RGB(0, 0, 0);
	m_nWidth = 0;
}

CLineGraph::CLineGraph(CPoint ptStart, CPoint ptEnd)
:m_ptStart(ptStart), m_ptEnd(ptEnd)
{
	m_clrLine = RGB(0, 0, 0);
	m_nWidth = 0;
}

BOOL CLineGraph::IsObjectEmpty()
{
	return (m_ptStart == m_ptEnd);
}

BOOL CLineGraph::HitTest(CPoint ptTest)
{
	BOOL bRet(FALSE);
	
	CPoint ptStart = m_ptStart;
	CPoint ptEnd = m_ptEnd;
	CRect rtRet( ptStart, ptEnd);

	rtRet.NormalizeRect();

	int nWidth = m_nWidth / 2;
	double dbRadius = nWidth;
	dbRadius += ERASER_DIST_COUNT;

	INT nInflat(nWidth);	

	if(nWidth == 0)
	{
		nInflat += 1;
		dbRadius += 1.0;
	}

	rtRet.InflateRect(nInflat, nInflat);
	if(rtRet.PtInRect(ptTest))
	{
		if(DistancePt2Line(ptStart, ptEnd, ptTest) <= dbRadius)	
		{
			bRet = TRUE;
		}
	}

	return bRet;
}

CSCGraphObject* CLineGraph::Clone()
{
	CSCGraphObject* pRet = new CLineGraph(*this);
	if(pRet != NULL)
	{
		pRet->ResetRefCount();
	}

	return pRet;
}

VOID CLineGraph::DrawGraph(CDCHandle dc)
{
	if(IsObjectEmpty()) return;
	_ASSERTE(m_nWidth > 0);
	
	CPen newPen = CreatePen(SC_PEN_DEF_STYLE, m_nWidth, m_clrLine);
	CPenHandle oldPen = dc.SelectPen(newPen);
	CSCDrawRop2Checker ropChecker(dc);
	dc.MoveTo(m_ptStart);
	dc.LineTo(m_ptEnd);
	dc.SelectPen(oldPen);
}



CArrowGraph::CArrowGraph(CPoint ptStart, CPoint ptEnd)
:CLineGraph(ptStart, ptEnd)
{

}
BOOL CArrowGraph::IsObjectEmpty()
{
	if(m_ptStart == m_ptEnd) return TRUE;

	double dbMin = Get2PtMinDistance();
	if(DistancePt2Pt(m_ptStart, m_ptEnd) < dbMin) return TRUE;

	return FALSE;
}
BOOL  CArrowGraph::HitTest(CPoint pt)
{
	if(CLineGraph::HitTest(pt)) return TRUE;

	CPoint ptArrow1(0, 0), ptArrow2(0, 0);
	if(!GetArrowPoint(ptArrow1, ptArrow2)) return FALSE;

	CPoint arPt[3];
	arPt[0] = m_ptEnd;
	arPt[1] = ptArrow1;
	arPt[2] = ptArrow2;

	CRgn rgn = CreatePolygonRgn(arPt, sizeof(arPt) / sizeof(arPt[0]), WINDING);
	if(::PtInRegion(rgn, pt.x, pt.y)) return TRUE;
	
	return FALSE;
}

VOID CArrowGraph::DrawGraph(CDCHandle dc)
{
	if(IsObjectEmpty()) return;

	CPoint ptArrow1(0, 0), ptArrow2(0, 0);
	if(!GetArrowPoint(ptArrow1, ptArrow2)) return;

	CPoint arPt[3];
	arPt[0] = m_ptEnd;
	arPt[1] = ptArrow1;
	arPt[2] = ptArrow2;

	CPen newPen = CreatePen(PS_SOLID, m_nWidth, m_clrLine);
	CBrush newBrush = CreateSolidBrush(m_clrLine);
	CPenHandle oldPen = dc.SelectPen(newPen);
	CBrushHandle oldBrush = dc.SelectBrush(newBrush);
	CSCDrawRop2Checker ropChecker(dc);
	dc.MoveTo(m_ptStart);
	dc.LineTo(m_ptEnd);
	dc.Polygon(arPt, sizeof(arPt) / sizeof(arPt[0]));
	dc.SelectBrush(oldBrush);
	dc.SelectPen(oldPen);
}


CSCGraphObject* CArrowGraph::Clone()
{
	CSCGraphObject* pRet = new CArrowGraph(*this);
	if(pRet != NULL)
	{
		pRet->ResetRefCount();
	}

	return pRet;
}

INT CArrowGraph::GetArrowWidth()
{
	_ASSERTE(m_nWidth != 0);
	
	INT nWidth = m_nWidth * 4;
	if(nWidth < 10) nWidth = 10;

	return nWidth;
}


double CArrowGraph::Get2PtMinDistance()
{
	double dbFrameLen = GetArrowWidth();
	return dbFrameLen / 2 * sqrt(3.0);
}

BOOL CArrowGraph::GetArrowPoint( CPoint& pt1, CPoint& pt2)
{
	if(IsObjectEmpty()) return FALSE;

	double dbMin = Get2PtMinDistance();
	
	int nArrowWidth = GetArrowWidth();
	
	if(m_ptStart.x == m_ptEnd.x)
	{
		pt1.x = m_ptStart.x - nArrowWidth / 2;
		pt2.x = m_ptStart.x + nArrowWidth / 2;;

		if(m_ptEnd.y > m_ptStart.y)
		{	
			pt1.y = m_ptEnd.y - dbMin;
			pt2.y = m_ptEnd.y - dbMin;
		}
		else
		{
			pt1.y = m_ptEnd.y + dbMin;
			pt2.y = m_ptEnd.y + dbMin;
		}
	}
	else if(m_ptStart.y == m_ptEnd.y)
	{
		pt1.y = m_ptStart.y - nArrowWidth / 2;
		pt2.y = m_ptStart.y + nArrowWidth / 2;;

		if(m_ptEnd.x > m_ptStart.x)
		{	
			pt1.x = m_ptEnd.x - dbMin;
			pt2.x = m_ptEnd.x - dbMin;
		}
		else
		{
			pt1.x = m_ptEnd.x + dbMin;
			pt2.x = m_ptEnd.x + dbMin;
		}
	}
	else
	{
		 double k = (double)(m_ptStart.y - m_ptEnd.y) / (m_ptStart.x - m_ptEnd.x);
		 double c = sqrt(1.0 + pow(k, 2));

		 double tempX = m_ptEnd.x + sqrt(3.0) * nArrowWidth / 2.0 / c;
		 double tempY = m_ptEnd.y + sqrt(3.0) * nArrowWidth* k / 2.0 / c;

		 if((m_ptStart.x < m_ptEnd.x && m_ptStart.y > m_ptEnd.y)
		  || (m_ptStart.x < m_ptEnd.x && m_ptStart.y < m_ptEnd.y))
		 {
			  tempX = m_ptEnd.x - sqrt(3.0) * nArrowWidth / 2.0 / c;
			  tempY = m_ptEnd.y - sqrt(3.0) * nArrowWidth* k / 2.0 / c;
		 }

		 pt1.x = tempX + nArrowWidth * k / 2 / c;
		 pt1.y = tempY - nArrowWidth / 2 / c;

		 pt2.x = tempX - nArrowWidth * k / 2 / c;
		 pt2.y = tempY + nArrowWidth / 2 / c; 
	}

	return TRUE;
}


CStrokeGraph::CStrokeGraph()
{
	m_nWidth = 0;
	m_clrStroke = RGB(0, 0, 0);
}

BOOL CStrokeGraph::HitTest(CPoint ptTest)
{
	BOOL bRet(FALSE);
	int nCount = m_arData.size();
	if(nCount >= 1)
	{
		INT nWidth = m_nWidth / 2;
		CPoint* pPrev = &m_arData[0];
		CRect rtLine;

		for(int i=0; i<nCount; ++i)
		{
			CPoint* pCurrent = &m_arData[i];
			CPoint pt1(*pPrev);
			CPoint pt2(*pCurrent);
			rtLine.SetRect(pt1, pt2);
			rtLine.NormalizeRect();

			double dbRad = nWidth;
			dbRad += ERASER_DIST_COUNT;
			
			INT nInflat(nWidth);

			if(nWidth == 0)
			{
				nInflat += 1;
				dbRad += 1.0;
			}
			rtLine.InflateRect(nInflat, nInflat);
			
			if(rtLine.PtInRect(ptTest))
			{
				if(DistancePt2Line(pt1, pt2, ptTest) <= dbRad)
				{
					bRet = TRUE;
					break;
				}
			}
			pPrev = pCurrent;
		}
	}

	return bRet;
}

BOOL CStrokeGraph::IsObjectEmpty()
{
	return (m_arData.size() == 0);
}

CSCGraphObject* CStrokeGraph::Clone()
{
	CSCGraphObject* pRet = new CStrokeGraph(*this);
	if(pRet != NULL)
	{
		pRet->ResetRefCount();
	}


	return pRet;
}

VOID CStrokeGraph::DrawGraph(CDCHandle dc)
{
	if(IsObjectEmpty()) return;

	CPen newPen = CreatePen(SC_PEN_DEF_STYLE, m_nWidth, m_clrStroke);
	CPenHandle oldPen = dc.SelectPen(newPen);
	CSCDrawRop2Checker ropChecker(dc);
	
	INT nCount = m_arData.size();
	dc.MoveTo(m_arData[0]);

	for(int i=0; i<nCount; ++i)
	{
		dc.LineTo(m_arData[i]);
	}

	dc.SelectPen(oldPen);	
}

VOID CStrokeGraph::AddPoint(CPoint pt)
{
	m_arData.push_back(pt);
}

INT CStrokeGraph::GetPointCount()
{
	return m_arData.size();
}

CPoint CStrokeGraph::GetPointByIndex(INT nIndex)
{
	_ASSERTE(nIndex >= 0 && nIndex < GetPointCount());

	return m_arData[nIndex];
}	



CTextGraph::CTextGraph()
:m_clrText(RGB(0, 0, 0)), m_strText(_T("")), m_ptText(0, 0)
{
	ZeroMemory(&m_logFnt, sizeof(m_logFnt));
}

BOOL CTextGraph::HitTest(CPoint pt)
{
	CRect rtText = GetCurrentTextRect();

	if(rtText.PtInRect(pt))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CTextGraph::IsObjectEmpty()
{
	return (m_strText == _T(""));
}

CSCGraphObject* CTextGraph::Clone()
{
	CSCGraphObject* pRet =  new CTextGraph(*this);
	if(pRet != NULL)
	{
		pRet->ResetRefCount();
	}

	return pRet;
}

VOID CTextGraph::DrawGraph(CDCHandle dc)
{
	if(IsObjectEmpty()) return;

#ifdef _DEBUG
	LPLOGFONT lf = {0};
	if(memcmp(&lf, &m_logFnt, sizeof(lf)) == 0) 
	{
		_ASSERTE(FALSE);
		return;
	}
#endif

	CFont fntNew = CreateFontIndirect(&m_logFnt);

	CRect rtText = GetCurrentTextRect();

	CFontHandle fntOld = dc.SelectFont(fntNew);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_clrText);
	CSCDrawRop2Checker ropChecker(dc);
	dc.DrawText(m_strText, lstrlen(m_strText), &rtText, DT_LEFT);
	dc.SelectFont(fntOld);
}

INT CTextGraph::GetLineHeight()	
{
	CRect rtText = GetTextRect(_T("N"));
	return rtText.Height();
}

INT CTextGraph::GetCharWidth()
{
	CRect rtText = GetTextRect(_T("N"));
	return rtText.Width();
}

CRect CTextGraph::GetTextRect(LPCTSTR lpszText)
{
	_ASSERTE(lpszText != NULL);
	CRect rtRet(0, 0, 0, 0);
	
	CClientDC dc(NULL);
	CFont fntNew = CreateFontIndirect(&m_logFnt);
	CFontHandle fntOld = dc.SelectFont(fntNew);
	dc.DrawText(lpszText, lstrlen(lpszText), &rtRet, DT_LEFT | DT_CALCRECT);
	dc.SelectFont(fntOld);	
	
	rtRet.OffsetRect(m_ptText);

	return rtRet;
}

VOID CTextGraph::SetFont(const LOGFONT& lf)
{
	m_logFnt = lf;
}

VOID CTextGraph::GetFont(LOGFONT& lf)
{
	lf = m_logFnt;
}

VOID CTextGraph::SetText(LPCTSTR lpszText)
{
	_ASSERTE(lpszText != NULL);
	
	m_strText = lpszText;
}
CRect CTextGraph::GetCurrentTextRect()
{
	return	GetTextRect(m_strText);
}

CPoint CTextGraph::GetLastCaretPos()
{
	CPoint ptRet(0, 0);
	CPoint ptStart(m_ptText);

	INT nPos = m_strText.ReverseFind(_T('\n'));
	if(nPos >= 0)
	{
		CString strLastLine = m_strText.Mid(nPos);
		CRect rtText = GetTextRect(strLastLine);
		CSize szText = CSize(rtText.Width(), rtText.Height());
		
		INT nLineCount = GetLineCount();
		
		ptRet.x = ptStart.x + szText.cx;
		ptRet.y = ptStart.y + (GetLineCount() - 1) * GetLineHeight();
	}
	else
	{
		CRect rtText = GetCurrentTextRect();

		ptRet.x = rtText.right;
		ptRet.y = rtText.top;
	}

	return ptRet;	
}

INT CTextGraph::GetLineCount()
{
	INT nCount(1);
	for(int i=0; i<m_strText.GetLength(); ++i)
	{
		if(m_strText[i] == _T('\n'))
		{
			nCount++;
		}	
	}

	return nCount;
}

VOID CTextGraph::AddChar(TCHAR c)
{
	m_strText += c;
}
VOID CTextGraph::BackChar(int nBackCount)
{
	INT nCount = m_strText.GetLength();
	
	INT nLeft = nCount - nBackCount;
	if(nLeft < 0)
	{
		nLeft = 0;
	}

	m_strText = m_strText.Left(nLeft);
}

VOID CTextGraph::AddStr(LPCTSTR str)
{
	_ASSERTE(str != NULL);
	m_strText += str;
}