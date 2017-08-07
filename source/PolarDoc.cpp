// CWinPolarDoc.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "PolarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinPolarDoc

IMPLEMENT_DYNCREATE(CWinPolarDoc, CDocument)

CWinPolarDoc::CWinPolarDoc()
{
}

BOOL CWinPolarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CWinPolarDoc::~CWinPolarDoc()
{
}


BEGIN_MESSAGE_MAP(CWinPolarDoc, CDocument)
	//{{AFX_MSG_MAP(CWinPolarDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinPolarDoc diagnostics

#ifdef _DEBUG
void CWinPolarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinPolarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinPolarDoc serialization

void CWinPolarDoc::Serialize(CArchive& ar)
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
// CWinPolarDoc commands

#include <sstream>

void CWinPolarDoc::LoadData(const std::string& string)
{
	using namespace std;
	istringstream strm(string);
	char c[256];
	strm.getline(c,255);
	while (!strm.eof()) {
		double t,p,_x,_y,_z,b;
		strm >> t >> p >> _x >> _y >> _z >> b;
		if (!strm.eof()) {
			x.push_back(_x);
			y.push_back(_y);
			z.push_back(_z);

		}
	}
}
