//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: ChildFrm.h
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
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "WinMIST2.h"

#include "ChildFrm.h"
#include "WinPolarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_CBN_SELCHANGE(IDC_XAxisCombo, OnSelchangeXAxisCombo)
	ON_BN_CLICKED(IDC_XLOG, OnXlog)
	ON_CBN_SELCHANGE(IDC_YAxisCombo, OnSelchangeYAxisCombo)
	ON_BN_CLICKED(IDC_YLOG, OnYlog)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, OnReleasedcaptureSlider1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnSelchangeXAxisCombo() 
{
	// TODO: Add your control notification handler code here
	GetActiveDocument()->UpdateAllViews(NULL,0L,NULL);
}

void CChildFrame::OnXlog() 
{
	// TODO: Add your control notification handler code here
	GetActiveDocument()->UpdateAllViews(NULL,0L,NULL);
	
}

void CChildFrame::OnSelchangeYAxisCombo() 
{
	// TODO: Add your control notification handler code here
	GetActiveDocument()->UpdateAllViews(NULL,0L,NULL);

}

void CChildFrame::OnYlog() 
{
	// TODO: Add your control notification handler code here
	GetActiveDocument()->UpdateAllViews(NULL,0L,NULL);

}

void CChildFrame::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	GetActiveDocument()->UpdateAllViews(NULL,0L,NULL);
	*pResult = 0;
}

void CChildFrame::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CWinPolarView* view = (CWinPolarView*)GetActiveView();
	CSliderCtrl* slider1 = (CSliderCtrl*)(view->m_wndDialogBar.GetDlgItem(IDC_SLIDER1));
	slider1->SetPos(0);
	CSliderCtrl* slider2 = (CSliderCtrl*)(view->m_wndDialogBar.GetDlgItem(IDC_SLIDER2));
	slider2->SetPos(0);
	GetActiveDocument()->UpdateAllViews(NULL,0L,NULL);
	
}
