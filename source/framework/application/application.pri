#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T18:18:11
#
#-------------------------------------------------

##
win32:commands += "$$THIS_DIR\\syncjframe.cmd" $$QT_MAJOR_VERSION $$debug_suffix &
unix:commands += "$$THIS_DIR/syncjframe.sh" $$QT_MAJOR_VERSION $$QT_MINOR_VERSION $$QT_VERSION $$debug_suffix &
