#pragma  once 

#include "HyPattern.h"
#include "HYAutoPtr.h"

class CSCCanvasSkin;
class CSCCanvasWindow;
class CSCCanvasConfig;

class CSCCanvasApp: public CHYSingleton<CSCCanvasApp>
{
public:
	BOOL Initialize(LPCTSTR lpszCmdLine);
	BOOL InitializeUI();

	VOID Uninitialize();
	VOID UnInitializeUI();
	
	CRefPtr<CSCCanvasWindow> GetCanvasWindow() const;
	CRefPtr<CSCCanvasSkin> GetCanvasSkin()  const;
	CRefPtr<CSCCanvasConfig> GetCanvasConfig() const;

protected:
	CSCCanvasApp();
	~CSCCanvasApp();

	friend class CHYSingleton<CSCCanvasApp>;

private:
	CRefPtr<CSCCanvasWindow> m_pCanvas;
	CRefPtr<CSCCanvasSkin> m_pSkin;
	CRefPtr<CSCCanvasConfig> m_pConfig;
};