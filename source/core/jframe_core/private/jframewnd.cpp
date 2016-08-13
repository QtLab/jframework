#include "precomp.h"
#include "jframewnd.h"
#include "jattempter.h"

JFrameWnd::JFrameWnd(JAttempter *attempter, QWidget *parent, Qt::WindowFlags f)
    : QtRibbon::RibbonMainWindow(parent, f)
    , q_attempter(attempter)
    , q_splashScreen(0)
    , q_centreWidget(0)
{
    //
    q_splashScreen = new QSplashScreen(this);

    //
    q_centreWidget = new QStackedWidget(this);

    //
    setCentralWidget(q_centreWidget);
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
    if (!q_attempter->loadInitComponent()) {
        return false;
    }

    //
    q_splashScreen->show();
    QApplication::processEvents();

    //
    if (!q_attempter->loadAllComponent()) {
        return false;
    }

    //
    q_splashScreen->finish(this);

    //
    if (!createOptionAction()) {
        return false;
    }

    //
    ribbonBar()->setMinimizationEnabled(false);
    //ribbonBar()->setMinimized(false);
    ribbonBar()->setTitleBarVisible(false);

    return 0;
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

void JFrameWnd::onStyleChanged(QAction *action)
{
    //
    QtRibbon::RibbonStyle *ribbonStyle =
            qobject_cast<QtRibbon::RibbonStyle *>(QApplication::style());
    if (!ribbonStyle) {
        return;
    }

    //
    QtRibbon::RibbonStyle::Theme theme = QtRibbon::RibbonStyle::Office2010Blue;
    const QString styleName = action->objectName();
    if (styleName == J_FRAME_THEME_OFFICE_2007BLUE) {
        theme = QtRibbon::RibbonStyle::Office2007Blue;
    } else if (styleName == J_FRAME_THEME_OFFICE_2007BLACK) {
        theme = QtRibbon::RibbonStyle::Office2007Black;
    }  else if (styleName == J_FRAME_THEME_OFFICE_2007SILVER) {
        theme = QtRibbon::RibbonStyle::Office2007Silver;
    }  else if (styleName == J_FRAME_THEME_OFFICE_2007AQUA) {
        theme = QtRibbon::RibbonStyle::Office2007Aqua;
    }  else if (styleName == J_FRAME_THEME_WINDOWs7_SCENIC) {
        theme = QtRibbon::RibbonStyle::Windows7Scenic;
    }  else if (styleName == J_FRAME_THEME_OFFICE_2010BLUE) {
        theme = QtRibbon::RibbonStyle::Office2010Blue;
    }  else if (styleName == J_FRAME_THEME_OFFICE_2010SILVER) {
        theme = QtRibbon::RibbonStyle::Office2010Silver;
    }  else if (styleName == J_FRAME_THEME_OFFICE_2010BLACK) {
        theme = QtRibbon::RibbonStyle::Office2010Black;
    }

    //
    QToolButton* toolButton = ribbonBar()->getSystemButton();
    if (toolButton) {
        if (theme == QtRibbon::OfficeStyle::Windows7Scenic ||
                theme == QtRibbon::OfficeStyle::Office2010Blue ||
                theme == QtRibbon::OfficeStyle::Office2010Silver ||
                theme == QtRibbon::OfficeStyle::Office2010Black) {
            toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
        } else {
            toolButton->setToolButtonStyle(Qt::ToolButtonFollowStyle);
        }
    }

    //
    ribbonStyle->setTheme(theme);
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
