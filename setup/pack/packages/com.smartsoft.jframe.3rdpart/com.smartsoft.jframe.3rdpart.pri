#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off
unix:commands = @echo
commands += && echo ----- $$PWD

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    srcdir = "$$jframe_dir/lib/3rdpart/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/3rdpart/"
        srcfiles =
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += jwt*.dll log4cpp*.dll qtribbon*.dll qtwinmigrate*.dll tinyxml*.dll
        } else:unix {
            srcfiles += libjwt*.so* liblog4cpp*.so* libqtribbon*.so* libqtwinmigrate*.so* libtinyxml*.so*
        }
        !exists("$$dstdir") {
            commands += && $(MKDIR) "\"$$dstdir\""
        }
        for (srcfile, srcfiles) {
            srcfile = "$$srcdir$$srcfile"
            exists("$$srcfile") {
                commands += && $(COPY_FILE) "\"$$srcfile\"" "\"$$dstdir\""
            }
        }
    }
} else {
    dstdir = "$$PWD/data/3rdpart/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += && $$RM_DIR "$$dstdir"
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.3rdpart.input = INPUT
com.smartsoft.jframe.3rdpart.output = com.smartsoft.jframe.3rdpart
com.smartsoft.jframe.3rdpart.commands = $$commands
com.smartsoft.jframe.3rdpart.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.3rdpart
