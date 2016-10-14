#ifndef __IceNotifyI_h__
#define __IceNotifyI_h__

#include <IceNotify.h>

class JNotifier;

namespace Notify
{

class JIceNotifyI : virtual public JIceNotify
{
public:
    explicit JIceNotifyI(JNotifier &notifier);
    ~JIceNotifyI();

    virtual ::Ice::Int sendBuffer(const ::std::string&,
                                  const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>&,
                                  const Ice::Current&);

    virtual ::Ice::Int sendString(const ::std::string&,
                                  const ::std::string&,
                                  const Ice::Current&);

    virtual ::Ice::Int sendList(const ::std::string&,
                                const ::Notify::StringList&,
                                const Ice::Current&);

    virtual ::Ice::Int sendVariant(const ::std::string&,
                                   const ::std::string&,
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

    virtual void postVariant(const ::std::string&,
                             const ::std::string&,
                             const Ice::Current&);

private:
    JNotifier &q_notifier;
};

}

#endif
