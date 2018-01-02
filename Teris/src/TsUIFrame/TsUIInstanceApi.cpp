#include "TsUIInstanceApi.h"

extern CTsGameMainWnd* g_gameMainWnd;
extern CTsGameContext* g_gameContext;
extern CTsResMgr* g_gameResMgr;
extern CTsDataFactory* g_gameDataFactory;
extern CTsGameBaseInfo* g_gameBaseInfo;
extern CTsPlayProcessInfo* g_gamePlayInfo;
extern CTsKeysTable* g_gameKeysTable;
extern CTsUserListData* g_pUserList;


CTsGameMainWnd* TsGetGameMainWnd()
{
	return g_gameMainWnd;
}
CTsGameContext* TsGetGameContext()
{
	return g_gameContext;
}

CTsResMgr* TsGetResMgr()
{
	return g_gameResMgr;
}

CTsDataFactory* TsGetDataFactory()
{
	return g_gameDataFactory;
}

CTsGameBaseInfo* TsGetGameBaseInfo()
{
	return g_gameBaseInfo;
}

CTsPlayProcessInfo* TsGetGameProcessInfo()
{
	return g_gamePlayInfo;
}

CTsKeysTable* TsGetKeysTable()
{
	return g_gameKeysTable;
}
CTsUserListData* TsGetUserList()
{
	return g_pUserList;
}