#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T18:09:05
#
#-------------------------------------------------

#-------------------------------------------------
include($$PWD/com.smartsoft.jframe.3rdpart/com.smartsoft.jframe.3rdpart.pri)
win32:include($$PWD/com.smartsoft.jframe.3rdpart.bcg/com.smartsoft.jframe.3rdpart.bcg.pri)
include($$PWD/com.smartsoft.jframe.3rdpart.opengl/com.smartsoft.jframe.3rdpart.opengl.pri)
include($$PWD/com.smartsoft.jframe.3rdpart.qt/com.smartsoft.jframe.3rdpart.qt.pri)
include($$PWD/com.smartsoft.jframe.coms/com.smartsoft.jframe.coms.pri)
include($$PWD/com.smartsoft.jframe.coms.com.demo1/com.smartsoft.jframe.coms.com.demo1.pri)
include($$PWD/com.smartsoft.jframe.coms.com.demo2/com.smartsoft.jframe.coms.com.demo2.pri)
include($$PWD/com.smartsoft.jframe.coms.com.demo3/com.smartsoft.jframe.coms.com.demo3.pri)
win32:include($$PWD/com.smartsoft.jframe.coms.com.demo4/com.smartsoft.jframe.coms.com.demo4.pri)
win32:include($$PWD/com.smartsoft.jframe.coms.com.demo5/com.smartsoft.jframe.coms.com.demo5.pri)
include($$PWD/com.smartsoft.jframe.coms.jcom/com.smartsoft.jframe.coms.jcom.pri)
include($$PWD/com.smartsoft.jframe.coms.jcom.layout/com.smartsoft.jframe.coms.jcom.layout.pri)
include($$PWD/com.smartsoft.jframe.coms.jcom.login/com.smartsoft.jframe.coms.jcom.login.pri)
include($$PWD/com.smartsoft.jframe.core/com.smartsoft.jframe.core.pri)
include($$PWD/com.smartsoft.jframe.core.facade/com.smartsoft.jframe.core.facade.pri)
include($$PWD/com.smartsoft.jframe.core.factory/com.smartsoft.jframe.core.factory.pri)
include($$PWD/com.smartsoft.jframe.core.kernel/com.smartsoft.jframe.core.kernel.pri)
include($$PWD/com.smartsoft.jframe.example/com.smartsoft.jframe.example.pri)
include($$PWD/com.smartsoft.jframe.example.config/com.smartsoft.jframe.example.config.pri)
include($$PWD/com.smartsoft.jframe.framework.jframeworkdir/com.smartsoft.jframe.framework.jframeworkdir.pri)
include($$PWD/com.smartsoft.jframe.framework.jframeworkenv/com.smartsoft.jframe.framework.jframeworkenv.pri)
include($$PWD/com.smartsoft.jframe.lib/com.smartsoft.jframe.lib.pri)

#-------------------------------------------------
module = "$$PWD/com.smartsoft.jframe.wizards/com.smartsoft.jframe.wizards.pri"
exists("$$module"):include($$module)
module = "$$PWD/com.smartsoft.jframe.wizards.qtcreator/com.smartsoft.jframe.wizards.qtcreator.pri"
exists("$$module"):include($$module)
module = "$$PWD/com.smartsoft.jframe.wizards.visualstudio/com.smartsoft.jframe.wizards.visualstudio.pri"
exists("$$module"):include($$module)
