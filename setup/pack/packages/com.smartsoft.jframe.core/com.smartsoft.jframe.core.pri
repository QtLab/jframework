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
com.smartsoft.jframe.core.input = INPUT
com.smartsoft.jframe.core.output = com.smartsoft.jframe.core
com.smartsoft.jframe.core.commands = $$commands
com.smartsoft.jframe.core.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.core
