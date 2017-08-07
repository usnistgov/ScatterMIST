//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: ViewRawMIST.cpp
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
// ViewRawMIST.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "ViewRawMIST.h"
#include "WinMIST2Doc.h"
#include "WinMIST2View.h"
#include "WinMIST2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ViewRawMIST dialog


ViewRawMIST::ViewRawMIST(CWnd* pParent /*=NULL*/)
	: CDialog(ViewRawMIST::IDD, pParent)
{
	//{{AFX_DATA_INIT(ViewRawMIST)
	m_Text = _T("");
	//}}AFX_DATA_INIT
}


void ViewRawMIST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ViewRawMIST)
	DDX_Text(pDX, IDC_TEXT, m_Text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ViewRawMIST, CDialog)
	//{{AFX_MSG_MAP(ViewRawMIST)
	ON_BN_CLICKED(IDC_CREATERAWMIST, OnCreateRawMISTDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ViewRawMIST message handlers

void ViewRawMIST::OnCreateRawMISTDoc() 
{
	// TODO: Add your control notification handler code here
	CWinMIST2Doc *pDoc = (CWinMIST2Doc *)GetApp()->pWinMIST2DocTemplate->CreateNewDocument();
	CFrameWnd *frame = GetApp()->pWinMIST2DocTemplate->CreateNewFrame(pDoc,NULL);
	GetApp()->pWinMIST2DocTemplate->InitialUpdateFrame(frame,pDoc, TRUE );

	CWinMIST2View* view;
	view = (CWinMIST2View*)pDoc->GetTopView();
	view->SetWindowText(m_Text);
	
	EndDialog(IDOK);
}
