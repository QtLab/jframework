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

    // ����������Ϣ
    result = result && loadConfig();

    // ���س�ʼ�����
    result = result && loadInitComponent();

    // ��ʼ��������
    result = result && dynamic_cast<JMainWindow *>(q_mainWindow)->init();

    //
    JwtCore::instance()->init();
    JwtCore::instance()->loadSystemLang();
    //qApp->setStyleSheet(JStyleSheet::instance()->styleSheet("default"));

    return result;
}

void JAttempter::releaseInterface()
{
    // �µ��������
    shutdownAllComponent();

    // �ͷſ��������
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

    // �����������
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
    // ������Ч�Լ��
    if (!componentId) {
        return 0;   // ��Ч
    }

    // �������
    QMap<QString, JComponentInfo>::const_iterator citer =
            q_mapComponent.find(QString::fromLatin1(componentId));
    if (citer == q_mapComponent.end()) {
        return 0;   // ������
    }

    return citer.value().component;
}

IJMainWindow *JAttempter::mainWindow()
{
    return q_mainWindow;
}

void *JAttempter::queryInterface(const char *componentId, const char *iid, unsigned int ver)
{
    // ��ѯ���
    IJComponent *component = queryComponent(componentId);
    if (!component) {
        return 0;   // ������
    }

    // ��ѯ����ӿ�
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
        return true;    // ������棬����Լ���
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

    // �򿪿����������ļ�
    QFile file(QString::fromStdString(frameComponentPath));
    if (!file.open(QFile::ReadWrite)) {
        return false;   // ��ʧ��
    }

    // �����ļ�
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("�����������ļ�\"%1\"����ʧ�ܣ�\n"
                                            "����������%2\n"
                                            "����λ�ã����кţ�%3���кã�%4��")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(0, QStringLiteral("����"), text);
        return false;
    }

    // �ر��ļ�
    file.close();

    // ��ȡ���ڵ�
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // ��Ч
    }

    //
    for (QDomElement emComponent = emRoot.firstChildElement("component");
         !emComponent.isNull();
         emComponent = emComponent.nextSiblingElement("component")) {
        // load
        if (!QVariant(emComponent.attribute("load", "false")).toBool()) {
            continue;   // ���������
        }
        // name
        const QString componentName = emComponent.attribute("name").trimmed();
        if (componentName.isEmpty()) {
            continue;   // ��Ч
        }
        // �������ظ��Լ�������
        if (q_mapComponent.contains(componentName)) {
            continue;   // �Ѽ���
        }
        // desc
        const QString componentDesc = emComponent.attribute("desc").trimmed();
        if (componentDesc.isEmpty()) {
            continue;   // ��Ч
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
        // ������Ϣ��ʾ
        QString msg = QStringLiteral("���ڼ���#").append(componentDesc).append("#...");
        q_mainWindow->updateSplashInfo(msg.toLocal8Bit().data());
        // �������
        if (!loadComponent(componentDir, componentName, componentDesc)) {
            continue;   // ����ʧ��
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
        // ж����Ϣ��ʾ
        QString msg = QStringLiteral("���ڼ���#").append(componentInfo.componentDesc).append("#...");
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
    // �ļ���Ч�Լ��
    if (!jframeFacade()->invoke("library_query_exists", 2,
                                componentDir.toLocal8Bit().data(),
                                componentName.toLocal8Bit().data())) {
        return true;    // ������棬����Լ���
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

    // �������
    IJComponent *component = (IJComponent *)fCreateComponent(static_cast<IJAttempter *>(this));
    if (!component) {
        return false;   // ����ʧ��
    }

    // �洢�����Ϣ
    JComponentInfo componentInfo;
    componentInfo.componentDir = componentDir;
    componentInfo.componentName = componentName;
    componentInfo.componentDesc = componentDesc;
    componentInfo.component = component;
    q_mapComponent[componentName] = componentInfo;

    // �����ʼ��
    component->initialize();

    // �����������
    q_mainWindow->createComponentUi(component, QString(componentName)
                                    .append(".xml").toLocal8Bit().data());

    return true;
}

bool JAttempter::loadConfig()
{
    // �򿪿��ȫ�������ļ�
    QFile file(QString::fromStdString(jframeFacade()->frameGlobalPath()));
    if (!file.open(QFile::ReadWrite)) {
        return false;   // ��ʧ��
    }

    // �����ļ�
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text = QStringLiteral("���ȫ�������ļ�\"%1\"����ʧ�ܣ�\n"
                                            "����������%2\n"
                                            "����λ�ã����кţ�%3���кã�%4��")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(0, QStringLiteral("����"), text);
        return false;
    }

    // �ر��ļ�
    file.close();

    // ��ȡ���ڵ�
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // ��Ч
    }

    // ��ȡ����ģʽ�ڵ�
    QDomElement emWorkMode = emRoot.firstChildElement("workMode");
    if (emWorkMode.isNull()) {
        return false;   // ��Ч
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
