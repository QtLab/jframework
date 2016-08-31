#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T18:09:05
#
#-------------------------------------------------

TEMPLATE = aux
INSTALLER = setup

#-------------------------------------------------
jframe_dir = $$absolute_path($$PWD/../..)    #
win32:jframe_dir = $$replace(jframe_dir, /, \\)

DEFINES += PACKAGE          # pack/clear flag

#-------------------------------------------------
win32:RM_DIR = rd /s /q
unix:RM_DIR = $(DEL_DIR)

#-------------------------------------------------
include($$PWD/pack.pri)

#-------------------------------------------------
include($$PWD/packages/packages.pri)

#-------------------------------------------------
win32:commands = @echo off
unix:commands = @echo
commands += && echo ----- $$PWD --- generate $$INSTALLER -----

!exists("$$PWD/bin/") {
    dstdir = "$$PWD/bin/"
    win32:dstdir = $$replace(dstdir, /, \\)
    commands += && $(MKDIR) "$$dstdir"
}

#-------------------------------------------------
commands += && echo generating ${QMAKE_FILE_OUT} file...please wait...
commands += && binarycreator --offline-only -c $$PWD/config/config.xml -p \
    $$PWD/packages "$$PWD/bin/${QMAKE_FILE_OUT}"

#-------------------------------------------------
INPUT = $$PWD/config/config.xml $$PWD/packages
setup.input = INPUT
setup.output = $$INSTALLER
setup.commands = $$commands
setup.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += setup
