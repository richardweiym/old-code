#pragma  once 

#include <atlctrls.h>
#include "messageManager.h"

typedef CWinTraitsOR<LVS_REPORT  | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS> CListTraits;
 
template<typename CMessageType>
class CSmsListView :public CWindowImpl<CSmsListView, CListViewCtrl, CListTraits>
{
public:
	BEGIN_MSG_MAP(CSmsListView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		
	END_MSG_MAP()

	LRESULT OnCreate(UINT nMsgType , WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	/*	m_imagelist.Create(MAKEINTRESOURCE(IDB_SMS_MESSSAGE), 32, 0, RGB(255,0,255));*/
		m_imagelist.Create(32, 32, ILC_COLOR24|ILC_MASK, 6, 0);
		HBITMAP hBmp = LoadBitmap(CMainFrame::GetResourceModule(), MAKEINTRESOURCE(IDB_SMS_MESSSAGE));
		if(hBmp != NULL)
		{
			m_imagelist.Add(hBmp, RGB(255,0,255));
			DeleteObject(hBmp);
		}
		SetImageList(m_imagelist,LVSIL_SMALL);
		
		bHandled = FALSE;
		
		DWORD dwExStyle = GetExtendedListViewStyle();
		dwExStyle |=  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES ;
		SetExtendedListViewStyle(dwExStyle);

		CMessageType::CreateListHeader(*this);	
		

		return 0;
	}
	BOOL Get1ToSendMessage(CToSendMessage& msg)
	{
		return 	CToSendMessage::Get1ToSendMsg(*this, msg);
	}
	BOOL SetSendedMessage(CToSendMessage& msg, BOOL bOK)
	{
		return CMessageType::SetSendedMsg(*this, msg, bOK);
	}
	VOID ResetSendingMsg(INT nId)
	{
		 CToSendMessage::ResetSendingMsg(*this, nId);
	}
	BOOL SetReceivedMsg(CReceiveMessage* msg)
	{
		return CReceiveMessage::SetReceivedMsg(*this, *msg);
	}
	VOID InsertMsg(CMessageType* pMsg)
	{
		CMessageType::InsertItem(*this, pMsg);
	}
	BOOL GetCurrentSelInfo(CString& strName, CString& strTel, CString& strContent)
	{
		BOOL bSel(FALSE);
		for(int i=0; i<GetItemCount(); i++)
		{
			bSel = GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED;
			if(bSel)
			{
				GetItemText(i, 1, strName);
				GetItemText(i, 2, strTel);
				GetItemText(i, 3, strContent);
				break;
			}
		}

		return bSel;
	}
	BOOL DeleteSelMsg(CSimpleArray<CDeletedMessage*>& arDelete, BOOL bDelAll)
	{
		return CMessageType::DeleteSelMsg(*this, arDelete, bDelAll);
	}
	BOOL SetDeleteMsg(CSimpleArray<CDeletedMessage*>& arDelete)
	{
		return	CDeletedMessage::SetDeleteMsg(*this, arDelete);
	}
	LRESULT LoadListContent()
	{
		const INT MAX_LIST_COUNT = 10000;
		
		DeleteAllItems();

		CMessageType* pMsg = m_msgManager.GetFirst();
		if(pMsg != NULL)
		{
			CMessageType::InsertItem(*this, pMsg);
			
			pMsg = m_msgManager.GetNext();
			while(pMsg != NULL)
			{	
				CMessageType::InsertItem(*this, pMsg);
				pMsg = m_msgManager.GetNext();
				
				if(GetItemCount() >= MAX_LIST_COUNT)
				{
					CString strText;
					CString strName;
					CMessageType::GetTypeName(strName);
					strText.Format(_T("%s信箱已满，短信信箱的最大短信数是%u!"), strName, MAX_LIST_COUNT);
					MessageBox(strText, _T("提示信息"), MB_OK|MB_ICONINFORMATION);
					break;
				}
			}
		}
		return 0;
	}

	INT GetCount(BOOL bSelCount = FALSE)
	{
		if(bSelCount) 
		{
			return	GetSelectedCount();
		}
		else		
		{
			return GetItemCount();
		}
	}

	VOID GetTypeName(CString& strName)
	{
		CMessageType::GetTypeName(strName);
	}
	LRESULT OnDestroy(UINT nMsgType, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_imagelist.Destroy();
		return 0;
	}
	VOID GetSelItemValue(INT nItem,CString& s1,CString& s2, CString& s3, CString& s4,CString& s5) 
	{
		CMessageType::GetListItem(*this,nItem,s1,s2,s3,s4,s5);
	}
private:
	CImageList m_imagelist;
	CMessageManager<CMessageType> m_msgManager;
};

