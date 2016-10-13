
#include <IceNotifyI.h>

::Ice::Int
notify::JIceNotifyDispI::sendBuffer(const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>& buffer,
                                    const Ice::Current& current)
{
    return 0;
}

::Ice::Int
notify::JIceNotifyDispI::sendString(const ::std::string& msg,
                                    const Ice::Current& current)
{
    return 0;
}

::Ice::Int
notify::JIceNotifyDispI::sendList(const ::notify::StringList& data,
                                  const Ice::Current& current)
{
    return 0;
}

void
notify::JIceNotifyDispI::postBuffer(const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>& buffer,
                                    const Ice::Current& current)
{
}

void
notify::JIceNotifyDispI::postString(const ::std::string& msg,
                                    const Ice::Current& current)
{
}

void
notify::JIceNotifyDispI::postList(const ::notify::StringList& data,
                                  const Ice::Current& current)
{
}
