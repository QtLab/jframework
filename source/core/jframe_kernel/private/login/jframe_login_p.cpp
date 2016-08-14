#include "precomp.h"
#include "jframe_login_p.h"
#include "jloginmanager.h"
//#include "login_database.h"
//#include "login_dialog.h"

// struct JFrameLoginData

struct JFrameLoginData
{
    IJLoginManager *loginManager;   // ��¼������ʵ��

    JFrameLoginData() :
        loginManager(0)
    {

    }
};

// class JFrameLogin

//
QMutex JFrameLogin::_instance_mutex;
JFrameLogin* JFrameLogin::_instance;

JFrameLogin* JFrameLogin::getInstance()
{
    // ˫�ؼ�⣨��ֹ���߳̾�����
    if (JFrameLogin::_instance == 0) {
        JFrameLogin::_instance_mutex.lock();
        if (JFrameLogin::_instance == 0) {
            JFrameLogin* tempInstance = new JFrameLogin;
            JFrameLogin::_instance = tempInstance;
        }
        JFrameLogin::_instance_mutex.unlock();
    }

    return JFrameLogin::_instance;
}

void JFrameLogin::releaseInstance()
{
    // ˫�ؼ�⣨��ֹ���߳̾�����
    if (JFrameLogin::_instance != 0) {
        JFrameLogin::_instance_mutex.lock();
        if (JFrameLogin::_instance != 0) {
            JFrameLogin::_instance_mutex.unlock();
            delete JFrameLogin::_instance;
            JFrameLogin::_instance_mutex.lock();
            JFrameLogin::_instance = 0;
        }
        JFrameLogin::_instance_mutex.unlock();
    }
}

void JFrameLogin::releaseInterface()
{
    //
    if (data->loginManager) {
        data->loginManager->releaseInterface();
    }

    JFrameLogin::releaseInstance();
}

void *JFrameLogin::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);
    J_QUERY_MEMBER_OBJECT(IJLoginManager, iid, ver, data->loginManager);

    return 0;
}

std::string JFrameLogin::objectIdentity() const
{
    return IID_IJFrameLogin;
}

unsigned int JFrameLogin::objectVersion() const
{
    return VER_IJFrameLogin;
}

bool JFrameLogin::invoke(const char *method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    // ������¼ģ�飬��ʾ��¼����
    if (strcmp(method, "frame_login") == 0) {
        if (data->loginManager) {
            result = data->loginManager->execute();
        }
    }
    // ע����¼��Ĭ�ϲ���������ܷ�ʽ��
    else if (strcmp(method, "frame_logout") == 0) {
        if (data->loginManager) {
            result = data->loginManager->logout();
        }
    }

    va_end(ap);

    return result;
}

IJLoginManager *JFrameLogin::loginManager()
{
    return data->loginManager;
}

JFrameLogin::JFrameLogin()
{
    data = new JFrameLoginData;

    data->loginManager = new JLoginManager();
}

JFrameLogin::~JFrameLogin()
{
    // ���ٵ�¼������ʵ��
    if (data->loginManager) {
        delete data->loginManager;
        data->loginManager = 0;
    }

    delete data;
}
