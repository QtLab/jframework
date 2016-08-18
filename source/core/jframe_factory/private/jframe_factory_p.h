#ifndef JFRAME_FACTORY_P_H
#define JFRAME_FACTORY_P_H

#include "../jframe_factory.h"
#include <QMutex>

// class JFrameFactory

struct JFrameFactoryData;
class INotifier;

class JFrameFactory : public IJFrameFactory
{
    // single instance
public:
    static JFrameFactory* getInstance();
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

    // IJFrameFactory interface
public:
    void *createFactory(const std::string &iid, unsigned int ver);
    void releaseFactory(void *iface, const std::string &iid, unsigned int ver);

private:
    JFrameFactory();
    ~JFrameFactory();

    JFrameFactoryData* data;

    //
    static QMutex _instance_mutex;      // 单实例保护锁
    static JFrameFactory* _instance;    // 单实例指针
};

#endif // JFRAME_FACTORY_P_H
