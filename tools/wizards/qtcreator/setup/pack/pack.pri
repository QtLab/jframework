#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T18:09:05
#
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off
unix:commands = @echo
commands += && echo ----- $$PWD

#-------------------------------------------------
INPUT =
pack.input = INPUT
pack.output = pack
pack.commands = $$commands
pack.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += pack
