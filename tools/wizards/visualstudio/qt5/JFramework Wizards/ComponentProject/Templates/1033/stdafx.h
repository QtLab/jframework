// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "Resource.h"

#pragma  warning(disable : 4005)	// macro redefinition

// Qt
[!if CORE_MODULE]
#include <QtCore>
[!endif]
[!if GUI_MODULE]
#include <QtGui>
[!endif]
[!if WIDGETS_MODULE]
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
[!endif]

// jframe
#include "jframe_facade.h"
#include "factory/jnotifier.h"
#include "kernel/jframe_kernel.h"
#include "kernel/jframe_core.h"
[!if INCLUDE_DYNAMIC_LAYOUT]
#include "kernel/jframe_layout.h"
[!endif]
[!if INCLUDE_CHECK_LOGIN_USER]
#include "kernel/jframe_login.h"
[!endif]

[!if QTWINMIGRATE_MODULE]
// 3rdpart - qtwinmigrate
#include "3rdpart/qtwinmigrate/qmfcapp.h"
[!endif]

[!if BCG_MODULE]
// BCG
#include "BCGCBPro/BCGCBProInc.h"
[!endif]
