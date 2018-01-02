#pragma  once 

#include "SmsGPRSDevice.h"
#include "SmsSocketCommunicate.h"

class ISMSDeviceSink;
class ISMSCommunicateSink;

#define  SMS_GPRS_DEVICE_TAG "GPRS"

#define  SMS_SOCKET_COMMUNICATE_TAG "SOCKET"

class CSmsFactory
{
public:
	static CSmsFactory& GetFactoryInstance()
	{
		static CSmsFactory f;
		return f;
	}
	
	ISmsCommunicate* CreateCommunicateObject(LPCTSTR szTag,ISMSCommunicateSink* p)
	{
		if(lstrcmpi(szTag, SMS_SOCKET_COMMUNICATE_TAG) == 0)
		{
			return  new CSmsSocketCommunicate(p);
		}
		else
		{
			return NULL;
		}
	}

	ISmsDevice* CreateDeviceObject(LPCTSTR szTag, ISMSDeviceSink* p)
	{
		if(lstrcmpi(szTag, SMS_GPRS_DEVICE_TAG) == 0)
		{
			return  new CSmsGPRSDevice(p);
		}
		else
		{
			return NULL;
		}
	}
private:
	CSmsFactory(){}
};