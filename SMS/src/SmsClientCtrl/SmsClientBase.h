#pragma  once 

// SMS client callback message type

//it will return this callback message after call connect() function.
const int MESSAGE_TYPE_CONNECT = 1;	

//it will return this callback message after call send_message() function.		
const int MESSAGE_TYPE_SEND_RETURN = 2;
	
//it will return this callback message when server receive new message.	
const int MESSAGE_TYPE_RECEIVE	= 3;	

//it will return this callback message when server stopped or connection is disconnected.	
const int MESSAGE_TYPE_SERVICE_STOP = 4;	

// callback message type
typedef void (__stdcall *SMS_CLIENT_CALLBACK)
				(int nMsgType,					// message type	,this parameter is always used.
				const char* lpszTel,			// phone number, when message type is MESSAGE_TYPE_SEND_RETURN 
												// or MESSAGE_TYPE_RECEIVE, this parameter is used.
				const char* lpszContent,		// content of message,when message type is MESSAGE_TYPE_SEND_RETURN 
												// or MESSAGE_TYPE_RECEIVE, this parameter is used.
				const char* lpszTag,			// id(tag) of send message, when message type is MESSAGE_TYPE_SEND_RETURN,
												// this parameter is used(it can be null),
				bool bOK);						// is executed result is OK, when message type is MESSAGE_TYPE_CONNECT
												// or MESSAGE_TYPE_SEND_RETURN, this parameter is used,
												// when connected OK or message send OK, is is TRUE,
												// when it is FALSE, you can call get_last_error_code() to know the reason.



#ifdef __cplusplus 
extern "C"
{
#endif
	// function: connect to server
	// parameters:
	//		lpszServerIP -- address of server IP
	//		uPort -- server socket listening port
	//		lpszLogonName -- logon name of account
	//		lpszLogonPassword -- logon password of account
	// return:
	//       it always return true,
	//		 note: if connected ok should judged by callback message MESSAGE_TYPE_CONNECT
	bool __stdcall connect_to_server
		(const char* lpszServerIP,
		int uPort, 
		const char* lpszLogonName, 
		const char* lpszLogonPassword );

	// function: disconnect from server
	// parameters: NULL
	// return:
	//       it always return true,
	bool __stdcall disconnect();

	// function: send message to server to send
	// parameters:
	//		lpszTel -- phone number 
	//		lpszContent -- content
	//		lpszTag -- tag or id of this message, it can be null
	// return:
	//       if this message is successfully send to server, it return true, otherwise it return false.
	//		 note: if this message is send to mobile successfully should judged by MESSAGE_TYPE_SEND_RETURN 
	bool __stdcall send_message
		(const char* lpszTel, 
		const char* lpszContent, 
		const char* lpszTag);
	
	//function: set callback
	void __stdcall set_callback(SMS_CLIENT_CALLBACK);
	
	//function: get last error code when return false
	int  __stdcall get_last_error_code();

#ifdef __cplusplus
}	
#endif

//error code
const int CLIENT_ERROR_CREATE_SOCKET = 101;
const int CLIENT_ERROR_IP_ADDRESS = 102;
const int CLIENT_ERROR_CONNECT = 103;
const int CLIENT_ERROR_CREATE_THREAD = 104;
const int CLIENT_ERROR_CREATE_AGENT = 105;
const int CLIENT_ERROR_ACCOUNT_ALREADY_USED = 106;
const int CLIENT_ERROR_ACCOUNT_NOTEXIST = 107;
const int CLIENT_ERROR_NOT_CONNECT = 108;
const int CLIENT_ERROR_BUSY_SENDING = 109;
