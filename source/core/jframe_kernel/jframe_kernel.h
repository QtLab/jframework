#ifndef JFRAME_KERNEL_H
#define JFRAME_KERNEL_H

#include "jframe_facade.h"

/** IJFrameKernel �ӿ����� */
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
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string intefcaceIdentity() const { return IID_IJFrameKernel; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameKernel; }

    /**
     * @brief ��ȡ��־������
     * @return ��־������
     */
    virtual IJLogManager *logManager() = 0;

    /**
     * @brief ��ȡ��ܺ���ϵͳ�ӿ�
     * @return ��ܺ���ϵͳ�ӿ�
     */
    virtual IJFrameCore *frameCore() = 0;

    /**
     * @brief ��ȡ��ܲ���ϵͳ�ӿ�
     * @return ��ܲ���ϵͳ�ӿ�
     */
    virtual IJFrameLayout *frameLayout() = 0;

    /**
     * @brief ��ȡ��ܵ�¼ϵͳ�ӿ�
     * @return ��ܵ�¼ϵͳ�ӿ�
     */
    virtual IJFrameLogin *frameLogin() = 0;
};

//

#ifdef JFRAME_KERNEL_LIB
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__MINGW32__)
#   ifdef JFRAME_KERNEL_BUILD
#       define JFRAME_KERNEL_EXPORT __declspec(dllexport)
#   else
#       define JFRAME_KERNEL_EXPORT __declspec(dllimport)
#       if defined(DEBUG) || defined(_DEBUG)
#           pragma comment(lib, "jframe_kerneld.lib")
#       else
#           pragma comment(lib, "jframe_kernel.lib")
#       endif
#   endif // !JFRAME_KERNEL_BUILD
#endif // _MSC_VER || ...
#endif // JFRAME_KERNEL_LIB

#ifndef JFRAME_KERNEL_EXPORT
#define JFRAME_KERNEL_EXPORT
#endif

/**
 * @brief ��ȡ����ں�ϵͳ�ӿڵ�ʵ��
 * @return ����ں�ϵͳ�ӿڵ�ʵ��
 */
JFRAME_KERNEL_EXPORT IJFrameKernel* jframeKernel();

#ifdef JFRAME_KERNEL_LIB

//
#ifndef JFRAME_LOGMANAGER_INSTANCE
#define JFRAME_LOGMANAGER_INSTANCE
#define jlogManager() jframeKernel()->logManager()      /** ��ȡ��־������ */
#endif

//
#ifndef JFRAME_CORE_INSTANCE
#define JFRAME_CORE_INSTANCE
#define jframeCore() jframeKernel()->frameCore()        /** ��ȡ��ܺ���ϵͳ�ӿ� */
#endif

//
#ifndef JFRAME_LAYOUT_INSTANCE
#define JFRAME_LAYOUT_INSTANCE
#define jframeLayout() jframeKernel()->frameLayout()    /** ��ȡ��ܲ���ϵͳ�ӿ� */
#endif

//
#ifndef JFRAME_LOGIN_INSTANCE
#define JFRAME_LOGIN_INSTANCE
#define jframeLogin() jframeKernel()->frameLogin()      /** ��ȡ��ܵ�¼ϵͳ�ӿ� */
#endif

//
#ifndef JFRAME_LOGINMANAGER_INSTANCE
#define JFRAME_LOGINMANAGER_INSTANCE
#define jloginManager() jframeLogin()->loginManager()   /** ��ȡ��ܵ�¼�������ӿ� */
#endif

//
#ifndef JFRAME_LOGINDBMGR_INSTANCE
#define JFRAME_LOGINDBMGR_INSTANCE
#define jloginDBMgr() jloginManager()->loginDBMgr()     /** ��ȡ��ܵ�¼���ݿ�������ӿ� */
#endif

#endif // JFRAME_KERNEL_LIB

//
#ifndef QStringLiteral
#define QStringLiteral(str) QString(str)
#endif  // QStringLiteral

#endif // JFRAME_KERNEL_H
