#ifndef __BUTTON_H__
#define __BUTTON_H__

class CButtonHandle
{
public:
	CButtonHandle(HWND hWndButton);

	VOID SetCheck(BOOL bCheck);
	BOOL GetCheck() const;

private:
	HWND m_hWndButton;
};


#endif //__BUTTON_H__