#include "precomp.h"
#include "jframe_facade.h"
#include "private/jframe_facade_p.h"

//
IJFrameFacade *jframeFacade()
{
    return JFrameFacade::getInstance();
}

//
Q_EXTERN_C JFRAME_FACADE_EXPORT IJUnknown* _func_jframe_facade()
{
    return jframeFacade();
}
