#ifndef JFRAME_LAYOUT_P_H
#define JFRAME_LAYOUT_P_H

#include "../jframe_layout.h"
#include <QMutex>
#include "jframe_login.h"
#include "IGF_Base.h"

// struct JComponentInfo

struct JComponentInfo
{
    bool firstAttach;           // 首次挂载标志
    bool attached;              // 挂载状态（由当前系统和模式决定）
    bool stayOn;                // 始终未挂载状态标志（主要用于标识框架全局有效组件）
    bool isView;                // 标识组件窗口是否为视图窗口（用于splitter布局）
    bool active;                // 组件激活状态（用户系统、模式切换时的组件激活标志）
    IUnknownEx *iface;          // 组件接口
    QWidget *widget;            // 组件窗口

    JComponentInfo():
        firstAttach(true),
        attached(false),
        stayOn(false),
        isView(false),
        active(false),
        iface(0),
        widget(0)
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

        firstAttach = other.firstAttach;
        attached = other.attached;
        stayOn = other.stayOn;
        isView = other.isView;
        active = other.active;
        iface = other.iface;
        widget = other.widget;

        return *this;
    }
};

// class JFrameLayout

struct JFrameLayoutData;
class LayoutManager;
class MainViewManager;
class ModuleManager;
class NotifyManager;
class IGF_Component;

class JFrameLayout : public IJFrameLayout
{
    // single instance
public:
    static JFrameLayout* getInstance();
    static void releaseInstance();

    // IJUnknown interface
public:
    void releaseInterface();
    void *queryInterface(const char *iid, unsigned int ver);

    // IJObject interface
public:
    std::string objectIdentity() const;
    unsigned int objectVersion() const;
    bool invoke(const char *method, int argc, ...);

    // IJFrameLayout interface
public:
    QWidget *mainWindow();
    INotifier *notifier();
    IGF_Attempter *gAttempter();
    void setFrameTheme(const char *theme);

    bool attachComponent(IGF_Component *component, bool stayOn);
    bool detachComponent(IGF_Component *component);
    bool attachComponentUi(IGF_Component *component, QWidget *widget);
    std::list<IGF_Component *> attachedComponent() const;

    int componentPowerLevel(const char *componentId) const;

    std::string currentSystem() const;
    std::string currentModule() const;

private:
    LayoutManager *layoutManager();
    MainViewManager *mainViewManager();
    ModuleManager *moduleManager();
    NotifyManager *notifyManager();

    // 用户、模块权限信息
    //const JFrameLoginSpace::UserElement &userInfo() const;
    //const JFrameLoginSpace::stPowerInfo &powerInfo() const;
    //const std::map<std::string, JFrameLoginSpace::stPowerElement> &mapPower() const;

private:
    int runQApp(void *mfcApp);
    long windowHandle(void *window, const std::string &winType);

    //
    bool loadDefaultSystem();

    //
    bool loadLoginInfo();

private:
    //
    bool invokeShowFrame(int argc, va_list ap);

private:
    JFrameLayout();
    ~JFrameLayout();

private:
    JFrameLayoutData *data;

    //
    static QMutex _instance_mutex;      // 单实例保护锁
    static JFrameLayout *_instance;     // 单实例指针

    //
    friend class LayoutManager;
    friend class MainViewManager;
    friend class ModuleManager;
    friend class NotifyManager;
};

#endif // JFRAME_LAYOUT_P_H
