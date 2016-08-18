#pragma once

#include <QWidget>

class CMfcDemo1Dlg;

class CMfcDemo1Container : public QWidget
{
	Q_OBJECT
public:
	CMfcDemo1Container(INotifier *notifier, QWidget *parent = 0);
	~CMfcDemo1Container(void);

	void releaseInterface();

private:
	INotifier *q_notifier;
	CMfcDemo1Dlg *q_mfcDemo1Dlg;
};
