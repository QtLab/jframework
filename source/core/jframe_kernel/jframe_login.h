#ifndef JFRAME_LOGIN_H
#define JFRAME_LOGIN_H

#include "jframe_kernel.h"
#include <vector>
#include <map>

namespace JFrameLoginSpace
{
    /**
     * @brief �û�Ȩ������ö��
     */
    enum UserType
    {
        UserTypeInvalid = -1,   /**< ��Ч�û� */
        UserTypeSuperAdmin,     /**< ��������Ա */
        UserTypeOrdinAdmin,     /**< ��ͨ����Ա */
        UserTypeSuperUser,      /**< �����û� */
        UserTypeOrdinUser       /**< ��ͨ�û� */
    };

    /**
     * @brief ģ��Ȩ�޵ȼ�����ö��
     */
    enum PowerLevel
    {
        PowerLevelInvalid = -1, /**< ��ЧȨ�ޣ���ģ����Ч�� */
        PowerLevelNo,           /**< ��Ȩ�� */
        PowerLevelView,         /**< �ɲ鿴 */
        PowerLevelNodify        /**< �ɲ鿴���޸� */
    };
}

/** IJLoginDBMgr �ӿڱ�ʶ */
#define VER_IJLoginDBMgr J_INTERFACE_VERSION(1, 0)
#define IID_IJLoginDBMgr J_IID_INTERFACE(IJLoginDBMgr)

/**
 * @brief ��¼���ݿ������
 */
class IJLoginDBMgr : public IJUnknown
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJLoginDBMgr() {}

    /**
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJLoginDBMgr; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJLoginDBMgr; }
};

//////////////////////////////////////////////////////////////////////

/** IJLoginManager �ӿڱ�ʶ */
#define VER_IJLoginManager J_INTERFACE_VERSION(1, 0)
#define IID_IJLoginManager J_IID_INTERFACE(IJLoginManager)

/**
 * @brief ��¼������
 */
class IJLoginManager : public IJUnknown
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJLoginManager() {}

    /**
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJLoginManager; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJLoginManager; }

    /**
     * @brief ��ȡ��¼������״̬
     * @return ��¼������״̬��true����Ч��false����Ч
     */
    virtual bool isValid() const = 0;

    /**
     * @brief ��¼
     * @return ��¼״̬��true����¼�ɹ���false����¼ʧ��
     */
    virtual bool login() = 0;

    /**
     * @brief ע��
     * @return ע��״̬��true��ע���ɹ���false��ע��ʧ��
     */
    virtual bool logout() = 0;

    /**
     * @brief ��ȡ��¼���ݿ������
     * @return ��¼���ݿ������
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
     * @brief ��ȡ��ǰ��¼�û��Ƿ�Ϊ����Ա
     * @return ��ѯ�����true���ǹ���Ա��false�����ǹ���Ա
     */
    virtual bool isAdminUser() const = 0;

    /**
     * @brief ��ȡ�û�����
     * @return �û�����
     */
    virtual std::string userName() const = 0;

    /**
     * @brief ��ȡ��ǰ��¼�û�����
     * @return ��ǰ��¼�û����͡��鿴 UserType ����
     */
    virtual int userType() const = 0;

    /**
     * @brief ��ȡ��ǰϯλ����
     * @return ��ǰϯλ����
     */
    virtual std::string currentSeat() const = 0;

    /**
     * @brief ��ȡ��ǰϵͳ����
     * @return ��ǰϵͳ����
     */
    virtual std::string currentSystem() const = 0;
};

//////////////////////////////////////////////////////////////////

/** IJFrameLogin �ӿڱ�ʶ */
#define VER_IJFrameLogin J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameLogin J_IID_INTERFACE(IJFrameLogin)

/**
 * @brief The IJFrameLogin class
 */
class IJFrameLogin : public IJUnknown
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJFrameLogin() {}

    /**
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameLogin; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameLogin; }

    /**
     * @brief ��ȡ��¼������
     * @return ��¼������
     */
    virtual IJLoginManager *loginManager() = 0;
};

#endif // JFRAME_LOGIN_H
