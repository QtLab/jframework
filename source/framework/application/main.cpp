#include <QApplication>
#include <QLibrary>
#include "jframe/jframe_facade.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ���ؿ��
    if (!jframeFacade()->loadFrame()) {
        return -1;      // ����ʧ��
    }

    // ��ʾ���������
    jframeFacade()->showFrame(true, true);

    return app.exec();
}
