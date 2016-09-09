#ifndef JLOGINMANAGER_H
#define JLOGINMANAGER_H

#include "../../jframe_login.h"

class JLoginManager : public IJLoginManager
{
public:
    explicit JLoginManager();
    ~JLoginManager();

    // IJUnknown interface
public:
    std::string interfaceIdentity() const;
    unsigned int interfaceVersion() const;
    void *queryInterface(const std::string &iid, unsigned int ver);
    bool loadInterface();
    void releaseInterface();
    std::list<std::string> queryMethod() const;
    bool invokeMethod(const std::string &method, int argc, ...);

    // IJLoginManager interface
public:
    bool isValid() const;
    bool login();
    bool logout();
    IJLoginDBMgr *loginDBMgr();
    bool isAdminUser() const;
    std::string userName() const;
    int userType() const;
    std::string currentSeat() const;
    std::string currentSystem() const;

private:
    IJLoginDBMgr *q_loginDBMgr;
};

#endif // JLOGINMANAGER_H
