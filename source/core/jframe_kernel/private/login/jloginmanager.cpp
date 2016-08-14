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

bool JLoginManager::init()
{
    bool result = true;

    //
    result = result && dynamic_cast<JLoginDBMgr *>(q_loginDBMgr)->init();

    return result;
}

void JLoginManager::releaseInterface()
{
    // ×¢ÏúµÇÂ¼
    if (!logout()) {
        //
    }

    //
    q_loginDBMgr->releaseInterface();
}

void *JLoginManager::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);

    return 0;
}

std::string JLoginManager::objectIdentity() const
{
    return IID_IJLoginManager;
}

unsigned int JLoginManager::objectVersion() const
{
    return VER_IJLoginManager;
}

bool JLoginManager::invoke(const char *method, int argc)
{
    if (!method) {
        return false;
    }

    bool result = false;
    va_list ap;
    va_start(ap, argc);

    va_end(ap);

    return result;
}

bool JLoginManager::isValid() const
{
    return false;
}

bool JLoginManager::execute()
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
