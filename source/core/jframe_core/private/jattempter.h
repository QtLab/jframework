#ifndef JATTEMPTER_H
#define JATTEMPTER_H

#include "../jframe_core.h"
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

    // IJAttempter interface
public:
    bool loadInitComponent();
    bool loadAllComponent();
    bool shutdownAllComponent();
    IJComponent *queryComponent(const char *componentId);
    IJMainWindow *mainWindow();
    void *queryInterface(const char *componentId, const char *iid, unsigned int ver);
    std::list<IJComponent *> allComponents() const;
    int currentWorkModeId() const;
    const char *currentWorkModeName() const;
    const char *currentWorkModeConfigDirName() const;

private:
    bool loadComponent(const QString &componentDir,
                       const QString &componentName,
                       const QString &componentDesc);

    bool loadConfig();

    //
    void commandSink(void *sender, const char *senderName);

private:
    friend class JMainWnd;

    IJMainWindow *q_mainWindow;
    QMap<QString, JComponentInfo> q_mapComponent;

    // workMode
    int q_workModeId;
    std::string q_workModeName;
    std::string q_workModeDir;
};

#endif // JATTEMPTER_H
