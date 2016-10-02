#include "precomp.h"
#include "global_config_widget.h"
#include "generate/generate_config_widget.h"
#include "envval/envval_config_widget.h"
#include "qtconf/qtconf_config_widget.h"
#include "database/database_config_widget.h"
#include "workmode/workmode_config_widget.h"
#include "logging/logging_config_widget.h"

GlobalConfigWidget::GlobalConfigWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);

    JSplitter *splitterMain = new JSplitter(this);
    splitterMain->setScales(QList<double>() << 1 << 2);
    horiLayoutMain->addWidget(splitterMain);

    q_treeWidget = new QTreeWidget(this);
    q_treeWidget->setHeaderHidden(true);
    splitterMain->addWidget(q_treeWidget);

    q_stackedWidget = new QStackedWidget(this);
    splitterMain->addWidget(q_stackedWidget);

    q_generateConfigWidget = new GenerateConfigWidget(q_stackedWidget);
    q_envvalConfigWidget = new EnvvalConfigWidget(q_stackedWidget);
    q_qtconfConfigWidget = new QtConfConfigWidget(q_stackedWidget);
    q_databaseConfigWidget = new DatabaseConfigWidget(q_stackedWidget);
    q_workModeConfigWidget = new WorkModeConfigWidget(q_stackedWidget);
    q_loggingConfigWidget = new LoggingConfigWidget(q_stackedWidget);
    q_stackedWidget->addWidget(q_generateConfigWidget);
    q_stackedWidget->addWidget(q_envvalConfigWidget);
    q_stackedWidget->addWidget(q_qtconfConfigWidget);
    q_stackedWidget->addWidget(q_databaseConfigWidget);
    q_stackedWidget->addWidget(q_workModeConfigWidget);
    q_stackedWidget->addWidget(q_loggingConfigWidget);

    //
    connect(q_treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(q_stackedWidget, SIGNAL(currentChanged(int)),
            this, SLOT(onCurrentWidgetChanged(int)));

    //
}

bool GlobalConfigWidget::appDirChanged(const QString &path)
{
    //
    q_appDir = path;

    //
    q_treeWidget->clear();

    //
    QTreeWidgetItem *rootItem = q_treeWidget->invisibleRootItem();
    if (!rootItem) {
        return false;
    }

    //
    QTreeWidgetItem *item = 0;

    // 常规
    item = new QTreeWidgetItem(QStringList() << QStringLiteral("常规"));
    rootItem->addChild(item);
    q_treeWidget->setCurrentItem(item);

    // 环境变量
    item = new QTreeWidgetItem(QStringList() << QStringLiteral("环境变量"));
    rootItem->addChild(item);

    // Qt配置文件
    item = new QTreeWidgetItem(QStringList() << QStringLiteral("Qt运行配置"));
    rootItem->addChild(item);

    // 数据库
    item = new QTreeWidgetItem(QStringList() << QStringLiteral("数据库"));
    rootItem->addChild(item);

    // 工作模式
    item = new QTreeWidgetItem(QStringList() << QStringLiteral("工作模式"));
    rootItem->addChild(item);

    // 日志
    item = new QTreeWidgetItem(QStringList() << QStringLiteral("日志"));
    rootItem->addChild(item);

    // 主窗口
    item = new QTreeWidgetItem(QStringList() << QStringLiteral("主窗口"));
    {
        //　工具栏
        QTreeWidgetItem *itemChild = new QTreeWidgetItem(QStringList() << QStringLiteral("工具栏"));
        item->addChild(itemChild);
        //　状态栏
        itemChild = new QTreeWidgetItem(QStringList() << QStringLiteral("状态栏"));
        item->addChild(itemChild);
        //　布局
        itemChild = new QTreeWidgetItem(QStringList() << QStringLiteral("布局"));
        item->addChild(itemChild);
    }
    rootItem->addChild(item);

    //
    q_treeWidget->expandAll();

    return true;
}

void GlobalConfigWidget::onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    //
    if (current) {
        q_stackedWidget->setVisible(true);
    } else {
        q_stackedWidget->setVisible(false);
        return;
    }

    //
    int oldIndex = q_stackedWidget->currentIndex();
    const QString name = current->text(0);
    if (name == QStringLiteral("常规")) {
        q_stackedWidget->setCurrentIndex(0);
    } else if (name == QStringLiteral("环境变量")) {
        q_stackedWidget->setCurrentIndex(1);
    } else if (name == QStringLiteral("Qt运行配置")) {
        q_stackedWidget->setCurrentIndex(2);
    } else if (name == QStringLiteral("数据库")) {
        q_stackedWidget->setCurrentIndex(3);
    } else if (name == QStringLiteral("工作模式")) {
        q_stackedWidget->setCurrentIndex(4);
    } else if (name == QStringLiteral("日志")) {
        q_stackedWidget->setCurrentIndex(5);
    } else if (name == QStringLiteral("主窗口")) {
        q_stackedWidget->setCurrentIndex(6);
    }

    //
    if (oldIndex == q_stackedWidget->currentIndex()) {
        if (!updateCurrentModule()) {
            //
        }
    }
}

void GlobalConfigWidget::onCurrentWidgetChanged(int index)
{
    if (index == -1) {
        return;
    }

    //
    if (!updateCurrentModule()) {
        return;
    }

    //
}

bool GlobalConfigWidget::updateCurrentModule()
{
    //
    QWidget *currentWidget = q_stackedWidget->currentWidget();
    if (!currentWidget) {
        return false;
    }

    //
    bool result = true;
    QMetaObject::invokeMethod(currentWidget, "init", Q_RETURN_ARG(bool, result),
                              Q_ARG(QString, q_appDir));

    return result;
}
