//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: TextListingView.cpp
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
// TextListingView.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "TextListingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextListingView

IMPLEMENT_DYNCREATE(CTextListingView, CEditView)

CTextListingView::CTextListingView()
{
}

CTextListingView::~CTextListingView()
{
}


BEGIN_MESSAGE_MAP(CTextListingView, CEditView)
	//{{AFX_MSG_MAP(CTextListingView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextListingView drawing

void CTextListingView::OnDraw(CDC* pDC)
{
	CWinMIST2Doc* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTextListingView diagnostics

#ifdef _DEBUG
void CTextListingView::AssertValid() const
{
	CEditView::AssertValid();
}

void CTextListingView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextListingView message handlers
#include <fstream>

void CTextListingView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWinMIST2Doc* pDoc = GetDocument();
	GetEditCtrl().SetWindowText(pDoc->data.listing.str().c_str());
	GetEditCtrl().SetReadOnly(TRUE);
}


BOOL CTextListingView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
