//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: GraphResultsView.h
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
#if !defined(AFX_GRAPHRESULTSVIEW_H__716124E7_C7F1_493F_95D3_20CAEFF3EBFD__INCLUDED_)
#define AFX_GRAPHRESULTSVIEW_H__716124E7_C7F1_493F_95D3_20CAEFF3EBFD__INCLUDED_

#include "AxisDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphResultsView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphResultsView view

class CGraphResultsView : public CView
{
protected:
	CGraphResultsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGraphResultsView)

// Attributes
public:

// Operations
public:
	CDialogBar m_wndDialogBar;
	void Update();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphResultsView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	CRect showrect;

// Implementation
protected:
	virtual ~CGraphResultsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphResultsView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHRESULTSVIEW_H__716124E7_C7F1_493F_95D3_20CAEFF3EBFD__INCLUDED_)
