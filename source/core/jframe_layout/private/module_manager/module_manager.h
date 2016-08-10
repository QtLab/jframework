#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <QWidget>
#include "kernel/jnotifier.h"
#include "../jframe_layout_p.h"

// class ModuleManager

class IGF_Component;

class ModuleManager : public JObserver
{
public:
    explicit ModuleManager(JFrameLayout *frameLayout);
    ~ModuleManager();

    // ��ʼ��ģ��
    bool init();

    // ���ؿ��ϵͳ
    bool loadSystem();

    // ��ȡ�����Ϣ
    JComponentInfo *componentById(const QString &componentId);

    // ����������״̬
    bool isComponentAttached(const JComponentInfo *componentInfo);

    // �������
    bool attachComponent(const JComponentInfo *componentInfo, bool show);

    // �����������
    bool attachComponentUi(const JComponentInfo *componentInfo);

    // �������
    bool detachComponent(const JComponentInfo *componentInfo);

    // ��������ͼ������Ϊδ����״̬
    void inactivateAllComponent();

    // �����д���δ����״̬���������
    void detachAllInactivateComponent();

    //
    bool attachComponent(IGF_Component *component, bool stayOn);
    JComponentInfo *attachComponent(IGF_Component *component);
    bool detachComponent(IGF_Component *component);
    bool attachComponentUi(IGF_Component *component, QWidget *widget);
    std::list<IGF_Component *> attachedComponents() const;

    // ��ȡ��ǰϵͳ����
    std::string currentSystem() const;

    // ��ȡ��ǰģʽ����
    std::string currentModule() const;

    // JObserver interface
public:
    virtual std::string jobserverId() const;

protected:
    // layout
    JLRESULT onPreviousSystemChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onPreciousModuleChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onLoadDefaultSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    // system
    JLRESULT onSwitchSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onSwitchModule(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onCurrentSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam);
    JLRESULT onCurrentModule(const std::string &id, JWPARAM wParam, JLPARAM lParam);

private:
    bool switchSystem(const std::string &system, JLPARAM lParam = 0);
    bool switchModule(const std::string &module, JLPARAM lParam = 0);

private:
    JFrameLayout *q_frameLayout;
    INotifier *q_notifier;
    std::string q_currentSystem;
    std::string q_currentModule;
    QMap<QString/*componentId*/, JComponentInfo> q_mapComponentInfo;
};

#endif // MODULE_MANAGER_H
