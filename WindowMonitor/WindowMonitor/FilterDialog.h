#ifndef __FILTER_DIALOG_H__
#define __FILTER_DIALOG_H__

#include "resource.h"

class CFilterDialog
{
public:
	enum { IDD = IDD_FILTER };

	CFilterDialog();
	~CFilterDialog();

	BOOL ShowDialog(HWND hWndParent);
	VOID DestroyDialog();

	BOOL IsDialogMessage(LPMSG pMsg);

	BOOL IsWindow() const;
	
public:
	VOID LoadInitData();
	VOID SaveData();

	HWND m_hWnd;

private:
	CFilterDialog(const CFilterDialog&);
	CFilterDialog& operator = (const CFilterDialog&);	
};


#endif //__FILTER_DIALOG_H__