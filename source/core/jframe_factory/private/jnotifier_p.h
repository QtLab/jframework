#ifndef JNOTIFIER_P_H
#define JNOTIFIER_P_H

#include "../jnotifier.h"
#include <QObject>
#include <map>
#include <vector>

//
typedef std::pair<std::string, jobserver_cb> pair_id_cb;
typedef std::list<pair_id_cb> list_pair_id_cb;

//
struct observer_info
{
    int offset;     // offset between observer and child object
    list_pair_id_cb pids;

    observer_info() : offset(0) {}
    observer_info(int offset, const list_pair_id_cb &pids) : offset(offset), pids(pids) {}
    observer_info(const observer_info &other)
    { *this = other; }

    observer_info& operator =(const observer_info &other)
    {
        if (this == &other) {
            return *this;
        }

        offset = other.offset;
        pids = other.pids;

        return *this;
    }
};

//
typedef std::map<JObserver*, observer_info> map_observer_info;

// - struct JNotifyMsg -

struct JNotifyMsg
{
    JObserver* observer;
    std::string id;
    JWPARAM wParam;
    JLPARAM lParam;
    std::string info;   // map to lParam

    JNotifyMsg() : observer(0), id(""), wParam(0), lParam(0), info("") {}
    JNotifyMsg(JObserver *observer, const std::string &id, JWPARAM wParam, JLPARAM lParam,
               const std::string &info) : observer(observer), id(id), wParam(wParam),
        lParam(lParam), info(info) {}

    JNotifyMsg(const JNotifyMsg &other)
    { *this = other; }

    JNotifyMsg& operator =(const JNotifyMsg &other)
    {
        if (this == &other) {
            return *this;
        }

        observer = other.observer;
        id = other.id;
        wParam = other.wParam;
        lParam = other.lParam;
        info = other.info;

        return *this;
    }
};
Q_DECLARE_METATYPE(JNotifyMsg)

// - class JNotifier -

class JNotifierPrivate;

class JNotifier :
        public QObject,
        public INotifier
{
    Q_OBJECT
public:
    explicit JNotifier(QObject *parent = 0);
    ~JNotifier();

    // IJUnknown interface
public:
    std::string interfaceIdentity() const;
    unsigned int interfaceVersion() const;
    void releaseInterface();

    // INotifier interface
public:
    INotifier &endGroup();
    void remove(JObserver *obs);
    INotifier &remove(JObserver *obs, const std::string &id);
    INotifier &remove(const std::string &id);
    void clear();
    JLRESULT sendMessage(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    void postMessage(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    void postMessage(const std::string &id, const std::string &info, JLPARAM lParam);
    INotifierImm &imm();

protected:
    INotifier &beginGroup(JObserver *obs, int offset);
    INotifier &append(JObserver *obs, const std::string &id, jobserver_cb cb, int offset);
    INotifier &append(const std::string &id, jobserver_cb cb);

Q_SIGNALS:
    void readyDispense(const JNotifyMsg &msg);

public Q_SLOTS:
    void dispense(const JNotifyMsg &msg);

private:
    //
    JLRESULT dispense(JObserver *obs, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT dispense(JObserver *obs, const std::string &id, JWPARAM wParam, JLPARAM lParam,
                      const observer_info &info);

    //
    static void task(JNotifier *receiver, JNotifyMsg msg);

    //
    JLRESULT immSendMessage(const std::string &obsid, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT immSendMessage(JObserver *obs, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    void immPostMessage(const std::string &obsid, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    void immPostMessage(JObserver *obs, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    void immPostMessage(const std::string &obsid, const std::string &id, const std::string &info, JLPARAM lParam);
    void immPostMessage(JObserver *obs, const std::string &id, const std::string &info, JLPARAM lParam);

private:
    JNotifierPrivate* d;
    friend class JNotifierImm;
};

// - class JNotifierImm -

class JNotifierImm : public INotifierImm
{
public:
    explicit JNotifierImm(JNotifier &notifier);

    // INotifierImm interface
public:
    inline JLRESULT sendMessage(const std::string &obsid, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    inline JLRESULT sendMessage(JObserver *obs, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    inline void postMessage(const std::string &obsid, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    inline void postMessage(JObserver *obs, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    inline void postMessage(const std::string &obsid, const std::string &id, const std::string &info, JLPARAM lParam);
    inline void postMessage(JObserver *obs, const std::string &id, const std::string &info, JLPARAM lParam);

private:
    JNotifier& q_notifier;
};

inline
JLRESULT JNotifierImm::sendMessage(const std::string &obsid, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    return q_notifier.immSendMessage(obsid, id, wParam, lParam);
}

inline
JLRESULT JNotifierImm::sendMessage(JObserver *obs, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    return q_notifier.immSendMessage(obs, id, wParam, lParam);
}

inline
void JNotifierImm::postMessage(const std::string &obsid, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    return q_notifier.immPostMessage(obsid, id, wParam, lParam);
}

inline
void JNotifierImm::postMessage(JObserver *obs, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    return q_notifier.immPostMessage(obs, id, wParam, lParam);
}

inline
void JNotifierImm::postMessage(const std::string &obsid, const std::string &id, const std::string &info, JLPARAM lParam)
{
    return q_notifier.immPostMessage(obsid, id, info, lParam);
}

inline
void JNotifierImm::postMessage(JObserver *obs, const std::string &id, const std::string &info, JLPARAM lParam)
{
    return q_notifier.immPostMessage(obs, id, info, lParam);
}

#endif // JNOTIFIER_P_H
