#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off
unix:commands = @echo
commands += && echo ----- $$PWD

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    # --- include
    srcdir = "$$jframe_dir/include/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframe/include/"
        srcfiles = *.h *.hh
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        !exists("$$dstdir") {
            commands += && $(MKDIR) "$$dstdir"
        }
        for (srcfile, srcfiles) {
            win32:commands += && $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir"
            unix:commands += && "$$jframe_dir/tools/xcopy.py" "$$srcdir" "$$dstdir" "$$srcfile"
        }
    }
    # --- lib - 3rdpart
    srcdir = "$$jframe_dir/bin/3rdpart/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframe/lib/3rdpart/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += \
                jwt?.dll jwt?.lib jwt?.pdb \
                log4cpp?.dll log4cpp?.lib log4cpp?.pdb \
                qtribbon?.dll qtribbon?.lib qtribbon?.pdb \
                qtwinmigrate?.dll qtwinmigrate?.lib qtwinmigrate?.pdb \
                tinyxml?.dll tinyxml?.lib tinyxml?.pdb
        } else:unix {
            srcfiles += \
                libjwt*.so* \
                liblog4cpp*.so* \
                libqtribbon*.so* \
                libqtwinmigrate*.so* \
                libtinyxml*.so*
        }
        !exists("$$dstdir") {
            commands += && $(MKDIR) "$$dstdir"
        }
        for (srcfile, srcfiles) {
            srcfile = "$$srcdir$$srcfile"
            exists("$$srcfile") {
                commands += && $(COPY_FILE) "$$srcfile" "$$dstdir"
            }
        }
    }
    # --- lib - core
    srcdir = "$$jframe_dir/bin/core/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframe/lib/core/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += \
                jframe_facade?.dll jframe_facade?.lib jframe_facade?.pdb \
                jframe_factory?.dll jframe_factory?.lib jframe_factory?.pdb \
                jframe_kernel?.dll jframe_kernel?.lib jframe_kernel?.pdb
        } else:unix {
            srcfiles += \
                libjframe_facade*.so* \
                libjframe_factory*.so* \
                libjframe_kernel*.so*
        }
        !exists("$$dstdir") {
            commands += && $(MKDIR) "$$dstdir"
        }
        for (srcfile, srcfiles) {
            srcfile = "$$srcdir$$srcfile"
            exists("$$srcfile") {
                commands += && $(COPY_FILE) "$$srcfile" "$$dstdir"
            }
        }
    }
} else {
    dstdir = "$$PWD/data/jframe/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += && $$RM_DIR "$$dstdir"
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.lib.input = INPUT
com.smartsoft.jframe.lib.output = com.smartsoft.jframe.lib
com.smartsoft.jframe.lib.commands = $$commands
com.smartsoft.jframe.lib.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.lib
