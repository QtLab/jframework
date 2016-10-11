#ifndef JNOTIFIER_H
#define JNOTIFIER_H

#include "jframe_facade.h"
#include <vector>

// 消息分发器

//
#ifdef _MSC_VER
typedef unsigned __int64 JWPARAM;
typedef __int64 JLPARAM;
typedef __int64 JLRESULT;
typedef bool jbool;
typedef char jchar;
typedef unsigned char juchar;
typedef short jshort;
typedef unsigned short jushort;
typedef int jint;
typedef unsigned juint;
typedef __int64 jlonglong;
typedef unsigned __int64 julonglong;
#else
typedef unsigned long long JWPARAM;
typedef long long JLPARAM;
typedef long long JLRESULT;
typedef bool jbool;
typedef char jchar;
typedef unsigned char juchar;
typedef short jshort;
typedef unsigned short jushort;
typedef int jint;
typedef unsigned juint;
typedef long long jlonglong;
typedef unsigned long long julonglong;
#endif

/** JObserver 接口描述 */
#define VER_JObserver J_INTERFACE_VERSION(1, 0)
#define IID_JObserver J_IID_INTERFACE(JObserver)

/**
 * @brief 观察者接口定义
 */
class JObserver
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~JObserver() {}

    /**
     * @brief 消息响应
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return 执行结果
     */
    virtual JLRESULT notifySink(const std::string &id, JWPARAM wParam, JLPARAM lParam)
    { (void)id; (void)wParam; (void)lParam; return 0; }

    /**
     * @brief 消息已订阅通知
     * @param id : 消息标识
     */
    virtual void notifyAppended(const std::string &id) { (void)id; }

    /**
     * @brief 消息已取消订阅通知
     * @param id : 消息标识
     */
    virtual void notifyRemoved(const std::string &id) { (void)id; }

    /**
     * @brief 获取观察者标识
     * @return 观察者标识
     */
    virtual std::string observerId() const { return std::string(); }
};

/** 观察者回调函数类型定义 */
typedef JLRESULT (JObserver::*jobserver_cb)(const std::string &id, JWPARAM wParam, JLPARAM lParam);

/** INotifier 接口描述 */
#define VER_INotifier J_INTERFACE_VERSION(1, 0)
#define IID_INotifier J_IID_INTERFACE(INotifier)

class IImmNotify;
class IDBusNotify;
class IIceNotify;

/**
 * @brief 消息分发器接口定义
 */
class INotifier : public IJUnknown
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~INotifier() {}

    /**
     * @brief 获取接口标识
     * @return 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_INotifier; }

    /**
     * @brief 获取接口版本
     * @return 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_INotifier; }

    /**
     * @brief 开始订阅消息组
     * @param [in] obs : 观察者
     * @return 消息分发器
     */
    template<typename T> INotifier& beginGroup(T *obs);

    /**
     * @brief 结束订阅消息组
     * @return 消息分发器
     */
    virtual INotifier& endGroup() = 0;

    /**
     * @brief 订阅消息
     * @param obs : 观察者
     * @param id : 消息标识
     * @param [in] cb : 消息响应函数接口
     * @return 消息分发器
     */
    template<typename T> INotifier& append(T *obs, const std::string &id,
                                           JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM));

    /**
     * @brief 订阅消息
     * @param id : 消息标识
     * @param [in] cb : 消息响应函数接口
     * @return 消息分发器
     */
    template<typename T> INotifier& append(const std::string &id,
                                           JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM));

    /**
     * @brief 取消订阅消息
     * @param [in] obs : 观察者
     */
    virtual void remove(JObserver *obs) = 0;

    /**
     * @brief 取消订阅消息
     * @param [in] obs : 观察者
     * @param id : 消息标识
     * @return 消息分发器
     */
    virtual INotifier& remove(JObserver *obs, const std::string &id) = 0;

    /**
     * @brief 取消订阅消息
     * @param id : 消息标识
     * @return 消息分发器
     */
    virtual INotifier& remove(const std::string &id) = 0;

    /**
     * @brief 清空订阅消息
     */
    virtual void clear() = 0;

    /**
     * @brief 发送消息（同步）
     * @param domain : 目标域名
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return 执行结果
     */
    virtual JLRESULT sendMessage(const std::string &domain, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief 发送消息（同步）
     * @param observer : 观察者
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return 执行结果
     */
    virtual JLRESULT sendMessage(JObserver *observer, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief 发送消息（异步，广播）
     * @param domain : 目标域名
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return 执行结果
     */
    virtual void postMessage(const std::string &domain, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief 发送消息（异步）
     * @param observer : 观察者
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return 执行结果
     */
    virtual void postMessage(JObserver *observer, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief 发送消息（异步）
     * @param domain : 目标域名
     * @param info : 参数1
     * @param lParam : 参数2
     * @return 执行结果
     */
    virtual void postMessage(const std::string &domain, const std::string &info, JLPARAM lParam = 0) = 0;

    /**
     * @brief 发送消息（异步）
     * @param observer : 观察者
     * @param id : 消息标识
     * @param info : 参数1
     * @param lParam : 参数2
     * @return 执行结果
     */
    virtual void postMessage(JObserver *observer, const std::string &id, const std::string &info, JLPARAM lParam = 0) = 0;

    /**
     * @brief 获取 D-BUS 进程间通信接口
     * @return D-BUS 进程间通信接口
     */
    virtual IDBusNotify& dbus() = 0;

    /**
     * @brief 获取 ice 分布式网络通信接口
     * @return ice 分布式网络通信接口
     */
    virtual IIceNotify& ice() = 0;

protected:
    /**
     * @brief 开始订阅消息组
     * @param [in] obs : 观察者
     * @param offset : 接口地址差值
     * @return 消息分发器
     */
    virtual INotifier& beginGroup(JObserver *obs, int offset = 0) = 0;

    /**
     * @brief 订阅消息
     * @param [in] obs : 观察者
     * @param id : 消息标识
     * @param [in] cb : 消息响应函数接口
     * @param offset : 接口地址差值
     * @return 消息分发器
     */
    virtual INotifier& append(JObserver* obs, const std::string &id, jobserver_cb cb, int offset) = 0;

    /**
     * @brief 订阅消息
     * @param id : 消息标识
     * @param [in] cb : 消息响应函数接口
     * @return 消息分发器
     */
    virtual INotifier& append(const std::string &id, jobserver_cb cb) = 0;
};

/** IDBusNotify 接口描述 */
#define VER_IDBusNotify J_INTERFACE_VERSION(1, 0)
#define IID_IDBusNotify J_IID_INTERFACE(IDBusNotify)

class QObject;
class QString;
class QVariant;

/**
 * @brief 基于 D-BUS 的通信接口定义
 * @details domain : 目的端域名。格式：[id]@[observerid]@[service]
 *  格式说明：id表示消息表示字符串；observerid表示观察者标识；service表示服务端（接收端）标识。
 *  所有标识缺省时用*表示，例如：setXXXValue@*@com.smartsoft.jframe1，表示将
 *  消息“setXXXValue”广播给软件实体“com.smartsoft.jframe1”的所有观察者。
 */
class IDBusNotify
{
public:
    enum ReturnCode {
        DomainFormatInvalid = -4,
        SessionBusNotConnected = -3,
        ConnectionInterfaceInvalid = -2,
        CallError = -1,
        CallSuccess = 0,
        InterruptBroadCast = 1,
    };

    /**
     * @brief 析构函数
     */
    virtual ~IDBusNotify() {}

    /**
     * @brief isConnected
     * @return
     */
    virtual bool isConnected() = 0;

    /**
     * @brief 初始化 D-BUS 模块
     * @param service : 服务名称
     * @param path : 路径
     * @param iface : 接口名称
     * @return true，初始化成功；false，初始化失败
     */
    virtual bool initialize(const std::string &service, const std::string &path,
                            const std::string &iface) = 0;

    /**
     * @brief 发送数据（同步）
     * @param domain : 目的端域名
     * @param buffer : 缓冲数据
     * @param size : 数据字节数
     * @return 操作结果
     */
    virtual int sendBuffer(const std::string &domain, const char *buffer, int size) = 0;

    /**
     * @brief 发送数据（同步）
     * @param domain : 目的端域名
     * @param msg : 消息
     * @return 操作结果
     */
    virtual int sendString(const std::string &domain, const std::string &msg) = 0;

    /**
     * @brief 发送数据（同步）
     * @param domain : 目的端域名
     * @param data : 数据
     * @return 操作结果
     */
    virtual int sendList(const std::string &domain, const std::list<std::string> &data) = 0;

    /**
     * @brief 发送数据（同步）
     * @param domain : 目的端域名
     * @param variant : 数据
     * @return 操作结果
     */
    virtual int sendVariant(const QString &domain, const QVariant &variant) = 0;

    /**
     * @brief 发送数据（异步）
     * @param domain : 目的端域名
     * @param buffer : 数据缓冲
     * @param size : 数据字节数
     */
    virtual void postBuffer(const std::string &domain, const char *buffer, int size) = 0;

    /**
     * @brief 发送数据（异步）
     * @param domain : 目的端域名
     * @param msg : 消息
     */
    virtual void postString(const std::string &domain, const std::string &msg) = 0;

    /**
     * @brief 发送数据（异步）
     * @param domain : 目的端域名
     * @param data : 数据
     */
    virtual void postList(const std::string &domain, const std::list<std::string> &data) = 0;

    /**
     * @brief 发送数据（异步）
     * @param domain : 目的端域名
     * @param variant : 数据
     */
    virtual void postVariant(const QString &domain, const QVariant &variant) = 0;
};

/** IByteStream 接口描述 */
#define VER_IByteStream J_INTERFACE_VERSION(1, 0)
#define IID_IByteStream J_IID_INTERFACE(IByteStream)

/**
 * @brief 字节流接口定义
 */
class IByteStream
{
public:
    enum ElementType {
        BasicType,
        VariantType,
        ArrayType,
        StructureType,
        MapType,
        MapEntryType,
        UnknownType = -1
    };

    /**
     * @brief 析构函数
     */
    virtual ~IByteStream() {}

    virtual IByteStream &operator=(IByteStream &other) = 0;

    virtual void clear() = 0;

    virtual IByteStream &operator <<(unsigned char arg) = 0;
    virtual IByteStream &operator <<(bool arg) = 0;
    virtual IByteStream &operator <<(short arg) = 0;
    virtual IByteStream &operator <<(unsigned short arg) = 0;
    virtual IByteStream &operator <<(int arg) = 0;
    virtual IByteStream &operator <<(unsigned int arg) = 0;
    virtual IByteStream &operator <<(jlonglong arg) = 0;
    virtual IByteStream &operator <<(julonglong arg) = 0;
    virtual IByteStream &operator <<(double arg) = 0;
    virtual IByteStream &operator <<(const std::string &arg) = 0;
    virtual IByteStream &operator <<(const std::vector<std::string> &arg) = 0;
    virtual IByteStream &operator <<(const std::list<std::string> &arg) = 0;
    virtual void writeData(const char* buffer, size_t size) = 0;

    virtual void beginStructure() = 0;
    virtual void endStructure() = 0;
    virtual void beginArray(int elementMetaTypeId) = 0;
    virtual void endArray() = 0;
    virtual void beginMap(int keyMetaTypeId, int valueMetaTypeId) = 0;
    virtual void endMap() = 0;
    virtual void beginMapEntry() = 0;
    virtual void endMapEntry() = 0;

    // used for de-marshalling (D-BUS -> Qt)
    virtual std::string currentSignature() const = 0;
    virtual ElementType currentType() const = 0;

    virtual const IByteStream &operator >>(unsigned char &arg) const = 0;
    virtual const IByteStream &operator >>(bool &arg) const = 0;
    virtual const IByteStream &operator >>(short &arg) const = 0;
    virtual const IByteStream &operator >>(unsigned short &arg) const = 0;
    virtual const IByteStream &operator >>(int &arg) const = 0;
    virtual const IByteStream &operator >>(unsigned int &arg) const = 0;
    virtual const IByteStream &operator >>(jlonglong &arg) const = 0;
    virtual const IByteStream &operator >>(julonglong &arg) const = 0;
    virtual const IByteStream &operator >>(double &arg) const = 0;
    virtual const IByteStream &operator >>(std::string &arg) const = 0;
    virtual const IByteStream &operator >>(std::vector<std::string> &arg) = 0;
    virtual const IByteStream &operator >>(std::list<std::string> &arg) = 0;
    virtual void readData(char* buffer, size_t size) = 0;

    virtual void beginStructure() const = 0;
    virtual void endStructure() const = 0;
    virtual void beginArray() const = 0;
    virtual void endArray() const = 0;
    virtual void beginMap() const = 0;
    virtual void endMap() const = 0;
    virtual void beginMapEntry() const = 0;
    virtual void endMapEntry() const = 0;
    virtual bool atEnd() const = 0;
};

/** IDBusArgument 接口描述 */
#define VER_ISerializable J_INTERFACE_VERSION(1, 0)
#define IID_ISerializable J_IID_INTERFACE(ISerializable)

/**
 * @brief 序列化接口定义
 */
template <typename T>
class ISerializable
{
public:

    /**
     * @brief 析构函数
     */
    virtual ~ISerializable() {}

    /**
     * @brief read
     * @param si
     */
    virtual void read(const IByteStream &si) const = 0;

    /**
     * @brief write
     * @param so
     */
    virtual void write(IByteStream &so) = 0;

    /**
     * @brief read
     * @param si
     * @param serial
     */
    inline static void read(const IByteStream &si, ISerializable<T> &serial);

    /**
     * @brief write
     * @param so
     * @param serial
     */
    inline static void write(IByteStream &so, const ISerializable<T> &serial);

    /**
     * @brief clone
     * @return
     */
    virtual T clone() = 0;
};

template<typename T> inline void read(const IByteStream &si, ISerializable<T> &serial)
{ serial.read(si); }

template<typename T> inline void write(IByteStream &si, const ISerializable<T> &serial)
{ serial.write(si); }

/** IIceNotify 接口描述 */
#define VER_IIceNotify J_INTERFACE_VERSION(1, 0)
#define IID_IIceNotify J_IID_INTERFACE(IIceNotify)

/**
 * @brief 基于 ICE 的分布式网络通信接口
 */
class IIceNotify
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~IIceNotify() {}

    /**
     * @brief isConnected
     * @return
     */
    virtual bool isConnected() = 0;
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
