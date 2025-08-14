//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: ProgressBar.cpp
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
// ProgressBar1.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "ProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ProgressBar dialog


ProgressBar::ProgressBar(CWnd* pParent /*=NULL*/)
	: CDialog(ProgressBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(ProgressBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_stop=false;
	//this->SetFocus();
}


void ProgressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ProgressBar)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	//}}AFX_DATA_MAP
	this->SetFocus();
}


BEGIN_MESSAGE_MAP(ProgressBar, CDialog)
	//{{AFX_MSG_MAP(ProgressBar)
	ON_COMMAND(IDCANCEL, &ProgressBar::OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ProgressBar message handlers

void ProgressBar::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_stop=true;
	//DestroyWindow();
	//CDialog::OnCancel();
}

void ProgressBar::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}
