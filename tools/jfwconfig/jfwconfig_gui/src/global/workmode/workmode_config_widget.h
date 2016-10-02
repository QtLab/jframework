#ifndef WORKMODECONFIGWIDGET_H
#define WORKMODECONFIGWIDGET_H

#include <QWidget>

class QLineEdit;

class WorkModeConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkModeConfigWidget(QWidget *parent = 0);

    Q_INVOKABLE bool init(const QString &path);

signals:

public slots:
    void onButtonSaveClicked();

private:
    QString q_appDir;
    QLineEdit *q_editName;
    QLineEdit *q_editDir;
};

#endif // WORKMODECONFIGWIDGET_H
