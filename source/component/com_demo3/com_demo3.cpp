#include "precomp.h"
#include "com_demo3.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void* attemper)
{
    // �ǹ���Ա�û�����ʹ�ô����
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
    if (q_widget) {
        q_widget->deleteLater();
        q_widget = 0;
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
    return "���ʾ�� #3";
}

void ComDemo3::attach()
{
    // ������Ϣ
    q_notifier->beginGroup(this)
            .endGroup();
}

void ComDemo3::detach()
{
    // ȡ��������Ϣ
    q_notifier->remove(this);
}

void *ComDemo3::createWindow(void *parent, const std::string &objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_widget) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_widget = new QPushButton(QStringLiteral("�������3"));

    return qobject_cast<QWidget *>(q_widget);
}

std::string ComDemo3::observerId() const
{
    return componentName();
}
