#if !defined(AFX_MODELEXPLORER_H__B33BFABA_3EE3_443E_90BE_A8FC6DC51A23__INCLUDED_)
#define AFX_MODELEXPLORER_H__B33BFABA_3EE3_443E_90BE_A8FC6DC51A23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelExplorer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ModelExplorer frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class ModelExplorer : public CMDIChildWnd
{
	DECLARE_DYNCREATE(ModelExplorer)
protected:
	ModelExplorer();           // protected constructor used by dynamic creation

// Attributes
protected:
	CSplitterWnd    m_wndSplitter;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModelExplorer)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ModelExplorer();

	// Generated message map functions
	//{{AFX_MSG(ModelExplorer)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELEXPLORER_H__B33BFABA_3EE3_443E_90BE_A8FC6DC51A23__INCLUDED_)
