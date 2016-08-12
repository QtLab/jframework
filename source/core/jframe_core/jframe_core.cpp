#include "precomp.h"
#include "jframe_core.h"
#include "private/jframe_core_p.h"

//
IJFrameCore *jframeCore()
{
    return JFrameCore::getInstance();
}

//
extern "C" JFRAME_CORE_EXPORT IJUnknown* _func_jframe_core()
{
    return jframeCore();
}
