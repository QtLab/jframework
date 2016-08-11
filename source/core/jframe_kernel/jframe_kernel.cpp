#include "precomp.h"
#include "jframe_kernel.h"
#include "private/jframe_kernel_p.h"

//
IJFrameKernel* jframeKernel()
{
    return JFrameKernel::getInstance();
}

//
extern "C" JFRAME_KERNEL_EXPORT IJUnknown* _func_jframe_kernel()
{
    return jframeKernel();
}
