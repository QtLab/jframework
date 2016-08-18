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
    , q_frameFilter(0)
{
    q_notifier = jframeLayout()->notifier();
}

JComLayout::~JComLayout()
{

}

bool JComLayout::loadInterface()
{
    // 创建框架过滤器
    q_frameFilter = new FrameFilter(q_notifier, q_attempter);
    if (!q_frameFilter->init()) {
        //
    }

    // 安装主窗口事件过滤
    q_frameFilter->attachEventFilter();

    // 启动默认系统
    q_notifier->postMessage("j_load_default_system");

    return true;
}

bool JComLayout::updateInterface()
{
    bool result = true;

    //

    return result;
}

void JComLayout::releaseInterface()
{
    //
    // 去除主窗口事件过滤
    q_frameFilter->detachEventFilter();

    //
    delete q_frameFilter;
    q_frameFilter = 0;
}

void *JComLayout::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string JComLayout::componentName() const
{
    return "jcom_layout";
}

std::string JComLayout::componentDesc() const
{
    return "框架布局组件";
}

void JComLayout::attach()
{
    // 订阅消息
    q_notifier->beginGroup(this)
            .endGroup();
}

void JComLayout::detach()
{
    // 取消订阅消息
    q_notifier->remove(this);
}

void *JComLayout::createWindow(void *parent, const std::string &objectName)
{
    Q_UNUSED(parent);
    QWidget *widget = 0;
    if (objectName == "mainView") {
        widget = jframeLayout()->mainView();
    }

    return qobject_cast<QWidget *>(widget);
}

std::string JComLayout::observerId() const
{
    return componentName();
}
