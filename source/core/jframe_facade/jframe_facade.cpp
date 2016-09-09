#include "precomp.h"
#include "jframe_facade.h"
#include "private/jframe_facade_p.h"

//
IJFrameFacade *jframeFacade()
{
    return JFrameFacade::getInstance();
}

//
J_EXTERN_C JFRAME_FACADE_EXPORT IJUnknown* CreateInstance()
{
    return static_cast<IJUnknown *>(JFrameFacade::getInstance());
}

//
J_EXTERN_C JFRAME_FACADE_EXPORT void ReleaseInstance()
{
    JFrameFacade::releaseInstance();
}
