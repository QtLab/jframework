#ifndef SYSTEMCONFIGWIDGET_H
#define SYSTEMCONFIGWIDGET_H

#include <QWidget>

class SystemConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SystemConfigWidget(QWidget *parent = 0);

    Q_INVOKABLE bool init(const QString &path);

signals:

public slots:

private:
    QString q_appDir;
};

#endif // SYSTEMCONFIGWIDGET_H
