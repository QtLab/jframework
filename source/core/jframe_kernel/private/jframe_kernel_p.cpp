#include "precomp.h"
#include "jframe_kernel_p.h"
#include "core/jframe_core_p.h"
#include "layout/jframe_layout_p.h"
#include "login/jframe_login_p.h"
#include "qmfcapp.h"
#include "factory/jlogmanager.h"

// struct JFrameKernelData

struct JFrameKernelData
{
    IJLogManager *logManager;       // 框架日志系统部件

    JFrameKernelData()
        : logManager(0)
    {

    }
};

// class JFrameKernel

//
QMutex JFrameKernel::_instance_mutex;
JFrameKernel *JFrameKernel::_instance = 0;

JFrameKernel *JFrameKernel::getInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameKernel::_instance == 0) {
        JFrameKernel::_instance_mutex.lock();
        if (JFrameKernel::_instance == 0) {
            JFrameKernel *tmpInstance = new JFrameKernel;
            JFrameKernel::_instance = tmpInstance;
        }
        JFrameKernel::_instance_mutex.unlock();
    }

    return JFrameKernel::_instance;
}

void JFrameKernel::releaseInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameKernel::_instance != 0) {
        JFrameKernel::_instance_mutex.lock();
        if (JFrameKernel::_instance != 0) {
            JFrameKernel::_instance_mutex.unlock();
            delete JFrameKernel::_instance;
            JFrameKernel::_instance_mutex.lock();
            JFrameKernel::_instance = 0;
        }
        JFrameKernel::_instance_mutex.unlock();
    }
}

void JFrameKernel::releaseInterface()
{
    // 释放框架登录部件
    JFrameLogin::getInstance()->releaseInterface();

    // 释放框架布局部件
    JFrameLayout::getInstance()->releaseInterface();

    // 释放框架核心部件
    JFrameCore::getInstance()->releaseInterface();

    // 释放日志管理部件
    if (data->logManager) {
        data->logManager->releaseInterface();
    }

    //
    JFrameKernel::releaseInstance();
}

void *JFrameKernel::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);
    J_QUERY_MEMBER_OBJECT(IJObject, iid, ver, frameCore());
    J_QUERY_MEMBER_OBJECT(IJObject, iid, ver, frameLayout());
    J_QUERY_MEMBER_OBJECT(IJObject, iid, ver, frameLogin());

    return 0;
}

std::string JFrameKernel::objectIdentity() const
{
    return IID_IJFrameKernel;
}

unsigned int JFrameKernel::objectVersion() const
{
    return VER_IJFrameKernel;
}

bool JFrameKernel::invoke(const char *method, int argc, ...)
{
    if (!method) {
        return false;
    }

    bool result = false;

    va_list ap;
    va_start(ap, argc);

    // 输出一条日志
    if (strcmp(method, "log") == 0) {
        result = invokeLog(argc, ap);
    }
    // 显示框架
    else if (strcmp(method, "frame_show") == 0) {
        if (argc == 2) {
            // 转到框架核心系统
            result = frameCore()->invoke("frame_show", 2, va_arg(ap, bool), va_arg(ap, bool));
        }
    }
    // 尝试退出框架（异步方式）
    else if (strcmp(method, "frame_try_exit") == 0) {
        // 转到框架布局系统
        result = frameLayout()->invoke("frame_try_exit");
    }
    // 退出框架（异步方式）
    else if (strcmp(method, "frame_exit") == 0) {
        // 转到框架布局系统
        result = frameLayout()->invoke("frame_exit");
    }
    // 重启框架（异步方式）
    else if (strcmp(method, "frame_restart") == 0) {
        // 转到框架布局系统
        result = frameLayout()->invoke("frame_restart");
    }
    // 登录框架
    else if (strcmp(method, "frame_login") == 0) {
        // 转到框架登录系统
        result = frameLogin()->invoke("frame_login");
    }
    // 注销框架
    else if (strcmp(method, "frame_logout") == 0) {
        // 转到框架登录系统
        result = frameLogin()->invoke("frame_logout");
    }
    // 运行Qt消息循环系统
    else if (strcmp(method, "run_q_app") == 0) {
        if (argc == 2) {
            // 转到框架布局系统
            result = frameLayout()->invoke("run_q_app", 2,
                                           va_arg(ap, void*), va_arg(ap, int*));
        }
    }
    // 运行Qt消息循环系统
    else if (strcmp(method, "window_handle") == 0) {
        if (argc == 3) {
            // 转到框架布局系统
            result = frameLayout()->invoke("window_handle", 3, va_arg(ap, void*),
                                           va_arg(ap, char*), va_arg(ap, long));
        }
    }
    // 启动框架
    else if (strcmp(method, "frame_start") == 0) {
        //
        result = startFrame();
    }
    // 加载默认框架系统
    else if (strcmp(method, "load_default_system") == 0) {
        // 转到框架布局系统
        result = frameLayout()->invoke("load_default_system");
    }

    va_end(ap);

    return result;
}

IJLogManager *JFrameKernel::logManager()
{
    return data->logManager;
}

IJFrameCore *JFrameKernel::frameCore()
{
    return JFrameCore::getInstance();
}

IJFrameLayout *JFrameKernel::frameLayout()
{
    return JFrameLayout::getInstance();
}

IJFrameLogin *JFrameKernel::frameLogin()
{
    return JFrameLogin::getInstance();
}

bool JFrameKernel::invokeLog(int argc, va_list ap)
{
    // 日志管理器实例的有效性检测
    if (!data->logManager) {
        return false;   // 无效
    }

    // 参数有效性检测
    if (argc < 2) {
        return false;   // 无效 (type, msg, ...)
    }

    //
    const char* sType = va_arg(ap, char*);
    if (!sType) {
        return false;   // 参数无效
    }

    //
    IJLogManager::MsgType msgType = IJLogManager::InfoMsg;
    if (strcmp(sType, "emerge") == 0) {
        msgType = IJLogManager::EmergeMsg;
    } else if (strcmp(sType, "fatal") == 0) {
        msgType = IJLogManager::FatalMsg;
    } if (strcmp(sType, "alert") == 0) {
        msgType = IJLogManager::AlertMsg;
    } if (strcmp(sType, "crit") == 0) {
        msgType = IJLogManager::CriticalMsg;
    } if (strcmp(sType, "error") == 0) {
        msgType = IJLogManager::ErrorMsg;
    } if (strcmp(sType, "warn") == 0) {
        msgType = IJLogManager::WarningMsg;
    } if (strcmp(sType, "notice") == 0) {
        msgType = IJLogManager::NoticeMsg;
    } if (strcmp(sType, "info") == 0) {
        msgType = IJLogManager::InfoMsg;
    } if (strcmp(sType, "debug") == 0) {
        msgType = IJLogManager::DebugMsg;
    }

    //
    const char* msg = va_arg(ap, char*);
    if (!msg) {
        return false;   // 参数无效
    }

    //
    const char* file = 0;
    int line = 0;
    const char* func = 0;
    if (argc > 2) {
        file = va_arg(ap, char*);
    }
    if (argc > 3) {
        line = va_arg(ap, int);
    }
    if (argc > 4) {
        func = va_arg(ap, char*);
    }

    // 调用日志管理器输出一条日志信息
    data->logManager->logging(msgType, msg, (argc - 1), file, line, func);

    return true;
}

bool JFrameKernel::startFrame()
{
    bool result = true;

    // 初始化框架核心系统
    JFrameCore::getInstance();

    // 初始化框架布局系统
    JFrameLayout::getInstance();

    // 初始化框架登录部件
    JFrameLogin::getInstance();

    // 加载框架核心系统
    result = result && JFrameCore::getInstance()->attempter()->loadComponent();

    return result;
}

JFrameKernel::JFrameKernel()
{
    // 创建私有数据实例
    data = new JFrameKernelData;

    // 加载默认系统样式表
    //KwtCore::instance().loadDefaultSystemSheet();

    // 创建日志管理器实例
    data->logManager = JFRAME_FACTORY_CREATE(IJLogManager);
}

JFrameKernel::~JFrameKernel()
{
    // 销毁日志管理器实例
    if (data->logManager) {
        JFRAME_FACTORY_RELEASE(data->logManager, IJLogManager);
    }

    // 销毁框架登录部件
    JFrameLogin::releaseInstance();

    // 销毁框架布局部件
    JFrameLayout::releaseInstance();

    // 销毁框架核心部件
    JFrameCore::releaseInstance();

    // 销毁私有数据实例
    delete data;
}
