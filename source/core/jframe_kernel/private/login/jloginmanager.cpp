#include "precomp.h"
#include "jloginmanager.h"
#include "jlogindbmgr.h"

// class JLoginManager

JLoginManager::JLoginManager()
    : q_loginDBMgr(0)
{
    q_loginDBMgr = new JLoginDBMgr();
}

JLoginManager::~JLoginManager()
{
    if (q_loginDBMgr) {
        delete q_loginDBMgr;
        q_loginDBMgr = 0;
    }
}

std::string JLoginManager::interfaceIdentity() const
{
    return IID_IJLoginManager;
}

unsigned int JLoginManager::interfaceVersion() const
{
    return VER_IJLoginManager;
}

void *JLoginManager::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);

    return 0;
}

bool JLoginManager::loadInterface()
{
    bool result = true;

    // 加载登录数据库管理器
    result = result && q_loginDBMgr->loadInterface();

    return result;
}

void JLoginManager::releaseInterface()
{
    // 注销登录
    if (!logout()) {
        //
    }

    // 注销登录数据库管理器
    q_loginDBMgr->releaseInterface();
}

std::list<std::string> JLoginManager::queryMethod() const
{
    std::list<std::string> methods;

    //

    return methods;
}

bool JLoginManager::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    Q_UNUSED(method);

    va_end(ap);

    return result;
}

bool JLoginManager::isValid() const
{
    return false;
}

bool JLoginManager::login()
{
    return false;
}

bool JLoginManager::logout()
{
    return false;
}

IJLoginDBMgr *JLoginManager::loginDBMgr()
{
    return q_loginDBMgr;
}

bool JLoginManager::isAdminUser() const
{
    return false;
}

std::string JLoginManager::userName() const
{
    return std::string();
}

int JLoginManager::userType() const
{
    return JFrameLoginSpace::UserTypeInvalid;
}

std::string JLoginManager::currentSeat() const
{
    return std::string();
}

std::string JLoginManager::currentSystem() const
{
    return std::string();
}
