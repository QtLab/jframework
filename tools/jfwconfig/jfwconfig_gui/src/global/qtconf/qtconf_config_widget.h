#ifndef QTCONFCONFIGWIDGET_H
#define QTCONFCONFIGWIDGET_H

#include <QWidget>

class QtConfConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QtConfConfigWidget(QWidget *parent = 0);

    Q_INVOKABLE bool init(const QString &path);

signals:

public slots:

private:
    QString q_appDir;
};

#endif // QTCONFCONFIGWIDGET_H
