#include "HYUtilBase.h"
#include "SCCanvasContext.h"
#include "SCIDDef.h"
#include "SCCanvasConfig.h"
#include "SCInstanceAPI.h"

using namespace std;

CSCCanvasContext::CSCCanvasContext()
{
}

void CSCCanvasContext::InitContextData()
{
	InitToolsData();

	m_eToolType = TOOL_TYPE_NONE;
	m_bShowHelp = TRUE;
	m_bDoubleClickExit = TRUE;
	m_bShowCursor = TRUE;
	m_bShowCursor = TRUE;

	m_bShowStartWin = TRUE;
	m_bTransparentMark = TRUE;
	m_nFileFilterIndex = 1;

	m_bExitAfterImageSaved = FALSE;
}

BOOL CSCCanvasContext::Initialize()
{
	InitContextData();

	CRefPtr<CSCCanvasConfig> pConfig = GetCanvasConfig();
	_ASSERTE(!pConfig.IsNull());
	INT nShowTip = pConfig->GetShowHelpTipFlag();
	if(nShowTip != -1)
	{
		m_bShowHelp = nShowTip;
	}
	
	INT nDoubleClickExit = pConfig->GetDoubleClickExit();
	if(nDoubleClickExit != -1)
	{
		m_bDoubleClickExit = nDoubleClickExit;
	}

	INT nTransparentMark = pConfig->GetTransparentMark();
	if(nTransparentMark != -1)
	{
		m_bTransparentMark = nTransparentMark;
	}

	INT nShowCursor = pConfig->GetShowCursor();
	if(nShowCursor != -1)
	{
		m_bShowCursor = nShowCursor;
	}

	INT nFileFilterIndex = pConfig->GetFileFilterIndex();
	if(nFileFilterIndex != -1)
	{
		m_nFileFilterIndex = nFileFilterIndex;
	}
	
	INT nExitAfterImageSaved = pConfig->GetExitAfterImageSaved();
	if(nExitAfterImageSaved != -1)
	{
		m_bExitAfterImageSaved = nExitAfterImageSaved;
	}

	return TRUE;
}

VOID CSCCanvasContext::Uninitialize()
{
	m_mapTools.clear();

	CRefPtr<CSCCanvasConfig> pConfig = GetCanvasConfig();
	_ASSERTE(!pConfig.IsNull());
	pConfig->SetShowHelpTipFlag(m_bShowHelp);
	pConfig->SetDoubleClickExit(m_bDoubleClickExit);
	pConfig->SetTransparentMark(m_bTransparentMark);
	pConfig->SetShowCursor(m_bShowCursor);
	pConfig->SetFileFilterIndex(m_nFileFilterIndex);
	pConfig->SetExitAfterImageSaved(m_bExitAfterImageSaved);
}

void CSCCanvasContext::InitToolsData()
{
	m_mapTools.clear();
	
	CANVAS_TOOL_DATA ta;
	for(int i=TOOL_TYPE_START; i<TOOL_TYPE_END; ++i)
	{
		m_mapTools.insert(make_pair((CANVAS_TOOL_TYPE)i, ta));
	}
}

CSCCanvasContext::~CSCCanvasContext()
{
	m_mapTools.clear();
}

void CSCCanvasContext::SetToolData(CANVAS_TOOL_TYPE eType, CANVAS_TOOL_DATA ta)
{
	CToolMap::iterator itr =m_mapTools.find(eType);
	if(itr != m_mapTools.end())
	{
		itr->second = ta;
	}
	else
	{
		_ASSERTE(FALSE);
	}
}


BOOL CSCCanvasContext::GetToolData(CANVAS_TOOL_TYPE eType, CANVAS_TOOL_DATA& taRet)
{
	CToolMap::iterator itr =m_mapTools.find(eType);
	if(itr != m_mapTools.end())
	{
		taRet = itr->second;
		return TRUE;
	}
	else
	{
		_ASSERTE(FALSE);
		return FALSE;
	}
}


DWORD CSCCanvasContext::SetToolType(DWORD eType)
{
	DWORD eTypeOld = m_eToolType;
	if((DWORD)m_eToolType != eType)
	{
		m_eToolType = (CANVAS_TOOL_TYPE)eType;
		NotifyObservers(UI_WPARAM_TOOL_CHANGED, (LPARAM)eType);
	}
	
	return eTypeOld;
}

CANVAS_TOOL_DATA* CSCCanvasContext::GetCurrentToolData()
{
	if(m_eToolType >= TOOL_TYPE_START && m_eToolType < TOOL_TYPE_END)
	{
		return &m_mapTools[m_eToolType];
	}
	else
	{
		_ASSERTE(FALSE);
		return NULL;
	}
}

VOID CSCCanvasContext::SetCurrentToolData( const CANVAS_TOOL_DATA& data)
{
	if(m_eToolType >= TOOL_TYPE_START && m_eToolType < TOOL_TYPE_END)
	{
		m_mapTools[m_eToolType] = data;
	}
	else
	{
		_ASSERTE(FALSE);
	}
}

VOID CSCCanvasContext::SetShowHelpFlag(BOOL bSet)
{
	if(m_bShowHelp != bSet)
	{
		m_bShowHelp = bSet;
	}
}

VOID CSCCanvasContext::SetDoubleClickExitFlag(BOOL bSet)
{
	if(m_bDoubleClickExit != bSet)
	{
		m_bDoubleClickExit = bSet;
	}
}