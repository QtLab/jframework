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

#if defined(JFRAME_FACADE_LIB) && !defined(JFRAME_FACADE_BUILD)
#include "jframe_facade.h"
#endif

#if defined(JFRAME_FACTORY_LIB) && !defined(JFRAME_FACTORY_BUILD)
#include "factory/jframe_factory.h"
#include "factory/jnotifier.h"
#endif

#if defined(JFRAME_KERNEL_LIB) && !defined(JFRAME_KERNEL_BUILD)
#include "kernel/jframe_kernel.h"
#include "kernel/jframe_core.h"
#include "kernel/jframe_layout.h"
#include "kernel/jframe_login.h"
#endif

#if defined(TINYXML_LIB) && !defined(TINYXML_BUILD)
#include "tinyxml.h"
#endif

#if defined(QTWINMIGRATE_LIB) && !defined(QTWINMIGRATE_BUILD)
#include "qmfcapp.h"
#endif

#if defined(QTRIBBON_LIB) && !defined(QTRIBBON_BUILD)
#include "QtRibbon.h"
#endif

#if defined(LOG4CPP_LIB) && !defined(LOG4CPP_BUILD)
#include "log4cpp/convenience.h"
#endif

#if defined(JWT_LIB) && !defined(JWT_BUILD)
#include "jwt_inc.h"
#endif

//
#if QT_VERSION < 0x050000
#ifndef QStringLiteral
#define QStringLiteral(_str_) QString(_str_)
#endif
#endif

#endif // PRECOMP_H
