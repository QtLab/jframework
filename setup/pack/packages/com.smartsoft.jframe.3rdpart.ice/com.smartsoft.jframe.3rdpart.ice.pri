#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    srcdir = "$$(ICE_HOME)/bin/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/ice"
        srcfiles =
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += bzip2*.dll ice36*.dll iceutil36*.dll
        } else:unix {
            srcfiles +=
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
    dstdir = "$$PWD/data/ice/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += $$RM_DIR "$$dstdir" &
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.3rdpart.ice.input = INPUT
com.smartsoft.jframe.3rdpart.ice.output = com.smartsoft.jframe.3rdpart.ice
com.smartsoft.jframe.3rdpart.ice.commands = $$commands
com.smartsoft.jframe.3rdpart.ice.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.3rdpart.ice
