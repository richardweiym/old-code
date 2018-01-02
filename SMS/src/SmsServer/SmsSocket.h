#pragma  once 

class CSmsSocketCommunicate;
class CSMSMessage;
#include "SmsSocketProtocol.h"

class CSmsSocket
{
public:
	CSmsSocket(SOCKET socket,BOOL bClient);
	virtual ~CSmsSocket();
	//发送信息
	BOOL SocketSend(CSMSMessage* pMsg);
	//客户端关闭
	VOID OnClose();
	//收到客户端新消息
	VOID OnRecv(SMS_SOCKET_HEADER* header);

	static VOID SetSocketManager(CSmsSocketCommunicate* pManager);

protected:
	INT SendWrap(char* p,int nLen);
	INT ReadWrap(char* p,int nLen);

private:
	GUID	m_gId;					//客户端ID	
	SOCKET  m_socket;	
	BOOL    m_bClient;				//是否是客户端
	BOOL    m_bLogoned;			    //是否已经验证通过
	BOOL    m_bDispatch;			//是否有收取短信的权限
	static  CSmsSocketCommunicate* m_pSocketManager;
	
	BOOL	m_bConnected;
	friend class  CSmsSocketCommunicate;
};