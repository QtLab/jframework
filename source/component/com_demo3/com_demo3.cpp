#include "precomp.h"
#include "com_demo3.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void* attemper)
{
    // 非管理员用户才能使用此组件
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }

    return static_cast<IJComponent *>
            (new ComDemo3(reinterpret_cast<IJAttempter *>(attemper)));
}

ComDemo3::ComDemo3(IJAttempter *attemper)
    : q_attempter(attemper)
    , q_widget(0)
{
    q_notifier = jframeLayout()->notifier();
}

ComDemo3::~ComDemo3()
{

}

void ComDemo3::releaseInterface()
{
    if (q_widget) {
        q_widget->deleteLater();
        q_widget = 0;
    }
}

void* ComDemo3::queryInterface(const char* iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string ComDemo3::componentId() const
{
    return "com_demo3";
}

std::string ComDemo3::componentDesc() const
{
    return "组件示例 #3";
}

bool ComDemo3::initialize()
{
    // 挂载组件
    jframeLayout()->attachComponent(this);

    return true;
}

void ComDemo3::shutdown()
{
    // 分离组件
    jframeLayout()->detachComponent(this);
}

void ComDemo3::attach()
{
    // 订阅消息
    q_notifier->begin(this)
            .end();
}

void ComDemo3::detach()
{
    // 取消订阅消息
    q_notifier->pop(this);
}

void *ComDemo3::createUi(void *parent, const char *windowName)
{
    Q_UNUSED(parent);
    Q_UNUSED(windowName);

    //
    if (q_widget) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_widget = new QPushButton(QStringLiteral("测试组件3"));

    // 挂载组件界面
    jframeLayout()->attachComponentUi(this, q_widget);

    return 0;
}

std::string ComDemo3::jobserverId() const
{
    return componentId();
}
