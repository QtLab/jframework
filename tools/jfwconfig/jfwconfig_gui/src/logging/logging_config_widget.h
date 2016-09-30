#ifndef LOGGINGCONFIGWIDGET_H
#define LOGGINGCONFIGWIDGET_H

#include <QWidget>

class LoggingConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoggingConfigWidget(QWidget *parent = 0);

signals:

public slots:
    bool appDirChanged(const QString &path);
};

#endif // LOGGINGCONFIGWIDGET_H
