#pragma  once
#include "../ClientSocketLib/SmsClientInterface.h"

class CMainFrame;
class CUserManager;
class CSmsUser;
class CSocketClient;
class CSplashDlg;

	enum INIT_STEP
	{
		ON_START = 0,
		ON_CREATE,
		ON_LOAD_TOSEND,
		ON_LOAD_SENDED,
		ON_LOAD_RECEIVED,
		ON_LOAD_DELETED,
		ON_LOAD_CONTACT,
		ON_FINISHED
	};
	
class CServiceManager:public ISmsClientSink
{
public:


	CServiceManager();
	~CServiceManager();

	//ISmsClientSink
	virtual VOID OnConnect(BOOL bOK);
	virtual VOID OnSend(CSMSMessage* pMsg);
	virtual VOID OnReceive(CSMSMessage* pMsg);
	virtual VOID OnServiceStop();
	
	VOID OnInitStep(INIT_STEP eStep);

	VOID StartService();
	CUserManager* GetUserManager();
	CSocketClient* GetSmsClient();
private:
	CSocketClient* m_pClient;
	CMainFrame* m_pMainFrame;
	CUserManager* m_pUserManager;

	CSplashDlg* m_pSplashWnd;
};