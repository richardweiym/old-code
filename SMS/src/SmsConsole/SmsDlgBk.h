#pragma  once 

#include "resource.h"

class CDialogSkin:public CMessageMap
{
public:
   BEGIN_MSG_MAP(CDialogSkin)
      MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
	  MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
   END_MSG_MAP()
		
   CDialogSkin()
   {
	   m_hBrush = NULL;
   }

   LRESULT OnCtlColorDlg(UINT, WPARAM, LPARAM lParam, BOOL&)
   {
      if(m_hBrush == NULL)
	  {
		  HBITMAP hBmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BKGND));
		  m_hBrush = CreatePatternBrush(hBmp);
		  ::DeleteObject(hBmp);
	  }
	  return (LRESULT)m_hBrush;
   }
   LRESULT OnCtlColorStatic(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
   {
	  SetBkMode((HDC)wParam, TRANSPARENT);
      if(m_hBrush == NULL)
	  {
		  HBITMAP hBmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BKGND));
		  m_hBrush = CreatePatternBrush(hBmp);
		  ::DeleteObject(hBmp);
	  }
	  return (LRESULT)m_hBrush;
   }
   ~CDialogSkin()
   {
	   if(m_hBrush != NULL)
	   {
		   ::DeleteObject(m_hBrush);
	   }
   }

private:
	HBRUSH m_hBrush;
};