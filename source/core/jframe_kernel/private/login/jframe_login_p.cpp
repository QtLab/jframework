#include "precomp.h"
#include "jframe_login_p.h"
#include "jloginmanager.h"

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

std::string JFrameLogin::interfaceIdentity() const
{
    return IID_IJFrameLogin;
}

unsigned int JFrameLogin::interfaceVersion() const
{
    return VER_IJFrameLogin;
}

void *JFrameLogin::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, data->loginManager);

    return 0;
}

bool JFrameLogin::loadInterface()
{
    bool result = true;

    // ���ص�¼������
    if (data->loginManager) {
        result = result && data->loginManager->loadInterface();
    }

    return result;
}

void JFrameLogin::releaseInterface()
{
    // �ͷŵ�¼������
    if (data->loginManager) {
        data->loginManager->releaseInterface();
    }
}

std::list<std::string> JFrameLogin::queryMethod() const
{
    std::list<std::string> methods;

    // frame_login
    methods.push_back(std::string("frame_login").append("..."));

    return methods;
}

bool JFrameLogin::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    // ������¼ģ�飬��ʾ��¼����
    if (method == "frame_login") {
        if (data->loginManager) {
            result = data->loginManager->login();
        }
    }
    // ע����¼��Ĭ�ϲ���������ܷ�ʽ��
    else if (method == "frame_logout") {
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

    // ������¼������
    data->loginManager = new JLoginManager();
}

JFrameLogin::~JFrameLogin()
{
    // ���ٵ�¼������
    if (data->loginManager) {
        delete data->loginManager;
        data->loginManager = 0;
    }

    delete data;
}
