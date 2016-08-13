#include "precomp.h"
#include "jframe_core_p.h"
#include "jattempter.h"

// struct JFrameCoreData

struct JFrameCoreData
{
    IJAttempter *attempter;

    JFrameCoreData()
        : attempter(0)
    {

    }
};

// class JFrameCore

//
QMutex JFrameCore::_instance_mutex;
JFrameCore* JFrameCore::_instance = 0;

JFrameCore* JFrameCore::getInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameCore::_instance == 0) {
        JFrameCore::_instance_mutex.lock();
        if (JFrameCore::_instance == 0) {
            JFrameCore* tempInstance = new JFrameCore;
            JFrameCore::_instance = tempInstance;
        }
        JFrameCore::_instance_mutex.unlock();
    }

    return JFrameCore::_instance;
}

void JFrameCore::releaseInstance()
{
    // 双重检测（防止多线程竞争）
    if (JFrameCore::_instance != 0) {
        JFrameCore::_instance_mutex.lock();
        if (JFrameCore::_instance != 0) {
            delete JFrameCore::_instance;
            JFrameCore::_instance = 0;
        }
        JFrameCore::_instance_mutex.unlock();
    }
}

void JFrameCore::releaseInterface()
{
    JFrameCore::releaseInstance();
}

void *JFrameCore::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);

    return 0;
}

std::string JFrameCore::objectIdentity() const
{
    return IID_IJFrameCore;
}

unsigned int JFrameCore::objectVersion() const
{
    return VER_IJFrameCore;
}

bool JFrameCore::invoke(const char *method, int argc, ...)
{
    if (!method) {
        return false;
    }

    bool result = false;
    va_list ap;
    va_start(ap, argc);

    va_end(ap);

    return result;
}

bool JFrameCore::initialize()
{
    //
    data->attempter->mainWindow()->initialize();

    return true;
}

IJAttempter *JFrameCore::attempter()
{
    return data->attempter;
}

JFrameCore::JFrameCore()
{
    data = new JFrameCoreData;

    //
    JAttempter *attempter = new JAttempter();
    if (!attempter->init()) {
        //
    }
    data->attempter = attempter;
}

JFrameCore::~JFrameCore()
{
    //
    if (data->attempter) {
        delete data->attempter;
        data->attempter = 0;
    }

    delete data;
}
