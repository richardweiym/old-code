#pragma  once

#include <atlwin.h>
#include "resource.h"
#include "DBOperate.h"

class CSmsSearchDlg: public CDialogImpl<CSmsSearchDlg>
{
public:
	enum ENUM_SEARCH_TYPE {ALL = 0, TOSEND, SENDED, RECEIVED, DELETED};
	enum {IDD = IDD_SEARCH};

	BEGIN_MSG_MAP(CSmsSearchDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_CLOSE, OnExit)
		COMMAND_ID_HANDLER(IDC_CLEAR, OnClear)
		COMMAND_ID_HANDLER(IDC_SEARCH, OnSearch)
		
		COMMAND_ID_HANDLER(ID_SMS_REPLY, OnSmsReply)
		COMMAND_ID_HANDLER(ID_SMS_TRANSFER, OnSmsTransfer)
		COMMAND_ID_HANDLER(ID_SMS_SAVE, OnSmsSave)

		NOTIFY_ID_HANDLER(IDC_MESSAGE, OnListviewNotify)
	END_MSG_MAP()
	
	CSmsSearchDlg();
	LRESULT OnInitDlg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnExit(WORD ,WORD, HWND, BOOL&);
	LRESULT OnClear(WORD, WORD, HWND, BOOL&);
	LRESULT OnSearch(WORD, WORD, HWND, BOOL&);

	LRESULT OnSmsReply(WORD, WORD, HWND, BOOL&);
	LRESULT OnSmsTransfer(WORD, WORD, HWND, BOOL&);
	LRESULT OnSmsSave(WORD, WORD, HWND, BOOL&);

	LRESULT OnListviewNotify(INT, LPNMHDR pnmh , BOOL&);
protected:
	VOID CloseRst();
	VOID Close();
	VOID Clear();
	
	BOOL GetCurrentSQL(CString& strSQL);
	BOOL GetCurrentSelInfo(CString& strName, CString& strTel, CString& strContent);
private:
	ENUM_SEARCH_TYPE m_eType;
	_RecordsetPtr m_pRst;
};