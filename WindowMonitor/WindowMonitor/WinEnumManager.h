#ifndef __WIN_ENUM_MANAGER_H__
#define __WIN_ENUM_MANAGER_H__

#include "WinEnum.h"
#include "WinFilter.h"

class CWinEnumManager: public IWinEnumEventSink
{
public:
	CWinEnumManager();
	~CWinEnumManager();

	VOID Refresh();

	//IWinEnumEventSink
	virtual VOID OnEnumStart(); 
	virtual VOID OnEnumEnd();
	virtual VOID OnEnumWindow(HWND hWnd);

private:
	CWinEnumManager(const CWinEnumManager&);
	CWinEnumManager& operator = (const CWinEnumManager&);

private:
	CWinEnum m_winEnum;
	CWinFilter m_winFilter;
};

#endif //__WIN_ENUM_MANAGER_H__