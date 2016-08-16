#ifndef TESTWIDGET1_H
#define TESTWIDGET1_H

#include <QWidget>

class INotifier;

class TestWidget1 : public QWidget
{
    Q_OBJECT
public:
    explicit TestWidget1(INotifier *notifier, QWidget *parent = 0);
    ~TestWidget1();

signals:

public slots:
    void onButton1Clicked();

private:
    INotifier *q_notifier;
};

#endif // TESTWIDGET1_H
