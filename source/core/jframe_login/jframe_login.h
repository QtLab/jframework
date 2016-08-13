#ifndef JFRAME_LOGIN_H
#define JFRAME_LOGIN_H

#include "jframe_core.h"
#include <vector>
#include <list>
#include <map>

namespace JFrameLoginSpace
{
    // 用户权限类型枚举
    enum UserType
    {
        UserTypeInvalid = -1,   // 无效用户
        UserTypeSuperAdmin,     // 超级管理员
        UserTypeOrdinAdmin,     // 普通管理员
        UserTypeSuperUser,      // 超级用户
        UserTypeOrdinUser       // 普通用户
    };

    // 模块权限等级类型枚举
    enum PowerLevel
    {
        PowerLevelInvalid = -1, // 无效权限（或模块无效）
        PowerLevelNo,           // 无权限
        PowerLevelView,         // 可查看
        PowerLevelNodify        // 可查看、修改
    };
}

// 接口标识
#define VER_IJLoginDBMgr J_INTERFACE_VERSION(1, 0)
#define IID_IJLoginDBMgr J_IID_INTERFACE(IJLoginDBMgr)

/**
 * @brief The IJLoginDBMgr class
 */
class IJLoginDBMgr
{
public:
    virtual ~IJLoginDBMgr() {}

    //

};

//////////////////////////////////////////////////////////////////////

// 接口标识
#define VER_IJLoginManager J_INTERFACE_VERSION(1, 0)
#define IID_IJLoginManager J_IID_INTERFACE(IJLoginManager)

/**
 * @brief The IJLoginManager class
 */
class IJLoginManager
{
public:
    virtual ~IJLoginManager() {}

    // 获取登录管理模块有效性
    virtual bool isValid() const = 0;

    // 显示登录界面
    virtual bool execute() = 0;

    // 注销登录
    virtual bool logout() = 0;

    // 获取登录数据库管理接口实例
    virtual IJLoginDBMgr *loginDBMgr() = 0;

    // 获取登录界面管理接口实例
    //virtual ILoginPage *loginPage() = 0;

    // 获取用户登录信息
    //virtual JFrameLoginSpace::UserElement userInfo() const = 0;

    // 获取当前席位信息
    //virtual JFrameLoginSpace::SeatElement currentSeatInfo() const = 0;

    // 获取当前系统信息
    //virtual JFrameLoginSpace::SystemElement currentSystemInfo() const = 0;

    // 获取当前登录用户是否为管理员
    virtual bool isAdminUser() const = 0;

    // 获取用户名称
    virtual std::string userName() const = 0;

    // 获取当前登录用户类型
    virtual int userType() const = 0;

    // 获取当前席位
    virtual std::string currentSeat() const = 0;

    // 获取当前系统
    virtual std::string currentSystem() const = 0;
};

//////////////////////////////////////////////////////////////////

// 接口标识
#define VER_IJFrameLogin J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameLogin J_IID_INTERFACE(IJFrameLogin)

/**
 * @brief The IJFrameLogin class
 */
class IJFrameLogin : public IJObject
{
public:
    virtual ~IJFrameLogin() {}

    // 获取登录管理接口实例
    virtual IJLoginManager *loginManager() = 0;
};

///

#ifdef JFRAME_LOGIN_DLL
#ifdef _MSC_VER
#   ifdef JFRAME_LOGIN_MAKEDLL
#       define JFRAME_LOGIN_EXPORT __declspec(dllexport)
#   else
#       define JFRAME_LOGIN_EXPORT __declspec(dllimport)

#       ifdef _MSC_VER
#           pragma comment(lib, "JFRAME_logind.lib")
#       else
#           pragma comment(lib, "JFRAME_login.lib")
#       endif // !_MSC_VER

#   endif // !JFRAME_LOGIN_MAKEDLL
#else
#define JFRAME_LOGIN_EXPORT
#endif // _MSC_VER

//
JFRAME_LOGIN_EXPORT IJFrameLogin *jframeLogin();

#endif // JFRAME_LOGIN_DLL

#ifndef JFRAME_LOGIN_EXPORT
#define JFRAME_LOGIN_EXPORT
#endif

#endif // JFRAME_LOGIN_H
