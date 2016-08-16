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
    (((_iid_) == IID_ ## _interface_) \
    && ((_ver_) == VER_ ## _interface_))

//
#define J_IS_INTERFACEOF(_interface_, _iid_, _ver_) \
    (((_iid_) == (_interface_)->interfaceIdentity()) \
    && ((_ver_) == (_interface_)->interfaceVersion()))

// 在当前接口子对象中查询

#define J_QUERY_INTERFACE(_interface_, _iid_, _ver_) \
    do { \
        if (J_IS_INSTANCEOF(_interface_, _iid_, _ver_)) \
        { return static_cast<_interface_ *>(this); } \
    } while(0)

// 在给定接口子对象中查询
#define J_QUERY_IJUNKNOWN(_interface_, _iid_, _ver_) \
    do { \
        if (J_IS_INSTANCEOF(IJUnknown, _iid_, _ver_)) \
        { return static_cast<IJUnknown *>(static_cast<_interface_ *>(this)); } \
    } while(0)

// 查询成员对象实例
#define J_QUERY_MEMBER_OBJECT(_interface_, _iid_, _ver_, _object_) \
    do { \
        if ((_object_) != 0) { \
            if (J_IS_INSTANCEOF(_interface_, _iid_, _ver_)) \
            { return static_cast<_interface_ *>(_object_); } \
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
    /**
     * @brief ~IJUnknown : 析构函数
     */
    virtual ~IJUnknown() {}

    /**
     * @brief interfaceIdentity : 获取接口标识
     * @return : 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJUnknown; }

    /**
     * @brief interfaceVersion : 获取接口版本
     * @return : 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJUnknown; }

    /**
     * @brief queryInterface : 查询接口
     * @param iid : 接口标识
     * @param ver : 接口版本
     * @return : 接口实例
     */
    virtual void* queryInterface(const std::string &iid, unsigned int ver)
    { (void)iid; (void)ver; return 0; }

    /**
     * @brief loadInterface : 加载接口
     * @return : true，加载成功；false，加载失败
     */
    virtual bool loadInterface() { return true; }

    /**
     * @brief releaseInterface : 释放接口
     */
    virtual void releaseInterface() {}

    /**
     * @brief queryMethod : 查询invoke函数内部指令信息
     * @return : 内部指令集
     */
    virtual std::list<std::string> queryMethod() const
    { return std::list<std::string>(); }

    /**
     * @brief invokeMethod : 执行函数调用
     * @param method : 内部函数名称
     * @param argc : 可变参数个数
     * @param ... : 可变参数
     * @return : 调用结果。true，调用成功；false，调用失败
     */
    virtual bool invokeMethod(const std::string &method, int argc = 0, ...)
    { (void)method; (void)argc; return false; }
};

// 接口标识
#define VER_IJFrameFacade J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameFacade J_IID_INTERFACE(IJFrameFacade)

/**
 * @brief The IJFrameFacade class
 */
class IJFrameFacade : public IJUnknown
{
public:
    /**
     * @brief ~IJFrameFacade : 析构函数
     */
    virtual ~IJFrameFacade() {}

    /**
     * @brief interfaceIdentity : 获取接口标识
     * @return : 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameFacade; }

    /**
     * @brief interfaceVersion : 获取接口版本
     * @return : 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameFacade; }

    /**
     * @brief frameDirPath : 获取框架路径
     * @return : 框架路径
     */
    virtual std::string frameDirPath() const = 0;

    /**
     * @brief appDirPath : 获取软件实体路径
     * @return : 软件实体路径
     */
    virtual std::string appDirPath() const = 0;

    /**
     * @brief frameConfigPath : 获取框架配置文件夹路径
     * @return : 框架配置文件夹路径
     */
    virtual std::string frameConfigPath() const = 0;

    /**
     * @brief frameGlobalPath : 获取框架全局配置文件路径
     * @return  : 框架全局配置文件路径
     */
    virtual std::string frameGlobalPath() const = 0;

    /**
     * @brief frameLayoutPath : 获取框架布局配置文件路径
     * @return : 框架布局配置文件路径
     */
    virtual std::string frameLayoutPath() const = 0;

    /**
     * @brief frameVersion : 获取框架版本
     * @return : 框架版本。为空时表示获取失败
     */
    virtual std::string frameVersion() const = 0;

    /**
     * @brief frameVersion : 获取框架版本
     * @param major : 主版本号
     * @param minor : 次版本号
     * @param patch : 补丁版本号
     * @return : true，获取成功；false，获取失败
     */
    virtual bool frameVersion(int &major, int &minor, int &patch) const = 0;

    /**
     * @brief loadFrame : 加载框架
     * @param argc : 参数个数
     * @param argv : 参数列表
     * @param app : 外部app实体（如MFC框架的CWinApp*）
     * @return : 执行结果。true，加载成功；false，加载失败
     */
    virtual bool loadFrame(int *argc = 0, char** argv = 0, void *app = 0) = 0;

    /**
     * @brief showFrame : 显示/隐藏框架主窗口
     * @param show : 显示标志。true，显示；false，隐藏
     * @param maximized : 最大化标志。true，最大化；false，正常
     */
    virtual void showFrame(bool show = true, bool maximized = true) = 0;

    /**
     * @brief tryExitFrame : 退出框架（带提示）
     */
    virtual void tryExitFrame() = 0;

    /**
     * @brief exitFrame : 退出框架
     */
    virtual void exitFrame() = 0;

    /**
     * @brief restartFrame : 重启框架
     * @param arguments : 参数列表
     */
    virtual void restartFrame(const std::list<std::string> &arguments) = 0;

    /**
     * @brief loginFrame : 执行登陆界面显示
     * @return : 执行结果。true，登录成功；false，登录失败
     */
    virtual bool loginFrame() = 0;

    /**
     * @brief logoutFrame : 注销登录框架
     * @return : 执行结果。true，注销成功；false，注销失败
     */
    virtual bool logoutFrame() = 0;

    /**
     * @brief getEnvValue : 获取环境变量值
     * @param name : 环境变量名称
     * @return : 环境变量值
     */
    virtual std::string getEnvValue(const std::string &name) const = 0;

    /**
     * @brief runQApp : 在MFC的theApp类中调用（theApp类的Run函数，实现Qt消息循环调度）
     * @param mfcApp : CWinApp类型实例，即theApp
     * @return : Qt环境退出码
     */
    virtual int runQApp(void *mfcApp) = 0;

    /**
     * @brief windowHandle : 获取窗口实例的句柄
     * @param window : 目标窗口实例（CWnd类型、QWidget类型）
     * @param winType : 目标窗口类型。1）"QWidget"；2）"CWnd"
     * @return : 目标窗口窗口句柄（HWND）
     */
    virtual long windowHandle(void *window, const std::string &winType) = 0;

    /**
     * @brief language : 获取软件系统语言
     * @return : 软件系统语言
     */
    virtual std::string language() const = 0;
};

//
typedef IJUnknown *(__cdecl *FuncFrameFacadeInst)(void);

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

/**
 * @brief jframeFacade : 获取框架门面系统单实例
 * @return : 框架门面系统单实例
 */
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
    jframeFacade()->invokeMethod("log", 5, type, msg, __FILENAME__, __LINE__, __FUNCTION__)

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
