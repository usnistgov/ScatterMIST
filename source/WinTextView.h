#if !defined(AFX_WINTEXTVIEW_H__E0AB20B4_84AF_485F_A6AB_5029452CEF1A__INCLUDED_)
#define AFX_WINTEXTVIEW_H__E0AB20B4_84AF_485F_A6AB_5029452CEF1A__INCLUDED_

#include "WinMIST2Doc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WinTextView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWinTextView view

class CWinTextView : public CEditView
{
protected:
	CWinTextView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWinTextView)

// Attributes
public:

	CWinMIST2Doc* GetDocument() {return (CWinMIST2Doc*)m_pDocument;}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinTextView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWinTextView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CWinTextView)
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINTEXTVIEW_H__E0AB20B4_84AF_485F_A6AB_5029452CEF1A__INCLUDED_)
