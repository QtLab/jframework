#ifndef JMAIN_WINDOW_H
#define JMAIN_WINDOW_H

#include "../jframe_core.h"

class JAttempter;
class JFrameWnd;

class JMainWindow : public IJMainWindow
{
public:
    explicit JMainWindow(JAttempter *attempter);
    ~JMainWindow();

    // IJMainWindow interface
public:
    bool initialize();
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
    void createComponentUi(IJComponent *component, const char *xmlName);
    void *mainWindowHandle();

private:
    void saveWindowState();
    void restoreWindowState();

private:
    JAttempter *q_attempter;
    JFrameWnd *q_frameWnd;
};

#endif // JMAIN_WINDOW_H
