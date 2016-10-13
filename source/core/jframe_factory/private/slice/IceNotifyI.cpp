
#include <IceNotifyI.h>

::Ice::Int
Notify::JIceNotifyI::sendBuffer(const ::std::string& domain,
                                const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>& buffer,
                                const Ice::Current& current)
{
    return 0;
}

::Ice::Int
Notify::JIceNotifyI::sendString(const ::std::string& domain,
                                const ::std::string& msg,
                                const Ice::Current& current)
{
    return 0;
}

::Ice::Int
Notify::JIceNotifyI::sendList(const ::std::string& domain,
                              const ::Notify::StringList& data,
                              const Ice::Current& current)
{
    return 0;
}

void
Notify::JIceNotifyI::postBuffer(const ::std::string& domain,
                                const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>& buffer,
                                const Ice::Current& current)
{
}

void
Notify::JIceNotifyI::postString(const ::std::string& domain,
                                const ::std::string& msg,
                                const Ice::Current& current)
{
}

void
Notify::JIceNotifyI::postList(const ::std::string& domain,
                              const ::Notify::StringList& data,
                              const Ice::Current& current)
{
}
