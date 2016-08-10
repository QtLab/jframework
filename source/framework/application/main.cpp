#include <QApplication>
#include <QLibrary>
#include "IGF_Kernel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 定义导出接口
    typedef void *(*CreateMainFrame)();

    // 获取导出接口
    CreateMainFrame fCreateMainFrame = (CreateMainFrame)QLibrary::resolve(
                QApplication::applicationDirPath().append("/GF_Core"),
                "CreateMainFram");
    if (!fCreateMainFrame) {
        return -1;  // 获取失败
    }

    // 创建主窗口
    IGF_MainWindow *pMainWindow = (IGF_MainWindow *)fCreateMainFrame();
    if (!pMainWindow) {
        return -1;  // 创建失败
    }

    // 初始化主窗口
    pMainWindow->Initialization();

    // 最大化显示主窗口
    pMainWindow->ShowMaximized();

    return app.exec();
}
