#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLineEdit;
class QToolBar;
class QStackedWidget;
class GlobalConfigWidget;
class ComConfigWidget;
class LayoutConfigWidget;
class LoggingConfigWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void onActionGlobalClicked();
    void onActionComponentClicked();
    void onActionLayoutClicked();
    void onActionLoggingClicked();
    void onButtonViewClicked();
    void onCurrentWidgetChanged(int);

private:
    bool pathIsValid(const QString &path) const;
    bool updateCurrentModule();

private:
    QWidget *q_centralWidget;
    QLineEdit *q_editAppPath;
    QToolBar *q_toolBarNav;
    QStackedWidget *q_stackedWidget;
    GlobalConfigWidget *q_globalConfigWidget;
    ComConfigWidget *q_comConfigWidget;
    LayoutConfigWidget *q_layoutConfigWidget;
    LoggingConfigWidget *q_loggingConfigWidget;
};

#endif // MAINWINDOW_H
