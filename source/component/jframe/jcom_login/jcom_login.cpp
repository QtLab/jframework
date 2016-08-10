#include "precomp.h"
#include "jcom_login.h"
#include "jframe_layout.h"

//
extern "C" __declspec(dllexport) void *func_create_component(void *attempter)
{
    // 登录显示
    if (!jframeFacade()->loginFrame()) {
        jframeFacade()->exitFrame();
        return 0;   // 登录失败
    }

    return static_cast<IGF_Component *>
            (new JComLogin(reinterpret_cast<IGF_Attempter *>(attempter)));
}

JComLogin::JComLogin(IGF_Attempter *gAttempter)
    : q_gAttempter(gAttmpter)
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

const char *JComLogin::GetComponentID()
{
    static const char* _componentId = "jcom_login";
    return _componentId;
}

const char *JComLogin::GetComponentName()
{
    static const char* _componentName = "框架登录组件";
    return _componentName;
}
