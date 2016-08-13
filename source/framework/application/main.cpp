#include <QApplication>
#include <QLibrary>
#include "jframe/jframe_facade.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ¼ÓÔØ¿ò¼Ü
    if (!jframeFacade()->loadFrame()) {
        return -1;      // ¼ÓÔØÊ§°Ü
    }

    // ÏÔÊ¾¿ò¼ÜÖ÷´°¿Ú
    jframeFacade()->showFrame(true, true);

    return app.exec();
}
