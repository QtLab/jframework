#include "precomp.h"
#include <QApplication>
#include <QTextCodec>
#include "jframe_facade.h"
#include "main_window.h"

/**
 * @brief ExecGui
 * @param argc
 * @param argv
 * @return
 */
J_EXTERN_C Q_DECL_EXPORT int ExecGui(int argc, char **argv)
{
    QApplication app(argc, argv);

    // text codec
    QString codeForName;
#ifdef _MSC_VER
    codeForName = "gbk";
#elif defined(__unix__)
    codeForName = "utf-8";
#else
    codeForName = "utf-8";
#endif

    QTextCodec::setCodecForLocale(QTextCodec::codecForName(codeForName.toUtf8()));
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
#endif

    // stylesheet
    QFile file(":/jfwconfiggui/qss/stylesheet_1.qss");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return -1;  // 样式文件打开失败
    }
    app.setStyleSheet(file.readAll());

    //
    app.setFont(QFont("microsoft yahei", 10));

    //
    const QRect screenGeometry = app.desktop()->screenGeometry();

    //
    MainWindow mainWindow;
    mainWindow.resize(screenGeometry.width() * 2 / 3,
                      screenGeometry.height() * 2 / 3);
    mainWindow.show();

    return app.exec();
}
