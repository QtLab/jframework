#-------------------------------------------------
# This file is generated by the Qt Creator
#-------------------------------------------------

jframe_root = $$PWD/../../..

QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += \
    PRO_COMPONENT

include($${jframe_root}/source/common/build.pri)

TARGET = $$qtLibraryTarget(com_demo1)

# The .h file which was generated for your project. Feel free to hack it.
HEADERS += \
    $$PWD/com_demo1.h \
    testwidget1.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    $$PWD/com_demo1.cpp \
    testwidget1.cpp

#
exists($$PWD/com_demo1.xml) {
    OTHER_FILES += \
        $$PWD/com_demo1.xml
}

#-------------------------------------------------
# import libraries
#-------------------------------------------------

#-------------------------------------------------
# global commands
#-------------------------------------------------

win32|unix: {
    commands += echo --- console - $$TARGET --- &

    ## copy files
    dstdir = $${jframe_root}/component/com_demo1/
    win32:dstdir = $$replace(dstdir, /, \\)
    !exists("$$dstdir"):commands += $(MKDIR) "\"$$dstdir\"" &
    srcfile = $$PWD/com_demo1.xml
    win32:srcfile = $$replace(srcfile, /, \\)
    exists("$$srcfile"):commands += $(COPY_FILE) "\"$$srcfile\"" "\"$$dstdir\"" &

    QMAKE_POST_LINK += $$commands
}
