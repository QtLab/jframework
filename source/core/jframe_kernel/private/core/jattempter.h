#ifndef JATTEMPTER_H
#define JATTEMPTER_H

#include "../../jframe_core.h"
#include "factory/jnotifier.h"
#include "jcomponent_notify.h"
#include <QMap>

/**
 * @brief The JComponentConfig struct
 */
struct JComponentConfig
{
    QString componentDir;       //
    QString componentName;      //
    QString componentDesc;      //
    QString componentType;      //
    bool stay;                  //
    IJComponent *component;

    JComponentConfig()
        : componentDir("")
        , componentName("")
        , componentDesc("")
        , componentType("")
        , stay(false)
        , component(0)
    {

    }

    JComponentConfig(const JComponentConfig &other)
    {
        *this = other;
    }

    JComponentConfig &operator =(const JComponentConfig &other)
    {
        if (this == &other) {
            return *this;
        }

        componentDir = other.componentDir;
        componentName = other.componentName;
        componentDesc = other.componentDesc;
        componentType = other.componentType;
        stay = other.stay;
        component = other.component;

        return *this;
    }
};

/**
 * @brief The JAttempter class
 */
class JAttempter : public IJAttempter
{
public:
    explicit JAttempter();
    ~JAttempter();

    // IJUnknown interface
public:
    std::string interfaceIdentity() const;
    unsigned int interfaceVersion() const;
    void *queryInterface(const std::string &iid, unsigned int ver);
    bool loadInterface();
    void releaseInterface();
    std::list<std::string> queryMethod() const;
    bool invokeMethod(const std::string &method, int argc, ...);

    // IJAttempter interface
public:
    bool loadComponent();
    void releaseComponent();
    IJComponent *queryComponent(const std::string &componentName);
    IJMainWindow *mainWindow();
    void *queryInterface(const std::string &componentName, const std::string &iid, unsigned int ver);
    std::list<IJComponent *> allComponents() const;
    int workModeId() const;
    std::string workModeName() const;
    std::string workModeDir() const;

    INotifier &notifier();

    void endGroup();
    IJAttempter &unsubMessage(const std::string &id);
    void unsubMessage(IJComponent *component);
    JLRESULT sendMessage(IJComponent *component, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0);
    void postMessage(IJComponent *component, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0);
    void postMessage(IJComponent *component, const std::string &id, const std::string &msg);

protected:
    IJAttempter &beginGroup(IJComponent *obs, int offset);
    IJAttempter &subMessage(const std::string &id, JMsgSinkCb cb = 0);

private:
    bool loadConfig();
    bool loadInitComponent();
    bool loadAllComponent();
    bool loadComponent(JComponentConfig &componentConfig);
    bool releaseAllComponent();

    //
    void commandSink(QObject *sender, const std::string &eventType, void *data);

private:
    friend class JFrameWnd;

    JComponentNotify componentNotify;
    INotifier *q_notifier;      // 消息分发器实例

    IJMainWindow *q_mainWindow;
    QMap<QString/*componentName*/, JComponentConfig> q_mapComponent;

    // workMode
    int q_workModeId;
    std::string q_workModeName;
    std::string q_workModeDir;
};

#endif // JATTEMPTER_H
