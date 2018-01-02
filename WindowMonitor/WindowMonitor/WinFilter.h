#ifndef __WIN_FILTER_H__
#define __WIN_FILTER_H__

class CWinFilter
{
public:
	CWinFilter();
	~CWinFilter();

	BOOL IsTargetWindow(HWND hWnd) const;

private:
	CWinFilter(const CWinFilter&);
	CWinFilter& operator=(const CWinFilter&);
};

#endif //__WIN_FILTER_H__