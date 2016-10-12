################################################################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
################################################################

# qmake project file for building the qwt libraries

THIS_DIR = $$PWD/../../../..

QWT_ROOT = $${PWD}/..
include( $${QWT_ROOT}/qwtconfig.pri )
include( $${QWT_ROOT}/qwtbuild.pri )
include( $${QWT_ROOT}/qwtfunctions.pri )

QWT_OUT_ROOT = $${OUT_PWD}/..

TEMPLATE = lib

TARGET = $$qtLibraryTarget(qwt)

DESTDIR = $$THIS_DIR/lib/3rdpart

contains(QWT_CONFIG, QwtDll) {

    CONFIG += dll
    win32|symbian: DEFINES += QT_DLL QWT_DLL QWT_MAKEDLL
}
else {
    CONFIG += staticlib
} 

##
DEFINES += \
    QWT_LIB \
    QWT_BUILD

contains(QWT_CONFIG, QwtFramework) {

    CONFIG += lib_bundle
}

include ( $${PWD}/src.pri )

# Install directives

target.path    = $${QWT_INSTALL_LIBS}
INSTALLS       = target 

CONFIG(lib_bundle) {
    frame_header.version = Versions
    frame_header.files = $${HEADERS}
    frame_header.path = Headers
    QMAKE_BUNDLE_DATA += frame_header
}
else {

    headers.files  = $${HEADERS}
    headers.path   = $${QWT_INSTALL_HEADERS}
    INSTALLS += headers
}

################################################################
# creating a precompiled header file (only supported on some platforms (Windows - all MSVC project types,
# Mac OS X - Xcode, Makefile, Unix - gcc 3.3 and up)
################################################################

CONFIG += precompile_header
PRECOMPILED_HEADER = precomp.h
INCLUDEPATH += $$PWD

precompile_header:!isEmpty(PRECOMPILED_HEADER) {
    DEFINES += USING_PCH
}

contains(QWT_CONFIG, QwtPkgConfig) {

    CONFIG     += create_pc create_prl no_install_prl

    QMAKE_PKGCONFIG_NAME = Qwt$${QWT_VER_MAJ}
    QMAKE_PKGCONFIG_DESCRIPTION = Qt Widgets for Technical Applications

    QMAKE_PKGCONFIG_LIBDIR = $${QWT_INSTALL_LIBS}
    QMAKE_PKGCONFIG_INCDIR = $${QWT_INSTALL_HEADERS}

    QMAKE_PKGCONFIG_DESTDIR = pkgconfig

    greaterThan(QT_MAJOR_VERSION, 4) {

        QMAKE_PKGCONFIG_FILE = Qt$${QT_MAJOR_VERSION}$${QMAKE_PKGCONFIG_NAME}
        QMAKE_PKGCONFIG_REQUIRES = Qt5Widgets Qt5Concurrent Qt5PrintSupport

        contains(QWT_CONFIG, QwtSvg) {
            QMAKE_PKGCONFIG_REQUIRES += Qt5Svg
        }

        contains(QWT_CONFIG, QwtOpenGL) {
            QMAKE_PKGCONFIG_REQUIRES += Qt5OpenGL
        }

        QMAKE_DISTCLEAN += $${DESTDIR}/$${QMAKE_PKGCONFIG_DESTDIR}/$${QMAKE_PKGCONFIG_FILE}.pc
    }
    else {

        # there is no QMAKE_PKGCONFIG_FILE fo Qt4
        QMAKE_PKGCONFIG_REQUIRES = QtGui 

        contains(QWT_CONFIG, QwtSvg) {
            QMAKE_PKGCONFIG_REQUIRES += QtSvg
        }

        contains(QWT_CONFIG, QwtOpenGL) {
            QMAKE_PKGCONFIG_REQUIRES += QtOpenGL
        }

        QMAKE_DISTCLEAN += $${DESTDIR}/$${QMAKE_PKGCONFIG_DESTDIR}/$${TARGET}.pc
    }

    QMAKE_DISTCLEAN += $${DESTDIR}/libqwt.prl
}

## copy libraries to framework

exists($${THIS_DIR}) {
    qwt_headers.version = Versions
    qwt_headers.path = Headers
    qwt_headers.files = $$HEADERS

    CONFIG += qt_framework

    macx: CONFIG(qt_framework, qt_framework|qt_no_framework) {
        QMAKE_LFLAGS_SONAME = -Wl,-install_name,$$DESTDIR
        CONFIG += lib_bundle
        QMAKE_BUNDLE_DATA += \
            qwt_headers
    } else {
        commands = echo --- console - $$TARGET --- &

        excludefile = $$PWD/copy.ignore
        !exists("$$excludefile"):excludefile = "$$THIS_DIR/source/common/copy.ignore"
        !exists("$$excludefile"):error("$$excludefile" is not exists!)
        win32:excludefile = $$replace(excludefile, /, \\)

        ## copy files
        dstdir = $$THIS_DIR/include/3rdpart/qwt
        win32:dstdir = $$replace(dstdir, /, \\)
        !exists("$$dstdir"):commands += $(MKDIR) "\"$$dstdir\"" &
        win32:srcdir = $$PWD/*.h
        unix:srcdir = $$PWD/
        win32:srcdir = $$replace(srcdir, /, \\)
        win32:commands += $(COPY_DIR) "\"$$srcdir\"" "\"$$dstdir\"" /exclude:"$$excludefile" &
        unix:commands += "\"$$THIS_DIR/tools/xcopy.py\"" "\"$$srcdir\"" "\"$$dstdir\"" "*.h" &

        first.depends = $(first)
        # for qwt
        qwt_headers.commands = $$commands
        first.depends += qwt_headers

        QMAKE_EXTRA_TARGETS += first \
            qwt_headers
    }
}
