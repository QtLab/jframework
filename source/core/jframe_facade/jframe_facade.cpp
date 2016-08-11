#include "precomp.h"
#include "jframe_facade.h"
#include "private/jframe_facade_p.h"

//
IJFrameFacade *jframeFacade()
{
    return JFrameFacade::getInstance();
}

//
extern "C" JFRAME_FACADE_EXPORT IJUnknown* _func_jframe_facade()
{
    return jframeFacade();
}
