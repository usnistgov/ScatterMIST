// NullDoc.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "NullDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNullDoc

IMPLEMENT_DYNCREATE(CNullDoc, CDocument)

CNullDoc::CNullDoc()
{
}

BOOL CNullDoc::OnNewDocument()
{
	//return FALSE;
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CNullDoc::~CNullDoc()
{
}


BEGIN_MESSAGE_MAP(CNullDoc, CDocument)
	//{{AFX_MSG_MAP(CNullDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNullDoc diagnostics

#ifdef _DEBUG
void CNullDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNullDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNullDoc serialization

void CNullDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNullDoc commands
