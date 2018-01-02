#include "TsWebContainer.h"

CTsWebBrowserContainer::CTsWebBrowserContainer(IWebBrowserEventSink* pEventSink /* = NULL */)
{
	m_pWebEventSink = pEventSink;
}

CTsWebBrowserContainer::~CTsWebBrowserContainer()
{

}

IWebBrowserEventSink* CTsWebBrowserContainer::SetWebEventSink(IWebBrowserEventSink* pSink)
{
	IWebBrowserEventSink* pOldSink = m_pWebEventSink;
	m_pWebEventSink = pSink;

	return pOldSink;
}

LRESULT CTsWebBrowserContainer::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	
	_ASSERTE(!m_AxHostWin.IsWindow());
	_ASSERTE(m_pWB2 == NULL);
	AtlAxWinInit();

    CComPtr<IUnknown> punkCtrl;
    m_AxHostWin.Create ( *this, rcDefault, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, TS_WEB_BROWSER_WIN_ID);
	_ASSERTE(m_AxHostWin.IsWindow());
    HRESULT hr =  m_AxHostWin.CreateControlEx ( L"{8856F961-340A-11D0-A96B-00C04FD705A2}", NULL, NULL, &punkCtrl);
    
	if(SUCCEEDED(hr))
	{
		m_pWB2 = punkCtrl;
		AtlAdviseSinkMap(this, TRUE);
	}

	return S_OK;
}

LRESULT CTsWebBrowserContainer::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;	

	if(m_AxHostWin.IsWindow())
	{
		AtlAdviseSinkMap(this, FALSE);
		if(m_pWB2 != NULL)
		{
			m_pWB2.Detach()->Release();
			_ASSERTE(m_pWB2 == NULL);
		}

		m_AxHostWin.DestroyWindow();
	}

	return S_OK;
}

LRESULT CTsWebBrowserContainer::OnSize(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	if(wParam != SIZE_MINIMIZED)
	{
		RECT rtClient = {0};
		GetClientRect(&rtClient);
		m_AxHostWin.MoveWindow(&rtClient, TRUE);
	}
	
	bHandled = FALSE;

	return S_OK;
}

BOOL CTsWebBrowserContainer::Navigate(LPCTSTR lpszURl)
{
	CComVariant v; 
	HRESULT hr(E_FAIL);
	
	_ASSERTE(m_AxHostWin.IsWindow());

    if ( m_pWB2 != NULL )
        hr = m_pWB2->Navigate ( CComBSTR(lpszURl), &v, &v, &v, &v );	

	return SUCCEEDED(hr);
}

BOOL CTsWebBrowserContainer::GoBack()
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->GoBack();
	}

	return SUCCEEDED(hr);
}
BOOL CTsWebBrowserContainer::GoForward()
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->GoForward();
	}

	return SUCCEEDED(hr);
}
BOOL CTsWebBrowserContainer::GoHome()
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->GoHome();
	}

	return SUCCEEDED(hr);
}
BOOL CTsWebBrowserContainer::GoSearch()
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->GoSearch();
	}

	return SUCCEEDED(hr);
}
BOOL CTsWebBrowserContainer::Refresh()
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->Refresh();
	}

	return SUCCEEDED(hr);
}
BOOL CTsWebBrowserContainer::Refresh2( INT nLevel)
{
	HRESULT hr(E_FAIL);
	CComVariant v((long)nLevel);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->Refresh2(&v);
	}

	return SUCCEEDED(hr);
}
BOOL CTsWebBrowserContainer::Stop()
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->Stop();
	}

	return SUCCEEDED(hr);
}
BOOL CTsWebBrowserContainer::get_Application(IDispatch** pRet)
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->get_Application(pRet);
	}

	return SUCCEEDED(hr);
}

BOOL CTsWebBrowserContainer::get_Parent(IDispatch** pRet)
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->get_Parent(pRet);
	}

	return SUCCEEDED(hr);
}

BOOL CTsWebBrowserContainer::get_Container(IDispatch** pRet)
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->get_Container(pRet);
	}

	return SUCCEEDED(hr);
}

BOOL CTsWebBrowserContainer::get_Document(IDispatch** pRet)
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->get_Document(pRet);
	}

	return SUCCEEDED(hr);
}

BOOL CTsWebBrowserContainer::get_TopLevelContainer()
{
	BOOL bRet(FALSE);
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		VARIANT_BOOL vb(VARIANT_FALSE);
		hr = m_pWB2->get_TopLevelContainer(&vb);
		if(SUCCEEDED(hr))
		{
			bRet = (vb == VARIANT_TRUE);
		}
	}
	
	return bRet;
}

BOOL CTsWebBrowserContainer::get_Type(LPTSTR lpszBuffer, INT nBufferLen)
{
	BOOL bRet(FALSE);
	if(m_pWB2 != NULL)
	{
		CComBSTR bstr;
		HRESULT hr = m_pWB2->get_Type(&bstr);
		if(SUCCEEDED(hr))
		{
			lstrcpyn(lpszBuffer, (LPTSTR)bstr.m_str, nBufferLen);
		}
	}

	return bRet;
}

BOOL CTsWebBrowserContainer::get_Left(long* pRet)
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->get_Left(pRet);
	}

	return SUCCEEDED(hr);
}

BOOL CTsWebBrowserContainer::get_Top(long* pRet)
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->get_Top(pRet);
	}

	return SUCCEEDED(hr);
}
BOOL CTsWebBrowserContainer::get_Width(long* pRet)
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->get_Width(pRet);
	}

	return SUCCEEDED(hr);	
}
BOOL CTsWebBrowserContainer::get_Height(long* pRet)
{
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		hr = m_pWB2->get_Height(pRet);
	}

	return SUCCEEDED(hr);	
}

BOOL CTsWebBrowserContainer::get_LocationName(LPTSTR lpszBuffer, INT nBufferLen)
{
	BOOL bRet(FALSE);
	if(m_pWB2 != NULL)
	{
		CComBSTR bstr;
		HRESULT hr = m_pWB2->get_LocationName(&bstr);
		if(SUCCEEDED(hr))
		{
			lstrcpyn(lpszBuffer, (LPTSTR)bstr.m_str, nBufferLen);
		}
	}

	return bRet;
}

BOOL CTsWebBrowserContainer::get_LocationURL(LPTSTR lpszBuffer, INT nBufferLen)
{
	BOOL bRet(FALSE);
	if(m_pWB2 != NULL)
	{
		CComBSTR bstr;
		HRESULT hr = m_pWB2->get_LocationURL(&bstr);
		if(SUCCEEDED(hr))
		{
			lstrcpyn(lpszBuffer, (LPTSTR)bstr.m_str, nBufferLen);
		}
	}

	return bRet;	
}

BOOL CTsWebBrowserContainer::get_Busy()
{
	BOOL bRet(FALSE);
	HRESULT hr(E_FAIL);
	if(m_pWB2 != NULL)
	{
		VARIANT_BOOL vb(VARIANT_FALSE);
		hr = m_pWB2->get_Busy(&vb);
		if(SUCCEEDED(hr))
		{
			bRet = (vb == VARIANT_TRUE);
		}
	}
	
	return bRet;
}

void CTsWebBrowserContainer::OnBeforeNavigate2 (
        IDispatch* pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName,
        VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel )
{
	_ASSERTE(m_pWebEventSink != NULL);
	if(m_pWebEventSink != NULL)
	{
		m_pWebEventSink->OnBeforeNavigate2(pDisp, URL, Flags, TargetFrameName, PostData, Headers, Cancel);
	}
}

void CTsWebBrowserContainer::OnNavigateComplete2 ( IDispatch* pDisp, VARIANT* URL )
{
	_ASSERTE(m_pWebEventSink != NULL);
	if(m_pWebEventSink != NULL)
	{
		m_pWebEventSink->OnNavigateComplete2(pDisp, URL);
	}
}
void CTsWebBrowserContainer::OnStatusChange ( BSTR bsText )
{
	_ASSERTE(m_pWebEventSink != NULL);
	if(m_pWebEventSink != NULL)
	{
		m_pWebEventSink->OnStatusChange(bsText);
	}
}
void CTsWebBrowserContainer::OnCommandStateChange ( long lCmd, VARIANT_BOOL vbEnabled )
{
	_ASSERTE(m_pWebEventSink != NULL);
	if(m_pWebEventSink != NULL)
	{
		m_pWebEventSink->OnCommandStateChange(lCmd, vbEnabled);
	}
}
void CTsWebBrowserContainer::OnDownloadBegin()
{
	_ASSERTE(m_pWebEventSink != NULL);
	if(m_pWebEventSink != NULL)
	{
		m_pWebEventSink->OnDownloadBegin( );
	}
}
void CTsWebBrowserContainer::OnDownloadComplete()
{
	_ASSERTE(m_pWebEventSink != NULL);
	if(m_pWebEventSink != NULL)
	{
		m_pWebEventSink->OnDownloadComplete( );
	}
}

void CTsWebBrowserContainer::OnDocumentComplete(IDispatch * pDisp, VARIANT * URL)
{
	_ASSERTE(m_pWebEventSink != NULL);
	if(m_pWebEventSink != NULL)
	{
		m_pWebEventSink->OnDocumentComplete( pDisp, URL );
	}
}

void CTsWebBrowserContainer::OnNavigateError(IDispatch* pDisp, VARIANT* URL
											 , VARIANT* Frame, VARIANT* StatusCode, VARIANT_BOOL* Cancel)
{
	_ASSERTE(m_pWebEventSink != NULL);
	if(m_pWebEventSink != NULL)
	{
		m_pWebEventSink->OnNavigateError( pDisp, URL, Frame, StatusCode, Cancel );
	}
}