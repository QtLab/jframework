#include "precomp.h"
#include "jcom_login.h"
#include "jframe_layout.h"

//
extern "C" __declspec(dllexport) void *CreateComponent(void *attempter)
{
    // µÇÂ¼ÏÔÊ¾
    if (!jframeFacade()->loginFrame()) {
        jframeFacade()->exitFrame();
        return 0;   // µÇÂ¼Ê§°Ü
    }

    return static_cast<IJComponent *>
            (new JComLogin(reinterpret_cast<IJAttempter *>(attempter)));
}

JComLogin::JComLogin(IJAttempter *attempter)
    : q_attempter(attempter)
{

}

JComLogin::~JComLogin()
{

}

void JComLogin::releaseInterface()
{

}

void *JComLogin::queryInterface(const char *iid, unsigned int ver)
{
    Q_UNUSED(iid);
    Q_UNUSED(ver);

    return 0;
}

std::string JComLogin::componentId() const
{
    return "jcom_login";
}

std::string JComLogin::componentDesc() const
{
    return "¿ò¼ÜµÇÂ¼×é¼þ";
}

bool JComLogin::initialize()
{
    // ¹ÒÔØ×é¼þ
    jframeLayout()->attachComponent(this);

    return true;
}

void JComLogin::shutdown()
{
    // ·ÖÀë×é¼þ
    jframeLayout()->detachComponent(this);
}

void JComLogin::attach()
{

}

void JComLogin::detach()
{

}
