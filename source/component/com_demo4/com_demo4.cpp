#include "precomp.h"
#include "com_demo4.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void* attemper)
{
    // 非管理员用户才能使用此组件
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }

    return static_cast<IJComponent *>
            (new ComDemo4(reinterpret_cast<IJAttempter *>(attemper)));
}

ComDemo4::ComDemo4(IJAttempter *attemper)
    : q_attempter(attemper)
    , q_widget(0)
{
    q_notifier = jframeLayout()->notifier();
}

ComDemo4::~ComDemo4()
{

}

void ComDemo4::releaseInterface()
{
    if (q_widget) {
        q_widget->deleteLater();
        q_widget = 0;
    }
}

void* ComDemo4::queryInterface(const char* iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string ComDemo4::componentId() const
{
    return "com_demo4";
}

std::string ComDemo4::componentDesc() const
{
    return "组件示例 #4";
}

bool ComDemo4::initialize()
{
    // 挂载组件
    jframeLayout()->attachComponent(this);

    return true;
}

void ComDemo4::shutdown()
{
    // 分离组件
    jframeLayout()->detachComponent(this);
}

void ComDemo4::attach()
{
    // 订阅消息
    q_notifier->begin(this)
            .end();
}

void ComDemo4::detach()
{
    // 取消订阅消息
    q_notifier->pop(this);
}

void *ComDemo4::createUi(void *parent, const char *windowName)
{
    Q_UNUSED(parent);
    Q_UNUSED(windowName);

    //
    if (q_widget) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_widget = new QPushButton(QStringLiteral("测试组件4"));

    // 挂载组件界面
    jframeLayout()->attachComponentUi(this, q_widget);

    return 0;
}

std::string ComDemo4::jobserverId() const
{
    return componentId();
}
