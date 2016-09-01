# ----------------------------------------------------
# This file is generated by the Qt Creator.
# ------------------------------------------------------

jframe_root = $$PWD/../../..

QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib

TARGET = $$qtLibraryTarget(qtribbon)

DESTDIR = $${jframe_root}/lib/3rdpart

##
DEFINES += \
    PRO_3RDPART \
    QTRIBBON_LIB \
    QTRIBBON_BUILD \
    QT_HAVE_MMX \
    QT_HAVE_3DNOW \
    QT_HAVE_SSE \
    QT_HAVE_MMXEXT \
    QT_HAVE_SSE2 \
    QT_THREAD_SUPPORT

win32: {
    DEFINES -= UNICODE
    LIBS += -lgdi32 -luser32 -lshell32
}

##################################################
# qmake internal options
##################################################

CONFIG += dll warn_on lib_bundle
#CONFIG += no_kewords slient
#DEFINES += QT_NO_DEBUG_OUTPUT

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
    CONFIG += build_all
#    CONFIG      += QT_NO_DEBUG_OUTPUT

} else {
    CONFIG      += release
}

linux-g++ {
#    CONFIG      += separate_debug_info
}

win32 {
    greaterThan(QT_MAJOR_VERSION, 4) {
        INCLUDEPATH += \
            $(QTDIR)/include/QtCore/$$QT_VERSION \
            $(QTDIR)/include/QtCore/$$QT_VERSION/QtCore \
            $(QTDIR)/include/QtGui/$$QT_VERSION/QtGui \
            $(QTDIR)/include/QtWidgets/$$QT_VERSION/QtWidgets
    }
}

# The .h file which was generated for your project. Feel free to hack it.
HEADERS += \
    QtnCommonStyle.h \
    QtnCommonStylePrivate.h \
    QtnOfficeDefines.h \
    QtnOfficeFrameHelper.h \
    QtnOfficePopupColorButton.h \
    QtnOfficePopupMenu.h \
    QtnOfficePopupWindow.h \
    QtnOfficeStyle.h \
    QtnPopupHelpers.h \
    QtnRibbonBackstageView.h \
    QtnRibbonBackstageViewPrivate.h \
    QtnRibbonBar.h \
    QtnRibbonBarPrivate.h \
    QtnRibbonButton.h \
    QtnRibbonGallery.h \
    QtnRibbonGalleryPrivate.h \
    QtnRibbonGroup.h \
    QtnRibbonGroupLayout.h \
    QtnRibbonGroupPrivate.h \
    QtnRibbonMainWindow.h \
    QtnRibbonPage.h \
    QtnRibbonPrivate.h \
    QtnRibbonQuickAccessBar.h \
    QtnRibbonSliderPane.h \
    QtnRibbonStatusBar.h \
    QtnRibbonStyle.h \
    QtnRibbonSystemPopupBar.h \
    QtnRibbonTabBar.h \
    QtnRibbonToolTip.h \
    QtnStyleHelpers.h \
    QtRibbon.h \
    QtRibbonDef.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    QtnCommonStyle.cpp \
    QtnOfficePopupColorButton.cpp \
    QtnOfficePopupMenu.cpp \
    QtnOfficePopupWindow.cpp \
    QtnOfficeStyle.cpp \
    QtnPopupHelpers.cpp \
    QtnRibbonBackstageView.cpp \
    QtnRibbonBar.cpp \
    QtnRibbonButton.cpp \
    QtnRibbonGallery.cpp \
    QtnRibbonGroup.cpp \
    QtnRibbonGroupLayout.cpp \
    QtnRibbonMainWindow.cpp \
    QtnRibbonPage.cpp \
    QtnRibbonPrivate.cpp \
    QtnRibbonQuickAccessBar.cpp \
    QtnRibbonSliderPane.cpp \
    QtnRibbonStatusBar.cpp \
    QtnRibbonStyle.cpp \
    QtnRibbonSystemPopupBar.cpp \
    QtnRibbonTabBar.cpp \
    QtnRibbonToolTip.cpp \
    QtnStyleHelpers.cpp

win32: {
    HEADERS += \
        QtnOfficeFrameHelper_win.h

    SOURCES += \
        QtnCommonStyle_win.cpp \
        QtnOfficeFrameHelper_win.cpp

} else: unix: {
    HEADERS +=

    SOURCES += \
        QtnCommonStyle_linux.cpp
}

################################################################
# resource files
################################################################

RESOURCES += \
    Resources/QtnRibbonResources.qrc

################################################################
# import libraries
################################################################

################################################################
# global commands
################################################################

win32|unix: {
    commands += echo --- console - $$TARGET --- &

    excludefile = $$PWD/copy.ignore
    !exists("$$excludefile"):excludefile = "$$jframe_root/source/Common/copy.ignore"
    !exists("$$excludefile"):error("$$excludefile" is not exist!)
    win32:excludefile = $$replace(excludefile, /, \\)

    ## copy files
    dstdir = $${jframe_root}/include/3rdpart/qtribbon
    win32:dstdir = $$replace(dstdir, /, \\)
    !exists("$${dstdir}"):commands += $(MKDIR) "\"$$dstdir\"" &
    win32:srcdir = $$PWD/*.h
    unix:srcdir = $$PWD/
    win32:srcfile = $$replace(srcfile, /, \\)
    win32:commands += $(COPY_DIR) "\"$$srcfile\"" "\"$$dstdir\"" /exclude:"$$excludefile" &
    unix:commands += "\"$$jframe_root/tools/xcopy.py\"" "\"$$srcdir\"" "\"$$dstdir\"" "*.h" &

    QMAKE_POST_LINK += $$commands
}
