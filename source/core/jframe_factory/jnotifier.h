#ifndef JNOTIFIER_H
#define JNOTIFIER_H

#include "jframe_facade.h"

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

// 接口描述
#define VER_JObserver J_INTERFACE_VERSION(1, 0)
#define IID_JObserver J_IID_INTERFACE(JObserver)

/**
 * @brief The JObserver class : 观察者接口定义
 */
class JObserver
{
public:
    /**
     * @brief ~JObserver : 析构函数
     */
    virtual ~JObserver() {}

    /**
     * @brief notifySink : 消息响应
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return
     */
    virtual JLRESULT notifySink(const std::string &id, JWPARAM wParam, JLPARAM lParam)
    { (void)id; (void)wParam; (void)lParam; return 0; }

    /**
     * @brief notifyAppended : 消息已订阅通知
     * @param id : 消息标识
     */
    virtual void notifyAppended(const std::string &id) { (void)id; }

    /**
     * @brief notifyRemoved : 消息已取消订阅通知
     * @param id : 消息标识
     */
    virtual void notifyRemoved(const std::string &id) { (void)id; }

    /**
     * @brief observerId : 获取观察者标识
     * @return : 观察者标识
     */
    virtual std::string observerId() const { return std::string(); }
};

//
typedef JLRESULT (JObserver::*jobserver_cb)(const std::string &, JWPARAM, JLPARAM);

// 接口描述
#define VER_INotifier J_INTERFACE_VERSION(1, 0)
#define IID_INotifier J_IID_INTERFACE(INotifier)

class INotifierImm;

/**
 * @brief The INotifier class : 消息分发器接口定义
 */
class INotifier : public IJUnknown
{
public:
    /**
     * @brief ~INotifier : 析构函数
     */
    virtual ~INotifier() {}

    /**
     * @brief interfaceIdentity : 获取接口标识
     * @return : 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_INotifier; }

    /**
     * @brief interfaceVersion : 获取接口版本
     * @return : 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_INotifier; }

    /**
     * @brief beginGroup : 开始订阅消息组
     * @param obs : 观察者
     * @return : 消息分发器
     */
    template<typename T> INotifier& beginGroup(T *obs);

    /**
     * @brief endGroup : 结束订阅消息组
     * @return : 消息分发器
     */
    virtual INotifier& endGroup() = 0;

    /**
     * @brief append : 订阅消息
     * @param obs : 观察者
     * @param id : 消息标识
     * @param cb : 消息响应函数接口
     * @return : 消息分发器
     */
    template<typename T> INotifier& append(T *obs, const std::string &id,
                                           JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM));

    /**
     * @brief append : 订阅消息
     * @param id : 消息标识
     * @param cb : 消息响应函数接口
     * @return : 消息分发器
     */
    template<typename T> INotifier& append(const std::string &id,
                                           JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM));

    /**
     * @brief remove : 取消订阅消息
     * @param obs : 观察者
     */
    virtual void remove(JObserver *obs) = 0;

    /**
     * @brief remove : 取消订阅消息
     * @param obs : 观察者
     * @param id : 消息标识
     * @return : 消息分发器
     */
    virtual INotifier& remove(JObserver *obs, const std::string &id) = 0;

    /**
     * @brief remove : 取消订阅消息
     * @param id : 消息标识
     * @return : 消息分发器
     */
    virtual INotifier& remove(const std::string &id) = 0;

    /**
     * @brief clear : 清空订阅消息
     */
    virtual void clear() = 0;

    /**
     * @brief sendMessage : 发送消息（同步，广播）
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return : 执行结果
     */
    virtual JLRESULT sendMessage(const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief postMessage : 发送消息（异步，广播）
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return : 执行结果
     */
    virtual void postMessage(const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief postMessage : 发送消息（异步，广播）
     * @param id : 消息标识
     * @param info : 参数1
     * @param lParam : 参数2
     * @return : 执行结果
     */
    virtual void postMessage(const std::string &id, const std::string &info, JLPARAM lParam = 0) = 0;

    /**
     * @brief imm : 获取端到端消息分发接口
     * @return : 端到端消息分发接口
     */
    virtual INotifierImm& imm() = 0;

protected:
    /**
     * @brief beginGroup : 开始订阅消息组
     * @param obs : 观察者
     * @param offset : 接口地址差值
     * @return : 消息分发器
     */
    virtual INotifier& beginGroup(JObserver *obs, int offset = 0) = 0;

    /**
     * @brief append : 订阅消息
     * @param obs : 观察者
     * @param id : 消息标识
     * @param cb : 消息响应函数接口
     * @param offset : 接口地址差值
     * @return : 消息分发器
     */
    virtual INotifier& append(JObserver* obs, const std::string &id, jobserver_cb cb, int offset) = 0;

    /**
     * @brief append : 订阅消息
     * @param id : 消息标识
     * @param cb : 消息响应函数接口
     * @return : 消息分发器
     */
    virtual INotifier& append(const std::string &id, jobserver_cb cb) = 0;
};

// 接口描述
#define VER_INotifierImm J_INTERFACE_VERSION(1, 0)
#define IID_INotifierImm J_IID_INTERFACE(INotifierImm)

/**
 * @brief The INotifierImm class : 端到端（直接发送给指定观察者）分发消息接口定义
 */
class INotifierImm
{
public:
    /**
     * @brief ~INotifierImm : 析构函数
     */
    virtual ~INotifierImm() {}

    /**
     * @brief sendMessage : 发送消息（同步，端到端）
     * @param obsid : 观察者标识
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return : 执行结果
     */
    virtual JLRESULT sendMessage(const std::string &obsid, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief sendMessage : 发送消息（同步，端到端）
     * @param obs : 观察者
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return : 执行结果
     */
    virtual JLRESULT sendMessage(JObserver *obs, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief postMessage : 发送消息（异步，端到端）
     * @param obsid : 观察者标识
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     */
    virtual void postMessage(const std::string &obsid, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief postMessage : 发送消息（异步，端到端）
     * @param obs : 观察者
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     */
    virtual void postMessage(JObserver *obs, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief postMessage : 发送消息（异步，端到端）
     * @param obsid : 观察者标识
     * @param id : 消息标识
     * @param info : 参数1
     * @param lParam : 参数2
     */
    virtual void postMessage(const std::string &obsid, const std::string &id, const std::string &info, JLPARAM lParam = 0) = 0;

    /**
     * @brief postMessage : 发送消息（异步，端到端）
     * @param obs : 观察者
     * @param id : 消息标识
     * @param info : 参数1
     * @param lParam : 参数2
     */
    virtual void postMessage(JObserver *obs, const std::string &id, const std::string &info, JLPARAM lParam = 0) = 0;
};

//
#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable : 4407)    // 在指向成员表示形式的不同指针之间进行转换，编译器可能生成不正确的代码
#endif

template<typename T>
inline INotifier& INotifier::beginGroup(T *obs)
{
    return beginGroup(obs,
                  #ifdef _MSC_VER
                      (reinterpret_cast<JObserver *>(obs) - static_cast<JObserver *>(obs))
                  #elif defined(__unix__)
                      0
                  #else
                  #pragma message("Not supported!"))
                      0
                  #endif
                      );
}

template<typename T>
inline INotifier& INotifier::append(T *obs, const std::string &id,
                                    JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM))
{
    return append(obs, id, static_cast<jobserver_cb>(cb),
              #ifdef _MSC_VER
                  (reinterpret_cast<JObserver *>(obs) - static_cast<JObserver *>(obs))
              #elif defined(__unix__)
                  0
              #else
              #pragma message("Not supported!"))
                  0
              #endif
                  );
}

template<typename T>
inline INotifier& INotifier::append(const std::string &id,
                                    JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM))
{
    return append(id, static_cast<jobserver_cb>(cb));
}

#if defined(_MSC_VER)
#pragma warning (pop)
#endif

#endif // JNOTIFIER_H
