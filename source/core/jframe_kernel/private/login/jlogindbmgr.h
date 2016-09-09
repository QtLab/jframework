#ifndef JLOGINDBMGR_H
#define JLOGINDBMGR_H

#include "../../jframe_login.h"

class JLoginDBMgr : public IJLoginDBMgr
{
public:
    explicit JLoginDBMgr();
    ~JLoginDBMgr();

    bool init();

    // IJUnknown interface
public:
    std::string interfaceIdentity() const;
    unsigned int interfaceVersion() const;
    void *queryInterface(const std::string &iid, unsigned int ver);
    bool loadInterface();
    void releaseInterface();
    std::list<std::string> queryMethod() const;
    bool invokeMethod(const std::string &method, int argc, ...);

private:
};

#endif // JLOGINDBMGR_H
