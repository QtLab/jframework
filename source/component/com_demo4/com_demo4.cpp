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
    return "���ʾ�� #4";
}

bool ComDemo4::initialize()
{
    // �������
    jframeLayout()->attachComponent(this);

    return true;
}

void ComDemo4::shutdown()
{
    // �������
    jframeLayout()->detachComponent(this);
}

void ComDemo4::attach()
{
    // ������Ϣ
    q_notifier->begin(this)
            .end();
}

void ComDemo4::detach()
{
    // ȡ��������Ϣ
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
    q_widget = new QPushButton(QStringLiteral("�������4"));

    // �����������
    jframeLayout()->attachComponentUi(this, q_widget);

    return 0;
}

std::string ComDemo4::jobserverId() const
{
    return componentId();
}
