#-------------------------------------------------
# Project created by QtCreator 2016-07-18T18:18:11
#-------------------------------------------------

jframe_root = $$PWD/../../..

QT += gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib

TARGET = $$qtLibraryTarget(jwt)

DESTDIR = $${jframe_root}/lib/3rdpart

##
DEFINES += \
    PRO_3RDPART \
    JWT_LIB \
    JWT_BUILD

win32 {
#    LIBS += -lws2_32 -ladvapi32
}

win32: {
    DEFINES -= UNICODE
}

##################################################
# qmake internal options
##################################################

CONFIG += dll warn_off lib_bundle
#CONFIG += no_kewords slient
#DEFINES += QT_NO_DEBUG_OUTPUT

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

##################################################
# creating a precompiled header file (only supported
# on some platforms (Windows - all MSVC project types,
# Mac OS X - Xcode, Makefile, Unix - gcc 3.3 and up)
##################################################

win32 {
    CONFIG += precompile_header
    PRECOMPILED_HEADER = $$PWD/precomp.h
    INCLUDEPATH += $$PWD
}

##
include($$PWD/src/src.pri)

###############################################################
# resource files
###############################################################

RESOURCES += \
    $$PWD/resource/jwt_resource.qrc

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
    dstdir = $${jframe_root}/include/3rdpart/jwt
    win32:dstdir = $$replace(dstdir, /, \\)
    !exists("$$dstdir"):copyCommand += && $(MKDIR) "$$dstdir"
    win32:srcdir = $$PWD/src/*.h
    unix:srcdir = $$PWD/src/
    win32:srcdir = $$replace(srcdir, /, \\)
    win32:copyCommand += && $(COPY_DIR) "$$srcdir" "$$dstdir" /exclude:"$$excludefile"
    unix:copyCommand += && "$${jframe_root}/tools/xcopy.py" "$${srcdir}" "$${dstdir}" "*.h"

    deployment.commands = $$copyCommand
    first.depends = $(first) deployment
    QMAKE_EXTRA_TARGETS += first deployment
}

HEADERS += \
    precomp.h
