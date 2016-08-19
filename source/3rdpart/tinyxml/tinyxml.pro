#-------------------------------------------------
# Project created by QtCreator 2016-07-18T18:18:11
#-------------------------------------------------

jframe_root = $$PWD/../../..

QT -= core gui
CONFIG -= qt
CONFIG -= app_bundle
TEMPLATE = lib

DESTDIR = $${jframe_root}/lib/3rdpart

DEFINES += \
    PRO_3RDPART \
    TINYXML_LIB \
    TINYXML_BUILD

TARGET = $$qtLibraryTarget(tinyxml)

##################################################
# qmake internal options
##################################################

CONFIG += dll warn lib_bundle
CONFIG += no_kewords slient

##################################################
# release/debug mode
##################################################

win32 {
    # On Windows you can't mix release and debug libraries.
    # The designer is built in release mode. If you like to use it
    # you need a release version.For your own application development you
    # might need a debug version.
    # Enable debug_and_release + build_all if you want to build both.

    CONFIG      += debug_and_release
    CONFIG += build_all
#    CONFIG      += QT_NO_DEBUG_OUTPUT

} else {
    CONFIG      += release
}

linux-g++ {
#    CONFIG      += separate_debug_info
}

##
INCLUDEPATH += $$PWD/include
DEFINES -= UNICODE

##
include($$PWD/src.pri)

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
    dstdir = $${jframe_root}/include/3rdpart/tinyxml
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
