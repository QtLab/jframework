#-------------------------------------------------
# Project created by QtCreator 2016-07-18T18:18:11
#-------------------------------------------------

THIS_DIR = $$PWD/../../..

QT += gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets dbus

DEFINES += \
    PRO_CORE \
    JFRAME_FACTORY_LIB \
    JFRAME_FACTORY_BUILD \
    JFRAME_NO_PRECOMPILED \
    LOG4CPP_LIB

include($$THIS_DIR/source/common/build.pri)

TARGET = $$qtLibraryTarget(jframe_factory)

##
INCLUDEPATH += \
    $$PWD/private/slice

# The .h file which was generated for your project. Feel free to hack it.
HEADERS += \
    jframe_factory.h \
    private/jframe_factory_p.h \
    jlogmanager.h \
    jnotifier.h \
    private/jlogmanager_p.h \
    private/jnotifier_p.h \
    private/jdbusnotify.h \
    private/jicenotify.h \
#    private/slice/IceNotify.h \
#    private/slice/IceNotifyI.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    jframe_factory.cpp \
    private/jframe_factory_p.cpp \
    private/jlogmanager_p.cpp \
    private/jnotifier_p.cpp \
    private/jdbusnotify.cpp \
    private/jicenotify.cpp \
#    private/slice/IceNotify.cpp \
#    private/slice/IceNotifyI.cpp

OTHER_FILES += \
    private/slice/IceNotify.ice

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
    ##
    exists("$$(ICE_HOME)") {
        ##
        commands = echo --- console - $$TARGET --- &

        win32:commands += "\"%ICE_HOME%/bin/slice2cpp.exe\"" -I"\"%ICE_HOME%/slice\"" "\"$$PWD/private/slice/IceNotify.ice\"" --output-dir "\"$$PWD/private/slice"# --impl
        unix:commands += "slice2cpp" -I"$(ICE_HOME)/slice" "$$PWD/private/slice/IceNotify.ice" --output-dir "$$PWD/private/slice"
        QMAKE_PRE_LINK += $$commands
    }
    ##
    commands = echo --- console - $$TARGET --- &

    excludefile = $$PWD/copy.ignore
    !exists("$$excludefile"):excludefile = "$$THIS_DIR/source/common/copy.ignore"
    !exists("$$excludefile"):error("$$excludefile" is not exists!)
    win32:excludefile = $$replace(excludefile, /, \\)

    ## copy files
    dstdir = $$THIS_DIR/include/core/factory
    win32:dstdir = $$replace(dstdir, /, \\)
    !exists("$$dstdir"):commands += $(MKDIR) "\"$$dstdir\"" &
    win32:srcdir = $$PWD/*.h
    unix:srcdir = $$PWD/
    win32:srcdir = $$replace(srcdir, /, \\)
    win32:commands += $(COPY_DIR) "\"$$srcdir\"" "\"$$dstdir\"" /exclude:"$$excludefile" &
    unix:commands += "\"$$THIS_DIR/tools/xcopy.py\"" "\"$$srcdir\"" "\"$$dstdir\"" "*.h" &

    QMAKE_POST_LINK += $$commands
}

DISTFILES += \
    private/slice/icenotify.ice
