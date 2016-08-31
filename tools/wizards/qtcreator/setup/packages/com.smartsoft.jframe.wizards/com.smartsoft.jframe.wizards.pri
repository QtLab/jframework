#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off
unix:commands = @echo
commands += && echo ----- $$PWD

#-------------------------------------------------
contains(DEFINES, PACKAGE) {

} else {

}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.wizards.input = INPUT
com.smartsoft.jframe.wizards.output = com.smartsoft.jframe.wizards
com.smartsoft.jframe.wizards.commands = $$commands
com.smartsoft.jframe.wizards.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.wizards
