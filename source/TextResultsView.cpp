//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: TextResultsView.cpp
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
// TextResultsView.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "TextResultsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextResultsView

IMPLEMENT_DYNCREATE(CTextResultsView, CEditView)

CTextResultsView::CTextResultsView()
{
}

CTextResultsView::~CTextResultsView()
{
}


BEGIN_MESSAGE_MAP(CTextResultsView, CEditView)
	//{{AFX_MSG_MAP(CTextResultsView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextResultsView drawing

void CTextResultsView::OnDraw(CDC* pDC)
{
	CWinMIST2Doc* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTextResultsView diagnostics

#ifdef _DEBUG
void CTextResultsView::AssertValid() const
{
	CEditView::AssertValid();
}

void CTextResultsView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextResultsView message handlers

void CTextResultsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWinMIST2Doc* pDoc = GetDocument();
	GetEditCtrl().SetWindowText(pDoc->data.results.str().c_str());	
	GetEditCtrl().SetReadOnly(TRUE);
}

