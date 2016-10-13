#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    # --- wizards - visualstudio
    srcdir = "$$jframe_dir/tools/wizards/visualstudio/qt5/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/qt5/"
        srcfiles = *
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        !exists("$$dstdir") {
            commands += $(MKDIR) "$$dstdir" &
        }
        for (srcfile, srcfiles) {
            commands += $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir" &
        }
    }
} else {
    dstdir = "$$PWD/data/qt5/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += $$RM_DIR "$$dstdir" &
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.wizards.visualstudio.input = INPUT
com.smartsoft.jframe.wizards.visualstudio.output = com.smartsoft.jframe.wizards.visualstudio
com.smartsoft.jframe.wizards.visualstudio.commands = $$commands
com.smartsoft.jframe.wizards.visualstudio.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.wizards.visualstudio
