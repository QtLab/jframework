#-------------------------------------------------
# This file is generated by the Qt Creator
#-------------------------------------------------

this_dir = $$PWD/../../..

QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += \\
    PRO_COMPONENT

include($$this_dir/source/common/build.pri)

TARGET = $$qtLibraryTarget(%{ProjectName})

# The .h file which was generated for your project. Feel free to hack it.
HEADERS += \\
    $$PWD/%{HdrFileName} \\
    $$PWD/%{UiHdrFileName}

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \\
    $$PWD/%{SrcFileName} \\
    $$PWD/%{UiSrcFileName}

#
exists($$PWD/%{ProjectName}.xml) {
    OTHER_FILES += \\
        $$PWD/%{ProjectName}.xml
}

#-------------------------------------------------
# import libraries
#-------------------------------------------------

#-------------------------------------------------
# global commands
#-------------------------------------------------

win32|unix: {
    commands += echo --- console - %{ProjectName} --- &

    ## copy files
    dstdir = $$this_dir/component/%{ProjectName}/
    win32:dstdir = $$replace(dstdir, /, \\\\)
    !exists("$$dstdir"):commands += $(MKDIR) "\\"$$dstdir\\"" &
    srcfile = $$PWD/%{ProjectName}.xml
    win32:srcfile = $$replace(srcfile, /, \\\\)
    exists("$$srcfile"):commands += $(COPY_FILE) "\\"$$srcfile\\"" "\\"$$dstdir\\"" &

    QMAKE_POST_LINK += $$commands
}
