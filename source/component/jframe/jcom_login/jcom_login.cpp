#include "precomp.h"
#include "jcom_login.h"

//
J_EXTERN_C J_ATTR_EXPORT void *CreateComponent(void *attempter)
{
    // 登录显示
    if (!jframeFacade()->loginFrame()) {
        jframeFacade()->exitFrame();
        return 0;   // 登录失败
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

bool JComLogin::loadInterface()
{
    return true;
}

bool JComLogin::updateInterface()
{
    bool result = true;

    //

    return result;
}

void JComLogin::releaseInterface()
{

}

void *JComLogin::queryInterface(const std::string &iid, unsigned int ver)
{
    Q_UNUSED(iid);
    Q_UNUSED(ver);

    return 0;
}

std::string JComLogin::componentName() const
{
    return "jcom_login";
}

std::string JComLogin::componentDesc() const
{
    return QStringLiteral("框架登录组件").toStdString();
}

void JComLogin::attach()
{

}

void JComLogin::detach()
{

}
