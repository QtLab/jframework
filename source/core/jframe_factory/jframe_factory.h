#ifndef JFRAME_FACTORY_H
#define JFRAME_FACTORY_H

#include "jframe_core.h"

// �ӿ�����
#define VER_IJFrameFactory J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameFactory J_IID_INTERFACE(IJFrameFactory)

/**
 * @brief The IJFrameFactory class
 */
class IJFrameFactory : public IJObject
{
public:
    virtual ~IJFrameFactory() {}

    // ����ָ������ʵ����iid�������ʶ����ver������汾
    virtual void* factory(const char *iid, unsigned int ver) = 0;

    // ���ٴ����Ķ���ʵ����object������ʵ��ָ�룬iid�������ʶ��ver������汾��
    virtual void releaseFactory(void *iface, const char *iid, unsigned int ver) = 0;
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

//
JFRAME_FACTORY_EXPORT IJFrameFactory* jframeFactory();

//
#define JFRAME_FACTORY_CREATE(_interface_) \
    reinterpret_cast<_interface_ *> \
    (jframeFactory()->factory(IID_ ## _interface_, VER_ ## _interface_))

//
#define JFRAME_FACTORY_RELEASE(_object_, _interface_) \
    (jframeFactory()->releaseFactory(_object_, IID_ ## _interface_, VER_ ## _interface_))

#endif // JFRAME_FACTORY_DLL

#ifndef JFRAME_FACTORY_EXPORT
#define JFRAME_FACTORY_EXPORT
#endif

#endif // JFRAME_FACTORY_H
