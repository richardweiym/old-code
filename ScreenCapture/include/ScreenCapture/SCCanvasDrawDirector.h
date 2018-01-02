#pragma  once 

#include "HYAutoPtr.h"
#include "HYUIBase.h"
#include "SCCanvasDrawer.h"

class CSCCanvasDrawDirector: public CRefCountBase,
							 public CMessageMap
{
public:
	BEGIN_MSG_MAP(CSCCanvasDrawDirector)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)

		CHAIN_MSG_MAP_MEMBER_REF_PTR(m_pCurrentDrawer)
		
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)

	END_MSG_MAP()

	CSCCanvasDrawDirector();
	~CSCCanvasDrawDirector();

	VOID OnSkinChanged(DWORD dwType);
	
	VOID CheckCurrentDrawer();
	CRefPtr<CSCCanvasBaseDrawer> GetCurrentDrawer() const;

	VOID RedrawCopyDataDC(BOOL bCopyFromCanvas = TRUE);

protected:
	VOID DrawScreenSnapshot(CDCHandle dc);
	VOID DrawAnnotation(CDCHandle dc);
	VOID DrawCursor(CDCHandle dc);
	VOID DrawSelectShadow(CDCHandle dc);
	VOID DrawWaterMark(CDCHandle dc);
	VOID DrawToolView(CDCHandle dc);

	VOID DrawCurrentCursor(CDCHandle dc);

protected:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&);

	LRESULT OnMouseMove(UINT, WPARAM, LPARAM, BOOL&);

protected:
//	BOOL LoadShadowBMP();
	VOID LoadResource();


private:
//	CBitmap m_bmpShadow;
	CBitmap m_bmpWaterMark;
	COLORREF m_clrShadow;

	CRefPtr<CSCCanvasBaseDrawer> m_pCurrentDrawer;
};