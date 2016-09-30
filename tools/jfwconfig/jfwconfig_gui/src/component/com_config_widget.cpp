#include "precomp.h"
#include "com_config_widget.h"
#include "component_detail.h"

ComConfigWidget::ComConfigWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(0, 0, 0, 0);

    JSplitter *splitterMain = new JSplitter(this);
    splitterMain->setScales(QList<double>() << 1 << 2.5);
    horiLayoutMain->addWidget(splitterMain);

    QWidget *widgetLeft = new QWidget(this);
    splitterMain->addWidget(widgetLeft);

    QVBoxLayout *vertLayoutLeft = new QVBoxLayout(widgetLeft);
    vertLayoutLeft->setContentsMargins(0, 0, 0, 0);
    vertLayoutLeft->setSpacing(1);

    JSplitter *splitterLeftTop = new JSplitter(Qt::Vertical, widgetLeft);
    splitterLeftTop->setScales(QList<double>() << 1 << 1.5);
    vertLayoutLeft->addWidget(splitterLeftTop);

    q_listWidgetDefCom = new QListWidget(widgetLeft);
    q_listWidgetUsrCom = new QListWidget(widgetLeft);
    splitterLeftTop->addWidget(q_listWidgetDefCom);
    splitterLeftTop->addWidget(q_listWidgetUsrCom);

    QPushButton *buttonAdd = new QPushButton(QStringLiteral("�������"), widgetLeft);
    vertLayoutLeft->addWidget(buttonAdd);

    q_componentDetail = new ComponentDetail(this);
    splitterMain->addWidget(q_componentDetail);

    //
    connect(buttonAdd, SIGNAL(clicked(bool)), this, SLOT(onButtonAddClicked()));
}

bool ComConfigWidget::appDirChanged(const QString &path)
{
    //
    q_listWidgetDefCom->clear();
    q_listWidgetUsrCom->clear();
    q_componentDetail->clearContent();

    // ��ȡ��������ļ�·��
    QString filePath = componentConfigPath();
    if (filePath.isEmpty()) {
        return false;
    }

    // �򿪿��ȫ�������ļ�
    QFile file(filePath);
    if (!file.exists()) {
        const QString text = QStringLiteral("��������ļ�\"%1\"�����ڣ�")
                .arg(file.fileName());
        QMessageBox::warning(this, QStringLiteral("����"), text);
        return false;   // �ļ�������
    }

    // ���ļ�
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;   // ��ʧ��
    }

    // �����ļ�
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text =
                QStringLiteral("��������ļ�\"%1\"����ʧ�ܣ�\n����������%2\n����λ�ã����кţ�%3���кţ�%4��")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(this, QStringLiteral("����"), text);
        return false;
    }

    // �ر��ļ�
    file.close();

    // ��ȡ���ڵ�
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // ��ȡʧ��
    }

    //
    QString componentName, componentDesc, itemText;
    for (QDomElement emComponent = emRoot.firstChildElement("component");
         !emComponent.isNull();
         emComponent = emComponent.nextSiblingElement("component")) {
        //
        componentName = emComponent.attribute("name").trimmed();
        componentDesc = emComponent.attribute("desc").trimmed();
        itemText = componentName + " (" + componentDesc + ")";
        if (emComponent.attribute("dir").contains("@FrameDir@")) {
            q_listWidgetDefCom->addItem(itemText);
        } else {
            q_listWidgetUsrCom->addItem(itemText);
        }
    }

    return true;
}

void ComConfigWidget::onButtonAddClicked()
{

}

QString ComConfigWidget::componentConfigPath()
{
    // �򿪿��ȫ�������ļ�
    QFile file(QString::fromStdString(jframeFacade()->configDirPath() + "/frame/jframe_global.xml"));
    if (!file.exists()) {
        const QString text = QStringLiteral("���ȫ�������ļ�\"%1\"�����ڣ�")
                .arg(file.fileName());
        QMessageBox::warning(this, QStringLiteral("����"), text);
        return QString::null;   // �ļ�������
    }

    // ���ļ�
    if (!file.open(QFile::ReadOnly)) {
        return QString::null;   // ��ʧ��
    }

    // �����ļ�
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text =
                QStringLiteral("���ȫ�������ļ�\"%1\"����ʧ�ܣ�\n����������%2\n����λ�ã����кţ�%3���кţ�%4��")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(this, QStringLiteral("����"), text);
        return QString::null;
    }

    // �ر��ļ�
    file.close();

    // ��ȡ���ڵ�
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return QString::null;   // ��ȡʧ��
    }

    // ��ȡ workMode �ڵ�
    QDomElement emWorkMode = emRoot.firstChildElement("workMode");
    if (emWorkMode.isNull()) {
        return QString::null;   // ��ȡʧ��
    }

    // ��ȡ dir ����
    QString path = QString::fromStdString(jframeFacade()
                                          ->parsePath(emWorkMode.attribute("dir").toStdString()));
    if (path.isEmpty()) {
        return QString::null;
    }

    //
    return path.append("/jframe_component.xml");
}
