#pragma  once 

#include "TsStdAfx.h"

class CTsCommonFunction
{
public:
	static bool GetCurrentFilePath(LPTSTR pBuffer, int nBufferLen, LPCTSTR lpszName); //lpszName: abc.txt
};