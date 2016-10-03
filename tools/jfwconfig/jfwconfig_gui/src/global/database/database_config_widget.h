#ifndef DATABASECONFIGWIDGET_H
#define DATABASECONFIGWIDGET_H

#include <QWidget>

class DatabaseConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DatabaseConfigWidget(QWidget *parent = 0);

    Q_INVOKABLE bool init(const QString &path);

signals:

public slots:

private:
    QString q_appDir;
};

#endif // DATABASECONFIGWIDGET_H
