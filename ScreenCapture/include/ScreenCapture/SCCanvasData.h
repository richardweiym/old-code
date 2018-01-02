#pragma  once 

#include "hyautoptr.h"
#include "HYPattern.h"
#include <vector>
#include "SCIDDef.h"

#ifndef SM_XVIRTUALSCREEN
#define SM_XVIRTUALSCREEN       76
#define SM_YVIRTUALSCREEN       77
#define SM_CXVIRTUALSCREEN      78
#define SM_CYVIRTUALSCREEN      79
#endif

// do screen snapshot
class CSCScreenSnapshot
{
public:
	CSCScreenSnapshot();
	void DoScreenSnapshot();
	
	operator HBITMAP() { return m_bmpSnapShot; }
	
private:
	CBitmapHandle m_bmpSnapShot;
};

class CSCCursorInfo: public ICONINFO
{
public:
	POINT m_ptCursor;
};


#ifndef CURSOR_SHOWING
typedef struct tagCURSORINFO
{
    DWORD   cbSize;
    DWORD   flags;
    HCURSOR hCursor;
    POINT   ptScreenPos;
} CURSORINFO, *PCURSORINFO, *LPCURSORINFO;
#define CURSOR_SHOWING     0x00000001
#endif

BOOL GetCursorInfoFun(PCURSORINFO pci );

//document class
class CSCGraphObject;
class CSCCanvasData: public CRefCountBase,
					 public CHYDataModuleBase
{
public:
	CSCCanvasData();
	~CSCCanvasData();
	
	VOID SetScreenBmp( CBitmapHandle bmp );
	CBitmapHandle GetScreenBmp() const { return m_bmpScreen; }

	CSize GetScreenSize() const { return m_sizeScreen; }
	
	VOID SetSelectRect(CRect& rt);
	CRect GetSelectRect() const { return m_rectSelect; }

	VOID ClearData();
	
	VOID AddGraphObject(CRefPtr<CSCGraphObject> pObj);
	BOOL RemoveGraphObject(INT nID);
	INT GetGraphCount() const;
	CRefPtr<CSCGraphObject> GetGraphByIndex(int nIndex);
	CRefPtr<CSCGraphObject> GetGraphByID(DWORD dwID);

	BOOL InitCopyDataDC(CDCHandle dcSrc);
	VOID ReleseCopyDataDC();
	CDCHandle GetCopyDataDC() const { return m_dcCopyCanvas; }
	CBitmapHandle GetSelectBMP();

	BOOL SnapshotCurrentCursor();
	VOID ReleaseCursorData();
	const	CSCCursorInfo* GetCursorData() const;

protected:
	void CheckScreenBmpSize();
	BOOL CheckIsGraphObjectExist(DWORD dwID);

private:
	CBitmapHandle m_bmpScreen; //screen bitmap
	CSize m_sizeScreen;
	CRect m_rectSelect;

	typedef std::vector<CRefPtr<CSCGraphObject> > CGraphArray; 
	CGraphArray m_arGraphs;//annotation data

	CDC m_dcCopyCanvas;			//DC to get pixel when move mouse
	CBitmap m_bmpCopyData;		//bitmap with screen snapshot and annotations	
	CBitmapHandle m_bmpOldData;

	CSCCursorInfo m_cursorInfo;	//cursor data(position and cursor bitmap)
};

template<typename T>
class CSCDataObserver
{
public:
	BEGIN_MSG_MAP(CSCDataObserver<T>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_HY_UI_DATA_CHANGED, OnUIDataChanged)
	END_MSG_MAP()

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pThis = static_cast<T*>(this);

		CRefPtr<CSCCanvasData> pData = GetCanvasData();
		_ASSERTE(!pData.IsNull());
		pData->AddObserver(pThis->m_hWnd);

		return S_OK;
	}
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pThis = static_cast<T*>(this);

		CRefPtr<CSCCanvasData> pData = GetCanvasData();
		_ASSERTE(!pData.IsNull());
		pData->AddObserver(pThis->m_hWnd);

		return S_OK;
	}
	LRESULT OnDestroy(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pThis = static_cast<T*>(this);

		CRefPtr<CSCCanvasData> pData = GetCanvasData();
		_ASSERTE(!pData.IsNull());
		pData->RemoveObserver(pThis->m_hWnd);

		return S_OK;
	}

	LRESULT OnUIDataChanged(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;

		T* pThis = static_cast<T*>(this);
		if(pThis->IsWindow() && wParam == UI_WPARAM_SELECT_RECT_CHANGED)
		{
			pThis->DoUIDataChanged(wParam, lParam);
			bHandled = TRUE;
		}

		return S_OK;
	}
	
};