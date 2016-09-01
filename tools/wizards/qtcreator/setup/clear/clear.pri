#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T18:09:05
#
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off
unix:commands =
commands += echo ----- $$PWD --- clear.pri &

#-------------------------------------------------
dstdir = "$$jframe_dir/setup/pack/packages/com.smartsoft.jframe.wizards/"
exists("$$dstdir") {
    win32:dstdir = $$replace(dstdir, /, \\)
    commands += $$RM_DIR "$$dstdir" &
}
dstdir = "$$jframe_dir/setup/pack/packages/com.smartsoft.jframe.wizards.qtcreator/"
exists("$$dstdir") {
    win32:dstdir = $$replace(dstdir, /, \\)
    commands += $$RM_DIR "$$dstdir" &
}
dstdir = "$$jframe_dir/setup/pack/packages/com.smartsoft.jframe.wizards.visualstudio/"
exists("$$dstdir") {
    win32:dstdir = $$replace(dstdir, /, \\)
    commands += $$RM_DIR "$$dstdir" &
}

#-------------------------------------------------
dstdir = "$$jframe_dir/setup/pack/bin/"
exists("$$dstdir") {
    win32:dstdir = $$replace(dstdir, /, \\)
    commands += $$RM_DIR "$$dstdir" &
}

#-------------------------------------------------
INPUT =
clear.wizards.input = INPUT
clear.wizards.output = clear.wizard
clear.wizards.commands = $$commands
clear.wizards.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += clear.wizards
