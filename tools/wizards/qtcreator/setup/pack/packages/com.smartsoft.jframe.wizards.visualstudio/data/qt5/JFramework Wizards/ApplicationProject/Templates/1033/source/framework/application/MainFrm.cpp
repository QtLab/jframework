
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "[!output APP_NAME].h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

BOOL CMainFrame::LoadFramework()
{
	IJFrameFacade *frameFacade = C[!output APP_CLASS_NAME]App::LoadFrameFacade();
	if (frameFacade == NULL)
	{
		return FALSE;
	}

	// 加载框架
	if (!frameFacade->loadFrame(0, 0, static_cast<CWinApp *>(&theApp))) {
		frameFacade->exitFrame();
		return FALSE;      // 加载失败
	}

	// BCG Style
	theApp.SetVisualTheme(CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2010_BLUE);

	// 显示框架主窗口
	frameFacade->showFrame(true, true);

	return TRUE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
#if 0
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
#endif
	// 加载框架
	if (!LoadFramework())
	{
		return -1;		// 加载失败
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
#if 0
	// forward focus to the view window
	if (m_wndView.IsWindowVisible())
	{
		m_wndView.SetFocus();
	}
#endif
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
#if 0
	if (m_wndView.IsWindowVisible())
	{
		// let the view have first crack at the command
		if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		{
			return TRUE;
		}
	}
#endif
	// otherwise, do default handling
	return CBCGPFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
