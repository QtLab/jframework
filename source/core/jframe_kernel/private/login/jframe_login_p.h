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
    std::string interfaceIdentity() const;
    unsigned int interfaceVersion() const;
    void *queryInterface(const std::string &iid, unsigned int ver);
    bool loadInterface();
    void releaseInterface();
    std::list<std::string> queryMethod() const;
    bool invokeMethod(const std::string &method, int argc, ...);

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
