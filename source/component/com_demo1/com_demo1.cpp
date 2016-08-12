#include "precomp.h"
#include "com_demo1.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void* gAttemper)
{
    // �ǹ���Ա�û�����ʹ�ô����
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }

    return static_cast<IGF_Component *>
            (new ComDemo1(reinterpret_cast<IGF_Attempter *>(gAttemper)));
}

ComDemo1::ComDemo1(IGF_Attempter* gAttemper)
    : q_gAttempter(gAttemper)
    , q_widget(0)
{
    q_notifier = jframeLayout()->notifier();
}

ComDemo1::~ComDemo1()
{

}

void ComDemo1::Release()
{
    if (q_widget) {
        q_widget->deleteLater();
        q_widget = 0;
    }
}

void* ComDemo1::QueryInterface(const char* IID, unsigned int dwQueryVer)
{
    QUERYINTERFACE(IGF_ComponentUI, IID, dwQueryVer);

    return 0;
}

void ComDemo1::Initialization()
{
    // ������Ϣ
    q_notifier->begin(this)
            .end();

    // �������
    jframeLayout()->attachComponent(this);
}

void ComDemo1::Shutdown()
{
    // ȡ��������Ϣ
    q_notifier->pop(this);

    // �������
    jframeLayout()->detachComponent(this);
}

const char* ComDemo1::GetComponentID() const
{
    static const char* _componentId = "com_demo1";
    return _componentId;
}

const char* ComDemo1::GetComponentName() const
{
    static const char* _componentName = "demo1���";
    return _componentName;
}

void* ComDemo1::CreateUI(void* parent, const char* windowName)
{
    Q_UNUSED(parent);
    Q_UNUSED(windowName);

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
    return GetComponentID();
}
