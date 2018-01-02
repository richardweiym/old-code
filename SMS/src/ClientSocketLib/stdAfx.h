// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__84C25E8F_0BED_4360_8740_1FF140AAFAB8__INCLUDED_)
#define AFX_STDAFX_H__84C25E8F_0BED_4360_8740_1FF140AAFAB8__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0400
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlmisc.h>

#pragma comment(lib,"Ws2_32.lib")

#pragma warning(disable: 4146)
#pragma warning(disable: 4003)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__84C25E8F_0BED_4360_8740_1FF140AAFAB8__INCLUDED_)
