#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T18:18:11
#
#-------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    docs \
    source \
    setup \
    tools/wizards

include($$PWD/config/config.pri)

win32 {
    OTHER_FILES += \
        $$PWD/syncjframe.cmd
} else:unix:{
    OTHER_FILES += \
        $$PWD/syncjframe.sh
}
