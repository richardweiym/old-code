#include "HYResMgr.h"
#include "HYUIBase.h"

CHYResLoader::CHYResLoader()
{
	m_hRes = NULL;
}

CHYResLoader::~CHYResLoader()
{
}

void CHYResLoader::SetResInstance(HINSTANCE hRes)
{
	m_hRes = hRes;
}

HINSTANCE CHYResLoader::GetResInstance() const
{
	return m_hRes;
}

HBITMAP CHYResLoader::LoadBitmap(INT nID) const
{
	HBITMAP hBmp = NULL;
	_ASSERTE(m_hRes != NULL);
	if(m_hRes != NULL)
	{
		hBmp = ::LoadBitmap(m_hRes, MAKEINTRESOURCE(nID));
	}
	return hBmp;
}

INT CHYResLoader::LoadString(UINT uID, LPTSTR lpBuffer, int nBufferMax) const
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

CImageList CHYResLoader::LoadImageList(INT nID, int nCount) const
{
	_ASSERTE(nCount >= 1);
	CImageList imgRet;
	_ASSERTE(m_hRes != NULL);
	if(m_hRes == NULL) return imgRet;

	CBitmap bmp = LoadBitmap(nID);
	_ASSERTE(!bmp.IsNull());
	if(bmp.IsNull()) return imgRet;

	CSize szBmp = CHYUICommonFunctionWrapper::GetBitmapSize(bmp);
	int nX = szBmp.cx / nCount;
	imgRet.Create(nX, szBmp.cy, ILC_COLOR24|ILC_MASK, nCount, 1);
	imgRet.Add(bmp, RGB(255, 0, 255));
	return imgRet;
}

COLORREF CHYResLoader::GetStringColor(INT nStringID) const
{
	COLORREF clrRet(GetSysColor(COLOR_WINDOW));

	TCHAR szString[32] = {0};
	INT nRet = LoadString(nStringID, szString, sizeof(szString) / sizeof(TCHAR) - 1);
	if(nRet > 0)
	{
		INT nR, nG, nB;
		_stscanf(szString, _T("RGB(%d,%d,%d"), &nR, &nG, &nB);
		clrRet = RGB(nR, nG, nB);
	}

	return clrRet;
}

HCURSOR CHYResLoader::GetCursor(UINT nID) const
{
	HCURSOR hRet(NULL);
	if(m_hRes != NULL)
	{
		hRet = LoadCursor(m_hRes, MAKEINTRESOURCE(nID));
	}

	return hRet;
}