#include "precomp.h"
#include "jframe_login_p.h"
#include "jloginmanager.h"

// struct JFrameLoginData

struct JFrameLoginData
{
    IJLoginManager *loginManager;   // 登录管理器实例

    JFrameLoginData() :
        loginManager(0)
    {

    }
};

// class JFrameLogin

//
QMutex JFrameLogin::_instance_mutex;
JFrameLogin* JFrameLogin::_instance;

JFrameLogin* JFrameLogin::getInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameLogin::_instance == 0) {
        JFrameLogin::_instance_mutex.lock();
        if (JFrameLogin::_instance == 0) {
            JFrameLogin* tempInstance = new JFrameLogin;
            JFrameLogin::_instance = tempInstance;
        }
        JFrameLogin::_instance_mutex.unlock();
    }

    return JFrameLogin::_instance;
}

void JFrameLogin::releaseInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameLogin::_instance != 0) {
        JFrameLogin::_instance_mutex.lock();
        if (JFrameLogin::_instance != 0) {
            JFrameLogin::_instance_mutex.unlock();
            delete JFrameLogin::_instance;
            JFrameLogin::_instance_mutex.lock();
            JFrameLogin::_instance = 0;
        }
        JFrameLogin::_instance_mutex.unlock();
    }
}

std::string JFrameLogin::interfaceIdentity() const
{
    return IID_IJFrameLogin;
}

unsigned int JFrameLogin::interfaceVersion() const
{
    return VER_IJFrameLogin;
}

void *JFrameLogin::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, data->loginManager);

    return 0;
}

bool JFrameLogin::loadInterface()
{
    bool result = true;

    // 加载登录管理器
    if (data->loginManager) {
        result = result && data->loginManager->loadInterface();
    }

    return result;
}

void JFrameLogin::releaseInterface()
{
    // 释放登录管理器
    if (data->loginManager) {
        data->loginManager->releaseInterface();
    }
}

std::list<std::string> JFrameLogin::queryMethod() const
{
    std::list<std::string> methods;

    // frame_login
    methods.push_back(std::string("frame_login").append("..."));

    return methods;
}

bool JFrameLogin::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    // 启动登录模块，显示登录界面
    if (method == "frame_login") {
        if (data->loginManager) {
            result = data->loginManager->login();
        }
    }
    // 注销登录（默认采用重启框架方式）
    else if (method == "frame_logout") {
        if (data->loginManager) {
            result = data->loginManager->logout();
        }
    }

    va_end(ap);

    return result;
}

IJLoginManager *JFrameLogin::loginManager()
{
    return data->loginManager;
}

JFrameLogin::JFrameLogin()
{
    data = new JFrameLoginData;

    // 创建登录管理器
    data->loginManager = new JLoginManager();
}

JFrameLogin::~JFrameLogin()
{
    // 销毁登录管理器
    if (data->loginManager) {
        delete data->loginManager;
        data->loginManager = 0;
    }

    delete data;
}
