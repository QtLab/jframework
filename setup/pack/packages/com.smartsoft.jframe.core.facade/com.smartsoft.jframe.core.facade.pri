#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    srcdir = "$$jframe_dir/lib/core/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/core/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += jframe_facade*.dll
        } else:unix {
            srcfiles += libjframe_facade.so*
        }
        !exists("$$dstdir") {
            commands += $(MKDIR) "$$dstdir" &
        }
        for (srcfile, srcfiles) {
            srcfile = "$$srcdir$$srcfile"
            exists("$$srcfile") {
                commands += $(COPY_FILE) "$$srcfile" "$$dstdir" &
            }
        }
    }
} else {
    dstdir = "$$PWD/data/core/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += $$RM_DIR "$$dstdir" &
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.core.facade.input = INPUT
com.smartsoft.jframe.core.facade.output = com.smartsoft.jframe.core.facade
com.smartsoft.jframe.core.facade.commands = $$commands
com.smartsoft.jframe.core.facade.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.core.facade
