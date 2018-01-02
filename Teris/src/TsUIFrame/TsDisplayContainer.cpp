#include "TsDisplayContainer.h"
#include "TsDataKernel.h"
#include "TsDrawBase.h"

#define TS_GROUND_FIELD_BK_COLOR RGB(0, 0, 0)
#define GRAY_COLOR(x) RGB(GetRValue(x)/2, GetGValue(x)/2, GetBValue(x)/2)


CTsDisplayContainer::CTsDisplayContainer()
{
	m_bPlaying = FALSE;
	m_eDisplayResutl = NONE_RESULT;
	m_bOutOfGround = FALSE;
	m_bDisplayData = TRUE;
	m_bDroppingDown = FALSE;
}
CSize CTsDisplayContainer::GetMinSize()
{
	double dbRadio = TS_GROUND_HEIGHT / TS_GROUND_WIDTH;
	CSize szRet((TS_MIN_FIELD + 1)* TS_GROUND_WIDTH, (TS_MIN_FIELD + 1)* TS_GROUND_WIDTH * dbRadio );
	return szRet;
}
BOOL CTsDisplayContainer::GetCorrectAutoSize(SIZE& szCheck)
{
	double dbRadio = TS_GROUND_HEIGHT / TS_GROUND_WIDTH;

	int nWidth = szCheck.cx;
	if(nWidth * dbRadio > szCheck.cy)
	{
		nWidth = szCheck.cy / dbRadio;
	}
	
	if(nWidth % TS_GROUND_WIDTH != 0)
	{
		nWidth = nWidth / TS_GROUND_WIDTH * TS_GROUND_WIDTH;
	}
	
	BOOL bRet(FALSE);
	if(nWidth > TS_MIN_FIELD * TS_GROUND_WIDTH)
	{
		szCheck.cx = nWidth;
		szCheck.cy = nWidth * dbRadio;
		bRet = TRUE;
	}

	return bRet;
}


LRESULT CTsDisplayContainer::OnSize(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam != SIZE_MINIMIZED)
	{
		CRect rtClient;
		rtClient.SetRectEmpty();
		GetClientRect(&rtClient);
		
		CSize szCheck(rtClient.Width(), rtClient.Height());
		if(GetCorrectAutoSize(szCheck))
		{
			CTsField::SetFieldWidth(szCheck.cx / TS_GROUND_WIDTH);
		}
	}

	return S_OK;
}
LRESULT CTsDisplayContainer::OnTsDisplayControl(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_ASSERTE(wParam != NULL);
	LPTsControlHeader pHeader = (LPTsControlHeader)wParam;
	LRESULT lRet(0);

	switch(pHeader->nControlCode)
	{
	case TS_CONTROL_INIT:
		lRet = OnControlInit(wParam, lParam);
		break;

	case TS_CONTROL_START:
		lRet = OnControlStart(wParam, lParam);
		break;
	
	case TS_CONTROL_END:
		lRet = OnControlEnd(wParam, lParam);
		break;

	case TS_CONTROL_SET_MOVING_ELE:
		lRet = OnControlSetMovingEle(wParam, lParam);
		break;
	
	case TS_CONTROL_MOVE_LEFT:
		lRet = OnControlMoveLeft(wParam, lParam);
		break;
	
	case TS_CONTROL_MOVE_RIGHT:
		lRet = OnControlMoveRight(wParam, lParam);
		break;

	case TS_CONTROL_MOVE_DOWN:
		lRet = OnControlMoveDown(wParam, lParam);
		break;
	
	case TS_CONTROL_SWITCH_LEFT:
		lRet = OnControlSwitchLeft(wParam, lParam);
		break;

	case TS_CONTROL_SWITCH_RIGHT:
		lRet = OnControlSwitchRight(wParam, lParam);
		break;

	case TS_CONTROL_DROP_DOWN:
		lRet = OnControlDropDown(wParam, lParam);
		break;

	case TS_CONTROL_PRODUCE_GARBAGE:
		lRet = OnControlProduceGarbage(wParam, lParam);
		break;
	
	case TS_CONTROL_SET_RESULT:
		lRet = OnControlSetResult(wParam, lParam);
		break;

	case TS_CONTROL_QUERY_STATUS:
		lRet = OnControlQueryStatus(wParam, lParam);
		break;
	case TS_CONTROL_GET_GROUND_DATA:
		lRet = OnControlGetGroundData(wParam, lParam);
		break;
	default:
		_ASSERTE(FALSE);
		break;
	}

	return lRet;
}

LRESULT CTsDisplayContainer::OnControlInit(WPARAM wParam, LPARAM lParam)
{
	_ASSERTE(wParam != NULL);
	LPTsControlHeader pHeader = (LPTsControlHeader)wParam;
	m_dwIDFrom = pHeader->dwIDFrom;

	INT nLen = (INT)(pHeader->dwUserData);
	_ASSERTE(lParam != NULL);

	m_dataMgr.InitData((CTsField*)lParam, nLen);

	_OnControlEvent(wParam, lParam);

	return S_OK;
}

LRESULT CTsDisplayContainer::OnControlStart(WPARAM wParam, LPARAM lParam)
{
	_ASSERTE(!m_bPlaying);
	if(!m_bPlaying)
	{
		m_bPlaying = TRUE;
		m_bOutOfGround = FALSE;
		m_eDisplayResutl = NONE_RESULT;
		
		if(m_bDisplayData)
		{
			InvalidateRect(NULL, TRUE);
		}


		LPTsControlHeader pHeader = (LPTsControlHeader)wParam;
		_ASSERTE(pHeader != NULL);
		m_dwIDFrom = pHeader->dwIDFrom;
		
		_OnControlEvent(wParam, lParam);

		_RequestNewElement(TRUE);
	}

	return S_OK;
}

LRESULT CTsDisplayContainer::OnControlEnd(WPARAM wParam, LPARAM lParam)
{
	if(m_bPlaying)
	{
		m_bPlaying = FALSE;

		if(m_bDisplayData)
		{
			InvalidateRect(NULL, TRUE);
			UpdateWindow();
		}

		_OnControlEvent(wParam, lParam);
	
	}

	return S_OK;
}
LRESULT CTsDisplayContainer::OnControlSetMovingEle(WPARAM wParam, LPARAM lParam)
{
	_ASSERTE(wParam != NULL);
	_ASSERTE(lParam != NULL);

	CTsTypeElement* pEle = (CTsTypeElement*)lParam;
	m_movingEle.SetElement(pEle);
	m_movingEle.SetCurrentType(TS_SWITCH_DEFAULT);


	int nPosX = (TS_GROUND_WIDTH - TS_ELEMENT_WIDHT) / 2 - 1;
	INT nPosY(0);
//	_ASSERTE(m_dataMgr.GetEmptyLineCount() >= 0);
	INT nEmptyLine = m_dataMgr.GetEmptyLineCount();
	if( nEmptyLine >= TS_ELEMENT_WIDHT + 1)
	{
		nPosY = 0;
	}
	else
	{
		nPosY = nEmptyLine - TS_ELEMENT_WIDHT-1;
	}
	m_movingEle.SetPos(nPosX, nPosY);

	if(m_bDisplayData)
	{
		CRect rtDraw = m_movingEle.GetElementRect();
		InvalidateRect(&rtDraw, TRUE);
		UpdateWindow();
	}
	
	_OnControlEvent(wParam, lParam);

	return S_OK;
}
LRESULT CTsDisplayContainer::OnControlMoveLeft(WPARAM wParam, LPARAM lParam)
{
	if(m_movingEle.IsEmpty()) return FALSE;

	POINT ptPos = m_movingEle.GetPos();
	--ptPos.x;
	if(_IsValidMovingElement(ptPos, m_movingEle.GetCurrentType()))
	{
		CRect rtOld = m_movingEle.GetElementRect();
		m_movingEle.SetPos(ptPos.x, ptPos.y);
		CRect rtNew = m_movingEle.GetElementRect();
		
		if(m_bDisplayData)
		{
			CRect rtDraw(0, 0, 0, 0);
			rtDraw.UnionRect(&rtOld, &rtNew);
			InvalidateRect(&rtDraw, TRUE);
			UpdateWindow();
		}

		_OnControlEvent(wParam, lParam);

		return TRUE;
	}

	return FALSE;
}
LRESULT CTsDisplayContainer::OnControlMoveRight(WPARAM wParam, LPARAM lParam)
{
	if(m_movingEle.IsEmpty()) return FALSE;

	POINT ptPos = m_movingEle.GetPos();
	++ptPos.x;
	if(_IsValidMovingElement(ptPos, m_movingEle.GetCurrentType()))
	{
		CRect rtOld = m_movingEle.GetElementRect();
		m_movingEle.SetPos(ptPos.x, ptPos.y);
		CRect rtNew = m_movingEle.GetElementRect();

		if(m_bDisplayData)
		{
			CRect rtDraw(0, 0, 0, 0);
			rtDraw.UnionRect(&rtOld, &rtNew);
			InvalidateRect(&rtDraw, TRUE);
			UpdateWindow();
		}

		_OnControlEvent(wParam, lParam);

		return TRUE;
	}	

	return FALSE;
}
LRESULT CTsDisplayContainer::OnControlMoveDown(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet(FALSE);
	if(m_movingEle.IsEmpty()) return bRet;

	POINT ptPos = m_movingEle.GetPos();
	++ptPos.y;
	if(_IsValidMovingElement(ptPos, m_movingEle.GetCurrentType()))
	{
		CRect rtOld = m_movingEle.GetElementRect();
		m_movingEle.SetPos(ptPos.x, ptPos.y);
		CRect rtNew = m_movingEle.GetElementRect();

		if(m_bDisplayData)
		{
			CRect rtDraw(0, 0, 0, 0);
			rtDraw.UnionRect(&rtOld, &rtNew);
			InvalidateRect(&rtDraw, TRUE);
			UpdateWindow();
		}

		bRet = TRUE;

		if(!m_bDroppingDown)
		{
			_OnControlEvent(wParam, lParam);
		}
	}
	else
	{
		_OnElementHitGround();

		_AddMovingElement2Ground();	

		INT nRemove = _RemoveFullLineData();
		if(nRemove > 0)
		{
			if(m_bDisplayData)
			{
				InvalidateRect(NULL, TRUE);
				UpdateWindow();	
			}

			_OnRemovedLine(nRemove);
		}

		if(_IsGameDead())
		{
			_SetDeadStatus();
		}
		else
		{
			_RequestNewElement(FALSE);
		}
	}

	return bRet;
}

LRESULT CTsDisplayContainer::OnControlSwitchLeft(WPARAM wParam, LPARAM lParam)
{
	if(m_movingEle.IsEmpty()) return FALSE;
	
	if(_IsValidMovingElement(m_movingEle.GetPos(), m_movingEle.GetPrevType()))
	{
		m_movingEle.SetToPrevType();
		
		if(m_bDisplayData)
		{
			CRect rtDraw = m_movingEle.GetElementRect();
			InvalidateRect(&rtDraw, TRUE);
			UpdateWindow();	
		}

		_OnControlEvent(wParam, lParam);

		return TRUE;
	}

	return FALSE;
}
LRESULT CTsDisplayContainer::OnControlSwitchRight(WPARAM wParam, LPARAM lParam)
{
	if(m_movingEle.IsEmpty()) return FALSE;
	
	if(_IsValidMovingElement(m_movingEle.GetPos(), m_movingEle.GetNextType()))
	{
		m_movingEle.SetToNextType();
	
		if(m_bDisplayData)
		{
			CRect rtDraw = m_movingEle.GetElementRect();
			InvalidateRect(&rtDraw, TRUE);
			UpdateWindow();
		}

		_OnControlEvent(wParam, lParam);
		return TRUE;
	}	

	return FALSE;
}

LRESULT CTsDisplayContainer::OnControlDropDown(WPARAM wParam, LPARAM lParam)
{
	if(m_movingEle.IsEmpty()) return S_FALSE;
	
	BOOL bRet(FALSE);

	TsControlHeader th = {0};
	th.nControlCode = TS_CONTROL_MOVE_DOWN;
	th.dwUserData = 0;

	th.dwIDFrom = m_dwIDFrom;
	
	m_bDroppingDown = TRUE;
	do 
	{
		bRet = SendMessage(WM_TS_DISPLAY_CONTROL, (WPARAM)&th, 0);
	} 
	while(bRet);
	m_bDroppingDown = FALSE;


	_OnControlEvent(wParam, lParam);

	return S_OK;
}
LRESULT CTsDisplayContainer::OnControlSetResult(WPARAM wParam, LPARAM lParam)
{
	m_eDisplayResutl = (TS_DISPLAY_RESULT)lParam;

	if(m_bDisplayData)
	{
		InvalidateRect(NULL, TRUE);
		UpdateWindow();
	}

	_OnControlEvent(wParam, lParam);

	return S_OK;
}
LRESULT CTsDisplayContainer::OnControlQueryStatus(WPARAM wParam, LPARAM lParam)
{
	return m_bPlaying;
}
LRESULT CTsDisplayContainer::OnControlGetGroundData(WPARAM wParam, LPARAM lParam)
{
	_ASSERTE(wParam != NULL);
	LPTsControlHeader pHeader = (LPTsControlHeader)wParam;
	m_dwIDFrom = pHeader->dwIDFrom;

	INT nLen = (INT)(pHeader->dwUserData);
	_ASSERTE(nLen >= TS_GROUND_WIDTH * TS_GROUND_HEIGHT);

	_ASSERTE(lParam != NULL);
	CTsField* pDataBuffer = (CTsField*)lParam;
	
	INT nIndex(0);
	for(int i=0; i<TS_GROUND_HEIGHT; ++i)
	{
		for(int k=0; k<TS_GROUND_WIDTH; ++k)
		{
			pDataBuffer[nIndex++] = *(m_dataMgr.GetField(k, i));
		}
	}
	
	return S_OK;
}
void CTsDisplayContainer::ClearDisplay()
{
	CTsField arData[TS_GROUND_HEIGHT * TS_GROUND_WIDTH];
	m_dataMgr.InitData(arData, TS_GROUND_HEIGHT * TS_GROUND_WIDTH);
	
	m_eDisplayResutl = NONE_RESULT;

	m_movingEle.SetEmpty();
	
	if(m_bDisplayData)
	{
		InvalidateRect(NULL, TRUE);
	}

}
LRESULT CTsDisplayContainer::OnControlProduceGarbage(WPARAM wParam, LPARAM lParam)
{
	_ASSERTE(wParam != NULL);
	LPTsControlHeader pHeader = (LPTsControlHeader)wParam;

	INT nLen = (INT)(pHeader->dwUserData);
	_ASSERTE(lParam != NULL);

	CTsField* pArrayField = (CTsField*)lParam;
	int nLine = nLen / TS_GROUND_WIDTH;
	_ASSERTE(nLen % TS_GROUND_WIDTH == 0);

	INT nEmptyLine = m_dataMgr.GetEmptyLineCount();
	if(nEmptyLine < nLine)
	{
		m_bOutOfGround = TRUE;
	}

	m_dataMgr.ProduceGarbage(nLine, pArrayField);

	_OnControlEvent(wParam, lParam);

	if(!m_movingEle.IsEmpty())
	{
		INT nPosY = m_movingEle.GetYPos();
		nPosY -= nLine;
		if(nPosY < -4)
		{
			nPosY = -4;
		}
		m_movingEle.SetPos(m_movingEle.GetXPos(), nPosY);
	}
	

	if(_IsGameDead())
	{
		_SetDeadStatus();
	}
	else
	{
		if(m_bDisplayData)
		{
			InvalidateRect(NULL, TRUE);
			UpdateWindow();
		}
	}

	return S_OK;
}

LRESULT CTsDisplayContainer::OnPaint(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	CPaintDC dcPaint(m_hWnd);
	if(m_bDisplayData)
	{
		CHYMemDC  memDc(dcPaint);
		CDCHandle dc(memDc.m_hDC);

		_DrawGrounField(dc);
		_DrawMovingElement(dc);
		_DrawDisplayResutl(dc);
	}
	else
	{
		CRect rtClient;
		GetClientRect(&rtClient);
		dcPaint.FillSolidRect(&rtClient, RGB(0, 0, 0));
	}
	return S_OK;
}
//lParam = 0 setup, lParam != 0 cancel setup
LRESULT CTsDisplayContainer::OnTsSetDisplaySink(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_ASSERTE(wParam != NULL);
	
	if(lParam != 0)
	{
		m_pControlSink = (CMessageMap*)wParam;
	}
	else
	{
		_ASSERTE(wParam == (WPARAM)m_pControlSink);
		m_pControlSink = NULL;
	}

	return S_OK;
}
LRESULT CTsDisplayContainer::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return TRUE;
}

VOID CTsDisplayContainer::_DrawBkgnd(CDCHandle dc)
{
	CRect rtBox;
	dc.GetClipBox(&rtBox);
	dc.FillSolidRect(&rtBox, TS_GROUND_FIELD_BK_COLOR);
}

VOID CTsDisplayContainer::_DrawGrounField(CDCHandle dc)
{
	CRect rtRet;
	CRect rtBox;
	dc.GetClipBox(&rtBox);
	
	for(int i=0; i<TS_GROUND_HEIGHT; ++i)
	{
		for(int j=0; j<TS_GROUND_WIDTH; ++j)
		{
			CRect rtField = m_dataMgr.GetFieldRect(j, i);
			if(IntersectRect(&rtRet, &rtBox, &rtField))
			{
				CTsField* pField = m_dataMgr.GetField(j, i);
				_ASSERTE(pField != NULL);
				
				_DrawFieldElement(dc, rtField.left, rtField.top, pField, !m_bPlaying);
			}			
		}
	}
}
VOID CTsDisplayContainer::_DrawFieldElement(CDCHandle dc, INT nX, INT nY, CTsField* pField, BOOL bDrawGray)
{
	_ASSERTE(pField != NULL);
	
	int nFieldWidth = CTsField::GetFieldWidth();
	int nFrameWidth = CTsField::GetFrameWidth();
	INT nGap = CTsField::GetGapWidth();
	
	CRect rtField(nX, nY, nX + nFieldWidth, nY + nFieldWidth);
	dc.FillSolidRect(&rtField, TS_GROUND_FIELD_BK_COLOR);
	
	if(!pField->IsEmpty())
	{
		COLORREF clrFace = pField->GetFaceColor();
		COLORREF clrLeftTop = pField->GetLeftTopFrameColor();
		COLORREF clrRightBottom = pField->GetRightBottomFrameColor();

		if(bDrawGray)
		{
			clrFace = GRAY_COLOR(clrFace);
			clrLeftTop = GRAY_COLOR(clrLeftTop);
			clrRightBottom = GRAY_COLOR(clrRightBottom);
		}

		rtField.InflateRect(-nGap, -nGap, -nGap, -nGap);
		dc.FillSolidRect(rtField, clrFace);
		DrawBorder(dc, rtField, clrLeftTop, clrRightBottom, nFrameWidth);
	}
}
BOOL CTsDisplayContainer::_IsValidDrawRect(CRect& rtTest)
{
	BOOL bRet(FALSE);

	CRect rtClient;
	GetClientRect(&rtClient);
	
	if(rtTest.left >= 0 && rtTest.top >= 0
		&&rtTest.left < rtClient.right 
		&& rtTest.top < rtClient.bottom)
	{
		bRet = TRUE;
	}
	
	return bRet;
}
BOOL CTsDisplayContainer::_IsGameDead()
{
	return (m_dataMgr.GetEmptyLineCount() <= 0) && m_bOutOfGround;
}
INT CTsDisplayContainer::_RemoveFullLineData()
{
	INT nRet(0);

	for(int i=TS_GROUND_HEIGHT - 1; i>=0;)
	{
		BOOL bEmpty(TRUE);
		BOOL bFull(TRUE);

		for(int j=0; j<TS_GROUND_WIDTH; ++j)
		{
			CTsField* pItem = m_dataMgr.GetField(j, i);
			_ASSERTE(pItem != NULL);

			if(pItem->IsEmpty())
			{
				bFull = FALSE;
			}
			else
			{
				bEmpty = FALSE;
			}
		}

		if(bFull)
		{
			m_dataMgr.RemoveLine(i);
			++nRet;
		}
		else
		{
			--i;
		}

		if(bEmpty)
		{
			break;
		}
	}
	
	return nRet;
}
VOID CTsDisplayContainer::_AddMovingElement2Ground()
{
	CTsBinData* pData = NULL;
	CTsTypeElement* pEle = NULL;
	if(!m_movingEle.IsEmpty())
	{
		pEle  = m_movingEle.GetElement();
		_ASSERTE(pEle != NULL);

		pData = &(pEle->m_arData)[m_movingEle.GetCurrentType()];
	}

	_ASSERTE(pData != NULL);
	if(pData == NULL) return;

	POINT ptPos = m_movingEle.GetPos();
	for(int i=0; i<TS_ELEMENT_WIDHT; ++i)
	{
		for(int j=0; j<TS_ELEMENT_WIDHT; ++j)
		{
			int nX = ptPos.x + j;
			int nY = ptPos.y + i;

			BYTE btVal = pData->m_arBin[i][j];
			
#ifdef _DEBUG
			if(btVal != 0)
			{
				_ASSERTE(nX >=0 && nX < TS_GROUND_WIDTH && nY < TS_GROUND_HEIGHT);
			}
#endif
			if(btVal != 0)
			{
				if(nX >= 0 && nX < TS_GROUND_WIDTH && nY >=0 && nY < TS_GROUND_HEIGHT)
				{
					m_dataMgr.SetField(nX, nY, CTsField(pEle));
				}
				else if(nY < 0)
				{
					m_bOutOfGround = TRUE;
				}
			}
		}
	}
	
	m_movingEle.SetEmpty();
}

BOOL CTsDisplayContainer::_IsValidMovingElement(POINT& ptPos, TsSwitchType eType)
{
	BOOL bRet(FALSE);
	CTsBinData* pData = NULL;
	if(!m_movingEle.IsEmpty())
	{
		CTsTypeElement* pEle  = m_movingEle.GetElement();
		_ASSERTE(pEle != NULL);

		pData = &(pEle->m_arData)[eType];
	}

	_ASSERTE(pData != NULL);
	if(pData == NULL) return bRet;

	bRet = TRUE;
	for(int i=0; i<TS_ELEMENT_WIDHT; ++i)
	{
		for(int j=0; j<TS_ELEMENT_WIDHT; ++j)
		{
			int nX = ptPos.x + j;
			int nY = ptPos.y + i;

			BYTE btVal = pData->m_arBin[i][j];
			if(btVal != 0)
			{
				if(nX < 0 || nX >= TS_GROUND_WIDTH || nY >= TS_GROUND_HEIGHT) //check if move out of the ground
				{
					bRet = FALSE;
					break;
				}
				
				if(nX >= 0 && nX < TS_GROUND_WIDTH && nY >= 0 && nY < TS_GROUND_HEIGHT) //check if covered with old ground data
				{
					CTsField* pField = m_dataMgr.GetField(nX, nY);	
					if(!pField->IsEmpty())
					{
						bRet = FALSE;
						break;
					}
				}
			}	
		}

		if(!bRet)
		{
			break;
		}
	}
	
	
	return bRet;

}
VOID CTsDisplayContainer::_DrawDisplayResutl(CDCHandle dc)
{
	if(m_eDisplayResutl == NONE_RESULT)
	{
		return ;
	}


	CRect rtClient;
	GetClientRect(&rtClient);
	TCHAR szText[32] = {0};
	if(m_eDisplayResutl == WIN_RESULT)
	{
		lstrcpyn(szText, _T("Win"), sizeof(szText) / sizeof(TCHAR) - 1);
	}
	else if(m_eDisplayResutl = LOST_RESULT)
	{
		lstrcpyn(szText, _T("Lost"), sizeof(szText) / sizeof(TCHAR) - 1);
	}
	else
	{
		_ASSERTE(FALSE);
	}

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255, 255, 255));
	dc.DrawText(szText, lstrlen(szText), &rtClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER| DT_END_ELLIPSIS);
}
VOID CTsDisplayContainer::_DrawMovingElement(CDCHandle dc)
{
	CRect rtRet, rtBox, rtMoving;
	dc.GetClipBox(&rtBox);	
	
	CTsBinData* pData = NULL;
	CTsTypeElement* pEle = NULL;
	if(!m_movingEle.IsEmpty())
	{
		rtMoving = m_movingEle.GetElementRect();
		if(IntersectRect(&rtRet, &rtBox, &rtMoving))
		{
			pEle = m_movingEle.GetElement();
			_ASSERTE(pEle != NULL);
			pData = &(pEle->m_arData)[m_movingEle.GetCurrentType()];
		}
	}
	
	if(pData != NULL)
	{
		CTsField tsEmpty(NULL);
		CTsField tsValid(pEle);
		for(int i=0; i<TS_ELEMENT_WIDHT; ++i)
		{
			for(int j=0; j<TS_ELEMENT_WIDHT; ++j)
			{
				CRect rtDraw = m_movingEle.GetSubFieldRect(j, i);
				if(!_IsValidDrawRect(rtDraw)) continue;

				if(pData->m_arBin[i][j] != 0)
				{
					_DrawFieldElement(dc, rtDraw.left, rtDraw.top, &tsValid, !m_bPlaying);
				}
			}
		}
	}

}

void CTsDisplayContainer::_OnControlEvent(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult(0);
	_ASSERTE(m_pControlSink != NULL);
	if(m_pControlSink != NULL)
	{
		m_pControlSink->ProcessWindowMessage(m_hWnd, WM_TS_DISPLAY_CONTROL_EVENT, wParam, lParam, lResult, 0);
	}
}

void CTsDisplayContainer::_SetDeadStatus()
{
	TsControlHeader th = {0};
	th.nControlCode = TS_CONTROL_END;
	th.dwIDFrom = m_dwIDFrom;

	
	SendMessage(WM_TS_DISPLAY_CONTROL, (WPARAM)&th, 1);
}

void CTsDisplayContainer::_RequestNewElement(BOOL bFirst /* = FALSE */)
{
	TsControlHeader th = {0};
	th.nControlCode = TS_CONTROL_REQUEST_ELEMENT;
	th.dwIDFrom = m_dwIDFrom;

	_OnControlEvent(WPARAM(&th), (LPARAM)bFirst);
}

void CTsDisplayContainer::_OnRemovedLine(INT nCount)
{
	TsControlHeader th = {0};
	th.nControlCode = TS_CONTROL_REMOVED_LINE;
	th.dwIDFrom = m_dwIDFrom;
	th.dwUserData = nCount;

	_OnControlEvent(WPARAM(&th), 0);
}

void CTsDisplayContainer::_OnElementHitGround()
{
	TsControlHeader th = {0};
	th.nControlCode = TS_CONTROL_ELE_GROUND_HIT;
	th.dwIDFrom = m_dwIDFrom;

	_OnControlEvent(WPARAM(&th), 0);
}