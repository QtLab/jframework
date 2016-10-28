#ifndef CUSTOMPLOTTESTUI_H
#define CUSTOMPLOTTESTUI_H

#include <QWidget>

class IJAttempter;

class CustomPlotTestUi : public QWidget
{
    Q_OBJECT
public:
    explicit CustomPlotTestUi(IJAttempter &attempter, QWidget *parent = 0);
    ~CustomPlotTestUi();

Q_SIGNALS:

public Q_SLOTS:

private:
    IJAttempter &q_attempter;
};

#endif // CUSTOMPLOTTESTUI_H