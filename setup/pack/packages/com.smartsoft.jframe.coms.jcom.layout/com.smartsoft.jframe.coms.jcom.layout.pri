#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    srcdir = "$$jframe_dir/component/jframe/jcom_layout/"
    exists("$$srcdir") {
        srcfiles =
        dstdir = "$$PWD/data/jcom_layout/"
        win32{
            srcdir = $$replace(srcdir, /, \\)
            srcfiles += jcom_layout*.dll jcom_layout*.xml
            dstdir = $$replace(dstdir, /, \\)
        } else:unix {
            srcfiles += libjcom_layout.so*
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
    dstdir = "$$PWD/data/jcom_layout/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += $$RM_DIR "$$dstdir" &
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.coms.jcom.layout.input = INPUT
com.smartsoft.jframe.coms.jcom.layout.output = com.smartsoft.jframe.coms.jcom.layout
com.smartsoft.jframe.coms.jcom.layout.commands = $$commands
com.smartsoft.jframe.coms.jcom.layout.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.coms.jcom.layout
