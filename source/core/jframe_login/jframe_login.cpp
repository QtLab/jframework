#include "precomp.h"
#include "jframe_login.h"
#include "private/jframe_login_p.h"

//
IJFrameLogin *jframeLogin()
{
    return JFrameLogin::getInstance();
}

extern "C" JFRAME_LOGIN_EXPORT IJObject *CreateInstance()
{
    return static_cast<IJObject *>(jframeLogin());
}
