#ifndef JFRAME_KERNEL_H
#define JFRAME_KERNEL_H

#include "jframe_core.h"
#include "jlogmanager.h"
#include "jnotifier.h"

// �ӿ�����
#define VER_IJFrameKernel J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameKernel J_IID_INTERFACE(IJFrameKernel)

/**
 * @brief The IJFrameKernel class
 */
class IJFrameKernel : public IJObject
{
public:
    virtual ~IJFrameKernel() {}

    // ��ȡ��־����ʵ���ӿ�
    virtual IJLogManager *logManager() = 0;
};

///

#ifdef JFRAME_KERNEL_DLL
#ifdef _MSC_VER
#   ifdef JFRAME_KERNEL_MAKEDLL
#       define JFRAME_KERNEL_EXPORT __declspec(dllexport)
#   else
#       define JFRAME_KERNEL_EXPORT __declspec(dllimport)

#       ifdef _MSC_VER
#           pragma comment(lib, "jframe_kerneld.lib")
#       else
#           pragma comment(lib, "jframe_kernel.lib")
#       endif // !_MSC_VER

#   endif // !JFRAME_KERNEL_MAKEDLL
#else
#define JFRAME_KERNEL_EXPORT
#endif // _MSC_VER

//
JFRAME_KERNEL_EXPORT IJFrameKernel* jframeKernel();

#endif // JFRAME_KERNEL_DLL

#ifndef JFRAME_KERNEL_EXPORT
#define JFRAME_KERNEL_EXPORT
#endif

#endif // JFRAME_KERNEL_H
