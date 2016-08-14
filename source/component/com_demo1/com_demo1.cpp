#include "precomp.h"
#include "com_demo1.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void* attemper)
{
    // �ǹ���Ա�û�����ʹ�ô����
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }

    return static_cast<IJComponent *>
            (new ComDemo1(reinterpret_cast<IJAttempter *>(attemper)));
}

ComDemo1::ComDemo1(IJAttempter *attemper)
    : q_attempter(attemper)
    , q_widget(0)
{
    q_notifier = jframeLayout()->notifier();
}

ComDemo1::~ComDemo1()
{

}

void ComDemo1::releaseInterface()
{
    if (q_widget) {
        //q_widget->deleteLater();
        //q_widget = 0;
    }
}

void* ComDemo1::queryInterface(const char* iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string ComDemo1::componentId() const
{
    return "com_demo1";
}

std::string ComDemo1::componentDesc() const
{
    return "���ʾ�� #1";
}

bool ComDemo1::initialize()
{
    // �������
    jframeLayout()->attachComponent(this);

    return true;
}

void ComDemo1::shutdown()
{
    // �������
    jframeLayout()->detachComponent(this);
}

void ComDemo1::attach()
{
    // ������Ϣ
    q_notifier->begin(this)
            .end();
}

void ComDemo1::detach()
{
    // ȡ��������Ϣ
    q_notifier->pop(this);
}

void *ComDemo1::createUi(void *parent, const char *objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_widget) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_widget = new QPushButton(QStringLiteral("�������1"));

    // �����������
    jframeLayout()->attachComponentUi(this, q_widget);

    return 0;
}

std::string ComDemo1::jobserverId() const
{
    return componentId();
}
