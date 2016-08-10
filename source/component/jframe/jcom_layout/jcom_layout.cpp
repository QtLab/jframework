#include "precomp.h"
#include "jcom_layout.h"
#include "private/frame_filter.h"
#include "jframe_layout.h"

//
extern "C" __declspec(dllexport) void *func_create_component(void *attempter)
{
    return static_cast<IGF_Component *>
            (new JComLayout(reinterpret_cast<IGF_Attempter *>(attempter)));
}

JComLayout::JComLayout(IGF_Attempter *gAttempter)
    : q_gAttempter(gAttmpter)
{
    q_notifier = jframeLayout()->notifier();

    //
    q_frameFilter = new FrameFilter(q_notifier, q_gAttempter);
    if (!q_frameFilter.init()) {
        //
    }

    // 初始化GF框架调度器到框架布局系统
    jframeLayout()->invoke("set_g_attempter", 1, q_gAttempter);
}

JComLayout::~JComLayout()
{

}

void JComLayout::Release()
{
    if (q_frameFilter) {
        q_frameFilter.deleteLater();
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
    // 订阅消息
    q_notifier->begin(this)
            .end();

    // 挂载组件
    jframeLayout()->attachComponent(this, true);

    // 启动默认系统
    q_notifier->post("j_load_default_system");
}

void JComLayout::Shutdown()
{
    // 取消订阅消息
    q_notifier->pop(this);

    // 分离组件
    jframeLayout()->detachComponent(this);
}

const char *JComLayout::GetComponentID()
{
    static const char* _componentId = "jcom_layout";
    return _componentId;
}

const char *JComLayout::GetComponentName()
{
    static const char* _componentName = "框架布局组件";
    return _componentName;
}

void *JComLayout::CreateUI(void *parent, const char *windowName)
{
    if (strcmp(windowName, "mainView") == 0) {
        QStackedWidget *stackedWidget = reinterpret_cast<QStackedWidget *>(parent);
        if (stackedWidget) {
            stackedWidget->addWidget(jframeLayout()->mainWindow());
            stackedWidget->setCurrenIndex(stackedWidget->count() - 1);
        }
    }

    // 挂载组件界面
    jframeLayout()->attachComponentUi(this, jframeLayout()->mainWindow());
}

std::string JComLayout::jobserverId() const
{
    return GetComponentID();
}
