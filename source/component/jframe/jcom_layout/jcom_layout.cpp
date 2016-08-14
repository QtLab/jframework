#include "precomp.h"
#include "jcom_layout.h"
#include "private/frame_filter.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void *attempter)
{
    return static_cast<IJComponent *>
            (new JComLayout(reinterpret_cast<IJAttempter *>(attempter)));
}

JComLayout::JComLayout(IJAttempter *attempter)
    : q_attempter(attempter)
{
    q_notifier = jframeLayout()->notifier();

    //
    q_frameFilter = new FrameFilter(q_notifier, q_attempter);
    if (!q_frameFilter->init()) {
        //
    }
}

JComLayout::~JComLayout()
{

}

void JComLayout::releaseInterface()
{
    if (q_frameFilter) {
        q_frameFilter->deleteLater();
        q_frameFilter = 0;
    }
}

void *JComLayout::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string JComLayout::componentId() const
{
    return "jcom_layout";
}

std::string JComLayout::componentDesc() const
{
    return "框架布局组件";
}

bool JComLayout::initialize()
{
    //
    if (q_frameFilter) {
        // 安装主窗口事件过滤
        q_frameFilter->attachEventFilter();
    }

    // 挂载组件
    jframeLayout()->attachComponent(this, true);

    // 启动默认系统
    q_notifier->post("j_load_default_system");

    return true;
}

void JComLayout::shutdown()
{
    //
    if (q_frameFilter) {
        // 去除主窗口事件过滤
        q_frameFilter->detachEventFilter();
    }

    // 分离组件
    jframeLayout()->detachComponent(this);
}

void JComLayout::attach()
{
    // 订阅消息
    q_notifier->begin(this)
            .end();
}

void JComLayout::detach()
{
    // 取消订阅消息
    q_notifier->pop(this);
}

void *JComLayout::createUi(void *parent, const char *objectName)
{
    Q_UNUSED(parent);
    QWidget *widget = 0;
    if (strcmp(objectName, "mainView") == 0) {
        widget = jframeLayout()->mainView();
    }

    // 挂载组件界面
    jframeLayout()->attachComponentUi(this, widget);

    return widget;
}

std::string JComLayout::jobserverId() const
{
    return componentId();
}
