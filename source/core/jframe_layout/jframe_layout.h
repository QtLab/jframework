#ifndef JFRAME_LAYOUT_H
#define JFRAME_LAYOUT_H

#include <string>
#include <list>
#include "factory/jframe_interface.h"

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

// interface IJFrameLayout

class INotifier;
class IGF_Attempter;
class QWidget;
class IGF_Component;

class IJFrameLayout : public IJObject
{
public:
    enum LogType {
        LogConsole,
        LogFile
    };

    virtual ~IJFrameLayout() {}

    // 获取GF框架主窗口实例
    virtual QWidget *mainWindow() = 0;

    // 获取JF框架消息分发器实例
    virtual INotifier *notifier() = 0;

    // 获取GF框架调度器实例
    virtual IGF_Attempter *gAttempter() = 0;

    // 设置窗体风格
    virtual void setFrameTheme(const char *theme) = 0;

    // 挂载组件
    virtual bool attachComponent(IGF_Component *component, bool stayOn = false) = 0;

    // 分离组件
    virtual bool detachComponent(IGF_Component *component) = 0;

    // 挂载组件UI
    virtual bool attachComponentUi(IGF_Component *component, QWidget *widget) = 0;

    // 获取所有挂载的组件
    virtual std::list<IGF_Component *> attachedComponent() const = 0;

    // 获取组件的权限等级（说明：查看jframe_login.h中的[PowerLevel]枚举定义）
    virtual int componentPowerLevel(const char *componentId) const = 0;

    // 获取当前系统
    virtual std::string currentSystem() const = 0;

    // 获取当前模式
    virtual std::string currentModule() const = 0;
};

// 接口标识
#define VER_IJFrameLayout J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameLayout J_IID_INTERFACE(IJFrameLayout)

///

#ifdef JFRAME_LAYOUT_DLL
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

//
JFRAME_LAYOUT_EXPORT IJFrameLayout* jframeLayout();

#endif // JFRAME_LAYOUT_DLL

#ifndef JFRAME_LAYOUT_EXPORT
#define JFRAME_LAYOUT_EXPORT
#endif

#endif JFRAME_LAYOUT_H
