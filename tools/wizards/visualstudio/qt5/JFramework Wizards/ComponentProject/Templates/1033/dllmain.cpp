// dllmain.cpp : Defines the initialization routines for the DLL.
//

[!if INCLUDE_PRECOMPILED]
#include "stdafx.h"
[!endif]
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE _AfxExtModule = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		#if defined(_DEBUG)
		TRACE0("[!output COMPONENT_NAME]d.dll Initializing!\n");
		#else
		TRACE0("[!output COMPONENT_NAME].dll Initializing!\n");
		#endif
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(_AfxExtModule, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(_AfxExtModule);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		#if defined(_DEBUG)
		TRACE0("[!output COMPONENT_NAME]d.dll Terminating!\n");
		#else
		TRACE0("[!output COMPONENT_NAME].dll Terminating!\n");
		#endif

		// Terminate the library before destructors are called
		AfxTermExtensionModule(_AfxExtModule);
	}
	return 1;   // ok
}
