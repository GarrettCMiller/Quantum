// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0500		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0500	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0500 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0601	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some String constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#ifdef _UNICODE
	#if defined _M_IX86
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#elif defined _M_IA64
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#elif defined _M_X64
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#else
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#endif
//#endif

#include "GLib.h"

#define DialogParentClass	CPropertyPage /*CDialog*/

#include "ParticleEmitter.h"
#include "ProfileSample.h"

#include "Logger.h"
#include "OutputDlg.h"

#include "Terrain.h"

#include <afxdlgs.h>

#define g_Output COutputDlg::m_Logger

//extern const float	g_fFloatPrecision;
//extern const float	g_fInvFloatPrecision;

#define SetFloatRange(min, max)		SetRange(min * g_fFloatPrecision, max * g_fFloatPrecision, true)
#define SetFloatPos(pos)			SetPos(pos * g_fFloatPrecision)

#define OutputResult(exp)			if (exp)g_Output << "Success!\n";\
									else	g_Output << "Failed!\n";

#define THREAD_SAFE(call)			if (g_pCurrentEmitter->m_bActive) {						\
									g_pCurrentEmitter->m_csCriticalSection.Lock();	\
									call;									\
									g_pCurrentEmitter->m_csCriticalSection.Unlock(); 		\
									} else call;

typedef CDynamicArray<CParticleEmitter*>	EmitterList;

extern Float	fCamDist	/*= 10.0f*/,
				fCamDir		/*= 0.0f*/,
				fCamAlt		/*= 250.0f*/;

extern Terrain grid;

extern CParticleEmitter* g_pRootEmitter;
extern CParticleEmitter* g_pCurrentEmitter;
extern CParticleEmitter* g_pDefaultEmitter;

static const Bool g_bVoxel = false;