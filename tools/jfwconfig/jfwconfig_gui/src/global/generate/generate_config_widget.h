#ifndef GENERATECONFIGWIDGET_H
#define GENERATECONFIGWIDGET_H

#include <QWidget>

class GenerateConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GenerateConfigWidget(QWidget *parent = 0);

    Q_INVOKABLE bool init(const QString &path);

signals:

public slots:

private:
    QString q_appDir;
};

#endif // GENERATECONFIGWIDGET_H
