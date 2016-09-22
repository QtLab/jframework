#ifndef JFRAME_CORE_H
#define JFRAME_CORE_H

#include "jframe_kernel.h"

/** �ӿڱ�ʶ */
#define VER_IJCommandSink J_INTERFACE_VERSION(1, 0)
#define IID_IJCommandSink J_IID_INTERFACE(IJCommandSink)

/**
 * @brief ������սӿ�
 */
class IJCommandSink
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJCommandSink() {}

    /**
     * @brief ����������Ϣ
     * @param [in] sender : ������ߣ�type: QObject *��
     * @param domain : ��������������� [componentName#...]
     * @param objectName : �����߶�������
     * @param eventType : �źš��¼�����
     * @param [in,out] data : ���ݡ�״̬����ʹ���ֲ�˵����
     * @return �ضϱ�־��true��ֹͣ�´���false�������´�
     */
    virtual bool commandSink(void *sender, const std::string &domain,
                             const std::string &objectName,
                             const std::string &eventType,
                             void *data) = 0;
};

/** �ӿڱ�ʶ */
#define VER_IJMessageSink J_INTERFACE_VERSION(1, 0)
#define IID_IJMessageSink J_IID_INTERFACE(IJMessageSink)

class IJComponent;

/**
 * @brief ��Ϣ���սӿ�
 */
class IJMessageSink
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJMessageSink() {}

    /**
     * @brief ������Ϣ
     * @param [in] sender : ��Ϣ������
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     * @return �ضϱ�־��true��ֹͣ�´���false�������´�
     */
    virtual bool messageSink(IJComponent *sender, const std::string &id, JWPARAM wParam, JLPARAM lParam) = 0;
};

/** �ӿڱ�ʶ */
#define VER_IJComponent J_INTERFACE_VERSION(1, 0)
#define IID_IJComponent J_IID_INTERFACE(IJComponent)

/**
 * @brief ����ӿ�
 */
class IJComponent : public IJUnknown
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJComponent() {}

    /**
     * @brief ��ȡ�������
     * @return �������
     */
    virtual std::string componentName() const = 0;

    /**
     * @brief ��ȡ�������
     * @return �������
     */
    virtual std::string componentDesc() const = 0;

    /**
     * @brief ��ȡ�������
     * @return �������
     */
    virtual std::string componentType() const { return "<unknown>"; }

    /**
     * @brief �������
     */
    virtual void attach() {}

    /**
     * @brief �������
     */
    virtual void detach() {}
};

/** �ӿڱ�ʶ */
#define VER_IJComponentUi J_INTERFACE_VERSION(1, 0)
#define IID_IJComponentUi J_IID_INTERFACE(IJComponentUi)

/**
 * @brief ������ڽӿ�
 */
class IJComponentUi
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJComponentUi() {}

    /**
     * @brief ��������
     * @param [in] parent : ������
     * @param objectName : Ŀ�괰������
     * @return Ŀ�괰��
     */
    virtual void *createWindow(void *parent, const std::string &objectName) = 0;
};

/** �ӿڱ�ʶ */
#define VER_IJMainWindow J_INTERFACE_VERSION(1, 0)
#define IID_IJMainWindow J_IID_INTERFACE(IJMainWindow)

/**
 * @brief ��������ڽӿ�
 */
class IJMainWindow : public IJUnknown
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJMainWindow() {}

    /**
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJMainWindow; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJMainWindow; }

    /**
     * @brief ���滯������
     */
    virtual void showNormal() = 0;

    /**
     * @brief ��С��������
     */
    virtual void showMinimized() = 0;

    /**
     * @brief ���������
     */
    virtual void showMaximized() = 0;

    /**
     * @brief ȫ����������
     */
    virtual void showFullScreen() = 0;

    /**
     * @brief �ر�������
     */
    virtual void closeWindow() = 0;

    /**
     * @brief ���������ڿɼ���
     * @param visible : �ɼ��Ա�־��true����ʾ�����ڣ�false������������
     */
    virtual void setVisible(bool visible) = 0;

    /**
     * @brief �ö�������
     * @param stayOnTop : �ö���־��true���ö���false��ȡ���ö�
     */
    virtual void showStaysOnTop(bool stayOnTop) = 0;

    /**
     * @brief �ı������ڴ�С
     * @param width : ���
     * @param height : �߶�
     */
    virtual void resize(int width, int height) = 0;

    /**
     * @brief ������ͼ
     * @param viewName : ��ͼ����
     */
    virtual void activeView(const std::string &viewName) = 0;

    /**
     * @brief ���¿�ʼ������Ϣ
     * @param info : ��Ϣ
     */
    virtual void updateSplashInfo(const std::string &info) = 0;

    /**
     * @brief �����������
     * @param [in] component : ���
     * @param filePath : ��������ļ�·��
     * @return ����״̬��true�������ɹ���false������ʧ��
     */
    virtual bool createComponentUi(IJComponent *component, const std::string &filePath) = 0;

    /**
     * @brief ��ȡ������
     * @return ������
     */
    virtual void *mainWidget() = 0;

    /**
     * @brief ���ÿ������
     * @param theme : ��������
     */
    virtual void setTheme(const std::string &theme) = 0;

    /**
     * @brief ��ȡ����������
     * @return ���������͡�1) "menuBar"; 2) "toolButton"; 3) "ribbonBar"
     */
    virtual std::string toolBarType() const = 0;

    /**
     * @brief ��ȡ��ͼ��������
     * @return ��ͼ�������͡�1) dynamic; 2) static
     */
    virtual std::string layoutType() const = 0;

    /**
     * @brief ��ѯָ������е��������ɵĶ���
     * @param objectName : ��������
     * @param componentName : ������� ���������Ϊ��ֵʱ������������е�һ����������ΪobjectName�Ķ���
     * @return ����ʵ��
     */
    virtual void *queryObject(const std::string &objectName, const std::string &componentName = "") = 0;

    /**
     * @brief ��ȡ�˵���
     * @return �˵���
     */
    virtual void *menuBar() = 0;

    /**
     * @brief ��ȡ ribbon ������
     * @return ribbon ������
     */
    virtual void *ribbonBar() = 0;

    /**
     * @brief ��ȡ״̬��
     * @return ״̬��
     */
    virtual void *statusBar() = 0;
};

/** �ӿڱ�ʶ */
#define VER_IJAttempter J_INTERFACE_VERSION(1, 0)
#define IID_IJAttempter J_IID_INTERFACE(IJAttempter)

//
#ifndef J_MESSAGESINK_CALLBACK
#define J_MESSAGESINK_CALLBACK
/** ��Ϣ���սӿں������� */
typedef JLRESULT (IJComponent::*JMsgSinkCb)
(IJComponent *sender, const std::string &id, JWPARAM wParam, JLPARAM lParam);
#endif // !J_MESSAGESINK_CALLBACK

class INotifier;

/**
 * @brief ��ܵ������ӿ�
 */
class IJAttempter : public IJUnknown
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJAttempter() {}

    /**
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJAttempter; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJAttempter; }

    /**
     * @brief �������
     * @return ����״̬��true�����سɹ���false������ʧ��
     */
    virtual bool loadComponent() = 0;

    /**
     * @brief ж�����
     */
    virtual void releaseComponent() = 0;

    /**
     * @brief ��ѯ���
     * @param componentName : �������
     * @return ���
     */
    virtual IJComponent *queryComponent(const std::string &componentName) = 0;

    /**
     * @brief ��ȡ��������ڽӿ�
     * @return ��������ڽӿ�
     */
    virtual IJMainWindow *mainWindow() = 0;

    /**
     * @brief ��ѯ�ӿ�
     * @param componentName : �������
     * @param iid : �ӿڱ�ʶ
     * @param ver : �ӿڰ汾
     * @return ��ѯ�ӿ�ʵ��
     */
    virtual void *queryInterface(const std::string &componentName, const std::string &iid, unsigned int ver) = 0;

    /**
     * @brief ��ȡ�������
     * @return �������
     */
    virtual std::list<IJComponent *> allComponents() const = 0;

    /**
     * @brief ��ȡ��ǰ����ģʽ��ʶ
     * @return ��ǰ����ģʽ��ʶ
     */
    virtual int currentWorkModeId() const = 0;

    /**
     * @brief ��ȡ��ǰ����ģʽ����
     * @return ��ǰ����ģʽ����
     */
    virtual std::string currentWorkModeName() const = 0;

    /**
     * @brief ��ȡ��ǰ����ģʽ�����ļ�������
     * @return ��ǰ����ģʽ�����ļ�������
     */
    virtual std::string currentWorkModeConfigDirName() const = 0;

    /**
     * @brief ��ȡ��Ϣ�ַ���
     * @return ��Ϣ�ַ���
     */
    virtual INotifier &notifier() = 0;

    /**
     * @brief ��ʼ��Ϣ������
     * @param [in] component : ���ʵ��
     * @return ����������
     */
    template<typename T> IJAttempter &beginGroup(T *component);

    /**
     * @brief ������Ϣ������
     */
    virtual void endGroup() = 0;

    /**
     * @brief ���������Ϣ����ѡ��ֱ�Ӱ�id��Ӧ����Ӧ������
     * @param id : ��Ϣ��ʶ
     * @param [in] cb : ��Ϣ��ʶ��Ӧ����Ӧ����
     * @return ����������
     */
    template<typename T>
    IJAttempter &subMessage(const std::string &id, JLRESULT (T::*cb)
                            (IJComponent *, const std::string &, JWPARAM, JLPARAM) = 0);

    /**
     * @brief ȡ�����������Ϣ
     * @param id : ��Ϣ��ʶ
     * @return ����������
     */
    virtual IJAttempter &unsubMessage(const std::string &id) = 0;

    /**
     * @brief ȡ��������Ϣ
     * @param [in] component : ���ʵ��
     */
    virtual void unsubMessage(IJComponent *component) = 0;

    /**
     * @brief ���������Ϣ��ͬ����
     * @param [in] component : ���
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     * @return ִ�н��
     */
    virtual JLRESULT sendMessage(IJComponent *component, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ���������Ϣ���첽��
     * @param [in] component : ���
     * @param id : ��Ϣ��ʶ
     * @param wParam : ����1
     * @param lParam : ����2
     */
    virtual void postMessage(IJComponent *component, const std::string &id, JWPARAM wParam = 0, JLPARAM lParam = 0) = 0;

    /**
     * @brief ���������Ϣ���첽��
     * @param [in] component : ���
     * @param id : ��Ϣ��ʶ
     * @param msg : ��Ϣ��Ϣ
     */
    virtual void postMessage(IJComponent *component, const std::string &id, const std::string &msg) = 0;

protected:
    /**
     * @brief ��ʼ��Ϣ������
     * @param [in] component : ���ʵ��
     * @param offset : reinterpret_cast<IJComponent *>(this) - static_cast<IJComponent *>(this)
     * @return ����������
     */
    virtual IJAttempter &beginGroup(IJComponent *component, int offset) = 0;

    /**
     * @brief ������Ϣ
     * @param id : ��Ϣ��ʶ
     * @param [in] cb : ��Ϣ��Ӧ������ַ
     * @return ����������
     */
    virtual IJAttempter &subMessage(const std::string &id, JMsgSinkCb cb) = 0;
};

template<typename T> inline
IJAttempter &IJAttempter::beginGroup(T *component)
{
    return beginGroup(component,
                  #ifdef _MSC_VER
                      (reinterpret_cast<IJComponent *>(component) - static_cast<IJComponent *>(component))
                  #elif defined(__unix__)
                      0
                  #else
                  #pragma message("not supported!"))
                      0
                  #endif
                      );
}

template<typename T> inline
IJAttempter &IJAttempter::subMessage(const std::string &id, JLRESULT (T::*cb)
                                     (IJComponent *, const std::string &, JWPARAM, JLPARAM))
{
    return subMessage(id, static_cast<JMsgSinkCb>(cb));
}

/** �ӿڱ�ʶ */
#define VER_IJFrameCore J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameCore J_IID_INTERFACE(IJFrameCore)

/**
 * @brief ��ܺ���ϵͳ�ӿ�
 */
class IJFrameCore : public IJUnknown
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJFrameCore() {}

    /**
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameCore; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameCore; }

    /**
     * @brief ��ȡ��ܵ�����
     * @return ��ܵ�����
     */
    virtual IJAttempter *attempter() = 0;
};

/** ��ȡ��ʼ��������������ӿڶ��� */
typedef bool (J_ATTR_CDECL *FuncInitComponent)(void);

/** ����������ʵ�����������ӿڶ��� */
typedef void *(J_ATTR_CDECL *FuncCreateAttempter)(void);

/** �������ʵ�����������ӿڶ��� */
typedef void *(J_ATTR_CDECL *FuncCreateComponent)(IJAttempter *attempter);

#endif // JFRAME_CORE_H
