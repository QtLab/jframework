#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    srcdir = "$$jframe_dir/component/com_demo1/"
    exists("$$srcdir") {
        srcfiles =
        dstdir = "$$PWD/data/com_demo1/"
        win32{
            srcdir = $$replace(srcdir, /, \\)
            srcfiles += com_demo1*.dll
            dstdir = $$replace(dstdir, /, \\)
        } else:unix {
            srcfiles += libcom_demo1.so*
        }
        srcfiles += com_demo1*.xml
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
    dstdir = "$$PWD/data/com_demo1/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += $$RM_DIR "$$dstdir" &
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.coms.com.demo1.input = INPUT
com.smartsoft.jframe.coms.com.demo1.output = com.smartsoft.jframe.coms.com.demo1
com.smartsoft.jframe.coms.com.demo1.commands = $$commands
com.smartsoft.jframe.coms.com.demo1.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.coms.com.demo1
