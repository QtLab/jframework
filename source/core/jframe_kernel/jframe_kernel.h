#ifndef JFRAME_KERNEL_H
#define JFRAME_KERNEL_H

#include "jframe_facade.h"

// �ӿ�����
#define VER_IJFrameKernel J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameKernel J_IID_INTERFACE(IJFrameKernel)

class IJLogManager;
class IJFrameCore;
class IJFrameLayout;
class IJFrameLogin;

/**
 * @brief The IJFrameKernel class
 */
class IJFrameKernel : public IJUnknown
{
public:
    virtual ~IJFrameKernel() {}

    /**
     * @brief intefcaceIdentity : ��ȡ�ӿڱ�ʶ
     * @return : �ӿڱ�ʶ
     */
    virtual std::string intefcaceIdentity() const { return IID_IJFrameKernel; }

    /**
     * @brief interfaceVersion : ��ȡ�ӿڰ汾
     * @return : �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameKernel; }

    /**
     * @brief logManager : ��ȡ��־������
     * @return : ��־������
     */
    virtual IJLogManager *logManager() = 0;

    /**
     * @brief frameCore : ��ȡ��ܺ���ϵͳ�ӿ�
     * @return : ��ܺ���ϵͳ�ӿ�
     */
    virtual IJFrameCore *frameCore() = 0;

    /**
     * @brief frameLayout : ��ȡ��ܲ���ϵͳ�ӿ�
     * @return : ��ܲ���ϵͳ�ӿ�
     */
    virtual IJFrameLayout *frameLayout() = 0;

    /**
     * @brief frameLogin : ��ȡ��ܵ�¼ϵͳ�ӿ�
     * @return : ��ܵ�¼ϵͳ�ӿ�
     */
    virtual IJFrameLogin *frameLogin() = 0;
};

///

#ifdef JFRAME_KERNEL_DLL
#ifdef _MSC_VER
#   ifdef JFRAME_KERNEL_MAKEDLL
#       define JFRAME_KERNEL_EXPORT __declspec(dllexport)
#   else
#       define JFRAME_KERNEL_EXPORT __declspec(dllimport)

#       ifdef _MSC_VER
#           pragma comment(lib, "jframe_kerneld.lib")
#       else
#           pragma comment(lib, "jframe_kernel.lib")
#       endif // !_MSC_VER

#   endif // !JFRAME_KERNEL_MAKEDLL
#else
#define JFRAME_KERNEL_EXPORT
#endif // _MSC_VER

#endif // JFRAME_KERNEL_DLL

#ifndef JFRAME_KERNEL_EXPORT
#define JFRAME_KERNEL_EXPORT
#endif

#ifdef JFRAME_KERNEL_DLL

/**
 * @brief jframeKernel : ��ȡ����ں�ϵͳ�ӿڵ�ʵ��
 * @return : ����ں�ϵͳ�ӿڵ�ʵ��
 */
JFRAME_KERNEL_EXPORT IJFrameKernel* jframeKernel();

//
#ifndef JFRAME_LOGMANAGER_INSTANCE
#define JFRAME_LOGMANAGER_INSTANCE
#define jlogManager() jframeKernel()->logManager()
#endif

//
#ifndef JFRAME_CORE_INSTANCE
#define JFRAME_CORE_INSTANCE
#define jframeCore() jframeKernel()->frameCore()
#endif

//
#ifndef JFRAME_LAYOUT_INSTANCE
#define JFRAME_LAYOUT_INSTANCE
#define jframeLayout() jframeKernel()->frameLayout()
#endif

//
#ifndef JFRAME_LOGIN_INSTANCE
#define JFRAME_LOGIN_INSTANCE
#define jframeLogin() jframeKernel()->frameLogin()
#endif

//
#ifndef JFRAME_LOGINMANAGER_INSTANCE
#define JFRAME_LOGINMANAGER_INSTANCE
#define jloginManager() jframeLogin()->loginManager()
#endif

//
#ifndef JFRAME_LOGINDBMGR_INSTANCE
#define JFRAME_LOGINDBMGR_INSTANCE
#define jloginDBMgr() jloginManager()->loginDBMgr()
#endif

#endif // JFRAME_KERNEL_DLL

#endif // JFRAME_KERNEL_H
