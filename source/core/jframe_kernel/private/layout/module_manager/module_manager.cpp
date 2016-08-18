#include "precomp.h"
#include "module_manager.h"
#include "../jframe_layout_p.h"
#include "../mainview_manager/mainview_manager.h"
#include "../layout_manager/layout_manager.h"
#include "../../../jframe_core.h"

// class ModuleManager

ModuleManager::ModuleManager(JFrameLayout *frameLayout)
    : q_frameLayout(frameLayout)
    , q_notifier(frameLayout->notifier())
{
}

ModuleManager::~ModuleManager()
{

}

bool ModuleManager::loadInterface()
{
    // 订阅消息
    q_notifier->beginGroup(this)
            // layout
            .append("j_previous_system_changed", &ModuleManager::onPreviousSystemChanged)
            .append("j_previous_module_changed", &ModuleManager::onPreviousModuleChanged)
            .append("j_load_default_system", &ModuleManager::onLoadDefaultSystem)
            // system
            .append("j_switch_system", &ModuleManager::onSwitchSystem)
            .append("j_switch_module", &ModuleManager::onSwitchModule)
            .append("j_current_system", &ModuleManager::onCurrentSystem)
            .append("j_current_module", &ModuleManager::onCurrentModule)
            .endGroup();

    return true;
}

void ModuleManager::releaseInterface()
{
    // 分离所有组件
    detachAllComponent();

    // 取消订阅
    q_notifier->remove(this);
}

bool ModuleManager::loadSystem()
{
    // 更新默认布局（软件启动初始化）
    if (!q_frameLayout->layoutManager()->updateLayout()) {
        return false;   // 更新失败
    }

    // 异步通知模式已经切换
    q_notifier->postMessage("j_module_changed", q_currentModule);

    return true;
}

JComponentInfo *ModuleManager::componentByName(const QString &componentName)
{
    QMap<QString, JComponentInfo>::iterator iter =
            q_mapComponentInfo.find(componentName);
    if (iter != q_mapComponentInfo.end()) {
        return &iter.value();
    }

    return 0;   // 未找到
}

bool ModuleManager::isComponentAttached(const JComponentInfo *componentInfo)
{
    if (!componentInfo) {
        return false;
    }

    return componentInfo->attached;
}

bool ModuleManager::attachComponent(JComponentInfo *componentInfo, bool show)
{
    //
    if (!componentInfo) {
        return false;
    }

    // 查询组件是否已挂载
    if (componentInfo->attached) {
        return true;
    }

    //
    IJComponent *component = dynamic_cast<IJComponent *>(componentInfo->iface);
    if (!component) {
        return false;   //
    }

    // 挂载组件
    component->attach();

    // 标识已挂载状态
    componentInfo->attached = true;

    // 更新组件
    component->updateInterface();

    // 如果窗口有效，则显示窗口
    if (componentInfo->widget && show) {
        componentInfo->widget->show();
    }

    return true;
}

bool ModuleManager::attachComponentUi(const JComponentInfo *componentInfo)
{
    if (!componentInfo) {
        return false;
    }

    // 查询组件界面是否已挂载
    if (componentInfo->widget) {
        return true;
    }

    // 首次加载，须创建界面（如果存在界面接口）
    IJComponentUi *componentUi = (IJComponentUi *)
            (dynamic_cast<IJComponent *>(componentInfo->iface)
             ->queryInterface(IID_IJComponentUi, VER_IJComponentUi));
    if (componentUi) {
        QWidget *widget = (QWidget *)componentUi->createWindow(q_frameLayout->mainViewManager(), "");
        if (widget) {
            attachComponentUi(dynamic_cast<IJComponent *>(componentInfo->iface), widget);
        }
    }

    return true;
}

bool ModuleManager::detachComponent(JComponentInfo *componentInfo)
{
    if (!componentInfo) {
        return false;
    }

    // 查询组件是否已分离
    if (!componentInfo->attached) {
        return true;
    }

    //
    IJComponent *component = dynamic_cast<IJComponent *>(componentInfo->iface);
    if (!component) {
        return false;
    }

    // 分离组件
    component->detach();

    // 标识已分离状态
    componentInfo->attached = false;

    return true;
}

void ModuleManager::inactivateAllComponent()
{
    QMutableMapIterator<QString, JComponentInfo> iter(q_mapComponentInfo);
    while (iter.hasNext()) {
        iter.next();
        JComponentInfo &componentInfo = iter.value();
        if (!componentInfo.stayOn) {
            //
            componentInfo.active = false;
            //
            if (componentInfo.widget && !componentInfo.isView
                    && componentInfo.widget->isVisible()) {
                componentInfo.widget->hide();
            }
        }
    }
}

void ModuleManager::detachAllInactivateComponent()
{
    QMutableMapIterator<QString, JComponentInfo> iter(q_mapComponentInfo);
    while (iter.hasNext()) {
        iter.next();
        JComponentInfo &componentInfo = iter.value();
        if (!componentInfo.active) {
            detachComponent(&componentInfo);
        }
    }
}

void ModuleManager::resetAllInactivateViewComponent()
{
    QMutableMapIterator<QString, JComponentInfo> iter(q_mapComponentInfo);
    while (iter.hasNext()) {
        iter.next();
        JComponentInfo &componentInfo = iter.value();
        if (!componentInfo.stayOn
                && !componentInfo.active
                && componentInfo.isView
                && componentInfo.widget) {
            componentInfo.widget->setParent(q_frameLayout->mainWindow());
        }
    }
}

void ModuleManager::showAllActivateViewComponent()
{
    QMutableMapIterator<QString, JComponentInfo> iter(q_mapComponentInfo);
    while (iter.hasNext()) {
        iter.next();
        JComponentInfo &componentInfo = iter.value();
        if (!componentInfo.stayOn
                && componentInfo.active
                && componentInfo.isView
                && componentInfo.widget) {
            componentInfo.widget->show();
        }
    }
}

bool ModuleManager::attachComponent(IJComponent *component, bool stayOn)
{
    JComponentInfo *componentInfo = attachComponent(component);
    if (!componentInfo) {
        return false;
    }

    //
    componentInfo->stayOn = stayOn;

    //
    if (componentInfo->stayOn
            && !componentInfo->attached) {
        // 挂载组件
        component->attach();
        // 设置状态
        componentInfo->attached = true;     // 设置为挂载状态
        componentInfo->active = true;       // 激活组件
    }

    return true;
}

JComponentInfo *ModuleManager::attachComponent(IJComponent *component)
{
    // 参数检测
    if (!component) {
        return 0;
    }

    //
    JComponentInfo *componentInfo = 0;

    // 设置（或更新）组件挂载信息
    const QString sComponentName = QString::fromStdString(component->componentName());
    QMap<QString, JComponentInfo>::iterator iterComponentInfo =
            q_mapComponentInfo.find(sComponentName);
    if (iterComponentInfo == q_mapComponentInfo.end()) {
        // 如果组件还没有挂载过，则挂载新的组件信息（并且，初次挂载设置active为未激活状态，即false）
        JComponentInfo info;
        info.iface = component;
        //
        q_mapComponentInfo.insert(sComponentName, info);
        //
        componentInfo = &q_mapComponentInfo[sComponentName];
    } else {
        // 更新组件挂载信息
        componentInfo = &iterComponentInfo.value();
        //
        if (!componentInfo->stayOn && componentInfo->attached) {
            Q_ASSERT(false);    // 不允许的调用状态
            return 0;           // 同意个组件不能重复挂载，可能会引起客户程序资源重复多次初始化导出内存泄漏等问题
        }
        //
        if (componentInfo->firstAttach) {
            componentInfo->firstAttach = false;
        } else {
            componentInfo->attached = true;     // 设置为挂载状态
        }
        // 设置激活标识
        componentInfo->active = true;           // 激活组件
    }

    return componentInfo;
}

bool ModuleManager::detachComponent(IJComponent *component)
{
    // 参数检测
    if (!component) {
        return false;
    }

    // 组件挂载信息有效性检测
    QMap<QString, JComponentInfo>::iterator iterComponentInfo =
            q_mapComponentInfo.find(QString::fromStdString(component->componentName()));
    if (iterComponentInfo == q_mapComponentInfo.end()) {
        return false;
    }

    // 设置（更新）组件挂载信息
    JComponentInfo &componentInfo = iterComponentInfo.value();
    // 设置为分离状态
    if (componentInfo.attached) {
        component->detach();    // 分离
        componentInfo.attached = false;
    }
    // 设置激活标识
    componentInfo.active = false;       // 取消激活组件
    // 如果窗口有效，则设置为不可见
    if (componentInfo.widget) {
        componentInfo.widget->hide();
    }

    return true;
}

bool ModuleManager::attachComponentUi(IJComponent *component, QWidget *widget)
{
    // 参数检测
    if (!component || !widget) {
        return false;
    }

    // 组件挂载信息有效性检测
    const QString sComponentName = QString::fromStdString(component->componentName());
    QMap<QString, JComponentInfo>::iterator iterComponentInfo =
            q_mapComponentInfo.find(sComponentName);
    if (iterComponentInfo == q_mapComponentInfo.end()) {
        // 如果组件还没有挂载过，则挂载新的组件信息（这种情况属于框架调度器创建自动组件，
        // 应该设置为激活状态，并且始终显示）
        JComponentInfo componentInfo;
        componentInfo.iface = component;
        // 存储窗口实例
        componentInfo.widget = widget;
        // 设置为始终挂载
        componentInfo.stayOn = true;
        // 增加到映射队列
        q_mapComponentInfo.insert(sComponentName, componentInfo);
    } else {
        JComponentInfo &componentInfo = iterComponentInfo.value();
        if (componentInfo.widget) {
            Q_ASSERT(false);    // 不允许的调用状态
            return false;       // 已经创建的当前组件窗口实例，视为错误操作
        }
        // 存储窗口实例
        componentInfo.widget = widget;
        // 初次设置为分离状态
        componentInfo.attached = false;
        // 初次设置为不可见
        //componentInfo.widget->hide();
    }

    return true;
}

std::string ModuleManager::currentSystem() const
{
    return q_currentSystem;
}

std::string ModuleManager::currentModule() const
{
    return q_currentModule;
}

std::string ModuleManager::observerId() const
{
    return "jlayout.module_manager";
}

JLRESULT ModuleManager::onPreviousSystemChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(lParam);

    // 还原参数
    std::string *pSystem = (std::string *)wParam;
    if (!pSystem) {
        return -1;
    }

    // 更新系统
    q_currentSystem = *pSystem;

    return 0;
}

JLRESULT ModuleManager::onPreviousModuleChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(lParam);

    // 还原参数
    std::string *pModule = (std::string *)wParam;
    if (!pModule) {
        return -1;
    }

    // 更新模式
    q_currentModule = *pModule;

    return 0;
}

JLRESULT ModuleManager::onLoadDefaultSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    // 加载登录信息
    if (!q_frameLayout->loadLoginInfo()) {
        return -1;
    }

    // 加载系统
    if (!loadSystem()) {
        return -1;
    }

    return 0;
}

JLRESULT ModuleManager::onSwitchSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);

    //
    if (jloginManager()->isAdminUser()) {
        return 0;   // 如果是管理员登录，则忽略系统切换
    }

    //
    std::string *pCurrentSystem = (std::string *)wParam;
    if (!pCurrentSystem) {
        return -1;
    }

    // 切换系统
    switchSystem(*pCurrentSystem, lParam);

    return 0;
}

JLRESULT ModuleManager::onSwitchModule(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);

    //
    if (jloginManager()->isAdminUser()) {
        return 0;   // 如果是管理员登录，则忽略系统切换
    }

    //
    std::string *pCurrentModule = (std::string *)wParam;
    if (!pCurrentModule) {
        return -1;
    }

    // 切换系统
    switchModule(*pCurrentModule, lParam);

    return 0;
}

JLRESULT ModuleManager::onCurrentSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(lParam);

    //
    std::string *pCurrentSystem = (std::string *)wParam;
    if (!pCurrentSystem) {
        return -1;
    }

    //
    *pCurrentSystem = q_currentSystem;

    return 0;
}

JLRESULT ModuleManager::onCurrentModule(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(lParam);

    //
    std::string *pCurrentModule = (std::string *)wParam;
    if (!pCurrentModule) {
        return -1;
    }

    //
    *pCurrentModule = q_currentModule;

    return 0;
}

bool ModuleManager::switchSystem(const std::string &system, JLPARAM lParam)
{
    // 参数检测
    if (system.empty()) {
        jframeLogCrit(QStringLiteral("参数检测：系统名称不能为空值！"));
        return false;
    }

    //
    if (q_currentSystem == system) {
        return true;        // 系统没变，不需要切换
    }

    // 处理系统切换业务
    bool result = false;    // 暂不支持在线切换系统，即不允许在廉运行期间动态切换系统
    if (!result) {
        return false;       // 系统切换失败
    }

    // 异步通知系统已经切换
    q_notifier->postMessage("j_system_changed", system, lParam);

    return true;
}

bool ModuleManager::switchModule(const std::string &module, JLPARAM lParam)
{
    // 参数检测
    if (module.empty()) {
        jframeLogCrit(QStringLiteral("参数检测：模式名称不能为空值！"));
        return false;
    }

    // 参数规格化，去掉多模式之间的空格，便于相等比较
    std::string specModule = QString::fromStdString(module)
            .split(">>", QString::SkipEmptyParts).join(">>").toStdString();
    if (q_currentModule == specModule) {
        return true;    // 模式没变，不需要切换
    }

    // 处理系统切换业务
    bool result = q_frameLayout->layoutManager()
            ->updateLayout(QString::fromStdString(specModule));
    if (!result) {
        return false;       // 系统切换失败
    }

    // 异步通知模式已经切换
    q_notifier->postMessage("j_module_changed", specModule, lParam);

    return true;
}

void ModuleManager::detachAllComponent()
{
    QMutableMapIterator<QString, JComponentInfo> iter(q_mapComponentInfo);
    while (iter.hasNext()) {
        iter.next();
        JComponentInfo &componentInfo = iter.value();
        if (componentInfo.attached) {
            IJComponent *component = dynamic_cast<IJComponent *>(componentInfo.iface);
            if (component) {
                component->detach();
                componentInfo.attached = false;
            }
        }
    }
}
