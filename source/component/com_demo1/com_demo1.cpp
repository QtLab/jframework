#include "precomp.h"
#include "com_demo1.h"
#include "testwidget1.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void* attemper)
{
    // 非管理员用户才能使用此组件
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }

    return static_cast<IJComponent *>
            (new ComDemo1(reinterpret_cast<IJAttempter *>(attemper)));
}

ComDemo1::ComDemo1(IJAttempter *attemper)
    : q_attempter(attemper)
    , q_testWidget1(0)
{
    q_notifier = jframeLayout()->notifier();
}

ComDemo1::~ComDemo1()
{
    if (q_testWidget1) {
        q_testWidget1->deleteLater();
        q_testWidget1 = 0;
    }
}

bool ComDemo1::loadInterface()
{
    return true;
}

bool ComDemo1::updateInterface()
{
    bool result = true;

    //

    return result;
}

void ComDemo1::releaseInterface()
{

}

void* ComDemo1::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string ComDemo1::componentName() const
{
    return "com_demo1";
}

std::string ComDemo1::componentDesc() const
{
    return "组件示例 #1";
}

void ComDemo1::attach()
{
    // 订阅消息
    q_notifier->beginGroup(this)
            .endGroup();
}

void ComDemo1::detach()
{
    // 取消订阅消息
    q_notifier->remove(this);
}

void *ComDemo1::createWindow(void *parent, const std::string &objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_testWidget1) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_testWidget1 = new TestWidget1(q_notifier);

    return qobject_cast<QWidget *>(q_testWidget1);
}

std::string ComDemo1::observerId() const
{
    return componentName();
}
