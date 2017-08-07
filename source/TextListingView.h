//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: TextListingView.h
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
#if !defined(AFX_TEXTLISTINGVIEW_H__40801A63_562D_4FF9_B2B2_C4BF2C60DC8E__INCLUDED_)
#define AFX_TEXTLISTINGVIEW_H__40801A63_562D_4FF9_B2B2_C4BF2C60DC8E__INCLUDED_

#include "WinMIST2Doc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextListingView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextListingView view

class CTextListingView : public CEditView
{
protected:
	CTextListingView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextListingView)
	
// Attributes
public:
	
	CWinMIST2Doc* GetDocument() {return (CWinMIST2Doc*)m_pDocument;}
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextListingView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTextListingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextListingView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTLISTINGVIEW_H__40801A63_562D_4FF9_B2B2_C4BF2C60DC8E__INCLUDED_)
