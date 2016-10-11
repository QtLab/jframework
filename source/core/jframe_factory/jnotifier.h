#ifndef JNOTIFIER_H
#define JNOTIFIER_H

#include "jframe_facade.h"
#include <vector>

// ��Ϣ�ַ���

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

/** JObserver �ӿ����� */
#define VER_JObserver J_INTERFACE_VERSION(1, 0)
#define IID_JObserver J_IID_INTERFACE(JObserver)

/**
 * @brief �۲��߽ӿڶ���
 */
class JObserver
{
public:
    /**
     * @brief ��������
     */
    virtual ~JObserver() {}

    /**
     * @brief ��Ϣ��Ӧ
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual JLRESULT notifySink(const std::string &id, JWPARAM wParam, JLPARAM lParam)
    { (void)id; (void)wParam; (void)lParam; return 0; }

    /**
     * @brief ��Ϣ�Ѷ���֪ͨ
     * @param id : ��Ϣ��ʶ
     */
    virtual void notifyAppended(const std::string &id) { (void)id; }

    /**
     * @brief ��Ϣ��ȡ������֪ͨ
     * @param id : ��Ϣ��ʶ
     */
    virtual void notifyRemoved(const std::string &id) { (void)id; }

    /**
     * @brief ��ȡ�۲��߱�ʶ
     * @return �۲��߱�ʶ
     */
    virtual std::string observerId() const { return std::string(); }
};

/** �۲��߻ص��������Ͷ��� */
typedef JLRESULT (JObserver::*jobserver_cb)(const std::string &id, JWPARAM wParam, JLPARAM lParam);

/** INotifier �ӿ����� */
#define VER_INotifier J_INTERFACE_VERSION(1, 0)
#define IID_INotifier J_IID_INTERFACE(INotifier)

class IImmNotify;
class IDBusNotify;
class IIceNotify;

/**
 * @brief ��Ϣ�ַ����ӿڶ���
 */
class INotifier : public IJUnknown
{
public:
    /**
     * @brief ��������
     */
    virtual ~INotifier() {}

    /**
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_INotifier; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_INotifier; }

    /**
     * @brief ��ʼ������Ϣ��
     * @param [in] obs : �۲���
     * @return ��Ϣ�ַ���
     */
    template<typename T> INotifier& beginGroup(T *obs);

    /**
     * @brief ����������Ϣ��
     * @return ��Ϣ�ַ���
     */
    virtual INotifier& endGroup() = 0;

    /**
     * @brief ������Ϣ
     * @param obs : �۲���
     * @param id : ��Ϣ��ʶ
     * @param [in] cb : ��Ϣ��Ӧ�����ӿ�
     * @return ��Ϣ�ַ���
     */
    template<typename T> INotifier& append(T *obs, const std::string &id,
                                           JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM));

    /**
     * @brief ������Ϣ
     * @param id : ��Ϣ��ʶ
     * @param [in] cb : ��Ϣ��Ӧ�����ӿ�
     * @return ��Ϣ�ַ���
     */
    template<typename T> INotifier& append(const std::string &id,
                                           JLRESULT(T::*cb)(const std::string &, JWPARAM, JLPARAM));

    /**
     * @brief ȡ��������Ϣ
     * @param [in] obs : �۲���
     */
    virtual void remove(JObserver *obs) = 0;

    /**
     * @brief ȡ��������Ϣ
     * @param [in] obs : �۲���
     * @param id : ��Ϣ��ʶ
     * @return ��Ϣ�ַ���
     */
    virtual INotifier& remove(JObserver *obs, const std::string &id) = 0;

    /**
     * @brief ȡ��������Ϣ
     * @param id : ��Ϣ��ʶ
     * @return ��Ϣ�ַ���
     */
    virtual INotifier& remove(const std::string &id) = 0;

    /**
     * @brief ��ն�����Ϣ
     */
    virtual void clear() = 0;

    /**
     * @brief ������Ϣ��ͬ����
     * @param domain : Ŀ������
     * @param wParam : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual JLRESULT sendMessage(const std::string &domain, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ��ͬ����
     * @param observer : �۲���
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual JLRESULT sendMessage(JObserver *observer, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ���첽���㲥��
     * @param domain : Ŀ������
     * @param wParam : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual void postMessage(const std::string &domain, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ���첽��
     * @param observer : �۲���
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual void postMessage(JObserver *observer, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ���첽��
     * @param domain : Ŀ������
     * @param info : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual void postMessage(const std::string &domain, const std::string &info, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ���첽��
     * @param observer : �۲���
     * @param id : ��Ϣ��ʶ
     * @param info : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual void postMessage(JObserver *observer, const std::string &id, const std::string &info, JLPARAM lParam = 0) = 0;

    /**
     * @brief ��ȡ D-BUS ���̼�ͨ�Žӿ�
     * @return D-BUS ���̼�ͨ�Žӿ�
     */
    virtual IDBusNotify& dbus() = 0;

    /**
     * @brief ��ȡ ice �ֲ�ʽ����ͨ�Žӿ�
     * @return ice �ֲ�ʽ����ͨ�Žӿ�
     */
    virtual IIceNotify& ice() = 0;

protected:
    /**
     * @brief ��ʼ������Ϣ��
     * @param [in] obs : �۲���
     * @param offset : �ӿڵ�ַ��ֵ
     * @return ��Ϣ�ַ���
     */
    virtual INotifier& beginGroup(JObserver *obs, int offset = 0) = 0;

    /**
     * @brief ������Ϣ
     * @param [in] obs : �۲���
     * @param id : ��Ϣ��ʶ
     * @param [in] cb : ��Ϣ��Ӧ�����ӿ�
     * @param offset : �ӿڵ�ַ��ֵ
     * @return ��Ϣ�ַ���
     */
    virtual INotifier& append(JObserver* obs, const std::string &id, jobserver_cb cb, int offset) = 0;

    /**
     * @brief ������Ϣ
     * @param id : ��Ϣ��ʶ
     * @param [in] cb : ��Ϣ��Ӧ�����ӿ�
     * @return ��Ϣ�ַ���
     */
    virtual INotifier& append(const std::string &id, jobserver_cb cb) = 0;
};

/** IDBusNotify �ӿ����� */
#define VER_IDBusNotify J_INTERFACE_VERSION(1, 0)
#define IID_IDBusNotify J_IID_INTERFACE(IDBusNotify)

class QObject;
class QString;
class QVariant;

/**
 * @brief ���� D-BUS ��ͨ�Žӿڶ���
 * @details domain : Ŀ�Ķ���������ʽ��[id]@[observerid]@[service]
 *  ��ʽ˵����id��ʾ��Ϣ��ʾ�ַ�����observerid��ʾ�۲��߱�ʶ��service��ʾ����ˣ����նˣ���ʶ��
 *  ���б�ʶȱʡʱ��*��ʾ�����磺setValue@*@com.smartsoft.jframe1����ʾ��
 *  ��Ϣ��setValue���㲥�����ʵ�塰com.smartsoft.jframe1�������й۲��ߡ�
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
     * @brief ��������
     */
    virtual ~IDBusNotify() {}

    /**
     * @brief isConnected
     * @return
     */
    virtual bool isConnected() = 0;

    /**
     * @brief ��ʼ�� D-BUS ģ��
     * @param service : ��������
     * @param path : ·��
     * @param iface : �ӿ�����
     * @return true����ʼ���ɹ���false����ʼ��ʧ��
     */
    virtual bool initialize(const std::string &service, const std::string &path,
                            const std::string &iface) = 0;

    /**
     * @brief �������ݣ�ͬ����
     * @param domain : Ŀ�Ķ�����
     * @param buffer : ��������
     * @param size : �����ֽ���
     * @return �������
     */
    virtual int sendBuffer(const std::string &domain, const char *buffer, int size) = 0;

    /**
     * @brief �������ݣ�ͬ����
     * @param domain : Ŀ�Ķ�����
     * @param msg : ��Ϣ
     * @return �������
     */
    virtual int sendString(const std::string &domain, const std::string &msg) = 0;

    /**
     * @brief �������ݣ�ͬ����
     * @param domain : Ŀ�Ķ�����
     * @param data : ����
     * @return �������
     */
    virtual int sendList(const std::string &domain, const std::list<std::string> &data) = 0;

    /**
     * @brief �������ݣ�ͬ����
     * @param domain : Ŀ�Ķ�����
     * @param variant : ����
     * @return �������
     */
    virtual int sendVariant(const QString &domain, const QVariant &variant) = 0;

    /**
     * @brief �������ݣ��첽��
     * @param domain : Ŀ�Ķ�����
     * @param buffer : ���ݻ���
     * @param size : �����ֽ���
     */
    virtual void postBuffer(const std::string &domain, const char *buffer, int size) = 0;

    /**
     * @brief �������ݣ��첽��
     * @param domain : Ŀ�Ķ�����
     * @param msg : ��Ϣ
     */
    virtual void postString(const std::string &domain, const std::string &msg) = 0;

    /**
     * @brief �������ݣ��첽��
     * @param domain : Ŀ�Ķ�����
     * @param data : ����
     */
    virtual void postList(const std::string &domain, const std::list<std::string> &data) = 0;

    /**
     * @brief �������ݣ��첽��
     * @param domain : Ŀ�Ķ�����
     * @param variant : ����
     */
    virtual void postVariant(const QString &domain, const QVariant &variant) = 0;
};

/** IIceNotify �ӿ����� */
#define VER_IIceNotify J_INTERFACE_VERSION(1, 0)
#define IID_IIceNotify J_IID_INTERFACE(IIceNotify)

/**
 * @brief ���� ICE �ķֲ�ʽ����ͨ�Žӿ�
 * @details domain : Ŀ�Ķ���������ʽ��[id]@[observerid]@[dbus-service]@[ice-service]
 *  ��ʽ˵����id��ʾ��Ϣ��ʾ�ַ�����observerid��ʾ�۲��߱�ʶ��dbus-service��ʾD-BUS����ˣ����նˣ���ʶ��
 *  ice-service��ʾIce����ˣ����նˣ���ʶ��
 *  ���б�ʶȱʡʱ��*��ʾ�����磺setValue@*@com.smartsoft.jframe1@192.168.0.1:2000����ʾ��
 *  ��Ϣ��setValue���㲥��Ice����ˡ�192.168.0.1:2000�������ʵ�塰com.smartsoft.jframe1�������й۲��ߡ�
 *  ��������ice-serviceʱ��Ĭ��ʹ��setCurrentService���õ�ֵ��
 */
class IIceNotify
{
public:
    /**
     * @brief ��������
     */
    virtual ~IIceNotify() {}

    /**
     * @brief isConnected
     * @return
     */
    virtual bool isConnected() = 0;

    /**
     * @brief ��ʼ�� Ice ģ��
     * @param service : ��������
     * @param path : ·��
     * @param iface : �ӿ�����
     * @return true����ʼ���ɹ���false����ʼ��ʧ��
     */
    virtual bool initialize(const std::string &service) = 0;

    /**
     * @brief setCurrentService
     * @param service
     * @return true�����óɹ���false������ʧ��
     */
    virtual bool setCurrentService(const std::string &service) = 0;

    /**
     * @brief �������ݣ�ͬ����
     * @param domain : Ŀ�Ķ�����
     * @param buffer : ��������
     * @param size : �����ֽ���
     * @return �������
     */
    virtual int sendBuffer(const std::string &domain, const char *buffer, int size) = 0;

    /**
     * @brief �������ݣ�ͬ����
     * @param domain : Ŀ�Ķ�����
     * @param msg : ��Ϣ
     * @return �������
     */
    virtual int sendString(const std::string &domain, const std::string &msg) = 0;

    /**
     * @brief �������ݣ�ͬ����
     * @param domain : Ŀ�Ķ�����
     * @param data : ����
     * @return �������
     */
    virtual int sendList(const std::string &domain, const std::list<std::string> &data) = 0;

    /**
     * @brief �������ݣ�ͬ����
     * @param domain : Ŀ�Ķ�����
     * @param variant : ����
     * @return �������
     */
    virtual int sendVariant(const QString &domain, const QVariant &variant) = 0;

    /**
     * @brief �������ݣ��첽��
     * @param domain : Ŀ�Ķ�����
     * @param buffer : ���ݻ���
     * @param size : �����ֽ���
     */
    virtual void postBuffer(const std::string &domain, const char *buffer, int size) = 0;

    /**
     * @brief �������ݣ��첽��
     * @param domain : Ŀ�Ķ�����
     * @param msg : ��Ϣ
     */
    virtual void postString(const std::string &domain, const std::string &msg) = 0;

    /**
     * @brief �������ݣ��첽��
     * @param domain : Ŀ�Ķ�����
     * @param data : ����
     */
    virtual void postList(const std::string &domain, const std::list<std::string> &data) = 0;

    /**
     * @brief �������ݣ��첽��
     * @param domain : Ŀ�Ķ�����
     * @param variant : ����
     */
    virtual void postVariant(const QString &domain, const QVariant &variant) = 0;
};

//
#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable : 4407)    // ��ָ���Ա��ʾ��ʽ�Ĳ�ָͬ��֮�����ת�����������������ɲ���ȷ�Ĵ���
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
