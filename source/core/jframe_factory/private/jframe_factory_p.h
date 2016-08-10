#ifndef JFRAME_FACTORY_H
#define JFRAME_FACTORY_H

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
    void releaseInterface();
    void *queryInterface(const char *iid, unsigned int ver);

    // IJObject interface
public:
    std::string objectIdentity() const;
    unsigned int objectVersion() const;

    // IJFrameFactory interface
public:
    void *factory(const char *iid, unsigned int ver);
    void releaseFactory(void *interface, const char *iid, unsigned int ver);

private:
    JFrameFactory();
    ~JFrameFactory();

    JFrameFactoryData* data;

    //
    static QMutex _instance_mutex;      // 单实例保护锁
    static JFrameFactory* _instance;    // 单实例指针
};

#endif // JFRAME_FACTORY_H
