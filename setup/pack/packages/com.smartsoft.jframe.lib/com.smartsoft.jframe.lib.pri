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
                jwt*.lib \
                jcustomplot*.lib \
                log4cpp*.lib \
                qtribbon*.lib \
                qtwinmigrate*.lib \
                qwt*.lib \
                tinyxml*.lib \
                BCGCBPRO*u*.lib
        } else:unix {
            srcfiles += \
                libjwt*.so* \
                libjcustomplot*.so* \
                liblog4cpp*.so* \
                libqtribbon*.so* \
                libqtwinmigrate*.so* \
                libqwt*.so* \
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

    # --- lib - 3rdpart - pdb
    srcdir = "$$jframe_dir/lib/3rdpart/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/bin/3rdpart/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += \
                jwt*.pdb \
                jcustomplot*.pdb \
                log4cpp*.pdb \
                qtribbon*.pdb \
                qtwinmigrate*.pdb \
                qwt*.pdb \
                tinyxml*.pdb
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
    # --- lib - 3rdpart - BCG - pdb
    srcdir = "$$jframe_dir/lib/3rdpart/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/bin/3rdpart/BCGCBPro/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += \
                BCGCBPRO*u*.pdb
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
                jframe_facade*.lib \
                jframe_factory*.lib \
                jframe_kernel*.lib
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
    # --- lib - core - pdb
    srcdir = "$$jframe_dir/lib/core/"
    exists("$$srcdir") {
        dstdir = "$$PWD/data/jframework/lib/core/"
        srcfiles =
        win32{
            srcdir = $$replace(srcdir, /, \\)
            dstdir = $$replace(dstdir, /, \\)
            srcfiles += \
                jframe_facade*.pdb \
                jframe_factory*.pdb \
                frame_kernel*.pdb
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
