#include "precomp.h"
#include "jicenotify.h"

JIceNotify::JIceNotify(JNotifier &notifier, QObject *parent)
    : QObject(parent)
    , q_notifier(notifier)
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

    return result;
}


bool JIceNotify::setCurrentService(const std::string &service)
{
    return true;
}

int JIceNotify::sendBuffer(const std::string &domain, const char *buffer, int size)
{
    return 0;
}

int JIceNotify::sendString(const std::string &domain, const std::string &msg)
{
    return 0;
}

int JIceNotify::sendList(const std::string &domain, const std::list<std::string> &data)
{
    return 0;
}

int JIceNotify::sendVariant(const QString &domain, const QVariant &variant)
{
    return 0;
}

void JIceNotify::postBuffer(const std::string &domain, const char *buffer, int size)
{
}

void JIceNotify::postString(const std::string &domain, const std::string &msg)
{
}

void JIceNotify::postList(const std::string &domain, const std::list<std::string> &data)
{
}

void JIceNotify::postVariant(const QString &domain, const QVariant &variant)
{
}
