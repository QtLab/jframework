#ifndef JICENOTIFY_H
#define JICENOTIFY_H

#include <QObject>
#include "../jnotifier.h"
#ifdef ICE_LIB
#include "slice/IceNotifyI.h"
#include <Ice/ObjectAdapter.h>
#include <QRunnable>

class JNotifier;

class JIceNotify : public QObject, public IIceNotify, public QRunnable
{
    Q_OBJECT
public:
    explicit JIceNotify(JNotifier &notifier, QObject *parent = 0);
    ~JIceNotify();

    void shutdown();

    // IIceNotify interface
public:
    bool isConnected();
    bool initialize(const std::string &service, const std::string &host, unsigned int port);
    bool setCurrentService(const std::string &service);
    bool setCurrentService(const std::string &host, unsigned int port);

    int sendBuffer(const std::string &domain, const char *buffer, int size);
    int sendString(const std::string &domain, const std::string &msg);
    int sendList(const std::string &domain, const std::list<std::string> &data);
    int sendVariant(const QString &domain, const QVariant &variant);

    void postBuffer(const std::string &domain, const char *buffer, int size);
    void postString(const std::string &domain, const std::string &msg);
    void postList(const std::string &domain, const std::list<std::string> &data);
    void postVariant(const QString &domain, const QVariant &variant);

    // QRunnable interface
public:
    virtual void run();

signals:

public slots:

private:
    ::Notify::JIceNotifyPrx parseDomain(const std::string &domain);

private:
    JNotifier &q_notifier;
    std::string q_service;
    std::string q_host;
    unsigned int q_port;
    ::Ice::CommunicatorPtr q_commPtr;
    ::Ice::ObjectAdapter::PointerType q_adapterPtr;
    ::Notify::JIceNotify::ProxyType q_proxy;
    ::Notify::JIceNotifyI::PointerType q_iceNotifyIPtr;
    //
    std::string q_dbusDomain;
};

#else

class JNotifier;

class JIceNotify : public QObject, public IIceNotify
{
    Q_OBJECT
public:
    explicit JIceNotify(JNotifier &, QObject * = 0) {}
    ~JIceNotify() {}

    void shutdown(){}

    // IIceNotify interface
public:
    bool isConnected() { return false; }
    bool initialize(const std::string &, const std::string &, unsigned int) { return false; }
    bool setCurrentService(const std::string &) { return false; }
    bool setCurrentService(const std::string &, unsigned int) { return false; }

    int sendBuffer(const std::string &, const char *, int) { return 0; }
    int sendString(const std::string &, const std::string &) { return 0; }
    int sendList(const std::string &, const std::list<std::string> &) { return 0; }
    int sendVariant(const QString &, const QVariant &) { return 0; }

    void postBuffer(const std::string &, const char *, int) { }
    void postString(const std::string &, const std::string &) { }
    void postList(const std::string &, const std::list<std::string> &) { }
    void postVariant(const QString &, const QVariant &) { }
};

#endif

#endif // JICENOTIFY_H
