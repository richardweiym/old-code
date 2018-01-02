#pragma  once 

#include "SmsMessage.h"

#define SMS_SOCKET_SEND_TAG		"SMS_SEND_TAG"
#define SMS_SOCKET_DISPATCH_TAG  "SMS_DISPATCH_TAG"
#define SMS_SOCKET_CLOSE_TAG	"SMS_CLOSE_TAG"
#define SMS_SOCKET_LOGON_TAG	"SMS_LOGON_TAG"
#define SMS_SOCKET_SENDBACK_TAG "SMS_SENDBACK_TAG"
#define SMS_SOCKET_STOP_TAG		"SMS_STOP_TAG"
#define SMS_SOCKET_HEART_BEAT_TAG "SMS_HEART_BEAT_TAG"

typedef enum			
{
	SMSSEND = 0,		//发送短信
	SMSRECEIVE = 1,		//收到短信
	SMSCLOSE = 2,		//客户端请求断开
	SMSLOGON = 3,		//客户端发送帐号登陆	
	SMSSENDBACK = 4,	//通知客户端短信是否发送成功
	SMSSTOP = 5	,		//服务停止
	SMSHEARTBEAT = 6
} ENUM_SMS_SOCKET_PACKAGE_TYPE;

struct SMS_SOCKET_HEADER
{
	char sHeadTag[20];		//包类型标记
	int	 nP1Len;			//号码或是帐号长度
	int	 nP2Len;			//短信内容或是密码长度
	int	 nP3Len;			//消息ID长度
	int	 bOK;			    //是否成功标记(0 失败, 1 成功 ,3登陆时帐号已在使用)
} ;
