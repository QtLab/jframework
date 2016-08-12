#include "precomp.h"
#include "jcom_layout.h"
#include "private/frame_filter.h"
#include "jframe_layout.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void *gAttempter)
{
    return static_cast<IGF_Component *>
            (new JComLayout(reinterpret_cast<IGF_Attempter *>(gAttempter)));
}

JComLayout::JComLayout(IGF_Attempter *gAttempter)
    : q_gAttempter(gAttempter)
{
    q_notifier = jframeLayout()->notifier();

    //
    q_frameFilter = new FrameFilter(q_notifier, q_gAttempter);
    if (!q_frameFilter->init()) {
        //
    }

    // ��ʼ��GF��ܵ���������ܲ���ϵͳ
    jframeLayout()->invoke("set_g_attempter", 1, q_gAttempter);
}

JComLayout::~JComLayout()
{

}

void JComLayout::Release()
{
    if (q_frameFilter) {
        q_frameFilter->deleteLater();
        q_frameFilter = 0;
    }
}

void *JComLayout::QueryInterface(const char *IID, unsigned int dwQueryVer)
{
    QUERYINTERFACE(IGF_ComponentUI, IID, dwQueryVer);

    return 0;
}

void JComLayout::Initialization()
{
    // ������Ϣ
    q_notifier->begin(this)
            .end();

    // �������
    jframeLayout()->attachComponent(this, true);

    // ����Ĭ��ϵͳ
    q_notifier->post("j_load_default_system");
}

void JComLayout::Shutdown()
{
    // ȡ��������Ϣ
    q_notifier->pop(this);

    // �������
    jframeLayout()->detachComponent(this);
}

const char *JComLayout::GetComponentID() const
{
    static const char* _componentId = "jcom_layout";
    return _componentId;
}

const char *JComLayout::GetComponentName() const
{
    static const char* _componentName = "��ܲ������";
    return _componentName;
}

void *JComLayout::CreateUI(void *parent, const char *windowName)
{
    if (strcmp(windowName, "mainView") == 0) {
        QStackedWidget *stackedWidget = reinterpret_cast<QStackedWidget *>(parent);
        if (stackedWidget) {
            stackedWidget->addWidget(jframeLayout()->mainWindow());
            stackedWidget->setCurrentIndex(stackedWidget->count() - 1);
        }
    }

    // �����������
    jframeLayout()->attachComponentUi(this, jframeLayout()->mainWindow());

    return 0;
}

std::string JComLayout::jobserverId() const
{
    return GetComponentID();
}
