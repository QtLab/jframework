#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    srcdir = "$$jframe_dir/component/com_demo5/"
    exists("$$srcdir") {
        srcfiles =
        dstdir = "$$PWD/data/com_demo5/"
        win32{
            srcdir = $$replace(srcdir, /, \\)
            srcfiles += com_demo5*.dll com_demo5*.xml
            dstdir = $$replace(dstdir, /, \\)
        } else:unix {
            srcfiles += libcom_demo5.so*
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
    dstdir = "$$PWD/data/com_demo5/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += $$RM_DIR "$$dstdir" &
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.coms.com.demo5.input = INPUT
com.smartsoft.jframe.coms.com.demo5.output = com.smartsoft.jframe.coms.com.demo5
com.smartsoft.jframe.coms.com.demo5.commands = $$commands
com.smartsoft.jframe.coms.com.demo5.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.coms.com.demo5
