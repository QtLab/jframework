
// Application.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Application.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CApplicationApp

BEGIN_MESSAGE_MAP(CApplicationApp, CBCGPWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CApplicationApp::OnAppAbout)
END_MESSAGE_MAP()


// CApplicationApp construction

CApplicationApp::CApplicationApp()
{

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CString CApplicationApp::ApplicationDirPath()
{
	static CString _path = _T("");
	if (_path.IsEmpty())
	{
		TCHAR cTmp[MAX_PATH];
		GetModuleFileName(NULL, cTmp, MAX_PATH);
		_path = cTmp;
		int index = _path.ReverseFind(_T('\\'));
		if (index == -1)
		{
			index = _path.ReverseFind(_T('/'));
			if (index == -1)
			{
				return _path;
			}
		}

		_path = _path.Left(index);
	}

	return _path;
}

IJFrameFacade * CApplicationApp::LoadFrameFacade()
{
	// �����ļ�·��
	CString filePath = ApplicationDirPath();
	filePath.Append(_T("\\jframeworkdir.dll"));

	/// ��ȡ FrameFacadeInstace �����ӿ�

	// ���ؿ��ļ�
	HMODULE hFrameworkDir = ::LoadLibrary(filePath.GetBuffer());
	if (hFrameworkDir == NULL)
	{
		return NULL;
	}

	// ��ȡ FrameFacadeInstance �����ӿ�
	typedef void* (J_ATTR_CDECL*FrameFacadeInstace)(int);
	FrameFacadeInstace frameFacadeInstace =
		(FrameFacadeInstace)::GetProcAddress(hFrameworkDir, "FrameFacadeInstace");
	if (frameFacadeInstace == NULL)
	{
		return NULL;	// ��ȡ�����ӿ�ʧ��
	}

	// ��ȡ IJFrameFacade �ӿ�ʵ��
	IJFrameFacade *frameFacade = dynamic_cast<IJFrameFacade *>
		((IJUnknown *)(frameFacadeInstace(
#if defined(_DEBUG)
			1
#else
			0
#endif
		)));
	if (frameFacade == NULL)
	{
		return NULL;      // ��ȡʵ��ʧ��
	}

	return frameFacade;
}

// The one and only CApplicationApp object

CApplicationApp theApp;


// CApplicationApp initialization

BOOL CApplicationApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CBCGPWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Smartsoft"));

	//
	InitKeyboardManager();
	InitContextMenuManager();
	InitTooltipManager();

	//
	CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPWinXPVisualManager));
	CBCGPButton::EnableWinXPTheme();
	globalData.m_bUseVisualManagerInBuiltInDialogs = TRUE;

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
#if 0
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	m_nCmdShow = SW_HIDE;
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
#endif
	// The one and only window has been initialized, so show and update it
	//pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}


int CApplicationApp::Run()
{
#if 1	// Qt��Ϣѭ��

	//
	IJFrameFacade *frameFacade = LoadFrameFacade();
	if (frameFacade != NULL)
	{
		return frameFacade->runQApp(AfxGetApp());
	}

#endif

	return CBCGPWinApp::Run();
}

int CApplicationApp::ExitInstance()
{
	//
	BCGCBProCleanUp();

	return CBCGPWinApp::ExitInstance();
}

// CApplicationApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

														// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CBCGPDialog(CAboutDlg::IDD)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CApplicationApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CApplicationApp::LoadWindowPlacement(CRect& rectNormalPosition, int& nFflags, int& nShowCmd)
{
	return FALSE;
}

// CApplicationApp message handlers



