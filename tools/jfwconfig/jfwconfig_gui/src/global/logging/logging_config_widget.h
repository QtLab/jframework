#ifndef LOGGINGCONFIGWIDGET_H
#define LOGGINGCONFIGWIDGET_H

#include <QWidget>

class QLineEdit;

class LoggingConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoggingConfigWidget(QWidget *parent = 0);

    Q_INVOKABLE bool init(const QString &path);

signals:

public slots:
    void onButtonSaveClicked();

private:
    QString q_appDir;
    QLineEdit *q_editPath;
};

#endif // LOGGINGCONFIGWIDGET_H
