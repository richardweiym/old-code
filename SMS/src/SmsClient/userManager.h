#pragma once 

#include <atlmisc.h>

class CSmsUser
{
public:
	CSmsUser();
	CSmsUser(const CSmsUser& user);
public:
	UINT m_nId;
	CString m_strName;
	CString m_strPwd;
	BOOL m_bAutoConnect;
	BOOL m_bTipOn;
	CString m_strServer;
	INT m_nPort;
};

class CUserManager
{
public:
	CUserManager();
	~CUserManager();
	
	CSmsUser* GetCurrentUser(BOOL& bExist);
	BOOL SaveCurrentUser();

private:
	CSmsUser* m_pCurrentUser;
	BOOL m_bLoaded;
};