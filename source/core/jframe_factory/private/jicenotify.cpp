#include "precomp.h"
#include "jicenotify.h"

JIceNotify::JIceNotify(JNotifier &notifier, QObject *parent)
    : QObject(parent)
    , q_notifier(notifier)
    , q_proxy(0)
{

}

JIceNotify::~JIceNotify()
{

}

bool JIceNotify::isConnected()
{
    return false;
}

bool JIceNotify::initialize(const std::string &service)
{
    bool result = true;

    //
    Q_UNUSED(service);

    return result;
}

bool JIceNotify::setCurrentService(const std::string &service)
{
    Q_UNUSED(service);
    return true;
}

int JIceNotify::sendBuffer(const std::string &domain, const char *buffer, int size)
{
    if (q_proxy) {
        return q_proxy->sendBuffer(domain, std::make_pair<::Ice::Byte*, ::Ice::Byte*>(
                                       (::Ice::Byte*)buffer, (::Ice::Byte*)(buffer + size)));
    } else {
        return IIceNotify::ProxyInvalid;
    }
}

int JIceNotify::sendString(const std::string &domain, const std::string &msg)
{
    if (q_proxy) {
        return q_proxy->sendString(domain, msg);
    } else {
        return IIceNotify::ProxyInvalid;
    }
}

int JIceNotify::sendList(const std::string &domain, const std::list<std::string> &data)
{
    if (q_proxy) {
        return q_proxy->sendList(domain, data);
    } else {
        return IIceNotify::ProxyInvalid;
    }
}

int JIceNotify::sendVariant(const QString &domain, const QVariant &variant)
{
    if (q_proxy) {
        return 0;
    } else {
        return IIceNotify::ProxyInvalid;
    }
}

void JIceNotify::postBuffer(const std::string &domain, const char *buffer, int size)
{
    if (q_proxy) {
        q_proxy->postBuffer(domain, std::make_pair<::Ice::Byte*, ::Ice::Byte*>(
                                (::Ice::Byte*)buffer, (::Ice::Byte*)(buffer + size)));
    }
}

void JIceNotify::postString(const std::string &domain, const std::string &msg)
{
    if (q_proxy) {
        q_proxy->postString(domain, msg);
    }
}

void JIceNotify::postList(const std::string &domain, const std::list<std::string> &data)
{
    if (q_proxy) {
        q_proxy->postList(domain, data);
    }
}

void JIceNotify::postVariant(const QString &domain, const QVariant &variant)
{
    if (!q_proxy) {
        return;
    }

    //
}
