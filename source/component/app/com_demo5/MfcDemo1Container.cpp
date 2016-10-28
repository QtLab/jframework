#include "StdAfx.h"
#include "MfcDemo1Container.h"
#include "MfcDemo1Dlg.h"
#include "qtwinmigrate/qwinhost.h"

CMfcDemo1Container::CMfcDemo1Container(IJAttempter &attempter, QWidget *parent)
	: QWidget(parent)
	, q_attempter(attempter)
	, q_mfcDemo1Dlg(0)
{
	QHBoxLayout *horiLayoutMain = new QHBoxLayout(this);
	horiLayoutMain->setContentsMargins(0, 0, 0, 0);
	
	HMODULE hModule = ::GetModuleHandle(_T("com_demo5d.dll"));
	HINSTANCE hOldResHandle = ::AfxGetResourceHandle();
	::AfxSetResourceHandle(hModule);

	q_mfcDemo1Dlg = new CMfcDemo1Dlg(attempter);
	q_mfcDemo1Dlg->Create(CMfcDemo1Dlg::IDD);

	AfxSetResourceHandle(hOldResHandle);

	QWinHost *winHost = new QWinHost(this);
	winHost->setWindow(q_mfcDemo1Dlg->GetSafeHwnd());

	horiLayoutMain->addWidget(winHost);
}

CMfcDemo1Container::~CMfcDemo1Container(void)
{
	
}

void CMfcDemo1Container::releaseInterface()
{
	q_mfcDemo1Dlg->DestroyWindow();
}
