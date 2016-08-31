#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off
unix:commands = @echo
commands += && echo ----- $$PWD

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    srcdir = "$$jframe_dir/lib/core/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/bin/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += application*.exe
        } else:unix {
            srcfiles += application?
        }
        !exists("$$dstdir") {
            commands += && $(MKDIR) "$$dstdir"
        }
        for (srcfile, srcfiles) {
            srcfile = "$$srcdir$$srcfile"
            exists("$$srcfile") {
                commands += && $(COPY_FILE) "$$srcfile" "$$dstdir"
            }
        }
    }
} else {
    dstdir = "$$PWD/data/bin/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += && $$RM_DIR "$$dstdir"
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.example.input = INPUT
com.smartsoft.jframe.example.output = com.smartsoft.jframe.example
com.smartsoft.jframe.example.commands = $$commands
com.smartsoft.jframe.example.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.example
