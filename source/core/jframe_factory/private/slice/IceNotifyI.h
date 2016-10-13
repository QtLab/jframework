#ifndef __IceNotifyI_h__
#define __IceNotifyI_h__

#include <IceNotify.h>

namespace notify
{

class JIceNotifyDispI : virtual public JIceNotifyDisp
{
public:

    virtual ::Ice::Int sendBuffer(const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>&,
                                  const Ice::Current&);

    virtual ::Ice::Int sendString(const ::std::string&,
                                  const Ice::Current&);

    virtual ::Ice::Int sendList(const ::notify::StringList&,
                                const Ice::Current&);

    virtual void postBuffer(const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>&,
                            const Ice::Current&);

    virtual void postString(const ::std::string&,
                            const Ice::Current&);

    virtual void postList(const ::notify::StringList&,
                          const Ice::Current&);
};

}

#endif
