#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T18:09:05
#
#-------------------------------------------------

TEMPLATE = aux
INSTALLER = setup
CONFIG += console ordered

#-------------------------------------------------
jframe_dir = $$PWD/../..    #
win32:jframe_dir = $$replace(jframe_dir, /, \\)

DEFINES += PACKAGE          # pack/clear flag

#-------------------------------------------------
win32:RM_DIR = rd /s /q
unix:RM_DIR = rm -rf

#-------------------------------------------------
include($$PWD/pack.pri)

#-------------------------------------------------
include($$PWD/packages/packages.pri)

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD --- generate $$INSTALLER ----- &

!exists("$$PWD/bin/") {
    dstdir = "$$PWD/bin/"
    win32:dstdir = $$replace(dstdir, /, \\)
    commands += $(MKDIR) "$$dstdir" &
}

#-------------------------------------------------
win32:QTIFW_CREATOR = $$(QTIFW_DIR)\\bin\\binarycreator.exe
unix:QTIFW_CREATOR = /opt/Qt/QtIFW2.0.3/bin/binarycreator
commands += echo generating $$INSTALLER file...please wait... &
commands += "$$QTIFW_CREATOR" --verbose --offline-only -c "$$PWD/config/config.xml" -p \
    "$$PWD/packages" "$$PWD/bin/$$INSTALLER" &

#-------------------------------------------------
INPUT = $$PWD/config/config.xml $$PWD/packages
setup.input = INPUT
setup.output = $$INSTALLER
setup.commands = $$commands
setup.CONFIG += target_predeps combine no_link
QMAKE_EXTRA_COMPILERS += setup
