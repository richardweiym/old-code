#ifndef __EDIT_H__
#define __EDIT_H__


class CEditHandle
{
public:
	CEditHandle(HWND hWndEdit);

	void Clear();

	void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE);
	void SetSelAll(BOOL bNoScroll = FALSE);
	void SetSelNone(BOOL bNoScroll = FALSE);

	void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);

	void InsertText(int nInsertAfterChar, LPCTSTR lpstrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE);

	void AppendText(LPCTSTR lpstrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE);

	void SetLimitTextLength(DWORD dwLength);

	void SetWindowText(LPCTSTR lpszText);
	bool GetWindowText(LPTSTR lpszBuffer, INT nBufferLen);

private:
	HWND m_hWnd;
};


#endif //__EDIT_H__
