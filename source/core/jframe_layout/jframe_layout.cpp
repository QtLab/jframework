#include "precomp.h"
#include "jframe_layout.h"
#include "private/jframe_layout_p.h"


//
IJFrameLayout *jframeLayout()
{
    return JFrameLayout::getInstance();
}

Q_EXTERN_C JFRAME_LAYOUT_EXPORT IJunknown *_func_jframe_layout()
{
    return jframeLayout();
}
