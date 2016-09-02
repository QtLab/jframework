#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    srcdir = "$$jframe_dir/component/jframe/jcom_login/"
    exists("$$srcdir") {
        srcfiles =
        dstdir = "$$PWD/data/jcom_login/"
        win32{
            srcdir = $$replace(srcdir, /, \\)
            srcfiles += jcom_login*.dll jcom_login*.xml
            dstdir = $$replace(dstdir, /, \\)
        } else:unix {
            srcfiles += libjcom_login.so*
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
    dstdir = "$$PWD/data/jcom_login/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += $$RM_DIR "$$dstdir" &
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.coms.jcom.login.input = INPUT
com.smartsoft.jframe.coms.jcom.login.output = com.smartsoft.jframe.coms.jcom.login
com.smartsoft.jframe.coms.jcom.login.commands = $$commands
com.smartsoft.jframe.coms.jcom.login.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.coms.jcom.login
