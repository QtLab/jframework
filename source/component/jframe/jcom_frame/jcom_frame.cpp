#include "precomp.h"
#include "jcom_frame.h"

//
extern "C" __declspec(dllexport) void *func_create_component(void *attempter)
{
    // 加载框架
    if (!jframeFacade()->loadFrame()) {
        jframeFacade()->exitFrame();
        return 0;   // 加载失败
    }

    return static_cast<IGF_Component *>
            (new JComFrame(reinterpret_cast<IGF_Attempter *>(attempter)));
}

JComFrame::JComFrame(IGF_Attempter *gAttempter)
    : q_gAttempter(gAttmpter)
{

}

JComFrame::~JComFrame()
{

}

void JComFrame::Release()
{
}

void *JComFrame::QueryInterface(const char *IID, unsigned int dwQueryVer)
{
    (void)(IID);
    (void)(dwQueryVer);

    return 0;
}

void JComFrame::Initialization()
{
}

void JComFrame::Shutdown()
{
}

const char *JComFrame::GetComponentID()
{
    static const char* _componentId = "jcom_frame";
    return _componentId;
}

const char *JComFrame::GetComponentName()
{
    static const char* _componentName = "框架加载组件";
    return _componentName;
}
