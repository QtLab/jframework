#pragma once

#include <QWidget>

class IJAttempter;
class CMfcDemo1Dlg;

class CMfcDemo1Container : public QWidget
{
	Q_OBJECT
public:
	CMfcDemo1Container(IJAttempter &attempter, QWidget *parent = 0);
	~CMfcDemo1Container(void);

	void releaseInterface();

private:
	IJAttempter &q_attempter;
	CMfcDemo1Dlg *q_mfcDemo1Dlg;
};
