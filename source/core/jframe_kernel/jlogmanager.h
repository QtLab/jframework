#ifndef JLOG_MANAGER_H
#define JLOG_MANAGER_H

#include <string>
#include "factory/jframe_interface.h"

// 日志管理模块

class IJLogManager
{
public:
    enum LogType {
        LogConsole, LogFile
    };

    enum MsgType {
        EmergeMsg,
        FatalMsg,
        AlertMsg,
        CriticalMsg,
        ErrorMsg,
        WarningMsg,
        NoticeMsg,
        InfoMsg,
        DebugMsg
    };

    virtual ~IJLogManager() {}

    // 获取日志类型
    virtual LogType logType() const = 0;

    // 设置日志类型
    virtual void setLogType(LogType type) = 0;

    // 记录一条日志
    virtual void logging(MsgType msgType, const std::string &msg, int argc, ...) = 0;
};

// 接口描述
#define VER_IJLogManager J_INTERFACE_VERSION(1, 0)
#define IID_IJLogManager J_IID_INTERFACE(IJLogManager)

#endif // JLOG_MANAGER_H
