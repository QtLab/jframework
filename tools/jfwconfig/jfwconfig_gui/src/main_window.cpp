#include "precomp.h"
#include "main_window.h"
#include "global/global_config_widget.h"
#include "component/com_config_widget.h"
#include "layout/layout_config_widget.h"
#include "logging/logging_conf_widget.h"

/**
 * @brief libraryPrefix
 * @return
 */
const char* libraryPrefix()
{
    static std::string _v = "";
    if (_v.empty()) {
#ifdef _MSC_VER
        _v = "";
#elif defined(__unix__)
        _v = "lib";
#else
#pragma message("not supported!")
        _v = "";
#endif
    }

    return _v.c_str();
}

/**
 * @brief librarySuffix
 * @param debug
 * @return
 */
const char* librarySuffix(bool debug)
{
#ifdef _MSC_VER
    return debug ? "d.dll" : ".dll";
#elif defined(__unix__)
    (void)debug;
    return ".so";
#else
#pragma message("not supported!")
    (void)debug;
    return "";
#endif
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("JFW软件配置助手"));

    q_centralWidget = new QWidget(this);
    setCentralWidget(q_centralWidget);

    QVBoxLayout *vertLayoutMain = new QVBoxLayout(q_centralWidget);
    vertLayoutMain->setContentsMargins(1, 1, 1, 1);
    vertLayoutMain->setSpacing(2);

    QHBoxLayout *horiLayoutAppPath = new QHBoxLayout();
    horiLayoutAppPath->setContentsMargins(2, 2, 2, 2);
    vertLayoutMain->addLayout(horiLayoutAppPath);

    QFormLayout *formLayoutAppPath = new QFormLayout();
    formLayoutAppPath->setFormAlignment(Qt::AlignVCenter);
    horiLayoutAppPath->addLayout(formLayoutAppPath);

    q_editAppPath = new QLineEdit(q_centralWidget);
    q_editAppPath->setReadOnly(true);
    formLayoutAppPath->addRow(QStringLiteral("目标软件路径："), q_editAppPath);

    QPushButton *buttonView = new QPushButton(QStringLiteral("浏览"), q_centralWidget);
    horiLayoutAppPath->addWidget(buttonView);

    QHBoxLayout *horiLayoutTop = new QHBoxLayout();
    horiLayoutTop->setContentsMargins(1, 1, 1, 1);
    vertLayoutMain->addLayout(horiLayoutTop);

    q_toolBarNav = new QToolBar(q_centralWidget);
    q_toolBarNav->setOrientation(Qt::Vertical);
    q_toolBarNav->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    horiLayoutTop->addWidget(q_toolBarNav);

    q_stackedWidget = new QStackedWidget(q_centralWidget);
    horiLayoutTop->addWidget(q_stackedWidget);

    q_globalConfigWidget = new GlobalConfigWidget(q_centralWidget);
    q_comConfigWidget = new ComConfigWidget(q_centralWidget);
    q_layoutConfigWidget = new LayoutConfigWidget(q_centralWidget);
    q_loggingConfWidget = new LoggingConfWidget(q_centralWidget);
    q_stackedWidget->addWidget(q_globalConfigWidget);
    q_stackedWidget->addWidget(q_comConfigWidget);
    q_stackedWidget->addWidget(q_layoutConfigWidget);
    q_stackedWidget->addWidget(q_loggingConfWidget);

    //
    q_toolBarNav->addAction(QIcon(":/jfwconfiggui/image/app_icon.png"),
                            QStringLiteral("全局配置"), this, SLOT(onActionGlobalClicked()));
    q_toolBarNav->addAction(QIcon(":/jfwconfiggui/image/app_icon.png"),
                            QStringLiteral("组件配置"), this, SLOT(onActionComponentClicked()));
    q_toolBarNav->addAction(QIcon(":/jfwconfiggui/image/app_icon.png"),
                            QStringLiteral("布局配置"), this, SLOT(onActionLayoutClicked()));
    q_toolBarNav->addAction(QIcon(":/jfwconfiggui/image/app_icon.png"),
                            QStringLiteral("日志配置"), this, SLOT(onActionLoggingClicked()));

    //
    connect(q_stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(onCurrentWidgetChanged(int)));
    connect(buttonView, SIGNAL(clicked(bool)), this, SLOT(onButtonViewClicked()));

    //
    if (!updateCurrentModule()) {
        //
    }
}

void MainWindow::onActionGlobalClicked()
{
    q_stackedWidget->setCurrentIndex(0);
}

void MainWindow::onActionComponentClicked()
{
    q_stackedWidget->setCurrentIndex(1);
}

void MainWindow::onActionLayoutClicked()
{
    q_stackedWidget->setCurrentIndex(2);
}

void MainWindow::onActionLoggingClicked()
{
    q_stackedWidget->setCurrentIndex(3);
}

void MainWindow::onButtonViewClicked()
{
    QString path = QFileDialog::getExistingDirectory(this, QStringLiteral("软件路径"));
    if (path.isEmpty()) {
        //
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("路径无效！"));
        return;
    }

    //
    path.replace('\\', '/');

    //
    if (!pathIsValid(path)) {
        //
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("不是有效的基于jframework框架的衍生软件路径！"));
        return;
    }

    //
    q_editAppPath->setText(path);

    //
    if (!updateCurrentModule()) {
        return;     // update failure
    }
}

void MainWindow::onCurrentWidgetChanged(int)
{
    //
    if (!updateCurrentModule()) {
        return;     // update failure
    }
}

bool MainWindow::pathIsValid(const QString &path) const
{
    return QFile(path + "/bin/" + libraryPrefix() +
                 "jframeworkdir" + librarySuffix(false)).exists();
}

bool MainWindow::updateCurrentModule()
{
    if (!pathIsValid(q_editAppPath->text())) {
        return false;   //
    }

    QWidget *currentWidget = q_stackedWidget->currentWidget();
    if (!currentWidget) {
        return false;
    }

    //
    bool result = true;
    QMetaObject::invokeMethod(currentWidget, "appDirChanged", Q_RETURN_ARG(bool, result),
                              Q_ARG(QString, q_editAppPath->text()));

    return result;
}
