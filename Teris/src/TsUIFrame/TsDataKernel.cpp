#include "TsDataKernel.h"
#include <crtdbg.h>

BYTE g_arDataTable[TS_ELEMENT_COUNT][TS_SWITCH_COUNT][TS_ELEMENT_WIDHT * TS_ELEMENT_WIDHT] = 
{
// 	口
// 	口
// 	口
// 	口
	0, 1, 0, 0,
	0, 1, 0, 0,
	0, 1, 0, 0,
	0, 1, 0, 0,

	0, 0, 0, 0, 
	1, 1, 1, 1, 
	0, 0, 0, 0,
	0, 0, 0, 0,

	0, 0, 1, 0,	
	0, 0, 1, 0, 
	0, 0, 1, 0,	
	0, 0, 1, 0,

	0, 0, 0, 0,
	0, 0, 0, 0,
	1, 1, 1, 1,
	0, 0, 0, 0,
//
//口口
//口口
//
	0, 0, 0, 0,
	0, 1, 1, 0,
	0, 1, 1, 0, 
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 1, 1, 0, 
	0, 1, 1, 0, 
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 1, 1, 0, 
	0, 1, 1, 0, 
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 1, 1, 0,
	0, 1, 1, 0, 
	0, 0, 0, 0,

//口口
//口
//口
//
	0, 1, 1, 0,
	0, 1, 0, 0,
	0, 1, 0, 0,
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 1, 1, 1,
	0, 0, 0, 1, 
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 0, 1, 0, 
	0, 0, 1, 0,
	0, 1, 1, 0,

	0, 0, 0, 0,
	1, 0, 0, 0,
	1, 1, 1, 0,
	0, 0, 0, 0,

//
//  口
//口口口
//
	0, 0, 1, 0,
	0, 1, 1, 0,
	0, 0, 1, 0, 
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 0, 1, 0, 
	0, 1, 1, 1, 
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 1, 0, 0,
	0, 1, 1, 0,
	0, 1, 0, 0,

	0, 0, 0, 0, 
	1, 1, 1, 0, 
	0, 1, 0, 0, 
	0, 0, 0, 0,



//口口
//  口
//  口
//
	0, 1, 1, 0, 
	0, 0, 1, 0,
	0, 0, 1, 0, 
	0, 0, 0, 0,

	0, 0, 0, 0, 
	0, 0, 0, 1,
	0, 1, 1, 1,
	0, 0, 0, 0, 

	0, 0, 0, 0,
	0, 1, 0, 0,
	0, 1, 0, 0, 
	0, 1, 1, 0,

	0, 0, 0, 0,
	1, 1, 1, 0, 
	1, 0, 0, 0, 
	0, 0, 0, 0, 

//  口
//口口
//口
//
	0, 0, 1, 0, 
	0, 1, 1, 0,
	0, 1, 0, 0,
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 1, 1, 0, 
	0, 0, 1, 1,
	0, 0, 0, 0,

	0, 0, 0, 0, 
	0, 0, 1, 0,
	0, 1, 1, 0, 
	0, 1, 0, 0,

	0, 0, 0, 0,
	1, 1, 0, 0,
	0, 1, 1, 0, 
	0, 0, 0, 0,


//口
//口口
//  口
//
	0, 1, 0, 0,
	0, 1, 1, 0,
	0, 0, 1, 0,
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 0, 1, 1, 
	0, 1, 1, 0, 
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 1, 0, 0, 
	0, 1, 1, 0,
	0, 0, 1, 0,

	0, 0, 0, 0,
	0, 1, 1, 0, 
	1, 1, 0, 0,
	0, 0, 0, 0
};	

COLORREF g_arColorTable[TS_ELEMENT_COUNT][3] = 
{
// 	口
// 	口
// 	口
// 	口
	RGB(191, 0, 0), RGB(255, 0, 0), RGB(128, 0, 0),

//
//口口
//口口
//
	RGB(0, 191, 191), RGB(0, 255, 255), RGB(0, 128, 128),

//口口
//口
//口
//
	RGB(191, 0, 191), RGB(255, 0, 255), RGB(128, 0, 128),

//
//  口
//口口口
//
	RGB(115, 115, 115), RGB(153, 153, 153), RGB(76, 76, 76),

//口口
//  口
//  口
//
	RGB(191, 191, 0), RGB(255, 255, 0), RGB(128, 128, 0),

//  口
//口口
//口
//
	RGB(0, 191, 0), RGB(0, 255, 0), RGB(0, 128, 0),
//口
//口口
//  口
//
	RGB(0, 0, 191), RGB(0, 0, 255), RGB(0, 0, 128)
};

INT CTsDataSourceBase::GetEleTypeCount()
{
	return TS_ELEMENT_COUNT;
}
INT CTsDataSourceBase::GetIdByIndex(INT nIndex)
{
	_ASSERTE(nIndex >= 0 && nIndex < TS_ELEMENT_COUNT);
	return nIndex;
}
BOOL CTsDataSourceBase::GetDataByIndex(BYTE arBuffer[TS_ELEMENT_WIDHT * TS_ELEMENT_WIDHT * TS_SWITCH_COUNT], INT nBufferLen, INT nIndex)
{
	const int conDataLen = TS_ELEMENT_WIDHT * TS_ELEMENT_WIDHT * TS_SWITCH_COUNT;

	_ASSERTE(nIndex >= 0 && nIndex < TS_ELEMENT_COUNT);
	if(arBuffer == NULL|| nBufferLen < conDataLen)
	{
		return FALSE;
	}
	
	BYTE* pData = &(g_arDataTable[nIndex][0][0]);
	memcpy(arBuffer, pData, conDataLen);
	
	return TRUE;
}

COLORREF CTsDataBlackSource::GetFaceColorByIndex(INT nIndex)
{
	_ASSERTE(nIndex >= 0 && nIndex < TS_ELEMENT_COUNT);
	return RGB(115, 115, 115);
}
COLORREF CTsDataBlackSource::GetLeftTopColorByIndex(INT nIndex)
{
	_ASSERTE(nIndex >= 0 && nIndex < TS_ELEMENT_COUNT);
	return RGB(153, 153, 153);
}
COLORREF CTsDataBlackSource::GetRightBottomColorByIndex(INT nIndex)
{
	_ASSERTE(nIndex >= 0 && nIndex < TS_ELEMENT_COUNT);
	return RGB(76, 76, 76);
}

COLORREF CTsDataColorSource::GetFaceColorByIndex(INT nIndex)
{
	_ASSERTE(nIndex >= 0 && nIndex < TS_ELEMENT_COUNT);
	return g_arColorTable[nIndex][0];	
}

COLORREF CTsDataColorSource::GetLeftTopColorByIndex(INT nIndex)
{
	_ASSERTE(nIndex >= 0 && nIndex < TS_ELEMENT_COUNT);
	return g_arColorTable[nIndex][1];
}
COLORREF CTsDataColorSource::GetRightBottomColorByIndex(INT nIndex)
{
	_ASSERTE(nIndex >= 0 && nIndex < TS_ELEMENT_COUNT);
	return g_arColorTable[nIndex][2]; 
}


CTsDataFactory* g_gameDataFactory = NULL;

CTsDataFactory::CTsDataFactory()
{
	TS_RUN_ONCE_CHECK
		
	SetEmpty();

	_ASSERTE(g_gameDataFactory == NULL);
	g_gameDataFactory = this;
}
CTsDataFactory::~CTsDataFactory()
{
	g_gameDataFactory = NULL;
}
VOID CTsDataFactory::SetEmpty()
{
	for( int i=0; i<GetElementCount(); ++i)
	{
		CTsTypeElement& item = m_vecPool[i];
		item.m_clrFace = item.m_clrLeftTopFrame = item.m_clrRightBottomFrame = RGB(0, 0, 0);
		item.m_dwID = 0;
	}
	m_bInited = FALSE;
}
BOOL CTsDataFactory::Init(CTsDataSourceBase& ts)
{
	if(m_bInited) Release();

	INT nCount = ts.GetEleTypeCount();
	_ASSERTE(nCount == GetElementCount());

	for(INT i=0; i<nCount; ++i)
	{
		CTsTypeElement&  ele = m_vecPool[i];
		ele.m_dwID = ts.GetIdByIndex(i);
		ele.m_clrFace = ts.GetFaceColorByIndex(i);
		ele.m_clrLeftTopFrame = ts.GetLeftTopColorByIndex(i);
		ele.m_clrRightBottomFrame = ts.GetRightBottomColorByIndex(i);
		BOOL bRet = ts.GetDataByIndex((BYTE*)ele.m_arData, sizeof(ele.m_arData), i);
		_ASSERTE(bRet);
	}

	m_bInited = TRUE;

	return m_bInited;
}

VOID CTsDataFactory::Release()
{
	SetEmpty();
}

INT CTsDataFactory::GetElementCount()
{
	return sizeof(m_vecPool) / sizeof(m_vecPool[0]);
}

CTsTypeElement* CTsDataFactory::GetElementByID(DWORD dwId)
{
	_ASSERTE(m_bInited);
	CTsTypeElement* pRet = NULL;
	
	for(int i=0; i<GetElementCount(); ++i)
	{
		CTsTypeElement& item = m_vecPool[i];
		if(item.m_dwID == dwId)
		{
			pRet = &item;
			break;
		}
	}

	return pRet;
}

CTsTypeElement* CTsDataFactory::GetElementByIndex(INT nIndex)
{
	_ASSERTE(m_bInited);
	_ASSERTE(nIndex >=0 && nIndex < GetElementCount());

	return &m_vecPool[nIndex];
}

INT CTsField::s_nFieldWidth = TS_MIN_FIELD;

CTsField& CTsField::operator = (CTsTypeElement* pEle)
{
	if(pEle != m_pEle)
	{
		m_pEle = pEle;
	}

	return *this;
}

COLORREF CTsField::GetFaceColor()
{
	COLORREF clrRet(RGB(0, 0, 0));
	_ASSERTE(m_pEle != NULL);
	if(m_pEle != NULL)
	{
		clrRet = m_pEle->m_clrFace;
	}

	return clrRet;
}

COLORREF CTsField::GetLeftTopFrameColor()
{
	COLORREF clrRet(RGB(0, 0, 0));
	_ASSERTE(m_pEle != NULL);
	if(m_pEle != NULL)
	{
		clrRet = m_pEle->m_clrLeftTopFrame;
	}

	return clrRet;
}

COLORREF CTsField::GetRightBottomFrameColor()
{
	COLORREF clrRet(RGB(0, 0, 0));
	_ASSERTE(m_pEle != NULL);
	if(m_pEle != NULL)
	{
		clrRet = m_pEle->m_clrRightBottomFrame;
	}

	return clrRet;
}


VOID CTsField::SetFieldWidth(int nValue)
{
	_ASSERTE(nValue >= TS_MIN_FIELD);
	s_nFieldWidth = nValue;
}

INT CTsField::GetFieldWidth()
{
	_ASSERTE(s_nFieldWidth >= TS_MIN_FIELD);

	return s_nFieldWidth;
}

INT CTsField::GetFrameWidth()
{
	_ASSERTE(s_nFieldWidth >= TS_MIN_FIELD);

	INT nRet = s_nFieldWidth / 10;

	return nRet;
}
INT CTsField::GetGapWidth()
{
	return 1;
}
TsSwitchType CTsMovingElement::SetCurrentType(TsSwitchType eType)
{
	TsSwitchType eOld(m_eType);

	m_eType = eType;

	return eOld;
}
TsSwitchType CTsMovingElement::SetToNextType()
{
	m_eType = GetNextType();

	return m_eType;
}
TsSwitchType CTsMovingElement::GetPrevType()
{
	int nTemp(m_eType);

	if( --nTemp < TS_SWITCH_1)
	{
		nTemp = TS_SWITCH_4;
	}

	return (TsSwitchType)nTemp;
	
}

TsSwitchType CTsMovingElement::GetNextType()
{
	int nTemp(m_eType);
	if(++nTemp > TS_SWITCH_4)
	{
		nTemp = TS_SWITCH_1;
	}

	return (TsSwitchType)nTemp;
}
TsSwitchType CTsMovingElement::SetToPrevType()
{
	m_eType = GetPrevType();

	return m_eType;
}

CRect CTsMovingElement::GetElementRect()
{
	CRect rtRet(0, 0, 0, 0);
	
	_ASSERTE(m_pCurrent != NULL);
	if(m_pCurrent != NULL)
	{
		INT nFieldWidth = CTsField::GetFieldWidth();

		rtRet.left = m_ptPos.x * nFieldWidth;
		rtRet.top = m_ptPos.y * nFieldWidth;

		rtRet.right = rtRet.left + TS_ELEMENT_WIDHT * nFieldWidth;
		rtRet.bottom = rtRet.top + TS_ELEMENT_WIDHT * nFieldWidth;
	}

	return rtRet;
}
CRect CTsMovingElement::GetSubFieldRect(int nX, int nY)
{
	CRect rtRet(0, 0, 0, 0);
	_ASSERTE(m_pCurrent != NULL);
	
	CRect rtEle = GetElementRect();
	INT nFieldWidth = CTsField::GetFieldWidth();

	if(!rtEle.IsRectEmpty())
	{
		rtRet.left = nX * nFieldWidth + rtEle.left;
		rtRet.top = nY * nFieldWidth + rtEle.top;

		rtRet.right = rtRet.left + nFieldWidth;
		rtRet.bottom = rtRet.top + nFieldWidth;
	}

	return rtRet;
}
CTsBinData* CTsMovingElement::GetCurrentBinData()
{
	CTsBinData* pData = NULL;
	if(!IsEmpty())
	{
		pData = &(m_pCurrent->m_arData)[m_eType];
	}

	return pData;
}


CTsGroundDataMgr::CTsGroundDataMgr()
{
	SetEmpty();
}
VOID CTsGroundDataMgr::SetEmpty()
{
	for(int i=0; i<TS_GROUND_HEIGHT; ++i)
	{
		for(int j=0; j<TS_GROUND_WIDTH; ++j)
		{
			m_arData[i][j].SetEmpty();
		}
	}
}
BOOL CTsGroundDataMgr::InitData(CTsField* pData, INT nCount)
{
	_ASSERTE(pData != NULL);
	_ASSERTE(nCount >= TS_GROUND_HEIGHT * TS_GROUND_WIDTH);

	for(int i=0; i<TS_GROUND_HEIGHT; ++i)
	{
		for(int j=0; j<TS_GROUND_WIDTH; ++j)
		{
			m_arData[i][j] = pData[i*TS_GROUND_WIDTH + j];
		}
	}

	return TRUE;
}

CTsField* CTsGroundDataMgr::GetField(int nXIndex, int nYIndex)
{
	_ASSERTE(nXIndex >= 0 && nYIndex >= 0);
	_ASSERTE(nXIndex < TS_GROUND_WIDTH);
	_ASSERTE(nYIndex < TS_GROUND_HEIGHT);

	return &m_arData[nYIndex][nXIndex];
}

VOID CTsGroundDataMgr::SetField(INT nXIndex, INT nYIndex, CTsField field)
{
	_ASSERTE(nXIndex >= 0 && nYIndex >= 0);
	_ASSERTE(nXIndex < TS_GROUND_WIDTH);
	_ASSERTE(nYIndex < TS_GROUND_HEIGHT);

	m_arData[nYIndex][nXIndex] = field;
}

CRect CTsGroundDataMgr::GetFieldRect(int nXIndex, int nYIndex)
{
	int nFieldWidth = CTsField::GetFieldWidth();
	
	CRect rtRect;
	rtRect.SetRectEmpty();
	rtRect.left = nXIndex * nFieldWidth;
	rtRect.top = nYIndex * nFieldWidth;
	rtRect.right = rtRect.left + nFieldWidth;
	rtRect.bottom = rtRect.top + nFieldWidth;

	return rtRect;
}
VOID CTsGroundDataMgr::RemoveLine(INT nIndex)
{
	_ASSERTE(nIndex >= 0 && nIndex < TS_GROUND_HEIGHT);

	for( int i=nIndex; i>=1; --i)
	{
		for(int j=0; j<TS_GROUND_WIDTH; ++j)
		{
			m_arData[i][j] = m_arData[i-1][j];
		}
	}

	for(int k=0; k<TS_GROUND_WIDTH; ++k)
	{
		m_arData[0][k] = CTsField(NULL);
	}
}
VOID CTsGroundDataMgr::ProduceGarbage(int nLine, CTsField* pData)
{
	int i(0);
	for(; i<TS_GROUND_HEIGHT - nLine; ++i)
	{
		for(int j=0; j<TS_GROUND_WIDTH; ++j)
		{
			m_arData[i][j] = m_arData[i + nLine][j];
		}
	}

	int nPos(0);
	for(; i<TS_GROUND_HEIGHT; ++i)
	{
		for(int j = 0; j<TS_GROUND_WIDTH; ++j)
		{
			m_arData[i][j] = pData[nPos++];
		}
	}

}
INT CTsGroundDataMgr::GetEmptyLineCount()
{
	INT nEmptyLine(0);
	for(int i=0; i<TS_GROUND_HEIGHT; ++i)
	{
		BOOL bEmpty(TRUE);
		for(int j=0; j<TS_GROUND_WIDTH; ++j)
		{
			CTsField& item = m_arData[i][j];
			if(!item.IsEmpty())
			{
				bEmpty = FALSE;
				break;
			}
		}

		if(bEmpty)
		{
			++nEmptyLine;
		}
		else
		{
			break;
		}
	}

	return nEmptyLine;
}
INT CTsGroundDataMgr::GetCurrentHeight()
{
	return TS_GROUND_HEIGHT - GetEmptyLineCount();
}
