#include "stdafx.h"
#include "WinOutput.h"
#include "Edit.h"
#include <sstream>

using namespace std;

CWinOutput CWinOutput::s_outputInstance;


CWinOutput::CWinOutput()
:m_hWndEdit(NULL), m_nLineIndex(0)
{

}

CWinOutput::~CWinOutput()
{

}

CWinOutput& CWinOutput::GetInstance()
{
	return s_outputInstance;
}

VOID CWinOutput::Init(HWND hWndEdit)
{
	m_hWndEdit = hWndEdit;

	CEditHandle edit(m_hWndEdit);
	edit.SetLimitTextLength(0x7FFFFFFE);
}

VOID CWinOutput::SelectAll()
{
	CEditHandle edit(m_hWndEdit);
	edit.SetSelAll(TRUE);
}

VOID CWinOutput::Clear()
{
	CEditHandle edit(m_hWndEdit);
	edit.SetSelAll(TRUE);
	edit.Clear();

	m_nLineIndex = 0;
}


VOID CWinOutput::OutputMessage(LPCTSTR lpszMsg, BOOL bAddIndex /*= TRUE*/)
{
	wostringstream os;
	if(bAddIndex)
	{
		os  << ++m_nLineIndex;
		os << L"\t";
	}

	if(lpszMsg != NULL)
	{
		os << lpszMsg;
	}
	
	CEditHandle edit(m_hWndEdit);
	edit.AppendText(os.str().c_str());
}



