#ifndef JFRAME_LAYOUT_H
#define JFRAME_LAYOUT_H

#include "jframe_kernel.h"
#include <list>

// 接口标识
#define VER_IJFrameLayout J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameLayout J_IID_INTERFACE(IJFrameLayout)

class INotifier;
class IJComponent;
class IJAttempter;
class QWidget;

/**
 * @brief The IJFrameLayout class
 */
class IJFrameLayout : public IJObject
{
public:
    enum LogType {
        LogConsole,
        LogFile
    };

    virtual ~IJFrameLayout() {}

    /**
     * @brief objectIdentity : 获取对象标识
     * @return
     */
    virtual std::string objectIdentity() const { return IID_IJFrameLayout; }

    /**
     * @brief objectVersion : 获取对象版本
     * @return
     */
    virtual unsigned int objectVersion() const { return VER_IJFrameLayout; }

    // 获取框架主窗口实例
    virtual QWidget *mainWindow() = 0;

    // 获取框架消息分发器实例
    virtual INotifier *notifier() = 0;

    // 获取框架调度器实例
    virtual IJAttempter *attempter() = 0;

    // 设置窗体风格
    virtual void setFrameTheme(const char *theme) = 0;

    // 挂载组件
    virtual bool attachComponent(IJComponent *component, bool stayOn = false) = 0;

    // 分离组件
    virtual bool detachComponent(IJComponent *component) = 0;

    // 挂载组件UI
    virtual bool attachComponentUi(IJComponent *component, QWidget *widget) = 0;

    // 获取所有挂载的组件
    virtual std::list<IJComponent *> attachedComponent() const = 0;

    // 获取组件的权限等级（说明：查看jframe_login.h中的[PowerLevel]枚举定义）
    virtual int componentPowerLevel(const std::string &componentId) const = 0;

    // 获取当前系统
    virtual std::string currentSystem() const = 0;

    // 获取当前模式
    virtual std::string currentModule() const = 0;
};

#endif JFRAME_LAYOUT_H
