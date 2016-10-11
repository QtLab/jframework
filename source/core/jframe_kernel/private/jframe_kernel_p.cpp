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
    std::string loggingFilePath;    // 框架日志配置文件

    JFrameKernelData()
        : logManager(0)
        , loggingFilePath("")
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

std::string JFrameKernel::interfaceIdentity() const
{
    return IID_IJFrameKernel;
}

unsigned int JFrameKernel::interfaceVersion() const
{
    return VER_IJFrameKernel;
}

void *JFrameKernel::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, frameCore());
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, frameLayout());
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, frameLogin());

    return 0;
}

bool JFrameKernel::loadInterface()
{
    bool result = true;

    //
    JwtCore::instance()->init();
    JwtCore::instance()->loadSystemLang();
    //qApp->setStyleSheet(JStyleSheet::instance()->styleSheet("default"));

    // 加载日志管理器
    if (data->logManager) {
        result = result && data->logManager->loadInterface();
    }

    // 加载框架核心系统
    result = result && frameCore()->loadInterface();

    // 加载框架布局系统
    result = result && frameLayout()->loadInterface();

    // 加载框架登录系统
    result = result && frameLogin()->loadInterface();

    // 加载组件
    result = result && frameCore()->attempter()->loadComponent();

    // 启动默认系统
    if (result) {
        frameLayout()->notifier().postMessage("j_load_default_system @ jlayout.module_manager");
    }

    return true;
}

void JFrameKernel::releaseInterface()
{
    // 释放框架登录系统
    frameLogin()->releaseInterface();

    // 释放框架布局系统
    frameLayout()->releaseInterface();

    // 释放框架核心系统
    frameCore()->releaseInterface();

    // 释放日志管理器
    if (data->logManager) {
        data->logManager->releaseInterface();
    }
}

std::list<std::string> JFrameKernel::queryMethod() const
{
    std::list<std::string> methods;

    // log
    methods.push_back(std::string("log").append("..."));

    return methods;
}

bool JFrameKernel::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;

    va_list ap;
    va_start(ap, argc);

    // 输出一条日志
    if (method == "log") {
        result = invokeLog(argc, ap);
    }
    // 显示框架
    else if (method == "frame_show") {
        if (argc == 2) {
            // 转到框架核心系统
#if defined(__unix__)
            bool show = va_arg(ap, int);
            bool maximized = va_arg(ap, int);
#else
            bool show = va_arg(ap, bool);
            bool maximized = va_arg(ap, bool);
#endif
            result = frameCore()->invokeMethod("frame_show", 2, show, maximized);
        }
    }
    // 尝试退出框架（异步方式）
    else if (method == "frame_try_exit") {
        // 转到框架布局系统
        result = frameLayout()->invokeMethod("frame_try_exit");
    }
    // 退出框架（异步方式）
    else if (method == "frame_exit") {
        // 转到框架布局系统
        result = frameLayout()->invokeMethod("frame_exit");
    }
    // 重启框架（异步方式）
    else if (method == "frame_restart") {
        // 转到框架布局系统
        result = frameLayout()->invokeMethod("frame_restart");
    }
    // 登录框架
    else if (method == "frame_login") {
        // 转到框架登录系统
        result = frameLogin()->invokeMethod("frame_login");
    }
    // 注销框架
    else if (method == "frame_logout") {
        // 转到框架登录系统
        result = frameLogin()->invokeMethod("frame_logout");
    }
    // 运行Qt消息循环系统
    else if (method == "run_q_app") {
        if (argc == 2) {
            // 转到框架核心系统
            void *mfcApp = va_arg(ap, void*);
            int *exitCode = va_arg(ap, int*);
            result = frameCore()->invokeMethod("run_q_app", 2, mfcApp, exitCode);
        }
    }
    // 运行Qt消息循环系统
    else if (method == "window_handle") {
        if (argc == 3) {
            // 转到框架核心系统
            void *window = va_arg(ap, void*);
            const char *winType = va_arg(ap, char*);
            long *handle = va_arg(ap, long*);
            result = frameCore()->invokeMethod("window_handle", 3, window, winType, handle);
        }
    }
    // 创建Qt应用实体
    else if (method == "create_qapp") {
        if (argc == 3) {
            // 转到框架核心系统
            int argc = va_arg(ap, int);
            char **argv = va_arg(ap, char**);
            void *app = va_arg(ap, void*);
            result = frameCore()->invokeMethod("create_qapp", 3, argc, argv, app);
        }
    }
    // 加载默认框架系统
    else if (method == "load_default_system") {
        // 转到框架布局系统
        result = frameLayout()->invokeMethod("load_default_system");
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

bool JFrameKernel::loadConfig()
{
    if (!QFileInfo(QString::fromStdString(jframeFacade()->frameGlobalPath())).isReadable()) {
        Q_ASSERT_X(false, "Warning", QStringLiteral("框架全局配置文件不存在！")
                   .toUtf8().data());
        return false;
    }

    // 读取配置文件
    TiXmlDocument document(jframeFacade()->frameGlobalPath());
    if (!document.LoadFile(TIXML_ENCODING_UTF8)) {
        Q_ASSERT_X(false, "Warning",
                   QStringLiteral("框架全局配置配置文件打\"%1\"开失败！\n错误标识：%2\n错误描述：%3\n错误位置：[%4, %5]")
                   .arg(QString::fromStdString(jframeFacade()->frameGlobalPath()))
                   .arg(document.ErrorId()).arg(QString::fromLatin1(document.ErrorDesc()))
                   .arg(document.ErrorRow()).arg(document.ErrorCol())
                   .toUtf8().data());
        return false;
    }

    // 获取根节点
    TiXmlElement *emRoot = document.RootElement();
    if (!emRoot) {
        return false;
    }

    // 获取logging节点
    TiXmlElement *emLogging = emRoot->FirstChildElement("logging");
    if (!emLogging) {
        return false;
    }

    //
    std::string sVal;

    // filePath
    if (emLogging->QueryStringAttribute("filePath", &sVal) != TIXML_SUCCESS) {
        Q_ASSERT_X(false, "Warning", QStringLiteral("未指定框架日志配置文件！[文件：%1]")
                   .arg(jframeFacade()->frameGlobalPath().c_str())
                   .toUtf8().data());
        return false;
    }

    // 替换框架内部变量
    QString filePath = QString::fromUtf8(emLogging->Attribute("filePath"));
    filePath.replace("@ConfigDir@", QString::fromStdString(jframeFacade()->configDirPath()));

    // 读取日志配置文件路径属性
    data->loggingFilePath = filePath.toStdString();

    // 文件有效性检测
    if (!QFile::exists(QString::fromStdString(data->loggingFilePath))) {
        Q_ASSERT_X(false, "Warning", QStringLiteral("框架日志配置文件不存在！[文件：%1]")
                   .arg(data->loggingFilePath.c_str())
                   .toUtf8().data());
        return false;
    }

    return true;
}

bool JFrameKernel::invokeLog(int argc, va_list ap)
{
    // 日志管理器实例的有效性检测
    if (!data->logManager) {
        return false;   // 无效
    }

    // 参数有效性检测
    if (argc < 3) {
        return false;   // 无效 (type, msg, where, ...)
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
    } else if (strcmp(sType, "alert") == 0) {
        msgType = IJLogManager::AlertMsg;
    } else if (strcmp(sType, "crit") == 0) {
        msgType = IJLogManager::CriticalMsg;
    } else if (strcmp(sType, "error") == 0) {
        msgType = IJLogManager::ErrorMsg;
    } else if (strcmp(sType, "warn") == 0) {
        msgType = IJLogManager::WarningMsg;
    } else if (strcmp(sType, "notice") == 0) {
        msgType = IJLogManager::NoticeMsg;
    } else if (strcmp(sType, "info") == 0) {
        msgType = IJLogManager::InfoMsg;
    } else if (strcmp(sType, "debug") == 0) {
        msgType = IJLogManager::DebugMsg;
    }

    //
    const char* msg = va_arg(ap, char*);
    if (!msg) {
        return false;   // 参数无效
    }

    //
    const char* where = va_arg(ap, char*);

    //
    const char* file = 0;
    int line = 0;
    const char* func = 0;
    if (argc > 3) {
        file = va_arg(ap, char*);
    }
    if (argc > 4) {
        line = va_arg(ap, int);
    }
    if (argc > 5) {
        func = va_arg(ap, char*);
    }

    // 调用日志管理器输出一条日志信息
    data->logManager->logging(msgType, msg, where, (argc - 3), file, line, func);

    return true;
}

JFrameKernel::JFrameKernel()
{
    // 创建私有数据实例
    data = new JFrameKernelData;

    // 创建日志管理器实例
    data->logManager = JFRAME_FACTORY_CREATE(IJLogManager);

    // 加载全局配置文件
    if (!loadConfig()) {
        // 加载失败
    }

    // 设置日志配置文件
    data->logManager->setConfig(data->loggingFilePath);
}

JFrameKernel::~JFrameKernel()
{
    // 销毁框架登录部件
    JFrameLogin::releaseInstance();

    // 销毁框架布局部件
    JFrameLayout::releaseInstance();

    // 销毁框架核心部件
    JFrameCore::releaseInstance();

    // 销毁日志管理器实例
    if (data->logManager) {
        JFRAME_FACTORY_RELEASE(data->logManager, IJLogManager);
    }

    // 销毁私有数据实例
    delete data;
}
