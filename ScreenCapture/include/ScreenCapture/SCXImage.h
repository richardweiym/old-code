#pragma once 

#include "HYUtilBase.h"

BOOL SC_SaveBitmap2File(HBITMAP hBitmap, LPCTSTR lpszFileName);

BOOL SC_GDI_Plus_SaveBitamp2File(HBITMAP hBitmap, LPCTSTR lpszFileName);
BOOL SC_CXImage_SaveBitmap2File(HBITMAP hBitmap, LPCTSTR lpszFileName);