#include "precomp.h"
#include "jframe_factory_p.h"
#include "kernel/jframe_kernel.h"
#include "kernel/jnotifier.h"

// imported from jframe_kernel -> jnotifier
Q_EXTERN_C extern void* _func_jnotifier_create();

// imported from jframe_kernel -> jnotifier
Q_EXTERN_C extern void _func_jnotifier_destroy(void *notifier);

// struct JFrameFactoryData

struct JFrameFactoryData
{

};

// class JFrameFactory

//
QMutex JFrameFactory::_instance_mutex;
JFrameFactory* JFrameFactory::_instance;

JFrameFactory* JFrameFactory::getInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameFactory::_instance == 0) {
        JFrameFactory::_instance_mutex.lock();
        if (JFrameFactory::_instance == 0) {
            JFrameFactory* tempInstance = new JFrameFactory;
            JFrameFactory::_instance = tempInstance;
        }
        JFrameFactory::_instance_mutex.unlock();
    }

    return JFrameFactory::_instance;
}

void JFrameFactory::releaseInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameFactory::_instance != 0) {
        JFrameFactory::_instance_mutex.lock();
        if (JFrameFactory::_instance != 0) {
            delete JFrameFactory::_instance;
            JFrameFactory::_instance = 0;
        }
        JFrameFactory::_instance_mutex.unlock();
    }
}

void JFrameFactory::releaseInterface()
{
    JFrameFactory::releaseInstance();
}

void *JFrameFactory::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);

    return 0;
}

std::string JFrameFactory::objectIdentity() const
{
    return IID_IJFrameFactory;
}

unsigned int JFrameFactory::objectVersion() const
{
    return VER_IJFrameFactory;
}

void *JFrameFactory::factory(const char *iid, unsigned int ver)
{
    if (J_IS_INSTANCEOF(INotifier, iid, ver)) {
        return _func_jnotifier_create();
    }

    return 0;
}

void JFrameFactory::releaseFactory(void *interface, const char *iid, unsigned int ver)
{
    if (J_IS_INSTANCEOF(INotifier, iid, ver)) {
        _func_jnotifier_destroy(interface);
    }
}

JFrameFactory::JFrameFactory()
{
    data = new JFrameFactoryData;
}

JFrameFactory::~IJFrameFactory()
{
    delete data;
}
