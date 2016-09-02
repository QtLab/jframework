#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T18:09:05
#
#-------------------------------------------------

TEMPLATE = aux
INSTALLER = setup

#-------------------------------------------------
jframe_dir = $$PWD/../..    #
win32:jframe_dir = $$replace(jframe_dir, /, \\)

#-------------------------------------------------
win32:RM_DIR = rd /s /q
unix:RM_DIR = rm -rf

#-------------------------------------------------
include($$PWD/../pack/packages/packages.pri)

#-------------------------------------------------
include($$PWD/clear.pri)

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD --- remove $$INSTALLER ----- &
dstdir = "$$jframe_dir/setup/pack/bin/"
exists("$$dsrdir") {
    win32:dstdir = $$replace(dstdir, /, \\)
    commands += $$RM_DIR "$$dstdir" &
}

INPUT = $$PWD/README
clear.input = INPUT
clear.output = clear.0
clear.commands = $$commands
clear.CONFIG += target_predeps no_link combine

#-------------------------------------------------

QMAKE_EXTRA_COMPILERS += clear
