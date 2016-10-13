#ifndef __IceNotifyI_h__
#define __IceNotifyI_h__

#include <IceNotify.h>

namespace Notify
{

class JIceNotifyI : virtual public JIceNotify
{
public:

    virtual ::Ice::Int sendBuffer(const ::std::string&,
                                  const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>&,
                                  const Ice::Current&);

    virtual ::Ice::Int sendString(const ::std::string&,
                                  const ::std::string&,
                                  const Ice::Current&);

    virtual ::Ice::Int sendList(const ::std::string&,
                                const ::Notify::StringList&,
                                const Ice::Current&);

    virtual void postBuffer(const ::std::string&,
                            const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>&,
                            const Ice::Current&);

    virtual void postString(const ::std::string&,
                            const ::std::string&,
                            const Ice::Current&);

    virtual void postList(const ::std::string&,
                          const ::Notify::StringList&,
                          const Ice::Current&);
};

}

#endif
