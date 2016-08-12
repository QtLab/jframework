#include "precomp.h"
#include "jframe_login.h"
#include "private/jframe_login_p.h"

//
IJFrameLogin *jframeLogin()
{
    return JFrameLogin::getInstance();
}

extern "C" JFRAME_LOGIN_EXPORT IJUnknown *_func_jframe_login()
{
    return jframeLogin();
}
