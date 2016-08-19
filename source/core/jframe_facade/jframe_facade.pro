#-------------------------------------------------
# Project created by QtCreator 2016-07-18T18:18:11
#-------------------------------------------------

jframe_root = $$PWD/../../..

QT -= gui

DEFINES += \
    PRO_CORE \
    JFRAME_FACADE_LIB \
    JFRAME_FACADE_BUILD \
    JFRAME_NO_PRECOMPILED \
    NO_LOG4CPP_LIB

include($${jframe_root}/source/common/build.pri)

TARGET = $$qtLibraryTarget(jframe_facade)

# The .h file which was generated for your project. Feel free to hack it.
HEADERS += \
    jframe_facade.h \
    private/jframe_facade_p.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    jframe_facade.cpp \
    private/jframe_facade_p.cpp

## tinyxml
include($$PWD/../../3rdpart/tinyxml/src.pri)
INCLUDEPATH += $$PWD/../../3rdpart/tinyxml
DEPENDPATH += $$PWD/../../3rdpart/tinyxml

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
    dstdir = $${jframe_root}/include/jframe
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
