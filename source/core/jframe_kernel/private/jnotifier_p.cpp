#include "precomp.h"
#include "jnotifier_p.h"
#include "../jnotifier.h"
#include "../jframe_kernel.h"
#include <QtConcurrentRun>

// 获取分发器实例
Q_EXTERN_C JFRAME_KERNEL_EXPORT INotifier* _func_jnotifier_create()
{
    return new JNotifier();
}

// 销毁分发器实例
Q_EXTERN_C JFRAME_KERNEL_EXPORT void _func_jnotifier_destroy(INotifier *notifier)
{
    if (notifier) {
        delete notifier;
    }
}

// - class JNotifierPrivate -

class JNotifierPrivate
{
    friend class JNotifier;
public:
    JNotifierPrivate(JNotifier &notifier) :
        currObs(0),
        currOffset(0),
        imm(notifier)
    {

    }

private:
    JObserver* currObs;
    int currOffset;
    map_observer_info mapObs;
    JNotifierImm imm;
};

// - class Jnotifier -

JNotifier::JNotifier(QObject *parent) :
    QObject(parent)
{
    d = new JNotifierPrivate(*this);

    //
    qRegisterMetaType<JNotifyMsg>();

    //
    connect(this, SIGNAL(readyDispense(JNotifyMsg)), SLOT(dispense(JNotifyMsg)),
            Qt::QueuedConnection);
}

INotifier &JNotifier::end()
{
    d->currObs = 0;
    d->currOffset = 0;
    return *this;
}

void JNotifier::pop(JObserver *obs)
{
    if (!obs) {
        Q_ASSERT(obs);
        return *this;
    }

    d->mapObs.erase(obs);
    obs->jpopped("");   // notify observer
}

INotifier &JNotifier::pop(JObserver *obs, const std::string &id)
{
    map_observer_info::iterator iterMap = d->mapObs.find(obs);
    if (iterMap == d->mapObs.end()) {
        return *this;
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
        d->mapObs.erase(obs);
    }

    obs->jpopped(id);   // notify observer
}

INotifier &JNotifier::pop(const std::string &id)
{
    return pop(d->currObs, id);
}

void JNotifier::clear()
{
    map_observer_info::iterator iterMap = d->mapObs.begin();
    for (; iterMap != d->mapObs.end(); ++iterMap) {
        list_pair_id_cb &pids = iterMap->second.pids;
        list_pair_id_cb::iterator iterPids = pids.begin();
        for (; iterPids != pids.end(); ++iterMap) {
            iterMap->first->jpopped(iterPids->first);
            iterPids = pids.erase(iterPids);
        }
        iterMap = d->mapObs.erase(iterMap);
    }
}

JLRESULT JNotifier::send(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    return dispense(0, id, wParam, lParam);
}

void JNotifier::post(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    QtConcurrent::run(JNotifier::task, this, JNotifyMsg(0, id, wParam, lParam, ""));
}

void JNotifier::post(const std::string &id, const std::string &info, JLPARAM lParam)
{
    QtConcurrent::run(JNotifier::task, this, JNotifyMsg(0, id, 0, lParam, info));
}

INotifierImm &JNotifier::imm()
{
    return d->imm;
}

INotifier &JNotifier::begin(JObserver *obs, int offset)
{
    Q_ASSERT(obs);
    d->currObs = obs;
    d->currOffset = offset;
    return *this;
}

INotifier &JNotifier::push(JObserver *obs, const std::string &id, jobserver_cb cb, int offset)
{
    if (!obs) {
        Q_ASSERT(obs);
        return *this;
    }

    map_observer_info::iterator iterMap = d->mapObs.find(obs);
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
            return *this;
        }

        pids.push_back(pair_id_cb(id, cb));
    } else {
        list_pair_id_cb pids;
        pids.push_back(pair_id_cb(id, cb));
        d->mapObs[obs] = observer_info(offset, pids);
    }

    obs->jpushed(id);   // notify observer

    return *this;
}

INotifier &JNotifier::push(const std::string &id, jobserver_cb cb, int offset)
{
    return push(d->currObs, id, cb, offset);
}

void JNotifier::dispense(const JNotifyMsg &msg)
{
    if (msg.info.empty()) {
        dispense(msg.observer, msg.id, msg.wParam, msg.lParam);
    } else {
        dispense(msg.observer, msg.id, (JWPARAM)&msg.info, msg.lParam);
    }
}

JLRESULT JNotifier::dispense(JObserver *obs, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    JLRESULT result = 0;

    if (obs) {
        map_observer_info::const_iterator citerMap = d->mapObs.find(obs);
        if (citerMap != d->mapObs.end()) {
            return dispense(obs, id, wParam, lParam);
        }
    }

    map_observer_info::const_iterator citerMap = d->mapObs.begin();
    for (; citerMap != d->mapObs.end(); ++citerMap) {
        result = dispense(citerMap->first, id, wParam, lParam, citerMap->second);
    }

    return result;
}

JLRESULT JNotifier::dispense(JObserver *obs, const std::string &id, JWPARAM wParam, JLPARAM lParam, const observer_info &info)
{
    JLRESULT result = 0;
    const int offset = info.offset;
    const list_pair_id_cb &pids = info.pids;
    list_pair_id_cb::const_iterator citerPids = pids.begin();
    for (; citerPids != pids.end(); ++citerPids) {
        const pair_id_cb &pid = *citerPids;
        if (pid.first == id) {
            if (pid.second) {
                result = ((obs + offset)->*(pid.second))(id, wParam, lParam);
            } else {
                result = obs->jnotify(id, wParam, lParam);
            }
            break;
        }
    }

    return result;
}

void JNotifier::task(JNotifier *receiver, JNotifyMsg msg)
{
    if (!receiver) {
        Q_EMIT receiver->readyDispense(msg);
    }
}

JLRESULT JNotifier::immSend(const std::string &obsid, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    map_observer_info::const_iterator citerMap = d->mapObs.begin();
    for (; citerMap != d->mapObs.end(); ++citerMap) {
        if (citerMap->first->joberverId() == obsid) {
            return immSend(citerMap->first, id, wParam, lParam);
        }
    }
}

JLRESULT JNotifier::immSend(JObserver *obs, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    return dispense(obs, id, wParam, lParam);
}

void JNotifier::immPost(const std::string &obsid, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    map_observer_info::const_iterator citerMap = d->mapObs.begin();
    for (; citerMap != d->mapObs.end(); ++citerMap) {
        if (citerMap->first->joberverId() == obsid) {
            immPost(citerMap->first, id, wParam, lParam);
            break;
        }
    }
}

void JNotifier::immPost(JObserver *obs, const std::string &id, JWPARAM wParams, JLPARAM lParam)
{
    QtConcurrent::run(JNotifier::task, this, JNotifyMsg(obs, id, wParam, lParam, ""));
}

void JNotifier::immPost(const std::string &obsid, const std::string &id, const std::string &info, JLPARAM lParam)
{
    map_observer_info::const_iterator citerMap = d->mapObs.begin();
    for (; citerMap != d->mapObs.end(); ++citerMap) {
        if (citerMap->first->joberverId() == obsid) {
            immPost(citerMap->first, id, info, lParam);
            break;
        }
    }
}

void JNotifier::immPost(JObserver *obs, const std::string &id, const std::string &info, JLPARAM lParam)
{
    QtConcurrent::run(JNotifier::task, this, JNotifyMsg(obs, id, 0, lParam, info));
}

// - class JNotifierImm -

JNotifierImm::JNotifierImm(JNotifier &notifier) :
    q_notifier(notifier)
{

}
