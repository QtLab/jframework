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
SUBDIRS += \\

## for project %{ProjectName}
equals(JFrameAppName, "%{ProjectName}") {

    SUBDIRS += \\
        $$JFrameAppName

} else:

## for project xxx
equals(JFrameAppName, "xxx") {

    SUBDIRS += \\

}
