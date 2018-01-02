#pragma  once

#define PORT_WRITE_BUFFER_SIZE	1024

class CSmsComPortAgentWnd;
class CGPRSManager;

class CSerialPortEx
{
//function
public:
	CSerialPortEx(CSmsComPortAgentWnd* p, CGPRSManager* pManager);
	~CSerialPortEx(void);
	//打开串口
	bool initPort(UINT portnr=1,UINT baud=9600,char parity='N',UINT databits=8,UINT stopbits=1);
	//关闭串口
	void releasePort();

	//串口写入数据
	void WriteToPort(BYTE* p,int nLen);
	//启动监视线程
	bool StartMonitoring();
	//停止监视线程
	void StopMonitoring();
	bool isPortOpen(){return m_bOpen;}
	bool isMonitorRunning(){return m_bRunning;}

public:	
	CSmsComPortAgentWnd* m_pAgentWnd;

protected:
	//串口监视线程
	static UINT __stdcall CommThread(LPVOID pParam);
	//读取数据
	static void ReceiveChar(CSerialPortEx* port,COMSTAT& comstat);
	//写入数据
	static void	WriteChar(CSerialPortEx* port);
private:
	
	CSerialPortEx();		//防止缺省构造
	unsigned long			m_hThread;
	CRITICAL_SECTION		m_csCommunicationSync;
	HANDLE			m_hShutdownEvent;
	HANDLE			m_hComm;
	HANDLE			m_hWriteEvent;
	HANDLE			m_hEventArray[3];
	OVERLAPPED			m_ov;
	COMMTIMEOUTS		m_CommTimeouts;

	BYTE				m_szWriteBuffer[PORT_WRITE_BUFFER_SIZE];//写缓冲
	bool				m_bRunning;		    //线程是否正在运行		
	bool				m_bOpen;		    //串口是否已经初始化
	DCB				m_dcb;
	int				m_nToSend;			//需要发送的信息的长度
};
