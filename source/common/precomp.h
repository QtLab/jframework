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

#ifdef JFRAME_FACTORY_DLL
#include "factory/jframe_factory.h"
#include "factory/jnotifier.h"
#endif

#ifdef JFRAME_KERNEL_DLL
#include "kernel/jframe_kernel.h"
#include "kernel/jframe_core.h"
#include "kernel/jframe_layout.h"
#include "kernel/jframe_login.h"
#endif

#ifdef JFRAME_FACADE_DLL
#include "jframe_facade.h"
#endif

#ifdef USE_TINYXML
#include "tinyxml.h"
#endif

#ifdef USE_QTWINMIGRATE
#include "qmfcapp.h"
#endif

#ifdef USE_QTRIBBON
#include "QtRibbon.h"
#endif

#ifdef USE_LOG4CPP
#include "log4cpp/convenience.h"
#endif

#ifdef USE_JWT
#include "jwt_inc.h"
#endif

//
#if QT_VERSION < 0x050000
#ifndef QStringLiteral
#define QStringLiteral(_str_) QString(_str_)
#endif
#endif

#endif // PRECOMP_H
