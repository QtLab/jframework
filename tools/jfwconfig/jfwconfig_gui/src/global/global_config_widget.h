#ifndef GLOBALCONFIGWIDGET_H
#define GLOBALCONFIGWIDGET_H

#include <QWidget>

class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;
class GenerateConfigWidget;
class EnvvalConfigWidget;
class QtConfConfigWidget;
class DatabaseConfigWidget;
class WorkModeConfigWidget;
class LoggingConfigWidget;
class MainWindowConfig;

class GlobalConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GlobalConfigWidget(QWidget *parent = 0);

signals:

public slots:
    bool appDirChanged(const QString &path);
    void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void onCurrentWidgetChanged(int index);

private:
    bool updateCurrentModule();

private:
    QString q_appDir;
    QTreeWidget *q_treeWidget;
    QStackedWidget *q_stackedWidget;
    GenerateConfigWidget *q_generateConfigWidget;
    EnvvalConfigWidget *q_envvalConfigWidget;
    QtConfConfigWidget *q_qtconfConfigWidget;
    DatabaseConfigWidget *q_databaseConfigWidget;
    WorkModeConfigWidget *q_workModeConfigWidget;
    LoggingConfigWidget *q_loggingConfigWidget;
    MainWindowConfig *q_mainWindowConfig;
};

#endif // GLOBALCONFIGWIDGET_H
