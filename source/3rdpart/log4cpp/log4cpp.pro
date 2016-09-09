#-------------------------------------------------
# Project created by QtCreator 2016-07-18T18:18:11
#-------------------------------------------------

THIS_DIR = $$PWD/../../..

QT -= core gui
CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = lib

TARGET = $$qtLibraryTarget(log4cpp)

DESTDIR = $$THIS_DIR/lib/3rdpart

##
DEFINES += \
    PRO_3RDPART \
    LOG4CPP_LIB \
    LOG4CPP_BUILD

win32 {
    LIBS += -lws2_32 -ladvapi32
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

##
INCLUDEPATH += $$PWD/include

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
    commands += echo --- console - $$TARGET --- &

    excludefile = $$PWD/copy.ignore
    !exists("$$excludefile"):excludefile = "$$THIS_DIR/source/common/copy.ignore"
    !exists("$$excludefile"):error("$$excludefile" is not exists!)
    win32:excludefile = $$replace(excludefile, /, \\)

    ## copy files
    dstdir = $$THIS_DIR/include/3rdpart/log4cpp
    win32:dstdir = $$replace(dstdir, /, \\)
    !exists("$$dstdir"):commands += $(MKDIR) "\"$$dstdir\"" &
    win32:srcdir = $$PWD/include/log4cpp/*.*
    unix:srcdir = $$PWD/include/log4cpp/
    win32:srcdir = $$replace(srcdir, /, \\)
    win32:commands += $(COPY_DIR) "\"$$srcdir\"" "\"$$dstdir\"" /exclude:"$$excludefile" &
    unix:commands += "\"$$THIS_DIR/tools/xcopy.py\"" "\"$$srcdir\"" "\"$$dstdir\"" "*.h" &

    QMAKE_POST_LINK += $$commands
}
