#-------------------------------------------------
# This file is generated by the Qt Creator
#-------------------------------------------------

jframe_root = $$PWD/../../..

QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += \
    PRO_COMPONENT

include($${jframe_root}/source/common/build.pri)

TARGET = $$qtLibraryTarget(com_demo2)

# The .h file which was generated for your project. Feel free to hack it.
HEADERS += \
    $$PWD/com_demo2.h \
    $$PWD/testwidget1.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    $$PWD/com_demo2.cpp \
    $$PWD/testwidget1.cpp

#
exists($$PWD/com_demo2.xml) {
    OTHER_FILES += \
        $$PWD/com_demo2.xml
}

#-------------------------------------------------
# import libraries
#-------------------------------------------------

#-------------------------------------------------
# global commands
#-------------------------------------------------

win32|unix: {
    win32:copyCommand = @echo off
    unix:copyCommand = @echo
    copyCommand += && echo --- console - $$TARGET ---

    ## copy files
    dstdir = $${jframe_root}/component/com_demo2
    win32:dstdir = $$replace(dstdir, /, \\)
    !exists("$$dstdir"):copyCommand += && $(MKDIR) "$$dstdir"
    srcfile = $$PWD/com_demo2.xml
    win32:srcfile = $$replace(srcfile, /, \\)
    exists("$$srcfile"):copyCommand += && $(COPY_FILE) "$$srcfile" "$$dstdir"

    deployment.commands = $$copyCommand
    first.depends = $(first) deployment
    QMAKE_EXTRA_TARGETS += first deployment
}
