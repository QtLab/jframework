#ifndef JFRAME_LOGIN_P_H
#define JFRAME_LOGIN_P_H

#include "../../jframe_login.h"
#include <QMutex>

// class JFrameLogin

struct JFrameLoginData;
class INotifier;

class JFrameLogin : public IJFrameLogin
{
    // single instance
public:
    static JFrameLogin* getInstance();
    static void releaseInstance();

    // IJUnknown interface
public:
    void releaseInterface();
    void *queryInterface(const char *iid, unsigned int ver);

    // IJObject interface
public:
    std::string objectIdentity() const;
    unsigned int objectVersion() const;
    bool invoke(const char *method, int argc, ...);

    // IJFrameLogin interface
public:
    IJLoginManager *loginManager();

private:
    JFrameLogin();
    ~JFrameLogin();

    JFrameLoginData* data;

    //
    static QMutex _instance_mutex;      // 单实例保护锁
    static JFrameLogin* _instance;    // 单实例指针
};

#endif // JFRAME_LOGIN_H
