#include "stdafx.h"
#include "Combobox.h"

CComboboxHandle::CComboboxHandle(HWND hWnd)
:m_hWnd(hWnd)
{

}

CComboboxHandle::~CComboboxHandle()
{

}

INT CComboboxHandle::GetCount() const
{
	return (INT)::SendMessage(m_hWnd, CB_GETCOUNT, 0, 0L);
}

INT CComboboxHandle::GetCurSel() const
{
	return (INT)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0L);
}

INT CComboboxHandle::SetCurSel(INT nIndex)
{
	return (INT)::SendMessage(m_hWnd, CB_SETCURSEL, nIndex, 0L);
}

INT CComboboxHandle::GetLBText(INT nIndex, LPTSTR lpszBuffer)
{
	return (INT)::SendMessage(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)lpszBuffer);
}

INT CComboboxHandle::AddString(LPCTSTR lpszText)
{
	return (INT)::SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)lpszText);
}

INT CComboboxHandle::InsertString(INT nIndedx, LPCTSTR lpszText)
{
	return (INT)::SendMessage(m_hWnd, CB_INSERTSTRING, nIndedx, (LPARAM)lpszText);
}

INT CComboboxHandle::SetItemData(INT nIndex, DWORD dwData)
{
	return (INT)::SendMessage(m_hWnd, CB_SETITEMDATA, nIndex, (DWORD)dwData);
}

DWORD CComboboxHandle::GetItemData(INT nIndex) const
{
	return (DWORD)::SendMessage(m_hWnd, CB_GETITEMDATA, nIndex, 0L);
}