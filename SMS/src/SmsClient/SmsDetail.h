#pragma  once 

#include <atlwin.h>
#include "resource.h"
#include <windowsx.h>
#include <atlmisc.h>

class CSmsDetailDlg: public CDialogImpl<CSmsDetailDlg>
{
public:
	enum { IDD = IDD_DETAIL };
	BEGIN_MSG_MAP(CSmsDetailDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		//MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	END_MSG_MAP()

	CSmsDetailDlg();
	~CSmsDetailDlg();

	LRESULT OnInitDialog(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCtlColorEdit(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	VOID SetValues(CString& strContactCaption, CString& strContactName, CString& strTimeCaption, CString& strTimeValue, CString& strContent);
private:
	HBRUSH m_hContentBrush;
};