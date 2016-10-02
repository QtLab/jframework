#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T18:09:05
#
#-------------------------------------------------

TEMPLATE = aux
INSTALLER = docs
CONFIG += console

#-------------------------------------------------
jframe_dir = $$PWD/../..    #
win32:jframe_dir = $$replace(jframe_dir, /, \\)

#-------------------------------------------------
win32:RM_DIR = rd /s /q
unix:RM_DIR = rm -rf

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD --- generate $$INSTALLER ----- &

#-------------------------------------------------
win32:DOXYGEN_DIR = "C:\\Program Files\\doxygen\\bin\\"
unix:DOXYGEN_DIR =
commands += echo generating $$INSTALLER file...please wait... &
commands += $${DOXYGEN_DIR}doxygen --verbose --offline-only -c "$$PWD/config/config.xml" -p \
    "$$PWD/packages" "$$PWD/bin/$$INSTALLER" &

#-------------------------------------------------
INPUT = $$PWD/*.*
setup.input = INPUT
setup.output = $$INSTALLER
setup.commands = $$commands
setup.CONFIG += target_predeps combine no_link
QMAKE_EXTRA_COMPILERS += setup
