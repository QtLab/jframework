#include "precomp.h"
#include "component_detail.h"

ComponentDetail::ComponentDetail(QWidget *parent)
    : QWidget(parent)
    , q_bNewCom(true)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignRight);
    vertLayoutMain->addLayout(formLayout);

    q_editComponentName = new QLineEdit(this);
    formLayout->addRow(QStringLiteral("组件名称："), q_editComponentName);

    q_editComponentDesc = new QLineEdit(this);
    formLayout->addRow(QStringLiteral("组件描述："), q_editComponentDesc);

    q_checkBoxLoad = new QCheckBox(QStringLiteral("加载（Load）"), this);
    formLayout->addRow(QStringLiteral(" "), q_checkBoxLoad);

    q_checkBoxStay = new QCheckBox(QStringLiteral("驻留（Stay）"), this);
    formLayout->addRow(QStringLiteral(" "), q_checkBoxStay);

    q_widgetComDir = new QWidget(this);
    formLayout->addRow(QStringLiteral("组件路径："), q_widgetComDir);

    QVBoxLayout *vertLayoutComDir = new QVBoxLayout(q_widgetComDir);
    vertLayoutComDir->setContentsMargins(0, 4, 0, 0);

    q_radioButtonFrame = new QRadioButton(QStringLiteral("框架路径 (@FrameDir@/...)"), q_widgetComDir);
    q_radioButtonThis = new QRadioButton(QStringLiteral("软件路径 (@ThisDir@/...)"), q_widgetComDir);
    q_radioButtonCustom = new QRadioButton(QStringLiteral("自定义"), q_widgetComDir);
    q_editComDir = new QLineEdit(q_widgetComDir);
    vertLayoutComDir->addWidget(q_radioButtonFrame);
    vertLayoutComDir->addWidget(q_radioButtonThis);
    vertLayoutComDir->addWidget(q_radioButtonCustom);
    vertLayoutComDir->addWidget(q_editComDir);

    q_buttonGroupComDir = new QButtonGroup(q_widgetComDir);
    q_buttonGroupComDir->addButton(q_radioButtonFrame, 0);
    q_buttonGroupComDir->addButton(q_radioButtonThis, 1);
    q_buttonGroupComDir->addButton(q_radioButtonCustom, 2);

    q_comboBoxType = new QComboBox(this);
    q_comboBoxType->addItem(QStringLiteral("Qt"));
    q_comboBoxType->addItem(QStringLiteral("MFC"));
    q_comboBoxType->addItem(QStringLiteral("Others"));
    formLayout->addRow(QStringLiteral("组件类型："), q_comboBoxType);

    q_buttonSave = new QPushButton(QStringLiteral("保存"), this);
    vertLayoutMain->addSpacing(10);
    vertLayoutMain->addWidget(q_buttonSave, 0, Qt::AlignRight);

    vertLayoutMain->addStretch();

    //
    connect(q_buttonGroupComDir, SIGNAL(buttonToggled(int, bool)),
            this, SLOT(onButtonGroupComDirToggled(int, bool)));
    connect(q_buttonSave, SIGNAL(clicked(bool)), this, SLOT(onButtonSaveClicked()));

    //
    q_radioButtonFrame->setVisible(false);
}

void ComponentDetail::setComConfigPath(const QString &filePath)
{
    q_comConfigPath = filePath;
}

void ComponentDetail::clearContents()
{
    q_editComponentName->clear();
    q_editComponentDesc->clear();
    q_checkBoxLoad->setChecked(true);
    q_checkBoxStay->setChecked(false);
    q_radioButtonThis->setChecked(true);
    q_comboBoxType->setCurrentIndex(0);
}

void ComponentDetail::focusComponent(const QString &componentName, bool def)
{
    //
    q_currComName = componentName;
    q_bNewCom = false;

    //
    q_editComponentName->setReadOnly(def);
    q_checkBoxStay->setDisabled(def);
    q_radioButtonFrame->setVisible(def);
    q_radioButtonThis->setVisible(!def);
    q_radioButtonCustom->setVisible(!def);
    q_editComDir->setReadOnly(def);
    q_comboBoxType->setDisabled(def);

    // 打开框架全局配置文件
    QFile file(q_comConfigPath);
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
        return;   // 获取失败
    }

    //
    for (QDomElement emComponent = emRoot.firstChildElement("component");
         !emComponent.isNull();
         emComponent = emComponent.nextSiblingElement("component")) {
        //
        if (emComponent.attribute("name").trimmed() == componentName) {
            // name
            q_editComponentName->setText(componentName);
            // desc
            q_editComponentDesc->setText(emComponent.attribute("desc").trimmed());
            // load
            q_checkBoxLoad->setChecked(QVariant(emComponent.attribute("load")).toBool());
            // stay
            q_checkBoxStay->setChecked(QVariant(emComponent.attribute("stay")).toBool());
            // dir
            const QString dir = emComponent.attribute("dir").trimmed();
            if (dir.contains("@FrameDir@")) {
                q_radioButtonFrame->setChecked(true);
            } else if (dir.contains("@ThisDir@")) {
                q_radioButtonThis->setChecked(true);
            } else {
                q_radioButtonCustom->setChecked(true);
            }
            q_editComDir->setText(dir);
            // type
            const QString type = emComponent.attribute("type").trimmed().toLower();
            if (type == "qt") {
                q_comboBoxType->setCurrentIndex(0);
            } else if (type == "mfc") {
                q_comboBoxType->setCurrentIndex(1);
            } else {
                q_comboBoxType->setCurrentIndex(2);
            }
            break;
        }
    }
}

void ComponentDetail::onButtonGroupComDirToggled(int id, bool checked)
{
    switch (id) {
    case 0:
        if (checked) {
            q_editComDir->setReadOnly(true);
            q_editComDir->setText("@FrameDir@/component");
        }
        break;
    case 1:
        if (checked) {
            q_editComDir->setReadOnly(true);
            q_editComDir->setText("@ThisDir@/component");
        }
        break;
    case 2:
        if (checked) {
            q_editComDir->setReadOnly(false);
            q_editComDir->setText("../component");
        }
        break;
    default:
        break;
    }
}

void ComponentDetail::onButtonSaveClicked()
{
    // 打开框架全局配置文件
    QFile file(q_comConfigPath);
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

    // name
    const QString componentName = q_editComponentName->text().trimmed();
    if (componentName.isEmpty()) {
        q_editComponentName->setFocus();
        return;     //
    }
    // desc
    const QString componentDesc = q_editComponentDesc->text().trimmed();
    if (componentDesc.isEmpty()) {
        q_editComponentDesc->setFocus();
        return;     //
    }

    //
    QDomElement emComponent;
    if (q_bNewCom) {
        emComponent = emRoot.insertAfter(document.createElement("component"),
                                          emRoot.lastChild()).toElement();
    } else {
        for (emComponent = emRoot.firstChildElement("component");
             !emComponent.isNull();
             emComponent = emComponent.nextSiblingElement("component")) {
            //
            if (emComponent.attribute("name").trimmed() == q_currComName) {
                break;
            }
        }
    }

    //
    if (emComponent.isNull()) {
        return;     //
    }

    // name
    emComponent.setAttribute("name", componentName);
    // desc
    emComponent.setAttribute("desc", componentDesc);
    // load
    emComponent.setAttribute("load", q_checkBoxLoad->isChecked());
    // stay
    emComponent.setAttribute("stay", q_checkBoxStay->isChecked());
    // dir
    const QString dir = q_editComDir->text().trimmed();
    emComponent.setAttribute("dir", dir);
    // type
    emComponent.setAttribute("type", q_comboBoxType->currentText().trimmed().toLower());

    //
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    //
    QTextStream textStream(&file);
    document.save(textStream, 2);

    //
    if (q_bNewCom) {
        Q_EMIT newComponentAdded(componentName, componentDesc, false);
    } else {
        Q_EMIT componentModify(componentName, componentDesc,
                                    dir.contains("@FrameDir@"));
    }
}

void ComponentDetail::newComponent()
{
    //
    q_bNewCom = true;

    //
    clearContents();
}
