#include "stdafx.h"
#include "Button.h"

CButtonHandle::CButtonHandle(HWND hWndButton)
:m_hWndButton(hWndButton)
{
	
}

VOID CButtonHandle::SetCheck(BOOL bCheck)
{
	if(m_hWndButton != NULL)
	{
		SendMessage(m_hWndButton, BM_SETCHECK, (bCheck? BST_CHECKED : BST_UNCHECKED), 0);
	}
}

BOOL CButtonHandle::GetCheck() const
{
	if(m_hWndButton != NULL)
	{
		return SendMessage(m_hWndButton, BM_GETCHECK, 0, 0) == BST_CHECKED;
	}

	return FALSE;
}