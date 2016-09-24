#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T18:18:11
#
#-------------------------------------------------

THIS_DIR = $$PWD/../../..

QT     -= core gui
CONFIG -= app_bundle
#CONFIG -= qt
#CONFIG += console
TEMPLATE = app

DESTDIR = $$THIS_DIR/bin

##
win32:{
    lessThan(QT_MAJOR_VERSION, 5):{
        CONFIG(debug, debug|release):TARGET = [!output APP_NAME]d
        else:TARGET = [!output APP_NAME]
    } else {
        TARGET = $$qtLibraryTarget([!output APP_NAME])
    }
} else {
    TARGET = $$qtLibraryTarget([!output APP_NAME])
}

## use JLibrary class
DEFINES += JLIBRARY_MODULE

##
unix {
    LIBS += -ldl
}

# The .h file which was generated for your project. Feel free to hck it.
HEADERS += \

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    main.cpp

OTHER_FILES += \

#######################################################
# Copyright Information
#######################################################

QMAKE_TARGET_COMPANY = "Smartsoft"
QMAKE_TARGET_DESCRIPTION = "[!output PROJECT_NAME] Instance"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2016 Smartsoft Inc."

win32:RC_ICONS = $$THIS_DIR/config/resource/app.ico

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
    win32:dstdir = $$replace(dstdir, /, \\)
    exists("$${dstdir}[!output APP_NAME]*.exp"):commands += $(DEL_FILE) "$${dstdir}[!output APP_NAME]*.exp" &
    exists("$${dstdir}[!output APP_NAME]*.ilk"):commands += $(DEL_FILE) "$${dstdir}[!output APP_NAME]*.ilk" &
    exists("$${dstdir}[!output APP_NAME]*.lib"):commands += $(DEL_FILE) "$${dstdir}[!output APP_NAME]*.lib" &

    #
    exists($$PWD/[!output APP_NAME].pri):include($$PWD/[!output APP_NAME].pri)

    QMAKE_POST_LINK += $$commands
}
