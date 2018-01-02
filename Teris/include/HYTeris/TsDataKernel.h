#pragma once 

#include "TsStdAfx.h"
#include "TsUtilBase.h"

const INT TS_ELEMENT_COUNT = 7;
const INT  TS_ELEMENT_WIDHT = 4;

enum TsSwitchType
{
	TS_SWITCH_1 = 0,
	TS_SWITCH_2 = 1,
	TS_SWITCH_3 = 2,
	TS_SWITCH_4 = 3,

	TS_SWITCH_COUNT = 4,

	TS_SWITCH_DEFAULT = 1
};

class CTsDataSourceBase
{
public:
	INT GetEleTypeCount();
	INT GetIdByIndex(INT nIndex);
	virtual COLORREF GetFaceColorByIndex(INT nIndex) = 0;
	virtual COLORREF GetLeftTopColorByIndex(INT nIndex) = 0;
	virtual COLORREF GetRightBottomColorByIndex(INT nIndex) = 0;
	BOOL GetDataByIndex(BYTE arBuffer[TS_ELEMENT_WIDHT * TS_ELEMENT_WIDHT * TS_SWITCH_COUNT], INT nBufferLen, INT nIndex);
	
	virtual ~CTsDataSourceBase() { NULL; }
};

class CTsDataColorSource: public CTsDataSourceBase
{
public:
	virtual COLORREF GetFaceColorByIndex(INT nIndex);
	virtual COLORREF GetLeftTopColorByIndex(INT nIndex);
	virtual COLORREF GetRightBottomColorByIndex(INT nIndex);
};

class CTsDataBlackSource: public CTsDataSourceBase
{
public:
	virtual COLORREF GetFaceColorByIndex(INT nIndex);
	virtual COLORREF GetLeftTopColorByIndex(INT nIndex);
	virtual COLORREF GetRightBottomColorByIndex(INT nIndex);
};

struct CTsBinData
{
	BYTE m_arBin[TS_ELEMENT_WIDHT][TS_ELEMENT_WIDHT];
};
class CTsTypeElement
{
public:
	DWORD m_dwID;
	COLORREF m_clrFace;
	COLORREF m_clrLeftTopFrame;
	COLORREF m_clrRightBottomFrame;
	CTsBinData m_arData[TS_SWITCH_COUNT];
};


class CTsDataFactory
{
public:
	CTsDataFactory();
	~CTsDataFactory();
	INT GetElementCount();
	CTsTypeElement* GetElementByID(DWORD dwId);
	CTsTypeElement* GetElementByIndex(INT nIndex);

public:
	BOOL Init(CTsDataSourceBase& ct);
	VOID Release();
	BOOL IsInited() { return m_bInited; }

protected:
	void SetEmpty();
	CTsDataFactory(const CTsDataFactory& ts) ;

private:
	CTsTypeElement m_vecPool[TS_ELEMENT_COUNT];
	BOOL m_bInited;
};

#define TS_MIN_FIELD 15
class CTsField
{
public:
	CTsField(CTsTypeElement* pEle = NULL)
	{
		m_pEle = pEle;
	}
	
	CTsField& operator = (CTsTypeElement* pEle);

	BOOL IsEmpty() { return m_pEle == NULL; }
	VOID SetEmpty() { m_pEle = NULL; }
	COLORREF GetFaceColor();
	COLORREF GetLeftTopFrameColor();
	COLORREF GetRightBottomFrameColor();

	static VOID SetFieldWidth(int nValue);
	static INT GetFieldWidth();
	static INT GetFrameWidth();
	static INT GetGapWidth();
	
private:
	CTsTypeElement* m_pEle;
	static INT s_nFieldWidth;
};


class CTsMovingElement
{
public:
	CTsMovingElement()
	{
		SetEmpty();
	}
	VOID SetEmpty() { m_ptPos.x = m_ptPos.y = 0; m_pCurrent = NULL; m_eType = TS_SWITCH_1; }
	BOOL IsEmpty() { return m_pCurrent == NULL; }

	VOID SetPos(int nX, int nY) { m_ptPos.x = nX; m_ptPos.y = nY; }
	INT GetXPos() { return m_ptPos.x; }
	INT GetYPos() { return m_ptPos.y; }
	POINT GetPos() {  return  m_ptPos; }
	
	VOID SetElement(CTsTypeElement* pEle) { m_pCurrent = pEle; }
	CTsTypeElement* GetElement() { return m_pCurrent; }
	
	TsSwitchType SetCurrentType(TsSwitchType eType);
	TsSwitchType SetToNextType();
	TsSwitchType GetNextType();
	TsSwitchType SetToPrevType();
	TsSwitchType GetPrevType();
	TsSwitchType GetCurrentType() { return m_eType; }

	CRect GetElementRect();
	CRect GetSubFieldRect(int nX, int nY);
	
	CTsBinData* GetCurrentBinData();
private:
	POINT m_ptPos;
	CTsTypeElement* m_pCurrent;
	TsSwitchType m_eType;
};

const int TS_GROUND_WIDTH = 10;
const int TS_GROUND_HEIGHT = 20;
class CTsGroundDataMgr
{
public:
	CTsGroundDataMgr();
	VOID SetEmpty();
	BOOL InitData(CTsField* pData, INT nCount);

	VOID SetField(INT nXIndex, INT nYIndex, CTsField field);
	CTsField* GetField(int nXIndex, int nYIndex);
	CRect GetFieldRect(int nXIndex, int nYIndex);

	INT GetCurrentHeight();
	INT GetEmptyLineCount();
	
	VOID RemoveLine(INT nIndex);
	VOID ProduceGarbage(int nLine, CTsField* pData);
private:
	CTsField m_arData[TS_GROUND_HEIGHT][TS_GROUND_WIDTH];
};
