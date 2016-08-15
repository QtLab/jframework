#include "precomp.h"
#include "layout_manager.h"
#include "../jframe_layout_p.h"
#include "../module_manager/module_manager.h"
#include "../mainview_manager/mainview_manager.h"
#include "../../../jframe_core.h"

// class LayoutManager

LayoutManager::LayoutManager(JFrameLayout *frameLayout)
    : q_frameLayout(frameLayout)
    , q_notifier(frameLayout->notifier())
    , q_mainSplitter(0)
{

}

LayoutManager::~LayoutManager()
{

}

bool LayoutManager::loadInterface()
{
    /// ����Ĭ������

    // splitter
    q_layoutConfig.splitter.width = 8;
    q_layoutConfig.splitter.checkable = true;
    q_layoutConfig.splitter.opaqueResize = false;
    q_layoutConfig.splitter.color = QColor(64, 64, 64, 255);
    q_layoutConfig.splitter.syncScales = true;
    // switch
    q_layoutConfig.sw.remember = true;  // Ĭ���������ģʽ�л����

    //
    bool result = true;

    // ���ز���������Ϣ
    result = result && loadLayoutConfig();

    return result;
}

void LayoutManager::releaseInterface()
{
}

JLayoutConfig LayoutManager::layoutConfig() const
{
    return q_layoutConfig;
}

bool LayoutManager::updateLayout(const QString &section)
{
    // �������������Ϊδ����״̬
    q_frameLayout->moduleManager()->inactivateAllComponent();

    // �򿪿�ܲ��������ļ�
    QFile file(QString::fromStdString(jframeFacade()->frameLayoutPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("��ܲ��������ļ�\"%1\"�����ڣ�")
                .arg(file.fileName());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), text);
        return false;   // �ļ�������
    }

    // ���ļ�
    if (!file.open(QFile::ReadOnly)) {
        return false;   // ��ʧ��
    }

    // �����ļ�
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("��ܲ��������ļ�\"%1\"����ʧ�ܣ�\n"
                                            "����������%2\n"
                                            "����λ�ã����кţ�%3���кţ�%4��")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), text);
        return false;
    }

    // �ر��ļ�
    file.close();

    // ��ȡ���ڵ�
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // ��ȡʧ��
    }

    // ����ϵͳ״̬
    QDomElement emSystem, emModule;
    if (!parseSystemStatus(emRoot, section, emSystem, emModule)) {
        return false;
    }

    // ����ģʽ�ڵ�
    if (!loadModuleNode(emModule)) {
        return false;
    }

    // load popup node (global popup plugin within current system <after module loaded>)
    emModule = emSystem.firstChildElement("module");
    if (!emModule.isNull()) {
        QDomElement emPopup = emModule.nextSiblingElement("popup");
        if (!emPopup.isNull()) {
            if (!loadModulePopupNode(emPopup)) {
                //return false; // load failure
            }
        }
    }

    // �����޸�
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;   // ��ʧ��
    }
    QTextStream textStream(&file);
    document.save(textStream, 2);

    // �����д���δ����״̬���������
    q_frameLayout->moduleManager()->detachAllInactivateComponent();

    return true;
}

bool LayoutManager::loadLayoutConfig()
{
    // ������ܲ��������ļ�
    QFile file(QString::fromStdString(jframeFacade()->frameLayoutPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("��ܲ��������ļ�\"%1\"�����ڣ�")
                .arg(file.fileName());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), text);
        return false;   // �ļ�������
    }

    // ���ļ�
    if (!file.open(QFile::ReadOnly)) {
        return false;   // ��ʧ��
    }

    // �����ļ�
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("��ܲ��������ļ�\"%1\"����ʧ�ܣ�"
                                            "����������%2\n"
                                            "����λ�ã����кţ�%3���кţ�%4��")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), text);
        return false;
    }

    // �ر��ļ�
    file.close();

    // ��ȡ���ڵ�
    const QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // ��Ч
    }

    // ��ȡ���ýڵ�
    const QDomElement emConfig = emRoot.firstChildElement("config");
    if (emConfig.isNull()) {
        return true;    // û�����������
    }

    // splitter
    const QDomElement emSplitter = emConfig.firstChildElement("splitter");
    if (!emSplitter.isNull()) {
        // width
        if (emSplitter.hasAttribute("width")) {
            q_layoutConfig.splitter.width = emSplitter.attribute("width", "8").toInt();
        }
        // checkable
        if (emSplitter.hasAttribute("checkable")) {
            q_layoutConfig.splitter.checkable =
                    QVariant(emSplitter.attribute("checkable")).toBool();
        }
        // opaqueResize
        if (emSplitter.hasAttribute("opaqueResize")) {
            q_layoutConfig.splitter.opaqueResize =
                    QVariant(emSplitter.attribute("opaqueResize")).toBool();
        }
        // color
        QColor color;
        if (emSplitter.hasAttribute("color")) {
            color = QColor(emSplitter.attribute("color"));
        }
        if (!color.isValid()) {
            color = q_layoutConfig.splitter.color;
        }
        // opacity
        if (emSplitter.hasAttribute("opacity")) {
            const QString sOpacity = emSplitter.attribute("opacity");
            if (sOpacity.contains('.')) {
                color.setAlphaF(sOpacity.toDouble());
            } else {
                color.setAlpha(sOpacity.toUInt());
            }
        }
        //
        q_layoutConfig.splitter.color = color;
        // syncScales
        if (emSplitter.hasAttribute("syncScales")) {
            q_layoutConfig.splitter.syncScales =
                    QVariant(emSplitter.attribute("syncScales")).toBool();
        }
    }

    // switch
    const QDomElement emSwitch = emConfig.firstChildElement("switch");
    if (!emSwitch.isNull()) {
        // remember
        if (emSwitch.hasAttribute("remember")) {
            q_layoutConfig.sw.remember = QVariant(emSwitch.attribute("remember")).toBool();
        }
    }

    return true;
}

bool LayoutManager::resetModuleElement()
{
    // �򿪿�ܲ��������ļ�
    QFile file(QString::fromStdString(jframeFacade()->frameLayoutPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("��ܲ��������ļ�\"%1\"�����ڣ�")
                .arg(file.fileName());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), text);
        return false;   // �ļ�������
    }

    // ���ļ�
    if (!file.open(QFile::ReadWrite)) {
        return false;   // ��ʧ��
    }

    // �����ļ�
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("��ܲ��������ļ�\"%1\"����ʧ�ܣ�\n"
                                            "����������%2\n"
                                            "����λ�ã����кţ�%3���кţ�%4��")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), text);
        return false;
    }

    // �ر��ļ�
    file.close();

    // ��ȡ���ڵ�
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // ��Ч
    }

    // system - module
    for (QDomElement emSystem = emRoot.firstChildElement("system");
         !emSystem.isNull();
         emSystem = emSystem.nextSiblingElement("system")) {
        //
        for (QDomElement emModule = emSystem.firstChildElement("module");
             !emModule.isNull();
             emModule = emModule.nextSiblingElement("module")) {
            //
            resetModuleElement();
        }
    }

    return true;
}

void LayoutManager::resetModuleElement(QDomElement emModule)
{
    // �������
    if (emModule.isNull()) {
        return;   // ��Ч
    }

    // ��λ״̬
    if (emModule.nodeName() == "module") {
        emModule.setAttribute("submodule", "");
    }

    // sub-module
    for (QDomElement emSubModule = emModule.firstChildElement("module");
         !emSubModule.isNull();
         emSubModule = emSubModule.nextSiblingElement("module")) {
        //
        resetModuleElement(emSubModule);
    }
}

QString LayoutManager::currentConfigModule(const QDomElement &emSystem)
{
    // �������
    if (emSystem.isNull()) {
        return QString::null;   // ��Ч
    }

    // ��ȡϯλ-ģʽӳ��ڵ�
    const QDomElement emSeatModule = emSystem.firstChildElement("seat-module");
    if (emSeatModule.isNull()) {
        return QString::null;   // ��Ч
    }

    //
    const QString lastModule = emSystem.attribute("module");

    // ����ָ��д��ϯλ-ģʽӳ��ڵ�
    QDomElement emItem;
    QDomElement emFirstMatch;
    for (emItem = emSeatModule.firstChildElement("item");
         !emItem.isNull();
         emItem = emItem.nextSiblingElement("item")) {
        // ƥ�䵱ǰϯλ
#if 0
        if (emItem.attribute("seat").toStdString()
                == q_frameLayout->powerInfo().seatElement.sName) {
            // ƥ��ϵͳ
            if (emFirstMatch.isNull()) {
                emFirstMatch = emItem;
            }
            // ƥ��ģʽ
            if (emItem.attribute("module") == lastModule) {
                break;  // ƥ��ɹ�
            }
        }
#else
        break;
#endif
    }

    //
    if (emItem.isNull()) {
        if (emFirstMatch.isNull()) {
            return QString::null;   // δ�ҵ�ƥ���ϵͳ
        }

        //
        emItem = emFirstMatch;  // δ�ҵ�ƥ���ϯλ��Ĭ��ʹ�ø�ϵͳ�µĵ�һ��ϯλ
    }

    // ��ȡϯλ��Ӧ��ģʽ���ֶ�
    return emItem.attribute("module").replace("@", ">>");
}

bool LayoutManager::parseSystemStatus(QDomElement &emRoot, const QString &section, QDomElement &emSystem, QDomElement &emModule)
{
    // �������
    if (emRoot.isNull()) {
        return false;   // ��Ч
    }

    //
    QString system, module = section;

    // �����ǰ��¼�û�Ϊ����Ա
    if (jloginManager()->isAdminUser()) {
        system = QStringLiteral("����Աϵͳ");
        module = QStringLiteral("Ȩ�޹���");
    } else {
        system = emRoot.attribute("system");
        if (system.isEmpty()) {
            QMessageBox::critical(q_frameLayout->mainViewManager(),
                                  QStringLiteral("����"),
                                  QStringLiteral("���ڿ�ܲ��������ļ���ָ����ǰϵͳ��"));
            return false;
        }
    }

    // find current system
    emSystem = findSystemNode(emRoot, system);
    if (emSystem.isNull()) {
        QMessageBox::critical(q_frameLayout->mainViewManager(),
                              QStringLiteral("����"),
                              QStringLiteral("û���ҵ���Ч��ϵͳ�ڵ�[ϵͳ��%2]�������ܲ��������ļ�\"%1\"��")
                              .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                              .arg(system));
        return false;
    }

    // ���µ�ǰϵͳ
    q_notifier->sendMessage("j_previous_system_changed", (JWPARAM)&system.toStdString());

    // get current module
    if (module.isEmpty()) {
        // ��ȡ�����ļ���ָ��ϵͳ�µĵ�ǰģʽ
        QString tempModule = currentConfigModule(emSystem);
        if (tempModule.isEmpty()) {
            // ��ȡ��ǰģʽ�����ֶ�
            module = emSystem.attribute("module");
        } else {
            module = tempModule;
        }
    }

    // ��λģʽ�ڵ���Ϣ
    resetModuleElement(emSystem);

    // find current module
    emModule = findModuleNode(emSystem, module);
    if (emModule.isNull()) {
        QMessageBox::critical(q_frameLayout->mainViewManager(),
                              QStringLiteral("����"),
                              QStringLiteral("û���ҵ���Ч��ģʽ�ڵ㣬�����ܲ��������ļ���"));
        return false;
    }

    // ���µ�ǰģʽ
    q_notifier->sendMessage("j_previous_module_changed", (JWPARAM)&module.toStdString());

    // load popup node (global component within current system <befor module loaded>)
    QDomElement emPopup = emSystem.firstChildElement("module");
    if (!emPopup.isNull()) {
        emPopup = emSystem.previousSiblingElement("popup");
        if (!emPopup.isNull()) {
            if (!loadModulePopupNode(emPopup)) {
                //return false; // load failure
            }
        }
    }

    return true;
}

JSplitter *LayoutManager::createView(const QDomElement &emSplitter, QWidget *parent)
{
    // �������
    if (emSplitter.isNull()) {
        return 0;   // ��Ч
    }

    // ����splitter
    JSplitter *splitter = 0;
    // type
    const QString type = emSplitter.attribute("type");
    if (type.isEmpty() || type == "splitter") {
        splitter = createSplitter(emSplitter, parent);
        if (!splitter) {
            const QString msg = QStringLiteral("���ڷָ�������ʧ�ܣ����ļ���%1��λ��[%2��%3]��")
                    .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                    .arg(emSplitter.lineNumber()).arg(emSplitter.columnNumber());
            jframeLogWarning(msg.toLocal8Bit().data());
            QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), msg);
            return false;
        }
    } else if (type == "dock") {
        //
    } else {
        //
    }

    // �������
    for (QDomElement emChild = emSplitter.firstChildElement();
         !emChild.isNull();
         emChild = emChild.nextSiblingElement()) {
        //
        const QString nodeName = emChild.nodeName();
        //
        if (nodeName == "horizontal" || nodeName == "vertical") {
            //
            JSplitter *childSplitter = createView(emChild, splitter);
            if (!childSplitter) {
                continue;
            }
            // ���봰�ڷָ���
            splitter->addWidget(childSplitter);
            // collapsible
            const QVariant variant = splitter->property(QString("collapsible:%1")
                                                        .arg(splitter->count() - 1).toLocal8Bit().data());
            if (variant.isValid()) {
                splitter->setCollapsible(splitter->count() - 1, variant.toBool());
            }
        } else if (nodeName == "component") {
            // �������
            JComponentInfo *componentInfo = activateComponent(emChild, false);
            if (!componentInfo) {
                continue;
            }
            // ���봰�ڷָ���
            if (componentInfo->widget && !componentInfo->stayOn) {
                // ��ʶ��ͼ�������
                componentInfo->isView = true;
                //
                splitter->addWidget(componentInfo->widget);
                // collapsible
                const QVariant variant = splitter->property(QString("collapsible:%1")
                                                            .arg(splitter->count() - 1).toLocal8Bit().data());
                if (variant.isValid()) {
                    splitter->setCollapsible(splitter->count() - 1, variant.toBool());
                }
            }
        }
    }

    return splitter;
}

bool LayoutManager::loadModuleNode(const QDomElement &emModule)
{
    // �������
    if (emModule.isNull()) {
        return false;   // ��Ч
    }

    // load popup component (before component loaded)
    {
        const QDomElement emPopup = emModule.firstChildElement();
        if (!emPopup.isNull() && emPopup.nodeName() == "popup") {
            if (!loadModulePopupNode(emPopup)) {
                //return false; // load failure
            }
        }
    }

    // get layout node
    const QDomElement emLayout = emModule.firstChildElement("layout");
    if (emLayout.isNull()) {
        return false;   // ��Ч
    }

    // load popup component (before subview loaded)
    {
        const QDomElement emPopup = emLayout.firstChildElement();
        if (!emPopup.isNull() && emPopup.nodeName() == "popup") {
            if (!loadModulePopupNode(emPopup)) {
                //return false; // load failure
            }
        }
    }

    // load view node
    {
        const QDomElement emView = emLayout.firstChildElement("view");
        if (!emView.isNull()) {
            if (!loadModuleViewNode(emView)) {
                //return false; // load failure
            }
        }
    }

    // load popup nodeo (after view loaded)
    {
        const QDomElement emView = emLayout.firstChildElement("view");
        if (!emView.isNull()) {
            if (!loadModulePopupNode(emView.nextSiblingElement("popup"))) {
                //return false; // load failure
            }
        }
    }

    // load popup component (after layout loaded)
    {
        const QDomElement emLayout2 = emModule.firstChildElement("layout");
        if (!emLayout2.isNull()) {
            if (!loadModulePopupNode(emLayout2.nextSiblingElement("popup"))) {
                //return false; // load failure
            }
        }
    }

    // load centerview node
    {
        const QDomElement emCenterview = emLayout.firstChildElement("centerview");
        if (!emCenterview.isNull()) {
            if (!loadModuleCenterviewNode(emCenterview)) {
                //return false; // load failure
            }
        }
    }

    // load statusbar node
    {
        const QDomElement emStatusbar = emLayout.firstChildElement("statusbar");
        if (!emStatusbar.isNull()) {
            if (!loadModuleStatusBarNode(emStatusbar)) {
                //return fales; // load failure
            }
        }
    }

    // load dock node
    {
        const QDomElement emDock = emLayout.firstChildElement("dock");
        if (!emDock.isNull()) {
            if (!loadModuleDockNode(emDock)) {
                //return false; // load failure
            }
        }
    }

    return true;
}

bool LayoutManager::loadModuleViewNode(const QDomElement &emView)
{
    // �������
    if (emView.isNull()) {
        return false;   // ��Ч
    }

    //
    JSplitter *splitter = 0;
    const QDomElement emSplitter = emView.firstChildElement();
    if (!emSplitter.isNull()) {
        //
        splitter = createView(emSplitter, q_frameLayout->mainViewManager());
        if (!splitter) {
            const QString msg = QStringLiteral("��ͼ����ʧ�ܣ����ļ���%1��λ��[$2��%3]��")
                    .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                    .arg(emView.lineNumber()).arg(emView.columnNumber());
            jframeLogWarning(msg.toLocal8Bit().data());
            QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), msg);
            return false;
        }
    }

    // ��λ���д���δ����״̬����ͼ����ĸ�����
    q_frameLayout->moduleManager()->resetAllInactivateViewComponent();

    // ���پɵĲ���
    if (q_mainSplitter) {
        q_mainSplitter->deleteLater();
        q_mainSplitter = 0;
    }

    // �����µĲ���
    if (splitter) {
        if (splitter->count() == 0) {
            splitter->deleteLater();
        } else {
            q_frameLayout->mainViewManager()->attachSplitter(splitter);
            q_mainSplitter = splitter;
        }
    }

    // ��λ���д���δ����״̬����ͼ����ĸ�����
    q_frameLayout->moduleManager()->showAllActivateViewComponent();

    return true;
}

bool LayoutManager::loadModulePopupNode(const QDomElement &emPopup)
{
    // �������
    if (emPopup.isNull()) {
        return false;   // ��Ч
    }

    // �������
    for (QDomElement emComponent = emPopup.firstChildElement("component");
         !emComponent.isNull();
         emComponent = emComponent.nextSiblingElement("component")) {
        // �������
        const JComponentInfo *componentInfo = activateComponent(emComponent, false);
        if (!componentInfo) {
            continue;
        }
        //
    }

    return true;
}

bool LayoutManager::loadModuleCenterviewNode(const QDomElement &emCenterView)
{
    Q_UNUSED(emCenterView);
    Q_ASSERT(false);    // �ݲ�֧��
    return false;
}

bool LayoutManager::loadModuleStatusBarNode(const QDomElement &emStatusBar)
{
    Q_UNUSED(emStatusBar);
    Q_ASSERT(false);    // �ݲ�֧��
    return false;
}

bool LayoutManager::loadModuleDockNode(const QDomElement &emDock)
{
    Q_UNUSED(emDock);
    Q_ASSERT(false);    // �ݲ�֧��
    return false;
}

QDomElement LayoutManager::findSystemNode(QDomElement &emParent, const QString &system)
{
    // �������
    if (emParent.isNull()) {
        return QDomElement();   // ��Ч
    }

    // find current system
    for (QDomElement emSystem = emParent.firstChildElement("system");
         !emSystem.isNull();
         emSystem = emSystem.nextSiblingElement("system")) {
        //
        if (emSystem.attribute("name") == system) {
            return emSystem;    // �ҵ�
        }
    }

    return QDomElement();
}

QDomElement LayoutManager::findModuleNode(QDomElement &emParent, QString &section)
{
    // �������
    if (emParent.isNull()) {
        return QDomElement();   // ��Ч
    }

    // system node
    if (section.isEmpty() && emParent.nodeName() == "system") {
        const QString currentModule = emParent.attribute("module");
        if (!currentModule.isEmpty()) {
            QString module;
            QDomElement emModule;
            for (emModule = emParent.firstChildElement("module");
                 !emModule.isNull();
                 emModule = emModule.nextSiblingElement("module")) {
                // ��ȡģʽ����
                module = emModule.attribute("name");
                if (module == currentModule) {
                    section = module;
                    break;  // �ҵ�
                }
            }
            //
            return emModule;
        }
    }

    //
    if (section.isEmpty()) {
        return QDomElement();   // ��Ч
    }

    //
    QString module = section.section(">>", 0, 0, QString::SectionSkipEmpty).trimmed();
    QString subSection = section.section(">>", 1, -1, QString::SectionSkipEmpty).trimmed();
    QString realSection = module;

    //
    QDomElement emModule;
    for (emModule = emParent.firstChildElement("module");
         !emModule.isNull();
         emModule = emModule.nextSiblingElement("module")) {
        //
        if (module.isEmpty()) {
            const QString _module = emModule.attribute("submodule");
            if (_module.isEmpty()) {
                realSection = emModule.attribute("name");
                break;  // find out by default module node
            } else {
                // truncate
                if (emModule.firstChildElement("module").isNull()) {
                    break;  // error
                }
                //
                subSection = _module;
            }
        } else {
            const QString name = emModule.attribute("name");
            if (name == module && subSection.isEmpty()) {
                break;  // found
            }
            // truncate
            if (emModule.firstChildElement("module").isNull()) {
                continue;
            }
        }

        //
        if (section.isEmpty()) {
            continue;
        }

        // sub-module
        QString _section = subSection;
        QDomElement emSubModule = findModuleNode(emModule, _section);
        if (!emSubModule.isNull()) {
            emModule = emSubModule;
            realSection.append(">>").append(_section);
            break;  // found
        }
    }

    // ���µ�ǰģʽ��
    section = realSection;

    // ���¸��ڵ�ģʽ״̬
    if (q_layoutConfig.sw.remember) {
        const QString currentModule =
                realSection.section(">>", 0, 0, QString::SectionSkipEmpty);
        if (emParent.nodeName() == "system") {
            emParent.setAttribute("module", currentModule);
        } else {
            emParent.setAttribute("submodule", currentModule);
        }
    }

    return emModule;
}

QDomElement LayoutManager::findModuleNode(const QDomElement &emParent, const QString &section)
{
    // �������
    if (emParent.isNull()) {
        return QDomElement();   // ��Ч
    }

    // ��ȡ����ģʽ����
    const QString firstSection =
            section.section(">>", 0, 0, QString::SectionSkipEmpty).trimmed();

    // ����ģʽ�ڵ�
    QDomElement emModule;
    for (emModule = emParent.firstChildElement("module");
         !emModule.isNull();
         emModule = emModule.nextSiblingElement("module")) {
        // ģʽ����ƥ��
        if (emModule.attribute("name") == firstSection) {
            break;  // found
        } else {
            const QString nextSection =
                    section.section(">>", 1, -1, QString::SectionSkipEmpty).trimmed();
            if (nextSection.isEmpty()) {
                break;  // δ�ҵ���������һ����ģʽ������Ч����ʱ����δ�ҵ����״̬
            } else {
                const QDomElement emSubModule = findModuleNode(emModule, nextSection);
                if (emSubModule.isNull()) {
                    continue;   // ��������
                } else {
                    emModule = emSubModule;
                    break;  // found
                }
            }
        }
    }

    return emModule;
}

bool LayoutManager::isComponentAttachEnabled(const JComponentInfo *componentInfo) const
{
    // �������
    if (!componentInfo) {
        return false;   // ��Ч
    }

    // ��δ���õ�¼ϵͳ������£� ���������Ȩ�޼��
    if (!jloginManager()->isValid()) {
        return true;
    }

    // ����ǹ���Ա�û���¼���򲻽������Ȩ�޼��
    if (jloginManager()->isAdminUser()) {
        return true;
    }

    // �ӿ�����
    IJComponent *component = dynamic_cast<IJComponent *>(componentInfo->iface);
    if (!component) {
        return false;   // ����ʧ��
    }

    // ��ȡ���Ȩ�޵ȼ�
    int componentPowerLevel = q_frameLayout->componentPowerLevel(component->componentName());

    // Ȩ���ж�
    if (componentPowerLevel <= JFrameLoginSpace::PowerLevelNo) {
        return false;   // ��Ȩ��
    }

    return true;
}

JComponentInfo *LayoutManager::activateComponent(const QDomElement &emComponent, bool show)
{
    // �������
    if (emComponent.isNull()) {
        return 0;   // ��Ч
    }

    // ���ñ�ʶ��load��
    if (emComponent.hasAttribute("load")) {
        if (!QVariant(emComponent.attribute("load")).toBool()) {
            return 0;   // ���������
        }
    }

    // ��ȡ�����ʶ
    const QString componentId = emComponent.attribute("id");
    if (componentId.isEmpty()) {
        return 0;   // ���������ʶΪ�յ���
    }

    // ��ȡ���������Ϣ
    JComponentInfo *componentInfo =
            q_frameLayout->moduleManager()->componentByName(componentId);

    // ��������Ч�ԣ����Ƿ���GF��ܳ������أ������Ѿ�����ӳ����У�
    if (!componentInfo) {
        const QString msg = QStringLiteral("���[%1]δ���أ����ļ���%2��λ��[%3��%4]��")
                .arg(componentId)
                .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                .arg(emComponent.lineNumber()).arg(emComponent.columnNumber());
        jframeLogWarning(msg.toLocal8Bit().data());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), msg);
        return 0;
    }
    // �������Ȩ���ж�
    if (!isComponentAttachEnabled(componentInfo)) {
        const QString msg = QStringLiteral("���[%1]���ܹ��أ����ļ���%2��λ��[%3��%4]��")
                .arg(componentId)
                .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                .arg(emComponent.lineNumber()).arg(emComponent.columnNumber());
        jframeLogWarning(msg.toLocal8Bit().data());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), msg);
        return 0;
    }
    // �����������
    if (!q_frameLayout->moduleManager()->attachComponentUi(componentInfo)) {
        const QString msg = QStringLiteral("���[%1]�������ʧ�ܣ����ļ���%2��λ��[%3��%4]��")
                .arg(componentId)
                .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                .arg(emComponent.lineNumber()).arg(emComponent.columnNumber());
        jframeLogWarning(msg.toLocal8Bit().data());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), msg);
        return 0;
    }
    // �������
    if (!q_frameLayout->moduleManager()->attachComponent(componentInfo, show)) {
        const QString msg = QStringLiteral("���[%1]����ʧ�ܣ����ļ���%2��λ��[%3��%4]��")
                .arg(componentId)
                .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                .arg(emComponent.lineNumber()).arg(emComponent.columnNumber());
        jframeLogWarning(msg.toLocal8Bit().data());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), msg);
        return 0;
    }

    // ��ʶ����״̬
    componentInfo->active = true;

    return componentInfo;
}

JSplitter *LayoutManager::createSplitter(const QDomElement &emSplitter, QWidget *parent)
{
    // �������
    if (emSplitter.isNull()) {
        return 0;   // ��Ч
    }

    //
    Qt::Orientation orientation = Qt::Horizontal;

    // �������ַ���
    {
        const QString nodeName = emSplitter.nodeName();
        if (nodeName == "horizontal") {
            orientation = Qt::Horizontal;
        } else if (nodeName == "vertical") {
            orientation = Qt::Vertical;
        } else {
            const QString msg = QStringLiteral("��֧�ֵĲ��ֽڵ�[%1]�����ļ���%2��λ��[%3��%4]��")
                    .arg(nodeName)
                    .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                    .arg(emSplitter.lineNumber()).arg(emSplitter.columnNumber());
            jframeLogWarning(msg.toLocal8Bit().data());
            QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("��������"), msg);
            return 0;
        }
    }

    // �������ڷָ���ʵ��
    JSplitter *splitter = 0;
    if (q_layoutConfig.splitter.syncScales) {
        splitter = new JSplitterSync(this, orientation, parent);
    } else {
        splitter = new JSplitter(orientation, parent);
    }

    // scales
    if (emSplitter.hasAttribute("scales")) {
        QString sScale = emSplitter.attribute("scales");
        if (sScale.isEmpty()) {
            sScale = "1:1";
        }
        const QStringList sScales = sScale.split(QRegExp("[:#]"), QString::SkipEmptyParts);
        QList<double> scales;
        QStringListIterator citerSScales(sScales);
        while (citerSScales.hasNext()) {
            scales << citerSScales.next().toDouble();
        }
        splitter->setScales(scales);
    }
    // opaqueResize
    if (emSplitter.hasAttribute("opaqueResize")) {
        splitter->setOpaqueResize(QVariant(emSplitter.attribute("opaqueResize")).toBool());
    } else {
        splitter->setOpaqueResize(q_layoutConfig.splitter.opaqueResize);
    }
    // collapsible
    if (emSplitter.hasAttribute("collapsible")) {
        const QString sCollapsible = emSplitter.attribute("collapsible");
        if (!sCollapsible.isEmpty()) {
            const QStringList sBools = sCollapsible.split(';', QString::SkipEmptyParts);
            if (!sBools.isEmpty()) {
                QStringListIterator citerSBools(sBools);
                while (citerSBools.hasNext()) {
                    const QStringList sItems = citerSBools.next().trimmed().split(':', QString::SkipEmptyParts);
                    if (sItems.count() >= 2) {
                        splitter->setProperty(QString("collapsible:%1").arg(sItems.at(0)).toLatin1(),
                                              QVariant(sItems.at(1)).toBool());
                    }
                }
            }
        }
    }
    // color of handle
    QColor color;
    if (emSplitter.hasAttribute("color")) {
        color = QColor(emSplitter.attribute("color"));
    }
    if (!color.isValid()) {
        color = q_layoutConfig.splitter.color;
    }
    if (emSplitter.hasAttribute("opacity")) {
        const QString handleOpacity = emSplitter.attribute("opacity");
        if (handleOpacity.contains('.')) {
            color.setAlphaF(handleOpacity.toDouble());
        } else {
            color.setAlpha(handleOpacity.toUInt());
        }
    }
    splitter->setHandleColor(color);
    // state of handle
    if (emSplitter.hasAttribute("enabled")) {
        const bool enabled = QVariant(emSplitter.attribute("enabled")).toBool();
        if (enabled) {
            if (emSplitter.hasAttribute("width")) {
                splitter->setHandleWidth(emSplitter.attribute("width").toUInt());
            } else {
                splitter->setHandleWidth(q_layoutConfig.splitter.width);
            }
        } else {
            splitter->setHandleWidth(1);
        }
    }
    // handle
    QSplitterHandle *splitterHandle = splitter->handle(1);
    if (splitterHandle) {
        splitterHandle->setEnabled(true);
    }
    // handle - checkable
    if (emSplitter.hasAttribute("checkable")) {
        splitter->setCheckable(QVariant(emSplitter.attribute("checkable")).toBool());
    } else {
        splitter->setCheckable(q_layoutConfig.splitter.checkable);
    }

    return splitter;
}

bool LayoutManager::saveCurrentSplitterScales()
{
    //
    if (!q_mainSplitter) {
        return false;   // ��Ч
    }

    // �򿪿�ܲ��������ļ�
    QFile file(QString::fromStdString(jframeFacade()->frameLayoutPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("��ܲ��������ļ�\"%1\"�����ڣ�")
                .arg(file.fileName());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), text);
        return false;   // �ļ�������
    }

    // ���ļ�
    if (!file.open(QFile::ReadWrite)) {
        return false;   // ��ʧ��
    }

    // �����ļ�
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("��ܲ��������ļ�\"%1\"����ʧ�ܣ�\n"
                                            "����������%2\n"
                                            "����λ�ã����кţ�%3���кţ�%4��")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("����"), text);
        return false;
    }

    // �ر��ļ�
    file.close();

    // ��ȡ���ڵ�
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // ��Ч
    }

    // ��ȡ��ǰϵͳ��ģʽ
    const QString currentSystem = QString::fromStdString(q_frameLayout->currentSystem());
    const QString currentModule = QString::fromStdString(q_frameLayout->currentModule());

    // ����ϵͳ�ڵ�
    QDomElement emSystem = findSystemNode(emRoot, currentSystem);
    if (emSystem.isNull()) {
        return false;   // δ�ҵ�
    }

    // ����ģʽ�ڵ�
    QDomElement emModule = findModuleNode(emSystem, currentModule);
    if (emModule.isNull()) {
        return false;   // δ�ҵ�
    }

    // ��ȡ���ֽڵ�
    QDomElement emLayout = emModule.firstChildElement("layout");
    if (emLayout.isNull()) {
        return false;   // ��Ч
    }

    // ��ȡ��ͼ�ڵ�
    QDomElement emView = emLayout.firstChildElement("view");
    if (emView.isNull()) {
        return false;   // ��Ч
    }

    // ����splitter��scales����
    if (!updateSpltterScales(emView, q_mainSplitter)) {
        return false;   // ����ʧ��
    }

    // �����޸�
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;   // ��ʧ��
    }
    QTextStream textStream(&file);
    document.save(textStream, 2);

    return true;
}

bool LayoutManager::updateSpltterScales(const QDomElement &emParent, const QWidget *widget)
{
    // �������
    if (emParent.isNull()) {
        return false;   // ��Ч
    }

    // �������ͼ��
    if (!widget->inherits("JSplitterSync")) {
        return true;    //
    }

    // ��������
    const JSplitter *splitter = qobject_cast<const JSplitter *>(widget);
    if (!splitter) {
        return false;   // ʧ��
    }

    //
    const Qt::Orientation orientation = splitter->orientation();
    QDomElement emSplitter;
    switch (orientation) {
    case Qt::Horizontal:
        emSplitter = emParent.firstChildElement("horizontal");
        break;
    case Qt::Vertical:
        emSplitter = emParent.firstChildElement("vertical");
        break;
    default:
        return false;   // ���ַ�����Ч
    }

    // ���ɱ����ַ���
    QString sScale;
    QListIterator<double> citerScales(splitter->scales());
    while (citerScales.hasNext()) {
        sScale.append(QString::number(citerScales.next(), 'g', 2) % ':');
    }
    if (!sScale.isEmpty()) {
        sScale.chop(1);
    }
    // ����scales����
    emSplitter.setAttribute("scales", sScale);

    // �ݹ����
    const int count = splitter->count();
    for (int i = 0; i < count; ++i) {
        if (!updateSpltterScales(emSplitter, splitter->widget(i))) {
            continue;
        }
    }

    return true;
}

// class JSplitterSync

JSplitterSync::JSplitterSync(LayoutManager *layoutManager, QWidget *parent):
    JSplitter(parent),
    q_layoutManager(layoutManager)
{
    connect(this, SIGNAL(splitterMoved(int,int)),
            SLOT(onSplitterMoved(int,int)), Qt::QueuedConnection);
}

JSplitterSync::JSplitterSync(LayoutManager *layoutManager, Qt::Orientation orientation, QWidget *parent):
    JSplitter(orientation, parent),
    q_layoutManager(layoutManager)
{
    connect(this, SIGNAL(splitterMoved(int,int)),
            SLOT(onSplitterMoved(int,int)), Qt::QueuedConnection);
}

void JSplitterSync::onSplitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    //
    if (q_layoutManager) {
        q_layoutManager->saveCurrentSplitterScales();
    }
}
