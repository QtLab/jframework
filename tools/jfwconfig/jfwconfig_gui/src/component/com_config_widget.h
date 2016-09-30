#ifndef COMCONFIGWIDGET_H
#define COMCONFIGWIDGET_H

#include <QWidget>

class QListWidget;
class ComponentDetail;

class ComConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ComConfigWidget(QWidget *parent = 0);

signals:

public slots:
    bool appDirChanged(const QString &path);
    void onButtonAddClicked();

private:
    QString componentConfigPath();

private:
    QListWidget *q_listWidgetDefCom;
    QListWidget *q_listWidgetUsrCom;
    ComponentDetail *q_componentDetail;
};

#endif // COMCONFIGWIDGET_H
