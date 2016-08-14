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

void JFrameKernel::releaseInterface()
{
    // �ͷſ�ܵ�¼����
    JFrameLogin::getInstance()->releaseInterface();

    // �ͷſ�ܲ��ֲ���
    JFrameLayout::getInstance()->releaseInterface();

    // �ͷſ�ܺ��Ĳ���
    JFrameCore::getInstance()->releaseInterface();

    // �ͷ���־������
    if (data->logManager) {
        data->logManager->releaseInterface();
    }

    //
    JFrameKernel::releaseInstance();
}

void *JFrameKernel::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);
    J_QUERY_MEMBER_OBJECT(IJObject, iid, ver, frameCore());
    J_QUERY_MEMBER_OBJECT(IJObject, iid, ver, frameLayout());
    J_QUERY_MEMBER_OBJECT(IJObject, iid, ver, frameLogin());

    return 0;
}

std::string JFrameKernel::objectIdentity() const
{
    return IID_IJFrameKernel;
}

unsigned int JFrameKernel::objectVersion() const
{
    return VER_IJFrameKernel;
}

bool JFrameKernel::invoke(const char *method, int argc, ...)
{
    if (!method) {
        return false;
    }

    bool result = false;

    va_list ap;
    va_start(ap, argc);

    // ���һ����־
    if (strcmp(method, "log") == 0) {
        result = invokeLog(argc, ap);
    }
    // ��ʾ���
    else if (strcmp(method, "frame_show") == 0) {
        if (argc == 2) {
            // ת����ܺ���ϵͳ
            result = frameCore()->invoke("frame_show", 2, va_arg(ap, bool), va_arg(ap, bool));
        }
    }
    // �����˳���ܣ��첽��ʽ��
    else if (strcmp(method, "frame_try_exit") == 0) {
        // ת����ܲ���ϵͳ
        result = frameLayout()->invoke("frame_try_exit");
    }
    // �˳���ܣ��첽��ʽ��
    else if (strcmp(method, "frame_exit") == 0) {
        // ת����ܲ���ϵͳ
        result = frameLayout()->invoke("frame_exit");
    }
    // ������ܣ��첽��ʽ��
    else if (strcmp(method, "frame_restart") == 0) {
        // ת����ܲ���ϵͳ
        result = frameLayout()->invoke("frame_restart");
    }
    // ��¼���
    else if (strcmp(method, "frame_login") == 0) {
        // ת����ܵ�¼ϵͳ
        result = frameLogin()->invoke("frame_login");
    }
    // ע�����
    else if (strcmp(method, "frame_logout") == 0) {
        // ת����ܵ�¼ϵͳ
        result = frameLogin()->invoke("frame_logout");
    }
    // ����Qt��Ϣѭ��ϵͳ
    else if (strcmp(method, "run_q_app") == 0) {
        if (argc == 2) {
            // ת����ܲ���ϵͳ
            result = frameLayout()->invoke("run_q_app", 2,
                                           va_arg(ap, void*), va_arg(ap, int*));
        }
    }
    // ����Qt��Ϣѭ��ϵͳ
    else if (strcmp(method, "window_handle") == 0) {
        if (argc == 3) {
            // ת����ܲ���ϵͳ
            result = frameLayout()->invoke("window_handle", 3, va_arg(ap, void*),
                                           va_arg(ap, char*), va_arg(ap, long));
        }
    }
    // �������
    else if (strcmp(method, "frame_start") == 0) {
        //
        result = startFrame();
    }
    // ����Ĭ�Ͽ��ϵͳ
    else if (strcmp(method, "load_default_system") == 0) {
        // ת����ܲ���ϵͳ
        result = frameLayout()->invoke("load_default_system");
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

bool JFrameKernel::startFrame()
{
    bool result = true;

    // ��ʼ����ܺ���ϵͳ
    JFrameCore::getInstance();

    // ��ʼ����ܲ���ϵͳ
    JFrameLayout::getInstance();

    // ��ʼ����ܵ�¼����
    JFrameLogin::getInstance();

    // ���ؿ�ܺ���ϵͳ
    result = result && JFrameCore::getInstance()->attempter()->loadComponent();

    return result;
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
    // ������־������ʵ��
    if (data->logManager) {
        JFRAME_FACTORY_RELEASE(data->logManager, IJLogManager);
    }

    // ���ٿ�ܵ�¼����
    JFrameLogin::releaseInstance();

    // ���ٿ�ܲ��ֲ���
    JFrameLayout::releaseInstance();

    // ���ٿ�ܺ��Ĳ���
    JFrameCore::releaseInstance();

    // ����˽������ʵ��
    delete data;
}
