#include "precomp.h"
#include "com_demo2.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void* attemper)
{
    // �ǹ���Ա�û�����ʹ�ô����
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }

    return static_cast<IJComponent *>
            (new ComDemo2(reinterpret_cast<IJAttempter *>(attemper)));
}

ComDemo2::ComDemo2(IJAttempter *attemper)
    : q_attempter(attemper)
    , q_widget(0)
{
    q_notifier = jframeLayout()->notifier();
}

ComDemo2::~ComDemo2()
{

}

void ComDemo2::releaseInterface()
{
    if (q_widget) {
        //q_widget->deleteLater();
        //q_widget = 0;
    }
}

void* ComDemo2::queryInterface(const char* iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string ComDemo2::componentId() const
{
    return "com_demo2";
}

std::string ComDemo2::componentDesc() const
{
    return "���ʾ�� #2";
}

bool ComDemo2::initialize()
{
    // �������
    jframeLayout()->attachComponent(this);

    return true;
}

void ComDemo2::shutdown()
{
    // �������
    jframeLayout()->detachComponent(this);
}

void ComDemo2::attach()
{
    // ������Ϣ
    q_notifier->begin(this)
            .end();
}

void ComDemo2::detach()
{
    // ȡ��������Ϣ
    q_notifier->pop(this);
}

void *ComDemo2::createUi(void *parent, const char *objectName)
{
    Q_UNUSED(parent);
    Q_UNUSED(objectName);

    //
    if (q_widget) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_widget = new QPushButton(QStringLiteral("�������2"));

    // �����������
    jframeLayout()->attachComponentUi(this, q_widget);

    return 0;
}

std::string ComDemo2::jobserverId() const
{
    return componentId();
}
