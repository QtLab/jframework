#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include "factory/jnotifier.h"
#include "../jframe_layout_p.h"

// class ModuleManager

class IGF_Component;

class ModuleManager :
        public IJUnknown,
        public JObserver
{
public:
    explicit ModuleManager(JFrameLayout *frameLayout);
    ~ModuleManager();

    // IJUnknown interface
public:
    bool loadInterface();
    void releaseInterface();

public:
    // ���ؿ��ϵͳ
    bool loadSystem();

    // ��ȡ�����Ϣ
    JComponentInfo *componentByName(const QString &componentName);

    // ����������״̬
    bool isComponentAttached(const JComponentInfo *componentInfo);

    // �������
    bool attachComponent(JComponentInfo *componentInfo, bool show);

    // �����������
    bool attachComponentUi(const JComponentInfo *componentInfo);

    // �������
    bool detachComponent(JComponentInfo *componentInfo);

    // ��������ͼ������Ϊδ����״̬
    void inactivateAllComponent();

    // �����д���δ����״̬���������
    void detachAllInactivateComponent();

    // ��λ���д���δ����״̬����ͼ����ĸ�����
    void resetAllInactivateViewComponent();

    // ��ʾ���д��ڼ���״̬����ͼ����ĸ�����
    void showAllActivateViewComponent();

    //
    bool attachComponent(IJComponent *component, bool stayOn);
    JComponentInfo *attachComponent(IJComponent *component);
    bool detachComponent(IJComponent *component);
    bool attachComponentUi(IJComponent *component, QWidget *widget);

    // ��ȡ��ǰϵͳ����
    std::string currentSystem() const;

    // ��ȡ��ǰģʽ����
    std::string currentModule() const;

    // JObserver interface
public:
    virtual std::string observerId() const;

protected:
    // layout
    JLRESULT onPreviousSystemChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onPreviousModuleChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onLoadDefaultSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    // system
    JLRESULT onSwitchSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onSwitchModule(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onCurrentSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onCurrentModule(const std::string &id, JWPARAM wParam, JLPARAM lParam);

private:
    bool switchSystem(const std::string &system, JLPARAM lParam = 0);
    bool switchModule(const std::string &module, JLPARAM lParam = 0);

    // �����������
    void detachAllComponent();

private:
    JFrameLayout *q_frameLayout;
    INotifier *q_notifier;
    std::string q_currentSystem;
    std::string q_currentModule;
    QMap<QString/*componentName*/, JComponentInfo> q_mapComponentInfo;
};

#endif // MODULE_MANAGER_H
