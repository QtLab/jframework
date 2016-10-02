#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T18:18:11
#
#-------------------------------------------------

THIS_DIR = $$PWD/../../..

QT += gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = $$THIS_DIR/bin
TARGET = jfwconfiggui

DEFINES += \
    PRO_JWFCONFIGGUI \
    JWT_LIB \
    JFRAME_FACADE_LIB

include($$THIS_DIR/source/common/build.pri)
include($$PWD/src/src.pri)

##
RESOURCES += \
    resource/jfwconfiggui.qrc

##
OTHER_FILES += \

#######################################################
# Copyright Information
#######################################################

QMAKE_TARGET_COMPANY = "Smartsoft"
QMAKE_TARGET_DESCRIPTION = "jframework config"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2016 Smartsoft Inc."

win32:RC_ICONS = $$THIS_DIR/config/resource/app.ico

#VER_MAJ = 1
#VER_MIN = 0
#VER_PAT = 0
#VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

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

} else {
#    CONFIG      += release
}

linux-g++ {
#    CONFIG      += separate_debug_info
}

###############################################################
# import libraries
###############################################################

INCLUDEPATH += $$(JFRAME_DIR)/include

###############################################################
# global commands
###############################################################

win32|unix: {
    win32:commands = @echo off &
    unix:commands =
    commands += echo --- console - $$TARGET --- &

    ## copy files

    ## remove files
    dstdir = $$DESTDIR/
    win32:dstdir = $$replace(dstdir, /, \\)
    exists("$${dstdir}$$TARGET*.exp"):commands += $(DEL_FILE) "$${dstdir}$$TARGET*.exp" &
    exists("$${dstdir}$$TARGET*.ilk"):commands += $(DEL_FILE) "$${dstdir}$$TARGET*.ilk" &
    exists("$${dstdir}$$TARGET*.lib"):commands += $(DEL_FILE) "$${dstdir}$$TARGET*.lib" &

    QMAKE_POST_LINK += $$commands
}
