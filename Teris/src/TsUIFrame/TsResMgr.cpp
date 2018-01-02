#include "TsResMgr.h"
#include "tsutilbase.h"
#include "TsDrawBase.h"

CTsResMgr* g_gameResMgr = NULL;

CTsResMgr::CTsResMgr()
{
	TS_RUN_ONCE_CHECK

	m_hRes = NULL;

	_ASSERTE(g_gameResMgr == NULL);
	g_gameResMgr = this;
}
CTsResMgr::~CTsResMgr()
{
	g_gameResMgr = NULL;
}
void CTsResMgr::SetResInstance(HINSTANCE hRes)
{
	m_hRes = hRes;
}
HINSTANCE CTsResMgr::GetResInstance()
{
	return m_hRes;
}
HBITMAP CTsResMgr::LoadBitmap(INT nID)
{
	HBITMAP hBmp = NULL;
	_ASSERTE(m_hRes != NULL);
	if(m_hRes != NULL)
	{
		hBmp = ::LoadBitmap(m_hRes, MAKEINTRESOURCE(nID));
	}
	return hBmp;
}
INT CTsResMgr::LoadString(UINT uID, LPTSTR lpBuffer, int nBufferMax)
{
	INT nRet(0);
	_ASSERTE(m_hRes != NULL);
	if(m_hRes != NULL)
	{
		nRet = ::LoadString(m_hRes, uID, lpBuffer, nBufferMax);
		_ASSERTE(nRet > 0);
	}
	return nRet;
}
CImageList CTsResMgr::LoadImageList(INT nID, int nCount)
{
	_ASSERTE(nCount >= 1);
	CImageList imgRet;
	_ASSERTE(m_hRes != NULL);
	if(m_hRes == NULL) return imgRet;

	CBitmap bmp = LoadBitmap(nID);
	_ASSERTE(!bmp.IsNull());
	if(bmp.IsNull()) return imgRet;

	CSize szBmp = GetBitmapSize(bmp);
	int nX = szBmp.cx / nCount;
	imgRet.Create(nX, szBmp.cy, ILC_COLOR24|ILC_MASK, nCount, 1);
	imgRet.Add(bmp, RGB(255, 0, 255));
	return imgRet;
}
