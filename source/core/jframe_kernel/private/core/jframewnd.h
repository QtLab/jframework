#ifndef JFRAMEWND_H
#define JFRAMEWND_H

#include "QtnRibbonMainWindow.h"
#include "QtnRibbonStyle.h"

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

private:
    bool testAnotherApp();
    bool loadConfig();

    int ribbonStyleTheme() const;

    bool createOptionAction();

    QSplashScreen *splashScreen();

private:
    friend class JMainWindow;
    JAttempter *q_attempter;

    // �������Ԫ��
    QSplashScreen *q_splashScreen;
    QStackedWidget *q_centralWidget;
    QList<QToolBar *> q_toolBars;
    QPixmap q_pixmapSplash;
    QPixmap q_pixmapFinish;
};

#endif // JFRAMEWND_H