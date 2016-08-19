#ifndef JATTEMPTER_H
#define JATTEMPTER_H

#include "../../jframe_core.h"
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
    int currentWorkModeId() const;
    const char *currentWorkModeName() const;
    const char *currentWorkModeConfigDirName() const;

    void subMessage(IJComponent *component, unsigned int id);
    void unsubMessage(IJComponent *component, unsigned int id);
    void sendMessage(IJComponent *component, unsigned int id, JWPARAM wParam, JLPARAM lParam);
    void postMessage(IJComponent *component, unsigned int id, JWPARAM wParam, JLPARAM lParam);

    INotifier *notifier();

private:
    bool loadConfig();
    bool loadInitComponent();
    bool loadAllComponent();
    bool loadComponent(JComponentConfig &componentConfig);
    bool releaseAllComponent();

    //
    void commandSink(void *sender, const std::string &senderName);

private:
    friend class JFrameWnd;

    INotifier *q_notifier;      // 消息分发器实例

    IJMainWindow *q_mainWindow;
    QMap<QString/*componentName*/, JComponentConfig> q_mapComponent;

    // workMode
    int q_workModeId;
    std::string q_workModeName;
    std::string q_workModeDir;
};

#endif // JATTEMPTER_H
