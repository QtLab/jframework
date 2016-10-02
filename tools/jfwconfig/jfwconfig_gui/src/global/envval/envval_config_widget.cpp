#include "precomp.h"
#include "envval_config_widget.h"

EnvvalConfigWidget::EnvvalConfigWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);

    //
    QGroupBox *groupBoxGlobal = new QGroupBox(QStringLiteral("全局环境变量"), this);
    vertLayoutMain->addWidget(groupBoxGlobal);

    QVBoxLayout *vertLayoutGlobal = new QVBoxLayout(groupBoxGlobal);

    JSplitter *splitterGlobal = new JSplitter(Qt::Vertical, groupBoxGlobal);
    splitterGlobal->setScales(QList<double>() << 1 << 1.5);
    vertLayoutGlobal->addWidget(splitterGlobal);

    q_tablePaths = new JFilterTableView(groupBoxGlobal);
    splitterGlobal->addWidget(q_tablePaths);

    q_widgetDetail = new QWidget(groupBoxGlobal);
    splitterGlobal->addWidget(q_widgetDetail);

    QVBoxLayout *vertLayoutDetail = new QVBoxLayout(q_widgetDetail);
    vertLayoutDetail->setContentsMargins(0, 0, 0, 0);

    QFormLayout *formLayoutDetail = new QFormLayout();
    vertLayoutDetail->addLayout(formLayoutDetail);

    q_editBase = new QLineEdit(q_widgetDetail);
    formLayoutDetail->addRow(QStringLiteral("路径前缀："), q_editBase);

    q_comboBoxType = new QComboBox(q_widgetDetail);
    q_comboBoxType->addItem(QStringLiteral("前置"));
    q_comboBoxType->addItem(QStringLiteral("追加"));
    formLayoutDetail->addRow(QStringLiteral("路径追加方式："), q_comboBoxType);

    q_tableItems = new JFilterTableView(q_widgetDetail);
    vertLayoutDetail->addWidget(q_tableItems);

    //
    q_tablePaths->setTitle(QStringLiteral("路径前缀信息"));
    q_tablePaths->view()->setHorizontalHeaderLabels(
                QStringList() << QStringLiteral("路径前缀") << QStringLiteral("类型"));
    q_tableItems->setTitle(QStringLiteral("路径信息"));
    q_tableItems->view()->setHorizontalHeaderLabels(QStringList() << QStringLiteral("路径"));

    //
    connect(q_tablePaths->view(), SIGNAL(currentCellChanged(int,int,int,int)),
            this, SLOT(onTablePathsCellChanged(int)));

    //
    q_widgetDetail->hide();
}

bool EnvvalConfigWidget::init(const QString &path)
{
    //
    q_appDir = path;

    //
    q_tablePaths->view()->clearContents();
    q_editBase->clear();
    q_comboBoxType->setCurrentIndex(0);
    q_tableItems->view()->clearContents();

    // 打开框架全局配置文件
    QFile file(q_appDir + "/config/frame/jframe_global.xml");
    if (!file.exists()) {
        const QString text = QStringLiteral("框架全局配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(this, QStringLiteral("警告"), text);
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
        const QString text =
                QStringLiteral("框架全局配置文件\"%1\"解析失败！\n错误描述：%2\n错误位置：（行号：%3，列号：%4）")
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

    // 获取 envval 节点
    QDomElement emEnvval = emRoot.firstChildElement("envval");
    if (emEnvval.isNull()) {
        return false;   // 获取失败
    }

    // 获取 global 节点
    QDomElement emGlobal = emEnvval.firstChildElement("global");
    if (emGlobal.isNull()) {
        return false;   // 获取失败
    }

    // 获取 path 信息
    for (QDomElement emPath = emGlobal.firstChildElement("path");
         !emPath.isNull();
         emPath = emPath.nextSiblingElement("path")) {
        //
        int rowIndex = q_tablePaths->view()->rowCount();
        q_tablePaths->view()->insertRow(rowIndex);
        q_tablePaths->view()->setItemData(rowIndex, 0, emPath.attribute("base"));
        q_tablePaths->view()->setItemData(rowIndex, 1, emPath.attribute("type"));
    }

    return true;
}

void EnvvalConfigWidget::onTablePathsCellChanged(int row)
{
    //
    if (row == -1) {
        q_widgetDetail->hide();
        return;
    }

    //
    q_widgetDetail->show();
    q_tableItems->view()->clearContents();

    // base
    q_editBase->setText(q_tablePaths->view()->itemData(row, 0).toString());
    // type
    q_comboBoxType->setCurrentText(q_tablePaths->view()->itemData(row, 1).toString());

    // items

    // 打开框架全局配置文件
    QFile file(q_appDir + "/config/frame/jframe_global.xml");
    if (!file.exists()) {
        const QString text = QStringLiteral("框架全局配置文件\"%1\"不存在！")
                .arg(file.fileName());
        QMessageBox::warning(this, QStringLiteral("警告"), text);
        return;   // 文件不存在
    }

    // 打开文件
    if (!file.open(QFile::ReadOnly)) {
        return;   // 打开失败
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
        return;
    }

    // 关闭文件
    file.close();

    // 获取根节点
    QDomElement emRoot = document.documentElement();
    if (emRoot.isNull()) {
        return;   // 获取失败
    }

    // 获取 envval 节点
    QDomElement emEnvval = emRoot.firstChildElement("envval");
    if (emEnvval.isNull()) {
        return;   // 获取失败
    }

    // 获取 global 节点
    QDomElement emGlobal = emEnvval.firstChildElement("global");
    if (emGlobal.isNull()) {
        return;   // 获取失败
    }

    // 获取 path 信息
    for (QDomElement emPath = emGlobal.firstChildElement("path");
         !emPath.isNull();
         emPath = emPath.nextSiblingElement("path")) {
        //
        if (emPath.attribute("base").trimmed() == q_editBase->text().trimmed()) {
            //
            for (QDomElement emItem = emPath.firstChildElement("item");
                 !emItem.isNull();
                 emItem = emItem.nextSiblingElement("item")) {
                //
                int rowIndex = q_tableItems->view()->rowCount();
                q_tableItems->view()->insertRow(rowIndex);
                q_tableItems->view()->setItemData(rowIndex, 0, emItem.attribute("path").trimmed());
            }
            break;
        }
    }
}

