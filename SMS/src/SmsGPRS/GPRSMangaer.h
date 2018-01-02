#pragma  once 

#include "../SmsServer/SmsDeviceExport.h"
#include "SmsATCommand.h"
#include <atlmisc.h>

class CSmsComPortAgentWnd;
class CSerialPortEx;
class CSMSMessage;
class CMemPool;

class CGPRSManager
{
public:
	CGPRSManager() ;
	~CGPRSManager();

	VOID SetPort(int n);
	VOID SetCenterNum(const char* sNum);
	VOID SetCallBack(SMS_DEVICE_CALLBACK);
	VOID SetMaxFailCount(INT n);
	VOID SetClearTag(BOOL b);

	INT GetLastErrorCode(){return m_nError;}
	BOOL IsPortBusying(){return m_bPortBusying;}
	VOID SetPortBusying(BOOL b) {m_bPortBusying = b;}
	BOOL SendMsg(CSMSMessage*, BOOL bSendAtOnce = FALSE);	
	
	BOOL Init();
	BOOL Start();
	BOOL Stop();
	BOOL IsRunning();

	BYTE* GetBuffer(UINT& nSize);
	VOID FreeBuffer(BYTE* pBuf);
	BOOL IsCanCheck() {return m_bCanCheck;}

protected:
	VOID CheckStoredMsg();
	BOOL DoATCommand(INT nTimeOut);
	VOID OnCommandReturn(const char*, INT nLen);
	VOID DoReadMsg(INT nIndex);
	VOID ResetCommand();
	char* EncodeToUnicode(const char* pMsg);
	bool checkIsNumberString(const CString& str);
private:
	friend class CSmsComPortAgentWnd;
	
	CMemPool* m_pMemPool;
	BOOL m_bCanCheck;

	ISmsATCommand* m_pCurrntCommand;
	ISmsATCommand* m_pCMTICommand;
	ISmsATCommand* m_pCMGLCommand;
	CSimpleArray<UINT> m_arCMGL;

	char m_szCenterNum[100];
	INT m_nPort;
	INT m_nError;
	BOOL m_bRunning;
	CSmsComPortAgentWnd* m_pAgentWnd;
	CSerialPortEx* m_pPort;
	INT m_bPortBusying;
	INT m_bCommandReturn;
	SMS_DEVICE_CALLBACK m_pCallBack;
	SMS_AT_COMMAND_RESULT m_eResult;

	BOOL m_bNeedFireStop;

	BOOL m_bClearWhenStart;
	INT  m_nMaxFailCount;
};