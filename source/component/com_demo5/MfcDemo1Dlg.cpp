// MfcDemo1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "com_demo5.h"
#include "MfcDemo1Dlg.h"


// CMfcDemo1Dlg dialog

IMPLEMENT_DYNAMIC(CMfcDemo1Dlg, CBCGPDialog)

CMfcDemo1Dlg::CMfcDemo1Dlg(INotifier *notifier, CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CMfcDemo1Dlg::IDD, pParent)
	, q_notifier(notifier)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

CMfcDemo1Dlg::~CMfcDemo1Dlg()
{

}

void CMfcDemo1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BOOL CMfcDemo1Dlg::PreTranslateMessage(MSG* pMsg)
{
	// ignore some keys
	switch (pMsg->message)
	{
	default:
		break;
	}

	return CBCGPDialog::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CMfcDemo1Dlg, CBCGPDialog)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CMfcDemo1Dlg::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CMfcDemo1Dlg message handlers

void CMfcDemo1Dlg::OnBnClickedButtonApply()
{
	if (q_notifier != NULL)
	{
		q_notifier->postMessage("j_switch_module", "module #1");
	}
}
