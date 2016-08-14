#include "precomp.h"
#include "module_manager.h"
#include "../jframe_layout_p.h"
#include "../mainview_manager/mainview_manager.h"
#include "../layout_manager/layout_manager.h"
#include "../../../jframe_core.h"

// class ModuleManager

ModuleManager::ModuleManager(JFrameLayout *frameLayout) :
    q_frameLayout(frameLayout),
    q_notifier(frameLayout->notifier())
{
}

ModuleManager::~ModuleManager()
{
    // ȡ������
    q_notifier->pop(this);
}

bool ModuleManager::init()
{
    // ������Ϣ
    q_notifier->begin(this)
            // layout
            .push("j_previous_system_changed", &ModuleManager::onPreviousSystemChanged)
            .push("j_previous_module_changed", &ModuleManager::onPreviousModuleChanged)
            .push("j_load_default_system", &ModuleManager::onLoadDefaultSystem)
            // system
            .push("j_switch_system", &ModuleManager::onSwitchSystem)
            .push("j_switch_module", &ModuleManager::onSwitchModule)
            .push("j_current_system", &ModuleManager::onCurrentSystem)
            .push("j_current_module", &ModuleManager::onCurrentModule)
            .end();

    return true;
}

bool ModuleManager::loadSystem()
{
    // ����Ĭ�ϲ��֣����������ʼ����
    if (!q_frameLayout->layoutManager()->updateLayout()) {
        return false;   // ����ʧ��
    }

    // �첽֪ͨģʽ�Ѿ��л�
    q_notifier->post("j_module_changed", q_currentModule);

    return true;
}

JComponentInfo *ModuleManager::componentById(const QString &componentId)
{
    QMap<QString, JComponentInfo>::iterator iter =
            q_mapComponentInfo.find(componentId);
    if (iter != q_mapComponentInfo.end()) {
        return &iter.value();
    }

    return 0;   // δ�ҵ�
}

bool ModuleManager::isComponentAttached(const JComponentInfo *componentInfo)
{
    if (!componentInfo) {
        return false;
    }

    return componentInfo->attached;
}

bool ModuleManager::attachComponent(const JComponentInfo *componentInfo, bool show)
{
    //
    if (!componentInfo) {
        return false;
    }

    // ��ѯ����Ƿ��ѹ���
    if (componentInfo->attached) {
        return true;
    }

    //
    IJComponent *component = dynamic_cast<IJComponent *>(componentInfo->iface);
    if (!component) {
        return false;   //
    }

    // �������
    component->attach();

    // ���������Ч������ʾ����
    if (componentInfo->widget && show) {
        componentInfo->widget->show();
    }

    return true;
}

bool ModuleManager::attachComponentUi(const JComponentInfo *componentInfo)
{
    if (!componentInfo) {
        return false;
    }

    // ��ѯ��������Ƿ��ѹ���
    if (componentInfo->widget) {
        return true;
    }

    // �״μ��أ��봴�����棨������ڽ���ӿڣ�
    IJComponentUi *componentUi = (IJComponentUi *)
            (dynamic_cast<IJComponent *>(componentInfo->iface)
             ->queryInterface(IID_IJComponentUi, VER_IJComponentUi));
    if (componentUi) {
        componentUi->createUi(q_frameLayout->mainViewManager(), "");
    }

    return true;
}

bool ModuleManager::detachComponent(const JComponentInfo *componentInfo)
{
    if (!componentInfo) {
        return false;
    }

    // ��ѯ����Ƿ��ѷ���
    if (!componentInfo->attached) {
        return true;
    }

    //
    IJComponent *component = dynamic_cast<IJComponent *>(componentInfo->iface);
    if (!component) {
        return false;
    }

    // �������
    component->detach();

    return true;
}

void ModuleManager::inactivateAllComponent()
{
    QMutableMapIterator<QString, JComponentInfo> iter(q_mapComponentInfo);
    while (iter.hasNext()) {
        iter.next();
        JComponentInfo &componentInfo = iter.value();
        if (!componentInfo.stayOn) {
            //
            componentInfo.active = false;
            //
            if (componentInfo.widget && !componentInfo.isView
                    && componentInfo.widget->isVisible()) {
                componentInfo.widget->hide();
            }
        }
    }
}

void ModuleManager::detachAllInactivateComponent()
{
    QMutableMapIterator<QString, JComponentInfo> iter(q_mapComponentInfo);
    while (iter.hasNext()) {
        iter.next();
        JComponentInfo &componentInfo = iter.value();
        if (!componentInfo.active) {
            detachComponent(&componentInfo);
        }
    }
}

void ModuleManager::resetAllInactivateViewComponent()
{
    QMutableMapIterator<QString, JComponentInfo> iter(q_mapComponentInfo);
    while (iter.hasNext()) {
        iter.next();
        JComponentInfo &componentInfo = iter.value();
        if (!componentInfo.active
                && componentInfo.isView
                && componentInfo.widget) {
            componentInfo.widget->setParent(q_frameLayout->mainWindow());
        }
    }
}

bool ModuleManager::attachComponent(IJComponent *component, bool stayOn)
{
    JComponentInfo *componentInfo = attachComponent(component);
    if (!componentInfo) {
        return false;
    }

    //
    if (!componentInfo->stayOn) {
        componentInfo->stayOn = stayOn;
        if (stayOn) {
            componentInfo->attached = true;     // ����Ϊ����״̬
            componentInfo->active = true;       // �������
        }
    }

    return true;
}

JComponentInfo *ModuleManager::attachComponent(IJComponent *component)
{
    // �������
    if (!component) {
        return 0;
    }

    //
    JComponentInfo *componentInfo = 0;

    // ���ã�����£����������Ϣ
    const QString sComponentId = QString::fromStdString(component->componentId());
    QMap<QString, JComponentInfo>::iterator iterComponentInfo =
            q_mapComponentInfo.find(sComponentId);
    if (iterComponentInfo == q_mapComponentInfo.end()) {
        // ��������û�й��ع���������µ������Ϣ�����ң����ι�������activeΪδ����״̬����false��
        JComponentInfo info;
        info.iface = component;
        // �״μ��أ���������
        //component->shutdown();
        //
        q_mapComponentInfo.insert(sComponentId, info);
        //
        componentInfo = &q_mapComponentInfo[sComponentId];
    } else {
        // �������������Ϣ
        componentInfo = &iterComponentInfo.value();
        //
        if (!componentInfo->stayOn && componentInfo->attached) {
            Q_ASSERT(false);    // ������ĵ���״̬
            return 0;           // ͬ�����������ظ����أ����ܻ�����ͻ�������Դ�ظ���γ�ʼ�������ڴ�й©������
        }
        //
        if (componentInfo->firstAttach) {
            componentInfo->firstAttach = false;
        } else {
            componentInfo->attached = true;     // ����Ϊ����״̬
        }
        // ���ü����ʶ
        componentInfo->active = true;           // �������
    }

    return componentInfo;
}

bool ModuleManager::detachComponent(IJComponent *component)
{
    // �������
    if (!component) {
        return false;
    }

    // ���������Ϣ��Ч�Լ��
    QMap<QString, JComponentInfo>::iterator iterComponentInfo =
            q_mapComponentInfo.find(QString::fromStdString(component->componentId()));
    if (iterComponentInfo == q_mapComponentInfo.end()) {
        return false;
    }

    // ���ã����£����������Ϣ
    JComponentInfo &componentInfo = iterComponentInfo.value();
    // ����Ϊ����״̬
    componentInfo.attached = false;
    // ���ü����ʶ
    componentInfo.active = false;       // ȡ���������
    // ���������Ч��������Ϊ���ɼ�
    if (componentInfo.widget) {
        componentInfo.widget->hide();
    }

    return true;
}

bool ModuleManager::attachComponentUi(IJComponent *component, QWidget *widget)
{
    // �������
    if (!component || !widget) {
        return false;
    }

    // ���������Ϣ��Ч�Լ��
    const QString sComponentId = QString::fromStdString(component->componentId());
    QMap<QString, JComponentInfo>::iterator iterComponentInfo =
            q_mapComponentInfo.find(sComponentId);
    if (iterComponentInfo == q_mapComponentInfo.end()) {
        // ��������û�й��ع���������µ������Ϣ�������������GF��������ļ���������Ĵ�����Ӧ������Ϊ����״̬������ʼ����ʾ��
        JComponentInfo componentInfo;
        componentInfo.iface = component;
        // �洢����ʵ��
        componentInfo.widget = widget;
        // ����Ϊ����״̬
        componentInfo.attached = true;
        // ���ü����ʶ
        componentInfo.active = true;        // �������
        // ����Ϊʼ�չ���
        componentInfo.stayOn = true;
        // ���ӵ�ӳ�����
        q_mapComponentInfo.insert(sComponentId, componentInfo);
    } else {
        JComponentInfo &componentInfo = iterComponentInfo.value();
        if (componentInfo.widget) {
            Q_ASSERT(false);    // ������ĵ���״̬
            return false;       // �Ѿ������ĵ�ǰ�������ʵ������Ϊ�������
        }
        // �洢����ʵ��
        componentInfo.widget = widget;
        // ��������Ϊ����״̬
        componentInfo.attached = false;
        // ��������Ϊ���ɼ�
        componentInfo.widget->hide();
    }

    return true;
}

std::list<IJComponent *> ModuleManager::attachedComponents() const
{
    std::list<IJComponent *> attacheds;
    QMapIterator<QString, JComponentInfo> citer(q_mapComponentInfo);
    while (citer.hasNext()) {
        citer.next();
        if (citer.value().attached) {
            attacheds.push_back(dynamic_cast<IJComponent *>(citer.value().iface));
        }
    }

    return attacheds;
}

std::string ModuleManager::currentSystem() const
{
    return q_currentSystem;
}

std::string ModuleManager::currentModule() const
{
    return q_currentModule;
}

std::string ModuleManager::jobserverId() const
{
    return "jlayout.module_manager";
}

JLRESULT ModuleManager::onPreviousSystemChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(lParam);

    // ��ԭ����
    std::string *pSystem = (std::string *)wParam;
    if (!pSystem) {
        return -1;
    }

    // ����ϵͳ
    q_currentSystem = *pSystem;

    return 0;
}

JLRESULT ModuleManager::onPreviousModuleChanged(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(lParam);

    // ��ԭ����
    std::string *pModule = (std::string *)wParam;
    if (!pModule) {
        return -1;
    }

    // ����ģʽ
    q_currentModule = *pModule;

    return 0;
}

JLRESULT ModuleManager::onLoadDefaultSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);

    // ���ص�¼��Ϣ
    if (!q_frameLayout->loadLoginInfo()) {
        return -1;
    }

    // ����ϵͳ
    if (!loadSystem()) {
        return -1;
    }

    return 0;
}

JLRESULT ModuleManager::onSwitchSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);

    //
    if (jloginManager()->isAdminUser()) {
        return 0;   // ����ǹ���Ա��¼�������ϵͳ�л�
    }

    //
    std::string *pCurrentSystem = (std::string *)wParam;
    if (!pCurrentSystem) {
        return -1;
    }

    // �л�ϵͳ
    switchSystem(*pCurrentSystem, lParam);

    return 0;
}

JLRESULT ModuleManager::onSwitchModule(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);

    //
    if (jloginManager()->isAdminUser()) {
        return 0;   // ����ǹ���Ա��¼�������ϵͳ�л�
    }

    //
    std::string *pCurrentModule = (std::string *)wParam;
    if (!pCurrentModule) {
        return -1;
    }

    // �л�ϵͳ
    switchModule(*pCurrentModule, lParam);

    return 0;
}

JLRESULT ModuleManager::onCurrentSystem(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(lParam);

    //
    std::string *pCurrentSystem = (std::string *)wParam;
    if (!pCurrentSystem) {
        return -1;
    }

    //
    *pCurrentSystem = q_currentSystem;

    return 0;
}

JLRESULT ModuleManager::onCurrentModule(const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(id);
    Q_UNUSED(lParam);

    //
    std::string *pCurrentModule = (std::string *)wParam;
    if (!pCurrentModule) {
        return -1;
    }

    //
    *pCurrentModule = q_currentModule;

    return 0;
}

bool ModuleManager::switchSystem(const std::string &system, JLPARAM lParam)
{
    // �������
    if (system.empty()) {
        jframeLogCrit(QStringLiteral("������⣺ϵͳ���Ʋ���Ϊ��ֵ��"));
        return false;
    }

    //
    if (q_currentSystem == system) {
        return true;        // ϵͳû�䣬����Ҫ�л�
    }

    // ����ϵͳ�л�ҵ��
    bool result = false;    // �ݲ�֧�������л�ϵͳ�������������������ڼ䶯̬�л�ϵͳ
    if (!result) {
        return false;       // ϵͳ�л�ʧ��
    }

    // �첽֪ͨϵͳ�Ѿ��л�
    q_notifier->post("j_system_changed", system, lParam);

    return true;
}

bool ModuleManager::switchModule(const std::string &module, JLPARAM lParam)
{
    // �������
    if (module.empty()) {
        jframeLogCrit(QStringLiteral("������⣺ģʽ���Ʋ���Ϊ��ֵ��"));
        return false;
    }

    // ������񻯣�ȥ����ģʽ֮��Ŀո񣬱�����ȱȽ�
    std::string specModule = QString::fromStdString(module)
            .split(">>", QString::SkipEmptyParts).join(">>").toStdString();
    if (q_currentModule == specModule) {
        return true;    // ģʽû�䣬����Ҫ�л�
    }

    // ����ϵͳ�л�ҵ��
    bool result = q_frameLayout->layoutManager()
            ->updateLayout(QString::fromStdString(specModule));
    if (!result) {
        return false;       // ϵͳ�л�ʧ��
    }

    // �첽֪ͨģʽ�Ѿ��л�
    q_notifier->post("j_module_changed", specModule, lParam);

    return true;
}
