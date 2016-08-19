#include <QCoreApplication>
#include <QLibrary>
#include <QFileInfo>
#include <QTextCodec>
#include <QDebug>
#include "jframe/jframe_facade.h"
#if defined(__unix__)
#include <unistd.h>
#endif

//
QString applicationDirPath()
{
#ifdef _MSC_VER
    extern QString qAppFileName();
    return QFileInfo(qAppFileName()).canonicalPath();
#elif defined(__unix__)
    // Try looking for a /proc/<pid>/exe symlink first which points to
    // the absolute path of the executable
    QFileInfo fileInfo(QString::fromLatin1("/proc/%1/exe").arg(getpid()));
    //printf("%s\n", fileInfo.canonicalPath().toLatin1().data());
    if (fileInfo.exists() && fileInfo.isSymLink()) {
        return fileInfo.canonicalPath();
    }
    return QString();
#else
#pragma message("Not supported!")
    return QString();
#endif
}

int main(int argc, char *argv[])
{
    //
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
#endif
    //
    FuncFrameFacadeInst fFrameFacadeInst = (FuncFrameFacadeInst)QLibrary::resolve(
                applicationDirPath().append("/jframe/")
            #ifdef _MSC_VER
            #if defined(_DEBUG) || defined(DEBUG)
                .append("jframe_facaded.dll")
            #else
                .append("jframe_facade.dll")
            #endif
            #elif defined(__apple__)
               #pragma message("Not supported!")
            #elif defined(__unix__)
                .append("libjframe_facade.so")
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
