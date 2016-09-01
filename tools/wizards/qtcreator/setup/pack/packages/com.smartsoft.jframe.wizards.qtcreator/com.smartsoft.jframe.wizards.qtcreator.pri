#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off
unix:commands = @echo
commands += && echo ----- $$PWD

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    # --- jframework - wizards
    srcdir = "$$jframe_dir/tools/wizards/qtcreator/jframework/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/"
        srcfiles = *
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        !exists("$$dstdir") {
            commands += && $(MKDIR) "\"$$dstdir\""
        }
        for (srcfile, srcfiles) {
            win32:commands += && $(COPY_DIR) "\"$$srcdir$$srcfile\"" "\"$$dstdir\""
            unix:commands += && "\"$$jframe_dir/tools/xcopy.py\"" "\"$$srcdir\"" "\"$$dstdir\"" "$$srcfile"
        }
    }
    # --- bin/jframeworkdir
    srcdir = "$$jframe_dir/bin/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/solution/bin/"
        srcfiles =
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += jframeworkdir*.dll
        } else:unix {
            srcfiles += libjframeworkdir.so*
        }
        !exists("$$dstdir") {
            commands += && $(MKDIR) "\"$$dstdir\""
        }
        for (srcfile, srcfiles) {
            commands += && $(COPY_FILE) "\"$$srcdir$$srcfile\"" "\"$$dstdir\""
        }
    }
    # --- config
    srcdir = "$$jframe_dir/config/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/solution/config/"
        srcfiles = *
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        !exists("$$dstdir") {
            commands += && $(MKDIR) "\"$$dstdir\""
        }
        for (srcfile, srcfiles) {
            win32:commands += && $(COPY_DIR) "\"$$srcdir$$srcfile\"" "\"$$dstdir\""
            unix:commands += && "\"$$jframe_dir/tools/xcopy.py\"" "\"$$srcdir\"" "\"$$dstdir\"" "$$srcfile"
        }
    }
    # --- source/common
    srcdir = "$$jframe_dir/source/common/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/solution/source/common/"
        srcfiles = *
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        !exists("$$dstdir") {
            commands += && $(MKDIR) "\"$$dstdir\""
        }
        for (srcfile, srcfiles) {
            win32:commands += && $(COPY_DIR) "\"$$srcdir$$srcfile\"" "\"$$dstdir\""
            unix:commands += && "\"$$jframe_dir/tools/xcopy.py\"" "\"$$srcdir\"" "\"$$dstdir\"" "$$srcfile"
        }
    }
    # --- source/framework/application
    srcdir = "$$jframe_dir/source/framework/application/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/solution/source/framework/application/"
        srcfiles =
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        srcfiles += application.pro main.cpp
        !exists("$$dstdir") {
            commands += && $(MKDIR) "\"$$dstdir\""
        }
        for (srcfile, srcfiles) {
            commands += && $(COPY_FILE) "\"$$srcdir$$srcfile\"" "\"$$dstdir\""
        }
    }
} else {
    dstdir = "$$PWD/data/jframework/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += && $$RM_DIR "\"$$dstdir\""
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.wizards.qtcreator.input = INPUT
com.smartsoft.jframe.wizards.qtcreator.output = com.smartsoft.jframe.wizards.qtcreator
com.smartsoft.jframe.wizards.qtcreator.commands = $$commands
com.smartsoft.jframe.wizards.qtcreator.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.wizards.qtcreator
