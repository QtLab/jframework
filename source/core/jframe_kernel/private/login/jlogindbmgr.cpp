#include "precomp.h"
#include "jlogindbmgr.h"

JLoginDBMgr::JLoginDBMgr()
{

}

JLoginDBMgr::~JLoginDBMgr()
{

}

bool JLoginDBMgr::init()
{
    bool result = true;

    //

    return result;
}

void JLoginDBMgr::releaseInterface()
{
}

void *JLoginDBMgr::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);

    return 0;
}

std::string JLoginDBMgr::objectIdentity() const
{
    return IID_IJLoginDBMgr;
}

unsigned int JLoginDBMgr::objectVersion() const
{
    return VER_IJLoginDBMgr;
}

bool JLoginDBMgr::invoke(const char *method, int argc)
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
