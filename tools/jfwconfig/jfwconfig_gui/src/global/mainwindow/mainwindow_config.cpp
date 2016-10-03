#include "precomp.h"
#include "mainwindow_config.h"

MainWindowConfig::MainWindowConfig(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(0, 0, 0, 0);

    q_scrollArea = new QScrollArea(this);
    q_scrollArea->setFrameShape(QFrame::NoFrame);
    q_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    q_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    q_scrollArea->setWidgetResizable(true);
    vertLayoutMain->addWidget(q_scrollArea);

    QWidget *widgetArea = new QWidget(q_scrollArea);
    q_scrollArea->setWidget(widgetArea);

    QVBoxLayout *vertLayoutArea = new QVBoxLayout(widgetArea);
    vertLayoutArea->setContentsMargins(0, 0, 0, 0);

    /// 常规设置

    q_groupBoxGenerate = new QGroupBox(QStringLiteral("常规设置"), widgetArea);
    vertLayoutArea->addWidget(q_groupBoxGenerate);

    QFormLayout *formLayoutGenerate = new QFormLayout(q_groupBoxGenerate);

    q_comboBoxWindowTheme = new QComboBox(q_groupBoxGenerate);
    formLayoutGenerate->addRow(QStringLiteral("框架主题："), q_comboBoxWindowTheme);

    q_editWindowIcon = new QLineEdit(q_groupBoxGenerate);
    formLayoutGenerate->addRow(QStringLiteral("框架图标："), q_editWindowIcon);

    q_editWindowTitle = new QLineEdit(q_groupBoxGenerate);
    formLayoutGenerate->addRow(QStringLiteral("框架标题："), q_editWindowTitle);

    q_checkBoxCloseKilled = new QCheckBox(QStringLiteral("关闭软件时强制退出"), q_groupBoxGenerate);
    formLayoutGenerate->addRow(QStringLiteral(" "), q_checkBoxCloseKilled);

    /// 启动界面

    q_groupBoxSplash = new QGroupBox(QStringLiteral("启动界面"), widgetArea);
    vertLayoutArea->addWidget(q_groupBoxSplash);

    QFormLayout *formLayoutSplash = new QFormLayout(q_groupBoxSplash);

    q_editImageStart = new QLineEdit(q_groupBoxSplash);
    formLayoutSplash->addRow(QStringLiteral("开始背景图："), q_editImageStart);

    q_editImageFinish = new QLineEdit(q_groupBoxSplash);
    formLayoutSplash->addRow(QStringLiteral("结束背景图："), q_editImageFinish);

    q_checkBoxFullScreen = new QCheckBox(QStringLiteral("全屏显示"), q_groupBoxSplash);
    formLayoutSplash->addRow(QStringLiteral(" "), q_checkBoxFullScreen);

    QWidget *widgetAlign = new QWidget(q_groupBoxSplash);

    QVBoxLayout *vertLayoutAlign = new QVBoxLayout(widgetAlign);
    vertLayoutAlign->setContentsMargins(0, 4, 0, 0);

    q_radioButtonAlignLeft = new QRadioButton(QStringLiteral("左对齐"), widgetAlign);
    q_radioButtonAlignTop = new QRadioButton(QStringLiteral("上对齐"), widgetAlign);
    q_radioButtonAlignRight = new QRadioButton(QStringLiteral("右对齐"), widgetAlign);
    q_radioButtonAlignBottom = new QRadioButton(QStringLiteral("下对齐"), widgetAlign);
    q_radioButtonAlignHCenter = new QRadioButton(QStringLiteral("水平居中对齐"), widgetAlign);
    q_radioButtonAlignVCenter = new QRadioButton(QStringLiteral("垂直居中对齐"), widgetAlign);
    vertLayoutAlign->addWidget(q_radioButtonAlignLeft);
    vertLayoutAlign->addWidget(q_radioButtonAlignTop);
    vertLayoutAlign->addWidget(q_radioButtonAlignRight);
    vertLayoutAlign->addWidget(q_radioButtonAlignBottom);
    vertLayoutAlign->addWidget(q_radioButtonAlignHCenter);
    vertLayoutAlign->addWidget(q_radioButtonAlignVCenter);

    QButtonGroup *buttonGroupAlign = new QButtonGroup(widgetAlign);
    buttonGroupAlign->setExclusive(false);
    buttonGroupAlign->addButton(q_radioButtonAlignLeft);
    buttonGroupAlign->addButton(q_radioButtonAlignTop);
    buttonGroupAlign->addButton(q_radioButtonAlignRight);
    buttonGroupAlign->addButton(q_radioButtonAlignBottom);
    buttonGroupAlign->addButton(q_radioButtonAlignHCenter);
    buttonGroupAlign->addButton(q_radioButtonAlignVCenter);

    /// 工具栏

    q_groupBoxToolBar = new QGroupBox(QStringLiteral("工具栏"), widgetArea);
    vertLayoutArea->addWidget(q_groupBoxToolBar);

    QFormLayout *formLayoutToolBar = new QFormLayout(q_groupBoxToolBar);

    q_comboBoxToolBarType = new QComboBox(q_groupBoxToolBar);
    formLayoutToolBar->addRow(QStringLiteral("工具栏类型："), q_comboBoxToolBarType);

    /// MenuBar

    q_groupBoxMenuBar = new QGroupBox(QStringLiteral("菜单型工具栏"), widgetArea);
    vertLayoutArea->addWidget(q_groupBoxMenuBar);

    //QFormLayout *formLayoutMenuBar = new QFormLayout(q_groupBoxMenuBar);

    /// ToolButton

    q_groupBoxToolButton = new QGroupBox(QStringLiteral("按钮型工具栏"), widgetArea);
    vertLayoutArea->addWidget(q_groupBoxToolButton);

    //QFormLayout *formLayoutToolButton = new QFormLayout(q_groupBoxToolButton);

    /// RibbonBar

    q_groupBoxRibbonBar = new QGroupBox(QStringLiteral("Ribbon型工具栏"), widgetArea);
    vertLayoutArea->addWidget(q_groupBoxRibbonBar);

    QFormLayout *formLayoutRibbonBar = new QFormLayout(q_groupBoxRibbonBar);

    q_checkBoxRibbonBarMinimized = new QCheckBox(QStringLiteral("最小化"), q_groupBoxRibbonBar);
    formLayoutRibbonBar->addRow(QStringLiteral(" "), q_checkBoxRibbonBarMinimized);

    q_checkBoxRibbonBarVisible = new QCheckBox(QStringLiteral("显示"), q_groupBoxRibbonBar);
    formLayoutRibbonBar->addRow(QStringLiteral(" "), q_checkBoxRibbonBarVisible);

    /// layout

    q_groupBoxLayout = new QGroupBox(QStringLiteral("布局"), widgetArea);
    vertLayoutArea->addWidget(q_groupBoxLayout);

    QFormLayout *formLayoutLayout = new QFormLayout(q_groupBoxLayout);

    q_comboBoxLayoutType = new QComboBox(q_groupBoxLayout);
    formLayoutLayout->addRow(QStringLiteral("布局方式："), q_comboBoxLayoutType);

    vertLayoutArea->addStretch();

    QPushButton *buttonSave = new QPushButton(QStringLiteral("保存"), this);
    vertLayoutMain->addSpacing(20);
    vertLayoutMain->addWidget(buttonSave, 0, Qt::AlignRight);

    //
    connect(q_comboBoxToolBarType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onToolBarTypeChanged(int)));
    connect(buttonSave, SIGNAL(clicked(bool)), this, SLOT(onButtonSaveClicked()));

    //
    q_comboBoxWindowTheme->addItem("Office 2007 Blue", "Office2007Blue");
    q_comboBoxWindowTheme->addItem("Office 2007 Black", "Office2007Black");
    q_comboBoxWindowTheme->addItem("Office 2007 Silver", "Office2007Silver");
    q_comboBoxWindowTheme->addItem("Office 2007 Aqua", "Office2007Aqua");
    q_comboBoxWindowTheme->addItem("Windows 7 Scenic", "Windows7Scenic");
    q_comboBoxWindowTheme->addItem("Office 2010 Silver", "Office2010Silver");
    q_comboBoxWindowTheme->addItem("Office 2010 Blue", "Office2010Blue");
    q_comboBoxWindowTheme->addItem("Office 2010 Black", "Office2010Black");

    q_comboBoxToolBarType->addItem("Menu Bar", "menuBar");
    q_comboBoxToolBarType->addItem("Tool Button", "toolButton");
    q_comboBoxToolBarType->addItem("Ribbon Bar", "ribbonBar");

    q_comboBoxLayoutType->addItem("Splitter", "splitter");
    q_comboBoxLayoutType->addItem("Dock Float", "dock");
}

bool MainWindowConfig::init(const QString &path)
{
    //
    q_appDir = path;

    return true;
}

void MainWindowConfig::onToolBarTypeChanged(int index)
{
    //
    switch (index) {
    case 0:
        q_groupBoxMenuBar->show();
        q_groupBoxToolButton->hide();
        q_groupBoxRibbonBar->hide();
        break;
    case 1:
        q_groupBoxMenuBar->hide();
        q_groupBoxToolButton->show();
        q_groupBoxRibbonBar->hide();
        break;
    case 2:
        q_groupBoxMenuBar->hide();
        q_groupBoxToolButton->hide();
        q_groupBoxRibbonBar->show();
        break;
    default:
        break;
    }
}

void MainWindowConfig::onButtonSaveClicked()
{

}
