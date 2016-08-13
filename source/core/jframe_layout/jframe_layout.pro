#-------------------------------------------------
# Project created by QtCreator 2016-07-18T18:18:11
#-------------------------------------------------

jframe_root = $$PWD/../../..

QT += gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += \
    PRO_CORE \
    USE_JFRAME_CORE \
    USE_JFRAME_FACADE \
    USE_JFRAME_FACTORY \
    USE_JFRAME_LOGIN \
    USE_QTWINMIGRATE \
    USE_QTRIBBON \
    USE_JWT

win32 {
    DEFINES += \
        _AFXDLL \
        JFRAME_LAYOUT_DLL \
        JFRAME_LAYOUT_MAKEDLL
}

include($${jframe_root}/source/common/build.pri)

TARGET = $$qtLibraryTarget(jframe_layout)

# The .h file which was generated for your project. Feel free to hack it.
HEADERS += \
    jframe_layout.h \
    private/jframe_layout_p.h \
    private/mainview_manager/mainview_manager.h \
    private/layout_manager/layout_manager.h \
    private/module_manager/module_manager.h \
    private/notify_manager/notify_manager.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    jframe_layout.cpp \
    private/jframe_layout_p.cpp \
    private/mainview_manager/mainview_manager.cpp \
    private/layout_manager/layout_manager.cpp \
    private/module_manager/module_manager.cpp \
    private/notify_manager/notify_manager.cpp

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
