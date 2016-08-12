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
    void releaseInterface();
    void *queryInterface(const char *iid, unsigned int ver);

    // IJObject interface
public:
    std::string objectIdentity() const;
    unsigned int objectVersion() const;
    bool invoke(const char *method, int argc);

    // IJFrameKernel interface
public:
    IJLogManager *logManager();

private:
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
