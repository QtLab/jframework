#ifndef JMAIN_WINDOW_H
#define JMAIN_WINDOW_H

#include "../../jframe_core.h"
#include <QHash>
#include "QtnRibbonBar.h"
#include "QtnRibbonPage.h"
#include "QtnRibbonGroup.h"

// 主窗口配置信息
struct JMainWindowConfig
{
    std::string toolBarType;    // 1) menuBar; 2) toolButton; 3) ribbonBar
    std::string layoutType;     // 1) splitter; 2) dock

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
class QMenu;
class QComboBox;
class QCheckBox;
class QRadioButton;
class QLabel;

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
    void activeView(const std::string &viewName);
    void updateSplashInfo(const std::string &info);
    bool createComponentUi(IJComponent *component, const std::string &filePath);
    void *mainWidget();
    void setTheme(const std::string &theme);
    std::string toolBarType() const;
    std::string layoutType() const;
    void *queryObject(const std::string &objectName, const std::string &componentName = "");
    void *menuBar();
    void *ribbonBar();
    void *statusBar();

public:
    void startSplash();
    void finishSplash();
    void updateSplash();

private:
    bool loadConfig();
    void saveWindowState();
    void restoreWindowState();

    //
    bool createComponentUi(IJComponent *component, const QDomElement &emWindow);

    //
    bool createRibbonPage(const QDomElement &emRibbon, const QString &componentName);

    //
    bool createRibbinGroup(const QDomElement &emPage, QtRibbon::RibbonPage* ribbonPage, const QString &componentName);

    //
    bool createRibbinItem(const QDomElement &emGroup, QtRibbon::RibbonGroup *ribbonGroup, const QString &componentName);

    //
    QAction *createItemAction(const QDomElement &emItem, const QString &componentName);
    //
    QComboBox *createItemComboBox(const QDomElement &emItem, const QString &componentName);
    //
    QCheckBox *createItemCheckBox(const QDomElement &emItem, const QString &componentName);
    //
    QRadioButton *createItemRadioButton(const QDomElement &emItem, const QString &componentName);
    //
    QLabel *createItemLabel(const QDomElement &emItem, const QString &componentName);
    //
    QMenu *createSubMenu(const QDomElement &emItem, QWidget *parentWidget, const QString &componentName);
    //
    bool createMenuItem(const QDomElement &emItem, QMenu *menu, const QString &componentName);

    //
    QString parsePath(const QString &src) const;

    //
    bool insertHashObject(const QString &componentName, const QString &objectName, QObject *object);

    //
    template<typename T>
    T *createHashObject(const QString &componentName, const QString &objectName,
                        QWidget *receiver = 0, const char *signal = 0, const char *slot = 0);

    bool checkObjectName(const QDomElement &element);

    //
    QtRibbon::ContextColor parseContextColor(const QString &text);

private:
    JAttempter *q_attempter;
    JFrameWnd *q_frameWnd;
    QHash<QString/*componentName*/, QHash<QString/*objectName*/, QObject*> > q_hashObject;
    JMainWindowConfig q_mainWindowConfig;
};

template<typename T>
T *JMainWindow::createHashObject(const QString &componentName, const QString &objectName,
                                 QWidget *receiver, const char *signal, const char *slot)
{
    //
    if (componentName.isEmpty() || objectName.isEmpty()) {
        Q_ASSERT(false);
        return 0;   //
    }

    //
    T *object = 0;
    QHash<QString, QHash<QString, QObject *> >::iterator iterHash =
            q_hashObject.find(componentName);
    if (iterHash != q_hashObject.end()) {
        QHash<QString, QObject *>::iterator iterObject =
                iterHash.value().find(objectName);
        if (iterObject != iterHash.value().end()) {
            qWarning() << QStringLiteral("objectName[%1] has been exists in component[%2]!"
                                         " ignore it!")
                          .arg(componentName).arg(objectName);
#if 0
            // replace
            object = qobject_cast<T *>(iterObject.value());
            if (object) {
                if (receiver && signal && slot) {
                    QObject::disconnect(object, signal, receiver, slot);
                }
            } else {
                iterObject.value()->deleteLater();  // destory old object
                object = new T();
                iterHash.value().insert(objectName, object);
            }
#else
            Q_UNUSED(receiver);
            Q_UNUSED(signal);
            Q_UNUSED(slot);
            return 0;   // ignore
#endif
        } else {
            object = new T();
            iterHash.value().insert(objectName, object);
        }
    } else {
        QHash<QString, QObject *> hash;
        object = new T();
        hash.insert(objectName, object);
        q_hashObject.insert(componentName, hash);
    }

    return object;
}

#endif // JMAIN_WINDOW_H
