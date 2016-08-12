#include "precomp.h"
#include "jframe_kernel_p.h"
#include "jlogmanager_p.h"
#include "qmfcapp.h"

// struct JFrameKernelData

struct JFrameKernelData
{
    JLogManagerPri *logManager;

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
            delete JFrameKernel::_instance;
            JFrameKernel::_instance = 0;
        }
        JFrameKernel::_instance_mutex.unlock();
    }
}

void JFrameKernel::releaseInterface()
{
    JFrameKernel::releaseInstance();
}

void *JFrameKernel::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);

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

bool JFrameKernel::invoke(const char *method, int argc)
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

    va_end(ap);

    return result;
}

IJLogManager *JFrameKernel::logManager()
{
    return data->logManager;
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
    data->logManager = new JLogManagerPri();
}

JFrameKernel::~JFrameKernel()
{
    // ������־������ʵ��
    if (data->logManager) {
        delete data->logManager;
        data->logManager = 0;
    }

    // ����˽������ʵ��
    delete data;
}
