#include "DBOperate.h"

CDBOperate::CDBOperate()
{

}
CDBOperate::~CDBOperate()
{

}
BOOL CDBOperate::Open(LPCTSTR szConString)
{
	return TRUE;
}
BOOL CDBOperate::IsOPen()
{
	return m_bOpen;
}
VOID CDBOperate::Close()
{

}
BOOL CDBOperate::ExecuteSQL(LPCTSTR szSQL)
{
	return TRUE;
}