#pragma  once 
#include "atlmisc.h"

enum SMS_AT_COMMAND_RESULT
{
	AT_COMMAND_OK = 0,
	AT_COMMAND_FAIL = 1,
	AT_COMMAND_TIMEOUT = 3,
	AT_COMMAND_OTHER = 4

};

class ISmsATCommand
{
public:

	ISmsATCommand()
	{
		m_nResult = -1;
		m_szCmd = NULL;
		m_szOK = new char[30];
		lstrcpy(m_szOK,"\r\nOK\r\n");
		m_szError = new char[30];
		lstrcpy(m_szError,"\r\nERROR\r\n");
	}
	virtual ~ISmsATCommand()
	{
		if(m_szOK != NULL)
		{
			delete []m_szOK;
			m_szOK = NULL;
		}
		if(m_szError != NULL)
		{
			delete []m_szError;
			m_szError = NULL;
		}
		if(m_szCmd != NULL)
		{
			delete []m_szCmd;
			m_szCmd = NULL;
		}
	}

	virtual char* GetCommand(){return m_szCmd;}
	virtual INT   GetLength() {return lstrlen(m_szCmd);}

	virtual SMS_AT_COMMAND_RESULT  ParseResult(char* pData) = 0;
	virtual INT  GetResult() {return m_nResult;}

protected:
	 char* m_szOK;
	 char* m_szError;
	 char* m_szCmd;
	 INT m_nResult;

};

class CCPINCommand:public ISmsATCommand
{
public:
	CCPINCommand()
	{
		m_szCmd = new char[30];
		lstrcpy(m_szCmd, "AT+CPIN?\n");	
	}
	virtual ~CCPINCommand()
	{
		if(m_szCmd != NULL)
		{
			delete []m_szCmd;
			m_szCmd = NULL;
		}
	}

	virtual SMS_AT_COMMAND_RESULT ParseResult(char* pData)
	{		
		char szType[] = "\r\n+CPIN: READY\r\n";
		char szOther[]="\r\n+CME";
		SMS_AT_COMMAND_RESULT eRet(AT_COMMAND_OTHER);
	    CString str(pData);

		if(str.Find(szType) >=0 )
		{
			eRet = AT_COMMAND_OK;
		}
		else if(str.Find("\r\n+CPIN:"))
		{
			eRet = AT_COMMAND_FAIL;
		}else if(str.Find(szOther) >= 0)
		{
			eRet = AT_COMMAND_FAIL;
		}

		return eRet;
	}
	
};

class CSimpleCommand:public ISmsATCommand
{
public:
	CSimpleCommand(char* pData = NULL)
	{
		if(pData != NULL)
		{
			m_szCmd = new char[100];
			lstrcpy(m_szCmd,pData);
		}
	}
	virtual  ~CSimpleCommand()
	{
		if(m_szCmd != NULL)
		{
			delete []m_szCmd;
			m_szCmd = NULL;
		}
	}

	virtual SMS_AT_COMMAND_RESULT ParseResult(char* pData)
	{
		CString s(pData);
		SMS_AT_COMMAND_RESULT eRet(AT_COMMAND_OTHER);

		if(s.Find(m_szOK) >= 0)
		{
			eRet = AT_COMMAND_OK;
		}
		else if(s.Find(m_szError) >= 0)
		{
			eRet = AT_COMMAND_FAIL;
		}
		return eRet;
	}
};

class CCMEECommand:public CSimpleCommand
{
public:
	CCMEECommand():CSimpleCommand("AT+CMEE=0\n")
	{
	}
};
class CCMGFCommand:public CSimpleCommand
{
public:
	CCMGFCommand():CSimpleCommand("AT+CMGF=0\n")
	{
	}
};
class CCNMICommand:public CSimpleCommand
{
public:
	CCNMICommand():CSimpleCommand("AT+CNMI=2,1,0,0,0\n")
	{
	}
};
class CCSCACommand:public CSimpleCommand
{
public:
	CCSCACommand(char* szNum)
	{
		m_szCmd = new char[100];
		lstrcpy(m_szCmd, "AT+CSCA=");
		lstrcat(m_szCmd, szNum);
		lstrcat(m_szCmd, "\n");
	}
};
class CCMGDALLCommand:public CSimpleCommand
{
public:
	CCMGDALLCommand():CSimpleCommand("AT+CMGD=1,4\n")
	{
	}
};
class CCSQCommand:public ISmsATCommand
{
public:
	CCSQCommand()
	{
		m_szCmd = new char[30];
		lstrcpy(m_szCmd, "AT+CSQ\n");
		m_szType = new char[30];
		lstrcpy(m_szType, "\r\n+CSQ");
	}
	virtual ~CCSQCommand()
	{
		if(m_szCmd != NULL)
		{
			delete []m_szCmd;
			m_szCmd = NULL;
		}
		if(m_szType != NULL)
		{
			delete []m_szType;
			m_szType = NULL;
		}
	}
	virtual SMS_AT_COMMAND_RESULT  ParseResult(char* pData)
	{
		CString s(pData);
		SMS_AT_COMMAND_RESULT eRet(AT_COMMAND_OTHER);
		INT nStart = s.Find(m_szType);

		if(nStart >= 0)
		{
			if(s.Find(m_szOK) >= 0)
			{
				char* p = pData + nStart;
				eRet = AT_COMMAND_OK;
				sscanf(p,"\r\n+CSQ: %d,",&m_nResult);	
			}
		
		}
		return eRet;
	}
	
private:
	char* m_szType;
};

class CCMGSCommand:public ISmsATCommand
{
public:
	CCMGSCommand(const char* szEncodedContent, const char* szTel)
	{
		CString str;
		CString str1(szTel),str2(szEncodedContent);
		PackagePDUCString(str1, str2, str);
		m_szCmd = new char[str.GetLength()+1];
		lstrcpyn(m_szCmd, (LPCTSTR)str,str.GetLength()+1);

		m_szType = new char[30];
		lstrcpy(m_szType, "\r\n+CMGS");
	}
	virtual ~CCMGSCommand()
	{
		if(m_szCmd != NULL)
		{
			delete []m_szCmd;
			m_szCmd = NULL;
		}
		if(m_szType != NULL)
		{
			delete []m_szType;
			m_szType = NULL;
		}
	}
	virtual SMS_AT_COMMAND_RESULT ParseResult(char* pData)
	{
		SMS_AT_COMMAND_RESULT eRet(AT_COMMAND_OTHER);
		
		CString s(pData);
		

		if(s.Find(m_szError) >= 0)
		{
			eRet = AT_COMMAND_FAIL;
		}
		else
		{
			if(s.Find(m_szType) >= 0)
			{
				if(s.Find(m_szOK) >= 0)
				{
					eRet  = AT_COMMAND_OK;
				}
				else if(s.Find(m_szError) >= 0)
				{
					eRet = AT_COMMAND_FAIL;
				}
			}
		}
		return eRet;
	}
private:
int PackagePDUCString(CString& strPhone, CString& sContent,CString & str)
{

	int PhoneLen=strPhone.GetLength();
	int NewLen=PhoneLen;

	char ex[3]={char(26),'\n',0};
	char pEncodePhone[50]={0};
	str="AT+CMGS=";
	
	char* sConst="\n0011000";//B81";

	if(PhoneLen%2!=0)
	{
		strPhone+="F";
		NewLen+=1;
	}
	char sTemp[10]={0};

	int nTotal=sContent.GetLength()/2+NewLen/2+8;

	itoa(nTotal,sTemp,10);
	str+=sTemp;
	str+=sConst;
	char* p=pEncodePhone;
	itoa(PhoneLen,sTemp,16);
	str+=sTemp[0];//加上号码长度
	str+="81";
	for(int i=0 ;i<NewLen;i=i+2)
	{
		*(p++)=strPhone.GetAt(i+1);
		*(p++)=strPhone.GetAt(i);
	}
	str+=pEncodePhone;
	str+="000801";
	memset(sTemp,0,4);
	itoa(sContent.GetLength()/2,sTemp,16);
	if(sTemp[1]==0)
	{
		str+="0";
		str+=sTemp[0];
	}
	else
	{
		str+=sTemp[0];
		str+=sTemp[1];
	}

	str+=sContent;

	str+=ex;


	return 0;
}
private:
	char* m_szType;
};
class CCMGRCommand:public ISmsATCommand
{
public:
	CCMGRCommand(INT nIndex,char* pContent, char* pTel ,INT nBufLen1, INT nBufLen2)
	{
		m_nIndex = nIndex;
		m_pContentBuf = pContent;
		m_pTelBuf = pTel;
		m_nBufLen1 = nBufLen1;
		m_nBufLen2 = nBufLen2;

		m_szCmd = new char[30];
		_snprintf(m_szCmd, 29, "AT+CMGR=%d\n",nIndex);

		m_szType = new char[30];
		lstrcpy(m_szType, "\r\n+CMGR");
	}
	virtual ~CCMGRCommand()
	{
		if(m_szCmd != NULL)
		{
			delete []m_szCmd;
			m_szCmd = NULL;
		}
		if(m_szType != NULL)
		{
			delete []m_szType;
			m_szType = NULL;
		}
	}
	virtual SMS_AT_COMMAND_RESULT ParseResult(char* pData)
	{
		SMS_AT_COMMAND_RESULT eRet(AT_COMMAND_OTHER);
		CString s(pData);

		if(s == m_szError)
		{
			eRet = AT_COMMAND_FAIL;
		}
		else
		{
			INT nStart = s.Find(m_szType);
			if(nStart >= 0)
			{
				if(s.Find(m_szOK >= 0))
				{
					CString sNum(""),sContent("");
					CString str(pData + nStart, s.GetLength()-nStart);
					if(ParseRecvMsg(str,sNum,sContent))
					{
						eRet = AT_COMMAND_OK;
						lstrcpyn(m_pContentBuf,sContent,m_nBufLen1);
						lstrcpyn(m_pTelBuf, sNum, m_nBufLen2);
					}
// 					else
// 					{
// 						OutputDebugString("ParseRecvMsg error\n");
// 					}

				}
				else if(s.Find(m_szError) >= 0)
				{
					eRet = AT_COMMAND_FAIL;
				}
			}
		}
		return eRet;		

	}
private:
 BYTE GetMoveCode(int i)
{
	switch(i)
	{
	case 0:	return	 1;
	case 1: return	 3;
	case 2: return	 7;
	case 3: return	 15;
	case 4: return	 31;
	case 5: return	 63;
	case 6: return   127;
	default:return   255;
	}	
	
}
 INT  GetHexCode(char c)
{
	INT nRet=-1;
	switch(c)
	{
	case '0':
		nRet=0;
		break;
	case '1':
		nRet=1;
		break;
	case '2':
		nRet=2;
		break;
	case '3':
		nRet=3;
		break;
	case '4':
		nRet=4;
		break;
	case '5':
		nRet=5;
		break;
	case '6':
		nRet=6;
		break;
	case '7':
		nRet=7;
		break;
	case '8':
		nRet=8;
		break;
	case '9':
		nRet=9;
		break;
	case 'A':
	case 'a':
		nRet=10;
		break;
	case 'B':
	case 'b':
		nRet=11;
		break;
	case 'C':
	case 'c':
		nRet=12;
		break;
	case 'D':
	case 'd':
		nRet=13;
		break;
	case 'E':
	case 'e':
		nRet=14;
		break;
	case 'F':
	case 'f':
		nRet=15;
		break;
	}
	return (BYTE)nRet;
}
char* DecodeToAscii(const char* pWchar)
{
	int nLen=(int)strlen(pWchar);
	char *pMulti=new char[nLen/2+2];
	char* pRet;
	int iPos=0;
	for(int i=0;i<nLen;i+=4)
	{	
		pMulti[iPos++]=char(GetHexCode(pWchar[i+2])*16+GetHexCode(pWchar[i+3]));
		pMulti[iPos++]=char(GetHexCode(pWchar[i])*16+GetHexCode(pWchar[i+1]));
	}
	pMulti[nLen/2]=pMulti[nLen/2+1]=0;
	int k=::WideCharToMultiByte(CP_ACP,0,(wchar_t*)pMulti,-1,NULL,0,NULL,NULL);
	pRet=new char[k+1];
	pRet[k]=0;
	::WideCharToMultiByte(CP_ACP,0,(wchar_t*)pMulti,-1,pRet,k+1,NULL,NULL);

	delete [] pMulti;
	return pRet;
}
BYTE* DecodeUTF7(const char* p)
{
	int len=(int)strlen(p);
	//BYTE* byteBuffer=new BYTE[len/2+1];
	BYTE* byteBuffer=new BYTE[len/2+10];
	int j=0;
	for(int i=0;i+1<len;i+=2)
	{
		byteBuffer[j++]=GetHexCode(p[i])*16+GetHexCode(p[i+1]);
	}
	byteBuffer[j]=0;
	
	//BYTE* pResult=new BYTE[j*8/7+1];
	BYTE* pResult=new BYTE[j*8/7+10];
	int iCur=0;
	int t;
	BYTE h(0),e(0);
	for(int k=0;k<j;k++)
	{
		t=(k+1)%7;
		e=h;				
		h=(byteBuffer[k]>>(8-t))&GetMoveCode(t-1);//取头
		if(t==0)
		{
			h=(byteBuffer[k]>>1)&0xFF;
		}
		if(t==1)
		{
			pResult[iCur++]=byteBuffer[k]&0x7F;		//每过7个字节开始处,内有8个字符
		}
		else
		{
			pResult[iCur++]=((byteBuffer[k]<<((t+7-1)%7))|e)&0x7f;
			if(t==0)
			{
				pResult[iCur++]=h;
			}
		}
	}
	pResult[iCur]=0;
	delete []byteBuffer;
	return pResult;
}
BOOL ParseRecvMsg(const CString& strInfo, CString& strNum, CString& strContent)
{
	BOOL bRet(FALSE);
	INT LenNum=-1;
	INT nEnd;
	INT nStart;
	char flag(0);
	//获得收到短信在手机中的位置
	char sp[3]={char(13),char(10),0};
	nStart=strInfo.Find(sp,7);//从"+CMGR"后面开始读
	if(nStart>0) 
	{
		nStart=nStart+23;
		if(nStart<strInfo.GetLength())
		{
			char c=strInfo.GetAt(nStart);
			LenNum=GetHexCode(c);//号码长度
		}
		if(LenNum>0)
		{
			nStart=nStart+3;
			CString strTemp=strInfo.Mid(nStart,LenNum+1);
			char c;
			for(int i=0;i<=LenNum;i=i+2)
			{
				c=strTemp.GetAt(i);
				if(i+1<=LenNum)
				{
					strTemp.SetAt(i,strTemp.GetAt(i+1));//格式化号码
					strTemp.SetAt(i+1,c);
				}
			}
			strNum="+"+strTemp.Left(LenNum);

			if(LenNum%2==1) 
			{
				flag=strInfo.GetAt(nStart+LenNum+4);
				nStart=nStart+LenNum+21;        //加上号码中多余的'F'
				
			}
			else
			{
				flag=strInfo.GetAt(nStart+LenNum+3);
				nStart=nStart+LenNum+20;
				
			}
			nEnd=strInfo.Find(sp,nStart);
			if(nEnd>0)
			{
				strContent=strInfo.Mid(nStart,nEnd-nStart);
			}
		
		}
	}

	if((strNum!="")&&(strContent!=""))
	{
		bRet = TRUE;
		char* pMyDecodeString = NULL;

		if(flag=='8')
			pMyDecodeString=DecodeToAscii(strContent);
		else
			pMyDecodeString=(char*)DecodeUTF7(strContent);

		strContent = pMyDecodeString;

		delete []pMyDecodeString;
		pMyDecodeString=NULL;

	}		
	return bRet;
}
private:
	char* m_pContentBuf;
	char* m_pTelBuf;
	char* m_szType;
	INT m_nBufLen1;
	INT m_nBufLen2;

	INT m_nIndex;
};

class CCMTICommand:public ISmsATCommand
{
public:
	CCMTICommand()
	{
		m_szType = new char[30];
		lstrcpy(m_szType, "\r\n+CMTI");
	}
	virtual ~CCMTICommand()
	{
		if(m_szType != NULL )
		{
			delete []m_szType;
			m_szType = NULL;
		}
	}
	virtual SMS_AT_COMMAND_RESULT ParseResult(char* pData)
	{
		SMS_AT_COMMAND_RESULT eRet(AT_COMMAND_OTHER);
	
		CString s(pData);
		INT nStart = s.Find(m_szType);

		if( nStart >= 0)
		{
			CString sT(pData + nStart, s.GetLength() - nStart);
			eRet = AT_COMMAND_OK;
			sscanf(sT,"\r\n+CMTI: \"SM\",%d\r\n",&m_nResult);
		}
		return eRet;	
	}

private:
	char* m_szType;
};

class CCMGDCommand:public ISmsATCommand
{
public:
	CCMGDCommand(INT nIndex)
	{
		m_szCmd = new char[30];
		_snprintf(m_szCmd, 30,"AT+CMGD=%d\n",nIndex);
	}
	virtual ~CCMGDCommand()
	{
		if(m_szCmd != NULL)
		{
			delete []m_szCmd;
			m_szCmd = NULL;
		}
	}
	virtual SMS_AT_COMMAND_RESULT ParseResult(char* pData)
	{
		SMS_AT_COMMAND_RESULT eRet(AT_COMMAND_OTHER);
		CString s(pData);
		if(s.Find(m_szOK) >= 0)
		{
			eRet = AT_COMMAND_OK;
		}
		return eRet;
	}
};

class CCMGLCommand: public ISmsATCommand
{
public:
	CCMGLCommand(INT nType, CSimpleArray<UINT>& arMsg)
	:m_arMsg(arMsg)
	{
		m_szCmd = new char[30];
		_snprintf(m_szCmd, 30, "AT+CMGL=%d\n", nType);
		m_szType = new char[30];
		lstrcpy(m_szType, "\r\n+CMGL: ");
	}
	virtual ~CCMGLCommand()
	{
		if(m_szCmd != NULL)
		{
			delete []m_szCmd;
			m_szCmd = NULL;
		}
		if(m_szType != NULL)
		{
			delete []m_szType;
			m_szType = NULL;
		}
	}
	virtual SMS_AT_COMMAND_RESULT ParseResult(char* pData)
	{
		SMS_AT_COMMAND_RESULT eRet(AT_COMMAND_OTHER);
		CString strData(pData);
		if(strData.Find(m_szError) >= 0)
		{
			eRet = AT_COMMAND_FAIL;
		}
		else
		{
			if(strData.Find(m_szOK) >= 0)
			{
				eRet = AT_COMMAND_OK;				
			
				INT nStart = strData.Find(m_szType);
				while(nStart >= 0)
				{
					INT nEnd = strData.Find(',', nStart);
					if(nEnd >= 0)
					{
						INT nLen = nEnd - nStart;
						if(nLen > 0 && nLen	< 15)
						{
							CString strTemp(strData.Mid(nStart, nLen));
							INT nIndex(INT_MAX);
							sscanf(strTemp, "\r\n+CMGL: %d,", &nIndex);
							if(nIndex != INT_MAX)
							{
								m_arMsg.Add((UINT&)nIndex);
							}
						}
					}

					 nStart = strData.Find(m_szType, nStart+5);	
				}		
			}
		}
		return eRet;
	}

private:
	CSimpleArray<UINT>& m_arMsg;
	char* m_szType;
};

