#ifndef JFRAME_KERNEL_P_H
#define JFRAME_KERNEL_P_H

#include "../jframe_kernel.h"
#include <QMutex>

// class JFrameKernel

struct JFrameKernelData;

class JFrameKernel : public IJFrameKernel
{
    // single instance
public:
    static JFrameKernel *getInstance();
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

    // IJFrameKernel interface
public:
    IJLogManager *logManager();
    IJFrameCore *frameCore();
    IJFrameLayout *frameLayout();
    IJFrameLogin *frameLogin();

private:
    bool loadConfig();

    bool invokeLog(int argc, va_list ap);

private:
    JFrameKernel();
    ~JFrameKernel();

    JFrameKernelData *data;

    //
    static QMutex _instance_mutex;      // 单实例锁
    static JFrameKernel *_instance;     // 单实例指针
};

#endif // JFRAME_KERNEL_P_H
