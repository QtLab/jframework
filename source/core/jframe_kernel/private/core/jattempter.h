#ifndef JATTEMPTER_H
#define JATTEMPTER_H

#include "../../jframe_core.h"
#include <QMap>

/**
 * @brief The JComponentInfo struct
 */
struct JComponentInfo
{
    QString componentDir;
    QString componentName;
    QString componentDesc;
    IJComponent *component;

    JComponentInfo()
        : componentDir("")
        , componentName("")
        , componentDesc("")
        , component(0)
    {

    }

    JComponentInfo(const JComponentInfo &other)
    {
        *this = other;
    }

    JComponentInfo &operator =(const JComponentInfo &other)
    {
        if (this == &other) {
            return *this;
        }

        componentDir = other.componentDir;
        componentName = other.componentName;
        componentDesc = other.componentDesc;
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

    bool init();

    // IJUnknown interface
public:
    void releaseInterface();
    void *queryInterface(const char *iid, unsigned int ver);

    // IJObject interface
public:
    std::string objectIdentity() const;
    unsigned int objectVersion() const;
    bool invoke(const char *method, int argc);

    // IJAttempter interface
public:
    bool loadComponent();
    void shutdownComponent();
    IJComponent *queryComponent(const char *componentId);
    IJMainWindow *mainWindow();
    void *queryInterface(const char *componentId, const char *iid, unsigned int ver);
    std::list<IJComponent *> allComponents() const;
    int currentWorkModeId() const;
    const char *currentWorkModeName() const;
    const char *currentWorkModeConfigDirName() const;

private:
    bool loadInitComponent();
    bool loadAllComponent();
    bool shutdownAllComponent();
    bool loadComponent(const QString &componentDir,
                       const QString &componentName,
                       const QString &componentDesc);

    bool loadConfig();

    //
    void commandSink(void *sender, const char *senderName);

private:
    friend class JFrameWnd;

    IJMainWindow *q_mainWindow;
    QMap<QString, JComponentInfo> q_mapComponent;

    // workMode
    int q_workModeId;
    std::string q_workModeName;
    std::string q_workModeDir;
};

#endif // JATTEMPTER_H
