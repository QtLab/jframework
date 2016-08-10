#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <QWidget>
#include "kernel/jnotifier.h"
#include "../jframe_layout_p.h"

// class ModuleManager

class IGF_Component;

class ModuleManager : public JObserver
{
public:
    explicit ModuleManager(JFrameLayout *frameLayout);
    ~ModuleManager();

    // 初始化模块
    bool init();

    // 加载框架系统
    bool loadSystem();

    // 获取组件信息
    JComponentInfo *componentById(const QString &componentId);

    // 检测组件挂载状态
    bool isComponentAttached(const JComponentInfo *componentInfo);

    // 挂载组件
    bool attachComponent(const JComponentInfo *componentInfo, bool show);

    // 挂载组件界面
    bool attachComponentUi(const JComponentInfo *componentInfo);

    // 分离组件
    bool detachComponent(const JComponentInfo *componentInfo);

    // 将所有视图窗口置为未激活状态
    void inactivateAllComponent();

    // 将所有处于未激活状态的组件分离
    void detachAllInactivateComponent();

    //
    bool attachComponent(IGF_Component *component, bool stayOn);
    JComponentInfo *attachComponent(IGF_Component *component);
    bool detachComponent(IGF_Component *component);
    bool attachComponentUi(IGF_Component *component, QWidget *widget);
    std::list<IGF_Component *> attachedComponents() const;

    // 获取当前系统名称
    std::string currentSystem() const;

    // 获取当前模式名称
    std::string currentModule() const;

    // JObserver interface
public:
    virtual std::string jobserverId() const;

protected:
    // layout
    JLRESULT onPreviousSystemChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onPreciousModuleChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onLoadDefaultSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    // system
    JLRESULT onSwitchSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onSwitchModule(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onCurrentSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onCurrentModule(const std::string &id, JWPARAM wParam, JLPARAM lParam);

private:
    bool switchSystem(const std::string &system, JLPARAM lParam = 0);
    bool switchModule(const std::string &module, JLPARAM lParam = 0);

private:
    JFrameLayout *q_frameLayout;
    INotifier *q_notifier;
    std::string q_currentSystem;
    std::string q_currentModule;
    QMap<QString/*componentId*/, JComponentInfo> q_mapComponentInfo;
};

#endif // MODULE_MANAGER_H
