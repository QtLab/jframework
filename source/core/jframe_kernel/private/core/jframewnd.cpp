#include "precomp.h"
#include "jframewnd.h"
#include "jattempter.h"

JFrameWnd::JFrameWnd(JAttempter *attempter, QWidget *parent, Qt::WindowFlags f)
    : QtRibbon::RibbonMainWindow(parent, f)
    , q_attempter(attempter)
    , q_splashScreen(0)
    , q_centralWidget(0)
{
    setObjectName("JFrameWnd");

    //
    setMinimumSize(800, 480);
    resize(1024, 700);

    //
    q_splashScreen = new QSplashScreen(this);
    q_centralWidget = new QStackedWidget(this);
    setCentralWidget(q_centralWidget);

    //
    QApplication::setStyle(new QtRibbon::RibbonStyle);
    ribbonBar()->setMinimizationEnabled(true);
    ribbonBar()->setTitleBarVisible(false);
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

    return true;
}

void JFrameWnd::updateSplashInfo(const QString &info)
{
    q_splashScreen->showMessage(info, Qt::AlignLeft | Qt::AlignBottom, Qt::blue);
}

void JFrameWnd::setCurrentWidget(QWidget *widget)
{
    q_centralWidget->setCurrentWidget(widget);
}

void JFrameWnd::setTheme(const char *theme)
{
    //
    QtRibbon::RibbonStyle *ribbonStyle =
            qobject_cast<QtRibbon::RibbonStyle *>(QApplication::style());
    if (!ribbonStyle) {
        ribbonStyle = new QtRibbon::RibbonStyle;
        QApplication::setStyle(ribbonStyle);
    }

    // 解析主题信息
    QtRibbon::RibbonStyle::Theme nTheme;
    if (strcmp(theme, J_FRAME_THEME_OFFICE_2007BLUE) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2007Blue;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2007BLACK) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2007Black;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2007SILVER) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2007Silver;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2007AQUA) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2007Aqua;
    } else if (strcmp(theme, J_FRAME_THEME_WINDOWs7_SCENIC) == 0) {
        nTheme = QtRibbon::RibbonStyle::Windows7Scenic;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2010SILVER) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2010Silver;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2010BLUE) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2010Blue;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2010BLACK) == 0) {
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
}

QStackedWidget *JFrameWnd::stackedWidget()
{
    return q_centralWidget;
}

void JFrameWnd::onStyleChanged(QAction *action)
{
    setTheme(action->objectName().toLocal8Bit().data());
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

    // 获取MainWindow节点
    QDomElement emMainWindow = emRoot.firstChildElement("mainWindow");
    if (emMainWindow.isNull()) {
        return false;
    }

    // windowTitle
    if (emMainWindow.hasAttribute("windowTitle")) {
        setWindowTitle(emMainWindow.attribute("windowTitle"));
    }

    // windowIcon
    if (emMainWindow.hasAttribute("windowIcon")) {
        setWindowIcon(QIcon(emMainWindow.attribute("windowIcon")
                            .replace("@ConfigDir@", QString::fromStdString(
                                         jframeFacade()->frameConfigPath()))));
        //qApp->setWindowIcon(windowIcon());
    }

    // windowTheme
    if (emMainWindow.hasAttribute("windowTheme")) {
        setTheme(emMainWindow.attribute("windowTheme").toStdString().c_str());
    }

    // imageSplash
    if (emMainWindow.hasAttribute("imageSplash")) {
        q_pixmapSplash = emMainWindow.attribute("imageSplash")
                .replace("@ConfigDir@", QString::fromStdString(
                             jframeFacade()->frameConfigPath()));
    }

    // imageFinish
    if (emMainWindow.hasAttribute("imageFinish")) {
        q_pixmapFinish = emMainWindow.attribute("imageFinish")
                .replace("@ConfigDir@", QString::fromStdString(
                             jframeFacade()->frameConfigPath()));
    }

    // systemFont
    if (emMainWindow.hasAttribute("systemFont")) {
        const QString systemFont = emMainWindow.attribute("systemFont");
        qApp->setFont(QFont(systemFont.section(',', 0, 0, QString::SectionSkipEmpty),
                            systemFont.section(',', 1).toInt()));
    }

    /// for ribbon
    //
    q_splashScreen->setPixmap(q_pixmapSplash);
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
    setStyleSheet(QString("%1#%2{backbground:'#6E7E93';}")
                  .arg(metaObject()->className()).arg(objectName()));

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

QSplashScreen *JFrameWnd::splashScreen()
{
    return q_splashScreen;
}
