#include "stdafx.h"
#include "com_demo5.h"
#include "MfcDemo1Container.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void* attemper)
{
    // 非管理员用户才能使用此组件
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }

    return static_cast<IJComponent *>
            (new ComDemo5(reinterpret_cast<IJAttempter *>(attemper)));
}

ComDemo5::ComDemo5(IJAttempter *attemper)
    : q_attempter(attemper)
    , q_widget(0)
{
    q_notifier = jframeLayout()->notifier();
}

ComDemo5::~ComDemo5()
{
    if (q_widget) {
        q_widget->deleteLater();
        q_widget = 0;
    }
}

bool ComDemo5::loadInterface()
{
    return true;
}

bool ComDemo5::updateInterface()
{
    bool result = true;

    //

    return result;
}

void ComDemo5::releaseInterface()
{
	if (q_widget) {
		q_widget->releaseInterface();
	}
}

void* ComDemo5::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string ComDemo5::componentName() const
{
    return "com_demo5";
}

std::string ComDemo5::componentDesc() const
{
    return "组件示例 #5";
}

std::string ComDemo5::componentType() const
{
	return "mfc";
}

void ComDemo5::attach()
{
    // 订阅消息
    q_notifier->beginGroup(this)
            .endGroup();
}

void ComDemo5::detach()
{
    // 取消订阅消息
    q_notifier->remove(this);
}

void *ComDemo5::createWindow(void *parent, const std::string &objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_widget) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_widget = new CMfcDemo1Container(q_notifier);

    return qobject_cast<QWidget *>(q_widget);
}

std::string ComDemo5::observerId() const
{
    return componentName();
}
