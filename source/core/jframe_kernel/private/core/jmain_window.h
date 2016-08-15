#ifndef JMAIN_WINDOW_H
#define JMAIN_WINDOW_H

#include "../../jframe_core.h"
#include <QHash>

// 主窗口配置信息
struct JMainWindowConfig
{
    std::string toolBarType;    // 1) ribbon; 2) menu
    std::string layoutType;     // 1) dynamic; 2) static

    JMainWindowConfig()
    {

    }

    JMainWindowConfig(const JMainWindowConfig &other)
    {
        *this = other;
    }

    JMainWindowConfig &operator =(const JMainWindowConfig &other)
    {
        if (this == &other) {
            return *this;
        }

        toolBarType = other.toolBarType;
        layoutType = other.layoutType;

        return *this;
    }
};

// class JMainWindow

class JAttempter;
class JFrameWnd;
class QDomElement;

class JMainWindow : public IJMainWindow
{
public:
    explicit JMainWindow(JAttempter *attempter);
    ~JMainWindow();

    // IJUnknown interface
public:
    std::string interfaceIdentity() const;
    unsigned int interfaceVersion() const;
    void *queryInterface(const std::string &iid, unsigned int ver);
    bool loadInterface();
    void releaseInterface();
    std::list<std::string> queryMethod() const;
    bool invokeMethod(const std::string &method, int argc, ...);

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
    void *queryObject(const std::string &objectName);
    void *statusBar();
    void activeView(const std::string &viewName);
    void updateSplashInfo(const std::string &info);
    bool createComponentUi(IJComponent *component, const std::string &filePath);
    void *mainWidget();
    void setTheme(const std::string &theme);
    std::string toolBarType() const;
    std::string layoutType() const;

public:
    void startSplash();
    void finishSplash();
    void updateSplash();

private:
    bool loadConfig();
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
    JMainWindowConfig q_mainWindowConfig;
};

#endif // JMAIN_WINDOW_H
