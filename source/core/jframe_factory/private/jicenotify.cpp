#include "precomp.h"
#include "jicenotify.h"
#ifdef ICE_LIB
#include <Ice/Ice.h>

// class JIceNotify

JIceNotify::JIceNotify(JNotifier &notifier, QObject *parent)
    : QObject(parent)
    , q_notifier(notifier)
    , q_port(0)
    , q_commPtr(0)
    , q_adapterPtr(0)
    , q_proxy(0)
    , q_iceNotifyIPtr(0)
{

}

JIceNotify::~JIceNotify()
{

}

void JIceNotify::shutdown()
{
    //
    if (q_commPtr) {
        q_commPtr->shutdown();
    }
}

bool JIceNotify::isConnected()
{
    return false;
}

bool JIceNotify::initialize(const std::string &service, const std::string &host, unsigned int port)
{
    bool result = true;

    //
    q_service = service;
    q_host = host;
    q_port = port;

    //
    try {
        // arguments
        const QStringList args = QApplication::arguments();
        ::Ice::StringSeq _args;
        QStringListIterator citerArgs(args);
        while (citerArgs.hasNext()) {
            _args.push_back(citerArgs.next().toStdString());
        }
        //
        q_commPtr = ::Ice::initialize(_args);
        if (!q_commPtr) {
            throw "invalid communicator!";
        }
        // serice
        QThreadPool::globalInstance()->start(this);

        // client
        if (!setCurrentService(host, port)) {
            //return false;
        }
    }
    catch (const Ice::Exception& e) {
        std::cout << e << std::endl;
        return false;
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
        return false;
    }

    return result;
}

bool JIceNotify::setCurrentService(const std::string &service)
{
    //
    const QString _service = QString::fromStdString(service);
    const std::string host = _service.section(':', 0, 0, QString::SectionSkipEmpty).trimmed().toStdString();
    const unsigned int port = _service.section(':', 1, 1, QString::SectionSkipEmpty).toUInt();

    //
    return setCurrentService(host, port);
}

bool JIceNotify::setCurrentService(const std::string &host, unsigned int port)
{
    if (!q_commPtr) {
        return false;
    }

    //
    try {
        //
        Ice::ObjectPrx base = q_commPtr->stringToProxy(
                    "JIceNotify:default -h " + host + " -p " + QString::number(port).toStdString());
        if (!base) {
            throw "invalid proxy!";
        }
        q_proxy = ::Notify::JIceNotifyPrx::checkedCast(base);
        if (!q_proxy) {
            throw "invalid proxy!";
        }
    }
    catch (const Ice::Exception& e) {
        std::cout << e << std::endl;
        return false;
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
        return false;
    }

    return true;
}

int JIceNotify::sendBuffer(const std::string &domain, const char *buffer, int size)
{
    ::Notify::JIceNotifyPrx proxy = parseDomain(domain);
    if (proxy) {
        return proxy->sendBuffer(domain, std::make_pair<::Ice::Byte*, ::Ice::Byte*>(
                                     (::Ice::Byte*)buffer, (::Ice::Byte*)(buffer + size)));
    } else {
        return IIceNotify::ProxyInvalid;
    }
}

int JIceNotify::sendString(const std::string &domain, const std::string &msg)
{
    ::Notify::JIceNotifyPrx proxy = parseDomain(domain);
    if (proxy) {
        return proxy->sendString(domain, msg);
    } else {
        return IIceNotify::ProxyInvalid;
    }
}

int JIceNotify::sendList(const std::string &domain, const std::list<std::string> &data)
{
    ::Notify::JIceNotifyPrx proxy = parseDomain(domain);
    if (proxy) {
        return proxy->sendList(domain, data);
    } else {
        return IIceNotify::ProxyInvalid;
    }
}

int JIceNotify::sendVariant(const QString &domain, const QVariant &variant)
{
    ::Notify::JIceNotifyPrx proxy = parseDomain(domain.toStdString());
    if (proxy) {
        std::string s;
        switch (variant.type()) {
        case QVariant::Bool:
            s = variant.toString().toStdString();
            break;
        default:
            return IIceNotify::VariantNotSupported;
        }
        return proxy->sendVariant(domain.toStdString(), s);
    } else {
        return IIceNotify::ProxyInvalid;
    }
}

void JIceNotify::postBuffer(const std::string &domain, const char *buffer, int size)
{
    ::Notify::JIceNotifyPrx proxy = parseDomain(domain);
    if (proxy) {
        proxy->postBuffer(domain, std::make_pair<::Ice::Byte*, ::Ice::Byte*>(
                              (::Ice::Byte*)buffer, (::Ice::Byte*)(buffer + size)));
    }
}

void JIceNotify::postString(const std::string &domain, const std::string &msg)
{
    ::Notify::JIceNotifyPrx proxy = parseDomain(domain);
    if (proxy) {
        proxy->postString(domain, msg);
    }
}

void JIceNotify::postList(const std::string &domain, const std::list<std::string> &data)
{
    ::Notify::JIceNotifyPrx proxy = parseDomain(domain);
    if (proxy) {
        proxy->postList(domain, data);
    }
}

void JIceNotify::postVariant(const QString &domain, const QVariant &variant)
{
    ::Notify::JIceNotifyPrx proxy = parseDomain(domain.toStdString());
    if (!proxy) {
        std::string s;
        switch (variant.type()) {
        case QVariant::Bool:

            break;
        default:
            return;
        }
        proxy->sendVariant(domain.toStdString(), s);
    }

    //
}

void JIceNotify::run()
{
    if (!q_commPtr) {
        return;
    }

    try {
        // server
        q_adapterPtr = q_commPtr->createObjectAdapterWithEndpoints(
                    q_service, "default -p " + QString::number(q_port).toStdString());
        if (!q_adapterPtr) {
            throw "invalid adapter!";
        }
        //
        q_iceNotifyIPtr = new ::Notify::JIceNotifyI(q_notifier);
        //
        q_adapterPtr->add(q_iceNotifyIPtr, ::Ice::stringToIdentity("JIceNotify"));
        //
        q_adapterPtr->activate();
        //
        q_commPtr->waitForShutdown();
        //
    }
    catch (const Ice::Exception& e) {
        std::cout << e << std::endl;
        return;
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
        return;
    }

    //
    if (q_commPtr) {
        q_commPtr->destroy();
        q_commPtr = 0;
    }
}

::Notify::JIceNotifyPrx JIceNotify::parseDomain(const std::string &domain)
{
    //
    const QString _domain = QString::fromStdString(domain);
    // ice-service
    QString iceService = _domain.section('@', 3, 3, QString::SectionSkipEmpty).trimmed();
    // dbus-domain
    q_dbusDomain = _domain.section('@', 0, 2, QString::SectionSkipEmpty).trimmed().toStdString();
    //
    if (iceService.isEmpty()) {
        return q_proxy;   //
    }
    //
    try {
        Ice::ObjectPrx base = q_commPtr->stringToProxy(
                "JIceNotify:default -h " + iceService.replace(":", " -p ").toStdString());
        if (!base) {
            throw "invalid proxy!";
        }
        //
        ::Notify::JIceNotifyPrx proxy = ::Notify::JIceNotifyPrx::checkedCast(base);
        if (!proxy) {
            throw "invalid proxy!";
        }
        //
        return proxy;
    }
    catch (const Ice::Exception& e) {
        std::cout << e << std::endl;
        return 0;
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
        return 0;
    }

    return 0;
}

#endif
