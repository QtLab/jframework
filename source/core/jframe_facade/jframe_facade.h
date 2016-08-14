#ifndef JFRAME_FACADE_H
#define JFRAME_FACADE_H

#include <string>
#include <list>

/// 版本管理

// 框架产品版本
#define JFRAME_MAJOR_VERSION 4
#define JFRAME_MINOR_VERSION 0
#define JFRAME_PATCH_VERSION 0
#define JFRAME_BUILD_VERSION 0

// 获取主版本
#define J_MAJOR_VERSION(ver) \
    ((((unsigned int)(ver)) & 0xff000000) >> 24)

// 获取次版本
#define J_MINOR_VERSION(ver) \
    ((((unsigned int)(ver)) & 0x00ff0000) >> 16)

// 获取补丁版本
#define J_PATCH_VERSION(ver) \
    ((((unsigned int)(ver)) & 0x0000ff00) >> 8)

// 获取内部版本
#define J_BUILD_VERSION(ver) \
    (((unsigned int)(ver)) & 0x000000ff)

// 接口版本
#define J_INTERFACE_VERSION(major, minor) \
    ((((unsigned int)(JFRAME_MAJOR_VERSION)) << 24) | \
    ((( unsigned int)(major)) << 16) | \
    ((  unsigned int)(minor)  << 8))

// 模块版本
#define J_PROCESS_VERSION(major, minor, patch) \
    ((((unsigned int)(JFRAME_MAJOR_VERSION)) << 24) | \
    ((( unsigned int)(major)) << 16) | \
    ((  unsigned int)(minor)  << 8) | \
    (   unsigned int)(patch))

//
#ifndef J_IID_INTERFACE
#define J_IID_INTERFACE(_interface_) "IID" # _interface_
#endif

/// 内部接口查询

//
#define J_IS_INSTANCEOF(_interface_, _iid_, _ver_) \
    ((strcmp(_iid_, IID_ ## _interface_) == 0) \
    && ((_ver_) == VER_ ## _interface_))

//
#define J_IS_OBJECT(_object_, _iid_, _ver_) \
    ((strcmp(_iid_, _object_->objectIdentity().c_str()) == 0) \
    && ((_ver_) == _object_->objectVersion()))

// 在当前接口子对象中查询

#define J_QUERY_INTERFACE(_interface_, _iid_, _ver_) \
    do { \
        if (J_IS_INSTANCEOF(_interface_, _iid_, _ver_)) \
        { return static_cast<_interface_ *>(this); } \
    } while(0)

// 在给定接口子对象中查询
#define J_QUERY_IJUNKNOWN(_interface_, _iid_, _ver_) \
    do { \
        if (J_IS_INSTANCE(IJUnknown, _iid_, _ver_)) \
        { return static_cast<IJUnknown *>(static_cast<_interface_ *>(this)); } \
    } while(0)

// 查询成员对象实例
#define J_QUERY_MEMBER_OBJECT(_interface_, _iid_, _ver_, _member_) \
    do { \
        if (_member_ != 0) { \
            if (J_IS_INSTANCEOF(_interface_, _iid_, _ver_)) \
            { return static_cast<_interface_ *>(_member_); } \
        } \
    } while(0)

/// 外部接口查询

// 当前接口查询
#define J_QUERY_THIS_INTERFACE(_interface_) \
    ((_interface_ *)queryInterface(IID_ ## _interface_, VER_ ## _interface_))

// 给对象引用查询
#define J_QUERY_OBJECT_INTERFACE(_object_, _interface_) \
    ((_interface_ *)_object_.queryInterface(IID_ ## _interface_, VER_ ## _interface_))

// 给定对象指针查询
#define J_QUERY_POBJECT_INTERFACE(_object_, _interface) \
    do { \
        if (_object_ != 0) { \
            _interface_ *_t = ((_interface_ *)_object_->queryInterface( \
                IID_ ## _interface_, VER_ ## _interface_)); \
            if (_t) { return _t_; } \
        } \
    } while(0)

///

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

//
#ifndef J_TO_STRING
#define J_TO_STRING(_s_) #_s_
#endif

// 框架窗体风格
#define J_FRAME_THEME_OFFICE_2007BLUE       J_TO_STRING(Office2007Blue)
#define J_FRAME_THEME_OFFICE_2007BLACK      J_TO_STRING(Office2007Black)
#define J_FRAME_THEME_OFFICE_2007SILVER     J_TO_STRING(Office2007Silver)
#define J_FRAME_THEME_OFFICE_2007AQUA       J_TO_STRING(Office2007Aqua)
#define J_FRAME_THEME_WINDOWs7_SCENIC       J_TO_STRING(Windows7Scenic)
#define J_FRAME_THEME_OFFICE_2010SILVER     J_TO_STRING(Office2010Silver)
#define J_FRAME_THEME_OFFICE_2010BLUE       J_TO_STRING(Office2010Blue)
#define J_FRAME_THEME_OFFICE_2010BLACK      J_TO_STRING(Office2010Black)

// 接口描述
#define VER_IJUnknown J_INTERFACE_VERSION(1, 0)
#define IID_IJUnknown J_IID_INTERFACE(IJUnknown)

/**
 * @brief The IJUnknown class
 */
class IJUnknown
{
public:
    virtual ~IJUnknown() {}

    // 释放接口
    virtual void releaseInterface() {}

    // 查询接口
    virtual void* queryInterface(const char *iid, unsigned int ver)
    { (void)iid; (void)ver; return 0; }
};

// 接口描述
#define VER_IJObject J_INTERFACE_VERSION(1, 0)
#define IID_IJObject J_IID_INTERFACE(IJObject)

/**
 * @brief The IJObject class
 */
class IJObject : public IJUnknown
{
public:
    virtual ~IJObject() {}

    // 获取对象标识
    virtual std::string objectIdentity() const { return IID_IJObject; }

    // 获取对象版本
    virtual unsigned int objectVersion() const { return VER_IJObject; }

    // 执行函数调用
    virtual bool invoke(const char *method, int argc = 0, ...)
    { (void)method; (void)argc; return false; }

    // 查询invoke函数内部指令信息
    virtual std::list<std::string> queryInvoking() const
    { return std::list<std::string>(); }
};

// 接口标识
#define VER_IJFrameFacade J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameFacade J_IID_INTERFACE(IJFrameFacade)

/**
 * @brief The IJFrameFacade class
 */
class IJFrameFacade : public IJObject
{
public:
    virtual ~IJFrameFacade() {}

    // 获取对象标识
    virtual std::string objectIdentity() const { return IID_IJFrameFacade; }

    // 获取对象版本
    virtual unsigned int objectVersion() const { return VER_IJFrameFacade; }

    // 获取框架路径
    virtual std::string frameDirPath() const = 0;

    // 获取框架进程路径
    virtual std::string appDirPath() const = 0;

    // 获取框架相关配置文件路径
    virtual std::string frameConfigPath() const = 0;
    virtual std::string frameGlobalPath() const = 0;
    virtual std::string frameLayoutPath() const = 0;

    // 获取框架版本
    virtual std::string frameVersion() const = 0;
    virtual bool frameVersion(int &major, int &minor, int &patch) const = 0;

    // 加载框架
    virtual bool loadFrame() = 0;
    virtual bool loadFrame(const char *version) = 0;
    virtual bool loadFrame(int major, int minor, int patch) = 0;

    // 显示/隐藏框架主窗口
    virtual void showFrame(bool show = true, bool maximumed = true) = 0;

    // 退出框架（带提示）
    virtual void tryExitFrame() = 0;

    // 推出框架
    virtual void exitFrame() = 0;

    // 重启框架
    virtual void restartFrame(const std::list<std::string> &arguments) = 0;

    // 执行登陆界面显示
    virtual bool loginFrame() = 0;

    // 注销登录框架
    virtual bool logoutFrame() = 0;

    // 获取环境变量值
    virtual std::string getEnvValue(const char *name) const = 0;

    // 在GF_Application中调用（theApp类的Run函数，实现Qt消息循环调度）
    virtual int runQApp(void *mfcApp) = 0;

    // 获取CWnd*或QWidget*窗口的窗口句柄（winType：“QWidget”、“CWnd”）
    virtual long windowHandle(void *window, const char *winType) = 0;

    // 获取软件系统语言
    virtual std::string language() const = 0;
};

///

#ifdef JFRAME_FACADE_DLL
#ifdef _MSC_VER
#   ifdef JFRAME_FACADE_MAKEDLL
#       define JFRAME_FACADE_EXPORT __declspec(dllexport)
#   else
#       define JFRAME_FACADE_EXPORT __declspec(dllimport)

#       ifdef _MSC_VER
#           pragma comment(lib, "jframe_facaded.lib")
#       else
#           pragma comment(lib, "jframe_facade.lib")
#       endif // !_MSC_VER

#   endif // !JFRAME_FACADE_MAKEDLL
#else
#define JFRAME_FACADE_EXPORT
#endif // _MSC_VER

//
JFRAME_FACADE_EXPORT IJFrameFacade* jframeFacade();

#endif // JFRAME_FACADE_DLL

#ifndef JFRAME_FACADE_EXPORT
#define JFRAME_FACADE_EXPORT
#endif

/// 框架::日志

#ifdef JFRAME_FACADE_DLL

//
#ifndef __FILENAME__
#ifdef _MSC_VER
#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)
#endif
#endif

// 引用宏（type: char*, msg, char*）
#define jframeLog(type, msg) \
    jframeFacade()->invoke("log", 5, type, msg, __FILENAME__, __LINE__, __FUNCTION__)

// log - emerge - (msg: char*)
#define jframeLogEmerge(msg) \
    jframeLog("emerge", msg)

// log - fatal - (msg: char*)
#define jframeLogFatal(msg) \
    jframeLog("fatal", msg)

// log - alert - (msg: char*)
#define jframeLogAlert(msg) \
    jframeLog("alert", msg)

// log - crit - (msg: char*)
#define jframeLogCrit(msg) \
    jframeLog("crit", msg)

// log - error - (msg: char*)
#define jframeLogError(msg) \
    jframeLog("error", msg)

// log - warn - (msg: char*)
#define jframeLogWarning(msg) \
    jframeLog("warn", msg)

// log - notice - (msg: char*)
#define jframeLogNotice(msg) \
    jframeLog("notice", msg)

// log - info - (msg: char*)
#define jframeLogInfo(msg) \
    jframeLog("info", msg)

// log - debug - (msg: char*)
#define jframeLogDebug(msg) \
    jframeLog("debug", msg)

#endif

#endif // JFRAME_FACADE_H
