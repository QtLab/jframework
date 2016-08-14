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
            JFrameCore::_instance_mutex.unlock();
            delete JFrameCore::_instance;
            JFrameCore::_instance_mutex.lock();
            JFrameCore::_instance = 0;
        }
        JFrameCore::_instance_mutex.unlock();
    }
}

void JFrameCore::releaseInterface()
{
    // 释放框架调度器
    if (data->attempter) {
        data->attempter->releaseInterface();
    }

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

    // 显示或隐藏框架主窗口
    if (strcmp(method, "frame_show") == 0) {
        result = invokeShowFrame(argc, ap);
    }

    va_end(ap);

    return result;
}

IJAttempter *JFrameCore::attempter()
{
    return data->attempter;
}

bool JFrameCore::invokeShowFrame(int argc, va_list ap)
{
    // 参数有效性检测
    if (argc != 2) {
        return false;
    }

    //
    if (!data->attempter || !data->attempter->mainWindow()) {
        return false;
    }

    //
    bool show = va_arg(ap, bool);
    bool maximized = va_arg(ap, bool);

    //
    if (show) {
        if (maximized) {
            data->attempter->mainWindow()->showMaximized();
        } else {
            data->attempter->mainWindow()->setVisible(true);
        }
    } else {
        data->attempter->mainWindow()->setVisible(false);
    }

    return true;
}

JFrameCore::JFrameCore()
{
    data = new JFrameCoreData;

    //
    data->attempter = new JAttempter();

    //
    bool result = true;

    //
    result = result && dynamic_cast<JAttempter *>(data->attempter)->init();

    // 结果检测
    if (!result) {
        //
    }
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
