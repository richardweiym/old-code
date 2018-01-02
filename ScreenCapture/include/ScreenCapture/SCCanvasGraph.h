#pragma  once 

#include "HYUtilBase.h"
#include "HYAutoPtr.h"
#include <vector>

double DistancePt2Pt(CPoint& pt1, CPoint& pt2);
double DistancePt2Line(CPoint& ptStart, CPoint& ptEnd, CPoint& ptTest);

class CSCDrawRop2Checker
{
public:
	CSCDrawRop2Checker(CDCHandle dc);
	~CSCDrawRop2Checker();

protected:
	CDCHandle m_dc;
	DWORD m_dwOldRop2;
};

class CSCGraphIDGen
{
public:
	static DWORD GetUniqueID();
};

class CSCGraphObject: public CRefCountBase
{
public:
	CSCGraphObject();
	virtual ~CSCGraphObject();

	virtual BOOL HitTest(CPoint pt) = 0;
	virtual BOOL IsObjectEmpty() = 0;

	VOID SetGraphID(DWORD dwID) { m_dwID = dwID; }
	DWORD GetGraphID() const { return m_dwID; }

	virtual CSCGraphObject* Clone() = 0;
	virtual VOID  DrawGraph(CDCHandle dc)  = 0;
	
protected:
	DWORD m_dwID;
};

class CRectGraph: public CSCGraphObject
{
public:
	CRectGraph();
	CRectGraph(CPoint ptStart, CPoint ptEnd);
	CRectGraph(int x1, int y1, int x2, int y2);

	virtual BOOL HitTest(CPoint pt);
	virtual BOOL IsObjectEmpty();

	virtual CSCGraphObject* Clone();
	virtual VOID  DrawGraph(CDCHandle dc);	

public:
	CRect m_rect;
	INT m_nFrameWidth;
	COLORREF m_clrFrame;
};

class CEllipseGraph: public CRectGraph
{
public:
	CEllipseGraph();
	CEllipseGraph(CPoint ptStart, CPoint ptEnd);
	CEllipseGraph(int x1, int y1, int x2, int y2);

	virtual BOOL HitTest(CPoint pt);
	
	virtual CSCGraphObject* Clone();
	virtual VOID DrawGraph(CDCHandle dc);
};


class CLineGraph: public CSCGraphObject
{
public:
	CLineGraph();
	CLineGraph(CPoint ptStart, CPoint ptEnd);

	virtual BOOL HitTest(CPoint pt);
	virtual BOOL IsObjectEmpty();

	virtual CSCGraphObject* Clone();
	virtual VOID  DrawGraph(CDCHandle dc);

public:
	CPoint m_ptStart;
	CPoint m_ptEnd;
	INT m_nWidth;
	COLORREF m_clrLine;
};

class CArrowGraph: public CLineGraph
{
public:
	CArrowGraph(CPoint ptStart, CPoint ptEnd);

	virtual BOOL HitTest(CPoint pt);
	virtual BOOL IsObjectEmpty();
	
	virtual CSCGraphObject* Clone();
	virtual VOID DrawGraph(CDCHandle dc);
	
	INT GetArrowWidth();

protected:
	
	double Get2PtMinDistance();
	BOOL GetArrowPoint(CPoint& pt1, CPoint& pt2);

};

class CStrokeGraph: public CSCGraphObject
{
public:
	CStrokeGraph();

	virtual BOOL HitTest(CPoint pt);
	virtual BOOL IsObjectEmpty();

	virtual CSCGraphObject* Clone();
	virtual VOID  DrawGraph(CDCHandle dc);
	
	VOID AddPoint(CPoint pt);
	INT GetPointCount();
	CPoint GetPointByIndex(INT nIndex);

public:
	INT m_nWidth;
	COLORREF m_clrStroke;

protected:
	std::vector<CPoint> m_arData;
};
	
class CTextGraph: public CSCGraphObject
{
public:
	CTextGraph();

	virtual BOOL HitTest(CPoint pt);
	virtual BOOL IsObjectEmpty();

	virtual CSCGraphObject* Clone();
	virtual VOID  DrawGraph(CDCHandle dc);

	VOID SetFont(const LOGFONT& lf);
	VOID GetFont(LOGFONT& lf);

	VOID SetText(LPCTSTR lpszText);
	LPCTSTR GetText() const { return m_strText; }
	INT GetTextLength() { return m_strText.GetLength(); }
	
	CRect GetCurrentTextRect();
	INT GetLineHeight();
	INT GetCharWidth();
	
	CPoint GetLastCaretPos();

	VOID AddChar(TCHAR);
	VOID AddStr(LPCTSTR str);
	VOID BackChar(int nCount = 1);

protected:
	INT GetLineCount();
	CRect GetTextRect(LPCTSTR lpszText);

public:
	INT m_clrText;
	CPoint m_ptText;

protected:
	LOGFONT m_logFnt;
	CString m_strText;
};