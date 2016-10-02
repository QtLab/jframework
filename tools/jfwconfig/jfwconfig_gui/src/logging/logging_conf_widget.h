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
};

#endif // LOGGINGCONFWIDGET_H
