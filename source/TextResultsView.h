//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: TextResultsView.h
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
#if !defined(AFX_TEXTRESULTSVIEW_H__E5007744_F2E7_46A9_9353_B37C10A13E85__INCLUDED_)
#define AFX_TEXTRESULTSVIEW_H__E5007744_F2E7_46A9_9353_B37C10A13E85__INCLUDED_

#include "WinMIST2Doc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextResultsView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextResultsView view

class CTextResultsView : public CEditView
{
protected:
	CTextResultsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextResultsView)

// Attributes
public:

// Operations
public:
	CWinMIST2Doc* GetDocument() {return (CWinMIST2Doc*)m_pDocument;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextResultsView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTextResultsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextResultsView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTRESULTSVIEW_H__E5007744_F2E7_46A9_9353_B37C10A13E85__INCLUDED_)
