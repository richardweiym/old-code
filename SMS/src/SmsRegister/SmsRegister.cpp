#include "SmsRegister.h"
#include "myBase64.h"
#include "des.h"

#pragma comment(lib, _T("des.lib"))

#include <nb30.h>

#pragma comment(lib,"Netapi32.lib")

#define FILENAME "reg.dat"
LONGLONG getMACAddress()
{
	struct ASTAT
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER	   NameBuff[30];
	} Adapter;
	LONGLONG ret=0;
	NCB Ncb;
	UCHAR uRetCode;
	LANA_ENUM lenum;
	int i=0;
	//枚举网络设备
	memset(&Ncb,0,sizeof(Ncb));
	Ncb.ncb_command=NCBENUM;
	Ncb.ncb_buffer=(UCHAR*)&lenum;
	Ncb.ncb_length=sizeof(lenum);
	uRetCode=Netbios(&Ncb);
	
	//重置
	memset(&Ncb,0,sizeof(Ncb));
	Ncb.ncb_command=NCBRESET;
	Ncb.ncb_lana_num=lenum.lana[i];
	uRetCode=Netbios(&Ncb);

	//得到状态
	memset(&Ncb,0,sizeof(Ncb));
	Ncb.ncb_command=NCBASTAT;
	Ncb.ncb_lana_num=lenum.lana[i];

	strcpy((char*)Ncb.ncb_callname,"*");
	Ncb.ncb_buffer=(UCHAR*)&Adapter;
	Ncb.ncb_length=sizeof(Adapter);

	uRetCode=Netbios(&Ncb);
	//如果成功，拷贝MAC地址
	if(uRetCode==0)
	{
		memcpy(&ret,Adapter.adapt.adapter_address,6);
	}
	return ret;	
}
bool inline getModPath(char* sBuf,const char* sName,int nLen)
{
	bool bOK(false);
	DWORD dw;
	char* p;
	if(nLen<MAX_PATH) return bOK;
	dw=::GetModuleFileName(NULL,sBuf,MAX_PATH);
	if(dw<=0) return bOK;
	p=strrchr(sBuf,'\\');
	*(++p) = 0;
	strcat(sBuf,sName);
	bOK=true;
	return bOK;
}
BOOL SaveDataToFile(const CString& strMac, const CString& strCode)
{
	BYTE* sBuffer;
	BYTE* p;
	bool b;
	BOOL bOK;
	DWORD dwWritten;
	HANDLE hFile(INVALID_HANDLE_VALUE);
	char sPath[MAX_PATH]={0};
	char sKey[]={char(123),'w',char(55),char(66),char(74),'y','m',0};
	struct tagHead
	{
		int nLen1;
		int nLen2;
		int nLen3;
		int nLen4;
	} head;

	CString strTime;
	CString strToCode;
	int nTotalLen,nTimeLen,nMacLen,nCodeLen;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strTime.Format("%d-%d-%d %d:%d:%d:%d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	nTimeLen=strTime.GetLength();
	nMacLen=strMac.GetLength();
	nCodeLen=strCode.GetLength();
	nTotalLen=nTimeLen+nMacLen+nCodeLen+sizeof(head);

	if(nTotalLen%8!=0)
	{
		nTotalLen=nTotalLen+(8-nTotalLen%8);
	}

	head.nLen1=nTotalLen;
	head.nLen2=nTimeLen;
	head.nLen3=nMacLen;
	head.nLen4=nCodeLen;

    sBuffer=new BYTE[nTotalLen+1];
	memset(sBuffer,0,nTotalLen+1);

	p=sBuffer;
	memcpy(p,&head,sizeof(head));
	p+=sizeof(head);
	memcpy(p,(LPCTSTR)strTime,nTimeLen);
	p+=nTimeLen;
	memcpy(p,(LPCSTR)strMac,nMacLen);
	p+=nMacLen;
	memcpy(p,(LPCSTR)strCode,nCodeLen);

	des3((UCHAR*)sBuffer,(UCHAR*)sKey,3,nTotalLen);

	b=getModPath(sPath,FILENAME,MAX_PATH);
	if(!b) goto end;

	hFile=CreateFile(sPath,FILE_WRITE_DATA,FILE_SHARE_READ,NULL,CREATE_ALWAYS,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE) goto end; 
	bOK=WriteFile(hFile,sBuffer,nTotalLen,&dwWritten,NULL);
	if(!bOK||(dwWritten!=(DWORD)nTotalLen)) goto end;
	CloseHandle(hFile);
	delete []sBuffer;
	
	return TRUE;
end:
	delete []sBuffer;
	if(hFile!=INVALID_HANDLE_VALUE) CloseHandle(hFile);
	return FALSE;
}
BOOL ReadDataFromFile(CString& strMachineCode, CString& strRegisterCode)
{
	char szMacBuf[512] = {0};
	char strCodeBuf[512] = {0};
	HANDLE hFile(INVALID_HANDLE_VALUE);
	char sPath[MAX_PATH]={0};
	bool b;
	BYTE* pBuffer;
	DWORD dwFileLen;
	BOOL bOK;
	DWORD dwWritten;
	BYTE* p;
	char sKey[]={char(123),'w',char(55),char(66),char(74),'y','m',0};

	struct tagHead
	{
		int nLen1;
		int nLen2;
		int nLen3;
		int nLen4;
	} head;
	
	pBuffer=NULL;
	b=getModPath(sPath,FILENAME,MAX_PATH);
	if(!b) goto end;
	hFile=CreateFile(sPath,FILE_READ_DATA|FILE_READ_ATTRIBUTES,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE) goto end;
	dwFileLen=GetFileSize(hFile,NULL);
	
	if(dwFileLen < 50) goto end;

	pBuffer=new BYTE[dwFileLen+1];
	bOK=ReadFile(hFile,pBuffer,dwFileLen,&dwWritten,NULL);
	if(!bOK||(dwFileLen!=dwWritten)) goto end;

	CloseHandle(hFile);
	hFile=INVALID_HANDLE_VALUE;

	Ddes3((UCHAR*)pBuffer,(UCHAR*)sKey,3,dwFileLen);	

	memcpy(&head,pBuffer,sizeof(head));
	if(head.nLen1!=(int)dwFileLen) goto end;

	
	p=pBuffer;
	p+=sizeof(head);
	p+=head.nLen2;
	memcpy(szMacBuf,p,head.nLen3);
	szMacBuf[head.nLen3]=0;
	p+=head.nLen3;
	memcpy(strCodeBuf,p,head.nLen4);
	strCodeBuf[head.nLen4]=0;
	
	delete []pBuffer;
	pBuffer=NULL;
	
	strMachineCode = szMacBuf;
	strRegisterCode = strCodeBuf;
	return TRUE;
end:
	strMachineCode = strRegisterCode = _T("");
	if(hFile!=INVALID_HANDLE_VALUE) CloseHandle(hFile);
	if(pBuffer!=NULL) delete []pBuffer;
	return FALSE;
}

BOOL CheckIsRegistered()
{
	LONGLONG ll;
	char szMacBuffer[512]={0};
	char szCodeBuffer[512]={0};
	char szCode[MAX_PATH]={0};
	char sBuf[30]={0};
	char szName[]={char(19),char(81),char(12),char(6),char(19),char(82),char(12),char(30),0};
	char sKey[20]={0};
	char* p;
	int nLen;
	char* pTemp;
	int nMacLen;
	BOOL bOK(TRUE);
	int nCodeLen;
	int nNeed;
	
	CString strMachineCode, strRegisterCode;
	if(!ReadDataFromFile(strMachineCode, strRegisterCode)) return FALSE;
	lstrcpy(szMacBuffer, strMachineCode);
	lstrcpy(szCodeBuffer, strRegisterCode);

	ll=getMACAddress();
	if(ll==0) return FALSE;
	_i64toa(ll,sBuf,16);
	CharUpper(sBuf);
	nLen=strlen(szMacBuffer);
	if(nLen!=(int)strlen(sBuf)) return FALSE;
	for(int i=0;i<nLen;i++)
	{
		if(szMacBuffer[i]!=sBuf[i])
		{
			bOK=FALSE;
			break;
		}
	}
	if(!bOK) return FALSE;

	//生成秘钥
	memset(sKey,0,20);
	strcpy(sKey, szName);
	nMacLen=strlen(szMacBuffer);
	if(nMacLen<=3)
	{
		strcat(sKey,szMacBuffer);
	}
	else
	{
		p=szMacBuffer+nMacLen-3;
		strcat(sKey,p);
	}

	//加密机器码
	memset(szCode,0,MAX_PATH);
	memcpy(szCode,szMacBuffer,nMacLen);
	if(nMacLen%8!=0)
	{
		nMacLen=nMacLen+(8-nMacLen%8);
	}
	des3((UCHAR*)szCode,(UCHAR*)sKey,3,nMacLen);

	//将注册码base64解码
	nCodeLen=strlen(szCodeBuffer);
	nNeed=Base64DecodeGetRequiredLength(nCodeLen);
	pTemp=new char[nNeed+1];
	bOK=Base64Decode(szCodeBuffer,nCodeLen,(BYTE*)pTemp,&nNeed);
	if(!bOK)
	{
			delete []pTemp;
			return FALSE;
	}
	if(nNeed!=nMacLen)
	{
		delete []pTemp;
		return FALSE;
	}
	
	//比较
	bOK=TRUE;
	for(int j=0;j<nNeed;j++)
	{
		if(pTemp[j]!=szCode[j])
		{
			bOK=FALSE;
			break;
		}
	}
	delete []pTemp;
	
	return bOK;

	return bOK;
}
BOOL GetMachineCode(CString& strMachineCode)
{
	__int64 nMac = getMACAddress();
	if(nMac == 0)
	{
		strMachineCode = _T("");
		return FALSE;
	}
	else
	{
		TCHAR szBuf[30] = {0};
		_i64toa(nMac,szBuf,16);
		strMachineCode = CharUpper(szBuf);
		return TRUE;
	}

}
BOOL GetRegisterCodeByMachineCode(const CString& strMachineCode, CString& strRegisterCode)
{
	//生成秘钥
	char szTemp[]  = {char(19),char(81),char(12),char(6),char(19),char(82),char(12),char(30),0};
	CString strKey(szTemp);
	INT nMacLen = strMachineCode.GetLength();

	strKey += strMachineCode.Right(3);

	
	////填充为8的倍数
	char szCode[512] = {0};
	memcpy(szCode, strMachineCode, nMacLen);
	INT nMod = nMacLen % 8;
	if(nMod!=0)
	{
		char* p=szCode + nMacLen;
		for(int i=1; i<=8-nMod; i++)
		{
			*(p++)=0;
		}
		nMacLen=nMacLen+(8-nMod);
	}
	
	//加密
	des3((UCHAR*)szCode, (UCHAR*)(LPCTSTR)strKey, 3, nMacLen);

	//base64编码
	INT nBufLen = Base64EncodeGetRequiredLength(nMacLen);
	char* pBuf = new char[nBufLen+1];
	pBuf[nBufLen] = 0;
	BOOL b = Base64Encode((BYTE*)szCode, nMacLen, pBuf, &nBufLen);	
	if(b)
	{
		pBuf[nBufLen] = 0;
		strRegisterCode = pBuf;
		delete []pBuf;
		return TRUE;
	}
	
	delete []pBuf;
	strRegisterCode = _T("");
	return FALSE;
}
BOOL RegisterAccount(const CString& strMachineCode, CString& strRegisterCode)
{
	BOOL bRet(FALSE);
	CString strCurrentMachine, strCurrentRegister;

	if(GetMachineCode(strCurrentMachine))
	{
		if(strCurrentMachine == strMachineCode)
		{
			if(GetRegisterCodeByMachineCode(strMachineCode, strCurrentRegister))
			{
				if(strCurrentRegister == strRegisterCode)
				{
					if(SaveDataToFile(strMachineCode, strRegisterCode))
					{
						bRet = TRUE;
					}
				}
			}
		}
	}

	return bRet;
}