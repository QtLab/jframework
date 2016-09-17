#-------------------------------------------------
# Project created by QtCreator 2016-08-31T18:09:05
#-------------------------------------------------

#-------------------------------------------------
win32:commands = @echo off &
unix:commands =
commands += echo ----- $$PWD &

#-------------------------------------------------
contains(DEFINES, PACKAGE) {
    # --- include
    srcdir = "$$jframe_dir/include/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/include/"
        srcfiles = *
        win32 {
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
        }
        !exists("$$dstdir") {
            commands += $(MKDIR) "$$dstdir" &
        }
        for (srcfile, srcfiles) {
            commands += $(COPY_DIR) "$$srcdir$$srcfile" "$$dstdir" &
        }
    }
    # --- lib - 3rdpart
    srcdir = "$$jframe_dir/lib/3rdpart/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/lib/3rdpart/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += \
                jwt*.dll jwt*.lib jwt*.pdb \
                log4cpp*.dll log4cpp*.lib log4cpp*.pdb \
                qtribbon*.dll qtribbon*.lib qtribbon*.pdb \
                qtwinmigrate*.dll qtwinmigrate*.lib qtwinmigrate*.pdb \
                tinyxml*.dll tinyxml*.lib tinyxml*.pdb \
                BCGCBPRO*u*.dll BCGCBPRO*u*.lib BCGCBPRO*u*.pdb
        } else:unix {
            srcfiles += \
                libjwt*.so* \
                liblog4cpp*.so* \
                libqtribbon*.so* \
                libqtwinmigrate*.so* \
                libtinyxml*.so*
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
    # --- lib - core
    srcdir = "$$jframe_dir/lib/core/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/lib/core/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += \
                jframe_facade*.dll jframe_facade*.lib jframe_facade*.pdb \
                jframe_factory*.dll jframe_factory*.lib jframe_factory*.pdb \
                jframe_kernel*.dll jframe_kernel*.lib jframe_kernel*.pdb
        } else:unix {
            srcfiles += \
                libjframe_facade*.so* \
                libjframe_factory*.so* \
                libjframe_kernel*.so*
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
    dstdir = "$$PWD/data/jframework/"
    exists("$$dstdir") {
        win32:dstdir = $$replace(dstdir, /, \\)
        commands += $$RM_DIR "$$dstdir" &
    }
}

#-------------------------------------------------
INPUT =
com.smartsoft.jframe.lib.input = INPUT
com.smartsoft.jframe.lib.output = com.smartsoft.jframe.lib
com.smartsoft.jframe.lib.commands = $$commands
com.smartsoft.jframe.lib.CONFIG += target_predeps no_link combine
QMAKE_EXTRA_COMPILERS += com.smartsoft.jframe.lib
