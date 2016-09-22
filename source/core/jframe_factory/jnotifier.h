#ifndef JNOTIFIER_H
#define JNOTIFIER_H

#include "jframe_facade.h"

// ��Ϣ�ַ���

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
     * @brief ������Ϣ��ͬ�����㲥��
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual JLRESULT sendMessage(const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ���첽���㲥��
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual void postMessage(const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ���첽���㲥��
     * @param id : ��Ϣ��ʶ
     * @param info : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual void postMessage(const std::string &id, const std::string &info, JLPARAM lParam = 0) = 0;

    /**
     * @brief ��ȡ�˵�����Ϣ�ַ��ӿ�
     * @return �˵�����Ϣ�ַ��ӿ�
     */
    virtual IImmNotify& imm() = 0;

    /**
     * @brief ��ȡ D-BUS ���̼�ͨ�Žӿ�
     * @return D-BUS ���̼�ͨ�Žӿ�
     */
    virtual IDBusNotify& dbus() = 0;

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

/** INotifierImm �ӿ����� */
#define VER_INotifierImm J_INTERFACE_VERSION(1, 0)
#define IID_INotifierImm J_IID_INTERFACE(INotifierImm)

/**
 * @brief �˵��ˣ�ֱ�ӷ��͸�ָ���۲��ߣ��ַ���Ϣ�ӿڶ���
 */
class IImmNotify
{
public:
    /**
     * @brief ��������
     */
    virtual ~IImmNotify() {}

    /**
     * @brief ������Ϣ��ͬ�����˵��ˣ�
     * @param [in] obsid : �۲��߱�ʶ
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual JLRESULT sendMessage(const std::string &obsid, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ��ͬ�����˵��ˣ�
     * @param [in] obs : �۲���
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual JLRESULT sendMessage(JObserver *obs, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ���첽���˵��ˣ�
     * @param obsid : �۲��߱�ʶ
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     */
    virtual void postMessage(const std::string &obsid, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ���첽���˵��ˣ�
     * @param [in] obs : �۲���
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     */
    virtual void postMessage(JObserver *obs, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ���첽���˵��ˣ�
     * @param [in] obsid : �۲��߱�ʶ
     * @param id : ��Ϣ��ʶ
     * @param info : ����1
     * @param lParam : ����2
     */
    virtual void postMessage(const std::string &obsid, const std::string &id, const std::string &info, JLPARAM lParam = 0) = 0;

    /**
     * @brief ������Ϣ���첽���˵��ˣ�
     * @param [in] obs : �۲���
     * @param id : ��Ϣ��ʶ
     * @param info : ����1
     * @param lParam : ����2
     */
    virtual void postMessage(JObserver *obs, const std::string &id, const std::string &info, JLPARAM lParam = 0) = 0;
};

class QObject;

/**
 * @brief ����D-BUS��ͨ�Žӿڶ���
 */
class IDBusNotify
{
public:
    /**
     * @brief ��������
     */
    virtual ~IDBusNotify() {}

    /**
     * @brief isConnected
     * @return
     */
    virtual bool isConnected() = 0;
#if 0   //TODO
    /**
     * @brief ...
     * @param path : ...
     * @param iface : ...
     * @param buffer : ...
     * @param size : ...
     * @return ...
     */
    virtual bool sendData(const std::string &path, const std::string &iface,
                          const char* buffer, size_t size) = 0;

    /**
     * @brief ...
     * @param path : ...
     * @param iface : ...
     * @param buffer : ...
     * @param size : ...
     */
    virtual void postData(const std::string &path, const std::string &iface,
                          const char* buffer, size_t size) = 0;

    //
    virtual std::string &service() = 0;

    virtual std::string path() = 0;

    virtual std::string iface() = 0;
#endif
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
