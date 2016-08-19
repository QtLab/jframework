#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include "factory/jnotifier.h"
#include "../jframe_layout_p.h"

// class ModuleManager

class IGF_Component;

class ModuleManager :
        public IJUnknown,
        public JObserver
{
public:
    explicit ModuleManager(JFrameLayout *frameLayout);
    ~ModuleManager();

    // IJUnknown interface
public:
    bool loadInterface();
    void releaseInterface();

public:
    // 加载框架系统
    bool loadSystem();

    // 获取组件信息
    JComponentInfo *componentByName(const QString &componentName);

    // 检测组件挂载状态
    bool isComponentAttached(const JComponentInfo *componentInfo);

    // 挂载组件
    bool attachComponent(JComponentInfo *componentInfo, bool show);

    // 挂载组件界面
    bool attachComponentUi(const JComponentInfo *componentInfo);

    // 分离组件
    bool detachComponent(JComponentInfo *componentInfo);

    // 将所有视图窗口置为未激活状态
    void inactivateAllComponent();

    // 将所有处于未激活状态的组件分离
    void detachAllInactivateComponent();

    // 复位所有处于未激活状态的视图组件的父窗口
    void resetAllInactivateViewComponent();

    // 显示所有处于激活状态的视图组件的父窗口
    void showAllActivateViewComponent();

    //
    bool attachComponent(IJComponent *component, bool stayOn);
    JComponentInfo *attachComponent(IJComponent *component);
    bool detachComponent(IJComponent *component);
    bool attachComponentUi(IJComponent *component, QWidget *widget);

    // 获取当前系统名称
    std::string currentSystem() const;

    // 获取当前模式名称
    std::string currentModule() const;

    // JObserver interface
public:
    virtual std::string observerId() const;

protected:
    // layout
    JLRESULT onPreviousSystemChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onPreviousModuleChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onLoadDefaultSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    // system
    JLRESULT onSwitchSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onSwitchModule(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onCurrentSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onCurrentModule(const std::string &id, JWPARAM wParam, JLPARAM lParam);

private:
    bool switchSystem(const std::string &system, JLPARAM lParam = 0);
    bool switchModule(const std::string &module, JLPARAM lParam = 0);

    // 分离所有组件
    void detachAllComponent();

private:
    JFrameLayout *q_frameLayout;
    INotifier *q_notifier;
    std::string q_currentSystem;
    std::string q_currentModule;
    QMap<QString/*componentName*/, JComponentInfo> q_mapComponentInfo;
};

#endif // MODULE_MANAGER_H
