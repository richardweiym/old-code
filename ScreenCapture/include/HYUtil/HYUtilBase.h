#pragma  once 

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

#pragma  warning(disable:4530)
#pragma  warning(disable:4786)

#define WM_HY_PAINT WM_USER + 900 //WPARAM: DC handle

#define WM_HY_UI_DATA_CHANGED WM_USER + 1001 //WPARAM: UI message type

#ifndef WM_MOUSEENTER
   #define WM_MOUSEENTER WM_USER + 253
#endif // WM_MOUSEENTER