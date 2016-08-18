#pragma once


// CMfcDemo1Dlg dialog

class CMfcDemo1Dlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CMfcDemo1Dlg)

public:
	CMfcDemo1Dlg(INotifier *notifier, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMfcDemo1Dlg();

// Dialog Data
	enum { IDD = IDD_MFC_DEMO1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonApply();

private:
	INotifier *q_notifier;
};
