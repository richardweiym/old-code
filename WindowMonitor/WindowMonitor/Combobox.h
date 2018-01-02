#ifndef __COMBOBOX_H__
#define __COMBOBOX_H__

class CComboboxHandle
{
public:
	CComboboxHandle(HWND hWnd);
	~CComboboxHandle();

	INT GetCount() const;
	INT GetCurSel() const;
	INT SetCurSel(INT nIndex);
	INT GetLBText(INT nIndex, LPTSTR lpszBuffer);
	
	INT AddString(LPCTSTR lpszText);
	INT InsertString(INT nIndedx, LPCTSTR lpszText);

	INT SetItemData(INT nIndex, DWORD dwData);
	DWORD GetItemData(INT nIndex) const;

private:
	HWND m_hWnd;
};

#endif//__COMBOBOX_H__