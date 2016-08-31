#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off
unix:commands = @echo
commands += && echo ----- $$PWD

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    srcdir = "$$jframe_dir/tools/wizards/qtcreator/jframework/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/"
        srcfiles = *
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        !exists("$$dstdir") {
            commands += && $(MKDIR) "$$dstdir"
        }
        for (srcfile, srcfiles) {
            win32:commands += && $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir"
            unix:commands += && "$$jframe_dir/tools/xcopy.py" "$$srcdir" "$$dstdir" "$$srcfile"
        }
    }
} else {
    dstdir = "$$PWD/data/jframework/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += && $$RM_DIR "$$dstdir"
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.wizards.qtcreator.input = INPUT
com.smartsoft.jframe.wizards.qtcreator.output = com.smartsoft.jframe.wizards.qtcreator
com.smartsoft.jframe.wizards.qtcreator.commands = $$commands
com.smartsoft.jframe.wizards.qtcreator.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.wizards.qtcreator
