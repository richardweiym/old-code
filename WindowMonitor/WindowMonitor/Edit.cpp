#include "stdafx.h"
#include "Edit.h"
#include <assert.h>

CEditHandle::CEditHandle(HWND hWndEdit)
{
	m_hWnd = hWndEdit;
}

void CEditHandle::Clear()
{
	assert(IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_CLEAR, 0, 0L);
}

void CEditHandle::SetSel(int nStartChar, int nEndChar, BOOL bNoScroll /*= FALSE*/)
{
	assert(IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
	if(!bNoScroll)
		::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0L);
}

void CEditHandle::SetSelAll(BOOL bNoScroll /*= FALSE*/)
{
	assert(IsWindow(m_hWnd));
	SetSel(0, -1, bNoScroll);
}

void CEditHandle::SetSelNone(BOOL bNoScroll /*= FALSE*/)
{
	assert(IsWindow(m_hWnd));
	SetSel(-1, 0, bNoScroll);
}

void CEditHandle::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo /*= FALSE*/)
{
	assert(IsWindow(m_hWnd));
	::SendMessage(m_hWnd, EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText);
}

void CEditHandle::InsertText(int nInsertAfterChar, LPCTSTR lpstrText, BOOL bNoScroll /*= FALSE*/, BOOL bCanUndo /*= FALSE*/)
{
	assert(IsWindow(m_hWnd));
	SetSel(nInsertAfterChar, nInsertAfterChar, bNoScroll);
	ReplaceSel(lpstrText, bCanUndo);
}

void CEditHandle::AppendText(LPCTSTR lpstrText, BOOL bNoScroll /*= FALSE*/, BOOL bCanUndo /*= FALSE*/)
{
	assert(IsWindow(m_hWnd));
	InsertText(GetWindowTextLength(m_hWnd), lpstrText, bNoScroll, bCanUndo);
}

VOID CEditHandle::SetLimitTextLength(DWORD dwLength)
{
	::SendMessage(m_hWnd, EM_SETLIMITTEXT, dwLength, 0);
}

void CEditHandle::SetWindowText(LPCTSTR lpszText)
{
	if(lpszText == NULL) lpszText = _T("");
	SendMessage(m_hWnd, WM_SETTEXT, 0, (LPARAM)lpszText);
}

bool CEditHandle::GetWindowText(LPTSTR lpszBuffer, INT nBufferLen)
{
	if(GetWindowTextLength(m_hWnd) > 0)
	{
		::GetWindowText(m_hWnd, lpszBuffer, nBufferLen);
	}
	else
	{
		lpszBuffer[0] = 0;
	}

	return TRUE;
}