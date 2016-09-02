#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
contains(DEFINES, PACKAGE) {

} else {

}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.coms.jcom.input = INPUT
com.smartsoft.jframe.coms.jcom.output = com.smartsoft.jframe.coms.jcom
com.smartsoft.jframe.coms.jcom.commands = $$commands
com.smartsoft.jframe.coms.jcom.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.coms.jcom
