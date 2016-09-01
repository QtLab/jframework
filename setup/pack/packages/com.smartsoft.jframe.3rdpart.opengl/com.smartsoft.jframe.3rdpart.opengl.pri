#-------------------------------------------------
# Project created by openglCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off
unix:commands = @echo
commands += && echo ----- $$PWD

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    srcdir = "$$jframe_dir/bin/3rdpart/opengl/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/opengl/"
        srcfiles =
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles +=
        } else:unix {
            srcfiles +=
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
    dstdir = "$$PWD/data/opengl/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += && $$RM_DIR "$$dstdir"
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.3rdpart.opengl.input = INPUT
com.smartsoft.jframe.3rdpart.opengl.output = com.smartsoft.jframe.3rdpart.opengl
com.smartsoft.jframe.3rdpart.opengl.commands = $$commands
com.smartsoft.jframe.3rdpart.opengl.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.3rdpart.opengl
