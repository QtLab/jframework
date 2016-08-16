#include "precomp.h"
#include "jframe_layout_p.h"
#include "factory/jnotifier.h"
#include "mainview_manager/mainview_manager.h"
#include "layout_manager/layout_manager.h"
#include "module_manager/module_manager.h"
#include "notify_manager/notify_manager.h"
#include "../../jframe_core.h"

// struct JFrameLayoutData

struct JFrameLayoutData
{
    LayoutManager *layoutManager;       // 布局管理器实例
    MainViewManager *mainViewManager;   // 框架主视图实例
    ModuleManager *moduleManager;       // 模块管理器实例
    NotifyManager *notifyManager;       // 消息管理器实例
#if 0
    // 用户、模块权限信息
    JFrameLoginSpace::UserElement userInfo;     // 登录用户信息
    JFrameLoginSpace::stPowerInfo powerInfo;    // 模块权限信息
    std::map<std::string/*组件标识*/, JFrameLoginSpace::stPowerElement> mapPower;   // 权限信息映射
#endif
    JFrameLayoutData()
        : layoutManager(0)
        , mainViewManager(0)
        , moduleManager(0)
        , notifyManager(0)
    {

    }
};

// class JFrameLayout

//
QMutex JFrameLayout::_instance_mutex;
JFrameLayout* JFrameLayout::_instance = 0;

JFrameLayout* JFrameLayout::getInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameLayout::_instance == 0) {
        JFrameLayout::_instance_mutex.lock();
        if (JFrameLayout::_instance == 0) {
            JFrameLayout* tempInstance = new JFrameLayout;
            JFrameLayout::_instance = tempInstance;
        }
        JFrameLayout::_instance_mutex.unlock();
    }

    return JFrameLayout::_instance;
}

void JFrameLayout::releaseInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameLayout::_instance != 0) {
        JFrameLayout::_instance_mutex.lock();
        if (JFrameLayout::_instance != 0) {
            JFrameLayout::_instance_mutex.unlock();
            delete JFrameLayout::_instance;
            JFrameLayout::_instance_mutex.lock();
            JFrameLayout::_instance = 0;
        }
        JFrameLayout::_instance_mutex.unlock();
    }
}

std::string JFrameLayout::interfaceIdentity() const
{
    return IID_IJFrameLayout;
}

unsigned int JFrameLayout::interfaceVersion() const
{
    return VER_IJFrameLayout;
}

void *JFrameLayout::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);

    return 0;
}

bool JFrameLayout::loadInterface()
{
    bool result = true;

    // 加载布局管理器
    result = result && data->layoutManager->loadInterface();

    // 加载主视图管理器
    result = result && data->mainViewManager->loadInterface();

    // 加载模式管理器
    result = result && data->moduleManager->loadInterface();

    // 加载消息管理器
    result = result && data->notifyManager->loadInterface();

    return result;
}

void JFrameLayout::releaseInterface()
{
    // 释放布局管理器实例
    if (data->layoutManager) {
        data->layoutManager->releaseInterface();
    }

    // 释放框架主视图实例
    if (data->mainViewManager) {
        data->mainViewManager->releaseInterface();
    }

    // 释放模块管理器实例
    if (data->moduleManager) {
        data->moduleManager->releaseInterface();
    }

    // 释放消息管理器实例
    if (data->notifyManager) {
        data->notifyManager->releaseInterface();
    }
}

std::list<std::string> JFrameLayout::queryMethod() const
{
    std::list<std::string> methods;

    // attach_component
    methods.push_back(std::string("attach_component").append("..."));

    return methods;
}

bool JFrameLayout::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    // 挂载组件
    if (method == "attach_component") {
        if (argc == 2) {
            IJComponent *component = va_arg(ap, IJComponent*);
            bool stayOn = va_arg(ap, bool);
            result = attachComponent(component, stayOn);
        }
    }
    // 分离组件
    else if (method == "detach_component") {
        if (argc == 1) {
            result = detachComponent((IJComponent *)va_arg(ap, void*));
        }
    }
    // 挂载组件窗口
    else if (method == "attach_component_ui") {
        if (argc == 2) {
            IJComponent *component = va_arg(ap, IJComponent*);
            QWidget *widget = va_arg(ap, QWidget*);
            result = attachComponentUi(component, widget);
        }
    }
    // 设置框架主题
    else if (method == "set_frame_theme") {
        if (argc > 0) {
            setFrameTheme(va_arg(ap, char*));
            result = true;
        }
    }
    // 加载默认框架系统
    else if (method == "load_default_system") {
        result = loadDefaultSystem();
    }
    // 尝试退出框架（异步方式，带提示窗口方式）
    else if (method == "frame_try_exit") {
        QWidget *mainWindow = this->mainWindow();
        if (mainWindow) {
            // 调用转入框架布局组件FrameFilter模块（异步事件）
            QApplication::postEvent(mainWindow, new QCloseEvent());
            result = true;
        }
    }
    // 退出框架（异步方式）
    else if (method == "frame_exit") {
        // 调用转入消息管理模块（异步消息）
        notifier()->imm().postMessage("jlayout.notify_manager", "j_frame_exit");
        result = true;
    }
    // 重启框架（异步方式）
    else if (method == "frame_restart") {
        // 调用转入消息管理模块（异步消息）
        notifier()->imm().postMessage("jlayout.notify_manager", "j_frame_restart");
        result = true;
    }

    va_end(ap);

    return result;
}

QWidget *JFrameLayout::mainWindow()
{
    return (QWidget *)jframeCore()->attempter()->mainWindow()->mainWidget();
}

QWidget *JFrameLayout::mainView()
{
    return data->mainViewManager;
}

INotifier *JFrameLayout::notifier()
{
    return jframeCore()->attempter()->notifier();
}

IJAttempter *JFrameLayout::attempter()
{
    return jframeCore()->attempter();
}

void JFrameLayout::setFrameTheme(const std::string &theme)
{
    // 设置框架主题
    jframeCore()->attempter()->mainWindow()->setTheme(theme);
}

int JFrameLayout::componentPowerLevel(const std::string &componentName) const
{
    Q_UNUSED(componentName);

    // 查找组件权限信息

    return JFrameLoginSpace::PowerLevelNodify;  //!!! 暂不支持
}

std::string JFrameLayout::currentSystem() const
{
    // 参数有效性检测
    if (!data->moduleManager) {
        return std::string();
    }

    return data->moduleManager->currentSystem();
}

std::string JFrameLayout::currentModule() const
{
    // 参数有效性检测
    if (!data->moduleManager) {
        return std::string();
    }

    return data->moduleManager->currentModule();
}

bool JFrameLayout::attachComponent(IJComponent *component, bool stayOn)
{
    return data->moduleManager->attachComponent(component, stayOn);
}

bool JFrameLayout::detachComponent(IJComponent *component)
{
    return data->moduleManager->detachComponent(component);
}

bool JFrameLayout::attachComponentUi(IJComponent *component, QWidget *widget)
{
    return data->moduleManager->attachComponentUi(component, widget);
}

LayoutManager *JFrameLayout::layoutManager()
{
    return data->layoutManager;
}

MainViewManager *JFrameLayout::mainViewManager()
{
    return data->mainViewManager;
}

ModuleManager *JFrameLayout::moduleManager()
{
    return data->moduleManager;
}

NotifyManager *JFrameLayout::notifyManager()
{
    return data->notifyManager;
}

bool JFrameLayout::loadDefaultSystem()
{
    // 参数有效性检测
    if (!data->moduleManager) {
        return false;   // 无效
    }

    // 异步方式启动默认设置系统
    notifier()->imm().postMessage("jlayout.module_manager", "j_load_default_system");

    return true;
}

bool JFrameLayout::loadLoginInfo()
{
    // 模块有效性检测
    if (!jloginManager()->isValid()) {
        return true;    // 若登录模块未启用，则忽略
    }
#if 0
    // 获取用户信息
    data->userInfo = jloginManager()->userInfo();
    data->userInfo.sPassword.clear();     // 不允许获取密码，防止恶插件或组件盗取

    // 获取当前席位权限信息
    data->powerInfo.vecElement.clear();
    std::string currentSeatId = jloginManager()->currentSeatInfo().sSeatID;
    if (currentSeatId.empty()) {
        jframeLogCrit(QStringLiteral("当前登录席位无效!"));
        return false;
    }

    // 获取当前登录用户、当前席位下的所有权限信息
    if (!jloginDBMgr()->getSeatPower(
                data->userInfo.sUserID, currentSeatId, data->powerInfo)) {
        jframeLogCrit(QStringLiteral("获取席位权限信息失败！"));
        return false;
    }

    // 映射：模块名称 <--> 权限信息
    std::vector<JFrameLoginSpace::stPowerElement>::const_iterator citerPowerInfo =
            data->powerInfo.vecElement.begin();
    for (; citerPowerInfo != data->powerInfo.vecElement.end(); ++citerPowerInfo) {
        const JFrameLoginSpace::stPowerElement &powerElement = *citerPowerInfo;
        if (powerElement.stModule.sDllName.empty()) {
            continue;       //
        }
        data->mapPower[powerElement.stModule.sDllName] = powerElement;
    }
#endif

    return true;
}

JFrameLayout::JFrameLayout()
{
    //
    data = new JFrameLayoutData;

    // 创建框架布局模块组成部件
    data->layoutManager = new LayoutManager(this);
    data->mainViewManager = new MainViewManager();
    data->moduleManager = new ModuleManager(this);
    data->notifyManager = new NotifyManager(this);
}

JFrameLayout::~JFrameLayout()
{
    // 销毁成员实例
    if (data->layoutManager) {
        delete data->layoutManager;
        data->layoutManager = 0;
    }
    if (data->mainViewManager) {
        delete data->mainViewManager;
        data->mainViewManager = 0;
    }
    if (data->moduleManager) {
        delete data->moduleManager;
        data->moduleManager = 0;
    }
    if (data->notifyManager) {
        delete data->notifyManager;
        data->notifyManager = 0;
    }

    delete data;
}
