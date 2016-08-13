#ifndef JFRAME_CORE_H
#define JFRAME_CORE_H

#include <string>

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
    virtual void releaseInterface() = 0;

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
    virtual std::string objectIdentity() const = 0;

    // 获取对象版本
    virtual unsigned int objectVersion() const = 0;

    // 执行函数调用
    virtual bool invoke(const char *method, int argc = 0, ...)
    { (void)method; (void)argc; return false; }
};

// 接口标识
#define VER_IJCommandSink J_INTERFACE_VERSION(1, 0)
#define IID_IJCommandSink J_IID_INTERFACE(IJCommandSink)

/**
 * @brief The IJCommandSink class
 */
class IJCommandSink
{
public:
    /**
     * @brief ~IJCommandSink
     */
    virtual ~IJCommandSink() {}

    /**
     * @brief commandSink : 执行UI命令
     * @param sender : 命令发起者
     * @param senderName : 发起者名称
     * @return
     */
    virtual bool commandSink(void *sender, const char* senderName) = 0;
};

// 接口标识
#define VER_IJMessageSink J_INTERFACE_VERSION(1, 0)
#define IID_IJMessageSink J_IID_INTERFACE(IJMessageSink)

/**
 * @brief The IJMessageSink class
 */
class IJMessageSink
{
public:
    /**
     * @brief ~IJMessageSink
     */
    virtual ~IJMessageSink() {}

    /**
     * @brief messageSink
     * @param sender
     * @param id
     * @param wParam
     * @param lParam
     * @return
     */
    virtual bool messageSink(void *sender, unsigned int id, JWPARAM wParam, JLPARAM lParam) = 0;
};

// 接口标识
#define VER_IJComponent J_INTERFACE_VERSION(1, 0)
#define IID_IJComponent J_IID_INTERFACE(IJComponent)

/**
 * @brief The IJComponent class
 */
class IJComponent : public IJUnknown
{
public:
    /**
     * @brief ~IJComponent
     */
    virtual ~IJComponent() {}

    /**
     * @brief componentId
     * @return
     */
    virtual std::string componentId() const = 0;

    /**
     * @brief componentDesc
     * @return
     */
    virtual std::string componentDesc() const = 0;

    /**
     * @brief initialize
     * @return
     */
    virtual bool initialize() = 0;

    /**
     * @brief shutdown
     */
    virtual void shutdown() = 0;

    /**
     * @brief attach
     */
    virtual void attach() {}

    /**
     * @brief detach
     */
    virtual void detach() {}
};

// 接口标识
#define VER_IJComponentUi J_INTERFACE_VERSION(1, 0)
#define IID_IJComponentUi J_IID_INTERFACE(IJComponentUi)

/**
 * @brief The IJComponentUi class
 */
class IJComponentUi
{
public:
    /**
     * @brief ~IJComponentUi
     */
    virtual ~IJComponentUi() {}

    /**
     * @brief createUi
     * @param parent
     * @param windowName
     * @return
     */
    virtual void *createUi(void *parent, const char *windowName) = 0;
};

// 接口标识
#define VER_IJMainWindow J_INTERFACE_VERSION(1, 0)
#define IID_IJMainWindow J_IID_INTERFACE(IJMainWindow)

/**
 * @brief The IJMainWindow class
 */
class IJMainWindow
{
public:
    /**
     * @brief ~IJMainWindow
     */
    virtual ~IJMainWindow() {}

    /**
     * @brief initialize
     * @return
     */
    virtual bool initialize() = 0;

    /**
     * @brief showNormal
     */
    virtual void showNormal() = 0;

    /**
     * @brief showMaximized
     */
    virtual void showMinimized() = 0;

    /**
     * @brief showMaximized
     */
    virtual void showMaximized() = 0;

    /**
     * @brief showFullScreen
     */
    virtual void showFullScreen() = 0;

    /**
     * @brief closeWindow
     */
    virtual void closeWindow() = 0;

    /**
     * @brief setVisible
     * @param visible
     */
    virtual void setVisible(bool visible) = 0;

    /**
     * @brief showTopLevel
     * @param stayOnTop
     */
    virtual void showStaysOnTop(bool stayOnTop) = 0;

    /**
     * @brief resize
     * @param width
     * @param height
     */
    virtual void resize(int width, int height) = 0;

    /**
     * @brief queryObject
     * @param objectName
     * @return
     */
    virtual void *queryObject(const char *objectName) = 0;

    /**
     * @brief statusBar
     * @return
     */
    virtual void *statusBar() = 0;

    /**
     * @brief activeView
     * @param viewName
     */
    virtual void activeView(const char *viewName) = 0;

    /**
     * @brief updateSplashInfo
     * @param info
     */
    virtual void updateSplashInfo(const char *info) = 0;

    /**
     * @brief createComponentUi
     * @param component
     * @param xmlName
     */
    virtual void createComponentUi(IJComponent *component, const char *xmlName) = 0;

    /**
     * @brief mainWindowHandle
     * @return
     */
    virtual void *mainWindowHandle() = 0;
};

// 接口标识
#define VER_IJAttempter J_INTERFACE_VERSION(1, 0)
#define IID_IJAttempter J_IID_INTERFACE(IJAttempter)

/**
 * @brief The IJAttempter class
 */
class IJAttempter
{
public:
    /**
     * @brief ~IJAttempter
     */
    virtual ~IJAttempter() {}

    /**
     * @brief loadInitComponent
     * @return
     */
    virtual bool loadInitComponent() = 0;

    /**
     * @brief loadAllComponent
     * @return
     */
    virtual bool loadAllComponent() = 0;

    /**
     * @brief shutdownAllComponent
     * @return
     */
    virtual bool shutdownAllComponent() = 0;

    /**
     * @brief queryComponent
     * @param componentId
     * @return
     */
    virtual IJComponent *queryComponent(const char *componentId) = 0;

    /**
     * @brief mainWindow
     * @return
     */
    virtual IJMainWindow *mainWindow() = 0;

    /**
     * @brief queryInterface
     * @param componentId
     * @param iid
     * @param ver
     * @return
     */
    virtual void *queryInterface(const char *componentId, const char *iid, unsigned int ver) = 0;

    /**
     * @brief allComponents
     * @return
     */
    virtual std::list<IJComponent *> allComponents() const = 0;

    /**
     * @brief currentWorkModeId
     * @return
     */
    virtual int currentWorkModeId() const = 0;

    /**
     * @brief currentWorkModeName
     * @return
     */
    virtual const char *currentWorkModeName() const = 0;

    /**
     * @brief currentWorkModeConfigDirName
     * @return
     */
    virtual const char *currentWorkModeConfigDirName() const = 0;
};

// 接口标识
#define VER_IJFrameCore J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameCore J_IID_INTERFACE(IJFrameCore)

/**
 * @brief The IJFrameCore class
 */
class IJFrameCore : public IJObject
{
public:
    /**
     * @brief ~IJFrameCore
     */
    virtual ~IJFrameCore() {}

    /**
     * @brief initialize
     * @return
     */
    virtual bool initialize() = 0;

    /**
     * @brief attempter
     * @return
     */
    virtual IJAttempter *attempter() = 0;
};

///

//
typedef bool (__cdecl *FuncInitComponent)(void);

//
typedef void *(__cdecl *FuncCreateAttempter)(void);

//
typedef void *(__cdecl *FuncCreateComponent)(IJAttempter *attempter);

///

#ifdef JFRAME_CORE_DLL
#ifdef _MSC_VER
#   ifdef JFRAME_CORE_MAKEDLL
#       define JFRAME_CORE_EXPORT __declspec(dllexport)
#   else
#       define JFRAME_CORE_EXPORT __declspec(dllimport)

#       ifdef _MSC_VER
#           pragma comment(lib, "jframe_cored.lib")
#       else
#           pragma comment(lib, "jframe_core.lib")
#       endif // !_MSC_VER

#   endif // !JFRAME_CORE_MAKEDLL
#else
#define JFRAME_CORE_EXPORT
#endif // _MSC_VER

//
JFRAME_CORE_EXPORT IJFrameCore* jframeCore();

#endif // JFRAME_CORE_DLL

#ifndef JFRAME_CORE_EXPORT
#define JFRAME_CORE_EXPORT
#endif

#endif // JFRAME_CORE_H
