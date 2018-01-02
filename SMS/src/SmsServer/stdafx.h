// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__80CCE58E_DBB0_4E33_84D6_1C0560EAAE93__INCLUDED_)
#define AFX_STDAFX_H__80CCE58E_DBB0_4E33_84D6_1C0560EAAE93__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0400
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#pragma comment(lib, "Ws2_32.lib")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__80CCE58E_DBB0_4E33_84D6_1C0560EAAE93__INCLUDED_)
