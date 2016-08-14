#ifndef JMAIN_WINDOW_H
#define JMAIN_WINDOW_H

#include "../../jframe_core.h"
#include <QHash>

class JAttempter;
class JFrameWnd;
class QDomElement;

class JMainWindow : public IJMainWindow
{
public:
    explicit JMainWindow(JAttempter *attempter);
    ~JMainWindow();

    bool init();

    void startSplash();
    void finishSplash();
    void updateSplash();

    // IJUnknown interface
public:
    void releaseInterface();
    void *queryInterface(const char *iid, unsigned int ver);

    // IJObject interface
public:
    std::string objectIdentity() const;
    unsigned int objectVersion() const;
    bool invoke(const char *method, int argc);

    // IJMainWindow interface
public:
    void showNormal();
    void showMinimized();
    void showMaximized();
    void showFullScreen();
    void closeWindow();
    void setVisible(bool visible);
    void showStaysOnTop(bool stayOnTop);
    void resize(int width, int height);
    void *queryObject(const char *objectName);
    void *statusBar();
    void activeView(const char *viewName);
    void updateSplashInfo(const char *info);
    bool createComponentUi(IJComponent *component, const char *filePath);
    void *mainWidget();
    void setTheme(const char *theme);

private:
    void saveWindowState();
    void restoreWindowState();

    //
    bool createRibbonPage(const QDomElement &emRibbonPage);

    //
    bool createComponentUi(IJComponent *component, const QDomElement &emWindow);

private:
    JAttempter *q_attempter;
    JFrameWnd *q_frameWnd;
    QHash<QString, QObject*> q_hashObject;
};

#endif // JMAIN_WINDOW_H
