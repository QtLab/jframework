#-------------------------------------------------
# Project created by QtCreator 2016-07-18T18:18:11
#-------------------------------------------------

jframe_root = $$PWD/../../..

QT += core gui xml concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += \
    PRO_CORE \
    JFRAME_KERNEL_LIB \
    JFRAME_KERNEL_BUILD \
    JFRAME_FACADE_LIB \
    JFRAME_FACTORY_LIB \
    QTRIBBON_LIB \
    QTWINMIGRATE_LIB \
    TINYXML_LIB \
    JWT_LIB

include($${jframe_root}/source/common/build.pri)

TARGET = $$qtLibraryTarget(jframe_kernel)

# The .h file which was generated for your project. Feel free to hack it.
HEADERS += \
    jframe_kernel.h \
    private/jframe_kernel_p.h \
    jframe_core.h \
    jframe_layout.h \
    jframe_login.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    jframe_kernel.cpp \
    private/jframe_kernel_p.cpp \
    jframe_core.cpp \
    jframe_layout.cpp \
    jframe_login.cpp

## submodule
include($$PWD/private/core/core.pri)
include($$PWD/private/layout/layout.pri)
include($$PWD/private/login/login.pri)

###############################################################
# resource files
###############################################################

RESOURCES +=

###############################################################
# import files
###############################################################

###############################################################
# global commands
###############################################################

win32|unix: {
    win32:copyCommand = @echo off
    unix:copyCommand = @echo
    copyCommand += && echo --- console - $$TARGET ---

    excludefile = $$PWD/copy.ignore
    !exists("$$excludefile"):excludefile = $${jframe_root}/source/common/copy.ignore
    !exists("$$excludefile"):error("$$excludefile" is not exists!)
    win32:excludefile = $$replace(excludefile, /, \\)

    ## copy files
    dstdir = $${jframe_root}/include/jframe/kernel
    win32:dstdir = $$replace(dstdir, /, \\)
    !exists("$$dstdir"):copyCommand += && $(MKDIR) "$$dstdir"
    win32:srcdir = $$PWD/*.h
    unix:srcdir = $$PWD/
    win32:srcdir = $$replace(srcdir, /, \\)
    win32:copyCommand += && $(COPY_DIR) "$$srcdir" "$$dstdir" /exclude:"$$excludefile"
    unix:copyCommand += && "$${jframe_root}/tools/xcopy.py" "$${srcdir}" "$${dstdir}" "*.h"

    deployment.commands = $$copyCommand
    first.depends = $(first) deployment
    QMAKE_EXTRA_TARGETS += first deployment
}
