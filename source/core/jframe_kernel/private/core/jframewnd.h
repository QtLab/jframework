#ifndef JFRAMEWND_H
#define JFRAMEWND_H

#include "QtnRibbonMainWindow.h"
#include "QtnRibbonStyle.h"
#include "jattempter.h"
#include "QtnRibbonBar.h"
#include "QtnRibbonPage.h"

class QSplashScreen;
class QStackedWidget;
class QToolBar;
class JAttempter;

class JFrameWnd : public QtRibbon::RibbonMainWindow
{
    Q_OBJECT
public:
    explicit JFrameWnd(JAttempter *attempter, QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~JFrameWnd();

    bool init();

    void updateSplashInfo(const QString &info);

    void setCurrentWidget(QWidget *widget);

    void setTheme(const QString &theme);

    QStackedWidget *stackedWidget();

Q_SIGNALS:

public Q_SLOTS:
    void onStyleChanged(QAction *action);

    // QtRibbon::RibbonPage
    void onCurrentPageChanged(int page);

    // QAction
    void onActionTriggered(bool checked);

    // QComboBox
    void onComboBoxCurrentIndexChanged(int index);

    // QCheckBox
    void onCheckBoxStateChanged(int state);

    // QRadioButton
    void onRadioButtonClicked(bool checked);

private:
    bool testAnotherApp();
    bool loadConfig();

    int ribbonStyleTheme() const;

    bool createOptionAction();

    void startSplash();
    void finishSplash();
    void updateSplash();

private:
    friend class JMainWindow;
    JAttempter *q_attempter;

    // 保存界面元素
    QSplashScreen *q_splashScreen;
    QStackedWidget *q_centralWidget;
    QList<QToolBar *> q_toolBars;
    QPixmap q_pixmapSplash;
    QPixmap q_pixmapFinish;
    QFont q_ribbonBarFont;
};

#endif // JFRAMEWND_H
