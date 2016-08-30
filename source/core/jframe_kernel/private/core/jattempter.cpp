#include "precomp.h"
#include "jattempter.h"
#include "jmain_window.h"
#include "jframe_facade.h"
#include "../../jframe_kernel.h"
#include "../layout/jframe_layout_p.h"
#include "jcomponent_notify.h"

// class JAttempter

JAttempter::JAttempter()
    : q_notifier(0)
    , q_mainWindow(0)
    , q_workModeId(0)
{
    // 创建消息分发器实例
    q_notifier = JFRAME_FACTORY_CREATE(INotifier);
    Q_ASSERT(q_notifier);

    //
    q_mainWindow = new JMainWindow(this);
}

JAttempter::~JAttempter()
{
    // 销毁消息分发器实例
    JFRAME_FACTORY_RELEASE(q_notifier, INotifier);

    //
    if (q_mainWindow) {
        delete q_mainWindow;
        q_mainWindow = 0;
    }
}

std::string JAttempter::interfaceIdentity() const
{
    return IID_IJAttempter;
}

unsigned int JAttempter::interfaceVersion() const
{
    return VER_IJAttempter;
}

void *JAttempter::queryInterface(const std::string &iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJUnknown, iid, ver);

    return 0;
}

bool JAttempter::loadInterface()
{
    bool result = true;

    // 加载配置信息
    result = result && loadConfig();

    // 加载初始化组件
    result = result && loadInitComponent();

    // 初始化主窗口
    result = result && q_mainWindow->loadInterface();

    return result;
}

void JAttempter::releaseInterface()
{
    // 下电所有组件
    releaseAllComponent();

    // 释放消息分发器接口
    q_notifier->releaseInterface();

    // 释放框架主窗口
    if (q_mainWindow) {
        q_mainWindow->releaseInterface();
    }
}

std::list<std::string> JAttempter::queryMethod() const
{
    std::list<std::string> methods;

    //

    return methods;
}

bool JAttempter::invokeMethod(const std::string &method, int argc, ...)
{
    bool result = false;
    va_list ap;
    va_start(ap, argc);

    Q_UNUSED(method);

    va_end(ap);

    return result;
}

bool JAttempter::loadComponent()
{
    bool result = true;

    //
    dynamic_cast<JMainWindow *>(q_mainWindow)->startSplash();

    // 加载所有组件
    result = result && loadAllComponent();

    //
    dynamic_cast<JMainWindow *>(q_mainWindow)->finishSplash();

    return result;
}

void JAttempter::releaseComponent()
{
    //
    if (!releaseAllComponent()) {
        //
    }
}

IJComponent *JAttempter::queryComponent(const std::string &componentName)
{
    // 查找组件
    QMap<QString, JComponentConfig>::const_iterator citer =
            q_mapComponent.find(QString::fromStdString(componentName));
    if (citer == q_mapComponent.end()) {
        return 0;   // 不存在
    }

    return citer.value().component;
}

IJMainWindow *JAttempter::mainWindow()
{
    return q_mainWindow;
}

void *JAttempter::queryInterface(const std::string &componentName, const std::string &iid, unsigned int ver)
{
    // 查询组件
    IJComponent *component = queryComponent(componentName);
    if (!component) {
        return 0;   // 不存在
    }

    // 查询组件接口
    return component->queryInterface(iid, ver);
}

std::list<IJComponent *> JAttempter::allComponents() const
{
    std::list<IJComponent *> components;
    QMapIterator<QString, JComponentConfig> citer(q_mapComponent);
    while (citer.hasNext()) {
        citer.next();
        components.push_back(citer.value().component);
    }

    return components;
}

int JAttempter::currentWorkModeId() const
{
    return q_workModeId;
}

std::string JAttempter::currentWorkModeName() const
{
    return q_workModeName;
}

std::string JAttempter::currentWorkModeConfigDirName() const
{
    return q_workModeDir;
}

INotifier &JAttempter::notifier()
{
    return *q_notifier;
}

void JAttempter::endGroup()
{
    componentNotify.endGroup();
}

IJAttempter &JAttempter::unsubMessage(const std::string &id)
{
    componentNotify.unsubMessage(id);
    return *this;
}

void JAttempter::unsubMessage(IJComponent *component)
{
    componentNotify.unsubMessage(component);
}

JLRESULT JAttempter::sendMessage(IJComponent *component, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    return componentNotify.sendMessage(component, id, wParam, lParam);
}

void JAttempter::postMessage(IJComponent *component, const std::string &id, JWPARAM wParam, JLPARAM lParam)
{
    componentNotify.postMessage(component, id, wParam, lParam);
}

void JAttempter::postMessage(IJComponent *component, const std::string &id, const std::string &msg)
{
    componentNotify.postMessage(component, id, msg);
}

IJAttempter &JAttempter::beginGroup(IJComponent *obs, int offset)
{
    //
    componentNotify.beginGroup(obs, offset);
    return *this;
}

IJAttempter &JAttempter::subMessage(const std::string &id, JMsgSinkCb cb)
{
    componentNotify.subMessage(id, cb);
    return *this;
}

bool JAttempter::loadConfig()
{
    // 打开框架全局配置文件
    QFile file(QString::fromStdString(jframeFacade()->frameGlobalPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("框架全局配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(reinterpret_cast<QWidget *>(q_mainWindow->mainWidget()),
                             QStringLiteral("警告"), text);
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
        const QString text = QStringLiteral("框架全局配置文件\"%1\"解析失败！\n"
                                            "错误描述：%2\n"
                                            "错误位置：（行号：%3，列号：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(reinterpret_cast<QWidget *>(q_mainWindow->mainWidget()),
                             QStringLiteral("警告"), text);
        return false;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // 无效
    }

    // 获取工作模式节点
    QDomElement emWorkMode = emRoot.firstChildElement("workMode");
    if (emWorkMode.isNull()) {
        return false;   // 无效
    }

    // id
    q_workModeId = emWorkMode.attribute("id", 0).toInt();
    // name
    q_workModeName = emWorkMode.attribute("name").toStdString();
    // dir
    q_workModeDir = emWorkMode.attribute("dir").toStdString();

    return true;
}

bool JAttempter::loadInitComponent()
{
    const std::string dirPath =
            QDir(QString::fromStdString(jframeFacade()->thisDirPath()).append("/initcomponent"))
            .absolutePath().toStdString();
    if (!jframeFacade()->invokeMethod(
                "library_query_exists", 3, dirPath.c_str(), "InitComponent")) {
        return true;    // 如果不存，则忽略加载
    }

    //
    FuncInitComponent fInitComponent = 0;
    if (!jframeFacade()
            ->invokeMethod("library_resolve", 4, dirPath.c_str(),
                           "InitComponent", "InitComponent", &fInitComponent)) {
        QMessageBox::critical(reinterpret_cast<QWidget *>(q_mainWindow->mainWidget()),
                              "Error", QString("Cannot find symbol \"InitComponent\" in library InitComponent \n"
                                               "or it's dependent libraries can't be found!\n"
                                               "Where: %1")
                              .arg(QString::fromStdString(dirPath)));
        return false;
    }

    //
    return fInitComponent();
}

bool JAttempter::loadAllComponent()
{
    //
    const std::string currentWorkModeConfigDirName = this->currentWorkModeConfigDirName();
    std::string frameComponentPath = jframeFacade()->configDirPath();
    if (currentWorkModeConfigDirName.empty()) {
        frameComponentPath.append("/frame");
    } else {
        frameComponentPath.append("/").append(currentWorkModeConfigDirName);
    }
    frameComponentPath.append("/jframe_component.xml");

    // 打开框架组件配置文件
    QFile file(QString::fromStdString(frameComponentPath));
    if (!file.exists()) {
        const QString text = QStringLiteral("框架组件配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(reinterpret_cast<QWidget *>(q_mainWindow->mainWidget()),
                             QStringLiteral("警告"), text);
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
        const QString text = QStringLiteral("框架组件配置文件\"%1\"解析失败！\n"
                                            "错误描述：%2\n"
                                            "错误位置：（行号：%3，列号：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning((reinterpret_cast<QWidget *>(q_mainWindow->mainWidget())),
                             QStringLiteral("警告"), text);
        return false;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // 无效
    }

    //
    for (QDomElement emComponent = emRoot.firstChildElement("component");
         !emComponent.isNull();
         emComponent = emComponent.nextSiblingElement("component")) {
        // load
        if (!QVariant(emComponent.attribute("load", "false")).toBool()) {
            continue;   // 不允许加载
        }
        JComponentConfig componentConfig;
        // name
        componentConfig.componentName = emComponent.attribute("name").toUtf8().trimmed();
        if (componentConfig.componentName.isEmpty()) {
            continue;   // 无效
        }
        // 检测组件重复性加载问题
        if (q_mapComponent.contains(componentConfig.componentName)) {
            continue;   // 已加载
        }
        // desc
        componentConfig.componentDesc = emComponent.attribute("desc").toUtf8().trimmed();
        if (componentConfig.componentDesc.isEmpty()) {
            continue;   // 无效
        }
        // dir
        QString componentDir = emComponent.attribute("dir").toUtf8().trimmed();
        if (componentDir.isEmpty()) {
            componentDir = QString::fromStdString(jframeFacade()->thisDirPath()).append("/component");
        } else {
            componentDir.replace("@FrameDir@", QString::fromStdString(jframeFacade()->frameDirPath()));
            componentDir.replace("@ThisDir@", QString::fromStdString(jframeFacade()->thisDirPath()));
        }
        componentDir.append("/").append(componentConfig.componentName);
        componentConfig.componentDir = componentDir;
        // stay
        componentConfig.stay = QVariant(emComponent.attribute("stay", "false")).toBool();
        // type
        componentConfig.componentType = emComponent.attribute("type").toUtf8().trimmed();
        //
        if (componentConfig.componentType == "mfc") {
            const QString msg = QStringLiteral("MFC类型组件未加载！(原因：非MFC框架) "
                                               "组件信息：[路径: %1]；[名称: %2]；[描述: %3]")
                                .arg(componentConfig.componentDir)
                                .arg(componentConfig.componentName)
                                .arg(componentConfig.componentDesc);
#ifdef _AFXDLL
            if (!AfxGetApp()) {
                jframeLogWarning(msg.toLocal8Bit().data());
                continue;       // 未启用MFC框架，忽略加载
            }
#else
            jframeLogWarning(msg.toLocal8Bit().data());
            continue;           // 未启用MFC框架，忽略加载
#endif
        }
        // 加载信息显示
        QString msg = QStringLiteral("正在加载#").append(componentConfig.componentDesc).append("#...");
        q_mainWindow->updateSplashInfo(msg.toStdString());
        // 加载组件
        if (!loadComponent(componentConfig)) {
            continue;   // 加载失败
        }
    }

    return true;
}

bool JAttempter::loadComponent(JComponentConfig &componentConfig)
{
    // 文件有效性检测
    if (!jframeFacade()->invokeMethod("library_query_exists", 2,
                                      componentConfig.componentDir.toLocal8Bit().data(),
                                      componentConfig.componentName.toLocal8Bit().data())) {
        return true;    // 如果不存，则忽略加载
    }

    //
    FuncCreateComponent fCreateComponent = 0;
    if (!jframeFacade()
            ->invokeMethod("library_resolve", 4,
                           componentConfig.componentDir.toLocal8Bit().data(),
                           componentConfig.componentName.toLocal8Bit().data(),
                           "CreateComponent", &fCreateComponent)) {
        QMessageBox::critical(reinterpret_cast<QWidget *>(q_mainWindow->mainWidget()),
                              "Error", QString("Cannot find symbol \"CreateComponent\" in library %1 \n"
                                               "or it's dependent libraries can't be found!\n"
                                               "Where: %2")
                              .arg(componentConfig.componentName)
                              .arg(componentConfig.componentDir));
        return false;
    }

    // 创建组件
    IJComponent *component = (IJComponent *)fCreateComponent(static_cast<IJAttempter *>(this));
    if (!component) {
        return false;   // 创建失败
    }
    // 组件类型检测
    if (component->componentType() == "mfc") {
        const QString msg = QStringLiteral("MFC类型组件未加载！(原因：非MFC框架) "
                                           "组件信息：[路径: %1]；[名称: %2]；[描述: %3]")
                            .arg(componentConfig.componentDir)
                            .arg(componentConfig.componentName)
                            .arg(componentConfig.componentDesc);
#ifdef _AFXDLL
        if (!AfxGetApp()) {
            jframeLogWarning(msg.toLocal8Bit().data());
            delete component;
            return false;       // 未启用MFC框架，忽略加载
        }
#else
        jframeLogWarning(msg.toLocal8Bit().data());
        delete component;
        return false;       // 未启用MFC框架，忽略加载
#endif
    }
    // 存储组件信息
    componentConfig.component = component;
    q_mapComponent[componentConfig.componentName] = componentConfig;

    // 创建组件界面
    q_mainWindow->createComponentUi(component, (componentConfig.componentDir + "/"
                                                + componentConfig.componentName + ".xml")
                                    .toStdString());

    // 加载组件资源
    if (!component->loadInterface()) {
        return false;
    }

    // 挂载组件到框架布局系统
    JFrameLayout::getInstance()->invokeMethod(
                "attach_component", 2, component, componentConfig.stay);

    return true;
}

bool JAttempter::releaseAllComponent()
{
    //
    QMapIterator<QString, JComponentConfig> citer(q_mapComponent);
    citer.toBack();
    while (citer.hasPrevious()) {
        citer.previous();
        const JComponentConfig &componentConfig = citer.value();
        // 卸载信息显示
        QString msg = QStringLiteral("正在卸载#")
                .append(componentConfig.componentDesc).append("#...");
        q_mainWindow->updateSplashInfo(msg.toLocal8Bit().data());

        // 卸载组件资源
        componentConfig.component->releaseInterface();

        // 从框架布局系统卸载组件
        jframeLayout()->invokeMethod("detach_component", 1, componentConfig.component);

        // 释放
        delete componentConfig.component;
    }

    //
    q_mapComponent.clear();

    return true;
}

void JAttempter::commandSink(QObject *sender, const std::string &eventType, void *data)
{
    if (!sender) {
        Q_ASSERT(false);    //
        return;
    }

    QMapIterator<QString, JComponentConfig> citer(q_mapComponent);
    while (citer.hasNext()) {
        citer.next();
        const JComponentConfig &componentConfig = citer.value();
        IJCommandSink *cmdSink = (IJCommandSink *)componentConfig.component
                ->queryInterface(IID_IJCommandSink, VER_IJCommandSink);
        if (!cmdSink) {
            continue;   //
        }
        // domain
        std::string domain;
        const QVariant variant = sender->property("domain");
        if (variant.isValid()) {
            domain = variant.toString().toStdString();
        } else {
            domain = componentConfig.componentName.toStdString();
        }
        //
        if (cmdSink->commandSink(sender, domain, sender->objectName().toStdString(),
                                 eventType, data)) {
            break;      // truncate
        }
    }
}
