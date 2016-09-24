
// [!output APP_NAME].h : main header file for the Application application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// C[!output APP_CLASS_NAME]App:
// See [!output APP_NAME].cpp for the implementation of this class
//

class IJFrameFacade;

class C[!output APP_CLASS_NAME]App : public CBCGPWinApp
{
public:
	C[!output APP_CLASS_NAME]App();

	//
	static CString ApplicationDirPath();

	static IJFrameFacade *LoadFrameFacade();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int Run();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL LoadWindowPlacement(CRect& rectNormalPosition, int& nFflags, int& nShowCmd);
};

extern C[!output APP_CLASS_NAME]App theApp;
