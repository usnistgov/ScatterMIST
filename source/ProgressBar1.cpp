// ProgressBar1.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "ProgressBar1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ProgressBar dialog


ProgressBar::ProgressBar(CWnd* pParent /*=NULL*/)
	: CDialog(ProgressBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(ProgressBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ProgressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ProgressBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ProgressBar, CDialog)
	//{{AFX_MSG_MAP(ProgressBar)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ProgressBar message handlers
