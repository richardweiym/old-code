#pragma  once 

#include <atlmisc.h>
#include "DBOperate.h"
#include <atlctrls.h>

template<typename CMessageType> class CMessageManager;
class CDeletedMessage;

class CToSendMessage
{
public:
	CToSendMessage();
	
	static  VOID GetTypeName(CString& str);
	static	BOOL GetRecordset(_Recordset** pRst, BOOL bDeleted = FALSE);
	static  BOOL GetSmsMessage(_RecordsetPtr pRst, CToSendMessage* pMsg);
	static  INT AddMessage( CToSendMessage& Msg);
	static  BOOL RemoveMessage(INT nId);
	static  VOID CreateListHeader(CListViewCtrl& list);
	static  VOID InsertItem(CListViewCtrl& list,CToSendMessage* pMsg, BOOL bTop = FALSE);
	static  VOID GetListItem(CListViewCtrl& list, INT nItem, CString& strContactCaption
					, CString& strContactName, CString& strTimeCaption, CString& strTimeValue, CString& strContent);
	static BOOL Get1ToSendMsg(CListViewCtrl& list, CToSendMessage& msg);
	static BOOL SetSendedMsg(CListViewCtrl& list, CToSendMessage& msg, BOOL);
	static VOID ResetSendingMsg(CListViewCtrl& list, INT nId);
	static BOOL DeleteSelMsg(CListViewCtrl& list, CSimpleArray<CDeletedMessage*>& arDelete, BOOL bDelAll);
	static VOID GetItemByIndex(CListViewCtrl& list, INT nIndex, CToSendMessage& msg);
public:
	INT m_nToSendId;
	CString m_strTel;
	CString m_strContent;
	BOOL m_bSendPromptly;
	CString m_strToSendTime;
	CString m_strCreateTime;
	INT m_nUserId;
	CString m_strName;
};

class CSendedMessage
{
public:
	CSendedMessage();
	CSendedMessage(const CToSendMessage& msg, BOOL bOK);
	CSendedMessage(const CSendedMessage& msg);
	
	static  VOID GetTypeName(CString& str);
	static	BOOL GetRecordset(_Recordset** pRst, BOOL bDeleted = FALSE);
	static  BOOL GetSmsMessage(_RecordsetPtr pRst, CSendedMessage* pMsg);
	static  INT AddMessage( CSendedMessage& Msg);
	static  BOOL RemoveMessage(INT nId);
	static  VOID CreateListHeader(CListViewCtrl& list);
	static  VOID InsertItem(CListViewCtrl& list,CSendedMessage* pMsg, BOOL bTop = FALSE);
	static  VOID GetListItem(CListViewCtrl& list, INT nItem, CString& strContactCaption
					, CString& strContactName, CString& strTimeCaption, CString& strTimeValue, CString& strContent);
	static BOOL SetSendedMsg(CListViewCtrl& list, CToSendMessage& msg, BOOL bOK);
	static BOOL DeleteSelMsg(CListViewCtrl& list, CSimpleArray<CDeletedMessage*>& arDelete, BOOL bDelAll);
	static VOID GetItemByIndex(CListViewCtrl& list, INT nIndex, CSendedMessage& msg);
public:
	INT m_nSendedId;
	INT m_nToSendId;
	CString m_strTel;
	CString m_strContent;
	BOOL m_bSendPromptly;
	CString m_strToSendTime;
	CString m_strSendedTime;
	CString m_strCreateTime;
	INT m_nUserId;
	BOOL m_bSendedOK;
	CString m_strName;;
	BOOL m_bDeleted;
};

class CReceiveMessage
{
public:
	CReceiveMessage();
	static  VOID GetTypeName(CString& str);
	static	BOOL GetRecordset(_Recordset** pRst, BOOL bDeleted = FALSE);
	static  BOOL GetSmsMessage(_RecordsetPtr pRst, CReceiveMessage* pMsg);
	static  INT AddMessage( CReceiveMessage& Msg);
	static  BOOL RemoveMessage(INT nId);
	static  VOID CreateListHeader(CListViewCtrl& list);
	static  VOID InsertItem(CListViewCtrl& list,CReceiveMessage* pMsg, BOOL bTop = FALSE);
	static  VOID GetListItem(CListViewCtrl& list, INT nItem, CString& strContactCaption
					, CString& strContactName, CString& strTimeCaption, CString& strTimeValue, CString& strContent);
	static  BOOL SetReceivedMsg(CListViewCtrl& list, CReceiveMessage& msg);
	static	BOOL SetReadedMsg(CListViewCtrl& list, INT nIndex); 
	static BOOL DeleteSelMsg(CListViewCtrl& list, CSimpleArray<CDeletedMessage*>& arDelete, BOOL bDelAll);
	static VOID GetItemByIndex(CListViewCtrl& list, INT nIndex, CReceiveMessage& msg);
public:
	INT m_nReceiveId;
	CString m_strTel;
	CString m_strContent;
	CString m_strReceiveTime;
	BOOL m_bReaded;
	INT m_nUserId;
	CString m_strName;
	BOOL m_bDeleted;
};

class CDeletedMessage
{
public:

	CDeletedMessage();
	CDeletedMessage(const CToSendMessage& msg);
	CDeletedMessage(const CSendedMessage& msg);
	CDeletedMessage(const CReceiveMessage& msg);
	static  VOID GetTypeName(CString& str);
	static	BOOL GetRecordset(_Recordset** pRst, BOOL bDeleted = FALSE);
	static  BOOL GetSmsMessage(_RecordsetPtr pRst, CDeletedMessage* pMsg);
	static  INT  AddMessage( CDeletedMessage& Msg);
	static  BOOL RemoveMessage(INT nId);
	static  VOID CreateListHeader(CListViewCtrl& list);
	static  VOID InsertItem(CListViewCtrl& list,CDeletedMessage* pMsg, BOOL bTop = FALSE);
	static  VOID GetListItem(CListViewCtrl& list, INT nItem, CString& strContactCaption
					, CString& strContactName, CString& strTimeCaption, CString& strTimeValue, CString& strContent);
	static BOOL GetCurrentSelInfo(CListViewCtrl& list, CString& strName, CString& strTel ,CString& m_strContent);
	static BOOL DeleteSelMsg(CListViewCtrl& list, CSimpleArray<CDeletedMessage*>& arDelete, BOOL bDelAll);
	static BOOL SetDeleteMsg(CListViewCtrl& list, CSimpleArray<CDeletedMessage*>& arDelete);
	static VOID GetItemByIndex(CListViewCtrl& list, INT nIndex, CDeletedMessage& msg);
public:
	INT m_nId;
	CString m_strTel;
	CString m_strName;
	CString m_strContent;
	CString m_strEventTime;
	INT m_nUserId;
	// 1发送失败 2发送成功 3接收成功
	INT m_nStatus;
};

template<typename CMessageType>
class CMessageManager
{
public:
	CMessageManager()
	{
		m_pRst = NULL;
		m_pCurrentMsg = new CMessageType;
	}
	~CMessageManager()
	{
		if(m_pCurrentMsg != NULL)
		{
			delete m_pCurrentMsg;
			m_pCurrentMsg = NULL;
		}
		CloseRecordset();
	}
	CMessageType* GetFirst(BOOL bDeleted = FALSE)
	{
		CMessageType* pMsg = NULL;
		CloseRecordset();
		BOOL b = CMessageType::GetRecordset(&m_pRst, bDeleted);
		if(b)
		{
			try
			{
				if(!m_pRst->GetADOEOF())
				{
					CMessageType::GetSmsMessage(m_pRst, m_pCurrentMsg);
					m_pRst->MoveNext();
					pMsg = m_pCurrentMsg;
				}
				else
				{
					CloseRecordset();
				}
			}
			catch (...)
			{
				NULL;
			}
		}

		return pMsg;
	}

	CMessageType* GetNext()
	{
		CMessageType* pMsg = NULL;
		if(m_pRst != NULL)
		{
			try
			{
				if(m_pRst->State == adStateOpen)
				{
					if(!m_pRst->GetADOEOF())
					{
						CMessageType::GetSmsMessage(m_pRst, m_pCurrentMsg);
						m_pRst->MoveNext();
						pMsg = m_pCurrentMsg;					
					}
					else
					{
						CloseRecordset();
					}
				}
			}
			catch (...)
			{
				NULL;	
			}
		}

		return pMsg;
	}
	BOOL SetMessageDeleted(INT nId)
	{
		return CMessageType::SetMessageDeleted(nId);
	}
	INT AddMessage(CMessageType& msg)
	{
		return CMessageType::AddMessage(msg);
	}
	BOOL RemoveMessage(INT nId)
	{
		return CMessageType::RemoveMessage(nId);
	}

protected:
	VOID CloseRecordset()
	{
		if(m_pRst != NULL)
		{
			try
			{
				if(m_pRst->State == adStateOpen)
				{
					m_pRst->Close();
				}
				m_pRst = NULL;
			}
			catch (...)
			{
				NULL;	
			}
		}
	}
private:
	_RecordsetPtr m_pRst;
	CMessageType* m_pCurrentMsg;
};
