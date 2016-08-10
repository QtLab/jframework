#ifndef JNOTIFIER_H
#define JNOTIFIRE_H

#include "factory/jframe_interface.h"
#include <string>

// 消息分发器

//
#ifdef _MSC_VER
typedef unsigned __int64 JWPARAM;
typedef __int64 JLPARAM;
typedef __int64 JLRESULT;
#else
typedef unsigned long long JWPARAM;
typedef long long JLPARAM;
typedef long long JLRESULT;
#endif

// 观察者接口定义
class JObserver
{
public:
    virtual ~JObserver() {}

    // 消息响应接口
    virtual JLRESULT jnotify(const std::string &, JWPARAM, JLPARAM) {}

    // （取消）订阅消息通知
    virtual void jpushed(const std::string &) {}
    virtual void jpopped(const std::string &) {}

    // 获取观察者标识
    virtual std::string jobserverId() const { return std::string(); }
};

// 接口标识

//
typedef JLRESULT (JObserver::*jobserver_cb)(const std::string &, JWPARAM, JLPARAM);

// 消息分发器接口定义

class INotifierImm;

class INotifier
{
public:
    virtual ~INotifier() {}

    // 开始、结束添加消息订阅
    template<typename T> INotifier& begin(T *obs) = 0;
    virtual INotifier& end() = 0;

    // 添加消息订阅
    template<typename T> INotifier& push(T *obs, const std::string &id,
                                         JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM));
    template<typename T> INotifier& push(const std::string &id,
                                         JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM));

    // 取消消息订阅
    virtual void pop(JObserver *obs) = 0;
    virtual INotifier& pop(JObserver *obs, const std::string &id) = 0;
    virtual INotifier& pop(const std::string &id) = 0;

    // 清空消息订阅
    virtual void clear() = 0;

    // 发送消息（同步，广播方式）
    virtual JLRESULT send(const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    // 发送消息（异步，广播方式）
    virtual void post(const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;
    virtual void post(const std::string &id, const std::string &info, JLPARAM lParam = 0) = 0;

    // 获取端到端发送消息操作接口
    virtual INotifierImm& imm() = 0;

protected:
    virtual INotifier& begin(JObserver *obs, int offset) = 0;
    virtual INotifier& push(JObserver* obs, const std::string &id, jobserver_cb cb, int offset) = 0;
    virtual INotifier& push(const std::string &id, jobserver_cb cb, int offset) = 0;
};

// 接口标识

// 端到端（直接发送给指定观察者）发送消息接口定义
class INotifierImm
{
public:
    virtual ~INotifierImm() {}

    // 发送消息（同步，发送给指定观察者）
    virtual JLRESULT send(const std::string &obsid, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;
    virtual JLRESULT send(JObserver *obs, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    // 发送消息（异步，发送给指定观察者）
    virtual void post(const std::string &obsid, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;
    virtual void post(JObserver *obs, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;
    virtual void post(const std::string &obsid, const std::string &id, const std::string &info, JLPARAM lParam = 0) = 0;
    virtual void post(JObserver *obs, const std::string &id, const std::string &info, JLPARAM lParam = 0) = 0;
};

// 接口标识

//
#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable : 4407)    // 在指向成员表示形式的不同指针之间进行转换，编译器可能生成不正确的代码
#endif

template<typename T>
inline INotifier& INotifier::begin(T *obs)
{
    return begin(obs,
                 (reinterpret_cast<JObserver *>(obs) - static_cast<JObserver *>(obs)));
}

template<typename T>
inline INotifier& INotifier::push(T *obs, const std::string &id,
                                  JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM))
{
    return push(obs, id, static_cast<jobserver_cb>(cb),
                (reinterpret_cast<JObserver *>(obs) - static_cast<JObserver *>(obs)));
}

template<typename T>
inline INotifier& INotifier::push(const std::string &id,
                                  JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM))
{
    return push(id, static_cast<jobserver_cb>(cb)
                (reinterpret_cast<JObserver *>(obs) - static_cast<JObserver *>(obs)));
}

#if defined(_MSC_VER)
#pragma warning (pop)
#endif

#endif // JNOTIFIER_H
