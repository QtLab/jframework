#ifndef JSTACKED_WIDGET_H
#define JSTACKED_WIDGET_H

#include <QWidget>

class JStackedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JStackedWidget(QWidget *parent = 0);
    ~JStackedWidget();

signals:

public slots:
};

#endif // JSTACKED_WIDGET_H
