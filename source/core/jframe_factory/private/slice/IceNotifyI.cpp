#include "precomp.h"
#include "IceNotifyI.h"
#include "../jnotifier_p.h"

Notify::JIceNotifyI::JIceNotifyI(JNotifier &notifier)
    : q_notifier(notifier)
{

}

Notify::JIceNotifyI::~JIceNotifyI()
{

}

::Ice::Int
Notify::JIceNotifyI::sendBuffer(const ::std::string& domain,
                                const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>& buffer,
                                const Ice::Current&)
{
    return q_notifier.dbus().sendBuffer(domain,
                                        (const char*)buffer.first,
                                        (int)(buffer.second - buffer.first));
}

::Ice::Int
Notify::JIceNotifyI::sendString(const ::std::string& domain,
                                const ::std::string& msg,
                                const Ice::Current&)
{
    return q_notifier.dbus().sendString(domain, msg);
}

::Ice::Int
Notify::JIceNotifyI::sendList(const ::std::string& domain,
                              const ::Notify::StringList& data,
                              const Ice::Current&)
{
    return q_notifier.dbus().sendList(domain, data);
}

::Ice::Int
Notify::JIceNotifyI::sendVariant(const ::std::string& domain,
                                 const ::std::string& s,
                                 const Ice::Current&)
{
    Q_UNUSED(domain);
    Q_UNUSED(s);
    return 0;
}

void
Notify::JIceNotifyI::postBuffer(const ::std::string& domain,
                                const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>& buffer,
                                const Ice::Current&)
{
    q_notifier.dbus().sendBuffer(domain,
                                 (const char*)buffer.first,
                                 (int)(buffer.second - buffer.first));
}

void
Notify::JIceNotifyI::postString(const ::std::string& domain,
                                const ::std::string& msg,
                                const Ice::Current&)
{
    q_notifier.dbus().postString(domain, msg);
}

void
Notify::JIceNotifyI::postList(const ::std::string& domain,
                              const ::Notify::StringList& data,
                              const Ice::Current&)
{
    q_notifier.dbus().postList(domain, data);
}

void
Notify::JIceNotifyI::postVariant(const ::std::string& domain,
                                 const ::std::string& s,
                                 const Ice::Current&)
{
    Q_UNUSED(domain);
    Q_UNUSED(s);
}
