#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T18:18:11
#
#-------------------------------------------------

THIS_DIR = $$PWD/../../..

##
JFrameAppName = $$(JFRAME_APPNAME)

QT     -= core gui
CONFIG -= app_bundle
#CONFIG -= qt
#CONFIG += console
TEMPLATE = app

DESTDIR = $$THIS_DIR/bin

##
win32:{
    lessThan(QT_MAJOR_VERSION, 5):{
        CONFIG(debug, debug|release):TARGET = $${JFrameAppName}d
        else:TARGET = $${JFrameAppName}
    } else {
        TARGET = $$qtLibraryTarget($${JFrameAppName})
    }
} else {
    TARGET = $$qtLibraryTarget($${JFrameAppName})
}

## use JLibrary class
DEFINES += JLIBRARY_MODULE

##
unix {
    LIBS += -ldl
}

# The .h file which was generated for your project. Feel free to hck it.
HEADERS += \\

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \\
    main.cpp

OTHER_FILES += \\

#######################################################
# Copyright Information
#######################################################

QMAKE_TARGET_COMPANY = "Smartsoft"
QMAKE_TARGET_DESCRIPTION = "$${JFrameAppName}"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2016 Smartsoft Inc."

win32:RC_ICONS = $$THIS_DIR/config/$${JFrameAppName}/resource/app.ico

VER_MAJ = 1
VER_MIN = 0
VER_PAT = 0
VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

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
    win32:dstdir = $$replace(dstdir, /, \\\\)
    exists("$${dstdir}$${JFrameAppName}*.exp"):commands += $(DEL_FILE) "$${dstdir}$${JFrameAppName}*.exp" &
    exists("$${dstdir}$${JFrameAppName}*.ilk"):commands += $(DEL_FILE) "$${dstdir}$${JFrameAppName}*.ilk" &
    exists("$${dstdir}$${JFrameAppName}*.lib"):commands += $(DEL_FILE) "$${dstdir}$${JFrameAppName}*.lib" &

    #
    exists($$PWD/$${JFrameAppName}.pri):include($$PWD/$${JFrameAppName}.pri)

    QMAKE_POST_LINK += $$commands
}
