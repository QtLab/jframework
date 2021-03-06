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
class IJFrameLayout : public IJUnknown
{
public:
    /**
     * @brief 日志类型枚举
     */
    enum LogType {
        LogConsole,     /**< 将日志输出到Output等终端 */
        LogFile         /**< 将日志输出到文件中 */
    };

    /**
     * @brief ~IJFrameLayout : 析构函数
     */
    virtual ~IJFrameLayout() {}

    /**
     * @brief 获取接口标识
     * @return 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameLayout; }

    /**
     * @brief 获取框架布局配置文件路径
     * @return 框架布局配置文件路径
     */
    virtual std::string layoutDirPath() const = 0;

    /**
     * @brief 获取接口版本
     * @return 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameLayout; }

    /**
     * @brief 获取框架主窗口
     * @return 框架主窗口
     */
    virtual QWidget *mainWindow() = 0;

    /**
     * @brief 获取框架主视图
     * @return 框架主视图
     */
    virtual QWidget *mainView() = 0;

    /**
     * @brief 获取框架消息分发器
     * @return 框架消息分发器
     */
    virtual INotifier &notifier() = 0;

    /**
     * @brief 获取框架调度器
     * @return 框架调度器
     */
    virtual IJAttempter *attempter() = 0;

    /**
     * @brief 设置框架风格主题
     * @param theme : 主题名称
     */
    virtual void setFrameTheme(const std::string &theme) = 0;

    /**
     * @brief 获取组件的权限等级
     * @param componentName : 组件名称
     * @return 组件权限等级。查看jframe_login.h中的[PowerLevel]枚举定义
     */
    virtual int componentPowerLevel(const std::string &componentName) const = 0;

    /**
     * @brief 获取当前系统名称
     * @return 当前系统名称
     */
    virtual std::string currentSystem() const = 0;

    /**
     * @brief 获取当前模式名称
     * @return 当前模式名称
     */
    virtual std::string currentModule() const = 0;
};

#endif // JFRAME_LAYOUT_H
