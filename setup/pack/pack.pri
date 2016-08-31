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
commands += && echo : copy wizards packages

srcdir = "$$jframe_dir/tools/wizards/qtcreator/setup/packages"
exists("$$srcdir") {
    win32:srcdir = $$replace(srcdir, /, \\)
    dstdir = "$$PWD/packages/"
    win32:dstdir = $$replace(dstdir, /, \\)
    commands += && $(COPY_DIR) "$$srcdir" "$$dstdir"
}

#-------------------------------------------------
INPUT =
pack.input = INPUT
pack.output = pack
pack.commands = $$commands
pack.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += pack
