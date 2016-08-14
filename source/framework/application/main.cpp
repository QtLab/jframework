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
        return -1;      // ��ȡ�����ӿ�ʧ��
    }

    IJFrameFacade *frameFacade = fCreateInstance();
    if (!frameFacade) {
        return -1;      // ��ȡʵ��ʧ��
    }

    // ���ؿ��
    if (!frameFacade->loadFrame()) {
        frameFacade->exitFrame();
        return -1;      // ����ʧ��
    }

    // ��ʾ���������
    frameFacade->showFrame(true, true);

    return app.exec();
}
