#include "precomp.h"
#include "com_demo2.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void* attemper)
{
    // 非管理员用户才能使用此组件
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }

    return static_cast<IJComponent *>
            (new ComDemo2(reinterpret_cast<IJAttempter *>(attemper)));
}

ComDemo2::ComDemo2(IJAttempter *attemper)
    : q_attempter(attemper)
    , q_widget(0)
{
    q_notifier = jframeLayout()->notifier();
}

ComDemo2::~ComDemo2()
{

}

void ComDemo2::releaseInterface()
{
    if (q_widget) {
        //q_widget->deleteLater();
        //q_widget = 0;
    }
}

void* ComDemo2::queryInterface(const char* iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string ComDemo2::componentId() const
{
    return "com_demo2";
}

std::string ComDemo2::componentDesc() const
{
    return "组件示例 #2";
}

bool ComDemo2::initialize()
{
    // 挂载组件
    jframeLayout()->attachComponent(this);

    return true;
}

void ComDemo2::shutdown()
{
    // 分离组件
    jframeLayout()->detachComponent(this);
}

void ComDemo2::attach()
{
    // 订阅消息
    q_notifier->begin(this)
            .end();
}

void ComDemo2::detach()
{
    // 取消订阅消息
    q_notifier->pop(this);
}

void *ComDemo2::createUi(void *parent, const char *objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_widget) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_widget = new QPushButton(QStringLiteral("测试组件2"));

    // 挂载组件界面
    jframeLayout()->attachComponentUi(this, q_widget);

    return 0;
}

std::string ComDemo2::jobserverId() const
{
    return componentId();
}
