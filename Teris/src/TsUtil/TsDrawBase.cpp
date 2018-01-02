#include "TsDrawBase.h"

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



void DrawBorder(CDCHandle dc, RECT& rt, COLORREF clrLeftTop, COLORREF clrRightBottom, int nFrameWidth)
{
	for(int i=0; i<nFrameWidth; ++i)
	{
		int n = i + 1;
		RECT rtLeft = {rt.left + i, rt.top + i, rt.left + n, rt.bottom - n};
		dc.FillSolidRect( &rtLeft, clrLeftTop);

		RECT rtTop = {rt.left + i, rt.top + i, rt.right - n, rt.top + n};
		dc.FillSolidRect( &rtTop, clrLeftTop);

		RECT rtBottom = {rt.left + i, rt.bottom - n, rt.right - i, rt.bottom - i};
		dc.FillSolidRect( &rtBottom, clrRightBottom);

		RECT rtRight = {rt.right - n, rt.top + i, rt.right - i, rt.bottom - n};
		dc.FillSolidRect( &rtRight, clrRightBottom);
	}
}

HRGN BitmapToRgn(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
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

CSize GetBitmapSize(HBITMAP hbmp)
{
	_ASSERTE(hbmp != NULL);

	CSize szRet(0, 0);
	BITMAP bmp = {0};
	GetObject(hbmp, sizeof(bmp), &bmp);
	szRet.cx = bmp.bmWidth;
	szRet.cy = bmp.bmHeight;

	return szRet;
}
HFONT CreateTsFont(BOOL bBold, int nHeight, BOOL lfItalic /* = FALSE */, BOOL lfUnderline /* = FALSE */, LPCTSTR lpszName /* = _T */)
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
HFONT GetTsDefultFont(BOOL bBold /* = FALSE */)
{
	HFONT hFontRet = NULL;
	static CFont fnt1;
	static CFont fnt2;

	if(!bBold)
	{
		if(fnt1.IsNull())
		{
			fnt1 = CreateTsFont(FALSE, -12);
		}
		hFontRet = fnt1;
	}
	else
	{
		if(fnt2.IsNull())
		{
			fnt2 = CreateTsFont(TRUE, -12);
		}
		hFontRet = fnt2	;
	}

	return hFontRet;
}

VOID FillGradientColor(CDCHandle& dc, CRect& rect, COLORREF clrBegin, COLORREF clrEnd, BOOL bHor)
{
	CHYMemDC memDc(dc, &rect);
	
	int R1(GetRValue(clrBegin)), G1(GetGValue(clrBegin)), B1(GetBValue(clrBegin));
	int R2(GetRValue(clrEnd)), G2(GetGValue(clrEnd)), B2(GetBValue(clrEnd));
	int R(0), G(0) , B(0);
	const int c_nStep = 1;
	
	if(!bHor)
	{
		int nHeight = rect.Height();
		for(int i = 0; i < nHeight; i += c_nStep)
		{
			R = R1 + i * (R2 - R1) / nHeight;
			G = G1 + i * (G2 - G1) / nHeight;
			B = B1 + i * (B2 - B1) / nHeight;
			memDc.FillSolidRect(rect.left, rect.top + i, rect.Width(), c_nStep, RGB(R, G, B));
		}
	}
	else 
	{
		int nWidth = rect.Width();
		for(int i = 0; i < nWidth; i += c_nStep)
		{
			R = R1 + i * (R2 - R1) / nWidth;
			G = G1 + i * (G2 - G1) / nWidth;
			B = B1 + i * (B2 - B1) / nWidth;
			memDc.FillSolidRect(rect.left + i, rect.top, c_nStep, rect.Height(), RGB(R, G, B));
		}
	}
}