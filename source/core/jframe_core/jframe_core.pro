#-------------------------------------------------
# Project created by QtCreator 2016-07-18T18:18:11
#-------------------------------------------------

jframe_root = $$PWD/../../..

QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += \
    PRO_CORE \
#    USE_TINYXML

win32 {
    DEFINES += \
        JFRAME_CORE_DLL \
        JFRAME_CORE_MAKEDLL
}

include($${jframe_root}/source/common/build.pri)

TARGET = $$qtLibraryTarget(jframe_core)

# The .h file which was generated for your project. Feel free to hack it.
HEADERS += \
    jframe_core.h \
    private/jframe_core_p.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    jframe_core.cpp \
    private/jframe_core_p.cpp

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
    copyCommand = @echo off
    copyCommand += && echo --- console - $$TARGET ---

    excludefile = $$PWD/copy.ignore
    !exists("$$excludefile"):excludefile = $${jframe_root}/source/common/copy.ignore
    !exists("$$excludefile"):error("$$excludefile" is not exists!)
    win32:excludefile = $$replace(excludefile, /, \\)

    ## copy files
    dstdir = $${jframe_root}/include/jframe
    win32:dstdir = $$replace(dstdir, /, \\)
    !exists("$$dstdir"):copyCommand += && $(MKDIR) "$$dstdir"
    srcdir = $$PWD/*.h
    win32:srcdir = $$replace(srcdir, /, \\)
    copyCommand += && $(COPY_DIR) "$$srcdir" "$$dstdir" /exclude:"$$excludefile"

    deployment.commands = $$copyCommand
    first.depends = $(first) deployment
    QMAKE_EXTRA_TARGETS += first deployment
}
