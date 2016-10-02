#ifndef LAYOUTCONFIGWIDGET_H
#define LAYOUTCONFIGWIDGET_H

#include <QWidget>

class LayoutConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LayoutConfigWidget(QWidget *parent = 0);

signals:

public slots:
    bool appDirChanged(const QString &path);

private:

};

#endif // LAYOUTCONFIGWIDGET_H
