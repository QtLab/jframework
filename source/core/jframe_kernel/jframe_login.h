#ifndef JFRAME_LOGIN_H
#define JFRAME_LOGIN_H

#include "jframe_kernel.h"
#include <vector>
#include <map>

namespace JFrameLoginSpace
{
    /**
     * @brief The UserType enum : 用户权限类型枚举
     */
    enum UserType
    {
        UserTypeInvalid = -1,   // 无效用户
        UserTypeSuperAdmin,     // 超级管理员
        UserTypeOrdinAdmin,     // 普通管理员
        UserTypeSuperUser,      // 超级用户
        UserTypeOrdinUser       // 普通用户
    };

    /**
     * @brief The PowerLevel enum : 模块权限等级类型枚举
     */
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
 * @brief The IJLoginDBMgr class : 登录数据库管理器
 */
class IJLoginDBMgr : public IJUnknown
{
public:
    /**
     * @brief ~IJLoginDBMgr : 析构函数
     */
    virtual ~IJLoginDBMgr() {}

    /**
     * @brief interfaceIdentity : 获取接口标识
     * @return : 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJLoginDBMgr; }

    /**
     * @brief interfaceVersion : 获取接口版本
     * @return : 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJLoginDBMgr; }
};

//////////////////////////////////////////////////////////////////////

// 接口标识
#define VER_IJLoginManager J_INTERFACE_VERSION(1, 0)
#define IID_IJLoginManager J_IID_INTERFACE(IJLoginManager)

/**
 * @brief The IJLoginManager class : 登录管理器
 */
class IJLoginManager : public IJUnknown
{
public:
    /**
     * @brief ~IJLoginManager : 析构函数
     */
    virtual ~IJLoginManager() {}

    /**
     * @brief interfaceIdentity : 获取接口标识
     * @return : 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJLoginManager; }

    /**
     * @brief interfaceVersion : 获取接口版本
     * @return : 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJLoginManager; }

    /**
     * @brief isValid : 获取登录管理器状态
     * @return : 登录管理器状态。true，有效；false，无效
     */
    virtual bool isValid() const = 0;

    /**
     * @brief login : 登录
     * @return : 登录状态。true，登录成功；false，登录失败
     */
    virtual bool login() = 0;

    /**
     * @brief logout : 注销
     * @return : 注销状态。true，注销成功；false，注销失败
     */
    virtual bool logout() = 0;

    /**
     * @brief loginDBMgr : 获取登录数据库管理器
     * @return : 登录数据库管理器
     */
    virtual IJLoginDBMgr *loginDBMgr() = 0;

    // 获取登录界面管理接口实例
    //virtual ILoginPage *loginPage() = 0;

    // 获取用户登录信息
    //virtual JFrameLoginSpace::UserElement userInfo() const = 0;

    // 获取当前席位信息
    //virtual JFrameLoginSpace::SeatElement currentSeatInfo() const = 0;

    // 获取当前系统信息
    //virtual JFrameLoginSpace::SystemElement currentSystemInfo() const = 0;

    /**
     * @brief isAdminUser : 获取当前登录用户是否为管理员
     * @return : 查询结果。true，是管理员；false，不是管理员
     */
    virtual bool isAdminUser() const = 0;

    /**
     * @brief userName : 获取用户名称
     * @return : 用户名称
     */
    virtual std::string userName() const = 0;

    /**
     * @brief userType : 获取当前登录用户类型
     * @return : 当前登录用户类型。查看 UserType 定义
     */
    virtual int userType() const = 0;

    /**
     * @brief currentSeat : 获取当前席位名称
     * @return : 当前席位名称
     */
    virtual std::string currentSeat() const = 0;

    /**
     * @brief currentSystem : 获取当前系统名称
     * @return : 当前系统名称
     */
    virtual std::string currentSystem() const = 0;
};

//////////////////////////////////////////////////////////////////

// 接口标识
#define VER_IJFrameLogin J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameLogin J_IID_INTERFACE(IJFrameLogin)

/**
 * @brief The IJFrameLogin class
 */
class IJFrameLogin : public IJUnknown
{
public:
    /**
     * @brief ~IJFrameLogin : 析构函数
     */
    virtual ~IJFrameLogin() {}

    /**
     * @brief interfaceIdentity : 获取接口标识
     * @return : 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameLogin; }

    /**
     * @brief interfaceVersion : 获取接口版本
     * @return : 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameLogin; }

    /**
     * @brief loginManager : 获取登录管理器
     * @return : 登录管理器
     */
    virtual IJLoginManager *loginManager() = 0;
};

#endif // JFRAME_LOGIN_H
