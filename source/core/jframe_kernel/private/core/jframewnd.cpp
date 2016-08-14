#include "precomp.h"
#include "jframewnd.h"
#include "jattempter.h"

JFrameWnd::JFrameWnd(JAttempter *attempter, QWidget *parent, Qt::WindowFlags f)
    : QtRibbon::RibbonMainWindow(parent, f)
    , q_attempter(attempter)
    , q_splashScreen(0)
    , q_centreWidget(0)
{
    setObjectName("JFrameWnd");

    //
    resize(1024, 700);

    //
    q_splashScreen = new QSplashScreen(this);
    q_centreWidget = new QStackedWidget(this);
    setCentralWidget(q_centreWidget);

    //
    QApplication::setStyle(new QtRibbon::RibbonStyle);
    ribbonBar()->setMinimizationEnabled(false);
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
    q_centreWidget->setCurrentWidget(widget);
}

QObject *JFrameWnd::queryObject(const QString &objectName)
{
    QHash<QString, QObject*>::const_iterator citer = q_hashObject.find(objectName);
    if (citer != q_hashObject.end()) {
        return citer.value();
    } else {
        return 0;
    }
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

void JFrameWnd::onStyleChanged(QAction *action)
{
    setTheme(action->objectName().toLocal8Bit().data());
}

bool JFrameWnd::loadConfig()
{
    //
    q_pixmapSplash = QPixmap(QString::fromStdString(
                                 jframeFacade()->frameConfigPath()
                                 .append("/resource/splash.png")));
    q_pixmapFinish = QPixmap(QString::fromStdString(
                                 jframeFacade()->frameConfigPath()
                                 .append("/resource/finish.png")));

    //
    q_splashScreen->setPixmap(q_pixmapSplash);

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
