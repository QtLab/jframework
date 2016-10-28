#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T18:18:11
#
#-------------------------------------------------

##
JFrameAppName = $$(JFRAME_APPNAME)

## check
equals(JFrameAppName, "") {
    error("Please set the project environment 'JFRAME_APPNAME'!")
}

TEMPLATE = subdirs
CONFIG += ordered

## default subdirs
SUBDIRS += \

## for project [!output APP_NAME]
equals(JFrameAppName, "[!output APP_NAME]") {

    SUBDIRS += \
        $$JFrameAppName

} else:

## for project xxx
equals(JFrameAppName, "xxx") {

    SUBDIRS += \

}
