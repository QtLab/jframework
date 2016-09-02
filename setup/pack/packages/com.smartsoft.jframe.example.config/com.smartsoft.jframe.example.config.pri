#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    # --- frame
    srcdir = "$$jframe_dir/config/frame/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/config/frame/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        srcfiles += *.*
        !exists("$$dstdir") {
            commands += $(MKDIR) "$$dstdir" &
        }
        for (srcfile, srcfiles) {
            win32:commands += $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir" &
            unix:commands += "$$jframe_dir/tools/xcopy.py" "$$srcdir" "$$dstdir" "$$srcfile" &
        }
    }
    # --- resource
    srcdir = "$$jframe_dir/config/resource/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/config/resource/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        srcfiles += *.*
        !exists("$$dstdir") {
            commands += $(MKDIR) "$$dstdir" &
        }
        for (srcfile, srcfiles) {
            win32:commands += $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir" &
            unix:commands += "$$jframe_dir/tools/xcopy.py" "$$srcdir" "$$dstdir" "$$srcfile" &
        }
    }
    # --- WorkMode_Test1
    srcdir = "$$jframe_dir/config/WorkMode_Test1/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/config/WorkMode_Test1/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        srcfiles += *.*
        !exists("$$dstdir") {
            commands += $(MKDIR) "$$dstdir" &
        }
        for (srcfile, srcfiles) {
            win32:commands += $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir" &
            unix:commands += "$$jframe_dir/tools/xcopy.py" "$$srcdir" "$$dstdir" "$$srcfile" &
        }
    }
    # --- WorkMode_Test2
    srcdir = "$$jframe_dir/config/WorkMode_Test2/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/config/WorkMode_Test2/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        srcfiles += *.*
        !exists("$$dstdir") {
            commands += $(MKDIR) "$$dstdir" &
        }
        for (srcfile, srcfiles) {
            win32:commands += $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir" &
            unix:commands += "$$jframe_dir/tools/xcopy.py" "$$srcdir" "$$dstdir" "$$srcfile" &
        }
    }
} else {
    dstdir = "$$PWD/data/config/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += $$RM_DIR "$$dstdir" &
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.config.input = INPUT
com.smartsoft.jframe.config.output = com.smartsoft.jframe.config
com.smartsoft.jframe.config.commands = $$commands
com.smartsoft.jframe.config.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.config
