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
    DESTDIR = $${jframe_root}/lib/jframe

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
        DESTDIR = $${jframe_root}/component/jframe/$$TARGET
    } else {
        ##
        DESTDIR = $${jframe_root}/component/$$TARGET
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
    $${jframe_root}/include \
    $${jframe_root}/include/3rdpart \
    $${jframe_root}/include/jframe

DEPENDPATH += \
    $${jframe_root}/include \
    $${jframe_root}/include/3rdpart \
    $${jframe_root}/include/jframe

contains(DEFINES, JFRAME_FACADE_LIB):!contains(DEFINES, JFRAME_FACADE_BUILD) {
    ## import jframe_facade library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_facade
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_facaded
    else:unix:LIBS += -L$${jframe_root}/lib/jframe -ljframe_facade
}

contains(DEFINES, JFRAME_FACTORY_LIB):!contains(DEFINES, JFRAME_FACTORY_BUILD) {
    ## import jframe_factory library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_factory
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_factoryd
    else:unix:LIBS += -L$${jframe_root}/lib/jframe -ljframe_factory
    INCLUDEPATH += $${jframe_root}/include/jframe/factory
    DEPENDPATH += $${jframe_root}/include/jframe/factory
}

contains(DEFINES, JFRAME_KERNEL_LIB):!contains(DEFINES, JFRAME_KERNEL_BUILD) {
    ## import jframe_layout library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_kernel
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_kerneld
    else:unix:LIBS += -L$${jframe_root}/lib/jframe -ljframe_kernel
    INCLUDEPATH += $${jframe_root}/include/jframe/kernel
    DEPENDPATH += $${jframe_root}/include/jframe/kernel
}

contains(DEFINES, TINYXML_LIB):!contains(DEFINES, TINYXML_BUILD) {
    ## import tinyxml library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -ltinyxml
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -ltinyxmld
    else:unix:LIBS += -L$${jframe_root}/lib/3rdpart -ltinyxml
    INCLUDEPATH += $${jframe_root}/include/3rdpart/tinyxml
    DEPENDPATH += $${jframe_root}/include/3rdpart/tinyxml
}

contains(DEFINES, QTWINMIGRATE_LIB):!contains(DEFINES, QTWINMIGRATE_BUILD) {
    ## import qtwinmigrate library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -lqtwinmigrate
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -lqtwinmigrated
    else:unix:LIBS += -L$${jframe_root}/lib/3rdpart -lqtwinmigrate
    INCLUDEPATH += $${jframe_root}/include/3rdpart/qtwinmigrate
    DEPENDPATH += $${jframe_root}/include/3rdpart/qtwinmigrate
    win32:DEFINES += _AFXDLL
}

contains(DEFINES, QTRIBBON_LIB):!contains(DEFINES, QTRIBBON_BUILD) {
    ## import qtribbon library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -lqtribbon
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -lqtribbond
    else:unix:LIBS += -L$${jframe_root}/lib/3rdpart -lqtribbon
    INCLUDEPATH += $${jframe_root}/include/3rdpart/qtribbon
    DEPENDPATH += $${jframe_root}/include/3rdpart/qtribbon
}

contains(DEFINES, LOG4CPP_LIB):!contains(DEFINES, LOG4CPP_BUILD) {
    ## import log4cpp library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -llog4cpp
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -llog4cppd
    else:unix:LIBS += -L$${jframe_root}/lib/3rdpart -llog4cpp
    INCLUDEPATH += $${jframe_root}/include/3rdpart/log4cpp
    DEPENDPATH += $${jframe_root}/include/3rdpart/log4cpp
}

contains(DEFINES, JWT_LIB):!contains(DEFINES, JWT_BUILD) {
    ## import jwt library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -ljwt
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/3rdpart -ljwtd
    else:unix:LIBS += -L$${jframe_root}/lib/3rdpart -ljwt
    INCLUDEPATH += $${jframe_root}/include/3rdpart/jwt
    DEPENDPATH += $${jframe_root}/include/3rdpart/jwt
}

contains(DEFINES, BCG_LIB):!contains(DEFINES, BCG_BUILD) {
    ## import BCGCBPro library
    error(not supported!)
}
