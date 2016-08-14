#include <QApplication>
#include <QLibrary>
#include "jframe/jframe_facade.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    typedef IJFrameFacade *(*FunCreateInstance)();
#if 0
    frameFacade = jframeFacade();
#else
    FunCreateInstance fCreateInstance = (FunCreateInstance)QLibrary::resolve(
                QCoreApplication::applicationDirPath().append("/jframe_facade")
            #ifdef _MSC_VER
            #if defined(DEBUG) || defined(_DEBUG)
                .append("d.dll")
            #else
                .append(".dll")
            #endif
            #else
                .append(".so")
            #endif
                , "CreateInstance");
#endif
    if (!fCreateInstance) {
        return -1;      // 获取导出接口失败
    }

    IJFrameFacade *frameFacade = fCreateInstance();
    if (!frameFacade) {
        return -1;      // 获取实例失败
    }

    // 加载框架
    if (!frameFacade->loadFrame()) {
        frameFacade->exitFrame();
        return -1;      // 加载失败
    }

    // 显示框架主窗口
    frameFacade->showFrame(true, true);

    return app.exec();
}
