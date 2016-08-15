#ifndef JFRAME_LOGIN_H
#define JFRAME_LOGIN_H

#include "jframe_kernel.h"
#include <vector>
#include <map>

namespace JFrameLoginSpace
{
    /**
     * @brief The UserType enum : �û�Ȩ������ö��
     */
    enum UserType
    {
        UserTypeInvalid = -1,   // ��Ч�û�
        UserTypeSuperAdmin,     // ��������Ա
        UserTypeOrdinAdmin,     // ��ͨ����Ա
        UserTypeSuperUser,      // �����û�
        UserTypeOrdinUser       // ��ͨ�û�
    };

    /**
     * @brief The PowerLevel enum : ģ��Ȩ�޵ȼ�����ö��
     */
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
 * @brief The IJLoginDBMgr class : ��¼���ݿ������
 */
class IJLoginDBMgr : public IJUnknown
{
public:
    /**
     * @brief ~IJLoginDBMgr : ��������
     */
    virtual ~IJLoginDBMgr() {}

    /**
     * @brief interfaceIdentity : ��ȡ�ӿڱ�ʶ
     * @return : �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJLoginDBMgr; }

    /**
     * @brief interfaceVersion : ��ȡ�ӿڰ汾
     * @return : �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJLoginDBMgr; }
};

//////////////////////////////////////////////////////////////////////

// �ӿڱ�ʶ
#define VER_IJLoginManager J_INTERFACE_VERSION(1, 0)
#define IID_IJLoginManager J_IID_INTERFACE(IJLoginManager)

/**
 * @brief The IJLoginManager class : ��¼������
 */
class IJLoginManager : public IJUnknown
{
public:
    /**
     * @brief ~IJLoginManager : ��������
     */
    virtual ~IJLoginManager() {}

    /**
     * @brief interfaceIdentity : ��ȡ�ӿڱ�ʶ
     * @return : �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJLoginManager; }

    /**
     * @brief interfaceVersion : ��ȡ�ӿڰ汾
     * @return : �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJLoginManager; }

    /**
     * @brief isValid : ��ȡ��¼������״̬
     * @return : ��¼������״̬��true����Ч��false����Ч
     */
    virtual bool isValid() const = 0;

    /**
     * @brief login : ��¼
     * @return : ��¼״̬��true����¼�ɹ���false����¼ʧ��
     */
    virtual bool login() = 0;

    /**
     * @brief logout : ע��
     * @return : ע��״̬��true��ע���ɹ���false��ע��ʧ��
     */
    virtual bool logout() = 0;

    /**
     * @brief loginDBMgr : ��ȡ��¼���ݿ������
     * @return : ��¼���ݿ������
     */
    virtual IJLoginDBMgr *loginDBMgr() = 0;

    // ��ȡ��¼�������ӿ�ʵ��
    //virtual ILoginPage *loginPage() = 0;

    // ��ȡ�û���¼��Ϣ
    //virtual JFrameLoginSpace::UserElement userInfo() const = 0;

    // ��ȡ��ǰϯλ��Ϣ
    //virtual JFrameLoginSpace::SeatElement currentSeatInfo() const = 0;

    // ��ȡ��ǰϵͳ��Ϣ
    //virtual JFrameLoginSpace::SystemElement currentSystemInfo() const = 0;

    /**
     * @brief isAdminUser : ��ȡ��ǰ��¼�û��Ƿ�Ϊ����Ա
     * @return : ��ѯ�����true���ǹ���Ա��false�����ǹ���Ա
     */
    virtual bool isAdminUser() const = 0;

    /**
     * @brief userName : ��ȡ�û�����
     * @return : �û�����
     */
    virtual std::string userName() const = 0;

    /**
     * @brief userType : ��ȡ��ǰ��¼�û�����
     * @return : ��ǰ��¼�û����͡��鿴jframe_login.hͷ�ļ��е�UserType����
     */
    virtual int userType() const = 0;

    /**
     * @brief currentSeat : ��ȡ��ǰϯλ����
     * @return : ��ǰϯλ����
     */
    virtual std::string currentSeat() const = 0;

    /**
     * @brief currentSystem : ��ȡ��ǰϵͳ����
     * @return : ��ǰϵͳ����
     */
    virtual std::string currentSystem() const = 0;
};

//////////////////////////////////////////////////////////////////

// �ӿڱ�ʶ
#define VER_IJFrameLogin J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameLogin J_IID_INTERFACE(IJFrameLogin)

/**
 * @brief The IJFrameLogin class
 */
class IJFrameLogin : public IJUnknown
{
public:
    /**
     * @brief ~IJFrameLogin : ��������
     */
    virtual ~IJFrameLogin() {}

    /**
     * @brief interfaceIdentity : ��ȡ�ӿڱ�ʶ
     * @return : �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameLogin; }

    /**
     * @brief interfaceVersion : ��ȡ�ӿڰ汾
     * @return : �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameLogin; }

    /**
     * @brief loginManager : ��ȡ��¼������
     * @return : ��¼������
     */
    virtual IJLoginManager *loginManager() = 0;
};

#endif // JFRAME_LOGIN_H
