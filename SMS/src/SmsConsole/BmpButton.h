#pragma  once 

#include <atlctrls.h>
#include <atlmisc.h>

class CIEButton:public CWindowImpl<CIEButton, CButton,CControlWinTraits>
{
public:
	BEGIN_MSG_MAP(CIEButton)
		MESSAGE_HANDLER(OCM_DRAWITEM,OnDrawItem)
		MESSAGE_HANDLER(WM_MOUSEMOVE,OnMouseMove)
	END_MSG_MAP()	
	
	CIEButton()
	{
		m_hBitmap = NULL;
		m_IsMouseOn = FALSE;
	}
	~CIEButton()
	{
		if(m_hBitmap != NULL)
		{
			::DeleteObject(m_hBitmap);
			m_hBitmap = NULL;
		}
	}
	BOOL SetBitmap(WORD id)
	{
		m_hBitmap = ::LoadBitmap(m_hResModule,MAKEINTRESOURCE(id));
		if(m_hBitmap != NULL)
		{
			return TRUE;
		}

		return	FALSE;
	}
	
	LRESULT OnMouseMove(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bhandled)
	{
		bhandled = FALSE ;
		if(!m_IsMouseOn)
		{
			SetIsMouseOn(TRUE);
			InvalidateRect(NULL);
		}
		return 0;
	}


	LRESULT OnDrawItem(UINT nMsgType, WPARAM wParam,LPARAM lParam, BOOL& bHandled)
	{
		LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;
		CDCHandle hDc(lpDrawItem->hDC);
		CDC memDc;
		BITMAP bmp;

		memDc.CreateCompatibleDC(hDc);
		::GetObject(m_hBitmap,sizeof(bmp),&bmp);
		
		INT x = bmp.bmWidth,y = bmp.bmHeight;
		
		memDc.SelectBitmap(m_hBitmap);
		BitBlt(hDc,0,0,x,y,memDc,0,0,SRCCOPY);
		
		CRect rc(lpDrawItem->rcItem),rc2(rc);

		 if(m_IsMouseOn)
		{
			rc.top=rc.top+1;
			rc.bottom=rc.bottom-1;
			hDc.Draw3dRect(&rc,RGB(255,255,255),RGB(0,0,0));
		}
		if(lpDrawItem->itemAction& ODA_SELECT)
		{
			rc2.top=rc2.top+2;
			rc2.bottom=rc2.bottom-2;
			rc2.left=rc2.left+2;
			rc2.right=rc2.right-2;
			hDc.Draw3dRect(&rc2,RGB(0,0,0),RGB(255,255,255));
		}	
		
		return 0;
	}
	
	BOOL GetIsMouseOn()
	{
		return m_IsMouseOn;
	}

	VOID SetIsMouseOn(BOOL b)
	{
		m_IsMouseOn = b;
	}
	static VOID SetResModule(HINSTANCE h) 
	{
		m_hResModule = h;
	}
	
private:
	BOOL m_IsMouseOn;
	HBITMAP m_hBitmap;
	static HINSTANCE m_hResModule;
};
