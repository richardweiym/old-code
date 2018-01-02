#ifndef __WIN_ENUM_H__
#define __WIN_ENUM_H__

#include <vector>

class IWinEnumEventSink
{
public:
	virtual VOID OnEnumStart() = 0;
	virtual VOID OnEnumEnd() = 0;

	virtual VOID OnEnumWindow(HWND hWnd) = 0;
};

class CWinEnum
{
public:
	CWinEnum();
	~CWinEnum();

	VOID SetEnumSink(IWinEnumEventSink* pSink);

	BOOL StartEnum();

private:
	CWinEnum(const CWinEnum&);
	CWinEnum& operator=(const CWinEnum&);


private:
	IWinEnumEventSink* m_pEnumSink;
	std::vector<HWND> m_arWinList;
};


#endif //__WIN_ENUM_H__