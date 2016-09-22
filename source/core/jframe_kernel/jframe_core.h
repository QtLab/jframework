#ifndef JFRAME_CORE_H
#define JFRAME_CORE_H

#include "jframe_kernel.h"

/** 接口标识 */
#define VER_IJCommandSink J_INTERFACE_VERSION(1, 0)
#define IID_IJCommandSink J_IID_INTERFACE(IJCommandSink)

/**
 * @brief 命令接收接口
 */
class IJCommandSink
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~IJCommandSink() {}

    /**
     * @brief 接收命令消息
     * @param [in] sender : 命令发起者（type: QObject *）
     * @param domain : 命令发起者所属域名 [componentName#...]
     * @param objectName : 发起者对象名称
     * @param eventType : 信号、事件类型
     * @param [in,out] data : 数据、状态（看使用手册说明）
     * @return 截断标志，true，停止下传；false，继续下传
     */
    virtual bool commandSink(void *sender, const std::string &domain,
                             const std::string &objectName,
                             const std::string &eventType,
                             void *data) = 0;
};

/** 接口标识 */
#define VER_IJMessageSink J_INTERFACE_VERSION(1, 0)
#define IID_IJMessageSink J_IID_INTERFACE(IJMessageSink)

class IJComponent;

/**
 * @brief 消息接收接口
 */
class IJMessageSink
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~IJMessageSink() {}

    /**
     * @brief 接收消息
     * @param [in] sender : 消息发起者
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return 截断标志。true，停止下传；false，继续下传
     */
    virtual bool messageSink(IJComponent *sender, const std::string &id, JWPARAM wParam, JLPARAM lParam) = 0;
};

/** 接口标识 */
#define VER_IJComponent J_INTERFACE_VERSION(1, 0)
#define IID_IJComponent J_IID_INTERFACE(IJComponent)

/**
 * @brief 组件接口
 */
class IJComponent : public IJUnknown
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~IJComponent() {}

    /**
     * @brief 获取组件名称
     * @return 组件名称
     */
    virtual std::string componentName() const = 0;

    /**
     * @brief 获取组件描述
     * @return 组件描述
     */
    virtual std::string componentDesc() const = 0;

    /**
     * @brief 获取组件类型
     * @return 组件类型
     */
    virtual std::string componentType() const { return "<unknown>"; }

    /**
     * @brief 挂载组件
     */
    virtual void attach() {}

    /**
     * @brief 分离组件
     */
    virtual void detach() {}
};

/** 接口标识 */
#define VER_IJComponentUi J_INTERFACE_VERSION(1, 0)
#define IID_IJComponentUi J_IID_INTERFACE(IJComponentUi)

/**
 * @brief 组件窗口接口
 */
class IJComponentUi
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~IJComponentUi() {}

    /**
     * @brief 创建窗口
     * @param [in] parent : 父窗口
     * @param objectName : 目标窗口名称
     * @return 目标窗口
     */
    virtual void *createWindow(void *parent, const std::string &objectName) = 0;
};

/** 接口标识 */
#define VER_IJMainWindow J_INTERFACE_VERSION(1, 0)
#define IID_IJMainWindow J_IID_INTERFACE(IJMainWindow)

/**
 * @brief 框架主窗口接口
 */
class IJMainWindow : public IJUnknown
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~IJMainWindow() {}

    /**
     * @brief 获取接口标识
     * @return 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJMainWindow; }

    /**
     * @brief 获取接口版本
     * @return 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJMainWindow; }

    /**
     * @brief 常规化主窗口
     */
    virtual void showNormal() = 0;

    /**
     * @brief 最小化主窗口
     */
    virtual void showMinimized() = 0;

    /**
     * @brief 最大化主窗口
     */
    virtual void showMaximized() = 0;

    /**
     * @brief 全屏话主窗口
     */
    virtual void showFullScreen() = 0;

    /**
     * @brief 关闭主窗口
     */
    virtual void closeWindow() = 0;

    /**
     * @brief 设置主窗口可见性
     * @param visible : 可见性标志。true，显示主窗口；false，隐藏主窗口
     */
    virtual void setVisible(bool visible) = 0;

    /**
     * @brief 置顶主窗口
     * @param stayOnTop : 置顶标志。true，置顶；false，取消置顶
     */
    virtual void showStaysOnTop(bool stayOnTop) = 0;

    /**
     * @brief 改变主窗口大小
     * @param width : 宽度
     * @param height : 高度
     */
    virtual void resize(int width, int height) = 0;

    /**
     * @brief 激活视图
     * @param viewName : 视图名称
     */
    virtual void activeView(const std::string &viewName) = 0;

    /**
     * @brief 更新开始界面信息
     * @param info : 信息
     */
    virtual void updateSplashInfo(const std::string &info) = 0;

    /**
     * @brief 创建组件窗口
     * @param [in] component : 组件
     * @param filePath : 组件配置文件路径
     * @return 创建状态。true，创建成功；false，创建失败
     */
    virtual bool createComponentUi(IJComponent *component, const std::string &filePath) = 0;

    /**
     * @brief 获取主窗口
     * @return 主窗口
     */
    virtual void *mainWidget() = 0;

    /**
     * @brief 设置框架主题
     * @param theme : 主题名称
     */
    virtual void setTheme(const std::string &theme) = 0;

    /**
     * @brief 获取工具栏类型
     * @return 工具栏类型。1) "menuBar"; 2) "toolButton"; 3) "ribbonBar"
     */
    virtual std::string toolBarType() const = 0;

    /**
     * @brief 获取视图布局类型
     * @return 视图布局类型。1) dynamic; 2) static
     */
    virtual std::string layoutType() const = 0;

    /**
     * @brief 查询指定组件中的配置生成的对象
     * @param objectName : 对象名称
     * @param componentName : 组件名称 （组件名称为空值时，查找所组件中第一个对象名称为objectName的对象）
     * @return 对象实例
     */
    virtual void *queryObject(const std::string &objectName, const std::string &componentName = "") = 0;

    /**
     * @brief 获取菜单栏
     * @return 菜单栏
     */
    virtual void *menuBar() = 0;

    /**
     * @brief 获取 ribbon 工具条
     * @return ribbon 工具条
     */
    virtual void *ribbonBar() = 0;

    /**
     * @brief 获取状态栏
     * @return 状态栏
     */
    virtual void *statusBar() = 0;
};

/** 接口标识 */
#define VER_IJAttempter J_INTERFACE_VERSION(1, 0)
#define IID_IJAttempter J_IID_INTERFACE(IJAttempter)

//
#ifndef J_MESSAGESINK_CALLBACK
#define J_MESSAGESINK_CALLBACK
/** 消息接收接口函数定义 */
typedef JLRESULT (IJComponent::*JMsgSinkCb)
(IJComponent *sender, const std::string &id, JWPARAM wParam, JLPARAM lParam);
#endif // !J_MESSAGESINK_CALLBACK

class INotifier;

/**
 * @brief 框架调度器接口
 */
class IJAttempter : public IJUnknown
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~IJAttempter() {}

    /**
     * @brief 获取接口标识
     * @return 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJAttempter; }

    /**
     * @brief 获取接口版本
     * @return 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJAttempter; }

    /**
     * @brief 加载组件
     * @return 加载状态。true，加载成功；false，加载失败
     */
    virtual bool loadComponent() = 0;

    /**
     * @brief 卸载组件
     */
    virtual void releaseComponent() = 0;

    /**
     * @brief 查询组件
     * @param componentName : 组件名称
     * @return 组件
     */
    virtual IJComponent *queryComponent(const std::string &componentName) = 0;

    /**
     * @brief 获取框架主窗口接口
     * @return 框架主窗口接口
     */
    virtual IJMainWindow *mainWindow() = 0;

    /**
     * @brief 查询接口
     * @param componentName : 组件名称
     * @param iid : 接口标识
     * @param ver : 接口版本
     * @return 查询接口实例
     */
    virtual void *queryInterface(const std::string &componentName, const std::string &iid, unsigned int ver) = 0;

    /**
     * @brief 获取所有组件
     * @return 所有组件
     */
    virtual std::list<IJComponent *> allComponents() const = 0;

    /**
     * @brief 获取当前工作模式标识
     * @return 当前工作模式标识
     */
    virtual int currentWorkModeId() const = 0;

    /**
     * @brief 获取当前工作模式名称
     * @return 当前工作模式名称
     */
    virtual std::string currentWorkModeName() const = 0;

    /**
     * @brief 获取当前工作模式配置文件夹名称
     * @return 当前工作模式配置文件夹名称
     */
    virtual std::string currentWorkModeConfigDirName() const = 0;

    /**
     * @brief 获取消息分发器
     * @return 消息分发器
     */
    virtual INotifier &notifier() = 0;

    /**
     * @brief 开始消息订阅组
     * @param [in] component : 组件实例
     * @return 调度器引用
     */
    template<typename T> IJAttempter &beginGroup(T *component);

    /**
     * @brief 结束消息订阅组
     */
    virtual void endGroup() = 0;

    /**
     * @brief 订阅组件消息（可选：直接绑定id对应的响应函数）
     * @param id : 消息标识
     * @param [in] cb : 消息标识对应的响应函数
     * @return 调度器引用
     */
    template<typename T>
    IJAttempter &subMessage(const std::string &id, JLRESULT (T::*cb)
                            (IJComponent *, const std::string &, JWPARAM, JLPARAM) = 0);

    /**
     * @brief 取消订阅组件消息
     * @param id : 消息标识
     * @return 调度器引用
     */
    virtual IJAttempter &unsubMessage(const std::string &id) = 0;

    /**
     * @brief 取消订阅消息
     * @param [in] component : 组件实例
     */
    virtual void unsubMessage(IJComponent *component) = 0;

    /**
     * @brief 发送组件消息（同步）
     * @param [in] component : 组件
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     * @return 执行结果
     */
    virtual JLRESULT sendMessage(IJComponent *component, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief 发送组件消息（异步）
     * @param [in] component : 组件
     * @param id : 消息标识
     * @param wParam : 参数1
     * @param lParam : 参数2
     */
    virtual void postMessage(IJComponent *component, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief 发送组件消息（异步）
     * @param [in] component : 组件
     * @param id : 消息标识
     * @param msg : 消息信息
     */
    virtual void postMessage(IJComponent *component, const std::string &id, const std::string &msg) = 0;

protected:
    /**
     * @brief 开始消息订阅组
     * @param [in] component : 组件实例
     * @param offset : reinterpret_cast<IJComponent *>(this) - static_cast<IJComponent *>(this)
     * @return 调度器引用
     */
    virtual IJAttempter &beginGroup(IJComponent *component, int offset) = 0;

    /**
     * @brief 订阅消息
     * @param id : 消息标识
     * @param [in] cb : 消息响应函数地址
     * @return 调度器引用
     */
    virtual IJAttempter &subMessage(const std::string &id, JMsgSinkCb cb) = 0;
};

template<typename T> inline
IJAttempter &IJAttempter::beginGroup(T *component)
{
    return beginGroup(component,
                  #ifdef _MSC_VER
                      (reinterpret_cast<IJComponent *>(component) - static_cast<IJComponent *>(component))
                  #elif defined(__unix__)
                      0
                  #else
                  #pragma message("not supported!"))
                      0
                  #endif
                      );
}

template<typename T> inline
IJAttempter &IJAttempter::subMessage(const std::string &id, JLRESULT (T::*cb)
                                     (IJComponent *, const std::string &, JWPARAM, JLPARAM))
{
    return subMessage(id, static_cast<JMsgSinkCb>(cb));
}

/** 接口标识 */
#define VER_IJFrameCore J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameCore J_IID_INTERFACE(IJFrameCore)

/**
 * @brief 框架核心系统接口
 */
class IJFrameCore : public IJUnknown
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~IJFrameCore() {}

    /**
     * @brief 获取接口标识
     * @return 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameCore; }

    /**
     * @brief 获取接口版本
     * @return 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameCore; }

    /**
     * @brief 获取框架调度器
     * @return 框架调度器
     */
    virtual IJAttempter *attempter() = 0;
};

/** 获取初始化组件导出函数接口定义 */
typedef bool (J_ATTR_CDECL *FuncInitComponent)(void);

/** 创建调度器实例导出函数接口定义 */
typedef void *(J_ATTR_CDECL *FuncCreateAttempter)(void);

/** 创建组件实例导出函数接口定义 */
typedef void *(J_ATTR_CDECL *FuncCreateComponent)(IJAttempter *attempter);

#endif // JFRAME_CORE_H
