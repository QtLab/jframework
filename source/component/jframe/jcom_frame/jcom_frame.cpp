#include "precomp.h"
#include "jcom_frame.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void *gAttempter)
{
    // ¼ÓÔØ¿ò¼Ü
    if (!jframeFacade()->loadFrame()) {
        jframeFacade()->exitFrame();
        return 0;   // ¼ÓÔØÊ§°Ü
    }

    return static_cast<IGF_Component *>
            (new JComFrame(reinterpret_cast<IGF_Attempter *>(gAttempter)));
}

JComFrame::JComFrame(IGF_Attempter *gAttempter)
    : q_gAttempter(gAttempter)
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

const char *JComFrame::GetComponentID() const
{
    static const char* _componentId = "jcom_frame";
    return _componentId;
}

const char *JComFrame::GetComponentName() const
{
    static const char* _componentName = "¿ò¼Ü¼ÓÔØ×é¼þ";
    return _componentName;
}
