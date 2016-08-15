#ifndef JLOG_MANAGER_H
#define JLOG_MANAGER_H

#include "jframe_facade.h"

// 接口描述
#define VER_IJLogManager J_INTERFACE_VERSION(1, 0)
#define IID_IJLogManager J_IID_INTERFACE(IJLogManager)

/**
 * @brief The IJLogManager class : 日志管理器接口
 */
class IJLogManager : public IJUnknown
{
public:
    /**
     * @brief The LogType enum : 日志类型
     */
    enum LogType {
        LogConsole, LogFile
    };

    /**
     * @brief The MsgType enum : 消息类型
     */
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

    /**
     * @brief ~IJLogManager : 析构函数
     */
    virtual ~IJLogManager() {}

    /**
     * @brief interfaceIdentity : 获取接口标识
     * @return : 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJLogManager; }

    /**
     * @brief interfaceVersion : 获取接口版本
     * @return : 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJLogManager; }

    /**
     * @brief logType : 获取日志类型
     * @return : 日志类型
     */
    virtual LogType logType() const = 0;

    /**
     * @brief setLogType : 设置日志类型
     * @param type : 日志类型
     */
    virtual void setLogType(LogType type) = 0;

    /**
     * @brief logging : 输出一条日志
     * @param msgType : 消息类型
     * @param msg : 消息
     * @param argc : 可变参数个数
     * @param ... : 可变参数列表
     */
    virtual void logging(MsgType msgType, const std::string &msg, int argc, ...) = 0;
};

#endif // JLOG_MANAGER_H
