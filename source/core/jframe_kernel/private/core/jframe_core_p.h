#ifndef JFRAME_CORE_P_H
#define JFRAME_CORE_P_H

#include "../../jframe_core.h"
#include <QMutex>

// class JFrameCore

struct JFrameCoreData;

class JFrameCore : public IJFrameCore
{
    // single instance
public:
    static JFrameCore* getInstance();
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

    // IJFrameCore interface
public:
    IJAttempter *attempter();

private:
    bool loadConfig();

private:
    //
    bool invokeShowFrame(int argc, va_list ap);
    int runQApp(void *mfcApp);
    long invokeWindowHandle(void *window, const std::string &winType);
    bool invokeCreateQApp(int argc, va_list ap);

private:
    JFrameCore();
    ~JFrameCore();

    JFrameCoreData* data;

    //
    static QMutex _instance_mutex;      // 单实例保护锁
    static JFrameCore* _instance;       // 单实例指针
};

#endif // JFRAME_CORE_P_H
