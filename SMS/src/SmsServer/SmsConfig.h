#pragma  once 

class ISmsConfig
{
public:
	virtual ~ISmsConfig() {}

	virtual INT GetUseDeviceTag(LPTSTR szBuf,INT nBufLen) = 0;
	virtual INT GetUseDeviceParameters(LPTSTR szBuf,INT nBufLen) = 0;
	virtual INT GetUseDeviceDllName(LPTSTR szBuf, INT nBufLen) =0;

	virtual INT GetUseCommunicateTag(LPTSTR szBuf,INT nBufLen) =0;
	virtual INT GetUseCommunicateParameters(LPTSTR szBuf,INT nBufLen) =0;

	virtual INT SetUserDeviceParameters(LPCTSTR str) = 0;
	virtual INT SetUserCommunicateParameters(LPCTSTR str) = 0;
};

class CSmsIniConfig: public ISmsConfig
{
public:
	CSmsIniConfig(){}
	virtual ~CSmsIniConfig() {}

	virtual INT GetUseDeviceTag(LPTSTR szBuf,INT nBufLen) ;
	virtual INT GetUseDeviceParameters(LPTSTR szBuf,INT nBufLen) ;
	virtual INT GetUseDeviceDllName(LPTSTR szBuf, INT nBufLen) ;

	virtual INT GetUseCommunicateTag(LPTSTR szBuf,INT nBufLen) ;
	virtual INT GetUseCommunicateParameters(LPTSTR szBuf,INT nBufLen) ;

	virtual INT SetUserDeviceParameters(LPCTSTR str) ;
	virtual INT SetUserCommunicateParameters(LPCTSTR str) ;

protected:
	INT GetUseValueFromIni(LPCTSTR szSection,LPCTSTR szKey,LPTSTR szBuf,INT nBufLen);
	INT SetUserValueToIni(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue);
};
