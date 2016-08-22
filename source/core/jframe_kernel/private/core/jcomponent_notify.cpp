#include "precomp.h"
#include "jcomponent_notify.h"

// - class JComNotifyData -

class JComNotifyData
{
    friend class JComponentNotify;
public:
    JComNotifyData() :
        currentComponent(0),
        currentOffset(0)
    {

    }

private:
    IJComponent* currentComponent;
    int currentOffset;
    map_observer_info mapObs;
};

// - class JComponentNotify -

JComponentNotify::JComponentNotify(QObject *parent)
    : QObject(parent)
{
    d = new JComNotifyData();

    //
    qRegisterMetaType<JComNotifyMsg>();

    //
    connect(this, SIGNAL(readyDispense(JComNotifyMsg)), SLOT(dispense(JComNotifyMsg)),
            Qt::QueuedConnection);
}

JComponentNotify::~JComponentNotify()
{
    delete d;
}

void JComponentNotify::beginGroup(IJComponent *component, int offset)
{
    d->currentComponent = component;
    d->currentOffset = offset;
}

void JComponentNotify::endGroup()
{
    d->currentComponent = 0;
    d->currentOffset = 0;
}

void JComponentNotify::subMessage(const std::string &id, JMsgSinkCb cb)
{
    if (!d->currentComponent) {
        Q_ASSERT(d->currentComponent);
        return;
    }

    map_observer_info::iterator iterMap = d->mapObs.find(d->currentComponent);
    if (iterMap != d->mapObs.end()) {
        list_pair_id_cb &pids = iterMap->second.pids;
        list_pair_id_cb::iterator iterPids = pids.begin();
        for (; iterPids != pids.end(); ++iterPids) {
            pair_id_cb &pid = *iterPids;
            if (pid.first == id) {
                break;
            }
        }

        if (iterPids != pids.end()) {
            return;
        }

        pids.push_back(pair_id_cb(id, cb));
    } else {
        list_pair_id_cb pids;
        pids.push_back(pair_id_cb(id, cb));
        d->mapObs[d->currentComponent] = observer_info(d->currentOffset, pids);
    }
}

void JComponentNotify::unsubMessage(const std::string &id)
{
    if (!d->currentComponent) {
        Q_ASSERT(false);
        return;
    }

    map_observer_info::iterator iterMap = d->mapObs.find(d->currentComponent);
    if (iterMap == d->mapObs.end()) {
        return;
    }

    list_pair_id_cb &pids = iterMap->second.pids;
    if (!pids.empty()) {
        list_pair_id_cb::iterator iterPids = pids.begin();
        for (; iterPids != pids.end(); ++iterPids) {
            pair_id_cb &pid = *iterPids;
            if (pid.first == id) {
                pids.erase(iterPids);
                break;
            }
        }
    }

    if (pids.empty()) {
        d->mapObs.erase(d->currentComponent);
    }
}

void JComponentNotify::unsubMessage(IJComponent *component)
{
    if (!component) {
        Q_ASSERT(component);
        return;
    }

    d->mapObs.erase(component);
}

JLRESULT JComponentNotify::sendMessage(IJComponent *component, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    return dispense(component, id, wParam, lParam);
}

void JComponentNotify::postMessage(IJComponent *component, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    QtConcurrent::run(task, this, JComNotifyMsg(component, id, wParam, lParam, ""));
}

void JComponentNotify::postMessage(IJComponent *component, const std::string &id, const std::string &msg)
{
    QtConcurrent::run(task, this, JComNotifyMsg(component, id, 0, 0, msg));
}

void JComponentNotify::dispense(const JComNotifyMsg &msg)
{
    if (msg.msg.empty()) {
        dispense(msg.observer, msg.id, msg.wParam, msg.lParam);
    } else {
        dispense(msg.observer, msg.id, (JWPARAM)&msg.msg, msg.lParam);
    }
}

void JComponentNotify::task(JComponentNotify *receiver, JComNotifyMsg msg)
{
    if (receiver) {
        Q_EMIT receiver->readyDispense(msg);
    }
}

JLRESULT JComponentNotify::dispense(IJComponent *component, const std::string &id,
                                    JWPARAM wParam, JLPARAM lParam)
{
    JLRESULT result = 0;

    if (component) {
        map_observer_info::const_iterator citerMap = d->mapObs.find(component);
        if (citerMap != d->mapObs.end()) {
            return dispense(citerMap->first, id, wParam, lParam, citerMap->second);
        }
    }

    map_observer_info::const_iterator citerMap = d->mapObs.begin();
    for (; citerMap != d->mapObs.end(); ++citerMap) {
        result = dispense(citerMap->first, id, wParam, lParam, citerMap->second);
    }

    return result;
}

JLRESULT JComponentNotify::dispense(IJComponent *component, const std::string &id,
                                    JWPARAM wParam, JLPARAM lParam, const observer_info &info)
{
    JLRESULT result = 0;
    const int offset = info.offset;
    const list_pair_id_cb &pids = info.pids;
    list_pair_id_cb::const_iterator citerPids = pids.begin();
    for (; citerPids != pids.end(); ++citerPids) {
        const pair_id_cb &pid = *citerPids;
        if (pid.first == id) {
            if (pid.second) {
                result = ((component + offset)->*(pid.second))(component, id, wParam, lParam);
            } else {
                IJMessageSink *msgSink = reinterpret_cast<IJMessageSink *>
                        (component->queryInterface(IID_IJMessageSink, VER_IJMessageSink));
                if (msgSink) {
                    result = msgSink->messageSink(component, id, wParam, lParam);
                }
            }
            break;
        }
    }

    return result;
}
