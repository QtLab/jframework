#include "precomp.h"
#include "jframe_login.h"
#include "private/jframe_login_p.h"

//
IJFrameLogin *jframeLogin()
{
    return JFrameLogin::getInstance();
}

Q_EXTERN_C JFRAME_LOGIN_EXPORT IJunknown *_func_jframe_login()
{
    return jframeLogin();
}
