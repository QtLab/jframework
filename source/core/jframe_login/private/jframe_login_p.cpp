#include "precomp.h"
#include "jframe_login_p.h"
//#include "login_database.h"
//#include "login_dialog.h"
//#include "login_manager.h"

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
            delete JFrameLogin::_instance;
            JFrameLogin::_instance = 0;
        }
        JFrameLogin::_instance_mutex.unlock();
    }
}

void JFrameLogin::releaseInterface()
{
    JFrameLogin::releaseInstance();
}

void *JFrameLogin::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);
    J_QUERY_MEMBER(IJLoginManager, iid, ver, data->loginManager);

    return 0;
}

std::string JFrameLogin::objectIdentity() const
{
    return IID_IJFrameLogin;
}

unsigned int JFrameLogin::objectVersion() const
{
    return VER_IJFrameLogin;
}

bool JFrameLogin::invoke(const char *method, int argc)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    // 启动登录模块，显示登录界面
    if (strcmp(method, "frame_login") == 0) {
        if (data->loginManager) {
#if 0
            result = data->loginManager->execute();
#endif
        }
    }
    // 注销登录（默认采用重启框架方式）
    else if (strcmp(method, "frame_logout") == 0) {
        if (data->loginManager) {
#if 0
            result = data->loginManager->logout();
#endif
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
#if 0
    data->loginManager = new LoginManager();
#endif
}

JFrameLogin::~IJFrameLogin()
{
    // 注销登录
#if 0
    if (!data->loginManager->logout()) {
        //
    }
#endif
    // 销毁登录管理器实例
    if (data->loginManager) {
        delete data->loginManager;
    }

    delete data;
}
