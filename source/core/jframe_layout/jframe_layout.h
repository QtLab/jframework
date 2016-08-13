#ifndef JFRAME_LAYOUT_H
#define JFRAME_LAYOUT_H

#include "jframe_core.h"
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

///

#ifdef JFRAME_LAYOUT_DLL
#ifdef _MSC_VER
#   ifdef JFRAME_LAYOUT_MAKEDLL
#       define JFRAME_LAYOUT_EXPORT __declspec(dllexport)
#   else
#       define JFRAME_LAYOUT_EXPORT __declspec(dllimport)

#       ifdef _MSC_VER
#           pragma comment(lib, "jframe_layoutd.lib")
#       else
#           pragma comment(lib, "jframe_layout.lib")
#       endif // !_MSC_VER

#   endif // !JFRAME_LAYOUT_MAKEDLL
#else
#define JFRAME_LAYOUT_EXPORT
#endif // _MSC_VER

//
JFRAME_LAYOUT_EXPORT IJFrameLayout* jframeLayout();

#endif // JFRAME_LAYOUT_DLL

#ifndef JFRAME_LAYOUT_EXPORT
#define JFRAME_LAYOUT_EXPORT
#endif

#endif JFRAME_LAYOUT_H
