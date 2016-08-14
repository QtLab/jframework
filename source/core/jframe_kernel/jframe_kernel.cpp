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
extern "C" JFRAME_KERNEL_EXPORT IJObject* CreateInstance()
{
    return static_cast<IJObject *>(JFrameKernel::getInstance());
}

/**
 * @brief ReleaseInstance
 */
extern "C" JFRAME_KERNEL_EXPORT void ReleaseInstance()
{
    JFrameKernel::releaseInstance();
}
