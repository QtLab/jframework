#ifndef PRECOMP_H
#define PRECOMP_H

#ifdef _AFXDLL
#include <afxwin.h>
#endif

#ifdef WIN32
#include <Windows.h>
#endif

#ifdef QT_CORE_LIB
#include <QtCore>
#endif

#ifdef QT_GUI_LIB
#include <QtGui>
#endif

#ifdef QT_SQL_LIB
#include <QtSql>
#endif

#ifdef QT_XML_LIB
#include <QtXml>
#endif

#if (QT_VERSION < 0x050000)

#include <QtDeclarative/QtDeclarative>

#else

#ifdef QT_WIDGETS_LIB
#include <QtWidgets>
#endif

#ifdef QT_WINEXTRAS_LIB
#include <QtWinExtras>
#endif

#ifdef QT_QUICK_LIB
#include <QtQuick>
#endif

#endif

#endif // PRECOMP_H
