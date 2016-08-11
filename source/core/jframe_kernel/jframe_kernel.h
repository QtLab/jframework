#ifndef JFRAME_KERNEL_H
#define JFRAME_KERNEL_H

// interface IJFrameKernel

class IJFrameKernel : public IJObject
{
public:
    virtual ~IJFrameKernel() {}
};

// 接口描述
#define VER_IJFrameKernel J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameKernel J_IID_INTERFACE(IJFrameKernel)

///

#ifdef JFRAME_KERNEL_DLL
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

//
JFRAME_KERNEL_EXPORT IJFrameKernel* jframeKernel();

#endif // JFRAME_KERNEL_DLL

#ifndef JFRAME_KERNEL_EXPORT
#define JFRAME_KERNEL_EXPORT
#endif

#endif // JFRAME_KERNEL_H
