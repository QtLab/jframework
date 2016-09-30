#ifndef GLOBALCONFIGWIDGET_H
#define GLOBALCONFIGWIDGET_H

#include <QWidget>

class GlobalConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GlobalConfigWidget(QWidget *parent = 0);

signals:

public slots:
    bool appDirChanged(const QString &path);
};

#endif // GLOBALCONFIGWIDGET_H
