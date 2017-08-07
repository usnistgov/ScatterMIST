// WinTextDoc.cpp : implementation file
//

#include "stdafx.h"
#include "WinMIST2.h"
#include "WinTextDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinTextDoc

IMPLEMENT_DYNCREATE(CWinTextDoc, CDocument)

CWinTextDoc::CWinTextDoc()
{
}

BOOL CWinTextDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CWinTextDoc::~CWinTextDoc()
{
}


BEGIN_MESSAGE_MAP(CWinTextDoc, CDocument)
	//{{AFX_MSG_MAP(CWinTextDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinTextDoc diagnostics

#ifdef _DEBUG
void CWinTextDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinTextDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinTextDoc serialization

void CWinTextDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CWinTextDoc commands

