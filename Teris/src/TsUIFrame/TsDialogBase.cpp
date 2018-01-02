#include "TsDialogBase.h"
#include "TerisRes.h"
#include "TsUtilBase.h"
#include "TsUIInstanceApi.h"

CBrush g_brushTsDlgBk;

LRESULT CTsDialogBKMap::OnCtlColorDlg(UINT, WPARAM, LPARAM, BOOL&)
{
	if(m_brushBk.IsNull())
	{
		_CreateBrush();
	}
	_ASSERTE(!m_brushBk.IsNull());

	return (LRESULT)m_brushBk.m_hBrush;
}

LRESULT CTsDialogBKMap::OnCtlColorStatic(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	CDCHandle dc = (HDC)wParam;
	_ASSERTE(!dc.IsNull());
	dc.SetBkMode(TRANSPARENT);

	if(m_brushBk.IsNull())
	{
		_CreateBrush();
	}
	_ASSERTE(!m_brushBk.IsNull());

	return (LRESULT)m_brushBk.m_hBrush;
}

void CTsDialogBKMap::_CreateBrush()
{
	if(m_brushBk.IsNull())
	{
		if(g_brushTsDlgBk.IsNull())
		{
			CTsResMgr* resMgr = TsGetResMgr();
			_ASSERTE(resMgr != NULL);
			HINSTANCE hRes = resMgr->GetResInstance();
			_ASSERTE(hRes != NULL);
			
			if(hRes != NULL)
			{
				CBitmap bmp = LoadBitmap(hRes, MAKEINTRESOURCE(IDB_DLG_BK));
				g_brushTsDlgBk = CreatePatternBrush(bmp);
			}
		}

		if(!g_brushTsDlgBk.IsNull())
		{
			m_brushBk = g_brushTsDlgBk;
		}
	}

}
