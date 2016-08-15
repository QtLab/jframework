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
    /// 设置默认配置

    // splitter
    q_layoutConfig.splitter.width = 8;
    q_layoutConfig.splitter.checkable = true;
    q_layoutConfig.splitter.opaqueResize = false;
    q_layoutConfig.splitter.color = QColor(64, 64, 64, 255);
    q_layoutConfig.splitter.syncScales = true;
    // switch
    q_layoutConfig.sw.remember = true;  // 默认允许记忆模式切换结果

    //
    bool result = true;

    // 加载布局配置信息
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
    // 将所有组件设置为未激活状态
    q_frameLayout->moduleManager()->inactivateAllComponent();

    // 打开框架布局配置文件
    QFile file(QString::fromStdString(jframeFacade()->frameLayoutPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("框架布局配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), text);
        return false;   // 文件不存在
    }

    // 打开文件
    if (!file.open(QFile::ReadOnly)) {
        return false;   // 打开失败
    }

    // 解析文件
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("框架布局配置文件\"%1\"解析失败！\n"
                                            "错误描述：%2\n"
                                            "错误位置：（行号：%3，列号：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), text);
        return false;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // 获取失败
    }

    // 解析系统状态
    QDomElement emSystem, emModule;
    if (!parseSystemStatus(emRoot, section, emSystem, emModule)) {
        return false;
    }

    // 加载模式节点
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

    // 保存修改
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;   // 打开失败
    }
    QTextStream textStream(&file);
    document.save(textStream, 2);

    // 将所有处于未激活状态的组件分离
    q_frameLayout->moduleManager()->detachAllInactivateComponent();

    return true;
}

bool LayoutManager::loadLayoutConfig()
{
    // 解析框架布局配置文件
    QFile file(QString::fromStdString(jframeFacade()->frameLayoutPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("框架布局配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), text);
        return false;   // 文件不存在
    }

    // 打开文件
    if (!file.open(QFile::ReadOnly)) {
        return false;   // 打开失败
    }

    // 解析文件
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("框架布局配置文件\"%1\"解析失败！"
                                            "错误描述：%2\n"
                                            "错误位置：（行号：%3，列号：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), text);
        return false;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    const QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // 无效
    }

    // 获取配置节点
    const QDomElement emConfig = emRoot.firstChildElement("config");
    if (emConfig.isNull()) {
        return true;    // 没有配置项，忽略
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
    // 打开框架布局配置文件
    QFile file(QString::fromStdString(jframeFacade()->frameLayoutPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("框架布局配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), text);
        return false;   // 文件不存在
    }

    // 打开文件
    if (!file.open(QFile::ReadWrite)) {
        return false;   // 打开失败
    }

    // 解析文件
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("框架布局配置文件\"%1\"解析失败！\n"
                                            "错误描述：%2\n"
                                            "错误位置：（行号：%3，列号：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), text);
        return false;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // 无效
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
    // 参数检测
    if (emModule.isNull()) {
        return;   // 无效
    }

    // 复位状态
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
    // 参数检测
    if (emSystem.isNull()) {
        return QString::null;   // 无效
    }

    // 获取席位-模式映射节点
    const QDomElement emSeatModule = emSystem.firstChildElement("seat-module");
    if (emSeatModule.isNull()) {
        return QString::null;   // 无效
    }

    //
    const QString lastModule = emSystem.attribute("module");

    // 查找指定写的席位-模式映射节点
    QDomElement emItem;
    QDomElement emFirstMatch;
    for (emItem = emSeatModule.firstChildElement("item");
         !emItem.isNull();
         emItem = emItem.nextSiblingElement("item")) {
        // 匹配当前席位
#if 0
        if (emItem.attribute("seat").toStdString()
                == q_frameLayout->powerInfo().seatElement.sName) {
            // 匹配系统
            if (emFirstMatch.isNull()) {
                emFirstMatch = emItem;
            }
            // 匹配模式
            if (emItem.attribute("module") == lastModule) {
                break;  // 匹配成功
            }
        }
#else
        break;
#endif
    }

    //
    if (emItem.isNull()) {
        if (emFirstMatch.isNull()) {
            return QString::null;   // 未找到匹配的系统
        }

        //
        emItem = emFirstMatch;  // 未找到匹配的席位，默认使用该系统下的第一个席位
    }

    // 获取席位对应的模式链字段
    return emItem.attribute("module").replace("@", ">>");
}

bool LayoutManager::parseSystemStatus(QDomElement &emRoot, const QString &section, QDomElement &emSystem, QDomElement &emModule)
{
    // 参数检测
    if (emRoot.isNull()) {
        return false;   // 无效
    }

    //
    QString system, module = section;

    // 如果当前登录用户为管理员
    if (jloginManager()->isAdminUser()) {
        system = QStringLiteral("管理员系统");
        module = QStringLiteral("权限管理");
    } else {
        system = emRoot.attribute("system");
        if (system.isEmpty()) {
            QMessageBox::critical(q_frameLayout->mainViewManager(),
                                  QStringLiteral("错误"),
                                  QStringLiteral("请在框架布局配置文件中指定当前系统！"));
            return false;
        }
    }

    // find current system
    emSystem = findSystemNode(emRoot, system);
    if (emSystem.isNull()) {
        QMessageBox::critical(q_frameLayout->mainViewManager(),
                              QStringLiteral("错误"),
                              QStringLiteral("没有找到有效的系统节点[系统：%2]，请检查框架布局配置文件\"%1\"！")
                              .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                              .arg(system));
        return false;
    }

    // 更新当前系统
    q_notifier->sendMessage("j_previous_system_changed", (JWPARAM)&system.toStdString());

    // get current module
    if (module.isEmpty()) {
        // 获取配置文件中指定系统下的当前模式
        QString tempModule = currentConfigModule(emSystem);
        if (tempModule.isEmpty()) {
            // 获取当前模式名称字段
            module = emSystem.attribute("module");
        } else {
            module = tempModule;
        }
    }

    // 复位模式节点信息
    resetModuleElement(emSystem);

    // find current module
    emModule = findModuleNode(emSystem, module);
    if (emModule.isNull()) {
        QMessageBox::critical(q_frameLayout->mainViewManager(),
                              QStringLiteral("错误"),
                              QStringLiteral("没有找到有效的模式节点，请检查框架布局配置文件！"));
        return false;
    }

    // 更新当前模式
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
    // 参数检测
    if (emSplitter.isNull()) {
        return 0;   // 无效
    }

    // 创建splitter
    JSplitter *splitter = 0;
    // type
    const QString type = emSplitter.attribute("type");
    if (type.isEmpty() || type == "splitter") {
        splitter = createSplitter(emSplitter, parent);
        if (!splitter) {
            const QString msg = QStringLiteral("窗口分割器创建失败！（文件：%1，位置[%2，%3]）")
                    .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                    .arg(emSplitter.lineNumber()).arg(emSplitter.columnNumber());
            jframeLogWarning(msg.toLocal8Bit().data());
            QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), msg);
            return false;
        }
    } else if (type == "dock") {
        //
    } else {
        //
    }

    // 挂载组件
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
            // 加入窗口分割器
            splitter->addWidget(childSplitter);
            // collapsible
            const QVariant variant = splitter->property(QString("collapsible:%1")
                                                        .arg(splitter->count() - 1).toLocal8Bit().data());
            if (variant.isValid()) {
                splitter->setCollapsible(splitter->count() - 1, variant.toBool());
            }
        } else if (nodeName == "component") {
            // 激活组件
            JComponentInfo *componentInfo = activateComponent(emChild, false);
            if (!componentInfo) {
                continue;
            }
            // 加入窗口分割器
            if (componentInfo->widget && !componentInfo->stayOn) {
                // 标识视图窗口组件
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
    // 参数检测
    if (emModule.isNull()) {
        return false;   // 无效
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
        return false;   // 无效
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
    // 参数检测
    if (emView.isNull()) {
        return false;   // 无效
    }

    //
    JSplitter *splitter = 0;
    const QDomElement emSplitter = emView.firstChildElement();
    if (!emSplitter.isNull()) {
        //
        splitter = createView(emSplitter, q_frameLayout->mainViewManager());
        if (!splitter) {
            const QString msg = QStringLiteral("视图创建失败！（文件：%1，位置[$2，%3]）")
                    .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                    .arg(emView.lineNumber()).arg(emView.columnNumber());
            jframeLogWarning(msg.toLocal8Bit().data());
            QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), msg);
            return false;
        }
    }

    // 复位所有处于未激活状态的视图组件的父窗口
    q_frameLayout->moduleManager()->resetAllInactivateViewComponent();

    // 销毁旧的布局
    if (q_mainSplitter) {
        q_mainSplitter->deleteLater();
        q_mainSplitter = 0;
    }

    // 加入新的布局
    if (splitter) {
        if (splitter->count() == 0) {
            splitter->deleteLater();
        } else {
            q_frameLayout->mainViewManager()->attachSplitter(splitter);
            q_mainSplitter = splitter;
        }
    }

    // 复位所有处于未激活状态的视图组件的父窗口
    q_frameLayout->moduleManager()->showAllActivateViewComponent();

    return true;
}

bool LayoutManager::loadModulePopupNode(const QDomElement &emPopup)
{
    // 参数检测
    if (emPopup.isNull()) {
        return false;   // 无效
    }

    // 挂载组件
    for (QDomElement emComponent = emPopup.firstChildElement("component");
         !emComponent.isNull();
         emComponent = emComponent.nextSiblingElement("component")) {
        // 激活组件
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
    Q_ASSERT(false);    // 暂不支持
    return false;
}

bool LayoutManager::loadModuleStatusBarNode(const QDomElement &emStatusBar)
{
    Q_UNUSED(emStatusBar);
    Q_ASSERT(false);    // 暂不支持
    return false;
}

bool LayoutManager::loadModuleDockNode(const QDomElement &emDock)
{
    Q_UNUSED(emDock);
    Q_ASSERT(false);    // 暂不支持
    return false;
}

QDomElement LayoutManager::findSystemNode(QDomElement &emParent, const QString &system)
{
    // 参数检测
    if (emParent.isNull()) {
        return QDomElement();   // 无效
    }

    // find current system
    for (QDomElement emSystem = emParent.firstChildElement("system");
         !emSystem.isNull();
         emSystem = emSystem.nextSiblingElement("system")) {
        //
        if (emSystem.attribute("name") == system) {
            return emSystem;    // 找到
        }
    }

    return QDomElement();
}

QDomElement LayoutManager::findModuleNode(QDomElement &emParent, QString &section)
{
    // 参数检测
    if (emParent.isNull()) {
        return QDomElement();   // 无效
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
                // 获取模式名称
                module = emModule.attribute("name");
                if (module == currentModule) {
                    section = module;
                    break;  // 找到
                }
            }
            //
            return emModule;
        }
    }

    //
    if (section.isEmpty()) {
        return QDomElement();   // 无效
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

    // 更新当前模式层
    section = realSection;

    // 更新父节点模式状态
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
    // 参数检测
    if (emParent.isNull()) {
        return QDomElement();   // 无效
    }

    // 获取顶层模式名称
    const QString firstSection =
            section.section(">>", 0, 0, QString::SectionSkipEmpty).trimmed();

    // 查找模式节点
    QDomElement emModule;
    for (emModule = emParent.firstChildElement("module");
         !emModule.isNull();
         emModule = emModule.nextSiblingElement("module")) {
        // 模式名称匹配
        if (emModule.attribute("name") == firstSection) {
            break;  // found
        } else {
            const QString nextSection =
                    section.section(">>", 1, -1, QString::SectionSkipEmpty).trimmed();
            if (nextSection.isEmpty()) {
                break;  // 未找到，并且下一个子模式名称无效，此时返回未找到结果状态
            } else {
                const QDomElement emSubModule = findModuleNode(emModule, nextSection);
                if (emSubModule.isNull()) {
                    continue;   // 继续查找
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
    // 参数检测
    if (!componentInfo) {
        return false;   // 无效
    }

    // 在未启用登录系统的情况下， 不进行组件权限检测
    if (!jloginManager()->isValid()) {
        return true;
    }

    // 如果是管理员用户登录，则不进行组件权限检测
    if (jloginManager()->isAdminUser()) {
        return true;
    }

    // 接口提升
    IJComponent *component = dynamic_cast<IJComponent *>(componentInfo->iface);
    if (!component) {
        return false;   // 提升失败
    }

    // 获取组件权限等级
    int componentPowerLevel = q_frameLayout->componentPowerLevel(component->componentName());

    // 权限判定
    if (componentPowerLevel <= JFrameLoginSpace::PowerLevelNo) {
        return false;   // 无权限
    }

    return true;
}

JComponentInfo *LayoutManager::activateComponent(const QDomElement &emComponent, bool show)
{
    // 参数检测
    if (emComponent.isNull()) {
        return 0;   // 无效
    }

    // 配置标识（load）
    if (emComponent.hasAttribute("load")) {
        if (!QVariant(emComponent.attribute("load")).toBool()) {
            return 0;   // 不允许加载
        }
    }

    // 获取组件标识
    const QString componentId = emComponent.attribute("id");
    if (componentId.isEmpty()) {
        return 0;   // 忽略组件标识为空的项
    }

    // 获取挂载组件信息
    JComponentInfo *componentInfo =
            q_frameLayout->moduleManager()->componentByName(componentId);

    // 检测组件有效性（即是否由GF框架初步加载，并且已经加入映射队列）
    if (!componentInfo) {
        const QString msg = QStringLiteral("组件[%1]未加载！（文件：%2，位置[%3，%4]）")
                .arg(componentId)
                .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                .arg(emComponent.lineNumber()).arg(emComponent.columnNumber());
        jframeLogWarning(msg.toLocal8Bit().data());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), msg);
        return 0;
    }
    // 组件挂载权限判定
    if (!isComponentAttachEnabled(componentInfo)) {
        const QString msg = QStringLiteral("组件[%1]不能挂载！（文件：%2，位置[%3，%4]）")
                .arg(componentId)
                .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                .arg(emComponent.lineNumber()).arg(emComponent.columnNumber());
        jframeLogWarning(msg.toLocal8Bit().data());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), msg);
        return 0;
    }
    // 挂载组件界面
    if (!q_frameLayout->moduleManager()->attachComponentUi(componentInfo)) {
        const QString msg = QStringLiteral("组件[%1]界面挂载失败！（文件：%2，位置[%3，%4]）")
                .arg(componentId)
                .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                .arg(emComponent.lineNumber()).arg(emComponent.columnNumber());
        jframeLogWarning(msg.toLocal8Bit().data());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), msg);
        return 0;
    }
    // 挂载组件
    if (!q_frameLayout->moduleManager()->attachComponent(componentInfo, show)) {
        const QString msg = QStringLiteral("组件[%1]挂载失败！（文件：%2，位置[%3，%4]）")
                .arg(componentId)
                .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                .arg(emComponent.lineNumber()).arg(emComponent.columnNumber());
        jframeLogWarning(msg.toLocal8Bit().data());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), msg);
        return 0;
    }

    // 标识激活状态
    componentInfo->active = true;

    return componentInfo;
}

JSplitter *LayoutManager::createSplitter(const QDomElement &emSplitter, QWidget *parent)
{
    // 参数检测
    if (emSplitter.isNull()) {
        return 0;   // 无效
    }

    //
    Qt::Orientation orientation = Qt::Horizontal;

    // 解析布局方向
    {
        const QString nodeName = emSplitter.nodeName();
        if (nodeName == "horizontal") {
            orientation = Qt::Horizontal;
        } else if (nodeName == "vertical") {
            orientation = Qt::Vertical;
        } else {
            const QString msg = QStringLiteral("不支持的布局节点[%1]！（文件：%2，位置[%3，%4]）")
                    .arg(nodeName)
                    .arg(QString::fromStdString(jframeFacade()->frameLayoutPath()))
                    .arg(emSplitter.lineNumber()).arg(emSplitter.columnNumber());
            jframeLogWarning(msg.toLocal8Bit().data());
            QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("创建布局"), msg);
            return 0;
        }
    }

    // 创建窗口分割器实例
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
        return false;   // 无效
    }

    // 打开框架布局配置文件
    QFile file(QString::fromStdString(jframeFacade()->frameLayoutPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("框架布局配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), text);
        return false;   // 文件不存在
    }

    // 打开文件
    if (!file.open(QFile::ReadWrite)) {
        return false;   // 打开失败
    }

    // 解析文件
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("框架布局配置文件\"%1\"解析失败！\n"
                                            "错误描述：%2\n"
                                            "错误位置：（行号：%3，列号：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(q_frameLayout->mainViewManager(), QStringLiteral("警告"), text);
        return false;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // 无效
    }

    // 获取当前系统和模式
    const QString currentSystem = QString::fromStdString(q_frameLayout->currentSystem());
    const QString currentModule = QString::fromStdString(q_frameLayout->currentModule());

    // 查找系统节点
    QDomElement emSystem = findSystemNode(emRoot, currentSystem);
    if (emSystem.isNull()) {
        return false;   // 未找到
    }

    // 查找模式节点
    QDomElement emModule = findModuleNode(emSystem, currentModule);
    if (emModule.isNull()) {
        return false;   // 未找到
    }

    // 获取布局节点
    QDomElement emLayout = emModule.firstChildElement("layout");
    if (emLayout.isNull()) {
        return false;   // 无效
    }

    // 获取视图节点
    QDomElement emView = emLayout.firstChildElement("view");
    if (emView.isNull()) {
        return false;   // 无效
    }

    // 更新splitter的scales属性
    if (!updateSpltterScales(emView, q_mainSplitter)) {
        return false;   // 更新失败
    }

    // 保存修改
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;   // 打开失败
    }
    QTextStream textStream(&file);
    document.save(textStream, 2);

    return true;
}

bool LayoutManager::updateSpltterScales(const QDomElement &emParent, const QWidget *widget)
{
    // 参数检测
    if (emParent.isNull()) {
        return false;   // 无效
    }

    // 对象类型检测
    if (!widget->inherits("JSplitterSync")) {
        return true;    //
    }

    // 提升类型
    const JSplitter *splitter = qobject_cast<const JSplitter *>(widget);
    if (!splitter) {
        return false;   // 失败
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
        return false;   // 布局方向无效
    }

    // 生成比例字符串
    QString sScale;
    QListIterator<double> citerScales(splitter->scales());
    while (citerScales.hasNext()) {
        sScale.append(QString::number(citerScales.next(), 'g', 2) % ':');
    }
    if (!sScale.isEmpty()) {
        sScale.chop(1);
    }
    // 设置scales属性
    emSplitter.setAttribute("scales", sScale);

    // 递归更新
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
