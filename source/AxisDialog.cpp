//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: AxisDialog.cpp
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
// AxisDialog.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "AxisDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AxisDialog dialog


AxisDialog::AxisDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AxisDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(AxisDialog)
	//}}AFX_DATA_INIT
}


void AxisDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AxisDialog)
	DDX_Control(pDX, IDC_YLOG, m_ylog);
	DDX_Control(pDX, IDC_XLOG, m_xlog);
	DDX_Control(pDX, IDC_XAxisCombo, m_YAxisControl);
	DDX_Control(pDX, IDC_YAxisCombo, m_XAxisControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AxisDialog, CDialog)
	//{{AFX_MSG_MAP(AxisDialog)
	ON_CBN_SELCHANGE(IDC_XAxisCombo, OnSelchangeXAxisCombo)
	ON_CBN_SELCHANGE(IDC_YAxisCombo, OnSelchangeYAxisCombo)
	ON_BN_CLICKED(IDC_XLOG, OnXlog)
	ON_BN_CLICKED(IDC_YLOG, OnYlog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AxisDialog message handlers

void AxisDialog::OnSelchangeXAxisCombo() 
{
}

void AxisDialog::OnSelchangeYAxisCombo() 
{
}

void AxisDialog::OnXlog() 
{
}

void AxisDialog::OnYlog() 
{
}
