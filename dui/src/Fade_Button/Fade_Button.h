#pragma  once 

#include "DUIControlBase.h"
#include "DUIButton.h"

DUI_USING_NAMESPACE



class CFadeButton: public CDUIAnimationControl
{
public:
	enum
	{
		FADE_IN_ID = 8,
		FADE_OUT_ID = 10,

		FADE_ELLAPSE = 20,
		FADE_FRAME_COUNT = 50
	};

	typedef CDUIAnimationControl theBase;
	CFadeButton()
	{
		m_nOldImage = BTN_INDEX_NORMAL;
	}

	virtual ~CFadeButton()
	{

	}


	virtual CRefPtr<IDUIControl> Clone()
	{
		return new CFadeButton(*this);
	}

	virtual BOOL Activate()
	{
		if(!theBase::Activate()) return FALSE;

		SendNotify(WM_NOTIFY_BUTTON_CLICKED);

		return TRUE;
	}

	virtual VOID SetAttribute(const CDUIString& strName, const CDUIString& strValue)
	{
		BOOL bHandled(FALSE);
		if(strName.compare(_T("back_image")) == 0)
		{
			IDUIApp* pApp = DUIGetAppInstance();
			if(pApp != NULL)
			{
				IDUISkinMgr* pSkinMgr = pApp->GetSkinMgr();
				if(pSkinMgr != NULL)
				{
					m_pImageBK= pSkinMgr->GetImage(strValue, FALSE);
				}
			}
			
			bHandled = TRUE;
		}

		if(!bHandled)
		{
			theBase::SetAttribute(strName, strValue);
		}
	}

	virtual LRESULT ProcessEvent(const DUIEvent& info, BOOL& bHandled)
	{
		switch(info.dwEventType)
		{
		case DUI_EVENT_LBUTTONDOWN:
		case DUI_EVENT_LBUTTONDBCLK:
			if(IsEnabled())
				ModifyStatus(0, CONTROL_STATUS_PRESSED);
			break;

		case DUI_EVENT_LBUTTONUP:
			if(IsEnabled())
			{
				ModifyStatus(CONTROL_STATUS_PRESSED, 0);
				if(PtInRect(&GetControlRect(), info.ptMouse))
				{
					Activate();
				}
			}
			break;

		case DUI_EVENT_MOUSEENTER:
			if(IsEnabled())
				ModifyStatus(0, CONTROL_STATUS_HOVER);
			break;

		case DUI_EVENT_MOUSELEAVE:
			if(IsEnabled())
				ModifyStatus(CONTROL_STATUS_HOVER, 0);
			break;

		case DUI_EVENT_KEYDOWN:
			if(IsEnabled())
			{
				if(info.wParam == VK_SPACE)
				{
					ModifyStatus(0, CONTROL_STATUS_PRESSED);
				}
			}
			break;

		case DUI_EVENT_KEYUP:
			if(IsEnabled())
			{
				if(info.wParam == VK_SPACE)
				{
					ModifyStatus(CONTROL_STATUS_PRESSED, 0);
					Activate();
				}			
			}
			break;

		default:
			break;

		}

		return theBase::ProcessEvent(info, bHandled);
	}

protected:
	BTN_IMAGE_STATUS GetImageStatusIndex()
	{
		DWORD dwStatus = GetStatus();

		BOOL bEnable = !(dwStatus & CONTROL_STATUS_DISABLE);
		BOOL bFocus = (dwStatus & CONTROL_STATUS_FOCUS);
		BOOL bPressed = (dwStatus & CONTROL_STATUS_PRESSED);
		BOOL bHover = (dwStatus & CONTROL_STATUS_HOVER);
		BOOL bChecked = (dwStatus & CONTROL_STATUS_CHECKED);

		BTN_IMAGE_STATUS eRet = BTN_INDEX_NORMAL;
		if(!bEnable)
		{
			eRet = BTN_INDEX_DISABLE;
		}
		else if(bPressed && bHover)
		{
			eRet = BTN_INDEX_PRESS;
		}
		else if(bHover)
		{
			eRet = BTN_INDEX_HOVER;
		}
		else if(bPressed)
		{
			eRet = BTN_INDEX_HOVER;
		}
		else if(bChecked)
		{
			eRet = BTN_INDEX_CHECK;
		}
		else if(bFocus)
		{
			eRet = BTN_INDEX_FOCUS;
		}

		return eRet;
	}


	virtual VOID PaintBkgnd(HDC dc)
	{
		RECT rtButton = m_rtControl;
		BTN_IMAGE_STATUS nImageIndex = GetImageStatusIndex();
		CRefPtr<CImageList> pImageBK = m_pImageBK;
		if(pImageBK.IsNull() || pImageBK->IsNull()) return;

		INT nIndex(nImageIndex);
		if(nImageIndex >= pImageBK->GetItemCount())
		{
			DUI_ASSERT(FALSE);
			nIndex = 0;
		}
		
		if((nIndex == BTN_INDEX_HOVER 
			|| nIndex == BTN_INDEX_NORMAL)
			&& 
			(m_nOldImage == BTN_INDEX_NORMAL 
			|| m_nOldImage == BTN_INDEX_HOVER))
		{
			if(m_nOldImage == BTN_INDEX_NORMAL 
				&& nIndex == BTN_INDEX_HOVER
				&& !IsAnimationRunning(FADE_IN_ID)) //mouser hover
			{
				m_bFadeAlpha = 0;

				StopAnimation(FADE_OUT_ID);
				StartAnimation(FADE_ELLAPSE, FADE_FRAME_COUNT, FADE_IN_ID);
				
				m_nOldImage = BTN_INDEX_HOVER;

				pImageBK->SetAlpha(255);
				pImageBK->MiddleStretch(dc, rtButton, BTN_INDEX_NORMAL);
			}
			else if(m_nOldImage == BTN_INDEX_HOVER
				&& nIndex == BTN_INDEX_NORMAL
				&& !IsAnimationRunning(FADE_OUT_ID)) //mouser leave
			{
				m_bFadeAlpha = 0;

				StopAnimation(FADE_IN_ID);
				StartAnimation(FADE_ELLAPSE, FADE_FRAME_COUNT, FADE_OUT_ID);

				m_nOldImage = BTN_INDEX_NORMAL;

				pImageBK->SetAlpha(255);
				pImageBK->MiddleStretch(dc, rtButton, BTN_INDEX_HOVER);
			}
			else if(IsAnimationRunning(FADE_IN_ID)
				|| IsAnimationRunning(FADE_OUT_ID)) //paint fade in/out
			{
				INT nOldIndex = BTN_INDEX_NORMAL;
				INT nNewIndex = BTN_INDEX_HOVER;
				if(IsAnimationRunning(FADE_OUT_ID))
				{
					nOldIndex = BTN_INDEX_HOVER;
					nNewIndex = BTN_INDEX_NORMAL;
				}

				//draw old status image
				pImageBK->SetAlpha(255 - m_bFadeAlpha);
				pImageBK->MiddleStretch(dc, rtButton, nOldIndex);
			
				//draw new status
				pImageBK->SetAlpha(m_bFadeAlpha);
				pImageBK->MiddleStretch(dc, rtButton, nNewIndex);
			}
 			else //fade end
 			{
				pImageBK->SetAlpha(255);
 				pImageBK->MiddleStretch(dc, rtButton, nIndex);
 				m_nOldImage = nIndex;
 			}
		}
		else
		{
			if(IsAnimationRunning(FADE_IN_ID)) StopAnimation(FADE_IN_ID);
			if(IsAnimationRunning(FADE_OUT_ID)) StopAnimation(FADE_OUT_ID);

			pImageBK->SetAlpha(255);
			pImageBK->MiddleStretch(dc, rtButton, nIndex);

			m_nOldImage = nIndex;
		}
	}
	
	
	virtual VOID OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID)
	{
		m_bFadeAlpha = (nCurFrame / (double)nTotalFrame) * 255;
		if(m_bFadeAlpha == 0) m_bFadeAlpha = 10;
		UpdateLayout(FALSE);
	}

	virtual VOID OnAnimationStart(INT nAnimationID, BOOL bFirstLoop)
	{
	
	}

	virtual VOID OnAnimationStop(INT nAnimationID)
	{
	}

protected:
	CRefPtr<CImageList> m_pImageBK;
	INT m_nOldImage;
	BYTE m_bFadeAlpha;
};