#ifndef JLOG_MANAGER_H
#define JLOG_MANAGER_H

#include "jframe_facade.h"

/** IJLogManager 接口描述 */
#define VER_IJLogManager J_INTERFACE_VERSION(1, 0)
#define IID_IJLogManager J_IID_INTERFACE(IJLogManager)

/**
 * @brief 日志管理器接口
 */
class IJLogManager : public IJUnknown
{
public:
    /**
     * @brief 消息类型
     */
    enum MsgType {
        EmergeMsg,      /**< emerge */
        FatalMsg,       /**< fatal */
        AlertMsg,       /**< alert */
        CriticalMsg,    /**< critical */
        ErrorMsg,       /**< error */
        WarningMsg,     /**< warning */
        NoticeMsg,      /**< notice */
        InfoMsg,        /**< info */
        DebugMsg        /**< debug */
    };

    /**
     * @brief 析构函数
     */
    virtual ~IJLogManager() {}

    /**
     * @brief 获取接口标识
     * @return 接口标识
     */
    virtual std::string interfaceIdentity() const { return IID_IJLogManager; }

    /**
     * @brief 获取接口版本
     * @return 接口版本
     */
    virtual unsigned int interfaceVersion() const { return VER_IJLogManager; }

    /**
     * @brief 获取日志配置文件路径
     * @return 日志配置文件路径
     */
    virtual std::string config() const = 0;

    /**
     * @brief 设置日志配置文件
     * @param filePath : 日志配置文件路径
     */
    virtual void setConfig(const std::string &filePath) = 0;

    /**
     * @brief logging : 输出一条日志
     * @param type : 消息类型
     * @param msg : 消息
     * @param where : 输出地（"all"、"file"、"console"）
     * @param argc : 可变参数个数
     * @param ... : 可变参数列表
     */
    virtual void logging(MsgType type, const std::string &msg, const std::string &where, int argc, ...) = 0;
};

#endif // JLOG_MANAGER_H
