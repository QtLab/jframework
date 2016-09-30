#include "precomp.h"
#include "component_detail.h"

ComponentDetail::ComponentDetail(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignRight);
    vertLayoutMain->addLayout(formLayout);

    q_editComponentName = new QLineEdit(this);
    formLayout->addRow(QStringLiteral("������ƣ�"), q_editComponentName);

    q_editComponentDesc = new QLineEdit(this);
    formLayout->addRow(QStringLiteral("���������"), q_editComponentDesc);

    q_checkBoxLoad = new QCheckBox(QStringLiteral("���أ�Load��"), this);
    q_checkBoxLoad->setChecked(true);
    formLayout->addRow(QStringLiteral(" "), q_checkBoxLoad);

    q_checkBoxStay = new QCheckBox(QStringLiteral("פ����Stay��"), this);
    q_checkBoxStay->setChecked(true);
    formLayout->addRow(QStringLiteral(" "), q_checkBoxStay);

    q_widgetComDir = new QWidget(this);
    formLayout->addRow(QStringLiteral("���·����"), q_widgetComDir);

    QVBoxLayout *vertLayoutComDir = new QVBoxLayout(q_widgetComDir);
    vertLayoutComDir->setContentsMargins(0, 0, 0, 0);

    q_radioButtonFrame = new QRadioButton(QStringLiteral("���·��"), q_widgetComDir);
    q_radioButtonThis = new QRadioButton(QStringLiteral("���·��"), q_widgetComDir);
    q_radioButtonCustom = new QRadioButton(QStringLiteral("�Զ���"), q_widgetComDir);
    q_editComDir = new QLineEdit(q_widgetComDir);
    vertLayoutComDir->addWidget(q_radioButtonFrame);
    vertLayoutComDir->addWidget(q_radioButtonThis);
    vertLayoutComDir->addWidget(q_radioButtonCustom);
    vertLayoutComDir->addWidget(q_editComDir);

    QButtonGroup *buttonGroupComDir = new QButtonGroup(q_widgetComDir);
    buttonGroupComDir->addButton(q_radioButtonFrame, 0);
    buttonGroupComDir->addButton(q_radioButtonThis, 1);
    buttonGroupComDir->addButton(q_radioButtonCustom, 2);

    q_comboBoxType = new QComboBox(this);
    q_comboBoxType->addItem(QStringLiteral("Qt"));
    q_comboBoxType->addItem(QStringLiteral("MFC"));
    q_comboBoxType->addItem(QStringLiteral("Others"));
    formLayout->addRow(QStringLiteral("������ͣ�"), q_comboBoxType);

    q_buttonSave = new QPushButton(QStringLiteral("����"), this);
    vertLayoutMain->addSpacing(10);
    vertLayoutMain->addWidget(q_buttonSave, 0, Qt::AlignRight);

    vertLayoutMain->addStretch();

    //
    connect(buttonGroupComDir, SIGNAL(buttonClicked(int)), this, SLOT(onButtonGroupComDirClicked(int)));
    connect(q_buttonSave, SIGNAL(clicked(bool)), this, SLOT(onButtonSaveClicked()));
}

void ComponentDetail::clearContent()
{

}

void ComponentDetail::onButtonGroupComDirClicked(int id)
{
    switch (id) {
    case 0:
        q_editComDir->setReadOnly(true);
        q_editComDir->setText("@FrameDir@/component");
        break;
    case 1:
        q_editComDir->setReadOnly(true);
        q_editComDir->setText("@ThisDir@/component");
        break;
    case 2:
        q_editComDir->setReadOnly(false);
        q_editComDir->setText("./component");
        break;
    default:
        break;
    }
}

void ComponentDetail::onButtonSaveClicked()
{

}
