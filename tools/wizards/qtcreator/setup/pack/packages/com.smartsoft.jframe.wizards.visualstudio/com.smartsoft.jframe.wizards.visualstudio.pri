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
com.smartsoft.jframe.wizards.visualstudio.input = INPUT
com.smartsoft.jframe.wizards.visualstudio.output = com.smartsoft.jframe.wizards.visualstudio
com.smartsoft.jframe.wizards.visualstudio.commands = $$commands
com.smartsoft.jframe.wizards.visualstudio.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.wizards.visualstudio
