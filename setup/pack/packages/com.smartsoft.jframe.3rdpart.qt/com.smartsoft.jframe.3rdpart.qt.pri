#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    srcdir = "$$jframe_dir/bin/3rdpart/Qt/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/Qt/"
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles = *.dll
        } else:unix {
            srcfiles = *.so*
        }
        !exists("$$dstdir") {
            commands += $(MKDIR) "$$dstdir" &
        }
        for (srcfile, srcfiles) {
            win32:commands += $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir" &
            unix:commands += "$$jframe_dir/tools/xcopy.py" "$$srcdir" "$$dstdir" "$$srcfile" &
        }
    }
} else {
    dstdir = "$$PWD/data/Qt/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += $$RM_DIR "$$dstdir" &
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.3rdpart.qt.input = INPUT
com.smartsoft.jframe.3rdpart.qt.output = com.smartsoft.jframe.3rdpart.qt
com.smartsoft.jframe.3rdpart.qt.commands = $$commands
com.smartsoft.jframe.3rdpart.qt.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.3rdpart.qt
