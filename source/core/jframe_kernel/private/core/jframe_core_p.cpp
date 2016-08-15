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

std::string JFrameCore::interfaceIdentity() const
{
    return IID_IJFrameCore;
}

unsigned int JFrameCore::interfaceVersion() const
{
    return VER_IJFrameCore;
}

void *JFrameCore::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);

    return 0;
}

bool JFrameCore::loadInterface()
{
    bool result = true;

    // 加载框架调度器
    result = result && data->attempter->loadInterface();

    return result;
}

void JFrameCore::releaseInterface()
{
    // 释放框架调度器
    if (data->attempter) {
        data->attempter->releaseInterface();
    }
}

std::list<std::string> JFrameCore::queryMethod() const
{
    std::list<std::string> methods;

    // frame_show
    methods.push_back(std::string("frame_show").append("..."));

    return methods;
}

bool JFrameCore::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    // 显示或隐藏框架主窗口
    if (method == "frame_show") {
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
