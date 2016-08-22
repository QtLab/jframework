#include "precomp.h"
#include "com_demo2.h"
#include "testwidget1.h"

//
J_EXTERN_C J_ATTR_EXPORT void *CreateComponent(void* attemper)
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
    , q_testWidget1(0)
{

}

ComDemo2::~ComDemo2()
{
    if (q_testWidget1) {
        q_testWidget1->deleteLater();
        q_testWidget1 = 0;
    }
}

bool ComDemo2::loadInterface()
{
    return true;
}

bool ComDemo2::updateInterface()
{
    bool result = true;

    //

    return result;
}

void ComDemo2::releaseInterface()
{

}

void* ComDemo2::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);
    J_QUERY_INTERFACE(IJCommandSink, iid, ver);
    J_QUERY_INTERFACE(IJMessageSink, iid, ver);

    return 0;
}

std::string ComDemo2::componentName() const
{
    return "com_demo2";
}

std::string ComDemo2::componentDesc() const
{
    return QStringLiteral("组件示例 #2").toStdString();
}

void ComDemo2::attach()
{
    // 订阅框架消息
    q_attempter->notifier().beginGroup(this)
            .endGroup();

    // 订阅组件消息
    q_attempter->beginGroup(this)
            .subMessage("show_text", &ComDemo2::onShowText)
            .endGroup();
}

void ComDemo2::detach()
{
    // 取消订阅框架消息
    q_attempter->notifier().remove(this);

    // 取消订阅组件消息
    q_attempter->unsubMessage(this);
}

void *ComDemo2::createWindow(void *parent, const std::string &objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_testWidget1) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_testWidget1 = new TestWidget1(*q_attempter);

    return qobject_cast<QWidget *>(q_testWidget1);
}

bool ComDemo2::messageSink(IJComponent *sender, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(sender);
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    return false;
}

bool ComDemo2::commandSink(void *sender, const std::string &domain, const std::string &objectName,
                           const std::string &eventType, void *data)
{
    Q_UNUSED(sender);
    Q_UNUSED(domain);
    Q_UNUSED(objectName);
    Q_UNUSED(eventType);
    Q_UNUSED(data);

    return false;
}

std::string ComDemo2::observerId() const
{
    return componentName();
}

JLRESULT ComDemo2::onShowText(IJComponent *component, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(component);
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    qDebug() << "onShowText >> component:"
             << QString::fromStdString(component->componentName())
             << "id:" << QString::fromStdString(id);

    return 0;
}
