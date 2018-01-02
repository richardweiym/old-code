#pragma  once 

#include "TsStdAfx.h"

class CTsResMgr
{
public:	
	CTsResMgr();
	~CTsResMgr();
	void SetResInstance(HINSTANCE hRes);
	HINSTANCE GetResInstance();
	INT LoadString(UINT uID,  LPTSTR lpBuffer, int nBufferMax);
	HBITMAP LoadBitmap(INT nID);
	CImageList LoadImageList(INT nBmpId, int nCount);
	
protected:
	CTsResMgr(const CTsResMgr&);
	HINSTANCE m_hRes;
};