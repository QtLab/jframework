#include "precomp.h"
#include "jframe_layout_p.h"
#include "kernel/jnotifier.h"
#include "mainview_manager/mainview_manager.h"
#include "layout_manager/layout_manager.h"
#include "module_manager/module_manager.h"
#include "notify_manager/notify_manager.h"
#include "IGF_Kernel.h"

// struct JFrameLayoutData

struct JFrameLayoutData
{
    QWidget *mainWindow;                // 框架主窗口实例
    IGF_Attempter *gAttempter;          // GF调度器实例
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
        : mainWindow(0)
        , gAttempter(0)
        , notifier(0)
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
            delete JFrameLayout::_instance;
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
    J_QUERY_MEMBER_OBJECT(IGF_Attempter, iid, ver, data->gAttempter);

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
    // 设置GF框架调度器实例
    else if (strcmp(method, "set_g_attempter") == 0) {
        if (argc > 0) {
            data->gAttempter = va_arg(ap, IGF_Attempter*);
            if (data->gAttempter) {
                // 获取GF框架主窗口实例接口
                IGF_MainWindow *pMainWindow =
                        reinterpret_cast<IGF_MainWindow *>(data->gAttempter->GetMainWindow());
                if (pMainWindow) {
                    // 获取GF框架主窗口实例
                    data->mainWindow = reinterpret_cast<QWidget *>(pMainWindow);
                    if (data->mainWindow) {
                        result = true;
                    }
                }
            }
        }
    }
    // 加载默认框架系统
    else if (strcmp(method, "load_default_system") == 0) {
        result = loadDefaultSystem();
    }
    // 尝试退出框架（异步方式，带提示窗口方式）
    else if (strcmp(method, "frame_try_exit") == 0) {
        if (data->mainWindow) {
            // 调用转入框架布局组件FrameFilter模块（异步事件）
            QApplication::postEvent(data->mainWindow, new QCloseEvent());
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
    // 显示或隐藏框架主窗口
    else if (strcmp(method, "frame_show") == 0) {
        result = invokeShowFrame(argc, ap);
    }

    va_end(ap);

    return result;
}

QWidget *JFrameLayout::mainWindow()
{
    return data->mainWindow;
}

INotifier *JFrameLayout::notifier()
{
    return data->notifier;
}

IGF_Attempter *JFrameLayout::gAttempter()
{
    return data->gAttempter;
}

void JFrameLayout::setFrameTheme(const char *theme)
{
    if (!theme) {
        return; // 参数无效
    }

    // 解析主题信息
    QtRibbon::RibbonStyle::Theme nTheme;
    if (strcmp(theme, J_FRAME_THEME_OFFICE_2007BLUE) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2007Blue;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2007BLACK) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2007Black;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2007SILVER) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2007Silver;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2007AQUA) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2007Aqua;
    } else if (strcmp(theme, J_FRAME_THEME_WINDOWs7_SCENIC) == 0) {
        nTheme = QtRibbon::RibbonStyle::Windows7Scenic;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2010SILVER) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2010Silver;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2010BLUE) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2010Blue;
    } else if (strcmp(theme, J_FRAME_THEME_OFFICE_2010BLACK) == 0) {
        nTheme = QtRibbon::RibbonStyle::Office2010Black;
    } else {
        nTheme = QtRibbon::RibbonStyle::Office2010Blue;
    }

    // 获取原有风格管理器
    QtRibbon::RibbonStyle *appStyle = qobject_cast<QtRibbon::RibbonStyle *>(qApp->style());
    if (!appStyle) {
        appStyle = new QtRibbon::RibbonStyle;
        qApp->setStyle(appStyle);
    }

    // 获取原有主题
    QtRibbon::RibbonStyle::Theme oldTheme = appStyle->getTheme();
    if (nTheme != oldTheme) {
        // 设置新主题
        appStyle->setTheme(nTheme);
    }
}

bool JFrameLayout::attachComponent(IGF_Component *component, bool stayOn)
{
    return data->moduleManager->attachComponent(component, stayOn);
}

bool JFrameLayout::detachComponent(IGF_Component *component)
{
    return data->moduleManager->detachComponent(component);
}

bool JFrameLayout::attachComponentUi(IGF_Component *component, QWidget *widget)
{
    return data->moduleManager->attachComponentUi(component, widget);
}

std::list<IGF_Component *> JFrameLayout::attachedComponent() const
{
    return data->moduleManager->attachedComponents();
}

int JFrameLayout::componentPowerLevel(const char *componentId) const
{
    // 参数有效性检测
    if (!componentId) {
        return JFrameLoginSpace::PowerLevelInvalid;     // 无效权限等级
    }

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

long JFrameLayout::windowHandle(void *window, const std::string &winType)
{
    // 参数有效性检测
    if (!window) {
        return 0;   // 参数无效
    }

    // 获取QWidget实例窗口句柄
    if (winType == "QWidget") {
        QWidget *widget = reinterpret_cast<QWidget *>(window);
        if (widget) {
            return (long)widget->winId();
        }
    }
#if defined(_AFXDLL)
    else if (winType == "CWnd") {
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
    if (!jframeLogin()->loginManager()->isValid()) {
        return true;    // 若登录模块未启用，则忽略
    }
#if 0
    // 获取用户信息
    data->userInfo = jframeLogin()->loginManager()->userInfo();
    data->userInfo.sPassword.clear();     // 不允许获取密码，防止恶插件或组件盗取

    // 获取当前席位权限信息
    data->powerInfo.vecElement.clear();
    std::string currentSeatId = jframeLogin()->loginManager()->currentSeatInfo().sSeatID;
    if (currentSeatId.empty()) {
        jframeLogCrit(QStringLiteral("当前登录席位无效!"));
        return false;
    }

    // 获取当前登录用户、当前席位下的所有权限信息
    if (!jframeLogin()->loginManager()->loginDBMgr()->getSeatPower(
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

bool JFrameLayout::invokeShowFrame(int argc, va_list ap)
{
    // 参数有效性检测
    if (argc != 2) {
        return false;
    }

    //
    if (!data->gAttempter) {
        return false;
    }

    //
    IGF_MainWindow *pMainWindow =
            reinterpret_cast<IGF_MainWindow *>(data->gAttempter->GetMainWindow());
    if (!pMainWindow) {
        return false;
    }

    //
    bool show = va_arg(ap, bool);
    bool maximized = va_arg(ap, bool);

    //
    if (show) {
        if (maximized) {
            pMainWindow->ShowMaximized();
        } else {
            pMainWindow->Show();
        }
    } else {
        pMainWindow->Hide();
    }

    return true;
}

JFrameLayout::JFrameLayout()
{
    //
    data = new JFrameLayoutData;

    // 设置窗口主题
    setFrameTheme(J_FRAME_THEME_OFFICE_2010BLUE);

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
    // 卸载所有组件
    if (data->gAttempter) {
        // 下电所有组件
        data->gAttempter->ShutDownAllComponent();

        // 销毁所有组件窗口实例
        std::list<IGF_Component *> components = data->gAttempter->GetAllComponentPtr();
        std::list<IGF_Component *>::const_iterator citerComponent = components.begin();
        for (; citerComponent != components.end(); ++citerComponent) {
            IGF_Component *component = *citerComponent;
            if (component) {
                component->Release();
                delete component;
            }
        }
    }

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
