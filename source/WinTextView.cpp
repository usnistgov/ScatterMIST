// WinTextView.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "WinTextView.h"
#include "WinMIST2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinTextView

IMPLEMENT_DYNCREATE(CWinTextView, CEditView)

CWinTextView::CWinTextView()
{
}

CWinTextView::~CWinTextView()
{
}


BEGIN_MESSAGE_MAP(CWinTextView, CEditView)
	//{{AFX_MSG_MAP(CWinTextView)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinTextView drawing

void CWinTextView::OnDraw(CDC* pDC)
{
	CWinMIST2Doc* pDoc = (CWinMIST2Doc*)GetDocument();
	// TODO: add draw code here
	//GetEditCtrl().SetWindowText(pDoc->results.c_str());
}

/////////////////////////////////////////////////////////////////////////////
// CWinTextView diagnostics

#ifdef _DEBUG
void CWinTextView::AssertValid() const
{
	CEditView::AssertValid();
}

void CWinTextView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinTextView message handlers

void CWinTextView::OnFileNew() 
{
	// TODO: Add your command handler code here
	
}

