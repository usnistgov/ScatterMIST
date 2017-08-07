// ModelExplorer.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "ModelExplorer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ModelExplorer

IMPLEMENT_DYNCREATE(ModelExplorer, CMDIChildWnd)

ModelExplorer::ModelExplorer()
{
}

ModelExplorer::~ModelExplorer()
{
}

BOOL ModelExplorer::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	return m_wndSplitter.Create(this,
		2, 1,       // TODO: adjust the number of rows, columns
		CSize(10, 10),  // TODO: adjust the minimum pane size
		pContext);
}


BEGIN_MESSAGE_MAP(ModelExplorer, CMDIChildWnd)
	//{{AFX_MSG_MAP(ModelExplorer)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ModelExplorer message handlers
