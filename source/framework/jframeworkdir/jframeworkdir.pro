#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T18:18:11
#
#-------------------------------------------------

QT -= gui

jframe_root = $$PWD/../../..

TEMPLATE = lib
DESTDIR = $${jframe_root}/lib/jframe

##
TARGET = jframeworkdir

## use JLibrary class
DEFINES += JLIBRARY_MODULE

##
win32 {
    LIBS += -ladvapi32  # registry operation
}

# The .h file which was generated for your project. Feel free to hck it.
HEADERS +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    main.cpp

# tinyxml
include($${jframe_root}/source/3rdpart/tinyxml/src.pri)
INCLUDEPATH += $${jframe_root}/source/3rdpart/tinyxml

#
OTHER_FILES +=

#######################################################
# Copyright Information
#######################################################

QMAKE_TARGET_COMPANY = "Smartsoft"
QMAKE_TARGET_DESCRIPTION = "jframeworkdir"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2016 Smartsoft Inc."

win32:RC_ICONS = $${jframe_root}/config/resource/app.ico

#VER_MAJ = 1
#VER_MIN = 0
#VER_PAT = 0
#VERSION = 2.2.3

#######################################################
# qmake internal options
#######################################################

CONFIG      += warn_on
#CONFIG      += no_keywords
#CONFIG      += silent
#CONFIG      += QT_NO_DEBUG_OUTPUT

#######################################################
# release/debug mode
#######################################################

win32 {
    # On Windows you can't mix release and debug libraries.
    # The designer is built in release mode. If you like to use it
    # you need a release version.For your own application development you
    # might need a debug version.
    # Enable debug_and_release + build_all if you want to build both.

    #CONFIG      += debug_and_release
    #CONFIG      += build_all
    #CONFIG      += release
} else {
    #CONFIG      += release
}

linux-g++ {
#    CONFIG      += separate_debug_info
}

###############################################################
# import libraries
###############################################################

INCLUDEPATH += $${jframe_root}/include

###############################################################
# global commands
###############################################################

win32|unix: {
    win32:copyCommand = @echo off
    unix:copyCommand = @echo
    copyCommand += && echo --- console - $$TARGET ---

    ## copy files

    ## remove files
    dstdir = $$DESTDIR/
    win32:dstdir = $$replace(dstdir, /, \\)

    deployment.commands = $$copyCommand
    first.depends = $(first) deployment
    QMAKE_EXTRA_TARGETS += first deployment
}
