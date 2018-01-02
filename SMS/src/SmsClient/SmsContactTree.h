#pragma  once 

#include <atlbase.h>
#include <atlctrls.h>

class CContactManager;
class CContact;

class CSmsContactTree: public CWindowImpl<CSmsContactTree,CTreeViewCtrl,CControlWinTraits>
{
public:
	BEGIN_MSG_MAP(CSmsContactTree)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	CSmsContactTree();
	~CSmsContactTree();

	LRESULT OnCreate(UINT nMsgType, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnDestroy(UINT nMsgType, WPARAM, LPARAM, BOOL& bHandled);
	
	VOID SelectItemById(INT nId);

	BOOL AddTeam(CString& strName);	
	BOOL ModifyTeam(INT nId, CString& strName);
	BOOL DeleteTeam(INT nId);
	BOOL AddContact(CString& strName, CString& strTel, INT nParentId);
	BOOL ModifyContact(CContact& c);
	BOOL DeleteContact(INT nId);
	VOID InitContacts();
	
	CSimpleMap<HTREEITEM, CContact*>& GetItemMap() {return m_MapItem;}
	CContactManager* GetContactManager() {return m_pContactManager;}
private:
	VOID ClearMap();

private:
	CImageList m_imagelist;
	CContactManager* m_pContactManager;
	CSimpleMap<HTREEITEM, CContact*> m_MapItem;
};
