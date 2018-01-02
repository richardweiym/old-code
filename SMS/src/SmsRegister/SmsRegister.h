#pragma  once 

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlmisc.h>

//class CString;

//检测当前的机器是否已经注册
BOOL CheckIsRegistered();

//获取当前机器的机器码
BOOL GetMachineCode(CString& strMachineCode);

//根据机器码获取注册码
BOOL GetRegisterCodeByMachineCode(const CString& strMachineCode, CString& strRegisterCode);

//注册账号
BOOL RegisterAccount(const CString& strMachineCode, CString& strRegisterCode);