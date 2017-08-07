//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: WinMIST2View.h
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
// WinMIST2View.h : interface of the CWinMIST2View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINMIST2VIEW_H__FCE1A054_FA52_4A54_B69F_87A11183FBED__INCLUDED_)
#define AFX_WINMIST2VIEW_H__FCE1A054_FA52_4A54_B69F_87A11183FBED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "childfrm.h"

class CWinMIST2View : public CEditView
{
protected: // create from serialization only
	CWinMIST2View();
	DECLARE_DYNCREATE(CWinMIST2View)

// Attributes
public:
	CWinMIST2Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinMIST2View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWinMIST2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWinMIST2View)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in WinMIST2View.cpp
inline CWinMIST2Doc* CWinMIST2View::GetDocument()
   { return (CWinMIST2Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINMIST2VIEW_H__FCE1A054_FA52_4A54_B69F_87A11183FBED__INCLUDED_)
