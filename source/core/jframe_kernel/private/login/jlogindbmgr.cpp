#include "precomp.h"
#include "jlogindbmgr.h"

JLoginDBMgr::JLoginDBMgr()
{

}

JLoginDBMgr::~JLoginDBMgr()
{

}

std::string JLoginDBMgr::interfaceIdentity() const
{
    return IID_IJLoginDBMgr;
}

unsigned int JLoginDBMgr::interfaceVersion() const
{
    return VER_IJLoginDBMgr;
}

void *JLoginDBMgr::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);

    return 0;
}

bool JLoginDBMgr::loadInterface()
{
    bool result = true;

    //

    return result;
}

void JLoginDBMgr::releaseInterface()
{
}

std::list<std::string> JLoginDBMgr::queryMethod() const
{
    std::list<std::string> methods;

    //

    return methods;
}

bool JLoginDBMgr::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    Q_UNUSED(method);

    va_end(ap);

    return result;
}
