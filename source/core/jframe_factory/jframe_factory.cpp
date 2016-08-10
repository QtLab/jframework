#include "precomp.h"
#include "jframe_factory.h"
#include "private/jframe_factory_p.h"


//
IJFrameFactory* jframeFactory()
{
    return JFrameFactory::getInstance();
}

//
Q_EXTERN_C JFRAME_FACTORY_EXPORT IJUnknown* _func_jframe_factory()
{
    return jframeFactory();
}
