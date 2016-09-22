#ifndef JLOG_MANAGER_H
#define JLOG_MANAGER_H

#include "jframe_facade.h"

/** IJLogManager �ӿ����� */
#define VER_IJLogManager J_INTERFACE_VERSION(1, 0)
#define IID_IJLogManager J_IID_INTERFACE(IJLogManager)

/**
 * @brief ��־�������ӿ�
 */
class IJLogManager : public IJUnknown
{
public:
    /**
     * @brief ��Ϣ����
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
     * @brief ��������
     */
    virtual ~IJLogManager() {}

    /**
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJLogManager; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJLogManager; }

    /**
     * @brief ��ȡ��־�����ļ�·��
     * @return ��־�����ļ�·��
     */
    virtual std::string config() const = 0;

    /**
     * @brief ������־�����ļ�
     * @param filePath : ��־�����ļ�·��
     */
    virtual void setConfig(const std::string &filePath) = 0;

    /**
     * @brief ���һ����־
     * @param type : ��Ϣ����
     * @param msg : ��Ϣ
     * @param where : ����أ�"all"��"file"��"console"��
     * @param argc : �ɱ��������
     * @param ... : �ɱ�����б�
     */
    virtual void logging(MsgType type, const std::string &msg, const std::string &where, int argc, ...) = 0;
};

#endif // JLOG_MANAGER_H
