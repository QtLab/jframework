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

    QPushButton *buttonAdd = new QPushButton(QStringLiteral("增加组件"), widgetLeft);
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

    // 获取组件配置文件路径
    QString filePath = componentConfigPath();
    if (filePath.isEmpty()) {
        return false;
    }

    // 打开框架全局配置文件
    QFile file(filePath);
    if (!file.exists()) {
        const QString text = QStringLiteral("组件配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(this, QStringLiteral("警告"), text);
        return false;   // 文件不存在
    }

    // 打开文件
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;   // 打开失败
    }

    // 解析文件
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text =
                QStringLiteral("组件配置文件\"%1\"解析失败！\n错误描述：%2\n错误位置：（行号：%3，列号：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(this, QStringLiteral("警告"), text);
        return false;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return false;   // 获取失败
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
    // 打开框架全局配置文件
    QFile file(QString::fromStdString(jframeFacade()->configDirPath() + "/frame/jframe_global.xml"));
    if (!file.exists()) {
        const QString text = QStringLiteral("框架全局配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(this, QStringLiteral("警告"), text);
        return QString::null;   // 文件不存在
    }

    // 打开文件
    if (!file.open(QFile::ReadOnly)) {
        return QString::null;   // 打开失败
    }

    // 解析文件
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        const QString text =
                QStringLiteral("框架全局配置文件\"%1\"解析失败！\n错误描述：%2\n错误位置：（行号：%3，列号：%4）")
                .arg(file.fileName())
                .arg(errorMsg).arg(errorLine).arg(errorColumn);
        QMessageBox::warning(this, QStringLiteral("警告"), text);
        return QString::null;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return QString::null;   // 获取失败
    }

    // 获取 workMode 节点
    QDomElement emWorkMode = emRoot.firstChildElement("workMode");
    if (emWorkMode.isNull()) {
        return QString::null;   // 获取失败
    }

    // 读取 dir 属性
    QString path = QString::fromStdString(jframeFacade()
                                          ->parsePath(emWorkMode.attribute("dir").toStdString()));
    if (path.isEmpty()) {
        return QString::null;
    }

    //
    return path.append("/jframe_component.xml");
}
