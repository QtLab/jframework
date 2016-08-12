#include "precomp.h"
#include "jcom_login.h"
#include "jframe_layout.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void *gAttempter)
{
    // µÇÂ¼ÏÔÊ¾
    if (!jframeFacade()->loginFrame()) {
        jframeFacade()->exitFrame();
        return 0;   // µÇÂ¼Ê§°Ü
    }

    return static_cast<IGF_Component *>
            (new JComLogin(reinterpret_cast<IGF_Attempter *>(gAttempter)));
}

JComLogin::JComLogin(IGF_Attempter *gAttempter)
    : q_gAttempter(gAttempter)
{

}

JComLogin::~JComLogin()
{

}

void JComLogin::Release()
{
}

void *JComLogin::QueryInterface(const char *IID, unsigned int dwQueryVer)
{
    (void)(IID);
    (void)(dwQueryVer);

    return 0;
}

void JComLogin::Initialization()
{
}

void JComLogin::Shutdown()
{
}

const char *JComLogin::GetComponentID() const
{
    static const char* _componentId = "jcom_login";
    return _componentId;
}

const char *JComLogin::GetComponentName() const
{
    static const char* _componentName = "¿ò¼ÜµÇÂ¼×é¼þ";
    return _componentName;
}
