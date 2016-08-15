#ifndef JFRAME_FACTORY_H
#define JFRAME_FACTORY_H

#include "jframe_facade.h"

// �ӿ�����
#define VER_IJFrameFactory J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameFactory J_IID_INTERFACE(IJFrameFactory)

/**
 * @brief The IJFrameFactory class
 */
class IJFrameFactory : public IJUnknown
{
public:
    /**
     * @brief ~IJFrameFactory : ��������
     */
    virtual ~IJFrameFactory() {}

    /**
     * @brief interfaceIdentity : ��ȡ�ӿڱ�ʶ
     * @return : �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameFactory; }

    /**
     * @brief interfaceVersion : ��ȡ�ӿڰ汾
     * @return : �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameFactory; }

    /**
     * @brief factory : ����ָ������ʵ��
     * @param iid : �ӿڱ�ʶ
     * @param ver : �ӿڰ汾
     * @return : ����ʵ��
     */
    virtual void* factory(const std::string &iid, unsigned int ver) = 0;

    /**
     * @brief releaseFactory : ���ٴ����Ķ���ʵ��
     * @param iface : ����ʵ��
     * @param iid : �ӿڱ�ʶ
     * @param ver : �ӿڰ汾
     */
    virtual void releaseFactory(void *iface, const std::string &iid, unsigned int ver) = 0;
};

///

#ifdef JFRAME_FACTORY_DLL
#ifdef _MSC_VER
#   ifdef JFRAME_FACTORY_MAKEDLL
#       define JFRAME_FACTORY_EXPORT __declspec(dllexport)
#   else
#       define JFRAME_FACTORY_EXPORT __declspec(dllimport)

#       ifdef _MSC_VER
#           pragma comment(lib, "jframe_factoryd.lib")
#       else
#           pragma comment(lib, "jframe_factory.lib")
#       endif // !_MSC_VER

#   endif // !JFRAME_FACTORY_MAKEDLL
#else
#define JFRAME_FACTORY_EXPORT
#endif // _MSC_VER

/**
 * @brief jframeFactory : ��ȡ��ܹ���ϵͳ��ʵ��
 * @return : ��ܹ���ϵͳ��ʵ��
 */
JFRAME_FACTORY_EXPORT IJFrameFactory* jframeFactory();

//
#define JFRAME_FACTORY_CREATE(_interface_) \
    reinterpret_cast<_interface_ *> \
    (jframeFactory()->factory(IID_ ## _interface_, VER_ ## _interface_))

//
#define JFRAME_FACTORY_RELEASE(_object_, _interface_) \
    do { \
        (jframeFactory()->releaseFactory(_object_, IID_ ## _interface_, VER_ ## _interface_)); \
        (_object_) = 0; \
    } while (0)

#else

//
#define JFRAME_FACTORY_CREATE(_factory_, _interface_) \
    reinterpret_cast<_interface_ *> \
    ((_factory_)->factory(IID_ ## _interface_, VER_ ## _interface_))

//
#define JFRAME_FACTORY_RELEASE(_factory_, _object_, _interface_) \
    do { \
        ((_factory_)->releaseFactory(_object_, \IID_ ## _interface_, VER_ ## _interface_)); \
        (_object_) = 0; \
    } while (0)

#endif // JFRAME_FACTORY_DLL

#ifndef JFRAME_FACTORY_EXPORT
#define JFRAME_FACTORY_EXPORT
#endif

#endif // JFRAME_FACTORY_H
