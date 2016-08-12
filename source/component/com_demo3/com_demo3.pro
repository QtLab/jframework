#-------------------------------------------------
# This file is generated by the Qt Creator
#-------------------------------------------------

jframe_root = $$PWD/../../..

QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += \
    PRO_COMPONENT

include($${jframe_root}/source/common/build.pri)

TARGET = $$qtLibraryTarget(com_demo3)

# The .h file which was generated for your project. Feel free to hack it.
HEADERS += \
    $$PWD/com_demo3.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    $$PWD/com_demo3.cpp

#
exists($$PWD/com_demo3.xml) {
    OTHER_FILES += \
        $$PWD/com_demo3.xml
}

#-------------------------------------------------
# import libraries
#-------------------------------------------------

#-------------------------------------------------
# global commands
#-------------------------------------------------

win32|unix: {
    copyCommand = @echo off
    copyCommand += && @echo ---- console - com_demo3 ----

    ## copy files
    dstdir = $${jframe_root}/component/com_demo3
    win32:dstdir = $$replace(dstdir, /, \\)
    !exists("$$dstdir"):copyCommand += && $(MKDIR) "$$dstdir"
    srcfile = $$PWD/com_demo3.xml
    win32:srcfile = $$replace(srcfile, /, \\)
    exists("$$srcfile"):copyCommand += && $(COPY_FILE) "$$srcfile" "$$dstdir"

    deployment.commands = $$copyCommand
    first.depends = $(first) deployment
    QMAKE_EXTRA_TARGETS += first deployment
}
