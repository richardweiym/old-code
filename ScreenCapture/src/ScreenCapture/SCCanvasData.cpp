#include "hyUtilbase.h"
#include "HYUIBase.h"
#include "SCCanvasData.h"
#include "SCCanvasGraph.h"

#include <algorithm>
#include <functional>

using namespace std;

#ifndef CAPTUREBLT  
#define CAPTUREBLT         (DWORD)0x40000000 
#endif


class CGraphObjCMP: public unary_function<CRefPtr<CSCGraphObject>, BOOL>
{
public:
	CGraphObjCMP(DWORD dwID)
	{
		m_dwID = dwID;
	}

	BOOL operator()(CRefPtr<CSCGraphObject> pEle)
	{
		_ASSERTE(!pEle.IsNull());
		return (pEle->GetGraphID() == m_dwID);
	}

protected:
	DWORD m_dwID;
};



BOOL GetCursorInfoFun(PCURSORINFO pci )
{
	typedef  BOOL (WINAPI *GetCursorInfoProc)(PCURSORINFO);
	HMODULE hUser32Dll = GetModuleHandle(_T("user32.dll"));
	if(hUser32Dll != NULL)
	{
		GetCursorInfoProc pFun = (GetCursorInfoProc)GetProcAddress(hUser32Dll, "GetCursorInfo");
		_ASSERTE(pFun != NULL);
		if(pFun != NULL)
		{
			return pFun(pci);
		}
	}
	else
	{
		_ASSERTE(FALSE);
	}

	return FALSE;
}


CSCScreenSnapshot::CSCScreenSnapshot()
{
	DoScreenSnapshot();
}

VOID CSCScreenSnapshot::DoScreenSnapshot()
{
	CClientDC dc(NULL);

	CDC memDC = CreateCompatibleDC(dc);
// 	INT nScreenX = GetSystemMetrics(SM_CXSCREEN);
// 	INT nScreenY = GetSystemMetrics(SM_CYSCREEN);
//	for multi-monitor support	
	INT nScreenX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	INT nScreenY = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	INT nLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
	INT nTop = GetSystemMetrics(SM_YVIRTUALSCREEN);

	m_bmpSnapShot = CreateCompatibleBitmap(dc, nScreenX, nScreenY);
	CBitmapHandle bmpOld = memDC.SelectBitmap(m_bmpSnapShot);
	
	memDC.BitBlt(0, 0, nScreenX, nScreenY, dc, nLeft, nTop, SRCCOPY | CAPTUREBLT);

	memDC.SelectBitmap(bmpOld);
}
	

CSCCanvasData::CSCCanvasData()
:m_sizeScreen(0, 0), m_rectSelect(0, 0, 0, 0)
{
	m_arGraphs.reserve(512);
	ZeroMemory(&m_cursorInfo, sizeof(m_cursorInfo));
}

CSCCanvasData::~CSCCanvasData()
{
	ClearData();
}

VOID CSCCanvasData::SetScreenBmp(CBitmapHandle bmp )
{
	CBitmapHandle bmpOld = m_bmpScreen;

	m_bmpScreen = bmp;

	if(!m_bmpScreen.IsNull())
	{
		CheckScreenBmpSize();
	}

	if(!bmpOld.IsNull())
	{
		bmpOld.DeleteObject();
	}
}

VOID CSCCanvasData::ClearData()
{
	if(!m_bmpScreen.IsNull())
	{
		m_bmpScreen.DeleteObject();
	}
	
	ReleseCopyDataDC();

	m_arGraphs.clear();

	ReleaseCursorData();
}

void CSCCanvasData::CheckScreenBmpSize()
{
	CSize sizeBmp(0, 0);

	if(!m_bmpScreen.IsNull())
	{
		sizeBmp = CHYUICommonFunctionWrapper::GetBitmapSize(m_bmpScreen);
	}

	m_sizeScreen = sizeBmp;
}

VOID CSCCanvasData::AddGraphObject(CRefPtr<CSCGraphObject> pObj)
{
	_ASSERTE(!pObj.IsNull());
	_ASSERTE(!CheckIsGraphObjectExist(pObj->GetGraphID()));	
	
	m_arGraphs.push_back(pObj);

}

VOID CSCCanvasData::SetSelectRect(CRect& rt)
{
	if(rt != m_rectSelect)
	{
		m_rectSelect = rt;
		NotifyObservers(UI_WPARAM_SELECT_RECT_CHANGED, (LPARAM)&rt);
	}
}

BOOL CSCCanvasData::RemoveGraphObject(INT nID)
{
	CGraphArray::iterator itr = remove_if(m_arGraphs.begin(), m_arGraphs.end(), CGraphObjCMP(nID));

	if(itr != m_arGraphs.end())
	{
		m_arGraphs.erase(itr, m_arGraphs.end());	
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
INT CSCCanvasData::GetGraphCount() const
{
	return m_arGraphs.size();
}
CRefPtr<CSCGraphObject> CSCCanvasData::GetGraphByIndex(int nIndex)
{
	_ASSERTE(nIndex < GetGraphCount() && nIndex >= 0 );
	return m_arGraphs[nIndex];
}

BOOL CSCCanvasData::CheckIsGraphObjectExist(DWORD dwID)
{
	CGraphArray::iterator itr = find_if(m_arGraphs.begin(), m_arGraphs.end(), CGraphObjCMP(dwID));

	return (itr != m_arGraphs.end());
}

CRefPtr<CSCGraphObject> CSCCanvasData::GetGraphByID(DWORD dwID)
{
	CGraphArray::iterator itr = find_if(m_arGraphs.begin(), m_arGraphs.end(), CGraphObjCMP(dwID));
	if(itr != m_arGraphs.end())
	{
		return *itr;
	}
	else
	{
		return NULL;
	}
}


BOOL CSCCanvasData::InitCopyDataDC(CDCHandle dc)
{
	if(!m_dcCopyCanvas.IsNull() && !m_bmpCopyData.IsNull())
	{
		return TRUE;
	}

	ReleseCopyDataDC();
	
	//the rect is bigger than screen, because there part of cursor image maybe out of screen

	CRect rtCopyBmp(-50, -50, m_sizeScreen.cx + 50, m_sizeScreen.cy + 50);

	m_dcCopyCanvas.CreateCompatibleDC((HDC)dc);
	m_bmpCopyData.CreateCompatibleBitmap(dc, rtCopyBmp.Width(), rtCopyBmp.Height());
	m_bmpOldData = m_dcCopyCanvas.SelectBitmap(m_bmpCopyData);

	m_dcCopyCanvas.SetViewportOrg(50, 50);
	m_dcCopyCanvas.FillSolidRect(&rtCopyBmp, RGB(0, 0, 0));
	
	_ASSERTE(!m_bmpOldData.IsNull());

	return (!m_dcCopyCanvas.IsNull()
		&& !m_bmpCopyData.IsNull());
}

VOID CSCCanvasData::ReleseCopyDataDC()
{
	if(!m_dcCopyCanvas.IsNull())
	{
		_ASSERTE(!m_bmpOldData.IsNull());
		if(!m_bmpOldData.IsNull())
		{
			_ASSERTE(::GetObjectType(m_bmpOldData) == OBJ_BITMAP);
			m_dcCopyCanvas.SelectBitmap(m_bmpOldData);
			m_bmpOldData = NULL;
		}

		m_dcCopyCanvas.DeleteDC();
	}

	if(!m_bmpCopyData.IsNull())
	{
		m_bmpCopyData.DeleteObject();
	}
}

CBitmapHandle CSCCanvasData::GetSelectBMP()
{
	CRect rtSelect = GetSelectRect();
	if(rtSelect.IsRectEmpty())
	{
		rtSelect.SetRect(0, 0, m_sizeScreen.cx, m_sizeScreen.cy);
	}

	CDCHandle dc = GetCopyDataDC();
	if(dc.IsNull()) return NULL;

	CDC dcMem = CreateCompatibleDC(dc);
	CBitmapHandle bmpClipboard = CreateCompatibleBitmap(dc, rtSelect.Width(), rtSelect.Height());
	CBitmapHandle bmpOld = dcMem.SelectBitmap(bmpClipboard);
	dcMem.BitBlt(0, 0, rtSelect.Width(), rtSelect.Height(), dc, rtSelect.left, rtSelect.top, SRCCOPY);
	dcMem.SelectBitmap(bmpOld);

	return bmpClipboard;
}

BOOL CSCCanvasData::SnapshotCurrentCursor()
{
	CURSORINFO ci = {0};
	ci.cbSize = sizeof(ci);
	if(!GetCursorInfoFun(&ci))
	{
		_ASSERTE(FALSE);
		return FALSE;
	}
	
	if(ci.flags == CURSOR_SHOWING)
	{
		m_cursorInfo.m_ptCursor = ci.ptScreenPos;
		HCURSOR hCursor = ci.hCursor;
		_ASSERTE(hCursor != NULL);

		if(!GetIconInfo(hCursor, (ICONINFO*)(&m_cursorInfo)))
		{
			_ASSERTE(FALSE);
			return FALSE;
		}
	}
	
	return TRUE;
}

VOID CSCCanvasData::ReleaseCursorData()
{
	if(m_cursorInfo.hbmColor != NULL)
	{
		DeleteObject(m_cursorInfo.hbmColor);
		m_cursorInfo.hbmColor = NULL;
	}

	if(m_cursorInfo.hbmMask != NULL)
	{
		DeleteObject(m_cursorInfo.hbmMask);
		m_cursorInfo.hbmMask = NULL;
	}
}

const CSCCursorInfo* CSCCanvasData::GetCursorData() const
{
	return &m_cursorInfo;
}