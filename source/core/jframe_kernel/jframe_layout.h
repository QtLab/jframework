#ifndef JFRAME_LAYOUT_H
#define JFRAME_LAYOUT_H

#include "jframe_kernel.h"
#include <list>

// �ӿڱ�ʶ
#define VER_IJFrameLayout J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameLayout J_IID_INTERFACE(IJFrameLayout)

class INotifier;
class IJComponent;
class IJAttempter;
class QWidget;

/**
 * @brief The IJFrameLayout class
 */
class IJFrameLayout : public IJUnknown
{
public:
    /**
     * @brief ��־����ö��
     */
    enum LogType {
        LogConsole,     /**< ����־�����Output���ն� */
        LogFile         /**< ����־������ļ��� */
    };

    /**
     * @brief ~IJFrameLayout : ��������
     */
    virtual ~IJFrameLayout() {}

    /**
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameLayout; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameLayout; }

    /**
     * @brief ��ȡ���������
     * @return ���������
     */
    virtual QWidget *mainWindow() = 0;

    /**
     * @brief ��ȡ�������ͼ
     * @return �������ͼ
     */
    virtual QWidget *mainView() = 0;

    /**
     * @brief ��ȡ�����Ϣ�ַ���
     * @return �����Ϣ�ַ���
     */
    virtual INotifier &notifier() = 0;

    /**
     * @brief ��ȡ��ܵ�����
     * @return ��ܵ�����
     */
    virtual IJAttempter *attempter() = 0;

    /**
     * @brief ���ÿ�ܷ������
     * @param theme : ��������
     */
    virtual void setFrameTheme(const std::string &theme) = 0;

    /**
     * @brief ��ȡ�����Ȩ�޵ȼ�
     * @param componentName : �������
     * @return ���Ȩ�޵ȼ����鿴jframe_login.h�е�[PowerLevel]ö�ٶ���
     */
    virtual int componentPowerLevel(const std::string &componentName) const = 0;

    /**
     * @brief ��ȡ��ǰϵͳ����
     * @return ��ǰϵͳ����
     */
    virtual std::string currentSystem() const = 0;

    /**
     * @brief ��ȡ��ǰģʽ����
     * @return ��ǰģʽ����
     */
    virtual std::string currentModule() const = 0;
};

#endif // JFRAME_LAYOUT_H
