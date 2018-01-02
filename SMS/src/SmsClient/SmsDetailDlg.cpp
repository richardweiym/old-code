#include "stdafx.h"
#include "SmsDetail.h"
#include <atlctrls.h>

CSmsDetailDlg::CSmsDetailDlg()
{
	m_hContentBrush = NULL;
}
CSmsDetailDlg::~CSmsDetailDlg()
{
	if(m_hContentBrush != NULL)
	{
		DeleteObject(m_hContentBrush);
		m_hContentBrush = NULL;
	}
}

LRESULT CSmsDetailDlg::OnInitDialog(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CEdit ctrlContent = GetDlgItem(IDC_CONTENT);
	ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CSmsDetailDlg::OnSize(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	INT nX = GET_X_LPARAM(lParam);
	INT nY = GET_Y_LPARAM(lParam);
	
	CEdit ctrlContent = GetDlgItem(IDC_CONTENT);
	ctrlContent.MoveWindow(0, 30, nX, nY - 30);
	
	Invalidate();

	return 0;
}
LRESULT CSmsDetailDlg::OnCtlColorEdit(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(GetDlgItem(IDC_CONTENT) == (HWND)lParam)
	{
		if(m_hContentBrush == NULL)
		{
			m_hContentBrush = CreateSolidBrush(RGB(247, 243, 189));
		}
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (LPARAM)m_hContentBrush;
	}
	
	bHandled = FALSE;
	
	return 0;
}

VOID CSmsDetailDlg::SetValues(CString& strContactCaption, CString& strContactName, CString& strTimeCaption, CString& strTimeValue, CString& strContent)
{
	HWND hContactCaption = GetDlgItem(IDC_CONTACT_CAPTION);
	HWND hContactName = GetDlgItem(IDC_CONTACT_NAME);
	HWND hTimeCaption = GetDlgItem(IDC_TIME_CAPTION);
	HWND hTimeValue = GetDlgItem(IDC_TIME_VALUE);
	HWND hContent = GetDlgItem(IDC_CONTENT);

	::SetWindowText(hContactCaption, strContactCaption);
	::SetWindowText(hContactName, strContactName);
	::SetWindowText(hTimeCaption, strTimeCaption);
	::SetWindowText(hTimeValue, strTimeValue);
	::SetWindowText(hContent, strContent);
}