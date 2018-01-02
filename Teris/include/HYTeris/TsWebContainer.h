#pragma  once 

#include "TsStdAfx.h"
#include <EXDISPID.H >
#include <atlcom.h>
#include <comdef.h>
#include <atlhost.h>

#ifndef DISPID_NAVIGATEERROR
#define DISPID_NAVIGATEERROR        271   // Fired to indicate the a binding error has occured
#endif 

// Web browser event handlers
class IWebBrowserEventSink
{
public:

    virtual	 void __stdcall OnBeforeNavigate2 (
        IDispatch* pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName,
        VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel ) { }

    virtual void __stdcall OnNavigateComplete2 ( IDispatch* pDisp, VARIANT* URL ) { }
    virtual void __stdcall OnStatusChange ( BSTR bsText ) { }
    virtual void __stdcall OnCommandStateChange ( long lCmd, VARIANT_BOOL vbEnabled ) { }
    virtual void __stdcall OnDownloadBegin() { }
    virtual void __stdcall OnDownloadComplete() { }
	virtual void __stdcall OnDocumentComplete(IDispatch * pDisp, VARIANT * URL) { }
	virtual void __stdcall OnNavigateError(IDispatch* pDisp, VARIANT* URL
		, VARIANT* Frame, VARIANT* StatusCode, VARIANT_BOOL* Cancel) { }
};

// Web browser operation functions
class IWebBrowserOperationBase
{
public:
	virtual IWebBrowserEventSink* SetWebEventSink(IWebBrowserEventSink* pSink) = 0;

	virtual BOOL Navigate(LPCTSTR lpszURl) = 0;
	virtual BOOL GoBack() = 0;
	virtual BOOL GoForward() = 0;
	virtual BOOL GoHome() = 0;
	virtual BOOL GoSearch() = 0;
	virtual BOOL Refresh() = 0;
    virtual BOOL Refresh2( INT nLevel) = 0;
	virtual BOOL Stop() = 0;

	virtual BOOL get_Application(IDispatch** pRet) = 0;
	virtual BOOL get_Parent(IDispatch** pRet) = 0;
	virtual BOOL get_Container(IDispatch** pRet) = 0;
 	virtual BOOL get_Document(IDispatch** pRet) = 0; 
	virtual BOOL get_TopLevelContainer() = 0;  
	virtual BOOL get_Type(LPTSTR lpszBuffer, INT nBufferLen) = 0;
	virtual BOOL get_Left(long* pRet) = 0;
	virtual BOOL get_Top(long* pRet) = 0;
	virtual BOOL get_Width(long* pRet) = 0;
	virtual BOOL get_Height(long* pRet) = 0;
	virtual BOOL get_LocationName(LPTSTR lpszBuffer, INT nBufferLen) = 0;
	virtual BOOL get_LocationURL(LPTSTR lpszBuffer, INT nBufferLen) = 0;
	virtual BOOL get_Busy() = 0;
};

#define TS_WEB_BROWSER_WIN_ID  28

// Web browser wrapper container
class CTsWebBrowserContainer: public CWindowImpl<CTsWebBrowserContainer>,
							  public IDispEventImpl<TS_WEB_BROWSER_WIN_ID, CTsWebBrowserContainer>,
							  public IWebBrowserOperationBase
{
public:
	DECLARE_WND_CLASS(_T("TsWebBrowserContainer"))
		
	CTsWebBrowserContainer(IWebBrowserEventSink* pEventSink = NULL);
	~CTsWebBrowserContainer();

	virtual IWebBrowserEventSink* SetWebEventSink(IWebBrowserEventSink* pSink);

	virtual BOOL Navigate(LPCTSTR lpszURl);
	virtual BOOL GoBack() ;
	virtual BOOL GoForward() ;
	virtual BOOL GoHome() ;
	virtual BOOL GoSearch() ;
	virtual BOOL Refresh() ;
    virtual BOOL Refresh2( INT nLevel) ;
	virtual BOOL Stop();

	virtual BOOL get_Application(IDispatch** pRet);
	virtual BOOL get_Parent(IDispatch** pRet);
	virtual BOOL get_Container(IDispatch** pRet);	
	virtual BOOL get_Document(IDispatch** pRet);
	virtual BOOL get_TopLevelContainer();  	
	virtual BOOL get_Type(LPTSTR lpszBuffer, INT nBufferLen);
	virtual BOOL get_Left(long* pRet) ;
	virtual BOOL get_Top(long* pRet) ;
	virtual BOOL get_Width(long* pRet) ;
	virtual BOOL get_Height(long* pRet) ;
	virtual BOOL get_LocationName(LPTSTR lpszBuffer, INT nBufferLen);
	virtual BOOL get_LocationURL(LPTSTR lpszBuffer, INT nBufferLen);
	virtual BOOL get_Busy();
	
public:
	BEGIN_MSG_MAP(CTsWebBrowserContainer)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()

    BEGIN_SINK_MAP(CTsWebBrowserContainer) //some common events, can add more if it is needed
		SINK_ENTRY(TS_WEB_BROWSER_WIN_ID, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2)
        SINK_ENTRY(TS_WEB_BROWSER_WIN_ID, DISPID_NAVIGATECOMPLETE2, OnNavigateComplete2)
        SINK_ENTRY(TS_WEB_BROWSER_WIN_ID, DISPID_STATUSTEXTCHANGE, OnStatusChange)
        SINK_ENTRY(TS_WEB_BROWSER_WIN_ID, DISPID_COMMANDSTATECHANGE, OnCommandStateChange)
        SINK_ENTRY(TS_WEB_BROWSER_WIN_ID, DISPID_DOWNLOADBEGIN, OnDownloadBegin)
        SINK_ENTRY(TS_WEB_BROWSER_WIN_ID, DISPID_DOWNLOADCOMPLETE, OnDownloadComplete)
		SINK_ENTRY(TS_WEB_BROWSER_WIN_ID, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
		SINK_ENTRY(TS_WEB_BROWSER_WIN_ID, DISPID_NAVIGATEERROR, OnNavigateError)
    END_SINK_MAP()

protected:
    virtual void __stdcall OnBeforeNavigate2 (
        IDispatch* pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName,
        VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel );

    virtual void __stdcall OnNavigateComplete2 ( IDispatch* pDisp, VARIANT* URL );
    virtual void __stdcall OnStatusChange ( BSTR bsText );
    virtual void __stdcall OnCommandStateChange ( long lCmd, VARIANT_BOOL vbEnabled );
    virtual	void __stdcall OnDownloadBegin();
    virtual void __stdcall OnDownloadComplete();
	virtual void __stdcall OnDocumentComplete(IDispatch * pDisp, VARIANT * URL);
	virtual void __stdcall OnNavigateError(IDispatch* pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* StatusCode, VARIANT_BOOL* Cancel);


	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled);

protected:
	 IWebBrowserEventSink* m_pWebEventSink;
	 CAxWindow m_AxHostWin;
	 CComQIPtr<IWebBrowser2> m_pWB2;	
};