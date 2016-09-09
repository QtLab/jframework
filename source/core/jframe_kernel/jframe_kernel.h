#ifndef JFRAME_KERNEL_H
#define JFRAME_KERNEL_H

#include "jframe_facade.h"

/** IJFrameKernel 接口描述 */
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
     * @brief 获取接口标识
     * @return 接口标识
     */
    virtual std::string intefcaceIdentity() const { return IID_IJFrameKernel; }

    /**
     * @brief 获取接口版本
     * @return 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameKernel; }

    /**
     * @brief 获取日志管理器
     * @return 日志管理器
     */
    virtual IJLogManager *logManager() = 0;

    /**
     * @brief 获取框架核心系统接口
     * @return 框架核心系统接口
     */
    virtual IJFrameCore *frameCore() = 0;

    /**
     * @brief 获取框架布局系统接口
     * @return 框架布局系统接口
     */
    virtual IJFrameLayout *frameLayout() = 0;

    /**
     * @brief 获取框架登录系统接口
     * @return 框架登录系统接口
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
 * @brief 获取框架内核系统接口单实例
 * @return 框架内核系统接口单实例
 */
JFRAME_KERNEL_EXPORT IJFrameKernel* jframeKernel();

#ifdef JFRAME_KERNEL_LIB

//
#ifndef JFRAME_LOGMANAGER_INSTANCE
#define JFRAME_LOGMANAGER_INSTANCE
#define jlogManager() jframeKernel()->logManager()      /** 获取日志管理器 */
#endif

//
#ifndef JFRAME_CORE_INSTANCE
#define JFRAME_CORE_INSTANCE
#define jframeCore() jframeKernel()->frameCore()        /** 获取框架核心系统接口 */
#endif

//
#ifndef JFRAME_LAYOUT_INSTANCE
#define JFRAME_LAYOUT_INSTANCE
#define jframeLayout() jframeKernel()->frameLayout()    /** 获取框架布局系统接口 */
#endif

//
#ifndef JFRAME_LOGIN_INSTANCE
#define JFRAME_LOGIN_INSTANCE
#define jframeLogin() jframeKernel()->frameLogin()      /** 获取框架登录系统接口 */
#endif

//
#ifndef JFRAME_LOGINMANAGER_INSTANCE
#define JFRAME_LOGINMANAGER_INSTANCE
#define jloginManager() jframeLogin()->loginManager()   /** 获取框架登录管理器接口 */
#endif

//
#ifndef JFRAME_LOGINDBMGR_INSTANCE
#define JFRAME_LOGINDBMGR_INSTANCE
#define jloginDBMgr() jloginManager()->loginDBMgr()     /** 获取框架登录数据库管理器接口 */
#endif

#endif // JFRAME_KERNEL_LIB

//
#ifndef QStringLiteral
#define QStringLiteral(str) QString(str)
#endif  // QStringLiteral

#endif // JFRAME_KERNEL_H
