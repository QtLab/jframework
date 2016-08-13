#include "precomp.h"
#include "jcom_layout.h"
#include "private/frame_filter.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void *attempter)
{
    return static_cast<IJComponent *>
            (new JComLayout(reinterpret_cast<IJAttempter *>(attempter)));
}

JComLayout::JComLayout(IJAttempter *attempter)
    : q_attempter(attempter)
{
    q_notifier = jframeLayout()->notifier();

    //
    q_frameFilter = new FrameFilter(q_notifier, q_attempter);
    if (!q_frameFilter->init()) {
        //
    }
}

JComLayout::~JComLayout()
{

}

void JComLayout::releaseInterface()
{
    if (q_frameFilter) {
        q_frameFilter->deleteLater();
        q_frameFilter = 0;
    }
}

void *JComLayout::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJComponentUi, iid, ver);

    return 0;
}

std::string JComLayout::componentId() const
{
    return "jcom_layout";
}

std::string JComLayout::componentDesc() const
{
    return "��ܲ������";
}

bool JComLayout::initialize()
{
    // �������
    jframeLayout()->attachComponent(this, true);

    // ����Ĭ��ϵͳ
    q_notifier->post("j_load_default_system");

    return true;
}

void JComLayout::shutdown()
{
    // �������
    jframeLayout()->detachComponent(this);
}

void JComLayout::attach()
{
    // ������Ϣ
    q_notifier->begin(this)
            .end();
}

void JComLayout::detach()
{
    // ȡ��������Ϣ
    q_notifier->pop(this);
}

void *JComLayout::createUi(void *parent, const char *windowName)
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
    return componentId();
}
