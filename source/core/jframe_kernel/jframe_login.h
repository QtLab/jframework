#ifndef JFRAME_LOGIN_H
#define JFRAME_LOGIN_H

#include "jframe_kernel.h"
#include <vector>
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
class IJLoginDBMgr : public IJObject
{
public:
    virtual ~IJLoginDBMgr() {}

    /**
     * @brief objectIdentity : 获取对象标识
     * @return
     */
    virtual std::string objectIdentity() const { return IID_IJLoginDBMgr; }

    /**
     * @brief objectVersion : 获取对象版本
     * @return
     */
    virtual unsigned int objectVersion() const { return VER_IJLoginDBMgr; }
};

//////////////////////////////////////////////////////////////////////

// 接口标识
#define VER_IJLoginManager J_INTERFACE_VERSION(1, 0)
#define IID_IJLoginManager J_IID_INTERFACE(IJLoginManager)

/**
 * @brief The IJLoginManager class
 */
class IJLoginManager : public IJObject
{
public:
    virtual ~IJLoginManager() {}

    /**
     * @brief objectIdentity : 获取对象标识
     * @return
     */
    virtual std::string objectIdentity() const { return IID_IJLoginManager; }

    /**
     * @brief objectVersion : 获取对象版本
     * @return
     */
    virtual unsigned int objectVersion() const { return VER_IJLoginManager; }

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

    /**
     * @brief objectIdentity : 获取对象标识
     * @return
     */
    virtual std::string objectIdentity() const { return IID_IJFrameLogin; }

    /**
     * @brief objectVersion : 获取对象版本
     * @return
     */
    virtual unsigned int objectVersion() const { return VER_IJFrameLogin; }

    // 获取登录管理接口实例
    virtual IJLoginManager *loginManager() = 0;
};

#endif // JFRAME_LOGIN_H
