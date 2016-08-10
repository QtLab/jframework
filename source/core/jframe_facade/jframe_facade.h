#ifndef JFRAME_FACADE_H
#define JFRAME_FACADE_H

#include "factory/jframe_interface.h"
#include <list>

// IGF_Component接口标识
#define VER_IGF_Component J_INTERFACE_VERSION(1, 0)
#define IID_IGF_Component J_IID_INTERFACE(IGF_Component)

// IGF_MainWindow接口标识
#define VER_IGF_MainWindow J_INTERFACE_VERSION(1, 0)
#define IID_IGF_MainWindow J_IID_INTERFACE(IGF_MainWindow)

// IGF_Attempter接口标识
#define VER_IGF_Attempter J_INTERFACE_VERSION(1, 0)
#define IID_IGF_Attempter J_IID_INTERFACE(IGF_Attempter)

// interface IJFrameFacade

class IJFrameFacade : public IJObject
{
public:
    virtual ~IJFrameFacade() {}

    // 获取框架路径
    virtual std::string frameDirPath() const = 0;

    // 获取框架进程路径
    virtual std::string appDirPath() const = 0;

    // 获取框架相关配置文件路径
    virtual std::string frameGlobalPath() const = 0;
    virtual std::string frameLayoutPath() const = 0;
    virtual std::string frameComponentPath() const = 0;

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
    virtual void restartFrame(const std::string<std::string> &arguments) = 0;

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
};

// 接口标识
#define VER_IJFrameFacade J_INTERFACE_VERSION(1, 0)
#defien IID_IJFrameFacade J_IID_INTERFACE(IJFrameFacade)

///

#ifdef JFRAME_FACADE_DLL
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
