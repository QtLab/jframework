#-------------------------------------------------
# Project created by QtCreator 2016-07-18T18:18:11
#-------------------------------------------------

QT += core
TEMPLATE = lib

## fixed on Windowx XP
#win32:QMAKE_LFLAGS_WINDOW = /SUBSYSTEM:WINDOW,5.02

##################################################
# creating a precompiled header file (only supported
# on some platforms (Windows - all MSVC project types,
# Mac OS X - Xcode, Makefile, Unix - gcc 3.3 and up)
##################################################

CONFIG += precompile_header
PRECOMPILED_HEADER = $$PWD/precomp.h
INCLUDEPATH += $$PWD

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
    CONFIG      += release
}

linux-g++ {
#    CONFIG      += separate_debug_info
}

##################################################
# other files
##################################################

contains(DEFINES, PRO_CORE) {
    DESTDIR = $${jframe_root}/lib/jframe

    ##
    win32:CONFIG += build_all

    win32 {
        !contains(DEFINES, USE_NO_LOG4CPP) {
            DEFINES += \
                USE_LOG4CPP
        }
    }
}

contains(DEFINES, PRO_COMPONENT) {
    ##
    win32:CONFIG += build_all

    ##
    contains(DEFINES, JFRAME_COMPONENT) {
        ##
        DESTDIR = $${jframe_root}/component/jframe/$$TARGET
    } else {
        ##
        DESTDIR = $${jframe_root}/component/$$TARGET
    }

    !contains(DEFINES, USE_NO_JFRAME_LIBS) {
        DEFINES += \
            USE_JFRAME_FACTORY \
            USE_JFRAME_KERNEL \
            USE_JFRAME_FACADE
    }
}

##################################################
# import libraries
##################################################

INCLUDEPATH += \
    $${jframe_root}/include \
    $${jframe_root}/include/3rdpart \
    $${jframe_root}/include/jframe

DEPENDPATH += \
    $${jframe_root}/include \
    $${jframe_root}/include/3rdpart \
    $${jframe_root}/include/jframe

contains(DEFINES, USE_JFRAME_FACTORY) {
    ## import jframe_factory library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_factory
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_factoryd
    else:unix:LIBS += -L$${jframe_root}/lib/jframe -ljframe_factory
    INCLUDEPATH += $${jframe_root}/include/jframe/factory
    DEPENDPATH += $${jframe_root}/include/jframe/factory
    DEFINES += JFRAME_FACTORY_DLL
}

contains(DEFINES, USE_JFRAME_KERNEL) {
    ## import jframe_layout library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_kernel
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_kerneld
    else:unix:LIBS += -L$${jframe_root}/lib/jframe -ljframe_kernel
    INCLUDEPATH += $${jframe_root}/include/jframe/kernel
    DEPENDPATH += $${jframe_root}/include/jframe/kernel
    DEFINES += JFRAME_KERNEL_DLL
}

contains(DEFINES, USE_JFRAME_FACADE) {
    ## import jframe_facade library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_facade
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_facaded
    else:unix:LIBS += -L$${jframe_root}/lib/jframe -ljframe_facade
    DEFINES += JFRAME_FACADE_DLL
}

contains(DEFINES, USE_TINYXML) {
    ## import tinyxml library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -ltinyxml
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -ltinyxmld
    else:unix:LIBS += -L$${jframe_root}/lib/3rdpart -ltinyxml
    INCLUDEPATH += $${jframe_root}/include/3rdpart/tinyxml
    DEPENDPATH += $${jframe_root}/include/3rdpart/tinyxml
    DEFINES += TINYXML_DLL
}

contains(DEFINES, USE_QTWINMIGRATE) {
    ## import qtwinmigrate library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -lqtwinmigrate
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -lqtwinmigrated
    else:unix:LIBS += -L$${jframe_root}/lib/3rdpart -lqtwinmigrate
    INCLUDEPATH += $${jframe_root}/include/3rdpart/qtwinmigrate
    DEPENDPATH += $${jframe_root}/include/3rdpart/qtwinmigrate
    DEFINES += _AFXDLL
}

contains(DEFINES, USE_QTRIBBON) {
    ## import qtribbon library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -lqtribbon
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -lqtribbond
    else:unix:LIBS += -L$${jframe_root}/lib/3rdpart -lqtribbon
    INCLUDEPATH += $${jframe_root}/include/3rdpart/qtribbon
    DEPENDPATH += $${jframe_root}/include/3rdpart/qtribbon
    DEFINES +=
}

contains(DEFINES, USE_LOG4CPP) {
    ## import log4cpp library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -llog4cpp
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -llog4cppd
    else:unix:LIBS += -L$${jframe_root}/lib/3rdpart -llog4cpp
    INCLUDEPATH += $${jframe_root}/include/3rdpart/log4cpp
    DEPENDPATH += $${jframe_root}/include/3rdpart/log4cpp
    DEFINES += LOG4CPP_HAS_DLL
}

contains(DEFINES, USE_JWT) {
    ## import jwt library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -ljwt
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -ljwtd
    else:unix:LIBS += -L$${jframe_root}/lib/3rdpart -ljwt
    INCLUDEPATH += $${jframe_root}/include/3rdpart/jwt
    DEPENDPATH += $${jframe_root}/include/3rdpart/jwt
    DEFINES += JWT_DLL
}

contains(DEFINES, USE_BCG) {
    ## import BCGCBPro library
    error(not supported!)
}
