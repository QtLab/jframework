#ifndef JCOMPONENT_NOTIFY_H
#define JCOMPONENT_NOTIFY_H

#include <QObject>
#include "../../jframe_core.h"
#include <qmetatype.h>

//
typedef std::pair<std::string, JMsgSinkCb> pair_id_cb;
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
typedef std::map<IJComponent*, observer_info> map_observer_info;

// - struct JComNotifyMsg -

struct JComNotifyMsg
{
    IJComponent* observer;
    std::string id;
    JWPARAM wParam;
    JLPARAM lParam;
    std::string msg;   // map to lParam

    JComNotifyMsg() : observer(0), id(""), wParam(0), lParam(0), msg("") {}
    JComNotifyMsg(IJComponent *observer, const std::string &id, JWPARAM wParam, JLPARAM lParam,
               const std::string &msg) : observer(observer), id(id), wParam(wParam),
        lParam(lParam), msg(msg) {}

    JComNotifyMsg(const JComNotifyMsg &other)
    { *this = other; }

    JComNotifyMsg& operator =(const JComNotifyMsg &other)
    {
        if (this == &other) {
            return *this;
        }

        observer = other.observer;
        id = other.id;
        wParam = other.wParam;
        lParam = other.lParam;
        msg = other.msg;

        return *this;
    }
};
Q_DECLARE_METATYPE(JComNotifyMsg)

// class JComponentNotify

class JComNotifyData;

class JComponentNotify : public QObject
{
    Q_OBJECT
public:
    explicit JComponentNotify(QObject *parent = 0);
    ~JComponentNotify();

    void beginGroup(IJComponent *component, int offset = 0);
    void endGroup();
    void subMessage(const std::string &id, JMsgSinkCb cb = 0);
    void unsubMessage(const std::string &id);
    void unsubMessage(IJComponent *component);
    JLRESULT sendMessage(IJComponent *component, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0);
    void postMessage(IJComponent *component, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0);
    void postMessage(IJComponent *component, const std::string &id, const std::string &msg);

Q_SIGNALS:
    void readyDispense(const JComNotifyMsg &msg);

public Q_SLOTS:
    void dispense(const JComNotifyMsg &msg);

private:
    static void task(JComponentNotify *receiver, JComNotifyMsg msg);

    //
    JLRESULT dispense(IJComponent *component, const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT dispense(IJComponent *component, const std::string &id, JWPARAM wParam, JLPARAM lParam,
                      const observer_info &info);

private:
    JComNotifyData *d;
};

#endif // JCOMPONENT_NOTIFY_H
