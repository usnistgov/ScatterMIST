//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: WinPolarView.h
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
#if !defined(AFX_CWinPolarView_H__3B675879_E16A_4792_B3BC_90A6917FEB25__INCLUDED_)
#define AFX_CWinPolarView_H__3B675879_E16A_4792_B3BC_90A6917FEB25__INCLUDED_

#include "WinMIST2Doc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CWinPolarView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PolarViewOptions dialog

class PolarViewOptions : public CDialog
{
// Construction
public:
	PolarViewOptions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PolarViewOptions)
	enum { IDD = IDD_POLAROPTIONS };
	BOOL	m_InvertScale;
	BOOL	m_ShowIntensity;
	int		m_PointSize;
	BOOL	m_AlwaysAutoScale;
	BOOL	m_FixZero;
	BOOL	m_ShowBRDF;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PolarViewOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PolarViewOptions)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CWinPolarView view

class CWinPolarView : public CView
{
protected:
	CWinPolarView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWinPolarView)

// Attributes
public:
	CWinMIST2Doc* GetDocument() {return (CWinMIST2Doc*)m_pDocument;}

	CDialogBar m_wndDialogBar;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinPolarView)
	public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	static PolarViewOptions options;
	CRect showrect;

// Implementation
protected:
	virtual ~CWinPolarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CWinPolarView)
	afx_msg void OnKillfocusPolarmin();
	afx_msg void OnKillfocusPolarmax();
	afx_msg void OnOptions();
	afx_msg void OnAutoscale();
	afx_msg void OnChangeOrder();
	afx_msg void OnKillfocusOrder();
	afx_msg void OnUpdateOrder();
	afx_msg void OnKillFocusPolarmin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_CWinPolarView_H__3B675879_E16A_4792_B3BC_90A6917FEB25__INCLUDED_)
