#include "precomp.h"
#include "jframe_layout.h"
#include "private/jframe_layout_p.h"

//
IJFrameLayout *jframeLayout()
{
    return JFrameLayout::getInstance();
}

extern "C" JFRAME_LAYOUT_EXPORT IJObject *CreateInstance()
{
    return static_cast<IJObject *>(jframeLayout());
}
