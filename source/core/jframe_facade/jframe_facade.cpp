#include "precomp.h"
#include "jframe_facade.h"
#include "private/jframe_facade_p.h"

//
IJFrameFacade *jframeFacade()
{
    return JFrameFacade::getInstance();
}

//
extern "C" JFRAME_FACADE_EXPORT IJObject* CreateInstance()
{
    return static_cast<IJObject *>(JFrameFacade::getInstance());
}

//
extern "C" JFRAME_FACADE_EXPORT void ReleaseInstance()
{
    JFrameFacade::releaseInstance();
}
