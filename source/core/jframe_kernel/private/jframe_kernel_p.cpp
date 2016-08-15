#include "precomp.h"
#include "jframe_kernel_p.h"
#include "core/jframe_core_p.h"
#include "layout/jframe_layout_p.h"
#include "login/jframe_login_p.h"
#include "qmfcapp.h"
#include "factory/jlogmanager.h"

// struct JFrameKernelData

struct JFrameKernelData
{
    IJLogManager *logManager;       // �����־ϵͳ����

    JFrameKernelData()
        : logManager(0)
    {

    }
};

// class JFrameKernel

//
QMutex JFrameKernel::_instance_mutex;
JFrameKernel *JFrameKernel::_instance = 0;

JFrameKernel *JFrameKernel::getInstance()
{
    // ˫�ؼ�⣨��ֹ���߳̾�����
    if (JFrameKernel::_instance == 0) {
        JFrameKernel::_instance_mutex.lock();
        if (JFrameKernel::_instance == 0) {
            JFrameKernel *tmpInstance = new JFrameKernel;
            JFrameKernel::_instance = tmpInstance;
        }
        JFrameKernel::_instance_mutex.unlock();
    }

    return JFrameKernel::_instance;
}

void JFrameKernel::releaseInstance()
{
    // ˫�ؼ�⣨��ֹ���߳̾�����
    if (JFrameKernel::_instance != 0) {
        JFrameKernel::_instance_mutex.lock();
        if (JFrameKernel::_instance != 0) {
            JFrameKernel::_instance_mutex.unlock();
            delete JFrameKernel::_instance;
            JFrameKernel::_instance_mutex.lock();
            JFrameKernel::_instance = 0;
        }
        JFrameKernel::_instance_mutex.unlock();
    }
}

std::string JFrameKernel::interfaceIdentity() const
{
    return IID_IJFrameKernel;
}

unsigned int JFrameKernel::interfaceVersion() const
{
    return VER_IJFrameKernel;
}

void *JFrameKernel::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, frameCore());
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, frameLayout());
    J_QUERY_MEMBER_OBJECT(IJUnknown, iid, ver, frameLogin());

    return 0;
}

bool JFrameKernel::loadInterface()
{
    bool result = true;

    //
    JwtCore::instance()->init();
    JwtCore::instance()->loadSystemLang();
    //qApp->setStyleSheet(JStyleSheet::instance()->styleSheet("default"));

    // ������־������
    if (data->logManager) {
        result = result && data->logManager->loadInterface();
    }

    // ���ؿ�ܺ���ϵͳ
    result = result && frameCore()->loadInterface();

    // ���ؿ�ܲ���ϵͳ
    result = result && frameLayout()->loadInterface();

    // ���ؿ�ܵ�¼ϵͳ
    result = result && frameLogin()->loadInterface();

    // �������
    result = result && frameCore()->attempter()->loadComponent();

    return true;
}

void JFrameKernel::releaseInterface()
{
    // �ͷſ�ܵ�¼ϵͳ
    frameLogin()->releaseInterface();

    // �ͷſ�ܲ���ϵͳ
    frameLayout()->releaseInterface();

    // �ͷſ�ܺ���ϵͳ
    frameCore()->releaseInterface();

    // �ͷ���־������
    if (data->logManager) {
        data->logManager->releaseInterface();
    }
}

std::list<std::string> JFrameKernel::queryMethod() const
{
    std::list<std::string> methods;

    // log
    methods.push_back(std::string("log").append("..."));

    return methods;
}

bool JFrameKernel::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;

    va_list ap;
    va_start(ap, argc);

    // ���һ����־
    if (method == "log") {
        result = invokeLog(argc, ap);
    }
    // ��ʾ���
    else if (method == "frame_show") {
        if (argc == 2) {
            // ת����ܺ���ϵͳ
            result = frameCore()->invokeMethod("frame_show", 2, va_arg(ap, bool), va_arg(ap, bool));
        }
    }
    // �����˳���ܣ��첽��ʽ��
    else if (method == "frame_try_exit") {
        // ת����ܲ���ϵͳ
        result = frameLayout()->invokeMethod("frame_try_exit");
    }
    // �˳���ܣ��첽��ʽ��
    else if (method == "frame_exit") {
        // ת����ܲ���ϵͳ
        result = frameLayout()->invokeMethod("frame_exit");
    }
    // ������ܣ��첽��ʽ��
    else if (method == "frame_restart") {
        // ת����ܲ���ϵͳ
        result = frameLayout()->invokeMethod("frame_restart");
    }
    // ��¼���
    else if (method == "frame_login") {
        // ת����ܵ�¼ϵͳ
        result = frameLogin()->invokeMethod("frame_login");
    }
    // ע�����
    else if (method == "frame_logout") {
        // ת����ܵ�¼ϵͳ
        result = frameLogin()->invokeMethod("frame_logout");
    }
    // ����Qt��Ϣѭ��ϵͳ
    else if (method == "run_q_app") {
        if (argc == 2) {
            // ת����ܲ���ϵͳ
            result = frameLayout()->invokeMethod("run_q_app", 2,
                                           va_arg(ap, void*), va_arg(ap, int*));
        }
    }
    // ����Qt��Ϣѭ��ϵͳ
    else if (method == "window_handle") {
        if (argc == 3) {
            // ת����ܲ���ϵͳ
            result = frameLayout()->invokeMethod("window_handle", 3, va_arg(ap, void*),
                                           va_arg(ap, char*), va_arg(ap, long));
        }
    }
    // ����Ĭ�Ͽ��ϵͳ
    else if (method == "load_default_system") {
        // ת����ܲ���ϵͳ
        result = frameLayout()->invokeMethod("load_default_system");
    }

    va_end(ap);

    return result;
}

IJLogManager *JFrameKernel::logManager()
{
    return data->logManager;
}

IJFrameCore *JFrameKernel::frameCore()
{
    return JFrameCore::getInstance();
}

IJFrameLayout *JFrameKernel::frameLayout()
{
    return JFrameLayout::getInstance();
}

IJFrameLogin *JFrameKernel::frameLogin()
{
    return JFrameLogin::getInstance();
}

bool JFrameKernel::invokeLog(int argc, va_list ap)
{
    // ��־������ʵ������Ч�Լ��
    if (!data->logManager) {
        return false;   // ��Ч
    }

    // ������Ч�Լ��
    if (argc < 2) {
        return false;   // ��Ч (type, msg, ...)
    }

    //
    const char* sType = va_arg(ap, char*);
    if (!sType) {
        return false;   // ������Ч
    }

    //
    IJLogManager::MsgType msgType = IJLogManager::InfoMsg;
    if (strcmp(sType, "emerge") == 0) {
        msgType = IJLogManager::EmergeMsg;
    } else if (strcmp(sType, "fatal") == 0) {
        msgType = IJLogManager::FatalMsg;
    } if (strcmp(sType, "alert") == 0) {
        msgType = IJLogManager::AlertMsg;
    } if (strcmp(sType, "crit") == 0) {
        msgType = IJLogManager::CriticalMsg;
    } if (strcmp(sType, "error") == 0) {
        msgType = IJLogManager::ErrorMsg;
    } if (strcmp(sType, "warn") == 0) {
        msgType = IJLogManager::WarningMsg;
    } if (strcmp(sType, "notice") == 0) {
        msgType = IJLogManager::NoticeMsg;
    } if (strcmp(sType, "info") == 0) {
        msgType = IJLogManager::InfoMsg;
    } if (strcmp(sType, "debug") == 0) {
        msgType = IJLogManager::DebugMsg;
    }

    //
    const char* msg = va_arg(ap, char*);
    if (!msg) {
        return false;   // ������Ч
    }

    //
    const char* file = 0;
    int line = 0;
    const char* func = 0;
    if (argc > 2) {
        file = va_arg(ap, char*);
    }
    if (argc > 3) {
        line = va_arg(ap, int);
    }
    if (argc > 4) {
        func = va_arg(ap, char*);
    }

    // ������־���������һ����־��Ϣ
    data->logManager->logging(msgType, msg, (argc - 1), file, line, func);

    return true;
}

JFrameKernel::JFrameKernel()
{
    // ����˽������ʵ��
    data = new JFrameKernelData;

    // ����Ĭ��ϵͳ��ʽ��
    //KwtCore::instance().loadDefaultSystemSheet();

    // ������־������ʵ��
    data->logManager = JFRAME_FACTORY_CREATE(IJLogManager);
}

JFrameKernel::~JFrameKernel()
{
    // ���ٿ�ܵ�¼����
    JFrameLogin::releaseInstance();

    // ���ٿ�ܲ��ֲ���
    JFrameLayout::releaseInstance();

    // ���ٿ�ܺ��Ĳ���
    JFrameCore::releaseInstance();

    // ������־������ʵ��
    if (data->logManager) {
        JFRAME_FACTORY_RELEASE(data->logManager, IJLogManager);
    }

    // ����˽������ʵ��
    delete data;
}
