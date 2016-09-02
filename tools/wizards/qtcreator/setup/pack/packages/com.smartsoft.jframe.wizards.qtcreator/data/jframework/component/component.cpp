#include "precomp.h"
#include "%{HdrFileName}"
@if '%{IncludeIJComponentUi}'
#include "%{UiHdrFileName}"
@endif

//
J_EXTERN_C J_ATTR_EXPORT void *CreateComponent(void* attemper)
{
@if '%{IncludeCheckLoginUser}'
    // 非管理员用户才能使用此组件
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }
    
@endif
    return static_cast<IJComponent *>
            (new %{ComponentClass}(reinterpret_cast<IJAttempter *>(attemper)));
}

%{ComponentClass}::%{ComponentClass}(IJAttempter *attemper)
    : q_attempter(attemper)
@if '%{IncludeIJComponentUi}'
    , q_ui(0)
@endif
{

}

%{ComponentClass}::~%{ComponentClass}()
{
@if '%{IncludeIJComponentUi}'
    if (q_ui) {
        q_ui->deleteLater();
        q_ui = 0;
    }
@endif
}

bool %{ComponentClass}::loadInterface()
{
@if !'%{IncludeDynamicLayout}' && '%{IncludeJObserver}'
    // 订阅消息
    q_attempter->notifier().beginGroup(this)
            .endGroup();

@endif
    bool result = true;

    //TODO: Add your loading statements.

    return result;
}

bool %{ComponentClass}::updateInterface()
{
    bool result = true;

    //TODO: Add your updating statements.

    return result;
}

void %{ComponentClass}::releaseInterface()
{
    //TODO: Add your release statements.
@if !'%{IncludeDynamicLayout}' && '%{IncludeJObserver}'

    // 取消订阅消息
    q_attempter->notifier().remove(this);
@endif
}

void* %{ComponentClass}::queryInterface(const std::string &iid, unsigned int ver)
{
@if '%{IncludeIJComponentUi}'
    J_QUERY_INTERFACE(%{IncludeIJComponentUi}, iid, ver);
@endif
@if '%{IncludeIJCommandSink}'
    J_QUERY_INTERFACE(%{IncludeIJCommandSink}, iid, ver);
@endif
@if '%{IncludeIJMessageSink}'
    J_QUERY_INTERFACE(%{IncludeIJMessageSink}, iid, ver);
@endif
@if !'%{IncludeIJComponentUi}' || !'%{IncludeIJCommandSink}' || !'%{IncludeIJMessageSink}'
    Q_UNUSED(iid);
    Q_UNUSED(ver);
@endif

    return 0;
}

std::string %{ComponentClass}::componentName() const
{
    return "%{ComponentName}";
}

std::string %{ComponentClass}::componentDesc() const
{
    return "%{ComponentDesc}";
}

@if '%{IncludeDynamicLayout}'
void %{ComponentClass}::attach()
{
@if  '%{IncludeJObserver}'
    // 订阅消息
    q_attempter->notifier().beginGroup(this)
            .endGroup();

@endif
    //TODO: Add your statements.
}

void %{ComponentClass}::detach()
{
    //TODO: Add your statements.

@if  '%{IncludeJObserver}'
    // 取消订阅消息
    q_attempter->notifier().remove(this);
@endif
}

@endif
@if '%{IncludeIJComponentUi}'
void *%{ComponentClass}::createWindow(void *parent, const std::string &objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_ui) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_ui = new %{UiClassName}(*q_attempter);

    return qobject_cast<QWidget *>(q_ui);
}

@endif
@if '%{IncludeIJCommandSink}'
bool %{ComponentClass}::commandSink(void *sender, const std::string &domain, const std::string &objectName,
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

@endif
@if '%{IncludeIJMessageSink}'
bool %{ComponentClass}::messageSink(IJComponent *sender, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(sender);
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    //TODO: Add your statements.

    return false;
}

@endif
std::string %{ComponentClass}::observerId() const
{
    return componentName();
}
