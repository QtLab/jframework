#ifndef JFRAME_FACADE_H
#define JFRAME_FACADE_H

#include <string>
#include <list>

// 框架产品版本
#define JFRAME_MAJOR_VERSION 4      /**< 框架主版本号 */
#define JFRAME_MINOR_VERSION 0      /**< 框架次版本号 */
#define JFRAME_PATCH_VERSION 0      /**< 框架补丁版本号 */
#define JFRAME_BUILD_VERSION 0      /**< 框架编译版本号 */

/**
 * @brief 获取主版本
 * @param ver : 框架版本号
 * @return int类型值
 */
#define J_MAJOR_VERSION(ver) \
    ((((unsigned int)(ver)) & 0xff000000) >> 24)

/**
 * @brief 获取次版本
 * @param ver : 框架版本号
 * @return int类型值
 */
#define J_MINOR_VERSION(ver) \
    ((((unsigned int)(ver)) & 0x00ff0000) >> 16)

/**
 * @brief 获取补丁版本
 * @param ver : 框架版本号
 * @return int类型值
 */
#define J_PATCH_VERSION(ver) \
    ((((unsigned int)(ver)) & 0x0000ff00) >> 8)

/**
 * @brief 获取内部版本
 * @param ver : 框架版本号
 * @return int类型值
 */
#define J_BUILD_VERSION(ver) \
    (((unsigned int)(ver)) & 0x000000ff)

/**
 * @brief 生成接口版本
 * @param major : 主版本号
 * @param minor : 次版本号
 * @return int类型值
 */
#define J_INTERFACE_VERSION(major, minor) \
    ((((unsigned int)(JFRAME_MAJOR_VERSION)) << 24) | \
    ((( unsigned int)(major)) << 16) | \
    ((  unsigned int)(minor)  << 8))

/**
 * @brief 生成进程版本
 * @param major : 主版本号
 * @param minor : 次版本号
 * @param patch : 修订版本号
 * @return int类型值
 */
#define J_PROCESS_VERSION(major, minor, patch) \
    ((((unsigned int)(JFRAME_MAJOR_VERSION)) << 24) | \
    ((( unsigned int)(major)) << 16) | \
    ((  unsigned int)(minor)  << 8) | \
    (   unsigned int)(patch))

#ifndef J_IID_INTERFACE
/**
 * @brief 接口标识定义
 * @param interface : 接口名称
 * @return char*类型值
 */
#define J_IID_INTERFACE(interface)  "IID" # interface
#endif

/**
 * @brief 接口标识判定
 * @param interface : 接口名称
 * @param iid : 接口标识
 * @param ver : 接口版本
 * @return bool类型值
 */
#define J_IS_INSTANCEOF(interface, iid, ver) \
    (((iid) == IID_ ## interface) \
    && ((ver) == VER_ ## interface))

/**
 * @brief 接口对象标识判定
 * @param interface : 接口名称
 * @param iid : 接口标识
 * @param ver : 接口版本
 * @return bool类型值
 */
#define J_IS_INTERFACEOF(interface, iid, ver) \
    (((iid) == (interface)->interfaceIdentity()) \
    && ((ver) == (interface)->interfaceVersion()))

/**
 * @brief 在当前接口子对象中查询接口
 * @param interface : 接口名称
 * @param iid : 接口标识
 * @param ver : 接口版本
 */
#define J_QUERY_INTERFACE(interface, iid, ver) \
    do { \
        if (J_IS_INSTANCEOF(interface, iid, ver)) \
        { return static_cast<interface *>(this); } \
    } while(0)

/**
 * @brief 在给定接口子对象中查询接口
 * @param interface : 接口名称
 * @param iid : 接口标识
 * @param ver : 接口版本
 */
#define J_QUERY_IJUNKNOWN(interface, iid, ver) \
    do { \
        if (J_IS_INSTANCEOF(IJUnknown, iid, ver)) \
        { return static_cast<IJUnknown *>(static_cast<interface *>(this)); } \
    } while(0)

/**
 * @brief 查询成员对象实例
 * @param interface : 接口名称
 * @param iid : 接口标识
 * @param ver : 接口版本
 * @param object : 成员对象
 */
#define J_QUERY_MEMBER_OBJECT(interface, iid, ver, object) \
    do { \
        if ((object) != 0) { \
            if (J_IS_INSTANCEOF(interface, iid, ver)) \
            { return static_cast<interface *>(object); } \
        } \
    } while(0)

/**
 * @brief 当前接口查询
 * @param interface : 接口名称
 */
#define J_QUERY_THIS_INTERFACE(interface) \
    ((interface *)queryInterface(IID_ ## interface, VER_ ## interface))

/**
 * @brief 给对象引用查询
 * @param object : 查询对象
 * @param interface : 接口名称
 */
#define J_QUERY_OBJECT_INTERFACE(object, interface) \
    ((interface *)object.queryInterface(IID_ ## interface, VER_ ## interface))

/**
 * @brief 给定对象指针查询
 * @param object : 查询对象
 * @param interface : 接口名称
 */
#define J_QUERY_POBJECT_INTERFACE(object, interface) \
    do { \
        if (object != 0) { \
            interface *_t = ((_interface_ *)object->queryInterface( \
            IID_ ## interface, VER_ ## interface)); \
            if (_t) { return _t_; } \
        } \
    } while(0)

//
#ifdef _MSC_VER
#define J_PATH_MAX MAX_PATH
#elif defined(__unix__)
#include <limits.h>
#define J_PATH_MAX PATH_MAX
#else
#define J_PATH_MAX 1024
#endif

//
#ifndef J_ATTR_CDECL
#if defined(_MSC_VER)
#  ifdef __cplusplus
#    define J_EXTERN_C extern "C"
#  else
#    define J_EXTERN_C extern
#  endif
#define J_ATTR_CDECL __cdecl
#define J_ATTR_STDCALL __stdcall
#define J_ATTR_EXPORT __declspec(dllexport)
#define J_EXTERN extern
#elif defined(__APPLE__)
#  ifdef __cplusplus
#    define J_EXTERN_C extern "C"
#  else
#    define J_EXTERN_C extern
#  endif
#define J_ATTR_CDECL
#define J_ATTR_STDCALL
#define J_ATTR_EXPORT
#define J_EXTERN
#elif defined(__unix__)
#  ifdef __cplusplus
#    define J_EXTERN_C extern "C"
#  else
#    define J_EXTERN_C extern
#  endif
#define J_ATTR_CDECL //__attribute__((__cdecl__))
#define J_ATTR_STDCALL __attribute__((__stdcall__))
#define J_ATTR_EXPORT __attribute__((visibility("default")))
#define J_EXTERN
#endif
#endif  // J_ATTR_CDECL

///

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

//
#ifndef J_TO_STRING
#define J_TO_STRING(_s_) #_s_
#endif

#ifndef J_FRAME_THEME
#define J_FRAME_THEME   /**< J_FRAME_THEME 宏 */
#define J_FRAME_THEME_OFFICE_2007BLUE       J_TO_STRING(Office2007Blue)     /**< Office2007Blue 风格 */
#define J_FRAME_THEME_OFFICE_2007BLACK      J_TO_STRING(Office2007Black)    /**< Office2007Black 风格 */
#define J_FRAME_THEME_OFFICE_2007SILVER     J_TO_STRING(Office2007Silver)   /**< Office2007Silver 风格 */
#define J_FRAME_THEME_OFFICE_2007AQUA       J_TO_STRING(Office2007Aqua)     /**< Office2007Aqua 风格 */
#define J_FRAME_THEME_WINDOWs7_SCENIC       J_TO_STRING(Windows7Scenic)     /**< Windows7Scenic 风格 */
#define J_FRAME_THEME_OFFICE_2010SILVER     J_TO_STRING(Office2010Silver)   /**< Office2010Silver 风格 */
#define J_FRAME_THEME_OFFICE_2010BLUE       J_TO_STRING(Office2010Blue)     /**< Office2010Blue 风格 */
#define J_FRAME_THEME_OFFICE_2010BLACK      J_TO_STRING(Office2010Black)    /**< Office2010Black 风格 */
#endif // J_FRAME_THEME

/** IJUnoknown 接口描述 */
#define VER_IJUnknown J_INTERFACE_VERSION(1, 0)
#define IID_IJUnknown J_IID_INTERFACE(IJUnknown)

/**
 * @brief The IJUnknown class
 */
class IJUnknown
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~IJUnknown() {}

    /**
     * @brief 获取接口标识
     * @return 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJUnknown; }

    /**
     * @brief 获取接口版本
     * @return 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJUnknown; }

    /**
     * @brief 查询接口
     * @param [in] iid : 接口标识
     * @param [in] ver : 接口版本
     * @return 接口实例
     */
    virtual void* queryInterface(const std::string &iid, unsigned int ver)
    { (void)iid; (void)ver; return 0; }

    /**
     * @brief 加载接口
     * @return true，加载成功；false，加载失败
     */
    virtual bool loadInterface() { return true; }

    /**
     * @brief 更新接口
     * @return true，更新成功；false，更新失败
     */
    virtual bool updateInterface() { return true; }

    /**
     * @brief 释放接口
     */
    virtual void releaseInterface() {}

    /**
     * @brief 查询invoke函数内部指令信息
     * @return 内部指令集
     */
    virtual std::list<std::string> queryMethod() const
    { return std::list<std::string>(); }

    /**
     * @brief 执行函数调用
     * @param [in] method : 内部函数名称
     * @param [in] argc : 可变参数个数
     * @param [in] ... : 可变参数
     * @return 调用结果。true，调用成功；false，调用失败
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
     * @brief 析构函数
     */
    virtual ~IJFrameFacade() {}

    /**
     * @brief 获取接口标识
     * @return 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameFacade; }

    /**
     * @brief 获取接口版本
     * @return 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameFacade; }

    /**
     * @brief 获取软件名称
     * @return 软件名称
     */
    virtual std::string appName() const = 0;

    /**
     * @brief 获取软件实体路径
     * @return 软件实体路径
     */
    virtual std::string appDirPath() const = 0;

    /**
     * @brief 获取框架配置文件夹路径
     * @return 框架配置文件夹路径
     */
    virtual std::string configDirPath() const = 0;

    /**
     * @brief 获取软件实体部署路径（application可执行文件上一级路径）
     * @return 框架部署路径
     */
    virtual std::string thisDirPath() const = 0;

    /**
     * @brief 获取框架路径
     * @return 框架路径
     */
    virtual std::string frameDirPath() const = 0;

    /**
     * @brief 获取框架全局配置文件路径
     * @return 框架全局配置文件路径
     */
    virtual std::string frameGlobalPath() const = 0;

    /**
     * @brief 获取框架版本
     * @return 框架版本。为空时表示获取失败
     */
    virtual std::string frameVersion() const = 0;

    /**
     * @brief 获取框架版本
     * @param [out] major : 主版本号
     * @param [out] minor : 次版本号
     * @param [out] patch : 补丁版本号
     * @return true，获取成功；false，获取失败
     */
    virtual bool frameVersion(int &major, int &minor, int &patch) const = 0;

    /**
     * @brief 加载框架
     * @param argc : 参数个数
     * @param [in] argv : 参数列表
     * @param [in] app : 外部app实体（如MFC框架的CWinApp*）
     * @return 执行结果。true，加载成功；false，加载失败
     */
    virtual bool loadFrame(int argc = 0, char** argv = 0, void *app = 0) = 0;

    /**
     * @brief 显示/隐藏框架主窗口
     * @param show : 显示标志。true，显示；false，隐藏
     * @param maximized : 最大化标志。true，最大化；false，正常
     */
    virtual void showFrame(bool show = true, bool maximized = true) = 0;

    /**
     * @brief 退出框架（带提示）
     */
    virtual void tryExitFrame() = 0;

    /**
     * @brief 退出框架
     */
    virtual void exitFrame() = 0;

    /**
     * @brief 重启框架
     * @param [in] arguments : 参数列表
     */
    virtual void restartFrame(const std::list<std::string> &arguments) = 0;

    /**
     * @brief 执行登录界面显示
     * @return 执行结果。true，登录成功；false，登录失败
     */
    virtual bool loginFrame() = 0;

    /**
     * @brief 注销登录框架
     * @return 执行结果。true，注销成功；false，注销失败
     */
    virtual bool logoutFrame() = 0;

    /**
     * @brief 获取环境变量值
     * @param [in] name : 环境变量名称
     * @return 环境变量值
     */
    virtual std::string getEnvValue(const std::string &name) const = 0;

    /**
     * @brief 在MFC的theApp类中调用（theApp类的Run函数，实现Qt消息循环调度）
     * @param [in] mfcApp : CWinApp类型实例，即theApp
     * @return Qt环境退出码
     */
    virtual int runQApp(void *mfcApp = 0) = 0;

    /**
     * @brief 获取窗口实例的句柄
     * @param [in] window : 目标窗口实例（CWnd类型、QWidget类型）
     * @param [in] winType : 目标窗口类型。1）"QWidget"；2）"CWnd"
     * @return 目标窗口窗口句柄（HWND）
     */
    virtual long windowHandle(void *window, const std::string &winType) = 0;

    /**
     * @brief 获取软件系统语言
     * @return 软件系统语言
     */
    virtual std::string language() const = 0;

    /**
     * @brief 解析路径
     * @param path : 给定路径
     * @return 解析后的路径
     */
    virtual std::string parsePath(const std::string &path) const = 0;
};

/** 获取框架门面单实例导出函数类型定义 */
typedef IJUnknown *(J_ATTR_CDECL *FuncFrameFacadeInst)(void);

///

#ifdef JFRAME_FACADE_LIB
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__MINGW32__)
#   ifdef JFRAME_FACADE_BUILD
#       define JFRAME_FACADE_EXPORT __declspec(dllexport)
#   else
#       define JFRAME_FACADE_EXPORT __declspec(dllimport)
#       if defined(DEBUG) || defined(_DEBUG)
#           pragma comment(lib, "jframe_facaded.lib")
#       else
#           pragma comment(lib, "jframe_facade.lib")
#       endif
#   endif // !JFRAME_FACADE_BUILD
#endif // _MSC_VER || ...
#endif // JFRAME_FACADE_LIB

#ifndef JFRAME_FACADE_EXPORT
#define JFRAME_FACADE_EXPORT
#endif

/**
 * @brief jframeFacade : 获取框架门面系统单实例
 * @return 框架门面系统单实例
 */
JFRAME_FACADE_EXPORT IJFrameFacade* jframeFacade();

// 框架 - 日志

#ifdef JFRAME_FACADE_LIB

//
#ifndef __FILENAME__
#ifdef _MSC_VER
#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)
#endif
#endif

/** 输出日志（type: char*; msg, char*; where, char*） */
#define jframeLog(type, msg, where) \
    jframeFacade()->invokeMethod("log", 6, type, msg, where, __FILENAME__, __LINE__, __FUNCTION__)

/** log - emerge - (msg: char*) */
#define jframeLogEmerge(msg) \
    jframeLog("emerge", msg, "all")

/** log - fatal - (msg: char*) */
#define jframeLogFatal(msg) \
    jframeLog("fatal", msg, "all")

/** log - alert - (msg: char*) */
#define jframeLogAlert(msg) \
    jframeLog("alert", msg, "all")

/** log - crit - (msg: char*) */
#define jframeLogCrit(msg) \
    jframeLog("crit", msg, "all")

/** log - error - (msg: char*) */
#define jframeLogError(msg) \
    jframeLog("error", msg, "all")

/** log - warn - (msg: char*) */
#define jframeLogWarning(msg) \
    jframeLog("warn", msg, "all")

/** log - notice - (msg: char*) */
#define jframeLogNotice(msg) \
    jframeLog("notice", msg, "all")

/** log - info - (msg: char*) */
#define jframeLogInfo(msg) \
    jframeLog("info", msg, "all")

/** log - debug - (msg: char*) */
#define jframeLogDebug(msg) \
    jframeLog("debug", msg, "all")

#endif

/////////////////////////////////////////////////////////////

#ifdef JLIBRARY_MODULE

#ifdef _MSC_VER
#include <Windows.h>
#elif defined(__unix__)
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#endif

/**
 * @brief 链接库加载管理类
 */
template<typename T> class JLibraryT
{
public:
    /**
     * @brief JLibraryT : 构造函数
     * @param filepath : 库文件路径
     * @param autoclose : 自动销毁。true，使能；false，禁止
     */
    explicit JLibraryT(const std::basic_string<T> &filepath, bool autoclose = true)
        : _handle(0)
        , _filepath(filepath)
        , _autoclose(autoclose)
    {
        if (!_autoclose) {
            load();
        }
    }

    ~JLibraryT()
    {
        unload();
    }

    /**
     * @brief load : 加载库
     * @return 加载状态。true，加载成功；false，加载失败
     */
    bool load()
    {
        if (_handle) {
            return true;
        }
#ifdef _MSC_VER
        _handle = (void *)LoadLibraryA(_filepath.c_str());
#elif defined(__unix__)
        _handle = dlopen(_filepath.c_str(), RTLD_LAZY);
#else
#pragma message("not supported!")
#endif
        if (_handle) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief load : 加载库
     * @param filepath : 库文件路径
     * @return 加载状态。true，加载成功；false，加载失败
     */
    bool load(const std::basic_string<T> &filepath)
    {
        unload();
        _filepath = filepath;
        return load();
    }

    /**
     * @brief unload : 卸载库
     */
    void unload()
    {
        if (_handle && _autoclose) {
#ifdef _MSC_VER
            CloseHandle((HANDLE)_handle);
#elif defined(__unix__)
            dlclose(_handle);
#else
#pragma message("not supported!")
#endif
            _handle = 0;
        }
    }

    /**
     * @brief resolve : 获取库中的方法接口
     * @param methodname : 方法接口名称
     * @return 方法接口
     */
    void *resolve(const std::basic_string<T> &methodname)
    {
        if (!_handle) {
            return 0;   // failure
        }

        // 获取导出接口
#ifdef _MSC_VER
        return GetProcAddress((HMODULE)_handle, methodname.c_str());
#elif defined(__unix__)
        return dlsym(_handle, methodname.c_str());
#else
#pragma message("not supported!")
        return 0;
#endif
    }

    /**
     * @brief resolve : 获取库中的方法接口
     * @param filepath : 库文件路径
     * @param methodname : 方法接口名称
     * @return 方法接口
     */
    static void *resolve(const std::basic_string<T> &filepath, const std::basic_string<T> &methodname)
    {
        JLibraryT<T> library(filepath, false);
        return library.resolve(methodname);
    }

private:
    void *_handle;
    std::basic_string<T> _filepath;
    bool _autoclose;
};

/**
 * @brief 类型定义
 */
typedef JLibraryT<char> JLibrary, JLibraryA;

#endif

#endif // JFRAME_FACADE_H
