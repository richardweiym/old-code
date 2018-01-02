#ifndef __WIN_OUTPUT_H__
#define __WIN_OUTPUT_H__

class CWinOutput
{
public:
	static CWinOutput& GetInstance();

	VOID Clear();
	VOID SelectAll();

	VOID Init(HWND hWndEdit);
	VOID OutputMessage(LPCTSTR lpszMsg, BOOL bAddIndex = TRUE);

private:
	CWinOutput();
	~CWinOutput();

	CWinOutput(const CWinOutput&);
	CWinOutput& operator = (const CWinOutput&);

private:
	HWND m_hWndEdit;
	static CWinOutput s_outputInstance;
	INT m_nLineIndex;
};


#endif //__WIN_OUTPUT_H__