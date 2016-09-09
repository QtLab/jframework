#include "precomp.h"
#include "jframe_factory.h"
#include "private/jframe_factory_p.h"


//
IJFrameFactory* jframeFactory()
{
    return JFrameFactory::getInstance();
}

//
J_EXTERN_C JFRAME_FACTORY_EXPORT IJUnknown* CreateInstance()
{
    return static_cast<IJUnknown *>(JFrameFactory::getInstance());
}

//
J_EXTERN_C JFRAME_FACTORY_EXPORT void ReleaseInstance()
{
    JFrameFactory::releaseInstance();
}
