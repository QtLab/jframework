#include "precomp.h"
#include "jframewnd.h"
#include "jattempter.h"

JFrameWnd::JFrameWnd(JAttempter *attempter, QWidget *parent, Qt::WindowFlags f)
    : QtRibbon::RibbonMainWindow(parent, f)
    , q_attempter(attempter)
    , q_splashScreen(0)
    , q_centralWidget(0)
{
    setObjectName("jframeWnd");

    //
    setMinimumSize(800, 480);
    resize(1024, 700);

    //
    ribbonBar()->setObjectName("ribbonBar");
    ribbonBar()->setProperty("domain", "framework");
    q_ribbonBarFont = ribbonBar()->font();

    //
    q_splashScreen = new QSplashScreen(this);
    q_splashScreen->setFont(QFont("microsoft yahei", 24));  //DEMO

    //
    q_centralWidget = new QStackedWidget(this);
    setCentralWidget(q_centralWidget);

    //
    QApplication::setStyle(new QtRibbon::RibbonStyle);
    ribbonBar()->setMinimizationEnabled(true);
    ribbonBar()->setFrameThemeEnabled(true);
    //ribbonBar()->setTitleBarVisible(true);

    //
    connect(ribbonBar(), SIGNAL(currentPageChanged(int)),
            this, SLOT(onCurrentPageChanged(int)));
}

JFrameWnd::~JFrameWnd()
{

}

bool JFrameWnd::init()
{
    if (!loadConfig()) {
        return false;
    }

    //
    if (!createOptionAction()) {
        return false;
    }

    //

    return true;
}

void JFrameWnd::updateSplashInfo(const QString &info)
{
    if (q_splashScreen) {
        q_splashScreen->showMessage(info,
                                    Qt::AlignHCenter | Qt::AlignBottom,
                                    QColor("steelblue"));
    }
}

void JFrameWnd::setCurrentWidget(QWidget *widget)
{
    q_centralWidget->setCurrentWidget(widget);
}

void JFrameWnd::setTheme(const QString &theme)
{
    //
    q_ribbonBarFont = ribbonBar()->font();

    //
    QtRibbon::RibbonStyle *ribbonStyle =
            qobject_cast<QtRibbon::RibbonStyle *>(QApplication::style());
    if (!ribbonStyle) {
        ribbonStyle = new QtRibbon::RibbonStyle;
        QApplication::setStyle(ribbonStyle);
    }

    // 解析主题信息
    QtRibbon::RibbonStyle::Theme nTheme;
    if (theme == J_FRAME_THEME_OFFICE_2007BLUE) {
        nTheme = QtRibbon::RibbonStyle::Office2007Blue;
    } else if (theme == J_FRAME_THEME_OFFICE_2007BLACK) {
        nTheme = QtRibbon::RibbonStyle::Office2007Black;
    } else if (theme == J_FRAME_THEME_OFFICE_2007SILVER) {
        nTheme = QtRibbon::RibbonStyle::Office2007Silver;
    } else if (theme == J_FRAME_THEME_OFFICE_2007AQUA) {
        nTheme = QtRibbon::RibbonStyle::Office2007Aqua;
    } else if (theme == J_FRAME_THEME_WINDOWs7_SCENIC) {
        nTheme = QtRibbon::RibbonStyle::Windows7Scenic;
    } else if (theme == J_FRAME_THEME_OFFICE_2010SILVER) {
        nTheme = QtRibbon::RibbonStyle::Office2010Silver;
    } else if (theme == J_FRAME_THEME_OFFICE_2010BLUE) {
        nTheme = QtRibbon::RibbonStyle::Office2010Blue;
    } else if (theme == J_FRAME_THEME_OFFICE_2010BLACK) {
        nTheme = QtRibbon::RibbonStyle::Office2010Black;
    } else {
        nTheme = QtRibbon::RibbonStyle::Office2010Blue;
    }

    //
    QToolButton* toolButton = ribbonBar()->getSystemButton();
    if (toolButton) {
        if (nTheme == QtRibbon::OfficeStyle::Windows7Scenic ||
                nTheme == QtRibbon::OfficeStyle::Office2010Blue ||
                nTheme == QtRibbon::OfficeStyle::Office2010Silver ||
                nTheme == QtRibbon::OfficeStyle::Office2010Black) {
            toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
        } else {
            toolButton->setToolButtonStyle(Qt::ToolButtonFollowStyle);
        }
    }

    //
    ribbonStyle->setTheme(nTheme);

    //
    ribbonBar()->setFont(q_ribbonBarFont);
}

QStackedWidget *JFrameWnd::stackedWidget()
{
    return q_centralWidget;
}

void JFrameWnd::onStyleChanged(QAction *action)
{
    setTheme(action->objectName());
}

void JFrameWnd::onCurrentPageChanged(int page)
{
    //
    QObject *sender = this->sender();
    if (!sender) {
        return;
    }

    //
    q_attempter->commandSink(sender, "currentPageChanged", (void *)&page);
}

void JFrameWnd::onActionTriggered(bool checked)
{
    //
    QObject *sender = this->sender();
    if (!sender) {
        return;
    }

    //
    q_attempter->commandSink(sender, "triggered", (void *)&checked);
}

void JFrameWnd::onComboBoxCurrentIndexChanged(int index)
{
    //
    QObject *sender = this->sender();
    if (!sender) {
        return;
    }

    //
    q_attempter->commandSink(sender, "currentIndexChanged", (void *)&index);
}

void JFrameWnd::onCheckBoxStateChanged(int state)
{
    //
    QObject *sender = this->sender();
    if (!sender) {
        return;
    }

    //
    q_attempter->commandSink(sender, "stateChanged", (void *)&state);
}

void JFrameWnd::onRadioButtonClicked(bool checked)
{
    //
    QObject *sender = this->sender();
    if (!sender) {
        return;
    }

    //
    q_attempter->commandSink(sender, "clicked", (void *)&checked);
}

bool JFrameWnd::loadConfig()
{
    //
    QFile file(QString::fromStdString(jframeFacade()->frameGlobalPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("框架全局配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(this, QStringLiteral("警告"), text);
        return false;   // 文件不存在
    }

    // 打开文件
    if (!file.open(QFile::ReadOnly)) {
        const QString text = QStringLiteral("框架全局配置文件\"%1\"打开失败！")
                .arg(file.fileName());
        QMessageBox::warning(this, QStringLiteral("警告"), text);
        return false;
    }

    //
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("框架全局配置文件\"%1\"解析失败！\n"
                                            "错误描述：%2\n"
                                            "错误位置：（行号：%3，列号：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(this, QStringLiteral("警告"), text);
        return false;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;
    }

    // 获取 MainWindow 节点
    QDomElement emMainWindow = emRoot.firstChildElement("mainWindow");
    if (emMainWindow.isNull()) {
        return false;
    }

    // windowTitle
    if (emMainWindow.hasAttribute("windowTitle")) {
        setWindowTitle(emMainWindow.attribute("windowTitle"));
        ribbonBar()->setWindowTitle(windowTitle());
    }

    // windowIcon
    if (emMainWindow.hasAttribute("windowIcon")) {
        setWindowIcon(QIcon(emMainWindow.attribute("windowIcon")
                            .replace("@ConfigDir@", QString::fromStdString(
                                         jframeFacade()->configDirPath()))));
        qApp->setWindowIcon(windowIcon());
    }

    // windowTheme
    if (emMainWindow.hasAttribute("windowTheme")) {
        setTheme(emMainWindow.attribute("windowTheme").toStdString().c_str());
    }

    // splash node
    QDomElement emSplash = emMainWindow.firstChildElement("splash");
    if (!emSplash.isNull() && q_splashScreen) {
        // imageSplash
        if (emSplash.hasAttribute("imageStart")) {
            const QString imageStart = emSplash.attribute("imageStart")
                    .replace("@ConfigDir@", QString::fromStdString(
                                 jframeFacade()->configDirPath()));
            if (!imageStart.isEmpty()) {
                QSize screenSize = QApplication::desktop()->screenGeometry().size();
                QSize splashSize = screenSize / 2;
                q_pixmapSplash = QPixmap(imageStart).scaled(splashSize);
            }
        }
        q_splashScreen->setPixmap(q_pixmapSplash);

        // imageFinish
        if (emSplash.hasAttribute("imageFinish")) {
            const QString imageFinish = emSplash.attribute("imageFinish")
                    .replace("@ConfigDir@", QString::fromStdString(
                                 jframeFacade()->configDirPath()));
            if (!imageFinish.isEmpty()) {
                q_pixmapFinish = QPixmap(imageFinish);
            }
        }
    }

    // systemFont
    if (emMainWindow.hasAttribute("systemFont")) {
        const QString systemFont = emMainWindow.attribute("systemFont");
        qApp->setFont(QFont(systemFont.section(',', 0, 0, QString::SectionSkipEmpty),
                            systemFont.section(',', 1).toInt()));
    }

    /// for ribbon
    //

    // 获取RibbonBar节点
    QDomElement emRibbonBar = emMainWindow.firstChildElement("ribbonBar");
    if (!emRibbonBar.isNull()) {
        // font
        ribbonBar()->setFont(QFont("microsoft yahei", 9));
        // visible
        if (emRibbonBar.hasAttribute("visible")) {
            ribbonBar()->setVisible(QVariant(emRibbonBar.attribute("visible")).toBool());
        }
        // minimized
        if (emRibbonBar.hasAttribute("minimized")) {
            ribbonBar()->setMinimized(QVariant(emRibbonBar.attribute("minimized")).toBool());
        }
    }

    // stylesheet - background
#if 0
    setStyleSheet(QString("%1#%2{backbground:'#6E7E93';}")
                  .arg(metaObject()->className()).arg(objectName()));
#endif

    return true;
}

int JFrameWnd::ribbonStyleTheme() const
{
    //
    QtRibbon::RibbonStyle *ribbonStyle =
            qobject_cast<QtRibbon::RibbonStyle *>(QApplication::style());
    if (!ribbonStyle) {
        return -1;
    }

    return (int)ribbonStyle->getTheme();
}

bool JFrameWnd::createOptionAction()
{
    //
    QMenu *menuOption = ribbonBar()->addMenu(QStringLiteral("选项"));
    QAction *actionStyle = menuOption->addAction(QStringLiteral("皮肤风格"));
    QMenu *menuStyle = new QMenu(ribbonBar());
    actionStyle->setMenu(menuStyle);
    QActionGroup *actionGroupStyle = new QActionGroup(this);
    QAction *actionTemp = 0;
    int theme = ribbonStyleTheme();

    // Office2007Blue
    actionTemp = menuStyle->addAction("Office 2007 Blue");
    actionTemp->setObjectName(J_FRAME_THEME_OFFICE_2007BLUE);
    actionTemp->setCheckable(true);
    actionTemp->setChecked(theme == QtRibbon::RibbonStyle::Office2007Blue);
    actionGroupStyle->addAction(actionTemp);

    // Office2007Black
    actionTemp = menuStyle->addAction("Office 2007 Black");
    actionTemp->setObjectName(J_FRAME_THEME_OFFICE_2007BLACK);
    actionTemp->setCheckable(true);
    actionTemp->setChecked(theme == QtRibbon::RibbonStyle::Office2007Black);
    actionGroupStyle->addAction(actionTemp);

    // Office2007Silver
    actionTemp = menuStyle->addAction("Office 2007 Silver");
    actionTemp->setObjectName(J_FRAME_THEME_OFFICE_2007SILVER);
    actionTemp->setCheckable(true);
    actionTemp->setChecked(theme == QtRibbon::RibbonStyle::Office2007Silver);
    actionGroupStyle->addAction(actionTemp);

    // Office2007Aqua
    actionTemp = menuStyle->addAction("Office 2007 Aqua");
    actionTemp->setObjectName(J_FRAME_THEME_OFFICE_2007AQUA);
    actionTemp->setCheckable(true);
    actionTemp->setChecked(theme == QtRibbon::RibbonStyle::Office2007Aqua);
    actionGroupStyle->addAction(actionTemp);

    // Windows7Scenic
    actionTemp = menuStyle->addAction("Windows 7 Scenic");
    actionTemp->setObjectName(J_FRAME_THEME_WINDOWs7_SCENIC);
    actionTemp->setCheckable(true);
    actionTemp->setChecked(theme == QtRibbon::RibbonStyle::Windows7Scenic);
    actionGroupStyle->addAction(actionTemp);

    // Office2010Blue
    actionTemp = menuStyle->addAction("Office 2010 Blue");
    actionTemp->setObjectName(J_FRAME_THEME_OFFICE_2010BLUE);
    actionTemp->setCheckable(true);
    actionTemp->setChecked(theme == QtRibbon::RibbonStyle::Office2010Blue);
    actionGroupStyle->addAction(actionTemp);

    // Office2010Silver
    actionTemp = menuStyle->addAction("Office 2010 Silver");
    actionTemp->setObjectName(J_FRAME_THEME_OFFICE_2010SILVER);
    actionTemp->setCheckable(true);
    actionTemp->setChecked(theme == QtRibbon::RibbonStyle::Office2010Silver);
    actionGroupStyle->addAction(actionTemp);

    // Office2010Black
    actionTemp = menuStyle->addAction("Office 2010 Black");
    actionTemp->setObjectName(J_FRAME_THEME_OFFICE_2010BLACK);
    actionTemp->setCheckable(true);
    actionTemp->setChecked(theme == QtRibbon::RibbonStyle::Office2010Black);
    actionGroupStyle->addAction(actionTemp);

    //
    connect(actionGroupStyle, SIGNAL(triggered(QAction*)),
            SLOT(onStyleChanged(QAction*)));

    return true;
}

void JFrameWnd::startSplash()
{
    if (q_splashScreen) {
        q_splashScreen->activateWindow();
        q_splashScreen->show();
        q_splashScreen->raise();
        updateSplash();
    }
}

void JFrameWnd::finishSplash()
{
    if (q_splashScreen) {
        q_splashScreen->finish(this);
        q_splashScreen->deleteLater();
        q_splashScreen = 0;
    }
}

void JFrameWnd::updateSplash()
{
    //QApplication::processEvents();
}
