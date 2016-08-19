/****************************************************************************
** 
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
** 
** This file is part of a Qt Solutions component.
**
** Commercial Usage  
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage 
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
** 
** If you are unsure which license is appropriate for your use, please
** contact Nokia at qt-info@nokia.com.
** 
****************************************************************************/


// Declaration of the QWinHost classes

#ifndef QWINHOST_H
#define QWINHOST_H

#include <qglobal.h>

#if QT_VERSION < 0x050000
#include <QWidget>
#else
#include <QtWidgets/QWidget>
#endif

#ifdef QTWINMIGRATE_LIB
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__MINGW32__)
#   ifdef QTWINMIGRATE_BUILD
#       define QT_QTWINMIGRATE_EXPORT __declspec(dllexport)
#   else
#       define QT_QTWINMIGRATE_EXPORT __declspec(dllimport)
#       if defined(DEBUG) || defined(_DEBUG)
#           pragma comment(lib, "qtwinmigrated.lib")
#       else
#           pragma comment(lib, "qtwinmigrate.lib")
#       endif
#   endif // !QTWINMIGRATE_BUILD
#endif // _MSC_VER || ...
#endif // QTWINMIGRATE_LIB

#ifndef QT_QTWINMIGRATE_EXPORT
#define QT_QTWINMIGRATE_EXPORT
#endif

class QT_QTWINMIGRATE_EXPORT QWinHost : public QWidget
{
    Q_OBJECT
public:
    QWinHost(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~QWinHost();

    void setWindow(HWND);
    HWND window() const;

protected:
    virtual HWND createWindow(HWND parent, HINSTANCE instance);

    bool event(QEvent *e);
    void showEvent(QShowEvent *);
    void focusInEvent(QFocusEvent*);
    void resizeEvent(QResizeEvent*);

#if QT_VERSION < 0x050000
    bool winEvent(MSG *msg, long *result);
#else
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif

private:
    void fixParent();
    friend void* getWindowProc(QWinHost*);

    void *wndproc;
    bool own_hwnd;
    HWND hwnd;
};

#endif // QWINHOST_H
