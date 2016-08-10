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
        !contains(DEFINES, JFRAME_FACADE_MAKEDLL) {
            DEFINES += \
                USE_LOG4CPP
        }
    }
}

contains(DEFINES, PRO_COMPONENT) {
    ##
    contains(DEFINES, JFRAME_COMPONENT) {
        DESTDIR = $${jframe_root}/component/jframe/$$TARGET
    } else {
        DESTDIR = $${jframe_root}/component/$$TARGET
    }

    !contains(DEFINES, USE_NO_JFRAME_LIBS) {
        win32 {
            DEFINES += \
                USE_JFAME_KERNEL \
                USE_JFRAME_FACTORY \
                USE_JFRAME_LAYOUT \
                USE_JFRAME_FACADE \
                USE_JFRAME_LOGIN
        }
    }
}

##################################################
# import libraries
##################################################

INCLUDEPATH += \
    $${jframe_root}/include \
    $${jframe_root}/include/jframe

DEPENDPATH += \
    $${jframe_root}/include \
    $${jframe_root}/include/jframe

contains(DEFINES, USE_JFRAME_KERNEL) {
    ## import jframe_layout library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_kernel
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_kerneld
    else:unix:LIBS += -L$${jframe_root}/lib/jframe -ljframe_kernel
    INCLUDEPATH += $${jframe_root}/include/jframe/kernel
    DEPENDPATH += $${jframe_root}/include/jframe/kernel
    DEFINES += \
        JFRAME_KERNEL_DLL
}

contains(DEFINES, USE_JFRAME_FACTORY) {
    ## import jframe_factory library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_factory
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_factoryd
    else:unix:LIBS += -L$${jframe_root}/lib/jframe -ljframe_factory
    INCLUDEPATH += $${jframe_root}/include/jframe/factory
    DEPENDPATH += $${jframe_root}/include/jframe/factory
    DEFINES += \
        JFRAME_ FACTORY_DLL
}

contains(DEFINES, USE_JFRAME_FACADE) {
    ## import jframe_facade library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_facade
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_facaded
    else:unix:LIBS += -L$${jframe_root}/lib/jframe -ljframe_facade
    DEFINES += \
        JFRAME_FACADE_DLL
}

contains(DEFINES, USE_JFRAME_LAYOUT) {
    ## import jframe_layout library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_layout
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_layoutd
    else:unix:LIBS += -L$${jframe_root}/lib/jframe -ljframe_layout
    DEFINES += \
        JFRAME_LAYOUT_DLL
}

contains(DEFINES, USE_JFRAME_LOGIN) {
    ## import jframe_login library
    win32:CONFIG(release, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_login
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${jframe_root}/lib/jframe -ljframe_logind
    else:unix:LIBS += -L$${jframe_root}/lib/jframe -ljframe_login
    DEFINES += \
        JFRAME_LOGIN_DLL
}

contains(DEFINES, USE_TINYXML) {
    ## import tinyxml library
    win32:CONFIG(release, debug|release):LIBS += -L$${SMARTKITS_HOME}/lib/tinyxml/lib -ltinyxml
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${SMARTKITS_HOME}/lib/tinyxml/lib -ltinyxmld
    else:unix:LIBS += -L$${SMARTKITS_HOME}/lib/tinyxml/lib -ltinyxml
    INCLUDEPATH += $${SMARTKITS_HOME}/lib/tinyxml/include
    DEPENDPATH += $${SMARTKITS_HOME}/lib/tinyxml/include
    DEFINES += \
        TINYXML_DLL
}

contains(DEFINES, USE_QTMFC) {
    ## import qtmfcmigration library
    win32:CONFIG(release, debug|release):LIBS += -L$${SMARTKITS_HOME}/lib/QtMFCMigration/lib -lQtMFCMigration
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${SMARTKITS_HOME}/lib/QtMFCMigration/lib -lQtMFCMigrationd
    else:unix:LIBS += -L$${SMARTKITS_HOME}/lib/QtMFCMigration -QtMFCMigration
    INCLUDEPATH += $${SMARTKITS_HOME}/lib/QtMFCMigration/include
    DEPENDPATH += $${SMARTKITS_HOME}/lib/QtMFCMigration/include
}

contains(DEFINES, USE_QTRIBBON) {
    ## import QtRibbon library
    win32:CONFIG(release, debug|release):LIBS += -L$${SMARTKITS_HOME}/lib/QtRibbon/lib -lQtRibbon
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${SMARTKITS_HOME}/lib/QtRibbon/lib -lQtRibbond
    else:unix:LIBS += -L$${SMARTKITS_HOME}/lib/QtRibbon/lib -lQtRibbon
    INCLUDEPATH += $${SMARTKITS_HOME}/lib/QtRibbon/include
    DEPENDPATH += $${SMARTKITS_HOME}/lib/QtRibbon/include
}


contains(DEFINES, USE_LOG4CPP) {
    ## import log4cpp library
    win32:CONFIG(release, debug|release):LIBS += -L$${SMARTKITS_HOME}/lib/log4cpp/lib -llog4cpp
    else:win32:CONFIG(debug, debug|release):LIBS += -L$${SMARTKITS_HOME}/lib/log4cpp/lib -llog4cppd
    else:unix:LIBS += -L$${SMARTKITS_HOME}/lib/log4cpp/lib -llog4cpp
    INCLUDEPATH += $${SMARTKITS_HOME}/lib/log4cpp/include
    DEPENDPATH += $${SMARTKITS_HOME}/lib/log4cpp/include
}
