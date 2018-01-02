#pragma  once 
#include "TsStdAfx.h"

class ITsWinFrame
{
public:
	ITsWinFrame(ITsWinFrame * pBaseFrame = NULL);
	void SetFrameRect(CRect& rtFrame);
	virtual CRect GetContentRect() = 0;
	virtual void DrawFrame(CDCHandle dc) = 0;
	virtual CRect GetIdealRect(CRect& rtContent) = 0;
	virtual ~ITsWinFrame() { NULL; }

protected:
	CRect m_rtFrame;
	ITsWinFrame* m_pBaseFrame;
};

class CTsColorFrame: public ITsWinFrame
{
public:
	CTsColorFrame(COLORREF clrFrame, int nWidth = 1, int nHeight = 1, ITsWinFrame* pBase = NULL);
	virtual CRect GetContentRect();
	virtual void DrawFrame(CDCHandle dc);
	virtual CRect GetIdealRect(CRect& rtContent);

private:
	COLORREF m_clrFrame;
	int m_nFrameWidth;
	int m_nFrameHight;
};

class CTsControlPaneFrame: public ITsWinFrame
{
public:
	CTsControlPaneFrame(ITsWinFrame* pBase = NULL);
	virtual CRect GetContentRect();
	virtual void DrawFrame(CDCHandle dc);
	virtual CRect GetIdealRect(CRect& rtContent);

	void SetTitleText(LPCTSTR lpszText);
	void SetIcon(HBITMAP bmp);
	void SetFont(HFONT hFont);
	void SetTitleTextColor(COLORREF clr);

protected:
	void _DrawFrame(CDCHandle dc);
	void _DrawTitleBar(CDCHandle dc);
	CFontHandle _GetFont();

private:
	TCHAR m_szTitle[128];
	CBitmapHandle m_bmpIcon;
	CFontHandle  m_fntText;
	COLORREF m_clrText;
};