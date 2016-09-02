#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T18:09:05
#
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
commands += echo : copy wizards packages &

# --- com.smartsoft.jframe.wizards
srcdir = "$$jframe_dir/tools/wizards/qtcreator/setup/pack/packages/com.smartsoft.jframe.wizards/"
exists("$$srcdir") {
    dstdir = "$$PWD/packages/com.smartsoft.jframe.wizards/"
    srcfiles = *
    win32 {
        srcdir = $$replace(srcdir, /, \\)
        dstdir = $$replace(dstdir, /, \\)
    }
    !exists("$$dstdir") {
        commands += $(MKDIR) "$$dstdir" &
    }
    for (srcfile, srcfiles) {
        win32:commands += $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir" &
        unix:commands += "$$jframe_dir/tools/xcopy.py" "$$srcdir" "$$dstdir" "$$srcfile" &
    }
}

# --- com.smartsoft.jframe.wizards.qtcreator
srcdir = "$$jframe_dir/tools/wizards/qtcreator/setup/pack/packages/com.smartsoft.jframe.wizards.qtcreator/"
exists("$$srcdir") {
    dstdir = "$$PWD/packages/com.smartsoft.jframe.wizards.qtcreator/"
    srcfiles = *
    win32 {
        srcdir = $$replace(srcdir, /, \\)
        dstdir = $$replace(dstdir, /, \\)
    }
    !exists("$$dstdir") {
        commands += $(MKDIR) "$$dstdir" &
    }
    for (srcfile, srcfiles) {
        win32:commands += $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir" &
        unix:commands += "$$jframe_dir/tools/xcopy.py" "$$srcdir" "$$dstdir" "$$srcfile" &
    }
}

# --- com.smartsoft.jframe.wizards.visualstudio
srcdir = "$$jframe_dir/tools/wizards/qtcreator/setup/pack/packages/com.smartsoft.jframe.wizards.visualstudio/"
exists("$$srcdir") {
    dstdir = "$$PWD/packages/com.smartsoft.jframe.wizards.visualstudio/"
    srcfiles = *
    win32 {
        srcdir = $$replace(srcdir, /, \\)
        dstdir = $$replace(dstdir, /, \\)
    }
    !exists("$$dstdir") {
        commands += $(MKDIR) "$$dstdir" &
    }
    for (srcfile, srcfiles) {
        win32:commands += $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir" &
        unix:commands += "$$jframe_dir/tools/xcopy.py" "$$srcdir" "$$dstdir" "$$srcfile" &
    }
}

#-------------------------------------------------
INPUT =
pack.input = INPUT
pack.output = pack.0
pack.commands = $$commands
pack.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += pack
