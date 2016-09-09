#ifndef TESTWIDGET1_H
#define TESTWIDGET1_H

#include <QWidget>

class IJAttempter;

class TestWidget1 : public QWidget
{
    Q_OBJECT
public:
    explicit TestWidget1(IJAttempter &attempter, QWidget *parent = 0);
    ~TestWidget1();

signals:

public slots:
    void onButtonModeTestClicked();
    void onButtonCmdTestClicked();
    void onButtonMsgTestClicked();

private:
    IJAttempter &q_attempter;
};

#endif // TESTWIDGET1_H
