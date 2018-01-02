#pragma  once 

#include "mainfrm.h"

class CIconTray
{
public:
	VOID AddTray(NOTIFYICONDATA& nid);
	VOID ModifyTray(NOTIFYICONDATA& nid);
	VOID DeleteTray(NOTIFYICONDATA& nid);
};

class CSmsTray
{
public:
	VOID AddTray();
	VOID ModifyTray(CMainFrame::ENUM_SMS_CLIENT_STATUS state);
	VOID DeleteTray();

private:
	CIconTray m_tray;
};
