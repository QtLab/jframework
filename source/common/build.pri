#-------------------------------------------------
# Project created by QtCreator 2016-07-18T18:18:11
#-------------------------------------------------

QT += core
TEMPLATE = lib

## fixed on Windowx XP
#win32:QMAKE_LFLAGS_WINDOW = /SUBSYSTEM:WINDOW,5.02

jframe_dir = $$(JFRAME_DIR)

##################################################
# creating a precompiled header file (only supported
# on some platforms (Windows - all MSVC project types,
# Mac OS X - Xcode, Makefile, Unix - gcc 3.3 and up)
##################################################

INCLUDEPATH += $$PWD

win32 {
    !contains(DEFINES, JFRAME_NO_PRECOMPILED) {
        CONFIG += precompile_header
        PRECOMPILED_HEADER = $$PWD/precomp.h
    }
}

##################################################
# qmake internal options
##################################################

CONFIG      += warn_on
#CONFIG      += no_kewords
#CONFIG      += slient

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
#    CONFIG      += QT_NO_DEBUG_OUTPUT

} else {
#    CONFIG      += release
}

linux-g++ {
#    CONFIG      += separate_debug_info
}

##################################################
# other files
##################################################

contains(DEFINES, PRO_CORE) {
    DESTDIR = $$this_dir/lib/core

    ##
    win32:CONFIG += build_all

    win32 {
        !contains(DEFINES, NO_LOG4CPP_LIB) {
            DEFINES += \
                LOG4CPP_LIB
        }
    }
}

contains(DEFINES, PRO_COMPONENT) {
    ##
    win32:CONFIG += build_all

    ##
    contains(DEFINES, JFRAME_COMPONENT) {
        ##
        DESTDIR = $$this_dir/component/jframe/$$TARGET
    } else {
        ##
        DESTDIR = $$this_dir/component/$$TARGET
    }

    !contains(DEFINES, NO_JFRAME_LIBS) {
        DEFINES += \
            JFRAME_FACADE_LIB \
            JFRAME_FACTORY_LIB \
            JFRAME_KERNEL_LIB
    }
}

##################################################
# import libraries
##################################################

INCLUDEPATH += \
    $$this_dir/include \
    $$this_dir/include/3rdpart \
    $$this_dir/include/core \
    $$jframe_dir/include \
    $$jframe_dir/include/3rdpart \
    $$jframe_dir/include/core

DEPENDPATH += \
    $$this_dir/include \
    $$this_dir/include/3rdpart \
    $$this_dir/include/core \
    $$jframe_dir/include \
    $$jframe_dir/include/3rdpart \
    $$jframe_dir/include/core

contains(DEFINES, JFRAME_FACADE_LIB):!contains(DEFINES, JFRAME_FACADE_BUILD) {
    ## import jframe_facade library
    win32:CONFIG(release, debug|release):LIBS += -L$$jframe_dir/lib/core -ljframe_facade
    else:win32:CONFIG(debug, debug|release):LIBS += -L$$jframe_dir/lib/core -ljframe_facaded
    else:unix:LIBS += -L$$jframe_dir/lib/core -ljframe_facade
}

contains(DEFINES, JFRAME_FACTORY_LIB):!contains(DEFINES, JFRAME_FACTORY_BUILD) {
    ## import jframe_factory library
    win32:CONFIG(release, debug|release):LIBS += -L$$jframe_dir/lib/core -ljframe_factory
    else:win32:CONFIG(debug, debug|release):LIBS += -L$$jframe_dir/lib/core -ljframe_factoryd
    else:unix:LIBS += -L$$jframe_dir/lib/core -ljframe_factory
    INCLUDEPATH += $$jframe_dir/include/core/factory
    DEPENDPATH += $$jframe_dir/include/core/factory
}

contains(DEFINES, JFRAME_KERNEL_LIB):!contains(DEFINES, JFRAME_KERNEL_BUILD) {
    ## import jframe_layout library
    win32:CONFIG(release, debug|release):LIBS += -L$$jframe_dir/lib/core -ljframe_kernel
    else:win32:CONFIG(debug, debug|release):LIBS += -L$$jframe_dir/lib/core -ljframe_kerneld
    else:unix:LIBS += -L$$jframe_dir/lib/core -ljframe_kernel
    INCLUDEPATH += $$jframe_dir/include/core/kernel
    DEPENDPATH += $$jframe_dir/include/core/kernel
}

contains(DEFINES, TINYXML_LIB):!contains(DEFINES, TINYXML_BUILD) {
    ## import tinyxml library
    win32:CONFIG(release, debug|release):LIBS += -L$$jframe_dir/lib/3rdpart -ltinyxml
    else:win32:CONFIG(debug, debug|release):LIBS += -L$$jframe_dir/lib/3rdpart -ltinyxmld
    else:unix:LIBS += -L$$jframe_dir/lib/3rdpart -ltinyxml
    INCLUDEPATH += $$jframe_dir/include/3rdpart/tinyxml
    DEPENDPATH += $$jframe_dir/include/3rdpart/tinyxml
}

contains(DEFINES, QTWINMIGRATE_LIB):!contains(DEFINES, QTWINMIGRATE_BUILD) {
    ## import qtwinmigrate library
    win32:CONFIG(release, debug|release):LIBS += -L$$jframe_dir/lib/3rdpart -lqtwinmigrate
    else:win32:CONFIG(debug, debug|release):LIBS += -L$$jframe_dir/lib/3rdpart -lqtwinmigrated
    else:unix:LIBS += -L$$jframe_dir/lib/3rdpart -lqtwinmigrate
    INCLUDEPATH += $$jframe_dir/include/3rdpart/qtwinmigrate
    DEPENDPATH += $$jframe_dir/include/3rdpart/qtwinmigrate
    win32:DEFINES += _AFXDLL
}

contains(DEFINES, QTRIBBON_LIB):!contains(DEFINES, QTRIBBON_BUILD) {
    ## import qtribbon library
    win32:CONFIG(release, debug|release):LIBS += -L$$jframe_dir/lib/3rdpart -lqtribbon
    else:win32:CONFIG(debug, debug|release):LIBS += -L$$jframe_dir/lib/3rdpart -lqtribbond
    else:unix:LIBS += -L$$jframe_dir/lib/3rdpart -lqtribbon
    INCLUDEPATH += $$jframe_dir/include/3rdpart/qtribbon
    DEPENDPATH += $$jframe_dir/include/3rdpart/qtribbon
}

contains(DEFINES, LOG4CPP_LIB):!contains(DEFINES, LOG4CPP_BUILD) {
    ## import log4cpp library
    win32:CONFIG(release, debug|release):LIBS += -L$$jframe_dir/lib/3rdpart -llog4cpp
    else:win32:CONFIG(debug, debug|release):LIBS += -L$$jframe_dir/lib/3rdpart -llog4cppd
    else:unix:LIBS += -L$$jframe_dir/lib/3rdpart -llog4cpp
    INCLUDEPATH += $$jframe_dir/include/3rdpart/log4cpp
    DEPENDPATH += $$jframe_dir/include/3rdpart/log4cpp
}

contains(DEFINES, JWT_LIB):!contains(DEFINES, JWT_BUILD) {
    ## import jwt library
    win32:CONFIG(release, debug|release):LIBS += -L$$jframe_dir/lib/3rdpart -ljwt
    else:win32:CONFIG(debug, debug|release):LIBS += -L$$jframe_dir/lib/3rdpart -ljwtd
    else:unix:LIBS += -L$$jframe_dir/lib/3rdpart -ljwt
    INCLUDEPATH += $$jframe_dir/include/3rdpart/jwt
    DEPENDPATH += $$jframe_dir/include/3rdpart/jwt
}

contains(DEFINES, BCG_LIB):!contains(DEFINES, BCG_BUILD) {
    ## import BCGCBPro library
    error(not supported!)
}
