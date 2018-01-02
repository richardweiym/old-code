#include "SCXImage.h"

#define SC_USE_CXIMAGE 0
#define SC_USE_GDIPLUS 1

#if SC_USE_CXIMAGE
#include "../CxImage/CxImage/ximage.h"
#pragma  comment(lib, "png.lib ")
#pragma  comment(lib, "jpeg.lib")
#pragma  comment(lib, "zlib.lib")
#pragma  comment(lib, "tiff.lib")
#pragma  comment(lib, "jbig.lib")
#pragma  comment(lib, "jasper.lib")
#pragma  comment(lib, "mng.lib")
#pragma  comment(lib, "cximage.lib")
#endif	//SC_USE_CXIMAGE

#if SC_USE_GDIPLUS
#include "GDIPlus.h"
#include "GdiPlusFlat.h"
using namespace Gdiplus;
#pragma  comment(lib, "GdiPlus.lib")
#endif

CString GetExtFileTypeName(LPCTSTR lpszFileName)
{
	_ASSERTE(lpszFileName != NULL);
	LPTSTR lpszExt = (LPTSTR)_tcsrchr(lpszFileName, _T('.'));
	if(lpszExt == NULL) return _T("");

	++lpszExt;
	CString strExt = lpszExt;
	strExt.MakeUpper();
	return strExt;
}

#if SC_USE_CXIMAGE
BOOL SC_CXImage_SaveBitmap2File(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
	_ASSERTE(lpszFileName != NULL);
	DWORD dwFileType = CXIMAGE_FORMAT_UNKNOWN;
	CString strExt = GetExtFileTypeName(lpszFileName);
	if(strExt == _T("BMP"))
	{
		dwFileType = CXIMAGE_FORMAT_BMP;
	}
	else if(strExt == _T("PNG"))
	{
		dwFileType = CXIMAGE_FORMAT_PNG;
	}
	else if(strExt == _T("JPG") || strExt == _T("JPEG"))
	{
		dwFileType = CXIMAGE_FORMAT_JPG;
	}
	else if(strExt == _T("GIF"))
	{
		dwFileType = CXIMAGE_FORMAT_GIF;
	}
	else if(strExt == _T("TIF") || strExt == _T("TIFF"))
	{
		dwFileType = CXIMAGE_FORMAT_TIF;
	}

	if(dwFileType == CXIMAGE_FORMAT_UNKNOWN) return FALSE;
	
	CxImage image;
	if(!image.CreateFromHBITMAP(hBitmap)) return FALSE;

	if(!image.Save(lpszFileName, dwFileType))
	{
		_ASSERTE(FALSE);
		return FALSE;
	}

	return TRUE;
}
#endif //SC_USE_CXIMAGE

#if SC_USE_GDIPLUS
class CSCGDIPlusStartup
{
public:
	CSCGDIPlusStartup()
	{
		m_nToken = 0;
		m_bOK = (GdiplusStartup((ULONG_PTR*)&m_nToken, &m_gdi, NULL) == Ok);
	}

	~CSCGDIPlusStartup()
	{
		if(m_bOK)
		{
			GdiplusShutdown(m_nToken);
		}
	}

	BOOL IsOK() const
	{
		return m_bOK;
	}

protected:
	GdiplusStartupInput m_gdi;
	UINT_PTR m_nToken;
	BOOL m_bOK;
};

CString GetMimeType(CString& strFileExtType)
{
	if(strFileExtType == _T("BMP"))
	{
		return _T("image/bmp");
	}
	else if(strFileExtType == _T("JPG") || strFileExtType == _T("JPEG"))
	{
		return _T("image/jpeg");
	}
	else if(strFileExtType == _T("GIF"))
	{
		return _T("image/gif");
	}
	else if(strFileExtType == _T("TIF") || strFileExtType == _T("TIFF"))
	{
		return _T("image/tiff");
	}
	else if(strFileExtType == _T("PNG"))
	{
		return _T("image/png");
	}
	else
	{
		return _T("");
	}
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)   
{   
    UINT num = 0;                    
    UINT size = 0;                 
    ImageCodecInfo* pImageCodecInfo = NULL;   
    GetImageEncodersSize(&num, &size);   
    if(size == 0)   
        return -1;    
    
    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));   
    if(pImageCodecInfo == NULL)   
        return -1;     
    
    GetImageEncoders(num, size, pImageCodecInfo);   
    for(UINT j = 0; j < num; ++j)   
    {   
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )   
        {   
            *pClsid = pImageCodecInfo[j].Clsid;   
            free(pImageCodecInfo);   
            return j;       
        }           
    }   
    free(pImageCodecInfo);   
    return -1;    
}

BOOL SC_GDI_Plus_SaveBitamp2File(HBITMAP hBitmap, LPCTSTR lpszFileName)
{	
	_ASSERTE(lpszFileName != NULL);
	CSCGDIPlusStartup gdiplus;
	if(!gdiplus.IsOK())
	{
		_ASSERTE(FALSE);
		return FALSE;
	}

	{
		int quality = 100;
		CString strType = GetExtFileTypeName(lpszFileName);
		if(strType == _T("")) return FALSE;
		if(strType == _T("JPG") || strType == _T("JPEG")) quality = 90;

		CString strFormatName = GetMimeType(strType);
		if(strFormatName ==_T("")) return FALSE;

		CLSID clsID;
		INT nIndex = GetEncoderClsid(strFormatName, &clsID);
		if(nIndex < 0) return FALSE;

		Bitmap bmp(hBitmap, NULL);
		EncoderParameters   encoderParameters;   
		
		encoderParameters.Count   =   1;   
		encoderParameters.Parameter[0].Guid   =   EncoderQuality;   
		encoderParameters.Parameter[0].Type   =   EncoderParameterValueTypeLong;   
		encoderParameters.Parameter[0].NumberOfValues   =   1;   
		encoderParameters.Parameter[0].Value   =   &quality;   
		bmp.Save(lpszFileName, &clsID, &encoderParameters);
		
	}
	return TRUE;
}
#endif //SC_USE_GDIPLUS


BOOL SC_SaveBitmap2File(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
#if SC_USE_CXIMAGE
	return SC_CXImage_SaveBitmap2File(hBitmap, lpszFileName);
#endif //SC_USE_CXIMAGE

#if SC_USE_GDIPLUS
	return SC_GDI_Plus_SaveBitamp2File(hBitmap, lpszFileName);
#endif //SC_USE_GDIPLUS

	return FALSE;
}