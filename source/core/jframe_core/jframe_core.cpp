#include "precomp.h"
#include "jframe_core.h"
#include "private/jframe_core_p.h"

//
IJFrameCore *jframeCore()
{
    return JFrameCore::getInstance();
}

//
extern "C" JFRAME_CORE_EXPORT IJObject* CreateInstance()
{
    return static_cast<IJObject *>(jframeCore());
}
