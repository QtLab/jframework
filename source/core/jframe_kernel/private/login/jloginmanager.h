#ifndef JLOGINMANAGER_H
#define JLOGINMANAGER_H

#include "../../jframe_login.h"

class JLoginManager : public IJLoginManager
{
public:
    explicit JLoginManager();
    ~JLoginManager();

    bool init();

    // IJUnknown interface
public:
    void releaseInterface();
    void *queryInterface(const char *iid, unsigned int ver);

    // IJObject interface
public:
    std::string objectIdentity() const;
    unsigned int objectVersion() const;
    bool invoke(const char *method, int argc);

    // IJLoginManager interface
public:
    bool isValid() const;
    bool execute();
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
