#pragma once 

#import "C:\Program Files\Common Files\System\ado\msado20.tlb" rename_namespace("ADODB") rename("EOF","ADOEOF")
using namespace ADODB;

class CDBOperate
{
public:
	CDBOperate();
	~CDBOperate();
	BOOL Open(LPCTSTR szConString);
	BOOL IsOPen();
	VOID Close();
	BOOL ExecuteSQL(LPCTSTR szSQL);
	BOOL GetRecordset(LPCTSTR szSQL, _Recordset** ppRst);
	_ConnectionPtr GetConnection() {return m_pDBCon;}

private:
	_ConnectionPtr m_pDBCon;
};

class CSmsDB
{
public:
	static CDBOperate* GetDBOperate();
private:
	static CDBOperate s_DBOperate;
};

// class CSmsServerDB
// {
// public:
// 	static CDBOperate* GetDBOperate();
// 
// private:
// 	static CDBOperate* s_DBOperate;
//};