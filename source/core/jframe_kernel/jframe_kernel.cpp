#include "precomp.h"
#include "jframe_kernel.h"
#include "private/jframe_kernel_p.h"

/**
 * @brief jframeKernel
 * @return
 */
IJFrameKernel* jframeKernel()
{
    return JFrameKernel::getInstance();
}

/**
 * @brief CreateInstance
 * @return
 */
J_EXTERN_C JFRAME_KERNEL_EXPORT IJUnknown* CreateInstance()
{
    return static_cast<IJUnknown *>(JFrameKernel::getInstance());
}

/**
 * @brief ReleaseInstance
 */
J_EXTERN_C JFRAME_KERNEL_EXPORT void ReleaseInstance()
{
    JFrameKernel::releaseInstance();
}
