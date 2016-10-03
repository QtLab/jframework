#ifndef LOGGINGCONFWIDGET_H
#define LOGGINGCONFWIDGET_H

#include <QWidget>

class LoggingConfWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoggingConfWidget(QWidget *parent = 0);

signals:

public slots:
    bool appDirChanged(const QString &path);

private:
    QString q_appDir;
};

#endif // LOGGINGCONFWIDGET_H
