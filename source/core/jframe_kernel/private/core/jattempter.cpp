#include "precomp.h"
#include "jattempter.h"
#include "jmain_window.h"
#include "jframe_facade.h"

// class JAttempter

JAttempter::JAttempter()
    : q_mainWindow(0)
    , q_workModeId(0)
{
    //
    q_mainWindow = new JMainWindow(this);

    //
}

JAttempter::~JAttempter()
{
    if (q_mainWindow) {
        delete q_mainWindow;
        q_mainWindow = 0;
    }
}

bool JAttempter::init()
{
    bool result = true;

    // 加载配置信息
    result = result && loadConfig();

    // 加载初始化组件
    result = result && loadInitComponent();

    // 初始化主窗口
    result = result && dynamic_cast<JMainWindow *>(q_mainWindow)->init();

    //
    JwtCore::instance()->init();
    JwtCore::instance()->loadSystemLang();
    //qApp->setStyleSheet(JStyleSheet::instance()->styleSheet("default"));

    return result;
}

void JAttempter::releaseInterface()
{
    // 下电所有组件
    shutdownAllComponent();

    // 释放框架主窗口
    if (q_mainWindow) {
        q_mainWindow->releaseInterface();
    }
}

void *JAttempter::queryInterface(const char *iid, unsigned int ver)
{
    J_QUERY_INTERFACE(IJObject, iid, ver);

    return 0;
}

std::string JAttempter::objectIdentity() const
{
    return IID_IJAttempter;
}

unsigned int JAttempter::objectVersion() const
{
    return VER_IJAttempter;
}

bool JAttempter::invoke(const char *method, int argc)
{
    if (!method) {
        return false;
    }

    bool result = false;
    va_list ap;
    va_start(ap, argc);

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

void JAttempter::shutdownComponent()
{
    //
    if (!shutdownAllComponent()) {
        //
    }
}

IJComponent *JAttempter::queryComponent(const char *componentId)
{
    // 参数有效性检测
    if (!componentId) {
        return 0;   // 无效
    }

    // 查找组件
    QMap<QString, JComponentInfo>::const_iterator citer =
            q_mapComponent.find(QString::fromLatin1(componentId));
    if (citer == q_mapComponent.end()) {
        return 0;   // 不存在
    }

    return citer.value().component;
}

IJMainWindow *JAttempter::mainWindow()
{
    return q_mainWindow;
}

void *JAttempter::queryInterface(const char *componentId, const char *iid, unsigned int ver)
{
    // 查询组件
    IJComponent *component = queryComponent(componentId);
    if (!component) {
        return 0;   // 不存在
    }

    // 查询组件接口
    return component->queryInterface(iid, ver);
}

std::list<IJComponent *> JAttempter::allComponents() const
{
    std::list<IJComponent *> components;
    QMapIterator<QString, JComponentInfo> citer(q_mapComponent);
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

const char *JAttempter::currentWorkModeName() const
{
    return q_workModeName.c_str();
}

const char *JAttempter::currentWorkModeConfigDirName() const
{
    return q_workModeDir.c_str();
}

bool JAttempter::loadInitComponent()
{
    const std::string dirPath = jframeFacade()->appDirPath()
            .append("/../initcomponent");
    if (!jframeFacade()->invoke("library_query_exists", 3, dirPath.c_str(), "InitComponent")) {
        return true;    // 如果不存，则忽略加载
    }

    //
    FuncInitComponent fInitComponent = 0;
    if (!jframeFacade()
            ->invoke("library_resolve", 4, dirPath.c_str(),
                     "InitComponent", "InitComponent", &fInitComponent)) {
        QMessageBox::critical(0, "Error",
                              QString("Cannot find symbol \"InitComponent\" in library InitComponent \n"
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
    std::string frameComponentPath = jframeFacade()->frameConfigPath();
    if (currentWorkModeConfigDirName.empty()) {
        frameComponentPath.append("/frame");
    } else {
        frameComponentPath.append("/").append(currentWorkModeConfigDirName);
    }
    frameComponentPath.append("/jframe_component.xml");

    // 打开框架组件配置文件
    QFile file(QString::fromStdString(frameComponentPath));
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
                                            "错误位置：（行号：%3，列好：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(0, QStringLiteral("警告"), text);
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
        // name
        const QString componentName = emComponent.attribute("name").trimmed();
        if (componentName.isEmpty()) {
            continue;   // 无效
        }
        // 检测组件重复性加载问题
        if (q_mapComponent.contains(componentName)) {
            continue;   // 已加载
        }
        // desc
        const QString componentDesc = emComponent.attribute("desc").trimmed();
        if (componentDesc.isEmpty()) {
            continue;   // 无效
        }
        // dir
        QString componentDir = emComponent.attribute("dir").trimmed();
        if (componentDir.isEmpty()) {
            componentDir = QString::fromStdString(jframeFacade()->appDirPath()).append("/../component");
        } else if (componentDir.startsWith("$(jframe)")) {
            componentDir.replace("$(jframe)", QString::fromStdString(jframeFacade()->frameDirPath()));
        } else if (componentDir.startsWith("$(app)")) {
            componentDir.replace("$(app)", QString::fromStdString(jframeFacade()->appDirPath()).append("/.."));
        } else {
            componentDir.prepend(QString::fromStdString(jframeFacade()->appDirPath()).append("/../"));
        }
        componentDir.append("/").append(componentName);
        // 加载信息显示
        QString msg = QStringLiteral("正在加载#").append(componentDesc).append("#...");
        q_mainWindow->updateSplashInfo(msg.toLocal8Bit().data());
        // 加载组件
        if (!loadComponent(componentDir, componentName, componentDesc)) {
            continue;   // 加载失败
        }
    }

    return true;
}

bool JAttempter::shutdownAllComponent()
{
    //
    QMapIterator<QString, JComponentInfo> citer(q_mapComponent);
    citer.toBack();
    while (citer.hasPrevious()) {
        citer.previous();
        const JComponentInfo &componentInfo = citer.value();
        // 卸载信息显示
        QString msg = QStringLiteral("正在加载#").append(componentInfo.componentDesc).append("#...");
        q_mainWindow->updateSplashInfo(msg.toLocal8Bit().data());
        componentInfo.component->shutdown();
        componentInfo.component->releaseInterface();
        delete componentInfo.component;
    }

    //
    q_mapComponent.clear();

    return true;
}

bool JAttempter::loadComponent(const QString &componentDir,
                               const QString &componentName,
                               const QString &componentDesc)
{
    // 文件有效性检测
    if (!jframeFacade()->invoke("library_query_exists", 2,
                                componentDir.toLocal8Bit().data(),
                                componentName.toLocal8Bit().data())) {
        return true;    // 如果不存，则忽略加载
    }

    //
    FuncCreateComponent fCreateComponent = 0;
    if (!jframeFacade()
            ->invoke("library_resolve", 4,
                     componentDir.toLocal8Bit().data(),
                     componentName.toLocal8Bit().data(),
                     "CreateComponent", &fCreateComponent)) {
        QMessageBox::critical(0, "Error",
                              QString("Cannot find symbol \"CreateComponent\" in library %1 \n"
                                      "or it's dependent libraries can't be found!\n"
                                      "Where: %2")
                              .arg(componentName)
                              .arg(componentDir));
        return false;
    }

    // 创建组件
    IJComponent *component = (IJComponent *)fCreateComponent(static_cast<IJAttempter *>(this));
    if (!component) {
        return false;   // 创建失败
    }

    // 存储组件信息
    JComponentInfo componentInfo;
    componentInfo.componentDir = componentDir;
    componentInfo.componentName = componentName;
    componentInfo.componentDesc = componentDesc;
    componentInfo.component = component;
    q_mapComponent[componentName] = componentInfo;

    // 组件初始化
    component->initialize();

    // 创建组件界面
    q_mainWindow->createComponentUi(component, QString(componentName)
                                    .append(".xml").toLocal8Bit().data());

    return true;
}

bool JAttempter::loadConfig()
{
    // 打开框架全局配置文件
    QFile file(QString::fromStdString(jframeFacade()->frameGlobalPath()));
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
                                            "错误位置：（行号：%3，列好：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(0, QStringLiteral("警告"), text);
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

void JAttempter::commandSink(void *sender, const char *senderName)
{
    QMapIterator<QString, JComponentInfo> citer(q_mapComponent);
    while (citer.hasNext()) {
        citer.next();
        const JComponentInfo &componentInfo = citer.value();
        IJCommandSink *cmdSink = (IJCommandSink *)componentInfo.component
                ->queryInterface(IID_IJCommandSink, VER_IJCommandSink);
        if (!cmdSink) {
            continue;   //
        }
        //
        if (!cmdSink->commandSink(sender, senderName)) {
            break;      //
        }
    }
}
