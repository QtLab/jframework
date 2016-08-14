#ifndef JFRAME_CORE_H
#define JFRAME_CORE_H

#include "jframe_kernel.h"

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
     * @param objectName
     * @return
     */
    virtual void *createUi(void *parent, const char *objectName) = 0;
};

// 接口标识
#define VER_IJMainWindow J_INTERFACE_VERSION(1, 0)
#define IID_IJMainWindow J_IID_INTERFACE(IJMainWindow)

/**
 * @brief The IJMainWindow class
 */
class IJMainWindow : public IJObject
{
public:
    /**
     * @brief ~IJMainWindow
     */
    virtual ~IJMainWindow() {}

    /**
     * @brief objectIdentity : 获取对象标识
     * @return
     */
    virtual std::string objectIdentity() const { return IID_IJMainWindow; }

    /**
     * @brief objectVersion : 获取对象版本
     * @return
     */
    virtual unsigned int objectVersion() const { return VER_IJMainWindow; }

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
     * @param filePath
     * @return
     */
    virtual bool createComponentUi(IJComponent *component, const char *filePath) = 0;

    /**
     * @brief mainWidget
     * @return
     */
    virtual void *mainWidget() = 0;

    /**
     * @brief setTheme
     * @param theme
     */
    virtual void setTheme(const char *theme) = 0;
};

// 接口标识
#define VER_IJAttempter J_INTERFACE_VERSION(1, 0)
#define IID_IJAttempter J_IID_INTERFACE(IJAttempter)

/**
 * @brief The IJAttempter class
 */
class IJAttempter : public IJObject
{
public:
    /**
     * @brief ~IJAttempter
     */
    virtual ~IJAttempter() {}

    /**
     * @brief objectIdentity : 获取对象标识
     * @return
     */
    virtual std::string objectIdentity() const { return IID_IJAttempter; }

    /**
     * @brief objectVersion : 获取对象版本
     * @return
     */
    virtual unsigned int objectVersion() const { return VER_IJAttempter; }

    /**
     * @brief loadComponent
     * @return
     */
    virtual bool loadComponent() = 0;

    /**
     * @brief shutdownComponent
     */
    virtual void shutdownComponent() = 0;

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
     * @brief objectIdentity : 获取对象标识
     * @return
     */
    virtual std::string objectIdentity() const { return IID_IJFrameCore; }

    /**
     * @brief objectVersion : 获取对象版本
     * @return
     */
    virtual unsigned int objectVersion() const { return VER_IJFrameCore; }

    /**
     * @brief attempter
     * @return
     */
    virtual IJAttempter *attempter() = 0;
};

//
typedef bool (__cdecl *FuncInitComponent)(void);

//
typedef void *(__cdecl *FuncCreateAttempter)(void);

//
typedef void *(__cdecl *FuncCreateComponent)(IJAttempter *attempter);

#endif // JFRAME_CORE_H
