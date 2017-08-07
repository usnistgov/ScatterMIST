//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: MainFrm.cpp
//**
//** Thomas A. Germer
//** Sensor Science Division, National Institute of Standards and Technology
//** 100 Bureau Dr. Stop 8443; Gaithersburg, MD 20899-8443
//** Phone: (301) 975-2876
//** Email: thomas.germer@nist.gov
//**
//** Version: 4.00 (January 2015) 
//**
//******************************************************************************
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "WinMIST2.h"
#include "WinMIST2Doc.h"
#include "WinTextView.h"
#include "WinTextDoc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "ModelBrowser.h"
#include "ParameterWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_WIZARD, OnWizard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	DragAcceptFiles();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



void CMainFrame::OnWizard() 
{
	// TODO: Add your command handler code here
	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS( ModelBrowser );
	CObject* pObject = pRuntimeClass->CreateObject();
	ASSERT( pObject->IsKindOf( RUNTIME_CLASS( ModelBrowser) ) );


	ModelBrowser* mb = (ModelBrowser*)pObject;
	if (!mb) AfxMessageBox("Failed to create ModelBrowser");
	else {
		//mb->LoadFrame(IDR_MAINFRAME,WS_CHILD|WS_OVERLAPPEDWINDOW|WS_VISIBLE,NULL,NULL);
		//mb->SetWindowText("Model Browser");	
		
		//mb->Create(NULL,"Model Browser",WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,rectDefault,this,NULL);
		mb->LoadFrame(IDR_MODELBROWSER,WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,NULL,NULL);
		mb->SetWindowText("Model Browser");	
	}

}


CMainFrame* pMainFrame;

