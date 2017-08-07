//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: TextSamplesView.cpp
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
// TextSamplesView.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "TextSamplesView.h"
#include "WinMIST2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextSamplesView

IMPLEMENT_DYNCREATE(CTextSamplesView, CEditView)

CTextSamplesView::CTextSamplesView()
{
}

CTextSamplesView::~CTextSamplesView()
{
}


BEGIN_MESSAGE_MAP(CTextSamplesView, CEditView)
	//{{AFX_MSG_MAP(CTextSamplesView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextSamplesView drawing

void CTextSamplesView::OnDraw(CDC* pDC)
{
	CWinMIST2Doc* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTextSamplesView diagnostics

#ifdef _DEBUG
void CTextSamplesView::AssertValid() const
{
	CEditView::AssertValid();
}

void CTextSamplesView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextSamplesView message handlers

void CTextSamplesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWinMIST2Doc* pDoc = GetDocument();
	GetEditCtrl().SetWindowText(pDoc->data.samples.str().c_str());	
	GetEditCtrl().SetReadOnly(TRUE);

}

