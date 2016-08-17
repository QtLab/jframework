#include "precomp.h"
#include "jframe_factory_p.h"
#include "jnotifier_p.h"
#include "jlogmanager_p.h"

// struct JFrameFactoryData

struct JFrameFactoryData
{
    JFrameFactoryData()
    {

    }
};

// class JFrameFactory

//
QMutex JFrameFactory::_instance_mutex;
JFrameFactory* JFrameFactory::_instance = 0;

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

std::string JFrameFactory::interfaceIdentity() const
{
    return IID_IJFrameFactory;
}

unsigned int JFrameFactory::interfaceVersion() const
{
    return VER_IJFrameFactory;
}

void *JFrameFactory::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);

    return 0;
}

bool JFrameFactory::loadInterface()
{
    bool result = true;

    //

    return result;
}

void JFrameFactory::releaseInterface()
{

}

std::list<std::string> JFrameFactory::queryMethod() const
{
    std::list<std::string> methods;

    //

    return methods;
}

bool JFrameFactory::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;

    va_list ap;
    va_start(ap, argc);

    Q_UNUSED(method);

    va_end(ap);

    return result;
}

void *JFrameFactory::factory(const std::string &iid, unsigned int ver)
{
    // 创建消息分发器
    if (J_IS_INSTANCEOF(INotifier, iid, ver)) {
        return static_cast<INotifier *>(new JNotifier());
    }
    // 创建日志管理器
    else if (J_IS_INSTANCEOF(IJLogManager, iid, ver)) {
        return static_cast<IJLogManager *>(new JLogManagerPri());
    }

    return 0;
}

void JFrameFactory::releaseFactory(void *iface, const std::string &iid, unsigned int ver)
{
    // 销毁消息分发器
    if (J_IS_INSTANCEOF(INotifier, iid, ver)) {
        INotifier *notifier = reinterpret_cast<INotifier *>(iface);
        if (notifier) {
            delete notifier;
        }
    }
    // 销毁日志管理器
    else if (J_IS_INSTANCEOF(IJLogManager, iid, ver)) {
        IJLogManager *logManager = reinterpret_cast<IJLogManager *>(iface);
        if (logManager) {
            delete logManager;
        }
    }
}

JFrameFactory::JFrameFactory()
{
    data = new JFrameFactoryData;
}

JFrameFactory::~JFrameFactory()
{
    delete data;
}
