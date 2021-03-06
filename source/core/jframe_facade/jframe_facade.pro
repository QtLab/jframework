#-------------------------------------------------
# Project created by QtCreator 2016-07-18T18:18:11
#-------------------------------------------------

THIS_DIR = $$PWD/../../..

QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += \
    PRO_CORE \
    JFRAME_FACADE_LIB \
    JFRAME_FACADE_BUILD \
    JFRAME_NO_PRECOMPILED \
    NO_LOG4CPP_LIB

include($$THIS_DIR/source/common/build.pri)

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
    commands += echo --- console - $$TARGET --- &

    excludefile = $$PWD/copy.ignore
    !exists("$$excludefile"):excludefile = "$$THIS_DIR/source/common/copy.ignore"
    !exists("$$excludefile"):error("$$excludefile" is not exists!)
    win32:excludefile = $$replace(excludefile, /, \\)

    ## copy files
    dstdir = $$THIS_DIR/include/core
    win32:dstdir = $$replace(dstdir, /, \\)
    !exists("$$dstdir"):commands += $(MKDIR) "\"$$dstdir\"" &
    win32:srcdir = $$PWD/*.h
    unix:srcdir = $$PWD/
    win32:srcdir = $$replace(srcdir, /, \\)
    win32:commands += $(COPY_DIR) "\"$$srcdir\"" "\"$$dstdir\"" /exclude:"$$excludefile" &
    unix:commands += "\"$$THIS_DIR/tools/xcopy.py\"" "\"$$srcdir\"" "\"$$dstdir\"" "*.h" &

    QMAKE_POST_LINK += $$commands
}
