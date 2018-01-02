#pragma once 

#include "HYUtilBase.h"
#include "HYUIBase.h"
#include "HYAutoPtr.h"

class CSCCanvasApp;
class CSCCanvasSkin;
class CSCCanvasWindow;
class CSCCanvasData;
class CSCCanvasContext;
class CSCCanvasConfig;



CSCCanvasApp& GetCanvasApp();

CRefPtr<CSCCanvasSkin> GetCanvasSkin();

CRefPtr<CSCCanvasWindow> GetCanvasWindow();

CRefPtr<CSCCanvasData> GetCanvasData();

CRefPtr<CSCCanvasContext> GetCanvasContext();

CRefPtr<CSCCanvasConfig> GetCanvasConfig();
