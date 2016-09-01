#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T18:18:11
#
#-------------------------------------------------

##
win32:commands += "\"$${jframe_root}\\syncjframe.cmd\"" $$QT_MAJOR_VERSION $$debug_suffix &
unix:commands += "\"$${jframe_root}/syncjframe.sh\"" $$QT_MAJOR_VERSION $$QT_MINOR_VERSION $$QT_VERSION $$debug_suffix &
