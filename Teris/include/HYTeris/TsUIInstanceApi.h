#pragma  once 

#include "TsStdAfx.h"

class CTsGameContext;
class CTsResMgr;
class CTsDataFactory;
class CTsGameBaseInfo;
class CTsPlayProcessInfo;
class CTsKeysTable;
class CTsGameMainWnd;
class CTsUserListData;

CTsGameMainWnd* TsGetGameMainWnd();
CTsGameContext* TsGetGameContext();
CTsResMgr* TsGetResMgr();

CTsDataFactory* TsGetDataFactory();

CTsGameBaseInfo* TsGetGameBaseInfo();
CTsPlayProcessInfo* TsGetGameProcessInfo();

CTsKeysTable* TsGetKeysTable();
CTsUserListData* TsGetUserList();