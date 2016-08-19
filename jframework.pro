#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T18:18:11
#
#-------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    source

include($$PWD/config/config.pri)

OTHER_FILES += \
    $$PWD/syncjframe.cmd \
    $$PWD/syncjframe.sh
