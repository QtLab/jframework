#include "precomp.h"
#include "workmode_config_widget.h"

WorkModeConfigWidget::WorkModeConfigWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);

    QFormLayout *formLayout = new QFormLayout();
    vertLayoutMain->addLayout(formLayout);

    q_editName = new QLineEdit(this);
    formLayout->addRow(QStringLiteral("名称："), q_editName);

    q_editDir = new QLineEdit(this);
    formLayout->addRow(QStringLiteral("路径："), q_editDir);

    QPushButton *buttonSave = new QPushButton(QStringLiteral("保存"), this);
    vertLayoutMain->addSpacing(20);
    vertLayoutMain->addWidget(buttonSave, 0, Qt::AlignRight);

    vertLayoutMain->addStretch();

    //
    connect(buttonSave, SIGNAL(clicked(bool)), this, SLOT(onButtonSaveClicked()));
}

bool WorkModeConfigWidget::init(const QString &path)
{
    //
    q_appDir = path;

    //
    q_editName->clear();
    q_editDir->clear();

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

    // 获取 workMode 节点
    QDomElement emWorkMode = emRoot.firstChildElement("workMode");
    if (emWorkMode.isNull()) {
        return false;   // 获取失败
    }

    // name
    q_editName->setText(emWorkMode.attribute("name").trimmed());
    // dir
    q_editDir->setText(emWorkMode.attribute("dir").trimmed());

    return true;
}

void WorkModeConfigWidget::onButtonSaveClicked()
{
    // name
    if (q_editName->text().trimmed().isEmpty()) {
        q_editName->setFocus();
        return;
    }

    // dir
    if (q_editDir->text().trimmed().isEmpty()) {
        q_editDir->setFocus();
        return;
    }

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

    // 获取 workMode 节点
    QDomElement emWorkMode = emRoot.firstChildElement("workMode");
    if (emWorkMode.isNull()) {
        return;   // 获取失败
    }

    // name
    emWorkMode.setAttribute("name", q_editName->text().trimmed());
    // dir
    emWorkMode.setAttribute("dir", q_editDir->text().trimmed());

    //
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    //
    QTextStream textStream(&file);
    document.save(textStream, 2);

    //
    QMessageBox::information(this, QStringLiteral("保存设置"), QStringLiteral("保存成功！"));
}
