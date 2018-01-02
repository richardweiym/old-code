#include "hyutilbase.h"
#include "hyuibase.h"
#include "HYTimer.h"

///////////////////////// CHYMemDC //////////////////////////////
CHYMemDC::CHYMemDC(HDC hDC, LPRECT pRect)
{
	_ASSERT(hDC != NULL);

	m_hDCDest = hDC;
	if(pRect != NULL)
	{
		m_rtDraw = *pRect;
	}
	else
	{
		m_hDCDest.GetClipBox(&m_rtDraw);
	}

	CreateCompatibleDC(m_hDCDest);
	
	::LPtoDP(m_hDCDest, (LPPOINT)&m_rtDraw, sizeof(CRect)/sizeof(POINT));
	m_bmpDest.CreateCompatibleBitmap(m_hDCDest, m_rtDraw.Width(), m_rtDraw.Height());
	m_hBmpOld = SelectBitmap(m_bmpDest);
	::DPtoLP(m_hDCDest, (LPPOINT)&m_rtDraw, sizeof(CRect)/sizeof(POINT));

	SetMapMode(m_hDCDest.GetMapMode());
	SIZE szWin = {0};
	m_hDCDest.GetWindowExt(&szWin);
	SetWindowExt(szWin);
	SIZE szView = {0};
	m_hDCDest.GetViewportExt(&szView);
	SetViewportExt(szView);
	SetWindowOrg(m_rtDraw.TopLeft());

//	FillSolidRect(&m_rtDraw, ::GetBkColor(m_hDCDest));
}
	
CHYMemDC::~CHYMemDC()
{
	m_hDCDest.BitBlt(m_rtDraw.left, m_rtDraw.top, m_rtDraw.Width(), m_rtDraw.Height()
		, (HDC)*this, m_rtDraw.left, m_rtDraw.top, SRCCOPY);

	SelectBitmap(m_hBmpOld);
}
///////////////////////// CHYMemDC //////////////////////////////

CSize CHYUICommonFunctionWrapper::GetBitmapSize(HBITMAP hBmp)
{
	_ASSERTE(hBmp != NULL);
	_ASSERT(::GetObjectType(hBmp) == OBJ_BITMAP);

	CSize szRet(0, 0);
	BITMAP bmp = {0};
	GetObject(hBmp, sizeof(bmp), &bmp);
	szRet.cx = bmp.bmWidth;
	szRet.cy = bmp.bmHeight;

	return szRet;	
}
HFONT CHYUICommonFunctionWrapper::CreateFont(BOOL bBold, int nHeight, BOOL lfItalic /* = FALSE */, BOOL lfUnderline /* = FALSE */, LPCTSTR lpszName /* = _T */)
{
	LOGFONT lfDefFont = {0};

	lfDefFont.lfHeight = nHeight;    //-12
	lfDefFont.lfWidth = 0;
	lfDefFont.lfEscapement = 0;
	lfDefFont.lfOrientation = 0;
	if(bBold)
	{
		lfDefFont.lfWeight = FW_BOLD;
	}
	else
	{
		lfDefFont.lfWeight = FW_NORMAL;
	}
	LANGID gId=GetSystemDefaultLangID();
	
	if(gId==0x0411) //Japanese
		lfDefFont.lfCharSet = SHIFTJIS_CHARSET;
	else if(gId==0x0404 || gId==0x0c04 || gId==0x1004) // big 5
		lfDefFont.lfCharSet = CHINESEBIG5_CHARSET;
	else if(gId==0x0804 || gId==0x0c04 || gId==0x1004)
		lfDefFont.lfCharSet = GB2312_CHARSET;
	else if(gId==0x0412 || gId==0x0812)
		lfDefFont.lfCharSet = ANSI_CHARSET;
	else
		lfDefFont.lfCharSet =  ANSI_CHARSET;
	lfDefFont.lfItalic = lfItalic;
	lfDefFont.lfUnderline = lfUnderline;
	lfDefFont.lfStrikeOut = FALSE;
	lfDefFont.lfPitchAndFamily = DEFAULT_PITCH;
	lfDefFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfDefFont.lfClipPrecision  = CLIP_DEFAULT_PRECIS; 
	lfDefFont.lfQuality = DEFAULT_QUALITY; 
	lstrcpyn( lfDefFont.lfFaceName, lpszName, sizeof(lfDefFont.lfFaceName)/sizeof(TCHAR) );
	
	return CreateFontIndirect(&lfDefFont);
}

HFONT CHYUICommonFunctionWrapper::GetDefaultFont(BOOL bBold /* = FALSE */)
{
	HFONT hFontRet = NULL;
	static CFont fnt1;
	static CFont fnt2;

	if(!bBold)
	{
		if(fnt1.IsNull())
		{
			fnt1 = CHYUICommonFunctionWrapper::CreateFont(FALSE, -12);
		}
		hFontRet = fnt1;
	}
	else
	{
		if(fnt2.IsNull())
		{
			fnt2 = CHYUICommonFunctionWrapper::CreateFont(TRUE, -12);
		}
		hFontRet = fnt2	;
	}

	return hFontRet;
}

HRGN CHYUICommonFunctionWrapper::BitmapToRgn(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
{
	HRGN hRgn = NULL;
	// return hRgn;

	if (hBmp)
	{
		// Create a memory DC inside which we will scan the bitmap content
		HDC hMemDC = ::CreateCompatibleDC(NULL);
		if (hMemDC)
		{
			// Get bitmap size
			BITMAP bm;
			::GetObject(hBmp, sizeof(bm), &bm);

			// Create a 32 bits depth bitmap and select it into the memory DC 
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
					sizeof(BITMAPINFOHEADER),	// biSize 
					bm.bmWidth,					// biWidth; 
					bm.bmHeight,				// biHeight; 
					1,							// biPlanes; 
					32,							// biBitCount 
					BI_RGB,						// biCompression; 
					0,							// biSizeImage; 
					0,							// biXPelsPerMeter; 
					0,							// biYPelsPerMeter; 
					0,							// biClrUsed; 
					0							// biClrImportant; 
			};
			VOID * pbits32; 
			HBITMAP hbm32 = ::CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			if (hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)::SelectObject(hMemDC, hbm32);

				// Create a DC just to copy the bitmap into the memory DC
				HDC hDC = ::CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
					BITMAP bm32;
					::GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;

					// Copy the bitmap into the memory DC
					HBITMAP holdBmp = (HBITMAP)::SelectObject(hDC, hBmp);
					::BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

					// For better performances, we will use the ExtCreateRegion() function to create the
					// region. This function take a RGNDATA structure on entry. We will add rectangles by
					// amount of ALLOC_UNIT number in this structure.
					#define ALLOC_UNIT	100
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *)::GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

					// Keep on hand highest and lowest values for the "transparent" pixels
					BYTE lr = GetRValue(cTransparentColor);
					BYTE lg = GetGValue(cTransparentColor);
					BYTE lb = GetBValue(cTransparentColor);
					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
					BYTE hb = min(0xff, lb + GetBValue(cTolerance));

					// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						// Scan each bitmap pixel from left to right
						for (int x = 0; x < bm.bmWidth; x++)
						{
							// Search for a continuous range of "non transparent pixels"
							int x0 = x;
							LONG *p = (LONG *)p32 + x;
							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								if (b >= lr && b <= hr)
								{
									b = GetGValue(*p);
									if (b >= lg && b <= hg)
									{
										b = GetBValue(*p);
										if (b >= lb && b <= hb)
											// This pixel is "transparent"
											break;
									}
								}
								p++;
								x++;
							}

							if (x > x0)
							{
								// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
								if (pData->rdh.nCount >= maxRects)
								{
									::GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = ::GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *)::GlobalLock(hData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								::SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y+1;
								pData->rdh.nCount++;

								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
								// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									if (hRgn)
									{
										::CombineRgn(hRgn, hRgn, h, RGN_OR);
										::DeleteObject(h);
									}
									else
										hRgn = h;
									pData->rdh.nCount = 0;
									::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// Go to next row (remember, the bitmap is inverted vertically)
						p32 -= bm32.bmWidthBytes;
					}

					// Create or extend the region with the remaining rectangles
					HRGN h = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					if (hRgn)
					{
						::CombineRgn(hRgn, hRgn, h, RGN_OR);
						::DeleteObject(h);
					}
					else
						hRgn = h;

					// Clean up
					::GlobalUnlock(hData);
					::GlobalFree(hData);
					::SelectObject(hDC, holdBmp);
					::DeleteDC(hDC);
				}

				::DeleteObject(::SelectObject(hMemDC, holdBmp));
			}

			::DeleteDC(hMemDC);
		}	
	}

	return hRgn;	
}

BOOL CHYCommonFunctionWrapper::GetCurrentFilePath(LPTSTR pBuffer, int nBufferLen, LPCTSTR lpszName)
{
	_ASSERTE(pBuffer != NULL && nBufferLen > 0 && lpszName != NULL);
	
	BOOL bRet(FALSE);
	DWORD dwLen = GetModuleFileName(NULL, pBuffer, nBufferLen);
	if(dwLen > 0)
	{
		int nTotal = dwLen + lstrlen(lpszName);
		if(nBufferLen > nTotal)
		{
			TCHAR* p = _tcsrchr(pBuffer, _T('\\'));
			if(p != NULL)
			{
				*++p = _T('\0');
				_tcsncat(pBuffer, lpszName, nBufferLen - dwLen);
				bRet = TRUE;
			}
		}
	}

	return bRet;
}


VOID CHYUICommonFunctionWrapper::GradientFill(CDCHandle dc, LPRECT pRect, COLORREF clrStart, COLORREF clrEnd, BOOL bVert /* = FALSE */)
{
	TRIVERTEX vert[2] ;
	GRADIENT_RECT    gRect;

	vert [0] .x      = pRect->left;
	vert [0] .y      = pRect->top;
	vert [0] .Red    = GetRValue(clrStart) << 8;
	vert [0] .Green  = GetGValue(clrStart) << 8;
	vert [0] .Blue   = GetBValue(clrStart) << 8;
	vert [0] .Alpha  = 0x0000;

	vert [1] .x      = pRect->right;
	vert [1] .y      = pRect->bottom; 
	vert [1] .Red    = GetRValue(clrEnd) << 8;
	vert [1] .Green  = GetGValue(clrEnd) << 8;
	vert [1] .Blue   = GetBValue(clrEnd) << 8;
	vert [1] .Alpha  = 0x0000;
	
	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;
	
	if(!bVert)
	{
		::GradientFill(dc,vert,2,&gRect,1,GRADIENT_FILL_RECT_H);
	}
	else
	{
		::GradientFill(dc,vert,2,&gRect,1,GRADIENT_FILL_RECT_V);
	}
}
VOID CHYUICommonFunctionWrapper::FillBmp(CDCHandle dc, CRect* rtDraw, CBitmapHandle bmp)
{
#ifdef _DEBUG
	CHYTimeCostCalculator timer(_T("CHYUICommonFunctionWrapper::FillBmp()"));
#endif

	CBrush brushNew = CreatePatternBrush(bmp);
	dc.FillRect(rtDraw, brushNew);
}
VOID CHYUICommonFunctionWrapper::DrawBmpInCenter(CDCHandle dc, CRect* pRtDraw, CBitmapHandle bmp, BOOL bTransparent)
{
	_ASSERTE(pRtDraw != NULL && !bmp.IsNull());
	CSize sizeBmp = CHYUICommonFunctionWrapper::GetBitmapSize(bmp);
	
	INT nX = pRtDraw->left;
	INT nY = pRtDraw->top;
	int nDrawWidth = pRtDraw->Width();
	int nDrawHeight = pRtDraw->Height();

	if(sizeBmp.cx < pRtDraw->Width())
	{
		nX += (pRtDraw->Width() - sizeBmp.cx) / 2;
		nDrawWidth = sizeBmp.cx;
	}

	if(sizeBmp.cy < pRtDraw->Height())
	{
		nY += (pRtDraw->Height() - sizeBmp.cy ) / 2;
		nDrawHeight = sizeBmp.cy;
	}


	CDC dcTemp = CreateCompatibleDC(dc);
	HBITMAP hOldBmp = dcTemp.SelectBitmap(bmp);
	
	if(!bTransparent)
	{
		dc.BitBlt(nX, nY, nDrawWidth, nDrawHeight, dcTemp, 0, 0, SRCCOPY);
	}
	else
	{
		TransparentBlt(dc, nX, nY, nDrawWidth, nDrawHeight, dcTemp, 0, 0, nDrawWidth, nDrawHeight, RGB(255, 0, 255));
	}

	dcTemp.SelectBitmap(hOldBmp);
}
VOID CHYUICommonFunctionWrapper::DrawBmp(CDCHandle dc, CRect* pRtDraw, CBitmapHandle bmp, BOOL bStretch /* = FALSE */, BOOL bTransparent)
{
	CSize sizeBmp = CHYUICommonFunctionWrapper::GetBitmapSize(bmp);
	CDC dcTemp = CreateCompatibleDC(dc);
	HBITMAP hOldBmp = dcTemp.SelectBitmap(bmp);

	if(!bTransparent)
	{
		if(bStretch)
		{
			DWORD dwMode = dc.SetStretchBltMode(HALFTONE);
			dc.StretchBlt(pRtDraw->left, pRtDraw->top, pRtDraw->Width(), pRtDraw->Height(), dcTemp, 0, 0, sizeBmp.cx, sizeBmp.cy, SRCCOPY);
			dc.SetStretchBltMode(dwMode);
		}
		else
		{
			dc.BitBlt(pRtDraw->left, pRtDraw->top, sizeBmp.cx, sizeBmp.cy, dcTemp, 0, 0, SRCCOPY);
		}
	}
	else
	{
		TransparentBlt(dc, pRtDraw->left, pRtDraw->top, pRtDraw->Width(), pRtDraw->Height(), dcTemp, 0, 0, sizeBmp.cx, sizeBmp.cy, RGB(255, 0, 255));
	}
	
	dcTemp.SelectBitmap(hOldBmp);
}
CBitmapHandle CHYUICommonFunctionWrapper::GetSubBmp(CBitmapHandle bmpTarget, CRect rtTarget)
{
	CClientDC	 dc(NULL);
	CDC dcMem1, dcMem2;
	dcMem1.CreateCompatibleDC(dc);
	dcMem2.CreateCompatibleDC(dc);

	CBitmapHandle bmpOld1 = dcMem1.SelectBitmap(bmpTarget);
	CBitmapHandle bmpRet = CreateCompatibleBitmap(dc, rtTarget.Width(), rtTarget.Height());
	CBitmapHandle bmpOld2 = dcMem2.SelectBitmap(bmpRet);
	dcMem2.BitBlt(0, 0, rtTarget.Width(), rtTarget.Height(), dcMem1, rtTarget.left, rtTarget.top, SRCCOPY);
	dcMem1.SelectBitmap(bmpOld1);
	dcMem2.SelectBitmap(bmpOld2);

	return bmpRet;
}
CBitmapHandle CHYUICommonFunctionWrapper::GetMiddleStretchBmp(CBitmapHandle bmp, int nLen, BOOL bVert)
{
	CSize sizeBmp = CHYUICommonFunctionWrapper::GetBitmapSize(bmp);
	if(!bVert)
	{
		_ASSERTE(nLen > sizeBmp.cx);
		CBitmap bmpLeft = GetSubBmp(bmp, CRect(0, 0, sizeBmp.cx / 2 - 1, sizeBmp.cy));
		CBitmap bmpMiddle = GetSubBmp(bmp, CRect(sizeBmp.cx / 2 - 1, 0, sizeBmp.cx / 2 + 1, sizeBmp.cy));
		CBitmap bmpRight = GetSubBmp(bmp, CRect(sizeBmp.cx / 2 + 1, 0, sizeBmp.cx, sizeBmp.cy));

		CClientDC dc(NULL);
		CDC dcMem = CreateCompatibleDC(dc);
		CBitmapHandle bmpMem = CreateCompatibleBitmap(dc, nLen, sizeBmp.cy);
		CBitmapHandle bmpOld = dcMem.SelectBitmap(bmpMem);
		CRect rtLeft(0, 0, sizeBmp.cx / 2 - 1, sizeBmp.cy);
		CRect rtMiddle(sizeBmp.cx / 2 - 1, 0, nLen - sizeBmp.cx / 2 + 1, sizeBmp.cy);
		CRect rtRight(nLen - sizeBmp.cx / 2 + 1, 0, nLen, sizeBmp.cy);
		DrawBmp((HDC)dcMem, &rtLeft, (HBITMAP)bmpLeft, FALSE, FALSE);
		FillBmp((HDC)dcMem, &rtMiddle, (HBITMAP)bmpMiddle);
		DrawBmp((HDC)dcMem, &rtRight, (HBITMAP)bmpRight, FALSE, FALSE);
		dcMem.SelectBitmap(bmpOld);

		return bmpMem;
	}
	else
	{
		_ASSERTE(nLen > sizeBmp.cy);
		CBitmap bmpTop = GetSubBmp(bmp, CRect(0, 0, sizeBmp.cx, sizeBmp.cy / 2 - 1));
		CBitmap bmpMiddle = GetSubBmp(bmp, CRect(0, sizeBmp.cy / 2 - 1, sizeBmp.cx, sizeBmp.cy / 2 + 1));
		CBitmap bmpBottom = GetSubBmp(bmp, CRect(0, sizeBmp.cy / 2 + 1, sizeBmp.cx, sizeBmp.cy));

		CClientDC dc(NULL);
		CDC dcMem = CreateCompatibleDC(dc);
		CBitmapHandle bmpMem = CreateCompatibleBitmap(dc, sizeBmp.cx, nLen);
		CBitmapHandle bmpOld = dcMem.SelectBitmap(bmpMem);
		CRect rtTop(0, 0, sizeBmp.cx, sizeBmp.cy / 2 - 1);
		CRect rtMiddle(0, sizeBmp.cy / 2 - 1, sizeBmp.cx, nLen - sizeBmp.cy / 2 + 1);
		CRect rtBottom(0, nLen - sizeBmp.cy / 2 + 1, sizeBmp.cx, nLen);
		DrawBmp((HDC)dcMem, &rtTop, (HBITMAP)bmpTop, FALSE, FALSE);
		FillBmp((HDC)dcMem, &rtMiddle, (HBITMAP)bmpMiddle);
		DrawBmp((HDC)dcMem, &rtBottom, (HBITMAP)bmpBottom, FALSE, FALSE);
		dcMem.SelectBitmap(bmpOld);

		return bmpMem;
	}
}
CBitmapHandle CHYUICommonFunctionWrapper::GetMiddleStretchBmp(CBitmapHandle bmp, CSize sizeTarget)
{
	CSize sizeBmp = CHYUICommonFunctionWrapper::GetBitmapSize(bmp);

	if(sizeBmp.cx >= sizeTarget.cx && sizeBmp.cy >= sizeTarget.cy)
	{
		return GetSubBmp(bmp, CRect(0, 0, sizeTarget.cx, sizeTarget.cy));
	}

	CBitmap bmpHorStretch;
	if(sizeTarget.cx > sizeBmp.cx)
	{
		bmpHorStretch = GetMiddleStretchBmp(bmp, sizeTarget.cx, FALSE);	
	}
	
	CBitmap bmpStretch;
	if(sizeTarget.cy > sizeBmp.cy)
	{
		if(!bmpHorStretch.IsNull())	
		{
			bmpStretch = GetMiddleStretchBmp((HBITMAP)bmpHorStretch, sizeTarget.cy, TRUE);
		}
		else
		{
			bmpStretch = GetMiddleStretchBmp((HBITMAP)bmp, sizeTarget.cy, TRUE);
		}
	}
	else
	{
		bmpStretch = bmpHorStretch.Detach();
	}

	return bmpStretch.Detach();
}
VOID CHYUICommonFunctionWrapper::MiddleStretchDrawBmp(CDCHandle dc, CRect* rtDraw, CBitmapHandle bmp)
{
	CBitmap bmpStretch = GetMiddleStretchBmp(bmp, CSize(rtDraw->Width(), rtDraw->Height()));

	DrawBmp((HDC)dc, rtDraw, (HBITMAP)bmpStretch, FALSE, TRUE);
}

VOID CHYUICommonFunctionWrapper::DrawFrame(CDCHandle dc, CRect& rtDraw, COLORREF clrFrame, INT nFrameWidth)
{
	CPen newPen = CreatePen(PS_SOLID | PS_INSIDEFRAME, nFrameWidth, clrFrame);
	CPenHandle penOld = dc.SelectPen(newPen);
	CBrushHandle brushOld = dc.SelectBrush((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.Rectangle(&rtDraw);
	dc.SelectBrush( brushOld);
	dc.SelectPen(penOld);
}

BOOL CHYUICommonFunctionWrapper::SaveBMP2File(HBITMAP hBitmap, LPCTSTR lpszFileName )
{
	_ASSERTE(hBitmap != NULL && lpszFileName != NULL);

	 HDC hDC;
	 int iBits;
	 WORD wBitCount;
	 DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0; 
	 BITMAP Bitmap;  
	 BITMAPFILEHEADER bmfHdr;  
	 BITMAPINFOHEADER bi;  
	 LPBITMAPINFOHEADER lpbi;  
	 HANDLE fh, hDib, hPal,hOldPal=NULL; 
 
	 hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	 iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES); 
	 DeleteDC(hDC); 
	 if (iBits <= 1)  wBitCount = 1; 
	 else if (iBits <= 4)  wBitCount = 4; 
	 else if (iBits <= 8)  wBitCount = 8; 
	 else      wBitCount = 24; 
 
	 GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	 bi.biSize   = sizeof(BITMAPINFOHEADER);
	 bi.biWidth   = Bitmap.bmWidth;
	 bi.biHeight   = Bitmap.bmHeight;
	 bi.biPlanes   = 1;
	 bi.biBitCount  = wBitCount;
	 bi.biCompression = BI_RGB;
	 bi.biSizeImage  = 0;
	 bi.biXPelsPerMeter = 0;
	 bi.biYPelsPerMeter = 0;
	 bi.biClrImportant = 0;
	 bi.biClrUsed  = 0;
 
	 dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
 
	 hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)); 
	 lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
	 *lpbi = bi; 

	 hPal = GetStockObject(DEFAULT_PALETTE); 
	 if (hPal) 
	 { 
		  hDC = ::GetDC(NULL); 
		  hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
		  RealizePalette(hDC); 
	 }

	 GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
	   +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS); 

	 if (hOldPal) 
	 { 
		  ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
		  RealizePalette(hDC); 
		  ::ReleaseDC(NULL, hDC); 
	 } 

	 fh = CreateFile(lpszFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS, 
		 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 
 
	 if (fh == INVALID_HANDLE_VALUE)  return FALSE; 
 
	 bmfHdr.bfType = 0x4D42; // "BM" 
	 dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  
	 bmfHdr.bfSize = dwDIBSize; 
	 bmfHdr.bfReserved1 = 0; 
	 bmfHdr.bfReserved2 = 0; 
	 bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; 

	 WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 

	 WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL); 

	 GlobalUnlock(hDib); 
	 GlobalFree(hDib); 
	 CloseHandle(fh); 

	 return TRUE;
}


BOOL CHYUICommonFunctionWrapper::SetLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags )
{
	BOOL bRet(FALSE);

	typedef BOOL (WINAPI * SetLayeredWindowAttributesProc)(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	SetLayeredWindowAttributesProc pFun = NULL;
	
	HMODULE hModUser32 = GetModuleHandle(_T("User32.dll"));
	if(hModUser32 != NULL)
	{
		pFun = (SetLayeredWindowAttributesProc)GetProcAddress(hModUser32, "SetLayeredWindowAttributes");
		if(pFun != NULL)
		{
			bRet = pFun(hwnd, crKey, bAlpha, dwFlags);
		}
	}

	return bRet;
}

BOOL CHYUICommonFunctionWrapper::UpdateLayeredWindow(HWND hwnd, HDC hdcDst, POINT *pptDst, SIZE *psize,HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags)
{
	BOOL bRet(FALSE);

	typedef BOOL (WINAPI * UpdateLayeredWindowProc)(HWND hwnd, HDC hdcDst, POINT *pptDst, SIZE *psize,HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);
	UpdateLayeredWindowProc pFun = NULL;

	HMODULE hModUser32 = GetModuleHandle(_T("User32.dll"));
	if(hModUser32 != NULL)
	{
		pFun = (UpdateLayeredWindowProc)GetProcAddress(hModUser32, "UpdateLayeredWindow");
		if(pFun != NULL)
		{
			bRet = pFun( hwnd,  hdcDst,  pptDst,  psize, hdcSrc,  pptSrc,  crKey,  pblend,  dwFlags);
		}
	}

	return bRet;
}