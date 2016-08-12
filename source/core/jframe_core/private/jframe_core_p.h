#ifndef JFRAME_CORE_P_H
#define JFRAME_CORE_P_H

#include "../jframe_core.h"
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
    void releaseInterface();
    void *queryInterface(const char *iid, unsigned int ver);

    // IJObject interface
public:
    std::string objectIdentity() const;
    unsigned int objectVersion() const;
    bool invoke(const char *method, int argc, ...);

    // IJFrameCore interface
public:

private:
    JFrameCore();
    ~JFrameCore();

    JFrameCoreData* data;

    //
    static QMutex _instance_mutex;      // 单实例保护锁
    static JFrameCore* _instance;       // 单实例指针
};

#endif // JFRAME_CORE_P_H
