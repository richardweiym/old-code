#ifndef __WIN_DEBUG_H__
#define __WIN_DEBUG_H__

#include <string>

class CWinDebug
{
public:
	CWinDebug(HWND hWnd);
	~CWinDebug();

	std::wstring GetDebugText() const;

private:
	CWinDebug(const CWinDebug&);
	CWinDebug& operator=(const CWinDebug&);

private:
	HWND m_hWnd;
};


#endif //__WIN_DEBUG_H__