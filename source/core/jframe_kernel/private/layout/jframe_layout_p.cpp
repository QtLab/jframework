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
    INotifier *notifier;                // 消息分发器实例
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
        : notifier(0)
        , layoutManager(0)
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

void JFrameLayout::releaseInterface()
{
    JFrameLayout::releaseInstance();
}

void *JFrameLayout::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);

    return 0;
}

std::string JFrameLayout::objectIdentity() const
{
    return IID_IJFrameLayout;
}

unsigned int JFrameLayout::objectVersion() const
{
    return VER_IJFrameLayout;
}

bool JFrameLayout::invoke(const char *method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    // 运行Qt消息循环系统
    if (strcmp(method, "run_q_app") == 0) {
        if (argc == 2) {
            int ret = runQApp(va_arg(ap, void*));
            int *pRet = va_arg(ap, int*);
            if (pRet) {
                *pRet = ret;
                result = true;
            }
        }
    }
    // 获取窗口实例的窗口句柄
    else if (strcmp(method, "window_handle") == 0) {
        if (argc == 3) {
            long handle = windowHandle(va_arg(ap, void*), va_arg(ap, char*));
            long *pHandle = va_arg(ap, long*);
            if (pHandle) {
                *pHandle = handle;
                result = true;
            }
        }
    }
    // 设置框架主题
    else if (strcmp(method, "set_frame_theme") == 0) {
        if (argc > 0) {
            setFrameTheme(va_arg(ap, char*));
            result = true;
        }
    }
    // 加载默认框架系统
    else if (strcmp(method, "load_default_system") == 0) {
        result = loadDefaultSystem();
    }
    // 尝试退出框架（异步方式，带提示窗口方式）
    else if (strcmp(method, "frame_try_exit") == 0) {
        QWidget *mainWindow = this->mainWindow();
        if (mainWindow) {
            // 调用转入框架布局组件FrameFilter模块（异步事件）
            QApplication::postEvent(mainWindow, new QCloseEvent());
            result = true;
        }
    }
    // 退出框架（异步方式）
    else if (strcmp(method, "frame_exit") == 0) {
        if (data->notifier) {
            // 调用转入消息管理模块（异步消息）
            data->notifier->imm().post("jlayout.notify_manager", "j_frame_exit");
            result = true;
        }
    }
    // 重启框架（异步方式）
    else if (strcmp(method, "frame_restart") == 0) {
        if (data->notifier) {
            // 调用转入消息管理模块（异步消息）
            data->notifier->imm().post("jlayout.notify_manager", "j_frame_restart");
            result = true;
        }
    }

    va_end(ap);

    return result;
}

QWidget *JFrameLayout::mainWindow()
{
    return (QWidget *)jframeCore()->attempter()->mainWindow()->mainWidget();
}

INotifier *JFrameLayout::notifier()
{
    return data->notifier;
}

IJAttempter *JFrameLayout::attempter()
{
    return jframeCore()->attempter();
}

void JFrameLayout::setFrameTheme(const char *theme)
{
    if (!theme) {
        return; // 参数无效
    }

    // 设置框架主题
    jframeCore()->attempter()->mainWindow()->setTheme(theme);
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

std::list<IJComponent *> JFrameLayout::attachedComponent() const
{
    return data->moduleManager->attachedComponents();
}

int JFrameLayout::componentPowerLevel(const std::string &componentId) const
{
    Q_UNUSED(componentId);

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

int JFrameLayout::runQApp(void *mfcApp)
{
#if defined(_AFXDLL)
    return QMfcApp::run(reinterpret_cast<CWinApp *>(mfcApp));
#else
    Q_UNUSED(mfcApp);
    return 0;
#endif
}

long JFrameLayout::windowHandle(void *window, const char *winType)
{
    // 参数有效性检测
    if (!window || !winType) {
        return 0;   // 参数无效
    }

    // 获取QWidget实例窗口句柄
    if (strcmp(winType, "QWidget") == 0) {
        QWidget *widget = reinterpret_cast<QWidget *>(window);
        if (widget) {
            return (long)widget->winId();
        }
    }
#if defined(_AFXDLL)
    else if (strcmp(winType, "CWnd") == 0) {
        CWnd *wnd = reinterpret_cast<CWnd *>(window);
        if (wnd) {
            return (long)wnd->GetSafeHwnd();
        }
    }
#endif

    return 0;
}

bool JFrameLayout::loadDefaultSystem()
{
    // 参数有效性检测
    if (!data->moduleManager) {
        return false;   // 无效
    }

    // 异步方式启动默认设置系统
    data->notifier->imm().post("jlayout.module_manager", "j_load_default_system");

    return true;
}

bool JFrameLayout::loadLoginInfo()
{
    // 模块有效性检测
    if (!jframeKernel()->frameLogin()->loginManager()->isValid()) {
        return true;    // 若登录模块未启用，则忽略
    }
#if 0
    // 获取用户信息
    data->userInfo = jframeKernel()->frameLogin()->loginManager()->userInfo();
    data->userInfo.sPassword.clear();     // 不允许获取密码，防止恶插件或组件盗取

    // 获取当前席位权限信息
    data->powerInfo.vecElement.clear();
    std::string currentSeatId = jframeKernel()->frameLogin()>loginManager()->currentSeatInfo().sSeatID;
    if (currentSeatId.empty()) {
        jframeLogCrit(QStringLiteral("当前登录席位无效!"));
        return false;
    }

    // 获取当前登录用户、当前席位下的所有权限信息
    if (!jframeKernel()->frameLogin()->loginManager()->loginDBMgr()->getSeatPower(
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

    return false;
}

JFrameLayout::JFrameLayout()
{
    //
    data = new JFrameLayoutData;

    // 创建消息分发器实例
    data->notifier = JFRAME_FACTORY_CREATE(INotifier);

    // 创建框架布局模块组成部件
    data->layoutManager = new LayoutManager(this);
    data->mainViewManager = new MainViewManager();
    data->moduleManager = new ModuleManager(this);
    data->notifyManager = new NotifyManager(this);

    //
    bool result = true;

    // 初始化各模块
    result = result && data->layoutManager->init();
    result = result && data->mainViewManager->init();
    result = result && data->moduleManager->init();
    result = result && data->notifyManager->init();

    // 结果检测
    if (!result) {
        //
    }
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

    // 销毁消息分发器实例
    JFRAME_FACTORY_RELEASE(data->notifier, INotifier);

    delete data;
}
