#include "precomp.h"
#include "jattempter.h"
#include "jmain_window.h"
#include "jframe_facade.h"

// class JAttempter

JAttempter::JAttempter()
    : q_notifier(0)
    , q_mainWindow(0)
    , q_workModeId(0)
{
    // ������Ϣ�ַ���ʵ��
    q_notifier = JFRAME_FACTORY_CREATE(INotifier);

    //
    q_mainWindow = new JMainWindow(this);
}

JAttempter::~JAttempter()
{
    // ������Ϣ�ַ���ʵ��
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

    // ����������Ϣ
    result = result && loadConfig();

    // ���س�ʼ�����
    result = result && loadInitComponent();

    // ��ʼ��������
    result = result && q_mainWindow->loadInterface();

    return result;
}

void JAttempter::releaseInterface()
{
    // �µ��������
    releaseAllComponent();

    // �ͷ���Ϣ�ַ����ӿ�
    q_notifier->releaseInterface();

    // �ͷſ��������
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

    // �����������
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
    // �������
    QMap<QString, JComponentConfig>::const_iterator citer =
            q_mapComponent.find(QString::fromStdString(componentName));
    if (citer == q_mapComponent.end()) {
        return 0;   // ������
    }

    return citer.value().component;
}

IJMainWindow *JAttempter::mainWindow()
{
    return q_mainWindow;
}

void *JAttempter::queryInterface(const std::string &componentName, const std::string &iid, unsigned int ver)
{
    // ��ѯ���
    IJComponent *component = queryComponent(componentName);
    if (!component) {
        return 0;   // ������
    }

    // ��ѯ����ӿ�
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

const char *JAttempter::currentWorkModeName() const
{
    return q_workModeName.c_str();
}

const char *JAttempter::currentWorkModeConfigDirName() const
{
    return q_workModeDir.c_str();
}

void JAttempter::subMessage(IJComponent *component, unsigned int id)
{
    Q_UNUSED(component);
    Q_UNUSED(id);
}

void JAttempter::unsubMessage(IJComponent *component, unsigned int id)
{
    Q_UNUSED(component);
    Q_UNUSED(id);
}

void JAttempter::sendMessage(IJComponent *component, unsigned int id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(component);
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);
}

void JAttempter::postMessage(IJComponent *component, unsigned int id, JWPARAM wParam, JLPARAM lParam)
{
    Q_UNUSED(component);
    Q_UNUSED(id);
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);
}

INotifier *JAttempter::notifier()
{
    return q_notifier;
}

bool JAttempter::loadConfig()
{
    // �򿪿��ȫ�������ļ�
    QFile file(QString::fromStdString(jframeFacade()->frameGlobalPath()));
    if (!file.exists()) {
        const QString text = QStringLiteral("���ȫ�������ļ�\"%1\"�����ڣ�")
                .arg(file.fileName());
        QMessageBox::warning(reinterpret_cast<QWidget *>(q_mainWindow->mainWidget()),
                             QStringLiteral("����"), text);
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
        const QString text = QStringLiteral("���ȫ�������ļ�\"%1\"����ʧ�ܣ�\n"
                                            "����������%2\n"
                                            "����λ�ã����кţ�%3���кţ�%4��")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(reinterpret_cast<QWidget *>(q_mainWindow->mainWidget()),
                             QStringLiteral("����"), text);
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

bool JAttempter::loadInitComponent()
{
    const std::string dirPath = jframeFacade()->appDirPath()
            .append("/../initcomponent");
    if (!jframeFacade()->invokeMethod(
                "library_query_exists", 3, dirPath.c_str(), "InitComponent")) {
        return true;    // ������棬����Լ���
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
    std::string frameComponentPath = jframeFacade()->frameConfigPath();
    if (currentWorkModeConfigDirName.empty()) {
        frameComponentPath.append("/frame");
    } else {
        frameComponentPath.append("/").append(currentWorkModeConfigDirName);
    }
    frameComponentPath.append("/jframe_component.xml");

    // �򿪿����������ļ�
    QFile file(QString::fromStdString(frameComponentPath));
    if (!file.exists()) {
        const QString text = QStringLiteral("�����������ļ�\"%1\"�����ڣ�")
                .arg(file.fileName());
        QMessageBox::warning(reinterpret_cast<QWidget *>(q_mainWindow->mainWidget()),
                             QStringLiteral("����"), text);
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
        const QString text = QStringLiteral("�����������ļ�\"%1\"����ʧ�ܣ�\n"
                                            "����������%2\n"
                                            "����λ�ã����кţ�%3���кţ�%4��")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning((reinterpret_cast<QWidget *>(q_mainWindow->mainWidget())),
                             QStringLiteral("����"), text);
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
        JComponentConfig componentConfig;
        // name
        componentConfig.componentName = emComponent.attribute("name").trimmed();
        if (componentConfig.componentName.isEmpty()) {
            continue;   // ��Ч
        }
        // �������ظ��Լ�������
        if (q_mapComponent.contains(componentConfig.componentName)) {
            continue;   // �Ѽ���
        }
        // desc
        componentConfig.componentDesc = emComponent.attribute("desc").trimmed();
        if (componentConfig.componentDesc.isEmpty()) {
            continue;   // ��Ч
        }
        // stay
        componentConfig.stay = QVariant(emComponent.attribute("stay", "false")).toBool();
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
        componentDir.append("/").append(componentConfig.componentName);
        componentConfig.componentDir = componentDir;
        // ������Ϣ��ʾ
        QString msg = QStringLiteral("���ڼ���#").append(componentConfig.componentDesc).append("#...");
        q_mainWindow->updateSplashInfo(msg.toLocal8Bit().data());
        // �������
        if (!loadComponent(componentConfig)) {
            continue;   // ����ʧ��
        }
    }

    return true;
}

bool JAttempter::loadComponent(JComponentConfig &componentConfig)
{
    // �ļ���Ч�Լ��
    if (!jframeFacade()->invokeMethod("library_query_exists", 2,
                                      componentConfig.componentDir.toLocal8Bit().data(),
                                      componentConfig.componentName.toLocal8Bit().data())) {
        return true;    // ������棬����Լ���
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

    // �������
    IJComponent *component = (IJComponent *)fCreateComponent(static_cast<IJAttempter *>(this));
    if (!component) {
        return false;   // ����ʧ��
    }

    // �洢�����Ϣ
    componentConfig.component = component;
    q_mapComponent[componentConfig.componentName] = componentConfig;

    // �����������
    q_mainWindow->createComponentUi(component, (componentConfig.componentDir + "/"
                                                + componentConfig.componentName + ".xml")
                                    .toStdString());

    // ���������Դ
    if (!component->loadInterface()) {
        return false;
    }

    // �����������ܲ���ϵͳ
    jframeLayout()->invokeMethod("attach_component", 2, component, componentConfig.stay);

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
        // ж����Ϣ��ʾ
        QString msg = QStringLiteral("����ж��#")
                .append(componentConfig.componentDesc).append("#...");
        q_mainWindow->updateSplashInfo(msg.toLocal8Bit().data());

        // ж�������Դ
        componentConfig.component->releaseInterface();

        // �ӿ�ܲ���ϵͳж�����
        jframeLayout()->invokeMethod("detach_component", 1, componentConfig.component);

        // �ͷ�
        delete componentConfig.component;
    }

    //
    q_mapComponent.clear();

    return true;
}

void JAttempter::commandSink(void *sender, const std::string &senderName)
{
    QMapIterator<QString, JComponentConfig> citer(q_mapComponent);
    while (citer.hasNext()) {
        citer.next();
        const JComponentConfig &componentConfig = citer.value();
        IJCommandSink *cmdSink = (IJCommandSink *)componentConfig.component
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
