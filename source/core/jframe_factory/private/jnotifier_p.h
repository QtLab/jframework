#ifndef JNOTIFIER_P_H
#define JNOTIFIER_P_H

#include "../jnotifier.h"
#include <QObject>
#include <qmetatype.h>
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
    std::string domain;
    JWPARAM wParam;
    JLPARAM lParam;
    std::string info;   // map to lParam

    JNotifyMsg() : observer(0), domain(""), wParam(0), lParam(0), info("") {}
    JNotifyMsg(JObserver *observer, const std::string &domain, JWPARAM wParam, JLPARAM lParam,
               const std::string &info) : observer(observer), domain(domain), wParam(wParam),
        lParam(lParam), info(info) {}

    JNotifyMsg(const JNotifyMsg &other)
    { *this = other; }

    JNotifyMsg& operator =(const JNotifyMsg &other)
    {
        if (this == &other) {
            return *this;
        }

        observer = other.observer;
        domain = other.domain;
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
    JLRESULT sendMessage(const std::string &domain, JWPARAM wParam, JLPARAM lParam);
    JLRESULT sendMessage(JObserver *observer, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    void postMessage(const std::string &domain, JWPARAM wParam, JLPARAM lParam);
    void postMessage(JObserver *observer, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    void postMessage(const std::string &domain, const std::string &info, JLPARAM lParam);
    void postMessage(JObserver *observer, const std::string &id, const std::string &info, JLPARAM lParam);

    IDBusNotify &dbus();
    IIceNotify &ice();

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
    JLRESULT dispense(JObserver *observer, const std::string &domain, JWPARAM wParam, JLPARAM lParam);
    JLRESULT dispense(JObserver *observer, const std::string &domain, JWPARAM wParam, JLPARAM lParam,
                      const observer_info &info);

    //
    static void task(JNotifier *receiver, JNotifyMsg msg);

    // for IDBusNotify
private:

    // for IIceNotify
private:

private:
    JNotifierPrivate* d;
};

/**
 * @brief IIceNotify 实现
 */
class JIceNotify : public IIceNotify
{
public:
    explicit JIceNotify(JNotifier &notifier);

    // IDBusNotify interface
public:
    bool isConnected();

private:
    JNotifier &q_notifier;
};

#endif // JNOTIFIER_P_H
