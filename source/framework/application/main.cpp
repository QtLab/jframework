#include <QApplication>
#include <QLibrary>
#include "IGF_Kernel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ���嵼���ӿ�
    typedef void *(*CreateMainFrame)();

    // ��ȡ�����ӿ�
    CreateMainFrame fCreateMainFrame = (CreateMainFrame)QLibrary::resolve(
                QApplication::applicationDirPath().append("/GF_Core"),
                "CreateMainFram");
    if (!fCreateMainFrame) {
        return -1;  // ��ȡʧ��
    }

    // ����������
    IGF_MainWindow *pMainWindow = (IGF_MainWindow *)fCreateMainFrame();
    if (!pMainWindow) {
        return -1;  // ����ʧ��
    }

    // ��ʼ��������
    pMainWindow->Initialization();

    // �����ʾ������
    pMainWindow->ShowMaximized();

    return app.exec();
}
