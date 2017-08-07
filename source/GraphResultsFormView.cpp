// GraphResultsFormView.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "GraphResultsFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphResultsFormView

IMPLEMENT_DYNCREATE(CGraphResultsFormView, CFormView)

CGraphResultsFormView::CGraphResultsFormView()
	: CFormView(CGraphResultsFormView::IDD)
{
	//{{AFX_DATA_INIT(CGraphResultsFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGraphResultsFormView::~CGraphResultsFormView()
{
}

void CGraphResultsFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraphResultsFormView)
	DDX_Control(pDX, IDC_COMBO2, m_YAxis);
	DDX_Control(pDX, IDC_COMBO1, m_XAxis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraphResultsFormView, CFormView)
	//{{AFX_MSG_MAP(CGraphResultsFormView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphResultsFormView diagnostics

#ifdef _DEBUG
void CGraphResultsFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGraphResultsFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphResultsFormView message handlers


void CGraphResultsFormView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
}
