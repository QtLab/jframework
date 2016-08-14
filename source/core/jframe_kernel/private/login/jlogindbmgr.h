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
    void releaseInterface();
    void *queryInterface(const char *iid, unsigned int ver);

    // IJObject interface
public:
    std::string objectIdentity() const;
    unsigned int objectVersion() const;
    bool invoke(const char *method, int argc);

private:
};

#endif // JLOGINDBMGR_H
