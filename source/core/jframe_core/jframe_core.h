﻿#ifndef JFRAME_CORE_H
#define JFRAME_CORE_H

#include <string>

/// 版本管理

// 框架产品版本
#define JFRAME_MAJOR_VERSION 4
#define JFRAME_MINOR_VERSION 0
#define JFRAME_PATCH_VERSION 0
#define JFRAME_BUILD_VERSION 0

// 获取主版本
#define J_MAJOR_VERSION(ver) \
    ((((unsigned int)(ver)) & 0xff000000) >> 24)

// 获取次版本
#define J_MINOR_VERSION(ver) \
    ((((unsigned int)(ver)) & 0x00ff0000) >> 16)

// 获取补丁版本
#define J_PATCH_VERSION(ver) \
    ((((unsigned int)(ver)) & 0x0000ff00) >> 8)

// 获取内部版本
#define J_BUILD_VERSION(ver) \
    (((unsigned int)(ver)) & 0x000000ff)

// 接口版本
#define J_INTERFACE_VERSION(major, minor) \
    ((((unsigned int)(JFRAME_MAJOR_VERSION)) << 24) | \
    ((( unsigned int)(major)) << 16) | \
    ((  unsigned int)(minor)  << 8))

// 模块版本
#define J_PROCESS_VERSION(major, minor, patch) \
    ((((unsigned int)(JFRAME_MAJOR_VERSION)) << 24) | \
    ((( unsigned int)(major)) << 16) | \
    ((  unsigned int)(minor)  << 8) | \
    (   unsigned int)(patch))

//
#ifndef J_IID_INTERFACE
#define J_IID_INTERFACE(_interface_) "IID" # _interface_
#endif

/// 内部接口查询

//
#define J_IS_INSTANCEOF(_interface_, _iid_, _ver_) \
    ((strcmp(_iid_, IID_ ## _interface_) == 0) \
    && ((_ver_) == VER_ ## _interface_))

//
#define J_IS_OBJECT(_object_, _iid_, _ver_) \
    ((strcmp(_iid_, _object_->objectIdentity().c_str()) == 0) \
    && ((_ver_) == _object_->objectVersion()))

// 在当前接口子对象中查询

#define J_QUERY_INTERFACE(_interface_, _iid_, _ver_) \
    do { \
        if (J_IS_INSTANCEOF(_interface_, _iid_, _ver_)) \
        { return static_cast<_interface_ *>(this); } \
    } while(0)

// 在给定接口子对象中查询
#define J_QUERY_IJUNKNOWN(_interface_, _iid_, _ver_) \
    do { \
        if (J_IS_INSTANCE(IJUnknown, _iid_, _ver_)) \
        { return static_cast<IJUnknown *>(static_cast<_interface_ *>(this)); } \
    } while(0)

// 查询成员对象实例
#define J_QUERY_MEMBER_OBJECT(_interface_, _iid_, _ver_, _member_) \
    do { \
        if (_member_ != 0) { \
            if (J_IS_INSTANCEOF(_interface_, _iid_, _ver_)) \
            { return static_cast<_interface_ *>(_member_); } \
        } \
    } while(0)

/// 外部接口查询

// 当前接口查询
#define J_QUERY_THIS_INTERFACE(_interface_) \
    ((_interface_ *)queryInterface(IID_ ## _interface_, VER_ ## _interface_))

// 给对象引用查询
#define J_QUERY_OBJECT_INTERFACE(_object_, _interface_) \
    ((_interface_ *)_object_.queryInterface(IID_ ## _interface_, VER_ ## _interface_))

// 给定对象指针查询
#define J_QUERY_POBJECT_INTERFACE(_object_, _interface) \
    do { \
        if (_object_ != 0) { \
            _interface_ *_t = ((_interface_ *)_object_->queryInterface( \
                IID_ ## _interface_, VER_ ## _interface_)); \
            if (_t) { return _t_; } \
        } \
    } while(0)

///

// interface IJUnknown

class IJUnknown
{
public:
    virtual ~IJUnknown() {}

    // 释放接口
    virtual void releaseInterface() = 0;

    // 查询接口
    virtual void* queryInterface(const char *iid, unsigned int ver) = 0;
};

// 接口描述
#define VER_IJUnknown J_INTERFACE_VERSION(1, 0)
#define IID_IJUnknown J_IID_INTERFACE(IJUnknown)

// interface IJObject

class IJObject : public IJUnknown
{
public:
    virtual ~IJObject() {}

    // 获取对象标识
    virtual std::string objectIdentity() const = 0;

    // 获取对象版本
    virtual unsigned int objectVersion() const = 0;

    // 执行函数调用
    virtual bool invoke(const char *method, int argc = 0, ...)
    { (void)method; (void)argc; return false; }
};

// 接口描述
#define VER_IJObject J_INTERFACE_VERSION(1, 0)
#define IID_IJObject J_IID_INTERFACE(IJObject)

// interface IJComponent

class IJComponent : public IJObject
{
public:
    virtual ~IJComponent() {}
};

// 接口标识
#define VER_IJComponent J_INTERFACE_VERSION(1, 0)
#define IID_IJComponent J_IID_INTERFACE(IJComponent)

// interface IJComponentUi

class IJComponentUi : public IJObject
{
public:
    virtual ~IJComponentUi() {}
};

// 接口标识
#define VER_IJComponentUi J_INTERFACE_VERSION(1, 0)
#define IID_IJComponentUi J_IID_INTERFACE(IJComponentUi)

// interface IJCommandSink

class IJCommandSink
{
public:
    virtual ~IJCommandSink() {}
};

// 接口标识
#define VER_IJCommandSink J_INTERFACE_VERSION(1, 0)
#define IID_IJCommandSink J_IID_INTERFACE(IJCommandSink)

// interface IJMessageSink

class IJMessageSink
{
public:
    virtual ~IJMessageSink() {}
};

// 接口标识
#define VER_IJMessageSink J_INTERFACE_VERSION(1, 0)
#define IID_IJMessageSink J_IID_INTERFACE(IJMessageSink)

// interface IJMainWindow

class IJMainWindow
{
public:
    virtual ~IJMainWindow() {}
};

// 接口标识
#define VER_IJMainWindow J_INTERFACE_VERSION(1, 0)
#define IID_IJMainWindow J_IID_INTERFACE(IJMainWindow)

// interface IJAttempter

class IJAttempter
{
public:
    virtual ~IJAttempter() {}
};

// 接口标识
#define VER_IJAttempter J_INTERFACE_VERSION(1, 0)
#define IID_IJAttempter J_IID_INTERFACE(IJAttempter)

// interface IJUiPlugin

class IJUiPlugin
{
public:
    virtual ~IJUiPlugin() {}
};

// 接口标识
#define VER_IJUiPlugin J_INTERFACE_VERSION(1, 0)
#define IID_IJUiPlugin J_IID_INTERFACE(IJUiPlugin)

// interface IJFrameCore

class IJFrameCore : public IJObject
{
public:
    virtual ~IJFrameCore() {}
};

// 接口标识
#define VER_IJFrameCore J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameCore J_IID_INTERFACE(IJFrameCore)

///

#ifdef JFRAME_CORE_DLL
#   ifdef JFRAME_CORE_MAKEDLL
#       define JFRAME_CORE_EXPORT __declspec(dllexport)
#   else
#       define JFRAME_CORE_EXPORT __declspec(dllimport)

#       ifdef _MSC_VER
#           pragma comment(lib, "jframe_cored.lib")
#       else
#           pragma comment(lib, "jframe_core.lib")
#       endif // !_MSC_VER

#   endif // !JFRAME_CORE_MAKEDLL

//
JFRAME_CORE_EXPORT IJFrameCore* jframeCore();

#endif // JFRAME_CORE_DLL

#ifndef JFRAME_CORE_EXPORT
#define JFRAME_CORE_EXPORT
#endif

#endif // JFRAME_CORE_H