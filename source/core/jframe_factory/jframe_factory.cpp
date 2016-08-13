#include "precomp.h"
#include "jframe_factory.h"
#include "private/jframe_factory_p.h"


//
IJFrameFactory* jframeFactory()
{
    return JFrameFactory::getInstance();
}

//
extern "C" JFRAME_FACTORY_EXPORT IJObject* CreateInstance()
{
    return static_cast<IJObject *>(jframeFactory());
}
