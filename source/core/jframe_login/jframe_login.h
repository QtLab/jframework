#ifndef JFRAME_LOGIN_H
#define JFRAME_LOGIN_H

#include "jframe_core.h"
#include <vector>
#include <list>
#include <map>

namespace JFrameLoginSpace
{
    // �û�Ȩ������ö��
    enum UserType
    {
        UserTypeInvalid = -1,   // ��Ч�û�
        UserTypeSuperAdmin,     // ��������Ա
        UserTypeOrdinAdmin,     // ��ͨ����Ա
        UserTypeSuperUser,      // �����û�
        UserTypeOrdinUser       // ��ͨ�û�
    };

    // ģ��Ȩ�޵ȼ�����ö��
    enum PowerLevel
    {
        PowerLevelInvalid = -1, // ��ЧȨ�ޣ���ģ����Ч��
        PowerLevelNo,           // ��Ȩ��
        PowerLevelView,         // �ɲ鿴
        PowerLevelNodify        // �ɲ鿴���޸�
    };
}

// �ӿڱ�ʶ
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

// �ӿڱ�ʶ
#define VER_IJLoginManager J_INTERFACE_VERSION(1, 0)
#define IID_IJLoginManager J_IID_INTERFACE(IJLoginManager)

/**
 * @brief The IJLoginManager class
 */
class IJLoginManager
{
public:
    virtual ~IJLoginManager() {}

    // ��ȡ��¼����ģ����Ч��
    virtual bool isValid() const = 0;

    // ��ʾ��¼����
    virtual bool execute() = 0;

    // ע����¼
    virtual bool logout() = 0;

    // ��ȡ��¼���ݿ����ӿ�ʵ��
    virtual IJLoginDBMgr *loginDBMgr() = 0;

    // ��ȡ��¼�������ӿ�ʵ��
    //virtual ILoginPage *loginPage() = 0;

    // ��ȡ�û���¼��Ϣ
    //virtual JFrameLoginSpace::UserElement userInfo() const = 0;

    // ��ȡ��ǰϯλ��Ϣ
    //virtual JFrameLoginSpace::SeatElement currentSeatInfo() const = 0;

    // ��ȡ��ǰϵͳ��Ϣ
    //virtual JFrameLoginSpace::SystemElement currentSystemInfo() const = 0;

    // ��ȡ��ǰ��¼�û��Ƿ�Ϊ����Ա
    virtual bool isAdminUser() const = 0;

    // ��ȡ�û�����
    virtual std::string userName() const = 0;

    // ��ȡ��ǰ��¼�û�����
    virtual int userType() const = 0;

    // ��ȡ��ǰϯλ
    virtual std::string currentSeat() const = 0;

    // ��ȡ��ǰϵͳ
    virtual std::string currentSystem() const = 0;
};

//////////////////////////////////////////////////////////////////

// �ӿڱ�ʶ
#define VER_IJFrameLogin J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameLogin J_IID_INTERFACE(IJFrameLogin)

/**
 * @brief The IJFrameLogin class
 */
class IJFrameLogin : public IJObject
{
public:
    virtual ~IJFrameLogin() {}

    // ��ȡ��¼����ӿ�ʵ��
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
