#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T18:18:11
#
#-------------------------------------------------

QT -= gui

greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets

jframe_root = $$PWD/../../..

TEMPLATE = app
DESTDIR = $${jframe_root}/bin

##
win32:{
    lessThan(QT_MAJOR_VERSION, 5):{
        CONFIG(debug, debug|release):TARGET = applicationd
        else:TARGET = application
    } else {
        TARGET = $$qtLibraryTarget(application)
    }
} else {
    TARGET = $$qtLibraryTarget(application)
}

##
DEFINES += \
#    USE_JFRAME_FACADE

# The .h file which was generated for your project. Feel free to hck it.
HEADERS += \

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    main.cpp

# Installation path
# target.path =

OTHER_FILES += \

#######################################################
# Copyright Information
#######################################################

QMAKE_TARGET_COMPANY = "Smartsoft"
QMAKE_TARGET_DESCRIPTION = "jframework"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2016 Smartsoft Inc."

win32:RC_ICONS = $${jframe_root}/config/resource/app.ico

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

    CONFIG      += debug_and_release
    CONFIG      += build_all

} else {
    CONFIG      += release
}

linux-g++ {
#    CONFIG      += separate_debug_info
}

###############################################################
# import libraries
###############################################################

INCLUDEPATH += $${jframe_root}/include
DEPENDPATH += $${jframe_root}/include

###############################################################
# global commands
###############################################################

win32|unix: {
    copyCommand = @echo off
    copyCommand += && echo --- console - $$TARGET ---

    ## copy files

    ## remove files
    dstdir = $$DESTDIR/
    win32:dstdir = $$replace(dstdir, /, \\)
    exists("$${dstdir}$${TARGET}.exp"):copyCommand += && $(DEL_FILE) "$${dstdir}$${TARGET}.exp"
    exists("$${dstdir}$${TARGET}.ilk"):copyCommand += && $(DEL_FILE) "$${dstdir}$${TARGET}.ilk"
    exists("$${dstdir}$${TARGET}.lib"):copyCommand += && $(DEL_FILE) "$${dstdir}$${TARGET}.lib"
    #exists("$${dstdir}$${TARGET}.pdb"):copyCommand += && $(DEL_FILE) "$${dstdir}$${TARGET}.pdb"

    deployment.commands = $$copyCommand
    first.depends = $(first) deployment
    QMAKE_EXTRA_TARGETS += first deployment

    ##
    win32:CONFIG(debug, debug|release):debug_suffix=d
    else:win32:debug_suffix=

    ##
    win32:system($${jframe_root}\\syncjframe.cmd $$QT_MAJOR_VERSION $$debug_suffix)
}
