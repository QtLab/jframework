#include "precomp.h"
#include "com_demo4.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void* attemper)
{
    // �ǹ���Ա�û�����ʹ�ô����
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
    if (q_widget) {
        q_widget->deleteLater();
        q_widget = 0;
    }
}

bool ComDemo4::loadInterface()
{
    return true;
}

void ComDemo4::releaseInterface()
{

}

void* ComDemo4::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string ComDemo4::componentName() const
{
    return "com_demo4";
}

std::string ComDemo4::componentDesc() const
{
    return "���ʾ�� #4";
}

void ComDemo4::attach()
{
    // ������Ϣ
    q_notifier->beginGroup(this)
            .endGroup();
}

void ComDemo4::detach()
{
    // ȡ��������Ϣ
    q_notifier->remove(this);
}

void *ComDemo4::createWindow(void *parent, const std::string &objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_widget) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_widget = new QPushButton(QStringLiteral("�������4"));

    return qobject_cast<QWidget *>(q_widget);
}

std::string ComDemo4::observerId() const
{
    return componentName();
}
