#include "stdafx.h"
#include "[!output HEADER_FILE_NAME]"
[!if INCLUDE_IJCOMPONENTUI]
#include "[!output UI_HEADER_FILE_NAME]"
[!endif]

//
J_EXTERN_C J_ATTR_EXPORT void *CreateComponent(void* attemper)
{
[!if INCLUDE_CHECK_LOGIN_USER]
    // 非管理员用户才能使用此组件
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }
    
[!endif]
    return static_cast<IJComponent *>
            (new [!output COMPONENT_CLASS_NAME](reinterpret_cast<IJAttempter *>(attemper)));
}

[!output COMPONENT_CLASS_NAME]::[!output COMPONENT_CLASS_NAME](IJAttempter *attemper)
    : q_attempter(attemper)
[!if INCLUDE_IJCOMPONENTUI]
    , q_ui(0)
[!endif]
{

}

[!output COMPONENT_CLASS_NAME]::~[!output COMPONENT_CLASS_NAME]()
{
[!if INCLUDE_IJCOMPONENTUI]
    if (q_ui) {
        q_ui->deleteLater();
        q_ui = 0;
    }
[!endif]
}

bool [!output COMPONENT_CLASS_NAME]::loadInterface()
{
[!if !INCLUDE_DYNAMIC_LAYOUT && INCLUDE_JOBSERVER]
    // 订阅消息
    q_attempter->notifier().beginGroup(this)
            .endGroup();

[!endif]
    bool result = true;

    //TODO: Add your loading statements.

    return result;
}

bool [!output COMPONENT_CLASS_NAME]::updateInterface()
{
    bool result = true;

    //TODO: Add your updating statements.

    return result;
}

void [!output COMPONENT_CLASS_NAME]::releaseInterface()
{
    //TODO: Add your release statements.
[!if !INCLUDE_DYNAMIC_LAYOUT && INCLUDE_JOBSERVER]

    // 取消订阅消息
    q_attempter->notifier().remove(this);
[!endif]
}

void* [!output COMPONENT_CLASS_NAME]::queryInterface(const std::string &iid, unsigned int ver)
{
[!if INCLUDE_IJCOMPONENTUI]
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);
[!endif]
[!if INCLUDE_IJCOMMANDSINK]
    J_QUERY_INTERFACE(IJCommandSink, iid, ver);
[!endif]
[!if INCLUDE_IJMESSAGESINK]
    J_QUERY_INTERFACE(IJMessageSink, iid, ver);
[!endif]
[!if !INCLUDE_IJCOMPONENTUI || !INCLUDE_IJCOMMANDSINK || !INCLUDE_IJMESSAGESINK]
    Q_UNUSED(iid);
    Q_UNUSED(ver);
[!endif]

    return 0;
}

std::string [!output COMPONENT_CLASS_NAME]::componentName() const
{
    return "[!output COMPONENT_NAME]";
}

std::string [!output COMPONENT_CLASS_NAME]::componentDesc() const
{
    return "[!output COMPONENT_DESC]";
}

[!if INCLUDE_DYNAMIC_LAYOUT]
void [!output COMPONENT_CLASS_NAME]::attach()
{
[!if INCLUDE_JOBSERVER]
    // 订阅消息
    q_attempter->notifier().beginGroup(this)
            .endGroup();

[!endif]
    //TODO: Add your statements.
}

void [!output COMPONENT_CLASS_NAME]::detach()
{
    //TODO: Add your statements.

[!if INCLUDE_JOBSERVER]
    // 取消订阅消息
    q_attempter->notifier().remove(this);
[!endif]
}

[!endif]
[!if INCLUDE_IJCOMPONENTUI]
void *[!output COMPONENT_CLASS_NAME]::createWindow(void *parent, const std::string &objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_ui) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_ui = new [!output COMPONENT_UI_CLASS_NAME](*q_attempter);

    return qobject_cast<QWidget *>(q_ui);
}

[!endif]
[!if INCLUDE_IJCOMMANDSINK]
bool [!output COMPONENT_CLASS_NAME]::commandSink(void *sender, const std::string &domain, const std::string &objectName,
                           const std::string &eventType, void *data)
{
    Q_UNUSED(domain);
    Q_UNUSED(objectName);
    Q_UNUSED(eventType);
    Q_UNUSED(data);

    QObject *objectSender = reinterpret_cast<QObject *>(sender);
    if (!objectSender) {
        return false;
    }

    //TODO: Add your statements.

    return false;
}

[!endif]
[!if INCLUDE_IJMESSAGESINK]
bool [!output COMPONENT_CLASS_NAME]::messageSink(IJComponent *sender, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(sender);
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    //TODO: Add your statements.

    return false;
}

[!endif]
[!if INCLUDE_JOBSERVER]
std::string [!output COMPONENT_CLASS_NAME]::observerId() const
{
    return componentName();
}

[!endif]