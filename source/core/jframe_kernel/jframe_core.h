#ifndef JFRAME_CORE_H
#define JFRAME_CORE_H

#include "jframe_kernel.h"

// 接口标识
#define VER_IJCommandSink J_INTERFACE_VERSION(1, 0)
#define IID_IJCommandSink J_IID_INTERFACE(IJCommandSink)

/**
 * @brief The IJCommandSink class : 命令接收接口
 */
class IJCommandSink
{
public:
    /**
     * @brief ~IJCommandSink : 析构函数
     */
    virtual ~IJCommandSink() {}

    /**
     * @brief commandSink : 接收命令消息
     * @param sender : 命令发起者
     * @param senderName : 发起者名称
     * @return : 截断标志，true，停止下传；false，继续下传
     */
    virtual bool commandSink(void *sender, const std::string &senderName) = 0;
};

// 接口标识
#define VER_IJMessageSink J_INTERFACE_VERSION(1, 0)
#define IID_IJMessageSink J_IID_INTERFACE(IJMessageSink)

/**
 * @brief The IJMessageSink class :消息接收接口
 */
class IJMessageSink
{
public:
    /**
     * @brief ~IJMessageSink : 析构函数
     */
    virtual ~IJMessageSink() {}

    /**
     * @brief messageSink : 接收消息
     * @param sender : 消息发起者
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return : 截断标志。true，停止下传；false，继续下传
     */
    virtual bool messageSink(void *sender, unsigned int id, JWPARAM wParam, JLPARAM lParam) = 0;
};

// 接口标识
#define VER_IJComponent J_INTERFACE_VERSION(1, 0)
#define IID_IJComponent J_IID_INTERFACE(IJComponent)

/**
 * @brief The IJComponent class : 组件接口
 */
class IJComponent : public IJUnknown
{
public:
    /**
     * @brief ~IJComponent : 析构函数
     */
    virtual ~IJComponent() {}

    /**
     * @brief componentName : 获取组件名称
     * @return : 组件名称
     */
    virtual std::string componentName() const = 0;

    /**
     * @brief componentDesc : 获取组件描述
     * @return : 组件描述
     */
    virtual std::string componentDesc() const = 0;

    /**
     * @brief componentType : 获取组件类型
     * @return : 组件类型
     */
    virtual std::string componentType() const { return "<unknown>"; }

    /**
     * @brief attach : 挂载组件
     */
    virtual void attach() {}

    /**
     * @brief detach : 分离组件
     */
    virtual void detach() {}
};

// 接口标识
#define VER_IJComponentUi J_INTERFACE_VERSION(1, 0)
#define IID_IJComponentUi J_IID_INTERFACE(IJComponentUi)

/**
 * @brief The IJComponentUi class : 组件窗口接口
 */
class IJComponentUi
{
public:
    /**
     * @brief ~IJComponentUi : 析构函数
     */
    virtual ~IJComponentUi() {}

    /**
     * @brief createWindow : 创建窗口
     * @param parent : 父窗口
     * @param objectName : 目标窗口名称
     * @return : 目标窗口
     */
    virtual void *createWindow(void *parent, const std::string &objectName) = 0;
};

// 接口标识
#define VER_IJMainWindow J_INTERFACE_VERSION(1, 0)
#define IID_IJMainWindow J_IID_INTERFACE(IJMainWindow)

/**
 * @brief The IJMainWindow class : 框架主窗口接口
 */
class IJMainWindow : public IJUnknown
{
public:
    /**
     * @brief ~IJMainWindow : 析构函数
     */
    virtual ~IJMainWindow() {}

    /**
     * @brief interfaceIdentity : 获取接口标识
     * @return : 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJMainWindow; }

    /**
     * @brief interfaceVersion : 获取接口版本
     * @return : 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJMainWindow; }

    /**
     * @brief showNormal : 常规化主窗口
     */
    virtual void showNormal() = 0;

    /**
     * @brief showMaximized : 最小化主窗口
     */
    virtual void showMinimized() = 0;

    /**
     * @brief showMaximized : 最大化主窗口
     */
    virtual void showMaximized() = 0;

    /**
     * @brief showFullScreen : 全屏话主窗口
     */
    virtual void showFullScreen() = 0;

    /**
     * @brief closeWindow : 关闭主窗口
     */
    virtual void closeWindow() = 0;

    /**
     * @brief setVisible : 设置主窗口可见性
     * @param visible : 可见性标志。true，显示主窗口；false，隐藏主窗口
     */
    virtual void setVisible(bool visible) = 0;

    /**
     * @brief showTopLevel : 置顶主窗口
     * @param stayOnTop : 置顶标志。true，置顶；false，取消置顶
     */
    virtual void showStaysOnTop(bool stayOnTop) = 0;

    /**
     * @brief resize : 改变主窗口大小
     * @param width : 宽度
     * @param height : 高度
     */
    virtual void resize(int width, int height) = 0;

    /**
     * @brief queryObject : 查询组件窗口
     * @param objectName : 窗口名称
     * @return : 组件窗口
     */
    virtual void *queryObject(const std::string &objectName) = 0;

    /**
     * @brief statusBar : 获取状态栏
     * @return : 状态栏
     */
    virtual void *statusBar() = 0;

    /**
     * @brief activeView : 激活视图
     * @param viewName : 视图名称
     */
    virtual void activeView(const std::string &viewName) = 0;

    /**
     * @brief updateSplashInfo : 更新开始界面信息
     * @param info : 信息
     */
    virtual void updateSplashInfo(const std::string &info) = 0;

    /**
     * @brief createComponentUi : 创建组件窗口
     * @param component : 组价
     * @param filePath : 组件配置文件路径
     * @return : 创建状态。true，创建成功；false，创建失败
     */
    virtual bool createComponentUi(IJComponent *component, const std::string &filePath) = 0;

    /**
     * @brief mainWidget : 获取主窗口
     * @return : 主窗口
     */
    virtual void *mainWidget() = 0;

    /**
     * @brief setTheme : 设置框架主题
     * @param theme : 主题名称
     */
    virtual void setTheme(const std::string &theme) = 0;

    /**
     * @brief toolBarType : 获取工具栏类型
     * @return : 工具栏类型。1) ribbon; 2) menu
     */
    virtual std::string toolBarType() const = 0;

    /**
     * @brief layoutType : 获取视图布局类型
     * @return : 视图布局类型。1) dynamic; 2) static
     */
    virtual std::string layoutType() const = 0;
};

// 接口标识
#define VER_IJAttempter J_INTERFACE_VERSION(1, 0)
#define IID_IJAttempter J_IID_INTERFACE(IJAttempter)

class INotifier;

/**
 * @brief The IJAttempter class : 框架调度器接口
 */
class IJAttempter : public IJUnknown
{
public:
    /**
     * @brief ~IJAttempter : 析构函数
     */
    virtual ~IJAttempter() {}

    /**
     * @brief interfaceIdentity : 获取接口标识
     * @return : 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJAttempter; }

    /**
     * @brief interfaceVersion : 获取接口版本
     * @return : 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJAttempter; }

    /**
     * @brief loadComponent : 加载组件
     * @return : 加载状态。true，加载成功；false，加载失败
     */
    virtual bool loadComponent() = 0;

    /**
     * @brief releaseComponent : 卸载组件
     */
    virtual void releaseComponent() = 0;

    /**
     * @brief queryComponent : 查询组件
     * @param componentName : 组件名称
     * @return : 组件
     */
    virtual IJComponent *queryComponent(const std::string &componentName) = 0;

    /**
     * @brief mainWindow : 获取框架主窗口接口
     * @return : 框架主窗口接口
     */
    virtual IJMainWindow *mainWindow() = 0;

    /**
     * @brief queryInterface : 查询接口
     * @param componentId : 组件名称
     * @param iid : 接口标识
     * @param ver : 接口版本
     * @return : 查询接口实例
     */
    virtual void *queryInterface(const std::string &componentName, const std::string &iid, unsigned int ver) = 0;

    /**
     * @brief allComponents : 获取所有组件
     * @return : 所有组件
     */
    virtual std::list<IJComponent *> allComponents() const = 0;

    /**
     * @brief currentWorkModeId : 获取当前工作模式标识
     * @return : 当前工作模式标识
     */
    virtual int currentWorkModeId() const = 0;

    /**
     * @brief currentWorkModeName : 获取当前工作模式名称
     * @return : 当前工作模式名称
     */
    virtual const char *currentWorkModeName() const = 0;

    /**
     * @brief currentWorkModeConfigDirName : 获取当前工作模式配置文件夹名称
     * @return : 当前工作模式配置文件夹名称
     */
    virtual const char *currentWorkModeConfigDirName() const = 0;

    /**
     * @brief subMessage : 订阅组件消息
     * @param component : 组件
     * @param id : 消息标识
     */
    virtual void subMessage(IJComponent *component, unsigned int id) = 0;

    /**
     * @brief unsubMessage : 取消订阅组件消息
     * @param component : 组件
     * @param id : 消息标识
     */
    virtual void unsubMessage(IJComponent *component, unsigned int id) = 0;

    /**
     * @brief pubMessage : 发送组件消息（同步）
     * @param component : 组件
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     */
    virtual void sendMessage(IJComponent *component, unsigned int id, JWPARAM wParam, JLPARAM lParam) = 0;

    /**
     * @brief pubMessage : 发送组件消息（异步）
     * @param component : 组件
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     */
    virtual void postMessage(IJComponent *component, unsigned int id, JWPARAM wParam, JLPARAM lParam) = 0;

    /**
     * @brief notifier : 获取消息分发器
     * @return : 消息分发器
     */
    virtual INotifier *notifier() = 0;
};

// 接口标识
#define VER_IJFrameCore J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameCore J_IID_INTERFACE(IJFrameCore)

/**
 * @brief The IJFrameCore class : 框架核心系统接口
 */
class IJFrameCore : public IJUnknown
{
public:
    /**
     * @brief ~IJFrameCore : 析构函数
     */
    virtual ~IJFrameCore() {}

    /**
     * @brief interfaceIdentity : 获取接口标识
     * @return : 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameCore; }

    /**
     * @brief interfaceVersion : 获取接口版本
     * @return : 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameCore; }

    /**
     * @brief attempter : 获取框架调度器
     * @return : 框架调度器
     */
    virtual IJAttempter *attempter() = 0;
};

//
typedef bool (J_ATTR_CDECL *FuncInitComponent)(void);

//
typedef void *(J_ATTR_CDECL *FuncCreateAttempter)(void);

//
typedef void *(J_ATTR_CDECL *FuncCreateComponent)(IJAttempter *attempter);

#endif // JFRAME_CORE_H
