#include "precomp.h"
#include "com_demo4.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void* gAttemper)
{
    // 非管理员用户才能使用此组件
    if (jframeLogin()->loginManager()->isAdminUser()) {
        return 0;
    }

    return static_cast<IGF_Component *>
            (new ComDemo4(reinterpret_cast<IGF_Attempter *>(gAttemper)));
}

ComDemo4::ComDemo4(IGF_Attempter* gAttemper)
    : q_gAttempter(gAttemper)
    , q_widget(0)
{
    q_notifier = jframeLayout()->notifier();
}

ComDemo4::~ComDemo4()
{

}

void ComDemo4::Release()
{
    if (q_widget) {
        q_widget->deleteLater();
        q_widget = 0;
    }
}

void* ComDemo4::QueryInterface(const char* IID, unsigned int dwQueryVer)
{
    QUERYINTERFACE(IGF_ComponentUI, IID, dwQueryVer);

    return 0;
}

void ComDemo4::Initialization()
{
    // 订阅消息
    q_notifier->begin(this)
            .end();

    // 挂载组件
    jframeLayout()->attachComponent(this);
}

void ComDemo4::Shutdown()
{
    // 取消订阅消息
    q_notifier->pop(this);

    // 分离组件
    jframeLayout()->detachComponent(this);
}

const char* ComDemo4::GetComponentID() const
{
    static const char* _componentId = "com_demo4";
    return _componentId;
}

const char* ComDemo4::GetComponentName() const
{
    static const char* _componentName = "demo4组件";
    return _componentName;
}

void* ComDemo4::CreateUI(void* parent, const char* windowName)
{
    Q_UNUSED(parent);
    Q_UNUSED(windowName);

    //
    if (q_widget) {
        Q_ASSERT(false);
        return 0;
    }

    //
    q_widget = new QPushButton(QStringLiteral("测试组件4"));

    // 挂载组件界面
    jframeLayout()->attachComponentUi(this, q_widget);

    return 0;
}

std::string ComDemo4::jobserverId() const
{
    return GetComponentID();
}
