#include <QCoreApplication>
#include <QLibrary>
#include <QFileInfo>
#include "jframe/jframe_facade.h"

//
QString applicationDirPath()
{
    extern QString qAppFileName();
    return QFileInfo(qAppFileName()).absolutePath();
}

int main(int argc, char *argv[])
{
    FuncFrameFacadeInst fFrameFacadeInst = (FuncFrameFacadeInst)QLibrary::resolve(
                applicationDirPath().append("/jframe_facade")
            #ifdef _MSC_VER
            #ifdef _DEBUG
                .append("d.dll")
            #else
                .append(".dll")
            #endif
            #elif defined(__apple__)
                .append("")
            #elif defined(__unix__)
                .append(".so")
            #endif
                , "CreateInstance");
    if (!fFrameFacadeInst) {
        return -1;      // 获取导出接口失败
    }

    IJFrameFacade *frameFacade = dynamic_cast<IJFrameFacade *>(fFrameFacadeInst());
    if (!frameFacade) {
        return -1;      // 获取实例失败
    }

    // 加载框架
    if (!frameFacade->loadFrame(&argc, argv)) {
        frameFacade->exitFrame();
        return -1;      // 加载失败
    }

    // 显示框架主窗口
    frameFacade->showFrame(true, true);

    //
    return frameFacade->runQApp(0);
}
