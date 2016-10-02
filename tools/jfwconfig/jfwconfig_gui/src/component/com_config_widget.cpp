#include "precomp.h"
#include "com_config_widget.h"
#include "component_detail.h"
#include "jfiltertableview.h"

ComConfigWidget::ComConfigWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(0, 0, 0, 0);

    JSplitter *splitterMain = new JSplitter(this);
    splitterMain->setScales(QList<double>() << 1 << 1.2);
    horiLayoutMain->addWidget(splitterMain);

    QWidget *widgetLeft = new QWidget(this);
    splitterMain->addWidget(widgetLeft);

    QVBoxLayout *vertLayoutLeft = new QVBoxLayout(widgetLeft);
    vertLayoutLeft->setContentsMargins(0, 0, 0, 0);
    vertLayoutLeft->setSpacing(1);

    JSplitter *splitterLeftTop = new JSplitter(Qt::Vertical, widgetLeft);
    splitterLeftTop->setScales(QList<double>() << 1 << 1.5);
    vertLayoutLeft->addWidget(splitterLeftTop);

    q_tableViewDefCom = new JFilterTableView(widgetLeft);
    q_tableViewUsrCom = new JFilterTableView(widgetLeft);
    splitterLeftTop->addWidget(q_tableViewDefCom);
    splitterLeftTop->addWidget(q_tableViewUsrCom);

    QPushButton *buttonAdd = new QPushButton(QStringLiteral("增加组件"), widgetLeft);
    vertLayoutLeft->addWidget(buttonAdd);

    q_componentDetail = new ComponentDetail(this);
    splitterMain->addWidget(q_componentDetail);

    //
    connect(q_tableViewDefCom->view(), SIGNAL(currentCellChanged(int,int,int,int)),
            this, SLOT(onTableViewDefComCellFocus(int)));
    connect(q_tableViewUsrCom->view(), SIGNAL(currentCellChanged(int,int,int,int)),
            this, SLOT(onTableViewUsrComCellFocus(int)));
    connect(q_tableViewUsrCom->view(), SIGNAL(cellPressed(int,int)),
            this, SLOT(onTableUsrCellPressed(int)));
    connect(q_tableViewDefCom->view(), SIGNAL(cellClicked(int,int)),
            this, SLOT(onTableViewDefComCellFocus(int)));
    connect(q_tableViewUsrCom->view(), SIGNAL(cellClicked(int,int)),
            this, SLOT(onTableViewUsrComCellFocus(int)));
    connect(buttonAdd, SIGNAL(clicked(bool)), this, SLOT(onButtonAddClicked()));

    connect(q_componentDetail, SIGNAL(newComponentAdded(QString,QString,bool)),
            this, SLOT(onNewComponentAdded(QString,QString,bool)));
    connect(q_componentDetail, SIGNAL(componentModify(QString,QString,bool)),
            this, SLOT(onComponentModify(QString,QString,bool)));

    connect(q_tableViewUsrCom->view()->verticalHeader(), SIGNAL(sectionMoved(int,int,int)),
            this, SLOT(onTableUsrSectionMoved(int,int,int)));

    //
    q_tableViewDefCom->setTitle(QStringLiteral("框架组件"));
    q_tableViewUsrCom->setTitle(QStringLiteral("用户组件"));
    q_tableViewDefCom->view()->setColumnCount(2);
    q_tableViewDefCom->view()->setHorizontalHeaderLabels(
                QStringList() << QStringLiteral("名称") << QStringLiteral("描述"));
    q_tableViewUsrCom->view()->setColumnCount(2);
    q_tableViewUsrCom->view()->setHorizontalHeaderLabels(
                QStringList() << QStringLiteral("名称") << QStringLiteral("描述"));
    q_tableViewUsrCom->view()->verticalHeader()->setSectionsMovable(true);
}

bool ComConfigWidget::appDirChanged(const QString &path)
{
    q_appDir = path;

    //
    q_tableViewDefCom->view()->clearContents();
    q_tableViewUsrCom->view()->clearContents();
    q_componentDetail->clearContents();

    // 获取组件配置文件路径
    QString comConfigPath = componentConfigPath();
    if (comConfigPath.isEmpty()) {
        return false;
    }

    // 打开框架全局配置文件
    QFile file(comConfigPath);
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
    QString componentName, componentDesc;
    int rowIndex = 0;
    JTableView *tableView = 0;
    for (QDomElement emComponent = emRoot.firstChildElement("component");
         !emComponent.isNull();
         emComponent = emComponent.nextSiblingElement("component")) {
        //
        componentName = emComponent.attribute("name").trimmed();
        componentDesc = emComponent.attribute("desc").trimmed();
        if (emComponent.attribute("dir").contains("@FrameDir@")) {
            tableView = q_tableViewDefCom->view();
        } else {
            tableView = q_tableViewUsrCom->view();
        }
        rowIndex = tableView->rowCount();
        tableView->insertRow(rowIndex);
        tableView->setItemData(rowIndex, 0, componentName);
        tableView->setItemData(rowIndex, 1, componentDesc);
    }

    //
    q_componentDetail->setComConfigPath(comConfigPath);

    return true;
}

void ComConfigWidget::onTableUsrCellPressed(int row)
{
    Qt::MouseButtons buttons = QApplication::mouseButtons();
    switch (buttons) {
    case Qt::RightButton:
    {
        QMenu menu(this);
        QAction *actionDelete = menu.addAction(QStringLiteral("Delete"), this, SLOT(onActionDeleteRow()));
        if (actionDelete) {
            actionDelete->setData(row);
        }
        menu.exec(QCursor::pos());
        break;
    }
    default:
        break;
    }
}

void ComConfigWidget::onTableViewDefComCellFocus(int row)
{
    //
    q_tableViewUsrCom->view()->clearSelection();

    //
    const QString componentName = q_tableViewDefCom->view()
            ->itemData(row, 0).toString();
    q_componentDetail->focusComponent(componentName, true);
}

void ComConfigWidget::onTableViewUsrComCellFocus(int row)
{
    //
    q_tableViewDefCom->view()->clearSelection();

    //
    const QString componentName = q_tableViewUsrCom->view()
            ->itemData(row, 0).toString();
    q_componentDetail->focusComponent(componentName, false);
}

void ComConfigWidget::onButtonAddClicked()
{
    //
    q_tableViewDefCom->view()->clearSelection();
    q_tableViewUsrCom->view()->clearSelection();

    //
    q_componentDetail->newComponent();
}

void ComConfigWidget::onNewComponentAdded(const QString &componentName,
                                          const QString &componentDesc, bool def)
{
    //
    JTableView *tableView = 0;
    if (def) {
        tableView = q_tableViewDefCom->view();
    } else {
        tableView = q_tableViewUsrCom->view();
    }
    int rowIndex = tableView->rowCount();
    tableView->insertRow(rowIndex);
    tableView->setItemData(rowIndex, 0, componentName);
    tableView->setItemData(rowIndex, 1, componentDesc);
}

void ComConfigWidget::onComponentModify(const QString &componentName,
                                        const QString &componentDesc, bool def)
{
    JTableView *tableView = 0;
    if (def) {
        tableView = q_tableViewDefCom->view();
    } else {
        tableView = q_tableViewUsrCom->view();
    }
    int rowIndex = tableView->currentRow();
    tableView->setItemData(rowIndex, 0, componentName);
    tableView->setItemData(rowIndex, 1, componentDesc);
}

void ComConfigWidget::onTableUsrSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    // 获取组件配置文件路径
    QString comConfigPath = componentConfigPath();
    if (comConfigPath.isEmpty()) {
        return;
    }

    // 打开框架全局配置文件
    QFile file(comConfigPath);
    if (!file.exists()) {
        const QString text = QStringLiteral("组件配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(this, QStringLiteral("警告"), text);
        return;   // 文件不存在
    }

    // 打开文件
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;   // 打开失败
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
        return;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return;     // 获取失败
    }

    //
    JTableView *tableView = q_tableViewUsrCom->view();
    const QString componentNameOld = tableView->itemData(logicalIndex, 0).toString();
    if (componentNameOld.isEmpty()) {
        return;     //
    }

    //
    QDomElement emComponentOld, emComponentNew;
    for (emComponentOld = emRoot.firstChildElement("component");
         !emComponentOld.isNull();
         emComponentOld = emComponentOld.nextSiblingElement("component")) {
        //
        if (emComponentOld.attribute("name").trimmed() == componentNameOld) {
            break;
        }
    }

    //
    if (emComponentOld.isNull()) {
        return;     //
    }

    //
    if (newVisualIndex == 0) {
        emComponentNew = emRoot.insertBefore(document.createElement("component"),
                                             emRoot.firstChild()).toElement();
    } else if (newVisualIndex == tableView->rowCount() - 1) {
        emComponentNew = emRoot.insertAfter(document.createElement("component"),
                                             emRoot.lastChild()).toElement();
    } else {
        //
        int newLogicalIndex = tableView->verticalHeader()->logicalIndex(newVisualIndex + 1);
        const QString componentNameNew = tableView->itemData(newLogicalIndex, 0).toString();
        if (componentNameNew.isEmpty()) {
            return;     //
        }
        //
        for (emComponentNew = emRoot.firstChildElement("component");
             !emComponentNew.isNull();
             emComponentNew = emComponentNew.nextSiblingElement("component")) {
            //
            if (emComponentNew.attribute("name").trimmed() == componentNameNew) {
                emComponentNew = emRoot.insertBefore(document.createElement("component"),
                                                     emComponentNew).toElement();
                break;
            }
        }
    }

    //
    if (emComponentNew.isNull()) {
        return;     //
    }

    //
    emRoot.removeChild(emComponentOld);

    // name
    if (emComponentOld.hasAttribute("name")) {
        emComponentNew.setAttribute("name", emComponentOld.attribute("name"));
    }
    // desc
    if (emComponentOld.hasAttribute("desc")) {
        emComponentNew.setAttribute("desc", emComponentOld.attribute("desc"));
    }
    // load
    if (emComponentOld.hasAttribute("load")) {
        emComponentNew.setAttribute("load", emComponentOld.attribute("load"));
    }
    // stay
    if (emComponentOld.hasAttribute("stay")) {
        emComponentNew.setAttribute("stay", emComponentOld.attribute("stay"));
    }
    // dir
    if (emComponentOld.hasAttribute("dir")) {
        emComponentNew.setAttribute("dir", emComponentOld.attribute("dir"));
    }
    // type
    if (emComponentOld.hasAttribute("type")) {
        emComponentNew.setAttribute("type", emComponentOld.attribute("type"));
    }

    //
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    //
    QTextStream textStream(&file);
    document.save(textStream, 2);
}

void ComConfigWidget::onActionDeleteRow()
{
    //
    QAction *actionDelete = qobject_cast<QAction *>(sender());
    if (!actionDelete) {
        return;     //
    }

    //
    JTableView *tableView = q_tableViewUsrCom->view();

    //
    int row = actionDelete->data().toInt();
    if (row < 0 || row >= tableView->rowCount()) {
        return;     //
    }

    //
    const QString componentName = tableView->itemData(row, 0).toString();
    if (componentName.isEmpty()) {
        return;     //
    }

    // 获取组件配置文件路径
    QString comConfigPath = componentConfigPath();
    if (comConfigPath.isEmpty()) {
        return;
    }

    // 打开框架全局配置文件
    QFile file(comConfigPath);
    if (!file.exists()) {
        const QString text = QStringLiteral("组件配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(this, QStringLiteral("警告"), text);
        return;   // 文件不存在
    }

    // 打开文件
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;   // 打开失败
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
        return;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return;     // 获取失败
    }

    //
    QDomElement emComponent;
    for (emComponent = emRoot.firstChildElement("component");
         !emComponent.isNull();
         emComponent = emComponent.nextSiblingElement("component")) {
        //
        if (emComponent.attribute("name").trimmed() == componentName) {
            break;
        }
    }

    //
    if (emComponent.isNull()) {
        return;     //
    }

    //
    emRoot.removeChild(emComponent);

    //
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    //
    QTextStream textStream(&file);
    document.save(textStream, 2);

    //
    tableView->removeRow(row);
}

QString ComConfigWidget::componentConfigPath()
{
    // 打开框架全局配置文件
    QFile file(q_appDir + "/config/frame/jframe_global.xml");
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
