#pragma  once 

#include "HYUtilBase.h"
#include "atlctrls.h"

class CHYResLoader
{
public:	
	CHYResLoader();
	~CHYResLoader();
	void SetResInstance(HINSTANCE hRes);
	HINSTANCE GetResInstance() const;
	INT LoadString(UINT uID,  LPTSTR lpBuffer, int nBufferMax) const;
	HBITMAP LoadBitmap(INT nID) const;
	CImageList LoadImageList(INT nBmpId, int nCount) const;

	COLORREF GetStringColor(INT nStringID) const;

	HCURSOR GetCursor(UINT nID) const;
	
protected:
	CHYResLoader(const CHYResLoader&);
	HINSTANCE m_hRes;	
};