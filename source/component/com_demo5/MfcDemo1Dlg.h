#pragma once


// CMfcDemo1Dlg dialog

class IJAttempter;

class CMfcDemo1Dlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CMfcDemo1Dlg)

public:
	CMfcDemo1Dlg(IJAttempter &attempter, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMfcDemo1Dlg();

// Dialog Data
	enum { IDD = IDD_MFC_DEMO1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonApply();

private:
	IJAttempter &q_attempter;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
