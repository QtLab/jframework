#include "precomp.h"
#include "com_demo3.h"
#include "testwidget1.h"

//
J_EXTERN_C J_ATTR_EXPORT void *CreateComponent(void* attemper)
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
    , q_testWidget1(0)
{
    q_notifier = jframeLayout()->notifier();
}

ComDemo3::~ComDemo3()
{
    if (q_testWidget1) {
        q_testWidget1->deleteLater();
        q_testWidget1 = 0;
    }
}

bool ComDemo3::loadInterface()
{
    return true;
}

bool ComDemo3::updateInterface()
{
    bool result = true;

    //

    return result;
}

void ComDemo3::releaseInterface()
{

}

void* ComDemo3::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string ComDemo3::componentName() const
{
    return "com_demo3";
}

std::string ComDemo3::componentDesc() const
{
    return QStringLiteral("组件示例 #2").toStdString();
}

void ComDemo3::attach()
{
    // 订阅消息
    q_notifier->beginGroup(this)
            .endGroup();
}

void ComDemo3::detach()
{
    // 取消订阅消息
    q_notifier->remove(this);
}

void *ComDemo3::createWindow(void *parent, const std::string &objectName)
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

std::string ComDemo3::observerId() const
{
    return componentName();
}
