#include "SCInstanceAPI.h"
#include "HYPattern.h"
#include "SCCanvasApp.h"
#include "SCCanvasData.h"
#include "SCCanvasWindow.h"
#include "SCCanvasSkin.h"
#include "SCCanvasContext.h"
#include "SCCanvasConfig.h"

CSCCanvasApp& GetCanvasApp()
{
	return *(CSCCanvasApp::GetInstance());
}

CRefPtr<CSCCanvasSkin> GetCanvasSkin()
{
	return GetCanvasApp().GetCanvasSkin();
}

CRefPtr<CSCCanvasWindow> GetCanvasWindow()
{
	return GetCanvasApp().GetCanvasWindow();
}



CRefPtr<CSCCanvasData> GetCanvasData()
{
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(!pCanvas.IsNull())
	{
		return pCanvas->GetCanvasData();
	}

	return NULL;
}

CRefPtr<CSCCanvasContext> GetCanvasContext()
{
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	if(!pCanvas.IsNull())
	{
		return pCanvas->GetCanvasContext();
	}

	return NULL;
}



CRefPtr<CSCCanvasConfig> GetCanvasConfig()
{
	return GetCanvasApp().GetCanvasConfig();
}