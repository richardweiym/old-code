#include "HYUtilBase.h"
#include "SCCanvasApp.h"
#include "SCCanvasWindow.h"
#include "SCCanvasSkin.h"
#include "SCCanvasConfig.h"

CSCCanvasApp::CSCCanvasApp()
{
	HY_RUN_ONCE_CHECK
}

CSCCanvasApp::~CSCCanvasApp()
{

}
CRefPtr<CSCCanvasConfig> CSCCanvasApp::GetCanvasConfig() const
{
	return m_pConfig;
}

CRefPtr<CSCCanvasSkin> CSCCanvasApp::GetCanvasSkin() const
{
	return m_pSkin;
}

CRefPtr<CSCCanvasWindow> CSCCanvasApp::GetCanvasWindow() const
{
	return m_pCanvas;
}

BOOL CSCCanvasApp::Initialize(LPCTSTR lpszCmdLine)
{
	_ASSERTE(m_pConfig.IsNull());
	if(m_pConfig.IsNull())
	{
		m_pConfig = new CSCCanvasConfig;
	}
	if(m_pConfig.IsNull()) return FALSE;
	m_pConfig->LoadConfig();


	_ASSERTE(m_pCanvas.IsNull());
	if(m_pCanvas.IsNull())
	{
		m_pCanvas = new CSCCanvasWindow;
	}
	if(m_pCanvas.IsNull())	return FALSE;

	if(!m_pCanvas->Initialize()) return FALSE;


	_ASSERTE(m_pSkin.IsNull());
	if(m_pSkin.IsNull())
	{
		m_pSkin = new CSCCanvasSkin;
	}
	if(m_pSkin.IsNull()) return FALSE;
	
	if(!m_pSkin->Initialize()) return FALSE;


	BOOL bShowStartWin(TRUE);
	if(lpszCmdLine != NULL && lstrcmpi(lpszCmdLine, _T("0")) == NULL)
	{
		bShowStartWin = FALSE;
	}
	CRefPtr<CSCCanvasContext> pCanvasContext = GetCanvasContext();
	_ASSERTE(!pCanvasContext.IsNull());
	pCanvasContext->SetShowStartWin(bShowStartWin);

	return TRUE;
}

BOOL CSCCanvasApp::InitializeUI()
{
	if(!m_pCanvas->InitializeUI())
	{
		return FALSE;
	}

	return TRUE;
}

VOID CSCCanvasApp::Uninitialize()
{
	if(!m_pCanvas.IsNull())
	{
		m_pCanvas->Uninitialize();
		m_pCanvas = NULL;
	}

	if(!m_pSkin.IsNull())
	{
		m_pSkin->Uninitialize();
		m_pSkin	 = NULL;
	}

	if(!m_pConfig.IsNull())
	{
		m_pConfig->SaveConfig();
		m_pConfig = NULL;
	}
}

VOID CSCCanvasApp::UnInitializeUI()
{
	if(!m_pCanvas.IsNull())
	{
		m_pCanvas->UninitializeUI();
	}
}

